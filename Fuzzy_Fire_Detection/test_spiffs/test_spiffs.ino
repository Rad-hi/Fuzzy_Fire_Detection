// In order to enable writing to the spiffs on your esp8266,
// visit this github page and follow the installation guide
// https://github.com/earlephilhower/arduino-esp8266littlefs-plugin

#include <LittleFS.h>

void setup() {
  Serial.begin(115200);
  Serial.println("\n================================");

  // If we can't mount the file system (eg you've confused SPIFFS with LITTLEFS) abort here
  if (!LittleFS.begin()) {
    Serial.println("LITTLEFS Mount Failed.");
    return;
  } else {
    Serial.println("LITTLEFS Mount SUCCESSFUL.");
  }

  // List directories in the flash 
  Serial.println("\n================================");
  listDir("/");

  const char * path = "/Test.txt";
  Serial.println("\n================================");
  Serial.printf("Reading file: %s\r\n", path);
  File file = LittleFS.open(path, "r");

  // Could we open the file?
  if (!file) {
    Serial.println("File failed to open.");
    return;
  } else {
    Serial.println("File opened successfully.");
  }

  Serial.println("\n================================");
  Serial.println("Reading from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();

  // Add done. We must execute the "end" here to allow OTA updates.
  LittleFS.end();

}

// Nothing to do in the loop. Execute a yield to keep the WiFi alive.
void loop() {
  yield();
}

void listDir(const char * dirname) {
  Serial.printf("Listing directory: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    Serial.print("  FILE: ");
    Serial.print(root.fileName());
    Serial.print("  SIZE: ");
    Serial.print(file.size());
    time_t cr = file.getCreationTime();
    time_t lw = file.getLastWrite();
    file.close();
    struct tm * tmstruct = localtime(&cr);
    Serial.printf("    CREATION: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    tmstruct = localtime(&lw);
    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
  }
}
