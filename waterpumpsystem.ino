/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP8266     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 //D4
OneWire oneWire(ONE_WIRE_BUS);
#include <Wire.h>
#include <Adafruit_ADS1015.h>
 
Adafruit_ADS1115 ads(0x48);
DallasTemperature sensors(&oneWire);
byte ldr= A0;
int nilai;
 float Celcius=0;
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "muhiqbal2019"
#define AIO_KEY  "6503a8c609e344f9a7aeafc4b90ee624"
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe waterpump = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/waterpump");
Adafruit_MQTT_Publish suhuair = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/suhuair");
Adafruit_MQTT_Publish cuaca = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/cuaca");
Adafruit_MQTT_Publish tanah = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/tanah");
boolean MQTT_connect();

boolean MQTT_connect() {  int8_t ret; if (mqtt.connected()) {    return true; }  uint8_t retries = 3;  while ((ret = mqtt.connect()) != 0) { mqtt.disconnect(); delay(2000);  retries--;if (retries == 0) { return false; }} return true;}

int  pump1;

void setup()
{
  Serial.begin(9600);
  ads.begin();
pinMode(14, OUTPUT); //D5
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
   WiFi.begin("SSID","Password");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP().toString()));

  mqtt.subscribe(&waterpump);



}


void loop()
{
  nilai= analogRead(ldr);
  int16_t adc0;
  adc0 = ads.readADC_SingleEnded(0)/26.502008032;
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
    if (MQTT_connect()) {
      
       if(nilai >= 200&& nilai <=500){
    if (cuaca.publish("Redup")) {
       Serial.print(" Nilai LDR : ");
  Serial.print(nilai);
        Serial.println(" Data sent! ");
      delay(3000);
      } else {
        Serial.println("Data Not Sent");
delay(3000);
      }
  }
  if(nilai < 200){
    if (cuaca.publish("Terang")) {
        Serial.print(" Nilai LDR : ");
  Serial.print(nilai);
        Serial.println(" Data sent! ");
        delay(3000);
      } else {
        Serial.println("Data Not Sent");
delay(3000);
      }
  }
  if(nilai > 500){
    if (cuaca.publish("Gelap")) {
        Serial.print(" Nilai LDR : ");
  Serial.print(nilai);
        Serial.println(" Data sent! ");
        delay(3000);
      } else {
        Serial.println("Data Not Sent");
delay(3000);
      }
  }
  
       if (suhuair.publish(Celcius)) {
         Serial.print(" Nilai Suhu: ");
  Serial.print(Celcius);
        Serial.println(" Data sent! ");
      delay(3000);
      } else {
        Serial.println("Data Not Sent");
delay(3000);
      }
      
      if(adc0 < 350){
    if (tanah.publish("Tanah Lembab")) {
        Serial.print(" Nilai Kelembaban Tanah : ");
  Serial.print(adc0);
    Serial.print(ads.readADC_SingleEnded(0));
        Serial.println(" Data sent! ");
        delay(3000);
      } else {
        Serial.println("Data Not Sent");
delay(3000);
      }
  }
   if(adc0 >= 350 && adc0<700){
    if (tanah.publish("Tanah Normal")) {
        Serial.print(" Nilai Kelembaban Tanah : ");
  Serial.print(adc0);
  
    Serial.print(ads.readADC_SingleEnded(0));
        Serial.println(" Data sent! ");
        delay(3000);
      } else {
        Serial.println("Data Not Sent");
delay(3000);
      }
  }
   if(adc0 >= 700){
    if (tanah.publish("Tanah Kering")) {
        Serial.print(" Nilai Kelembaban Tanah : ");
  Serial.print(adc0);
    Serial.print(ads.readADC_SingleEnded(0));
        Serial.println(" Data sent! ");
        delay(3000);
      } else {
        Serial.println("Data Not Sent");
delay(3000);
      }
  }
       
      Adafruit_MQTT_Subscribe *subscription_name;
      while ((subscription_name = mqtt.readSubscription(5000))) {
        if (subscription_name == &waterpump) {
          Serial.println(((char *)waterpump.lastread));
          pump1 = atof((char *)waterpump.lastread);
          if (pump1 == 1) {
            digitalWrite(14,HIGH);

          } else {
            digitalWrite(14,LOW);

          }

        }
      

      }

    }

}
