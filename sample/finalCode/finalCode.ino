#include "UbidotsMicroESP8266.h"
#include "DHT.h"
#define DHTPIN D4    // what digital pin we're connected to dht11 sensor
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

#define humidity "591f57bf762542541c68df54"  //  Ubidots variable ID humidity
#define temparature  "591f5794762542541c68dc55"  //  Ubidots variable ID temparature
#define relay "591f57ac762542541a7fe68f"  //  Ubidots variable ID relay
#define UbidotsAccessKey "r4NrrWCRWcNqYVzpW4VkdXrFi5Kqon"  // Ubidots reference api token

#define WIFISSID "my"//wifi connection ssid
#define PASSWORD "kpmauran"//wifi password
#define RELAY D2// modemcu pin connected to 5v relay

Ubidots ubiclient(UbidotsAccessKey);
void setup()
{
    Serial.begin(115200);
    dht.begin();
    delay(10);
    pinMode(RELAY, OUTPUT);
    ubiclient.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();//read temparature
   Serial.println(t);
   if (isnan(h) || isnan(t)) {
        Serial.println("Un correct input");
        return;
    }
   // send temparature humidity values to ubiots platform
    ubiclient.add(humidity, h);
    ubiclient.add(temparature, t);
  
    ubiclient.sendAll();
    float setRelay = ubiclient.getValue(relay);
    if (setRelay == 1.00) {
       Serial.println(setRelay);
        digitalWrite(RELAY, HIGH); //On relay
    }
    else if (setRelay == 0.00) {
       Serial.println(setRelay);
        digitalWrite(RELAY, LOW); //Off relay
    }
}
