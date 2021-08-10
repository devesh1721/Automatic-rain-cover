#include "ESP8266WiFi.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "AndroidAP"
#define WLAN_PASS       "098765432111"



#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "DEVESH"
#define AIO_KEY         "038bdc81de394b85abbf082221532aef"



WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


Adafruit_MQTT_Publish data = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/TXT");
Adafruit_MQTT_Publish data1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/TXT");


void MQTT_connect();
#include <Servo.h>
const int capteur_D = 4;
Servo myservo;      
int val_analogique;
int pos = 0;
void setup()
{
  pinMode(capteur_D, INPUT);
  pinMode(capteur_A, INPUT);
  myservo.attach(D2);
  Serial.begin(115200);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  MQTT_connect();
  data.publish("OFF");
}

void loop()
{
if(digitalRead(capteur_D) == LOW) 
  {
    Serial.println("Digital value : wet"); 
    myservo.write(180);
    data.publish(1);
    delay(10); 
  }
else
  {
    Serial.println("Digital value : dry");
    myservo.write(pos);
    data.publish(0);
    delay(10); 
  }

}
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
