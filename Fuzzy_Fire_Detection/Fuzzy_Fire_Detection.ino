/*
 * Main file for the Fuzzy_Fire_Detection IoT node.
 * ---
 * This file requires a collection of header files to run properly.
 * Header files: {Fuzzy_system.h(Fuzzy_system.cpp),
 *                WIFI.h(WIFI.cpp), 
 *                MQTT.h(MQTT.cpp),
 *                Data_Logger.h(Data_Logger.cpp)}
 * ---
 * Written by: Radhi SGHAIER, https://github.com/Rad-hi 
 */

#include"Data_Logger.h"
#include"Fuzzy_system.h"
#include"WIFI.h"

// Turn ON/OFF debug printing (debugging section won't be included in compiled code when VERBOSE is false) 
#define VERBOSE                 true

// If this is true, two random numbers in the appropriate range will simulate the Temp & Smoke inputs
#define SIMULATE_RANDOM         true

// 115.2K baud serial connection to computer
#define SERIAL_MON_BAUD_RATE    115200

// Device states
#define START                   0
#define NORMAL                  1
#define STANDBY                 2
#define ALERT                   3

// Fire threshholds
#define STANDBY_THRESHHOLD      40.0
#define ALERT_THRESHHOLD        60.0

// Sleep times
#define NORMAL_SLEEP            60000000UL  // 1 minute   (uSeconds) (60e6 works too)
#define STANDBY_SLEEP           10000000UL  // 10 seconds 
#define ALERT_SLEEP             5000000UL   // 5 seconds 

// Period counters
#define NORMAL_COUNTER          60
#define STANDBY_COUNTER         10

// RTC Memory locations (It's divided into chunks of 4 bytes)
#define PATTERN_RTC_LOC         0
#define PREV_T_RTC_LOC          4
#define PREV_S_RTC_LOC          8
#define WAKE_COUNTER_RTC_LOC    12
#define HOUR_COUNTER_RTC_LOC    16

// 
#define SEC_IN_DAY              86400
#define SEC_IN_HOUR             3600

// Message types
#define DAILY_MSG               0
#define STANDBY_MSG             1
#define ALERT_MSG               2

// Functions' prototypes
void init_sensors();
float read_sensors();
void communicate_(byte);
void check_rtc_mem_validity();
void check_for_daily_report(byte);
void go_to_sleep(unsigned long);

// Global variables
byte device_state = START;
byte end_hour = 0;
uint32_t wake_counter; // This is required to be uint32_t by the RTC memory
                       // even though we don't need a 32 bits interger
uint32_t hour_counter; 
float* fuzzy_inputs;

void setup(){
  // Turn off Wi-Fi
  off_unnecessary(); 
  
  // Mount the file-system
  LittleFS.begin();
  
  #if VERBOSE
    Serial.begin(SERIAL_MON_BAUD_RATE);
  #endif

  // Sim
  #if SIMULATE_RANDOM
    randomSeed(analogRead(0)); 
  #endif
}

void loop() {
  switch(device_state){
    
    case START:{
      check_rtc_mem_validity();
      init_sensors();    
      setup_fuzzy();     
      read_sensors(&fuzzy_inputs);
      set_fuzzy_inputs(*fuzzy_inputs, *(fuzzy_inputs + 1), *(fuzzy_inputs + 2), *(fuzzy_inputs + 3)); //Set fuzzy-system's inputs
      fuzzify_system();  
      float fire_percentage = defuzzify_system();
      
      // Viz
      #if VERBOSE
        Serial.printf("\nInputs:\nT: %f, S: %f, DT: %f, DS: %f\n", *fuzzy_inputs, *(fuzzy_inputs + 1), *(fuzzy_inputs + 2), *(fuzzy_inputs + 3));
        fire_conf fc = get_fire_conf();
        Serial.printf("Output: \nFire Confidence: Low-> %f, Med-> %f, High-> %f\n", fc.fire_low, fc.fire_med, fc.fire_high);
        Serial.printf("Result:\nFire Confidence: %f\n", fire_percentage);
      #endif
      
      // Decide the next action
      if(fire_percentage - ALERT_THRESHHOLD > 0){ // fire_percentage > 60.0
        device_state = ALERT;
        break;
      }
      else if(fire_percentage - STANDBY_THRESHHOLD > 0){ // fire_percentage > 40.0 && fire_percentage < 60.0
        device_state = STANDBY;
        break;
      }
      else device_state = NORMAL; // fire_percentage < 40.0
      break;
    }
    
    case NORMAL:{
      check_for_daily_report(NORMAL_COUNTER);
      // Write temperature value to the LOG
      write_temp(*fuzzy_inputs, end_hour);
      go_to_sleep(NORMAL_SLEEP);
      break;
    }
    
    case STANDBY:{
      communicate_(STANDBY_MSG);
      check_for_daily_report(STANDBY_COUNTER);
      // Write temperature value to the LOG
      write_temp(*fuzzy_inputs, end_hour);
      go_to_sleep(STANDBY_SLEEP);
      break;
    }
    
    case ALERT:{
      communicate_(ALERT_MSG);
      go_to_sleep(ALERT_SLEEP);
      break;
    }
    default: break;
  }
}

void go_to_sleep(unsigned long time_to_sleep){

  free(fuzzy_inputs);

  // Unmount the file-system
  LittleFS.end();
  
  // Off WiFi
  off_unnecessary();
  
  #if VERBOSE
    Serial.println("Going to sleep...");
  #endif
    
  // WAKE_RF_DISABLED to keep the WiFi radio disabled when we wake up
  ESP.deepSleepInstant(time_to_sleep, WAKE_RF_DISABLED);
}

