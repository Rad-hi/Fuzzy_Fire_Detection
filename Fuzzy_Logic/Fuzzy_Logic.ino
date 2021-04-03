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
  
  // get random entrances
  float input_t = random(0, 100);
  float input_s = random(-0.001, 0.02);
  float input_dt = input_t - prev_t;
  float input_ds = input_s - prev_s;
  prev_t = input_t;
  prev_s = input_s;

  set_fuzzy_inputs(input_t, input_s, input_dt, input_ds);

  fuzzify_system();
  
  float output1 = defuzzify_system();

  #if VERBOSE

    Serial.printf("Inputs:\nT: %f,S: %f, DT: %f, DS: %f\n", input_t, input_s, input_dt, input_ds);
    
    fire_conf fc = get_fire_conf();
  
    Serial.println("Output confidence: ");
    Serial.print("Fire Confidence: Low-> ");
    Serial.print(fc.fire_low);
    Serial.print(", Med-> ");
    Serial.print(fc.fire_med);
    Serial.print(", High-> ");
    Serial.println(fc.fire_high);
  
    Serial.println("Result: ");
    Serial.print("Fire Confidence: ");
    Serial.println(output1);
  
  #endif
  
  delay(2000);
}
