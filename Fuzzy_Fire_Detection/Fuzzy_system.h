#ifndef __FUZZY_SYS_H__
  #define __FUZZY_SYS_H__
  #include <Fuzzy.h>

  typedef struct{
    float fire_low;
    float fire_med;
    float fire_high;
  }fire_conf;
  
  void setup_fuzzy(); // Sets up the fuzzy system's inputs, output, and rule-base
  void set_fuzzy_inputs(float, float, float, float);
  void fuzzify_system();
  float defuzzify_system(); // Get system's output
  fire_conf get_fire_conf(); // Get fire confidences from the fuzzy system

#endif
