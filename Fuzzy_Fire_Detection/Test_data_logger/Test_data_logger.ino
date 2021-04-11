#include"Data_Logger.h"

// 115.2K baud serial connection to computer
#define SERIAL_MON_BAUD_RATE    115200

//
void readFile(const char*);

void setup() {
  delay(1000);

  // Mount the file-system
  LittleFS.begin();
  
  unsigned long start_ = millis();
  int num_lines = 0;

  // Set the Serial output
  Serial.begin(SERIAL_MON_BAUD_RATE);


// Uncomment this part if you want to write new data to the Flash of your ESP
// I'm simulating a data logging of a temperature value that ranges between:
// Once every minute -to- once every 10 seconds (depending on the operation mode)


  //LittleFS.format(); // This deletes the files (formats the flash)
  
  randomSeed(analogRead(0));

  // Writing
  Serial.println("\nWriting to the FLash memory");
  for (int i = 0; i < 2; i++) { // The 24h of the day
    byte num_readings = random(0, 3); // randomly choosing the number of points to best
                                        // test the parser since the real thing will need
                                        // to parse non consisting intervals
    while (num_readings--) {
      float temp = random(0, 99); // Random Temp values
      write_temp(temp, false);    // false --> not the end of the hour yet
      num_lines++;
    }
    write_temp(69, true); // true --> Terminate hour
    num_lines++;
  }

  Serial.printf("Done writing. Wrote %d values!\n", num_lines);


  readFile("/Temp_Log.txt"); // Read file's content

  // Reading
  Serial.println("\nGoing to read the file's content");
  
  char* buffer = (char*)malloc(sizeof(char)*JSON_BUFFER_SIZE); // Allocate memory for the buffer
  
  read_day(buffer); // Parse the day's data

  Serial.print("Read: ");
  Serial.println(buffer);
  
  free(buffer); // Garbage collection

  unsigned long tt = millis() - start_;
  long m = tt/60000, s = (tt%60000)/1000, ms = tt - 60000 * m - s * 1000; 
  Serial.printf("\nProcess took: %d:%d:%d (minutes, seconds, milli-seconds)\n", m, s, ms);

  // Unmount the file-system
  LittleFS.end();

  delay(5000);
}

void loop() {
  yield();
}


void readFile(const char * path){
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
