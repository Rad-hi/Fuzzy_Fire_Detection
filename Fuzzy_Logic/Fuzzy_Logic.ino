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
FuzzySet *FC_Med     = new FuzzySet(0, 50, 50, 100);
FuzzySet *FC_High    = new FuzzySet(50, 100, 125, 125);

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

  /******************************************************
   *             Building fuzzy rule-base               *
   *  First its elements, then the joints between them, *
   *            and then the actual rules.              *
   ******************************************************/
   
  // Singletons
  
      // Smoke Low
      FuzzyRuleAntecedent *if_smoke_low  = new FuzzyRuleAntecedent();
      if_smoke_low->joinSingle(IS_Low);
    
      // Smoke Med
      FuzzyRuleAntecedent *if_smoke_med  = new FuzzyRuleAntecedent();
      if_smoke_med->joinSingle(IS_Med);
      
      // Smoke High
      FuzzyRuleAntecedent *if_smoke_high = new FuzzyRuleAntecedent();
      if_smoke_high->joinSingle(IS_High);

      //

      // Temp Low
      FuzzyRuleAntecedent *if_temp_low  = new FuzzyRuleAntecedent();
      if_temp_low->joinSingle(IT_Low);
    
      // Temp High
      FuzzyRuleAntecedent *if_temp_high  = new FuzzyRuleAntecedent();
      if_temp_high->joinSingle(IT_High);

      //

      // Delta Smoke Low
      FuzzyRuleAntecedent *if_delta_smoke_low  = new FuzzyRuleAntecedent();
      if_delta_smoke_low->joinSingle(D_IS_Low);
    
      // Delta Smoke Med
      FuzzyRuleAntecedent *if_delta_smoke_med  = new FuzzyRuleAntecedent();
      if_delta_smoke_med->joinSingle(D_IS_Med);

      // Delta Smoke High
      FuzzyRuleAntecedent *if_delta_smoke_high = new FuzzyRuleAntecedent();
      if_delta_smoke_high->joinSingle(D_IS_High);

      //

      // Delta Temp Low
      FuzzyRuleAntecedent *if_delta_temp_low  = new FuzzyRuleAntecedent();
      if_delta_temp_low->joinSingle(D_IT_Low);
  
  //

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
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_low  = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_low->joinWithAND(IT_High, D_IT_Low);

  /*
  // Temp High && Delta Temp Med
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_med  = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_med->joinWithAND(IT_High, D_IT_Med);
  */
  
  // Temp High && Delta Temp High
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_high = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_high->joinWithAND(IT_High, D_IT_High);

  // Smoke Low OR Smoke Med
  FuzzyRuleAntecedent *if_smoke_low_or_smoke_med = new FuzzyRuleAntecedent();
  if_smoke_low_or_smoke_med->joinWithOR(IS_Low, IS_Med);

  // Smoke Med OR Smoke High
  FuzzyRuleAntecedent *if_smoke_med_or_smoke_high = new FuzzyRuleAntecedent();
  if_smoke_med_or_smoke_high->joinWithOR(IS_Med, IS_High);

  // Any value of Smoke (smoke_low OR smoke_med OR smoke_high)
  
      // Smoke any value
      FuzzyRuleAntecedent *if_any_value_smoke = new FuzzyRuleAntecedent();
      if_any_value_smoke->joinWithOR(if_smoke_low, if_smoke_med_or_smoke_high);
  
  //
         
  // Any value of delta Smoke (delta_smoke_low OR delta_smoke_med OR delta_smoke_high)
  
      // Delta Smoke low OR delta Smoke Med
      FuzzyRuleAntecedent *if_delta_smoke_low_or_delta_smoke_med = new FuzzyRuleAntecedent();
      if_delta_smoke_low_or_delta_smoke_med->joinWithOR(D_IS_Low, D_IS_Med);

      // Delta Smoke any value
      FuzzyRuleAntecedent *if_any_value_delta_smoke = new FuzzyRuleAntecedent();
      if_any_value_delta_smoke->joinWithOR(if_delta_smoke_low_or_delta_smoke_med, if_delta_smoke_high);
  
  //
  
  // Delta Smoke Med OR delta Smoke High
  FuzzyRuleAntecedent *if_delta_smoke_med_or_delta_smoke_high = new FuzzyRuleAntecedent();
  if_delta_smoke_med_or_delta_smoke_high->joinWithOR(D_IS_Med, D_IS_High);

  // Delta Temp Med OR delta Temp High
  FuzzyRuleAntecedent *if_delta_temp_med_or_delta_temp_high = new FuzzyRuleAntecedent();
  if_delta_temp_med_or_delta_temp_high->joinWithOR(D_IT_Med, D_IT_High);

  // Delta Temp Low OR delta Temp Med
  FuzzyRuleAntecedent *if_delta_temp_low_or_delta_temp_med = new FuzzyRuleAntecedent();
  if_delta_temp_low_or_delta_temp_med->joinWithOR(D_IT_Low, D_IT_Med);

  // Any value of delta Temp (delta_temp_low OR delta_temp_med OR delta_temp_high)

      // Delta temp any value
      FuzzyRuleAntecedent *if_any_value_delta_temp = new FuzzyRuleAntecedent();
      if_any_value_delta_temp->joinWithOR(if_delta_temp_low, if_delta_temp_med_or_delta_temp_high);

  //

  // Temp Low AND (delta temp med OR delta temp high)
  FuzzyRuleAntecedent *if_temp_low_and_delta_temp_med_or_high  = new FuzzyRuleAntecedent();
  if_temp_low_and_delta_temp_med_or_high->joinWithAND(if_temp_low, if_delta_temp_med_or_delta_temp_high);

  // Temp High AND (delta temp low OR delta temp med)
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_low_or_med  = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_low_or_med->joinWithAND(if_temp_high, if_delta_temp_low_or_delta_temp_med);

  // Temp High AND (delta temp med OR delta temp high)
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_med_or_high  = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_med_or_high->joinWithAND(if_temp_high, if_delta_temp_med_or_delta_temp_high);

  // Temp High AND Any value of delta temp
  FuzzyRuleAntecedent *if_temp_high_and_delta_temp_any_value  = new FuzzyRuleAntecedent();
  if_temp_high_and_delta_temp_any_value->joinWithAND(if_temp_high, if_any_value_delta_temp);

  // (Smoke Low || Smoke Med) && (Any value of Delta Smoke)
  FuzzyRuleAntecedent *if_smoke_low_or_med_and_any_delta_smoke  = new FuzzyRuleAntecedent();
  if_smoke_low_or_med_and_any_delta_smoke->joinWithAND(if_smoke_low_or_smoke_med, if_any_value_delta_smoke);

  // Smoke High && (delta Smoke Low || delta Smoke Med)
  FuzzyRuleAntecedent *if_smoke_high_and_delta_smoke_low_or_med  = new FuzzyRuleAntecedent();
  if_smoke_high_and_delta_smoke_low_or_med->joinWithAND(if_smoke_high, if_delta_smoke_low_or_delta_smoke_med);

  // Smoke High && Delta Smoke High
  FuzzyRuleAntecedent *if_smoke_high_and_delta_smoke_high  = new FuzzyRuleAntecedent();
  if_smoke_high_and_delta_smoke_high->joinWithAND(if_smoke_high, if_delta_smoke_high);

  // Smoke Low && Delta Smoke Low
  FuzzyRuleAntecedent *if_smoke_low_and_delta_smoke_low  = new FuzzyRuleAntecedent();
  if_smoke_low_and_delta_smoke_low->joinWithAND(if_smoke_low, if_delta_smoke_low);

  // Smoke Low && Delta Smoke Med
  FuzzyRuleAntecedent *if_smoke_low_and_delta_smoke_med  = new FuzzyRuleAntecedent();
  if_smoke_low_and_delta_smoke_med->joinWithAND(if_smoke_low, if_delta_smoke_med);

  // Smoke Low && Delta Smoke ANY
  FuzzyRuleAntecedent *if_smoke_low_and_any_delta_smoke  = new FuzzyRuleAntecedent();
  if_smoke_low_and_any_delta_smoke->joinWithAND(if_smoke_low, if_any_value_delta_smoke);

  // Smoke Med && Delta Smoke Low
  FuzzyRuleAntecedent *if_smoke_med_and_delta_smoke_low  = new FuzzyRuleAntecedent();
  if_smoke_med_and_delta_smoke_low->joinWithAND(if_smoke_med, if_delta_smoke_low);
  
  

  // Antecedent of rule n°1
  
      // Temp Low && delta Temp Low && (Smoke Low || Smoke Med) && (Any value of Delta Smoke)
      FuzzyRuleAntecedent *if_t_l_and_dt_l_and_s_l_or_m_and_any_ds  = new FuzzyRuleAntecedent();
      if_t_l_and_dt_l_and_s_l_or_m_and_any_ds->joinWithAND(if_temp_low_and_delta_temp_low, if_smoke_low_or_med_and_any_delta_smoke);

  //

  // Antecedent of rule n°2
  
      // Temp Low && delta Temp Low && Smoke High && (Delta Smoke Low || Delta Smoke Med)
      FuzzyRuleAntecedent *if_t_l_and_dt_l_and_s_h_and_ds_l_or_m  = new FuzzyRuleAntecedent();
      if_t_l_and_dt_l_and_s_h_and_ds_l_or_m->joinWithAND(if_temp_low_and_delta_temp_low, if_smoke_high_and_delta_smoke_low_or_med);

  //  
  
  // Antecedent of rule n°3
  
      // Temp Low && delta Temp Low && Smoke High && Delta Smoke High
      FuzzyRuleAntecedent *if_t_l_and_dt_l_and_s_h_and_ds_h = new FuzzyRuleAntecedent();
      if_t_l_and_dt_l_and_s_h_and_ds_h->joinWithAND(if_temp_low_and_delta_temp_low, if_smoke_high_and_delta_smoke_high);

  //
    
  // Antecedent of rule n°4
  
      // Temp Low && (delta Temp Med || delta Temp High) && Smoke Low && Delta Smoke Low
      FuzzyRuleAntecedent *if_t_l_and_dt_m_or_h_and_s_l_and_ds_l = new FuzzyRuleAntecedent();
      if_t_l_and_dt_m_or_h_and_s_l_and_ds_l->joinWithAND(if_temp_low_and_delta_temp_med_or_high, if_smoke_low_and_delta_smoke_low);

  //
    
  // Antecedent of rule n°5
  
      // Temp Low && delta Temp High && Smoke Low && Delta Smoke Med
      FuzzyRuleAntecedent *if_t_l_and_dt_h_and_s_l_and_ds_m = new FuzzyRuleAntecedent();
      if_t_l_and_dt_h_and_s_l_and_ds_m->joinWithAND(if_temp_low_and_delta_temp_high, if_smoke_low_and_delta_smoke_med);

  //  

  // Antecedent of rule n°6

    // Temp Med && delta Temp Low && Smoke Low && (Any Delta Smoke)
    FuzzyRuleAntecedent *if_t_m_and_dt_l_and_s_l_and_any_ds = new FuzzyRuleAntecedent();
    if_t_m_and_dt_l_and_s_l_and_any_ds->joinWithAND(if_temp_med_and_delta_temp_low, if_smoke_low_and_any_delta_smoke);

  //  

  // Antecedent of rule n°7

  // Temp Med && delta Temp Low && Smoke Med && Delta Smoke Low
  FuzzyRuleAntecedent *if_t_m_and_dt_l_and_s_m_and_ds_l = new FuzzyRuleAntecedent();
  if_t_m_and_dt_l_and_s_m_and_ds_l->joinWithAND(if_temp_med_and_delta_temp_low, if_smoke_med_and_delta_smoke_low);

  //  
}

void loop() {
  
}
