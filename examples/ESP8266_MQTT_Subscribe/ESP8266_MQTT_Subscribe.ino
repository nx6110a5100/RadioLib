/*
 * KiteLib ESP8266 MQTT Subscribe Example
 * 
 * This example subscribes to MQTT topic using ESP8266 WiFi module.
 * 
 * The messages are pulled from https://shiftr.io/try. You can use this namespace
 * for testing purposes, but remember that it is publicly accessible!
 * 
 * IMPORTANT: Before uploading this example, make sure that the ESP8266 module is running
 * AT firmware (can be found in the /extras folder of the library)!
 */

// include the library
#include <KiteLib.h>

// ESP8266 module is in slot A on the shield
ESP8266 wifi = Kite.ModuleA;

// create MQTT client instance using the wifi module
// the default port used for MQTT is 1883
MQTTClient mqtt(&wifi, 1883);

void setup() {
  Serial.begin(9600);

  // initialize ESP8266
  Serial.print(F("[ESP8266] Initializing ... "));
  // baudrate:  9600 baud
  byte state = wifi.begin(9600);
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code 0x"));
    Serial.println(state, HEX);
    while(true);
  }

  // join access point
  Serial.print(F("[ESP8266] Joining AP ... "));
  // name:      SSID
  // password:  password
  state = wifi.join("SSID", "password");
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code 0x"));
    Serial.println(state, HEX);
    while(true);
  }

  // connect to MQTT server
  Serial.print(F("[ESP8266] Connecting to MQTT server ... "));
  // server URL:  broker.shiftr.io
  // client ID:   arduino
  // username:    try
  // password:    try
  state = mqtt.connect("broker.shiftr.io", "arduino", "try", "try");
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code 0x"));
    Serial.println(state, HEX);
    while(true);
  }

  // subscribe to MQTT topic
  // after calling this method, server will send PUBLISH packets
  // to this client each time a new message was published at the topic
  Serial.print(F("[ESP8266] Subscribing to MQTT topic ... "));
  // topic name:  hello
  state = wifi.subscribe("hello");
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
    } else {
    Serial.print(F("failed, code 0x"));
    Serial.println(state, HEX);
  }

  // unsubscribe from MQTT topic
  // after calling this method, server will stop sending PUBLISH packets
  Serial.print(F("[ESP8266] Unsubscribing from MQTT topic ... "));
  // topic filter:  hello
  state = wifi.unsubscribe("hello");
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
    } else {
    Serial.print(F("failed, code 0x"));
    Serial.println(state, HEX);
  }
}

// create a function that will be called when a new PUBLISH packet
// arrives from the server
//
// IMPORTANT: This function MUST have two C-strings as arguments!
void onPublish(const char* topic, const char* message) {
  Serial.println("[ESP8266] Received packet from MQTT server: ");
  Serial.print("[ESP8266] Topic:\t");
  Serial.println(topic);
  Serial.print("[ESP8266] Message:\t");
  Serial.println(message);
}

void loop() {
  // check for new MQTT packets from server each time the loop() runs
  // this will also send a PING packet, restarting the keep alive timer
  byte state = wifi.check(onPublish);
  Serial.print("[ESP8266] MQTT check ");
  if(state == ERR_NONE) {
    Serial.println(F("success!"));
    } else {
    Serial.print(F("failed, code 0x"));
    Serial.println(state, HEX);
  }

  // the rest of your loop() code goes here
  // make sure that the maximum time the loop() runs is less than 1.5x keep alive,
  // otherwise the server will close the network connection
}

