
#include <ESP8266WiFi.h>
#include <MQTTClient.h>

int RelayPin= D2; 
/*const char* ssid = "ATT2.4";
const char* password = "Srirama7*";*/
const char* ssid = "aftab";
const char* password = "abgoosht";
const char* mqtt_server = "ec2-54-205-193-143.compute-1.amazonaws.com";

WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.begin(mqtt_server, net);
  connect();
  client.publish("/switch", "initialised");
  pinMode(RelayPin,OUTPUT);
}
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}


void loop() {
  if (!client.connected()) {
    connect();
  }
          char str[150];
          // read the input on analog pin 0:
          int sensorValue = analogRead(A0);
          // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
          float voltage = sensorValue * (5.0 / 1023.0);
          // print out the value you read:
          
        //dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
    
          dtostrf(voltage, 0, 2, str);
          Serial.println(voltage);
          if (voltage<1.0)
          {
            client.publish("/photo","dark ");
            digitalWrite(RelayPin,HIGH);
            
          }
          else
          {
            client.publish("/photo","day light sufficient ");
             digitalWrite(RelayPin,LOW);
          }
           
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}
