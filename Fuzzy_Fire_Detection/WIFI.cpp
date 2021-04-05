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
  yield();
  
  // Disable the WiFi persistence. ESP8266 will not load and save WiFi settings in the flash memory.
  WiFi.persistent(false);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASSWD);
  
  /*
  IPAddress staticIP(192,168,1,22);
  IPAddress gateway(192,168,1,9);
  IPAddress subnet(255,255,255,0);
    
  // Make a static IP address to DHCP IP request time
  WiFi.config(staticIP, gateway, subnet);
  */
}
