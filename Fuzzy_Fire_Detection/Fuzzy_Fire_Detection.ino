#include "Fuzzy_system.h"
#include<ESP8266WiFi.h>

// Device states
#define START                   0
#define NORMAL                  1
#define STANDBY                 2
#define ALERT                   3

// Fire threshholds
#define STANDBY_THRESHHOLD      40.0
#define ALERT_THRESHHOLD        60.0

// Sleep times
#define NORMAL_SLEEP            60000000UL  // 1 minute   (uSeconds)
#define STANDBY_SLEEP           10000000UL  // 10 seconds 
#define ALERT_SLEEP             5000000UL   // 5 seconds 

// WiFi data
#define WLAN_SSID               ""
#define WLAN_PASSWD             ""

// Message types
#define DAILY_MSG               0
#define STANDBY_MSG             1
#define ALERT_MSG               2

// Turn ON/OFF debug printing (debugging section not included in compiled code when VERBOSE is false) 
#define VERBOSE                 false

// Functions' prototypes
void off_unnecessary();
void wake_wifi_up();
void init_sensors();
float* read_sensors();
void communicate_(byte);


byte device_state = START;

float prev_t = 0;
float prev_s = 0;

void setup(){
  #if VERBOSE
    // Set the Serial output
    Serial.begin(115200);
  #endif
}

void loop() {
  switch(device_state){
    
    case START:{
      
      off_unnecessary(); // Turn of Wi-Fi and Bluetooth
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

      communicate_(DAILY_MSG);
      // WAKE_RF_DISABLED to keep the WiFi radio disabled when we wake up
      ESP.deepSleep(NORMAL_SLEEP, WAKE_RF_DISABLED);
      break;
    }
    
    case STANDBY:{

      communicate_(STANDBY_MSG);
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

void init_sensors(){
  ;
}

float* read_sensors(){
  ;
}

void off_unnecessary(){
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  yield();
}

void wake_wifi_up(){
  
  WiFi.forceSleepWake();
  yield();
  
  // Disable the WiFi persistence. ESP8266 will not load and save WiFi settings in the flash memory.
  WiFi.persistent(false);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASSWD);
  
  /*
  IPAddress staticIP(192,168,1,22);
  IPAddress gateway(192,168,1,9);
  IPAddress subnet(255,255,255,0);
    
  // Make a static IP address to DHCP IP request time
  WiFi.config(staticIP, gateway, subnet);
  */
}

void communicate_(byte msg_type){
  wake_wifi_up();
  switch(msg_type){
    case DAILY_MSG:{
      break;
    }
    case STANDBY_MSG:{
      break;
    }
    case ALERT_MSG:{
      break;
    }
  }
}
