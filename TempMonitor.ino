#include <ArduinoJson.h>  //ArduinoJson by Benoit Blanchon Version 6.13.0
#include <MQTT.h> 
#include <WiFi.h>
#include <DHTesp.h>

const char* ssid = "YourWiFi_Name";
const char* password = "YourWiFi_Password";
WiFiClient client;

const char* broker ="broker.hivemq.com";
const char* unique_id = "aeea3754-b8de-4806-8ef6-59f26d24c4bc";
String topic_subscribe = "Temp/iot"; 
String topic_publish = "Temp/iot";     
MQTTClient mqtt(1024);

const byte dhtPin = 25;
DHTesp dht;

unsigned long sysTick=0;
const unsigned int TIME_INTERVAL = 15000; //15 Sec

void setup() {
  Serial.begin(115200);
  dht.setup(dhtPin,DHTesp::DHT22);
  mqtt.begin(broker,1883,client);
  mqtt.onMessage(messageReveived);
  setup_wifi();
  setup_mqtt();
}
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//------------------------------------------------

//User Function - setup_mqtt()
//------------------------------------------------
void setup_mqtt(){
  while(!mqtt.connect(unique_id)){
    Serial.print(".");
    delay(500);
  }
  Serial.println("MQTT connected.");
  mqtt.subscribe(topic_subscribe);
}

void messageReveived(String &topic_subscribe, String &payload){
Serial.print("Incoming Topic:");
Serial.println(topic_subscribe);
Serial.print(", Payload");
Serial.println(payload);
//Decode DATA from JSON format
//e.g.: {"value":"on"}
DynamicJsonDocument doc(1024);
deserializeJson (doc,payload);
String value = doc["temperature"];
Serial.println(value);

//Application according to JSON Data
if(value <= "35"){
}
else if(value >= "36"){

}
}
//------------------------------------------------------------------

void loop() {
    //MQTT Running
  mqtt.loop();
  if(!mqtt.connected()){
    setup_mqtt();
  }

  //Publish Payload to MQTT every 15 Sec
if(millis()>sysTick){
   float temperature = dht.getTemperature();
    sysTick = millis()+TIME_INTERVAL;
        Serial.print("client_id: ");
        Serial.print("egg_incubator1");
        Serial.print("temperature:");
        Serial.println(temperature);

    StaticJsonDocument<200>data;
    data["client_id"]="egg_incubator1";
    data["temperature"]=temperature;
    
    //====Debuging Message=====
    serializeJson(data,Serial);
    Serial.println();
    serializeJsonPretty(data,Serial);
    Serial.println();
    //=========================
    
    String payload;
    serializeJson(data,payload);    

    mqtt.publish(topic_publish,payload);                        
  }
}


