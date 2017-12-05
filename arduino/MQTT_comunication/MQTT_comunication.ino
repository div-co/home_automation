#include <UIPEthernet.h>
#include "PubSubClient.h"

#define MQTT_PORT 1883        // MQTT Server port
const char OFF = '0';
const char ON = '1';
const char* VENTILATION_HAB = "/ventilation/hab/#";
const char* LIGHTS_HAB = "/lights/hab/#";
const char* VENTILATION_REMOTE_COOKER_HOOD_FAN = "/ventilation/remote/CookerHoodFan";
const char* LIGHTS_REMOTE_SINK_LIGHTS = "/lights/remote/SinkLights";
const char* LIGHTS_REMOTE_BAR_LIGHTS = "/lights/remote/BarLights";
const char* LIGHTS_REMOTE_COOKER_HOOD_LIGHTS = "/lights/remote/CookerHoodLights";
const char* LIGHTS_HAB_SINK_LIGHTS = "/lights/hab/SinkLights";
const char* LIGHTS_HAB_BAR_LIGHTS = "/lights/hab/BarLights";
const char* LIGHTS_HAB_COOKER_HOOD_LIGHTS = "/lights/hab/CookerHoodLights";
const char* VENTILATION_HAB_COOKER_HOOD_FAN = "/ventilation/hab/CookerHoodFan";

byte MQTT_server[] = { 192, 168, 50, 7 };      // IP Address of the MQTT server
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 50, 200);

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

void MQTT_Connect() {
    if (!MQTT_Client.connected()) {
        if (MQTT_Client.connect("Kitchen controller")) {
            MQTT_Client.publish("Arduino", "Kitchen controller");

            //post initial state to MQTT brocker on connect
            itoa(state_CookerHoodLights, strConvert, 5);
            MQTT_Client.publish(LIGHTS_REMOTE_COOKER_HOOD_LIGHTS, strConvert);
            itoa(state_BarLights, strConvert, 5);
            MQTT_Client.publish(LIGHTS_REMOTE_BAR_LIGHTS, strConvert);
            itoa(state_SinkLights, strConvert, 5);
            MQTT_Client.publish(LIGHTS_REMOTE_SINK_LIGHTS, strConvert);
            itoa(state_CookerHoodFan, strConvert, 5);
            MQTT_Client.publish(VENTILATION_REMOTE_COOKER_HOOD_FAN, strConvert);

            MQTT_Client.subscribe(LIGHTS_HAB);
            MQTT_Client.subscribe(VENTILATION_HAB);
            //Serial.print("Connected to MQTT brocker at porrt: ")& Serial.print(MQTT_PORT)&Serial.println("\n");
        } else {
            Serial.println("Error connecting to MQTT\n");
        }
    }
}

void updateStateFromMQTT(byte pin, unsigned int length, byte* payload) {
    for (int i = 0; i < length; i++) {
        char receivedChar = (char) (payload[i]);
        if (receivedChar == ON)
            digitalWrite(pin, HIGH);

        if (receivedChar == OFF)
            digitalWrite(pin, LOW);
    }
}

void callback(char* sub_topic, byte* payload, unsigned int length) {
    // ##### this function is the main part that triggers event based on the MQTT received messages. All the important input stuff is here...

    char cPayload[30];
    for (int i = 0; i <= length; i++) {
        cPayload[i] = (char) payload[i];
    }

    cPayload[length] = '\0';
    Serial.print("MQTT (callback):  ") & Serial.print(sub_topic) & Serial.print("  ") & Serial.println(cPayload);

    // тези две променливи не виждам да ги ползваш някъде
//      String pub_topic = String(cPayload);
//      int iPayload = atoi(cPayload);
    if (strcmp(sub_topic, VENTILATION_HAB_COOKER_HOOD_FAN) == 0) {
        updateStateFromMQTT(control_CookerHoodFan, length, payload);
    }

    if (strcmp(sub_topic, LIGHTS_HAB_COOKER_HOOD_LIGHTS) == 0) {
        updateStateFromMQTT(control_CookerHoodLights, length, payload);
    }

    if (strcmp(sub_topic, LIGHTS_HAB_BAR_LIGHTS) == 0) {
        updateStateFromMQTT(control_BarLights, length, payload);
    }

    if (strcmp(sub_topic, LIGHTS_HAB_SINK_LIGHTS) == 0) {
        updateStateFromMQTT(controcontrol_SinkLightsl_BarLights, length, payload);
    }
}

