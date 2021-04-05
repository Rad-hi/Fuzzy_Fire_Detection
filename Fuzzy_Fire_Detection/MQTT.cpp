#include"MQTT.h"

WiFiClient client; // Used in MQTT.h 
// Create mqtt port (client might be a wifi or gprs client)
PubSubClient  mqtt(client);

void MQTT_setup(){
  mqtt.setServer(BROKER, MQTT_PORT);
  //mqtt.setCallback(MQTT_callback);
}

void MQTT_connect(){
  mqtt.connect(BROKER, MQTT_USER, MQTT_PASSWORD);
  yield();
}

void MQTT_subscribe(){
   mqtt.subscribe(TOPIC);
}

// Use this function in case the module is expected to receive data
// as well as send it. In such case, depending on the topic, 
// you can do whatever you want.
void MQTT_callback(char* topic, byte* message, unsigned int len){
  String recieved_msg = "";
  for (int i = 0; i < len; i++) {
    recieved_msg += (char)message[i];
  }
}

void send_data(const char* buffer){
  mqtt.publish(TOPIC, buffer); // Publish the data
  mqtt.loop();
}