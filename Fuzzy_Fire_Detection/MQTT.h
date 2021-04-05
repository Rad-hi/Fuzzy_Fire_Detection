#ifndef __MQTT_H__
  #define __MQTT_H__
      
  #include"Arduino.h"
  
  #include<ESP8266WiFi.h>
  #include <PubSubClient.h>  // https://github.com/knolleary/pubsubclient/
  
  #define BROKER            "io.adafruit.com"
  #define MQTT_PORT         1883
  #define MQTT_USER         "Radhi"
  #define MQTT_PASSWORD     ""
  #define TOPIC             "Radhi/feeds/Fire"
  
  void MQTT_setup();
  void MQTT_connect();
  void MQTT_subscribe();
  void MQTT_callback(char*, byte*, unsigned int);
  void send_data(const char*);
#endif
