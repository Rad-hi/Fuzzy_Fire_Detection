#include <Fuzzy.h>

// Fuzzy system instance
Fuzzy *fuzzy = new Fuzzy();

// Membership functions' definition
// https://blog.zerokol.com/2012/09/arduinofuzzy-fuzzy-library-for-arduino.html

// Inputs' membership funtions
FuzzySet *IT_Low     = new FuzzySet(0, 0, 30, 50);
FuzzySet *IT_Med     = new FuzzySet(30, 50, 50, 70);
FuzzySet *IT_High    = new FuzzySet(50, 70, 80, 80);

FuzzySet *D_IT_Low   = new FuzzySet(-20, -20, 0, 5);
FuzzySet *D_IT_Med   = new FuzzySet(0, 5, 5, 10);
FuzzySet *D_IT_High  = new FuzzySet(5, 10, 20, 20);

FuzzySet *IS_Low     = new FuzzySet(0, 0, 0.07, 0.15);
FuzzySet *IS_Med     = new FuzzySet(0.07, 0.15, 0.15, 0.23);
FuzzySet *IS_High    = new FuzzySet(0.15, 0.23, 0.25, 0.25);

FuzzySet *D_IS_Low   = new FuzzySet(-0.01, -0.01, 0, 0.02);
FuzzySet *D_IS_Med   = new FuzzySet(0, 0.02, 0.02, 0.04);
FuzzySet *D_IS_High  = new FuzzySet(0.02, 0.04, 0.05, 0.05);

// Output's memebership funtion (Fire Confidence)
FuzzySet *FC_Low     = new FuzzySet(-25, -25, 0, 50);
FuzzySet *FC_Med    = new FuzzySet(0, 50, 50, 100);
FuzzySet *FC_High   = new FuzzySet(50, 100, 125, 125);

void setup() {

  // Input Temperature
  FuzzyInput *IT   = new FuzzyInput(1);
  IT->addFuzzySet(IT_Low);
  IT->addFuzzySet(IT_Med);
  IT->addFuzzySet(IT_High);
  fuzzy->addFuzzyInput(IT);
  
  // Delta Input Temperature
  FuzzyInput *D_IT = new FuzzyInput(2);
  D_IT->addFuzzySet(D_IT_Low);
  D_IT->addFuzzySet(D_IT_Med);
  D_IT->addFuzzySet(D_IT_High);
  fuzzy->addFuzzyInput(D_IT);
    
  // Input Smoke
  FuzzyInput *IS   = new FuzzyInput(3);
  IS->addFuzzySet(IS_Low);
  IS->addFuzzySet(IS_Med);
  IS->addFuzzySet(IS_High);
  fuzzy->addFuzzyInput(IS);
  
  // Delta Input Smoke
  FuzzyInput *D_IS = new FuzzyInput(4);
  D_IS->addFuzzySet(D_IS_Low);
  D_IS->addFuzzySet(D_IS_Med);
  D_IS->addFuzzySet(D_IS_High);
  fuzzy->addFuzzyInput(D_IS);

  // Fire Confidence --> Output
  FuzzyOutput *FC = new FuzzyOutput(1);
  FC->addFuzzySet(FC_Low);
  FC->addFuzzySet(FC_Med);
  FC->addFuzzySet(FC_High);
  fuzzy->addFuzzyOutput(FC);

  // Building fuzzy rule-base

  // Temp Low && Delta Temp Low
  FuzzyRuleAntecedent *if_temp_low_and_delta_temp_low   = new FuzzyRuleAntecedent();
  if_temp_low_and_delta_temp_low->joinWithAND(IT_Low, D_IT_Low);

  // Temp Low && Delta Temp Med
  FuzzyRuleAntecedent *if_temp_low_and_delta_temp_med   = new FuzzyRuleAntecedent();
  if_temp_low_and_delta_temp_med->joinWithAND(IT_Low, D_IT_Med);

  // Temp Low && Delta Temp High
  FuzzyRuleAntecedent *if_temp_low_and_delta_temp_high  = new FuzzyRuleAntecedent();
  if_temp_low_and_delta_temp_high->joinWithAND(IT_Low, D_IT_High);

  // Temp Med && Delta Temp Low
  FuzzyRuleAntecedent *if_temp_med_and_delta_temp_low   = new FuzzyRuleAntecedent();
  if_temp_med_and_delta_temp_low->joinWithAND(IT_Med, D_IT_Low);

  // Temp Med && Delta Temp Med
  FuzzyRuleAntecedent *if_temp_med_and_delta_temp_med   = new FuzzyRuleAntecedent();
  if_temp_med_and_delta_temp_med->joinWithAND(IT_Med, D_IT_Med);

  // Temp Med && Delta Temp High
  FuzzyRuleAntecedent *if_temp_med_and_delta_temp_high  = new FuzzyRuleAntecedent();
  if_temp_med_and_delta_temp_high->joinWithAND(IT_Med, D_IT_High);
  
  // Temp High && Delta Temp Low
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_low   = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_low->joinWithAND(IT_High, D_IT_Low);

  // Temp High && Delta Temp Med
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_med   = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_med->joinWithAND(IT_High, D_IT_Med);

  // Temp High && Delta Temp High
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_high  = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_high->joinWithAND(IT_High, D_IT_High);

  
}

void loop() {
  
}
