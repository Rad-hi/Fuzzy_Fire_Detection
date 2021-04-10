#include"Data_Logger.h"

void write_temp(float temp, bool hour_){
  // The file is created if doesn't exist, we're appending at the end of it
  File file = LittleFS.open(FILE_PATH, "a");
  yield();
  
  // The '#' char indicates an end of hour. We'll need that in the read_day()
  // function for the daily-report's data parsing
  if(hour_) file.print(String(temp)+",#");
  else      file.print(String(temp)+",");
  file.close();
}

void read_day(const char* buffer_){
  StaticJsonDocument<JSON_BUFFER_SIZE> DATA;  // Json file that'll contain all data, and then be sent via mqtt
  
  File file = LittleFS.open(FILE_PATH, "r");
  yield();

  float max_ = 0.0001, min_ = 1000, mean_ = 0;  
  byte hour_ = 0;
  byte readings = 0;
  String val = "";
  while(file.available()){
    char c = file.read(); 
    switch(c - '0'){
      case -2: // Read a '.'
      case 0 ... 9:{ // Read a val
        val += c; 
        break;
      }
      case -4:{ // Read a ',' --> end of Temp value
        float f = strtof(val.c_str(), NULL);
        if(f >= max_) max_ = f;
        if(f <= min_) min_ = f;
        mean_ += f;
        readings ++;
        val = "";
        break;
      }
      case -13:{ // Read a '#' --> end of hour
        mean_ /= readings;

        // Fill data to be sent
        String tag = "H"+String(hour_);
        DATA[tag] = max_;
        DATA[tag] = min_;
        DATA[tag] = mean_;
        
        // Reset all values
        max_ = 0.0001;
        min_ = 1000;
        mean_ = 0;
        readings = 0;
        hour_ ++;
        break;
      }
      default: break;
    }
  }
  file.close();
  //serializeJson(DATA, buffer_); // There's a problem with this, fix it! It's with the buffer !!!
}

void remove_file(){
  LittleFS.remove(FILE_PATH);
}
