#include <UIPEthernet.h>
#include "PubSubClient.h"

#define MANUAL_INTERVAL 9000  // Manual function check interval will be on each 15 min
#define MQTT_PORT 1883        // MQTT Server port

byte MQTT_server[] = { 192,168,50,7 };      // IP Address of the MQTT server
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192,168,50,200);

byte pushButton_LightsCounter = 0;           // counter for the number of button presses
unsigned long startingTime = 0;
char strConvert[10];

byte pushButton_Lights = 4;
byte pushButton_Fan = 3;
byte control_CookerHoodLights = 18;
byte control_CookerHoodFan = 6;
byte control_BarLights = 16;
byte control_SinkLights = 15;

byte state_BarLights = LOW;
byte state_CookerHoodLights = LOW;  // the current state of the pin
byte state_SinkLights = LOW;        // the current state of the pin
byte state_CookerHoodFan = LOW;
byte pushButton_Lights_State = 0;  // the current reading from the input pin
byte pushButton_Fan_State = 0;
byte lastButtonState_Lights = LOW; // the previous reading from the input pin
byte lastButtonState_Fan = LOW;

const unsigned long timeout = 1000;
long lastDebounceTime = 0;          // the last time the output pin was toggled
byte debounceDelay = 30;            // the debounce time; increase if the output flickers

EthernetClient ethClient;
void callback(char* sub_topic, byte* payload, unsigned int length);
PubSubClient MQTT_Client(MQTT_server, MQTT_PORT, callback, ethClient);

int MQTT_Connect () 
{
      if (!MQTT_Client.connected()) {
          if ( MQTT_Client.connect("Kitchen controller") ) {
              MQTT_Client.publish("Arduino","Kitchen controller");
              
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
              //Serial.print("Connected to MQTT brocker at porrt: ")& Serial.print(MQTT_PORT)&Serial.println("\n");
              return 1;
          } else {
              Serial.println("Error connecting to MQTT\n");
              return 0;
         }
      } else return 1;
}

void callback(char* sub_topic, byte* payload, unsigned int length) 
{
      // ##### this function is the main part that triggers event based on the MQTT received messages. All the important input stuff is here... 

      char cPayload[30];
      for (int i=0; i<=length; i++) {
        cPayload[i]=(char)payload[i];
      }
      
      cPayload[length] = '\0';
      Serial.print("MQTT (callback):  ") & Serial.print(sub_topic) & Serial.print("  ") & Serial.println(cPayload);
 
      String pub_topic = String(cPayload); 
      
      int iPayload = atoi(cPayload);
      
      //Serial.println(sub_topic);
      //Serial.println(cPayload);
      
      if(strcmp(sub_topic, "/ventilation/hab/CookerHoodFan") == 0) {
        for (int i=0;i<length;i++) {
          char receivedChar = (char)payload[i];
            if (receivedChar == '1')
              digitalWrite(control_CookerHoodFan, HIGH);
            if (receivedChar == '0')
              digitalWrite(control_CookerHoodFan, LOW);
            }
      }

      if(strcmp(sub_topic, "/lights/hab/CookerHoodLights") == 0) {
        for (int i=0;i<length;i++) {
          char receivedChar = (char)payload[i];
            if (receivedChar == '1')
              digitalWrite(control_CookerHoodLights, HIGH);
            if (receivedChar == '0')
              digitalWrite(control_CookerHoodLights, LOW);
            }
      }

            
      if(strcmp(sub_topic, "/lights/hab/BarLights") == 0) {
        for (int i=0;i<length;i++) {
          char receivedChar = (char)payload[i];
            if (receivedChar == '1')
              digitalWrite(control_BarLights, HIGH);
            if (receivedChar == '0')
              digitalWrite(control_BarLights, LOW);
            }
      }

      if(strcmp(sub_topic, "/lights/hab/SinkLights") == 0) {
        for (int i=0;i<length;i++) {
          char receivedChar = (char)payload[i];
            if (receivedChar == '1')
              digitalWrite(control_SinkLights, HIGH);
            if (receivedChar == '0')
              digitalWrite(control_SinkLights, LOW);
            }
      }
}

void setup()
{
      Serial.begin(115200);  
      Ethernet.begin(mac,ip);
      pinMode(pushButton_Lights, INPUT);
      pinMode(pushButton_Fan, INPUT);
      pinMode(control_CookerHoodFan, OUTPUT);
      pinMode(control_CookerHoodLights, OUTPUT);
      pinMode(control_SinkLights, OUTPUT);
      pinMode(control_BarLights, OUTPUT);
      
      digitalWrite(control_CookerHoodLights, state_CookerHoodLights); 
      digitalWrite(control_BarLights, state_BarLights);
      digitalWrite(control_SinkLights, state_SinkLights);
      digitalWrite(control_CookerHoodFan, state_CookerHoodFan);
      
      Serial.print("IP Address: ")& Serial.println(Ethernet.localIP());
      MQTT_Connect();      
}


