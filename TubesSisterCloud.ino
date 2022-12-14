#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>
#define LED 2
#define buzzer 15
#define api 25
#define gas 26
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "KP Sanak";
const char* password = "minangkabau99";

#define BOTtoken "5925192251:AAEOPqvHaNofCbGRlM53rKwAflD3e05MMzA"
#define CHAT_ID "1137528117"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  int sensor_api = digitalRead(api);
  int sensor_gas = analogRead(gas);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  String sensorgas, lembab, temp;
  sensorgas = String(sensor_gas);
  lembab = String(h);
  temp = String(t);
 
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
  
    String text = bot.messages[i].text;
    Serial.println(text);
  
    String from_name = bot.messages[i].from_name;
  
    if (text == "/mulai") {
      String welcome = "Selamat Datang, " + from_name + ".\n";
      welcome += "/gas untuk melihat kondisi gas sekitar \n";
      welcome += "/kelembaban untuk melihat kondisi kelembaban udara sekitar \n";
      welcome += "/temperatur untuk melihat temperatur sekitar \n";
      welcome += "/semua_kondisi untuk melihat semua kondisi \n";
      bot.sendMessage(chat_id, welcome, "");
    }
  
     if (text == "/gas") {
       bot.sendMessage(chat_id, sensorgas, "");
     }
     if (text == "/kelembaban") {
       bot.sendMessage(chat_id, lembab, "");
     }
     if (text == "/temperatur") {
       bot.sendMessage(chat_id, temp, "");
     }
     if (text == "/semua_kondisi") {
       bot.sendMessage(chat_id, sensorgas, "");
       bot.sendMessage(chat_id, lembab, "");
       bot.sendMessage(chat_id, temp, "");
     }
  
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(api, INPUT);
  pinMode(gas, INPUT);
  dht.begin();

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Klik /mulai untuk lanjut", "");
}

void loop() {
  int sensor_api = digitalRead(api);
  int sensor_gas = analogRead(gas);
  Serial.print(sensor_gas);       
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (sensor_api == LOW){
    bot.sendMessage(CHAT_ID, "KEBAKARAN!!!", "");
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
  }
  if (sensor_gas < 2500){
    bot.sendMessage(CHAT_ID, "Ada Gas Terdeteksi", "");
    digitalWrite(LED, HIGH);
    Serial.print(sensor_gas);
    digitalWrite(buzzer, HIGH);    
  }
  else{
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
  }
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  delay(1000);
  
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}