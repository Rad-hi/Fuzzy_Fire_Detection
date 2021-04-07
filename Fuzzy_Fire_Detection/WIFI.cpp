#include"WIFI.h"

#ifndef __MQTT_H__
  #include<ESP8266WiFi.h>
#endif  

void off_unnecessary(){
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  yield();
}

void wake_wifi_up(){
  WiFi.forceSleepWake();
  delay(1);
  
  // Disable the WiFi persistence. ESP8266 will not load and save WiFi settings in the flash memory.
  WiFi.persistent(false);


  // Make a static IP address to decrease DHCP IP request time
  IPAddress staticIP(104,21,47,183); // IP of adafruit.io
  IPAddress dns(107,21,47,183);      // DNS address of adafruit.io
  IPAddress gateway(192,168,1,9);    // Optional, so left them randomly
  IPAddress subnet(255,255,255,0);   // Default
  WiFi.config(staticIP, dns, gateway, subnet);

  // Turn on WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASSWD);
  delay(50);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(50);
  }
}