void check_for_daily_report(byte period){
  // Read how many seconds we've been asleep for, since the last 24h-report
  ESP.rtcUserMemoryRead(WAKE_COUNTER_RTC_LOC, &wake_counter, sizeof(wake_counter));
  // Read which hour of the day we're in
  ESP.rtcUserMemoryRead(HOUR_COUNTER_RTC_LOC, &hour_counter, sizeof(hour_counter));

  // Viz
  #if VERBOSE
    Serial.printf("Wake counter: %d\n", wake_counter);
    Serial.printf("Hour n°%d of the day\n", hour_counter);
  #endif

  // If we've finished an hour, we need this flag for when we write the temp to the LOG
  // (checking with a tolerance of 1 minute)
  if((wake_counter - (hour_counter+1)*SEC_IN_HOUR >= 0) 
      && (wake_counter - (hour_counter+1)*SEC_IN_HOUR <= 60)){
        // hour_counter goes from 0 to 23
        if(++hour_counter == 24)hour_counter = 0;
        end_hour = 1;
      }

  // It's been a day already
  if(wake_counter >= SEC_IN_DAY){
    communicate_(DAILY_MSG);
    wake_counter = 0;
  }
  else wake_counter += period;
  // Update the values in the RTC memory
  ESP.rtcUserMemoryWrite(HOUR_COUNTER_RTC_LOC, &hour_counter, sizeof(hour_counter));
  ESP.rtcUserMemoryWrite(WAKE_COUNTER_RTC_LOC, &wake_counter, sizeof(wake_counter));
}

void check_rtc_mem_validity(){
  // At boot, the RTC memory will contain random values, so we need a way to check
  // If the data we're reading is valid on not (edited by us, or random), so to do that 
  // we read the 4 first bytes, and these need to be exactly 0x66669420 (any specific pattern)
  
  uint32_t pattern = 0, reset_var = 0;
  ESP.rtcUserMemoryRead(PATTERN_RTC_LOC, &pattern, sizeof(pattern));

  // Viz
  #if VERBOSE
    Serial.printf("\nRTC Pattern: %d, Ours: %d", pattern, 0x66669420);
  #endif
  
  if(pattern != 0x66669420){ // Pattern is invalid

    // Viz
    #if VERBOSE
      Serial.println("Resetting RTC patterns");
    #endif
    
    // Update the pattern to indicate that we indeed modified it
    pattern = 0x66669420; 
    ESP.rtcUserMemoryWrite(PATTERN_RTC_LOC, &pattern, sizeof(pattern));

    // Reset all RTC memory savings that we care about their value to 0
    ESP.rtcUserMemoryWrite(PREV_T_RTC_LOC, &reset_var, sizeof(reset_var));
    ESP.rtcUserMemoryWrite(PREV_S_RTC_LOC, &reset_var, sizeof(reset_var));
    ESP.rtcUserMemoryWrite(WAKE_COUNTER_RTC_LOC, &reset_var, sizeof(reset_var)); 
    ESP.rtcUserMemoryWrite(HOUR_COUNTER_RTC_LOC, &reset_var, sizeof(reset_var));   
  }
}

void init_sensors(){
  ;
}

float read_sensors(float **output){
  uint32_t prev_t = 0, prev_s = 0, input_t, input_s;

  #if SIMULATE_RANDOM // Simulate sensor's readings with a random number generator
    input_t = random(0, 80);
    input_s = random(0, 25);
  #else // Read the sensor values from the two sensors
    /*
     * INSERT CODE HERE
     */
  #endif
  
  // Read previous values of the Temp and Smoke variables from RTC memory
  ESP.rtcUserMemoryRead(PREV_T_RTC_LOC, &prev_t, sizeof(prev_t));
  ESP.rtcUserMemoryRead(PREV_S_RTC_LOC, &prev_s, sizeof(prev_s));

  // Allocate memory for the fuzzy input data
  *output = (float*)malloc(4*sizeof(float));
  (*output)[0] = (float)input_t;
  (*output)[1] = (float)input_s;
  (*output)[2] = (float)input_t - (float)prev_t;
  (*output)[3] = (float)input_s - (float)prev_s;

  // Update the previous values of the Temp and Smoke varibales in RTC memory
  ESP.rtcUserMemoryWrite(PREV_T_RTC_LOC, &input_t, sizeof(input_t));
  ESP.rtcUserMemoryWrite(PREV_S_RTC_LOC, &input_s, sizeof(input_s));
}


void communicate_(byte msg_type){

  char* buffer = (char*)malloc(sizeof(char)*JSON_BUFFER_SIZE); // Allocate memory for the buffer
  switch(msg_type){
    case DAILY_MSG:{    // Fill daily msg data
      read_day(buffer);
      break;
    }
    case STANDBY_MSG:{  // Fill satndby msg data
      strcpy(buffer, "{\"Msg\":\"Standby\"}"); // Message tag contains "Standby" indicating that more attention is required
      break;
    }
    case ALERT_MSG:{    // Fill alert data
      strcpy(buffer, "{\"Msg\":\"Alert\"}");// Message tag contains"Alert" indicating a fire alert
      break;
    }
    default: break;
  }

  MQTT_setup();

  // Viz
  #if VERBOSE 
    Serial.println("Connecting to WiFi...");
  #endif
  
  wake_wifi_up();

  // Viz
  #if VERBOSE
    Serial.println("Connecting to MQTT broker...");
  #endif
  
  MQTT_connect();
  send_data(buffer);
  
  // Viz
  #if VERBOSE
    Serial.print("Sent: ");
    Serial.println(buffer);
  #endif

  // Garbage collection
  free(buffer); 
}
