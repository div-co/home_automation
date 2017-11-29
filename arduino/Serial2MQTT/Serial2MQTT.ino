#include <Wire.h>
#include <UIPEthernet.h>
#include "PubSubClient.h"

#define MQTT_PORT 1883        // MQTT Server port

byte MQTT_server[] = { 10,55,80,200};      // IP Address of the MQTT server
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(10,55,84,30);
EthernetClient ethClient;
PubSubClient MQTT_Client(MQTT_server, MQTT_PORT, callback, ethClient);

char strConvert[10];
byte state_BarLights = 0;
byte state_CookerHoodLights = 0; 
byte state_SinkLights = 0; 
byte state_CookerHoodFan = 0;

int MQTT_Connect () 
{
      if (!MQTT_Client.connected()) {
          if (MQTT_Client.connect("Kitchen control_ler")) {
              MQTT_Client.publish("Arduino","Kitchen control_ler");
              
              //post initial state to MQTT brocker on connect
              itoa(state_CookerHoodLights,strConvert,5);
              MQTT_Client.publish("/lights/remote/CookerHoodLights",strConvert);
              itoa(state_BarLights,strConvert,5);
              MQTT_Client.publish("/lights/remote/BarLights",strConvert);
              itoa(state_SinkLights,strConvert,5);
              MQTT_Client.publish("/lights/remote/SinkLights",strConvert);
              itoa(state_SinkLights,strConvert,5);
              MQTT_Client.publish("/ventilation/remote/CookerHoodFan",strConvert);
              MQTT_Client.subscribe("/lights/hab/#");
              MQTT_Client.subscribe("/ventilation/hab/#");
              Serial.print("Connected to MQTT brocker at porrt: ")& Serial.print(MQTT_PORT)&Serial.println("\n");
              return 1;
          } else {
              Serial.println("Error connecting to MQTT\n");
              return 0;
         }
      } else return 1;
}

void callback(char* sub_topic, byte* payload, unsigned int length) 
{
      // ##### this function is the main part that triggers event based on the MQTT received messages. 

      char cPayload[30];
      for (int i=0; i<=length; i++) {
          cPayload[i]=(char)payload[i];
      }
      cPayload[length] = '\0';
      Serial.print("MQTT (callback):  ") & Serial.print(sub_topic) & Serial.print("  ") & Serial.println(cPayload);
 
      String pub_topic = String(cPayload);     
      
      int iPayload = atoi(cPayload);
      
      if (String (sub_topic) == "/lights/hab/BarLights") {
           if (iPayload == 0) {
              //state_BarLights = LOW;
              //digitalWrite(control_BarLights, state_BarLights);
           } else { 
              //state_BarLights = HIGH;
              //digitalWrite(control_BarLights, state_BarLights);
           }
      }
      if (String (sub_topic) == "/lights/hab/CookerHoodLights") {
           if (iPayload == 0) {
              //state_CookerHoodLights = LOW;
              //digitalWrite(control_CookerHoodLights, state_CookerHoodLights);
           } else { 
              //state_CookerHoodLights = HIGH;
              //digitalWrite(control_CookerHoodLights, state_CookerHoodLights);
           }
      }
      if (String (sub_topic) == "/lights/hab/SinkLights") {
           if (iPayload == 0) {
              //state_SinkLights = LOW;
              //digitalWrite(control_SinkLights, state_SinkLights);
           } else { 
              //state_SinkLights = HIGH;
              //digitalWrite(control_SinkLights, state_SinkLights);
           }
      }
      if (String (sub_topic) == "/ventilation/hab/CookerHoodFan") {
           if (iPayload == 0) {
             //state_CookerHoodFan = LOW;
             //digitalWrite(control_CookerHoodFan, state_CookerHoodFan);
           }else{
             //state_CookerHoodFan = HIGH;
             //digitalWrite(control_CookerHoodFan, state_CookerHoodFan);
           }
      }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

void setup() {
  Wire.begin(2);                // join i2c bus with address #2
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(115200);           // start serial for output
  MQTT_Connect();
}

void loop() {
  MQTT_Client.loop();
  delay(100);
}

