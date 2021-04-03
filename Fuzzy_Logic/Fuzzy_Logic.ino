#include <Fuzzy.h>

#define VERBOSE       true

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

  // (Smoke Low || Smoke Med) && (Delta Smoke Low || Med)
  FuzzyRuleAntecedent *if_smoke_low_or_med_and_delta_smoke_low_or_med  = new FuzzyRuleAntecedent();
  if_smoke_low_or_med_and_delta_smoke_low_or_med->joinWithAND(if_smoke_low_or_smoke_med, if_delta_smoke_low_or_delta_smoke_med);

  // (Smoke Med || Smoke High) && Delta Smoke Low
  FuzzyRuleAntecedent *if_smoke_med_or_high_and_delta_smoke_low  = new FuzzyRuleAntecedent();
  if_smoke_med_or_high_and_delta_smoke_low->joinWithAND(if_smoke_med_or_smoke_high, if_delta_smoke_low);

  // (Smoke Med || Smoke High) && ANY Delta Smoke 
  FuzzyRuleAntecedent *if_smoke_med_or_high_and_any_delta_smoke  = new FuzzyRuleAntecedent();
  if_smoke_med_or_high_and_any_delta_smoke->joinWithAND(if_smoke_med_or_smoke_high, if_any_value_delta_smoke);

  // (Smoke Low || Smoke Med) && Delta Smoke Low
  FuzzyRuleAntecedent *if_smoke_low_or_med_and_delta_smoke_low  = new FuzzyRuleAntecedent();
  if_smoke_low_or_med_and_delta_smoke_low->joinWithAND(if_smoke_low_or_smoke_med, if_delta_smoke_low);

  // Smoke High && (delta Smoke Low || delta Smoke Med)
  FuzzyRuleAntecedent *if_smoke_high_and_delta_smoke_low_or_med  = new FuzzyRuleAntecedent();
  if_smoke_high_and_delta_smoke_low_or_med->joinWithAND(if_smoke_high, if_delta_smoke_low_or_delta_smoke_med);

  // Smoke Low && (delta Smoke Med || delta Smoke High)
  FuzzyRuleAntecedent *if_smoke_low_and_delta_smoke_med_or_high  = new FuzzyRuleAntecedent();
  if_smoke_low_and_delta_smoke_med_or_high->joinWithAND(if_smoke_low, if_delta_smoke_med_or_delta_smoke_high);

  // Smoke High && Delta Smoke High
  FuzzyRuleAntecedent *if_smoke_high_and_delta_smoke_high  = new FuzzyRuleAntecedent();
  if_smoke_high_and_delta_smoke_high->joinWithAND(if_smoke_high, if_delta_smoke_high);

  // Smoke High && Delta Smoke Med
  FuzzyRuleAntecedent *if_smoke_high_and_delta_smoke_med  = new FuzzyRuleAntecedent();
  if_smoke_high_and_delta_smoke_med->joinWithAND(if_smoke_high, if_delta_smoke_med);

  // Smoke High && Delta Smoke Low
  FuzzyRuleAntecedent *if_smoke_high_and_delta_smoke_low  = new FuzzyRuleAntecedent();
  if_smoke_high_and_delta_smoke_low->joinWithAND(if_smoke_high, if_delta_smoke_low);

  // Smoke Low && Delta Smoke Low
  FuzzyRuleAntecedent *if_smoke_low_and_delta_smoke_low  = new FuzzyRuleAntecedent();
  if_smoke_low_and_delta_smoke_low->joinWithAND(if_smoke_low, if_delta_smoke_low);

  // Smoke Low && Delta Smoke Med
  FuzzyRuleAntecedent *if_smoke_low_and_delta_smoke_med  = new FuzzyRuleAntecedent();
  if_smoke_low_and_delta_smoke_med->joinWithAND(if_smoke_low, if_delta_smoke_med);

  // Smoke Low && Delta Smoke High
  FuzzyRuleAntecedent *if_smoke_low_and_delta_smoke_high  = new FuzzyRuleAntecedent();
  if_smoke_low_and_delta_smoke_high->joinWithAND(if_smoke_low, if_delta_smoke_high);

  // Smoke Low && Delta Smoke ANY
  FuzzyRuleAntecedent *if_smoke_low_and_any_delta_smoke  = new FuzzyRuleAntecedent();
  if_smoke_low_and_any_delta_smoke->joinWithAND(if_smoke_low, if_any_value_delta_smoke);

  // Smoke Med && Delta Smoke ANY
  FuzzyRuleAntecedent *if_smoke_med_and_any_delta_smoke  = new FuzzyRuleAntecedent();
  if_smoke_med_and_any_delta_smoke->joinWithAND(if_smoke_med, if_any_value_delta_smoke);

  // Smoke Med && Delta Smoke Low
  FuzzyRuleAntecedent *if_smoke_med_and_delta_smoke_low  = new FuzzyRuleAntecedent();
  if_smoke_med_and_delta_smoke_low->joinWithAND(if_smoke_med, if_delta_smoke_low);
  
  // Smoke Med && Delta Smoke Med
  FuzzyRuleAntecedent *if_smoke_med_and_delta_smoke_med  = new FuzzyRuleAntecedent();
  if_smoke_med_and_delta_smoke_med->joinWithAND(if_smoke_med, if_delta_smoke_med);

  // Smoke Med && Delta Smoke High
  FuzzyRuleAntecedent *if_smoke_med_and_delta_smoke_high  = new FuzzyRuleAntecedent();
  if_smoke_med_and_delta_smoke_high->joinWithAND(if_smoke_med, if_delta_smoke_high);

  // Smoke Med && (Delta Smoke Med || High)
  FuzzyRuleAntecedent *if_smoke_med_and_delta_smoke_med_or_high  = new FuzzyRuleAntecedent();
  if_smoke_med_and_delta_smoke_med_or_high->joinWithAND(if_smoke_med, if_delta_smoke_med_or_delta_smoke_high);

  // Smoke High && (Delta Smoke Med || High)
  FuzzyRuleAntecedent *if_smoke_high_and_delta_smoke_med_or_high  = new FuzzyRuleAntecedent();
  if_smoke_high_and_delta_smoke_med_or_high->joinWithAND(if_smoke_high, if_delta_smoke_med_or_delta_smoke_high);

  // Smoke ANY && Delta Smoke High
  FuzzyRuleAntecedent *if_any_smoke_and_delta_smoke_high  = new FuzzyRuleAntecedent();
  if_any_smoke_and_delta_smoke_high->joinWithAND(if_any_value_smoke, if_delta_smoke_high);

  // Smoke ANY && (Delta Smoke Med || High)
  FuzzyRuleAntecedent *if_any_smoke_and_delta_smoke_med_or_high  = new FuzzyRuleAntecedent();
  if_any_smoke_and_delta_smoke_med_or_high->joinWithAND(if_any_value_smoke, if_delta_smoke_med_or_delta_smoke_high);

  //////////////////////////////////
  //   Antecedents of all rules   //
  //////////////////////////////////
  
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

  // Antecedent of rule n°8

  // Temp High && delta Temp Low && Smoke Low && Delta Smoke Low
  FuzzyRuleAntecedent *if_t_h_and_dt_l_and_s_l_and_ds_l = new FuzzyRuleAntecedent();
  if_t_h_and_dt_l_and_s_l_and_ds_l->joinWithAND(if_temp_high_and_delta_temp_low, if_smoke_low_and_delta_smoke_low);

  // 

  // Antecedent of rule n°9

  // Temp Low && delta Temp Med && Smoke Low && (Delta Smoke Med || High)
  FuzzyRuleAntecedent *if_t_l_and_dt_m_and_s_l_and_ds_m_or_h = new FuzzyRuleAntecedent();
  if_t_l_and_dt_m_and_s_l_and_ds_m_or_h->joinWithAND(if_temp_low_and_delta_temp_med, if_smoke_low_and_delta_smoke_med_or_high);

  // 

  // Antecedent of rule n°10

  // Temp Low && delta Temp Med && Smoke Med && (Delta Smoke ANY)
  FuzzyRuleAntecedent *if_t_l_and_dt_m_and_s_m_and_any_ds = new FuzzyRuleAntecedent();
  if_t_l_and_dt_m_and_s_m_and_any_ds->joinWithAND(if_temp_low_and_delta_temp_med, if_smoke_med_and_any_delta_smoke);

  // 

  // Antecedent of rule n°11

  // Temp Low && delta Temp Med && Smoke High && (Delta Smoke Low || Med)
  FuzzyRuleAntecedent *if_t_l_and_dt_m_and_s_h_and_ds_l_or_m = new FuzzyRuleAntecedent();
  if_t_l_and_dt_m_and_s_h_and_ds_l_or_m->joinWithAND(if_temp_low_and_delta_temp_med, if_smoke_high_and_delta_smoke_low_or_med);

  // 

  // Antecedent of rule n°12

  // Temp Low && delta Temp High && Smoke Low && Delta Smoke High
  FuzzyRuleAntecedent *if_t_l_and_dt_h_and_s_l_and_ds_h = new FuzzyRuleAntecedent();
  if_t_l_and_dt_h_and_s_l_and_ds_h->joinWithAND(if_temp_low_and_delta_temp_high, if_smoke_low_and_delta_smoke_high);

  // 

  // Antecedent of rule n°13

  // Temp Low && delta Temp High && (Smoke Med || High) && Delta Smoke Low
  FuzzyRuleAntecedent *if_t_l_and_dt_h_and_s_m_or_h_and_ds_l = new FuzzyRuleAntecedent();
  if_t_l_and_dt_h_and_s_m_or_h_and_ds_l->joinWithAND(if_temp_low_and_delta_temp_high, if_smoke_med_or_high_and_delta_smoke_low);

  //

  // Antecedent of rule n°14

  // Temp Low && delta Temp High && Smoke Med && Delta Smoke Med
  FuzzyRuleAntecedent *if_t_l_and_dt_h_and_s_m_and_ds_m = new FuzzyRuleAntecedent();
  if_t_l_and_dt_h_and_s_m_and_ds_m->joinWithAND(if_temp_low_and_delta_temp_high, if_smoke_med_and_delta_smoke_med);

  //

  // Antecedent of rule n°15

  // Temp Med && delta Temp Low && Smoke Med && (Delta Smoke Med || High)
  FuzzyRuleAntecedent *if_t_m_and_dt_l_and_s_m_and_ds_m_or_h = new FuzzyRuleAntecedent();
  if_t_m_and_dt_l_and_s_m_and_ds_m_or_h->joinWithAND(if_temp_med_and_delta_temp_low, if_smoke_med_and_delta_smoke_med_or_high);

  //

  // Antecedent of rule n°16

  // Temp Med && delta Temp Low && Smoke High && (Delta Smoke Low || Med)
  FuzzyRuleAntecedent *if_t_m_and_dt_l_and_s_h_and_ds_l_or_m = new FuzzyRuleAntecedent();
  if_t_m_and_dt_l_and_s_h_and_ds_l_or_m->joinWithAND(if_temp_med_and_delta_temp_low, if_smoke_high_and_delta_smoke_low_or_med);

  //

  // Antecedent of rule n°17

  // Temp Med && delta Temp Med && (Smoke Low || Med) && (Delta Smoke Low || Med)
  FuzzyRuleAntecedent *if_t_m_and_dt_m_and_s_l_or_m_and_ds_l_or_m = new FuzzyRuleAntecedent();
  if_t_m_and_dt_m_and_s_l_or_m_and_ds_l_or_m->joinWithAND(if_temp_med_and_delta_temp_med, if_smoke_low_or_med_and_delta_smoke_low_or_med);

  //

  // Antecedent of rule n°18

  // Temp Med && delta Temp Med && Smoke High && Delta Smoke Low 
  FuzzyRuleAntecedent *if_t_m_and_dt_m_and_s_h_and_ds_l = new FuzzyRuleAntecedent();
  if_t_m_and_dt_m_and_s_h_and_ds_l->joinWithAND(if_temp_med_and_delta_temp_med, if_smoke_high_and_delta_smoke_low);

  //

  // Antecedent of rule n°19

  // Temp Med && delta Temp High && (Smoke Low || Med ) && Delta Smoke Low 
  FuzzyRuleAntecedent *if_t_m_and_dt_h_and_s_l_or_m_and_ds_l = new FuzzyRuleAntecedent();
  if_t_m_and_dt_h_and_s_l_or_m_and_ds_l->joinWithAND(if_temp_med_and_delta_temp_high, if_smoke_low_or_med_and_delta_smoke_low);

  //

  // Antecedent of rule n°20

  // Temp High && (delta Temp Low || Med) && Smoke Low && Delta Smoke Med
  FuzzyRuleAntecedent *if_t_h_and_dt_l_or_m_and_s_l_and_ds_m = new FuzzyRuleAntecedent();
  if_t_m_and_dt_h_and_s_l_or_m_and_ds_l->joinWithAND(if_temp_high_and_delta_temp_low_or_med, if_smoke_low_and_delta_smoke_med);

  //

  // Antecedent of rule n°21

  // Temp High && (delta Temp Med || High) && Smoke Low && Delta Smoke Low
  FuzzyRuleAntecedent *if_t_h_and_dt_m_or_h_and_s_l_and_ds_l = new FuzzyRuleAntecedent();
  if_t_h_and_dt_m_or_h_and_s_l_and_ds_l->joinWithAND(if_temp_high_and_delta_temp_med_or_high, if_smoke_low_and_delta_smoke_low);

  //

  // Antecedent of rule n°22

  // Temp Low && delta Temp Med && Smoke High && Delta Smoke High
  FuzzyRuleAntecedent *if_t_l_and_dt_m_and_s_h_and_ds_h = new FuzzyRuleAntecedent();
  if_t_l_and_dt_m_and_s_h_and_ds_h->joinWithAND(if_temp_low_and_delta_temp_med, if_smoke_high_and_delta_smoke_high);

  //

  // Antecedent of rule n°23

  // Temp Low && delta Temp High && Smoke Med && Delta Smoke High
  FuzzyRuleAntecedent *if_t_l_and_dt_h_and_s_m_and_ds_h = new FuzzyRuleAntecedent();
  if_t_l_and_dt_h_and_s_m_and_ds_h->joinWithAND(if_temp_low_and_delta_temp_high, if_smoke_med_and_delta_smoke_high);

  //

  // Antecedent of rule n°24

  // Temp Low && delta Temp High && Smoke High && (Delta Smoke Med || High)
  FuzzyRuleAntecedent *if_t_l_and_dt_h_and_s_h_and_ds_m_or_h = new FuzzyRuleAntecedent();
  if_t_l_and_dt_h_and_s_h_and_ds_m_or_h->joinWithAND(if_temp_low_and_delta_temp_high, if_smoke_high_and_delta_smoke_med_or_high);

  //

  // Antecedent of rule n°25

  // Temp Med && delta Temp Low && Smoke High && Delta Smoke High
  FuzzyRuleAntecedent *if_t_m_and_dt_l_and_s_h_and_ds_h = new FuzzyRuleAntecedent();
  if_t_m_and_dt_l_and_s_h_and_ds_h->joinWithAND(if_temp_med_and_delta_temp_low, if_smoke_high_and_delta_smoke_high);

  //

  // Antecedent of rule n°26

  // Temp Med && delta Temp Med && Smoke ANY && Delta Smoke High
  FuzzyRuleAntecedent *if_t_m_and_dt_m_and_any_s_and_ds_h = new FuzzyRuleAntecedent();
  if_t_m_and_dt_m_and_any_s_and_ds_h->joinWithAND(if_temp_med_and_delta_temp_med, if_any_smoke_and_delta_smoke_high);

  //

  // Antecedent of rule n°27

  // Temp Med && delta Temp Med && Smoke High && Delta Smoke Med
  FuzzyRuleAntecedent *if_t_m_and_dt_m_and_s_h_and_ds_m = new FuzzyRuleAntecedent();
  if_t_m_and_dt_m_and_s_h_and_ds_m->joinWithAND(if_temp_med_and_delta_temp_med, if_smoke_high_and_delta_smoke_med);

  //

  // Antecedent of rule n°28

  // Temp Med && delta Temp High && ANY Smoke && (Delta Smoke Med || High)
  FuzzyRuleAntecedent *if_t_m_and_dt_h_and_any_s_and_ds_m_or_h = new FuzzyRuleAntecedent();
  if_t_m_and_dt_h_and_any_s_and_ds_m_or_h->joinWithAND(if_temp_med_and_delta_temp_high, if_any_smoke_and_delta_smoke_med_or_high);

  //

  // Antecedent of rule n°29

  // Temp Med && delta Temp High && Smoke High && Delta Smoke Low
  FuzzyRuleAntecedent *if_t_m_and_dt_h_and_s_h_and_ds_l = new FuzzyRuleAntecedent();
  if_t_m_and_dt_h_and_s_h_and_ds_l->joinWithAND(if_temp_med_and_delta_temp_high, if_smoke_high_and_delta_smoke_low);

  //

  // Antecedent of rule n°30

  // Temp High && ANY delta Temp && Smoke Low && Delta Smoke High
  FuzzyRuleAntecedent *if_t_h_and_any_dt_and_s_l_and_ds_h = new FuzzyRuleAntecedent();
  if_t_h_and_any_dt_and_s_l_and_ds_h->joinWithAND(if_temp_high_and_delta_temp_any_value, if_smoke_low_and_delta_smoke_high);

  //

  // Antecedent of rule n°31

  // Temp High && ANY delta Temp && (Smoke Med || High) && ANY Delta Smoke 
  FuzzyRuleAntecedent *if_t_h_and_any_dt_and_s_m_or_h_and_any_ds = new FuzzyRuleAntecedent();
  if_t_h_and_any_dt_and_s_m_or_h_and_any_ds->joinWithAND(if_temp_high_and_delta_temp_any_value, if_smoke_med_or_high_and_any_delta_smoke);

  //

  // Antecedent of rule n°32

  // Temp High && delta Temp High && Smoke Low && Delta Smoke Med
  FuzzyRuleAntecedent *if_t_h_and_dt_h_and_s_l_and_ds_m = new FuzzyRuleAntecedent();
  if_t_h_and_dt_h_and_s_l_and_ds_m->joinWithAND(if_temp_high_and_delta_temp_high, if_smoke_low_and_delta_smoke_med);

  //

  // Consequents of all possible antecedents
  FuzzyRuleConsequent *then_fire_confidence_low = new FuzzyRuleConsequent();
  then_fire_confidence_low->addOutput(FC_Low);  

  FuzzyRuleConsequent *then_fire_confidence_med = new FuzzyRuleConsequent();
  then_fire_confidence_med->addOutput(FC_Med);

  FuzzyRuleConsequent *then_fire_confidence_high = new FuzzyRuleConsequent();
  then_fire_confidence_high->addOutput(FC_High);

  /////////////////////
  //    All rules    //
  /////////////////////

  // Rule n°1
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, if_t_l_and_dt_l_and_s_l_or_m_and_any_ds, then_fire_confidence_low);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Rule n°2
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, if_t_l_and_dt_l_and_s_h_and_ds_l_or_m, then_fire_confidence_low);
  fuzzy->addFuzzyRule(fuzzyRule2);

  // Rule n°3
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, if_t_l_and_dt_l_and_s_h_and_ds_h, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule3);

  // Rule n°4
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, if_t_l_and_dt_m_or_h_and_s_l_and_ds_l, then_fire_confidence_low);
  fuzzy->addFuzzyRule(fuzzyRule4);

  // Rule n°5
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, if_t_l_and_dt_h_and_s_l_and_ds_m, then_fire_confidence_low);
  fuzzy->addFuzzyRule(fuzzyRule5);

  // Rule n°6
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, if_t_m_and_dt_l_and_s_l_and_any_ds, then_fire_confidence_low);
  fuzzy->addFuzzyRule(fuzzyRule6);

  // Rule n°7
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, if_t_m_and_dt_l_and_s_m_and_ds_l, then_fire_confidence_low);
  fuzzy->addFuzzyRule(fuzzyRule7);

  // Rule n°8
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, if_t_h_and_dt_l_and_s_l_and_ds_l, then_fire_confidence_low);
  fuzzy->addFuzzyRule(fuzzyRule8);

  // Rule n°9
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, if_t_l_and_dt_m_and_s_l_and_ds_m_or_h, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule9);

  // Rule n°10
  FuzzyRule *fuzzyRule10 = new FuzzyRule(10, if_t_l_and_dt_m_and_s_m_and_any_ds, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule10);

  // Rule n°11
  FuzzyRule *fuzzyRule11 = new FuzzyRule(11, if_t_l_and_dt_m_and_s_h_and_ds_l_or_m, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule11);

  // Rule n°12
  FuzzyRule *fuzzyRule12 = new FuzzyRule(12, if_t_l_and_dt_h_and_s_l_and_ds_h, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule12);

  // Rule n°13
  FuzzyRule *fuzzyRule13 = new FuzzyRule(13, if_t_l_and_dt_h_and_s_m_or_h_and_ds_l, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule13);

  // Rule n°14
  FuzzyRule *fuzzyRule14 = new FuzzyRule(14, if_t_l_and_dt_h_and_s_m_and_ds_m, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule14);

  // Rule n°15
  FuzzyRule *fuzzyRule15 = new FuzzyRule(15, if_t_m_and_dt_l_and_s_m_and_ds_m_or_h, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule15);

  // Rule n°16
  FuzzyRule *fuzzyRule16 = new FuzzyRule(16, if_t_m_and_dt_l_and_s_h_and_ds_l_or_m, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule16);

  // Rule n°17
  FuzzyRule *fuzzyRule17 = new FuzzyRule(17, if_t_m_and_dt_m_and_s_l_or_m_and_ds_l_or_m, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule17);

  // Rule n°18
  FuzzyRule *fuzzyRule18 = new FuzzyRule(18, if_t_m_and_dt_m_and_s_h_and_ds_l, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule18);

  // Rule n°19
  FuzzyRule *fuzzyRule19 = new FuzzyRule(19, if_t_m_and_dt_h_and_s_l_or_m_and_ds_l, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule19);

  // Rule n°20
  FuzzyRule *fuzzyRule20 = new FuzzyRule(20, if_t_m_and_dt_h_and_s_l_or_m_and_ds_l, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule20);

  // Rule n°21
  FuzzyRule *fuzzyRule21 = new FuzzyRule(21, if_t_h_and_dt_m_or_h_and_s_l_and_ds_l, then_fire_confidence_med);
  fuzzy->addFuzzyRule(fuzzyRule21);

  // Rule n°22
  FuzzyRule *fuzzyRule22 = new FuzzyRule(22, if_t_l_and_dt_m_and_s_h_and_ds_h, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule22);

  // Rule n°23
  FuzzyRule *fuzzyRule23 = new FuzzyRule(23, if_t_l_and_dt_h_and_s_m_and_ds_h, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule23);

  // Rule n°24
  FuzzyRule *fuzzyRule24 = new FuzzyRule(24, if_t_l_and_dt_h_and_s_h_and_ds_m_or_h, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule24);

  // Rule n°25
  FuzzyRule *fuzzyRule25 = new FuzzyRule(25, if_t_m_and_dt_l_and_s_h_and_ds_h, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule25);

  // Rule n°26
  FuzzyRule *fuzzyRule26 = new FuzzyRule(26, if_t_m_and_dt_m_and_any_s_and_ds_h, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule26);

  // Rule n°27
  FuzzyRule *fuzzyRule27 = new FuzzyRule(27, if_t_m_and_dt_m_and_s_h_and_ds_m, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule27);

  // Rule n°28
  FuzzyRule *fuzzyRule28 = new FuzzyRule(28, if_t_m_and_dt_h_and_any_s_and_ds_m_or_h, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule28);

  // Rule n°29
  FuzzyRule *fuzzyRule29 = new FuzzyRule(29, if_t_m_and_dt_h_and_s_h_and_ds_l, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule29);

  // Rule n°30
  FuzzyRule *fuzzyRule30 = new FuzzyRule(30, if_t_h_and_any_dt_and_s_l_and_ds_h, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule30);

  // Rule n°31
  FuzzyRule *fuzzyRule31 = new FuzzyRule(31, if_t_h_and_any_dt_and_s_m_or_h_and_any_ds, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule31);

  // Rule n°32
  FuzzyRule *fuzzyRule32 = new FuzzyRule(32, if_t_h_and_dt_h_and_s_l_and_ds_m, then_fire_confidence_high);
  fuzzy->addFuzzyRule(fuzzyRule32);

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
  
  fuzzy->setInput(1, input_t);
  fuzzy->setInput(2, input_s);
  fuzzy->setInput(3, input_dt);
  fuzzy->setInput(4, input_ds);

  fuzzy->fuzzify();

  #if VERBOSE
  
    Serial.printf("T: %f,S: %f, DT: %f, DS:%f\n", input_t, input_s, input_dt, input_ds);
  
    Serial.println("Input: ");
    Serial.print("Temp: Low-> ");
    Serial.print(IT_Low->getPertinence());
    Serial.print(", Med-> ");
    Serial.print(IT_Med->getPertinence());
    Serial.print(", High-> ");
    Serial.println(IT_High->getPertinence());
  
    Serial.print("Smoke: Low-> ");
    Serial.print(IS_Low->getPertinence());
    Serial.print(", Med-> ");
    Serial.print(IS_Med->getPertinence());
    Serial.print(", High-> ");
    Serial.println(IS_High->getPertinence());
    
  #endif
  
  float output1 = fuzzy->defuzzify(1);

  #if VERBOSE
  
    Serial.println("Output: ");
    Serial.print("Fire Confidence: Low-> ");
    Serial.print(FC_Low->getPertinence());
    Serial.print(", Med-> ");
    Serial.print(FC_Med->getPertinence());
    Serial.print(", High-> ");
    Serial.println(FC_High->getPertinence());
  
    Serial.println("Result: ");
    Serial.print("Fire Confidence: ");
    Serial.println(output1);
  
  #endif
  
  delay(2000);
}
