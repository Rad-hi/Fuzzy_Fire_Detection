/*
 * Main file for the Fuzzy_Fire_Detection IoT node.
 * ---
 * This file requires a collection of header files to run properly.
 * Header files: {Fuzzy_system.h(Fuzzy_system.cpp),
 *                WIFI.h(WIFI.cpp), 
 *                MQTT.h(MQTT.cpp)}
 * ---
 * Written by: Radhi SGHAIER, https://github.com/Rad-hi 
 */

#include"Fuzzy_system.h"
#include"WIFI.h"

#include <ArduinoJson.h>

// Turn ON/OFF debug printing (debugging section won't be included in compiled code when VERBOSE is false) 
#define VERBOSE                 false

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

// RTC Memory locations
#define PATTERN_H_RTC_LOC       0
#define PATTERN_L_RTC_LOC       1
#define PREV_T_RTC_LOC          2
#define PREV_S_RTC_LOC          3
#define WAKE_COUNTER            4

// 
#define SEC_IN_DAY              86400

// Message types
#define DAILY_MSG               0
#define STANDBY_MSG             1
#define ALERT_MSG               2

// Message buffer
#define JSON_BUFFER_SIZE        128         // Calculate the correct size using:
                                            // https://arduinojson.org/v5/assistant/
                                             
StaticJsonDocument<JSON_BUFFER_SIZE> DATA;  // Json file that'll contain all data, and then be sent via mqtt

// Functions' prototypes
void init_sensors();
float* read_sensors();
void communicate_(byte);
void check_rtc_mem_validity();
void check_for_daily_report(byte);

// Global variables
byte device_state = START;
uint32_t wake_counter;

void setup(){
  off_unnecessary(); // Turn off Wi-Fi
  check_rtc_mem_validity();
  
  #if VERBOSE
    // Set the Serial output
    Serial.begin(SERIAL_MON_BAUD_RATE);
  #endif
}

void loop() {
  switch(device_state){
    
    case START:{
      init_sensors();    // Initialize sensors
      setup_fuzzy();     // Initialize the fuzzy system
      float* fuzzy_inputs = read_sensors();
      set_fuzzy_inputs(*fuzzy_inputs, *(fuzzy_inputs + 1), *(fuzzy_inputs + 2), *(fuzzy_inputs + 3)); //Set fuzzy-system's inputs
      fuzzify_system();  // Fuzzify inputs
      float fire_percentage = defuzzify_system(); // Defuzzify and get final result

      // Viz
      #if VERBOSE
        Serial.printf("Inputs:\nT: %f, S: %f, DT: %f, DS: %f\n", *fuzzy_inputs, *(fuzzy_inputs + 1), *(fuzzy_inputs + 2), *(fuzzy_inputs + 3));
        fire_conf fc = get_fire_conf();
        Serial.printf("Output: \nFire Confidence: Low-> %f, Med-> %f, High-> %f\n", fc.fire_low, fc.fire_med, fc.fire_high);
        Serial.printf("Result:\nFire Confidence: %f\n", fire_percentage);
        Serial.println("########################");
      #endif

      // Decide the next action
      if(fire_percentage - ALERT_THRESHHOLD){ // fire_percentage > 60.0
        device_state = ALERT;
        break;
      }
      else if(fire_percentage - STANDBY_THRESHHOLD){ // fire_percentage > 40.0 && fire_percentage < 60.0
        device_state = STANDBY;
        break;
      }
      else device_state = NORMAL; // fire_percentage < 40.0
      break;
    }
    
    case NORMAL:{

      check_for_daily_report(60);
      // WAKE_RF_DISABLED to keep the WiFi radio disabled when we wake up
      ESP.deepSleep(NORMAL_SLEEP, WAKE_RF_DISABLED);
      break;
    }
    
    case STANDBY:{

      check_for_daily_report(10);
      // WAKE_RF_DISABLED to keep the WiFi radio disabled when we wake up
      ESP.deepSleep(STANDBY_SLEEP, WAKE_RF_DISABLED);
      break;
    }
    
    case ALERT:{

      communicate_(ALERT_MSG);
      // WAKE_RF_DISABLED to keep the WiFi radio disabled when we wake up
      ESP.deepSleep(ALERT_SLEEP, WAKE_RF_DISABLED);
      break;
    }
    
    default: break;
  }
}

