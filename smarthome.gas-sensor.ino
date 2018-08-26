#include <InfluxDb.h>
#include <ESP8266WiFi.h>

#define INFLUXDB_HOST "192.168.13.10"
#define INFLUXDB_DATABASE "sensor"

const char* ssid     = "orokmozgo";
const char* password = "";
char* mac = "n/a";

const int smokePIN = A0;
const int sensorThreshold = 400;

WiFiClient client;
Influxdb influx(INFLUXDB_HOST); // port defaults to 8086

void setup() {
  module_setup();
  
  Serial.begin(9600);
  delay(2000);

  WiFi.disconnect(true);
  delay(1000);
  
  Serial.println();
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
  Serial.println("MAC: ");
  WiFi.macAddress().toCharArray(mac, 18); 
  Serial.println(mac);  

  influx.setDb(INFLUXDB_DATABASE);
}

void module_setup()
{
  pinMode(smokePIN, INPUT);
}

void prepare_data(String valueType, float value)
{
  InfluxData measurement (valueType);
  measurement.addTag("device", mac);
  measurement.addTag("sensor", "dht11");
  measurement.addValue("value", value);
  
  influx.prepare(measurement);
}

void loop() {
  int analogSensor = analogRead(smokePIN);

  Serial.print("Sensor value: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThreshold)
  {
    Serial.println("Smoke detected!");
  }

//  prepare_data("temperature", t);
//  prepare_data("humidity", h);

//  influx.write();
  
  delay(100);
}
