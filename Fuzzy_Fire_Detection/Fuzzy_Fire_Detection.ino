#include "Fuzzy_system.h"

// Device states
#define START                   0
#define NORMAL                  1
#define STANDBY                 2
#define ALERT                   3

// Fire threshholds
#define STANDBY_THRESHHOLD      40.0
#define ALERT_THRESHHOLD        60.0

// Sleep times
#define NORMAL_SLEEP            60000000  // 1 minute   (uSeconds)
#define STANDBY_SLEEP           10000000  // 10 seconds 
#define ALERT_SLEEP             5000000   // 5 seconds 

#define VERBOSE                 true

//
void off_unnecessary();
void init_sensors();
float* read_sensors();

void setup(){
  #if VERBOSE
    // Set the Serial output
    Serial.begin(115200);
  #endif
}

byte device_state = START;

float prev_t = 0;
float prev_s = 0;

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
      ESP.deepSleep(NORMAL_SLEEP);
      break;
    }
    case STANDBY:{
      ESP.deepSleep(STANDBY_SLEEP);
      break;
    }
    case ALERT:{
      ESP.deepSleep(ALERT_SLEEP);
      break;
    }
    default: break;
  }
  /*
  // Acquire inputs from sensors/random/testing-dataset
  
  // get random entrances
  float input_t = random(0, 100);
  float input_dt = input_t - prev_t;
  float input_s = random(-1, 20);
  float input_ds = input_s - prev_s;
  prev_t = input_t;
  prev_s = input_s;

  //Set fuzzy-system's inputs
  
  set_fuzzy_inputs(input_t, input_s, input_dt, input_ds);

  // Fuzzify inputs
  
  fuzzify_system();

  // Defuzzify and get final result
  
  float output1 = defuzzify_system();

  // Viz
  #if VERBOSE

    Serial.printf("Inputs:\nT: %f, S: %f, DT: %f, DS: %f\n", input_t, input_s, input_dt, input_ds);
    
    fire_conf fc = get_fire_conf();
  
    Serial.printf("Output: \nFire Confidence: Low-> %f, Med-> %f, High-> %f\n", fc.fire_low, fc.fire_med, fc.fire_high);
    Serial.printf("Result:\nFire Confidence: %f\n", output1);
    Serial.println("########################");
  
  #endif
  
  delay(2000); // just for ease of reading
  */
}

void off_unnecessary(){
  ;
}
void init_sensors(){
  ;
}
float* read_sensors(){
  ;
}