void check_for_daily_report(byte period){
  // Read how many seconds we've been asleep for, since the last 24h-report
  ESP.rtcUserMemoryRead(WAKE_COUNTER, &wake_counter, sizeof(wake_counter));
  if(wake_counter == SEC_IN_DAY){
    switch(period){
      case 60: communicate_(DAILY_MSG); break;
      case 10: communicate_(STANDBY_MSG); break;
      default: break;
    }
    wake_counter = 0;
  }
  else wake_counter += period;
  ESP.rtcUserMemoryWrite(WAKE_COUNTER, &wake_counter, sizeof(wake_counter));
}

void check_rtc_mem_validity(){
  // At boot, the RTC memory will contain random values, so we need a way to check
  // If the data we're reading is valid on not (edited by us, or random), so to do that 
  // we read the 8 first bytes, and these need to be exactly 66669420 (any specific pattern)

  uint32_t p_H, p_L, reset_var = 0;
  ESP.rtcUserMemoryRead(PATTERN_H_RTC_LOC, &p_H, sizeof(p_H));
  ESP.rtcUserMemoryRead(PATTERN_L_RTC_LOC, &p_L, sizeof(p_L));
  if(p_H != 0x6666 && p_L != 0x9420){ // Pattern is invalid

    // Update the pattern to indicate that we indeed modified it
    p_H = 0x6666; 
    p_L = 0x9420;
    ESP.rtcUserMemoryWrite(PATTERN_H_RTC_LOC, &p_H, sizeof(p_H));
    ESP.rtcUserMemoryWrite(PATTERN_L_RTC_LOC, &p_L, sizeof(p_L));

    // Reset all RTC memory savings that we care about their value to 0
    ESP.rtcUserMemoryWrite(PREV_T_RTC_LOC, &reset_var, sizeof(reset_var));
    ESP.rtcUserMemoryWrite(PREV_S_RTC_LOC, &reset_var, sizeof(reset_var));
    ESP.rtcUserMemoryWrite(WAKE_COUNTER, &reset_var, sizeof(reset_var));    
  }
}

void init_sensors(){
  ;
}

float* read_sensors(){
  uint32_t prev_t = 0, prev_s = 0, input_t, input_s;
    
  // Read the sensor values from the two sensors
  /*
   * INSERT CODE HERE
   */
  
  // Read previous values of the Temp and Smoke variables from RTC memory
  ESP.rtcUserMemoryRead(PREV_T_RTC_LOC, &prev_t, sizeof(prev_t));
  ESP.rtcUserMemoryRead(PREV_S_RTC_LOC, &prev_s, sizeof(prev_s));

  // Update the previous values of the Temp and Smoke varibales in RTC memory
  ESP.rtcUserMemoryWrite(PREV_T_RTC_LOC, &input_t, sizeof(input_t));
  ESP.rtcUserMemoryWrite(PREV_S_RTC_LOC, &input_s, sizeof(input_s));
  
  // Temp, Smoke, Delta_Temp, Delta_Smoke
  float output[4] = {input_t, input_s, input_t - prev_t, input_s - prev_s};
  return output;
}


void communicate_(byte msg_type){
  MQTT_setup();
  wake_wifi_up();
  char buffer[JSON_BUFFER_SIZE];
  
  switch(msg_type){
    case DAILY_MSG:{    // Fill daily msg data
      DATA["MSG"] = "N"; // Message tag contains N (Normal) indicating the node is still alive   
      break;
    }
    case STANDBY_MSG:{  // Fill satndby msg data
      DATA["MSG"] = "S"; // Message tag contains S (Standby) indicating that more attention is required
      break;
    }
    case ALERT_MSG:{    // Fill alert data
      DATA["MSG"] = "A"; // Message tag contains A (Alert) indicating a fire alert
      break;
    }
    default: break;
  }

  // Serialize data, connect, and send.
  serializeJson(DATA, buffer);
  MQTT_connect();
  send_data(buffer);
  
  #if VERBOSE
    Serial.print("Sent: ");
    Serial.println(buffer);
  #endif
 
}