void loop()
{
       MQTT_Client.loop();
       
       byte reading = digitalRead(pushButton_Lights); // read the state of the switch into a local variable 
       byte readingFan = digitalRead(pushButton_Fan); // read the state of the switch into a local variable 
       
       if (readingFan != lastButtonState_Fan) {
           lastButtonState_Fan = millis(); // reset the debouncing timer
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {
           if (readingFan != pushButton_Fan_State) {
              pushButton_Fan_State = readingFan;
              if (pushButton_Fan_State == HIGH) {
                  state_CookerHoodFan = !state_CookerHoodFan;
                  digitalWrite(control_CookerHoodFan, state_CookerHoodFan);
                  itoa(state_CookerHoodFan,strConvert,5);
                  if ( MQTT_Connect () ) { 
                        MQTT_Client.publish("/ventilation/remote/CookerHoodFan", strConvert);
                      }
                 }
              }
           }
           lastButtonState_Fan = readingFan;
          
       if (reading != lastButtonState_Lights) {
           lastDebounceTime = millis(); // reset the debouncing timer
       }
       if ((millis() - lastDebounceTime) > debounceDelay) {
            if (reading != pushButton_Lights_State) {
                pushButton_Lights_State = reading;
                if (pushButton_Lights_State == HIGH) {
                    if (pushButton_LightsCounter==0) { 
                      startingTime = millis(); 
                    }
                    pushButton_LightsCounter++; // count button pushes if 'timeout' is not reached
                }
             }
         }
         lastButtonState_Lights = reading;
       
         if(millis()-startingTime > timeout) {
     
          switch (pushButton_LightsCounter) {
          
             case 1: // pushButton_LightsCounter = 1
                  state_CookerHoodLights = !state_CookerHoodLights;
                  digitalWrite(control_CookerHoodLights, state_CookerHoodLights);
                  itoa(state_CookerHoodLights,strConvert,5);
                  if ( MQTT_Connect () ) { 
                      MQTT_Client.publish("/lights/remote/CookerHoodLights", strConvert); 
                   } else {
                    Enc28J60.init(mac);
                    MQTT_Connect();
                    }
                  pushButton_LightsCounter=0;
                  break;
              
              case 2: // pushButton_LightsCounter = 2
                  state_SinkLights = !state_SinkLights;
                  digitalWrite(control_SinkLights, state_SinkLights);
                  itoa(state_SinkLights,strConvert,5);
                  if ( MQTT_Connect () ) { 
                      MQTT_Client.publish("/lights/remote/SinkLights",strConvert); 
                   } else {
                    Enc28J60.init(mac);
                    MQTT_Connect();
                    }
                  pushButton_LightsCounter=0;
                break;  
              
              case 3: // pushButton_LightsCounter = 3
                  state_BarLights = !state_BarLights;
                  digitalWrite(control_BarLights, state_BarLights);
                  itoa(state_BarLights,strConvert,5);
                  if ( MQTT_Connect () ) { 
                      MQTT_Client.publish("/lights/remote/BarLights",strConvert); 
                   }else {
                    Enc28J60.init(mac);
                    MQTT_Connect();
                    }
                  pushButton_LightsCounter=0;
                break;

               case 4:
                Serial.println("Lights button pressed 4 times: Do nothing.");
                pushButton_LightsCounter=0;
               break;

               case 5:
                  Serial.println("Reconnecting MQTT Server ... ");
                  digitalWrite(control_SinkLights, !digitalRead(control_SinkLights));
                  delay(500);
                  digitalWrite(control_SinkLights, !digitalRead(control_SinkLights));
                  Enc28J60.init(mac);
                  MQTT_Connect();
                  Ethernet.maintain();
                  if ( !MQTT_Connect () ) {
                    byte counter = 0;
                    for(int ii = 0; ii <= 4; ii++) {
                      digitalWrite(control_SinkLights, !digitalRead(control_SinkLights) );
                      delay(300);
                      counter++;
                    }
                  } 
                  digitalWrite(control_SinkLights, LOW);
                  pushButton_LightsCounter=0;
                break;
              
              default:
                  pushButton_LightsCounter=0;
           } 
      } 
}
