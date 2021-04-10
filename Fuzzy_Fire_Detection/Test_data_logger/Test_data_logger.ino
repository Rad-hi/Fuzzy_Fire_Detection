#include"Data_Logger.h"

// 115.2K baud serial connection to computer
#define SERIAL_MON_BAUD_RATE    115200

void readFile(const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void setup() {
  delay(1000);

  // Mount the file-system
  LittleFS.begin();
  
  unsigned long start_ = millis();
  int num_lines = 0;

  // Set the Serial output
  Serial.begin(SERIAL_MON_BAUD_RATE);

/*

  LittleFS.format();
  
  randomSeed(analogRead(0));

  // Writing
  Serial.println("\n\nWriting to the FLash memory");
  for (int i = 0; i < 24; i++) {
    byte num_readings = random(0, 3);
    while (num_readings--) {
      float temp = random(0, 100);
      write_temp(temp, false);
      num_lines++;
    }
    write_temp(69, true); // Terminate hour
    num_lines++;
  }

  Serial.printf("Done writing. Wrote %d lines!\n", num_lines);

*/


//  readFile("/Temp_Log.txt");


  // Reading
  Serial.println("Going to read the file's content");
  char* buffer = read_day();


  Serial.print("Read: ");
  Serial.println(buffer);
  
  free(buffer);

  //
  Serial.printf("\nReading took: %d seconds\n", (millis() - start_));

 
  // Unmount the file-system
  LittleFS.end();

  delay(5000);
}

void loop() {
  yield();
}
