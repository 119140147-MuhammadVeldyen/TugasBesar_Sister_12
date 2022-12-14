#include <DHT.h>
#define LED 2
#define buzzer 15
#define api 25
#define gas 26
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(api, INPUT);
  pinMode(gas, INPUT);
  dht.begin();
}

void loop(){
  int sensor_api = digitalRead(api);
  int sensor_gas = analogRead(gas);       
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Fire Sensor: ");
  Serial.print(sensor_api);
  Serial.print("\t");
  Serial.print("Gas Sensor: ");
  Serial.print(sensor_gas);
  Serial.print("\t");
  Serial.print(F("Kelembaban: "));
  Serial.print(h);
  Serial.print(F("% Temperature: "));
  Serial.print(t);
  Serial.print(F("°C"));
  Serial.print("\t");
  
  if (sensor_api == LOW){
    Serial.println("Ada Api!");
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);
  }else{
    Serial.println("Tidak ada Api!");
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
  }if (sensor_gas < 2500){      
    Serial.println("Ada Gas!");
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);    
  }else{
    Serial.println("Tidak ada Gas!");
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
  }if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  delay(1000);
}