void setup() {
    Serial.begin(115200);
    Ethernet.begin(mac, ip);
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

    Serial.print("IP Address: ") & Serial.println(Ethernet.localIP());
    MQTT_Connect();
}

void buttonControl(byte* state, byte control, const char* topic, byte active_mqtt) {
    *state = !*state;
    digitalWrite(control, state);
    if (active_mqtt) {
        char strConvert[10];
        itoa(state, strConvert, 5);
        MQTT_Client.publish(topic, strConvert);
    }
    return 0;
}

void pushButtonHandler(byte active_mqtt) {
    switch (pushButton_LightsCounter) {
        case 1: // pushButton_LightsCounter = 1
            buttonControl(&state_CookerHoodLights, control_CookerHoodLights, LIGHTS_REMOTE_COOKER_HOOD_LIGHTS, active_mqtt);
            pushButton_LightsCounter = 0;
            break;
        case 2: // pushButton_LightsCounter = 2
            buttonControl(&state_SinkLights, control_SinkLights, LIGHTS_REMOTE_SINK_LIGHTS, active_mqtt);
            pushButton_LightsCounter = 0;
            break;
        case 3: // pushButton_LightsCounter = 3
            buttonControl(&state_BarLights, control_BarLights, LIGHTS_REMOTE_BAR_LIGHTS, active_mqtt);
            pushButton_LightsCounter = 0;
            break;
        case 4:
            Serial.println("MQTT Server Connection Status Check ... ");
            if (!active_mqtt) {
                digitalWrite(control_SinkLights, !digitalRead(control_SinkLights));
                delay(1000);
                digitalWrite(control_SinkLights, !digitalRead(control_SinkLights));
            } else {
                digitalWrite(control_SinkLights, !digitalRead(control_SinkLights));
            }
            pushButton_LightsCounter = 0;
            break;
        case 5:
            Serial.println("Reconnecting MQTT Server ... ");
            digitalWrite(control_SinkLights, !digitalRead(control_SinkLights));
            delay(1000);
            digitalWrite(control_SinkLights, !digitalRead(control_SinkLights));
            Enc28J60.init(mac);
            MQTT_Connect();
            Ethernet.maintain();
            if (!active_mqtt) {
                byte counter = 0;
                for (int ii = 0; ii <= 5; ii++) {
                    digitalWrite(control_SinkLights, !digitalRead(control_SinkLights));
                    delay(300);
                    counter++;
                }
            }
            digitalWrite(control_SinkLights, LOW);
            pushButton_LightsCounter = 0;
            break;
        default:
            pushButton_LightsCounter = 0;
    }
}

void loop() {
    MQTT_Client.loop();
    byte active_mqtt = MQTT_Client.connected();

    byte readingFan = digitalRead(pushButton_Fan); // read the state of the switch into a local variable
    if (readingFan != lastButtonState_Fan) {
        //      ТУКА НЕ ТРЯБВА ЛИ ДА Е lastDebounceTime = millis() вместо lastButtonState_Fan?
        //      Това с вентилатора не ми изглежда да работи много надеждно...
        lastButtonState_Fan = millis(); // reset the debouncing timer
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (readingFan != pushButton_Fan_State) {
            pushButton_Fan_State = readingFan;
            if (pushButton_Fan_State == HIGH) {
                state_CookerHoodFan = !state_CookerHoodFan;
                digitalWrite(control_CookerHoodFan, state_CookerHoodFan);
                //Serial.print("Fan: ")& Serial.println(state_CookerHoodFan);
                itoa(state_CookerHoodFan, strConvert, 5);
                if (active_mqtt) {
                    MQTT_Client.publish(VENTILATION_REMOTE_COOKER_HOOD_FAN, strConvert);
                }
            }
        }
    }
    lastButtonState_Fan = readingFan;

    byte reading = digitalRead(pushButton_Lights); // read the state of the switch into a local variable
    if (reading != lastButtonState_Lights) {
        lastDebounceTime = millis(); // reset the debouncing timer
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != pushButton_Lights_State) {
            pushButton_Lights_State = reading;
            if (pushButton_Lights_State == HIGH) {
                if (pushButton_LightsCounter == 0) {
                    startingTime = millis();
                }
                pushButton_LightsCounter++; // count button pushes if 'timeout' is not reached
            }
        }
    }
    lastButtonState_Lights = reading;

    if (millis() - startingTime > timeout) {
        pushButtonHandler(active_mqtt);
    }
}
