#include "Fuzzy_system.h"

#define VERBOSE       true

void setup(){

  #if VERBOSE
    // Set the Serial output
    Serial.begin(115200);
  #endif
  
  // Set a random seed
  randomSeed(analogRead(0));
}

float prev_t = 0;
float prev_s = 0;

void loop() {

  // Acquire inputs from sensors/random/testing-dataset
  
  // get random entrances
  float input_t = random(0, 100);
  float input_s = random(-0.001, 0.02);
  float input_dt = input_t - prev_t;
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

    Serial.printf("Inputs:\nT: %f,S: %f, DT: %f, DS: %f\n", input_t, input_s, input_dt, input_ds);
    
    fire_conf fc = get_fire_conf();
  
    Serial.printf("Output: \nFire Confidence: Low-> %f, Med-> %f, High-> %f\n", fc.fire_low, fc.fire_med, fc.fire_high);
    Serial.printf("Result:\nFire Confidence: %f\n", output1);
  
  #endif
  
  delay(2000); // just for ease of reading
}
