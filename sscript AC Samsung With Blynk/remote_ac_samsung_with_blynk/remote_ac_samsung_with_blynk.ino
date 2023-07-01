/* 

By : Rinto Harahap

Modul IoT 101
NodeMCU ESP8266

refrensi : https://github.com/crankyoldgit/IRremoteESP8266/issues/1675

Date : 16/01/2023

*/

#define BLYNK_TEMPLATE_ID "TMPLcCYoLodH"
#define BLYNK_DEVICE_NAME "Smart Building 2"
#define BLYNK_AUTH_TOKEN "xUbK57JzIQG1gUzg_TURrciufEqIpLE_"

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <time.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Samsung.h>

char auth[] = BLYNK_AUTH_TOKEN;

const char* ALL_SSID[] = {"RH", "realme", "ENGINEERING.TR 7", "PUBLIC_TR7", "404 - Not Found!"};
const char* ALL_PASS[] = {"1234567890", "makassar", "TIDAKKUTAUKI", "indihom3nkri", "Makassar_2015"};
const int SSID_TERBACA = sizeof(ALL_SSID) / sizeof(ALL_SSID[0]);
const int IR_LED_PIN = 4;

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRSamsungAc ac(kIrLed);     // Set the GPIO used for sending messages.

#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

WidgetLCD lcd(V9);

void sendSensorDHT()
{
  int h = dht.readHumidity();
  int t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  if(t >= 40){
    Blynk.setProperty(V7, "color", "#FF0000");
    Blynk.logEvent("notifku2","Suhu diatas 35 Derajat celcius");   
  }else{
     Blynk.setProperty(V7, "color", "#2EFF26");
  }
  Blynk.virtualWrite(V8, h);
  Blynk.virtualWrite(V7, t);
 
}


void setup()
{

  ac.begin();  

  // pinMode(IR_LED_PIN, OUTPUT);
  pinMode(D6, OUTPUT);

  boolean wifiFound = false;
  int i, n;

  // Debug console
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Penyiapan selesai");

  Serial.println(F("Pindai mulai"));
  int nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println(F("Pemindaian Selesai"));
  if (nbVisibleNetworks == 0) {
  Serial.println(F("Tidak ada jaringan yang ditemukan. Setel ulang untuk mencoba lagi!"));
  while (true);
  }

  Serial.print(nbVisibleNetworks);
  Serial.println(" jaringan(s) ditemukan!");
  
  digitalWrite(D6, HIGH);


  for (i = 0; i < nbVisibleNetworks; ++i) {
  Serial.println(WiFi.SSID(i));
  for (n = 0; n < SSID_TERBACA; n++) {
  if (strcmp(ALL_SSID[n], WiFi.SSID(i).c_str())) {
  Serial.print(F("\tTidak cocok! "));
  Serial.println(ALL_SSID[n]);
  } else {
    wifiFound = true;
    break;
    }
  }
  if (wifiFound) break;
  }

  if (!wifiFound) {
  Serial.println(F("Tidak ada Jaringan Dikenal yang teridentifikasi. Setel ulang untuk mencoba lagi !"));
  while (true);

  }

  const char* ssid = (ALL_SSID[n]);
  const char* pass = (ALL_PASS[n]);
  Serial.println(WiFi.localIP());
  Blynk.begin(auth, ssid, pass);
  Serial.println("Blynk Connected");

  
  timer.setInterval(1000L, sendSensorDHT);
  // setTime(8);

}


BLYNK_WRITE(V0) {
  if (param.asInt() == 1)
  {        
  
    ac.on();
    ac.send();

    Blynk.setProperty(V0, "color", "#59981A");
   Blynk.setProperty(V1, "isDisabled", false);
   Blynk.setProperty(V2, "isDisabled", false);
   Blynk.setProperty(V3, "isDisabled", false);
   Blynk.setProperty(V4, "isDisabled", false);
   Blynk.setProperty(V5, "isDisabled", false);
   Blynk.setProperty(V6, "isDisabled", false);

  }
  else
  { 

    ac.off();
    ac.send();

    Blynk.setProperty(V2, "isDisabled", true); 
    Blynk.virtualWrite(V2, 0);
    Blynk.setProperty(V3, "isDisabled", true); 
    Blynk.virtualWrite(V3, 0);
    Blynk.setProperty(V4, "isDisabled", true); 
    Blynk.virtualWrite(V4, 0);
    Blynk.setProperty(V5, "isDisabled", true); 
    Blynk.virtualWrite(V5, 0);
    Blynk.setProperty(V6, "isDisabled", true); 
    Blynk.virtualWrite(V6, 0);
    Blynk.setProperty(V1, "isDisabled", true); 
  }
}

BLYNK_WRITE(V1) {
  if (param.asInt() == 16) {
      ac.setTemp(16);
      ac.send();
  } else if (param.asInt() == 17) {
      ac.setTemp(17);
      ac.send();
  }else if(param.asInt() == 18){
      ac.setTemp(18);
      ac.send();
  }else if(param.asInt() == 19){
      ac.setTemp(19);
      ac.send();
  }else if(param.asInt() == 20){
      ac.setTemp(20);
      ac.send();
  }else if(param.asInt() == 21){
      ac.setTemp(21);
      ac.send();
  }else if(param.asInt() == 22){
      ac.setTemp(22);
      ac.send();
  }else if(param.asInt() == 23){
      ac.setTemp(23);
      ac.send();
  }else if(param.asInt() == 24){
      ac.setTemp(24);
      ac.send();
  }else if(param.asInt() == 25){
      ac.setTemp(25);
      ac.send();
  }else if(param.asInt() == 26){
      ac.setTemp(26);
      ac.send();
  }else if(param.asInt() == 27){
      ac.setTemp(27);
      ac.send();
  }else if(param.asInt() == 28){
      ac.setTemp(28);
      ac.send();
  }else if(param.asInt() == 29){
      ac.setTemp(29);
      ac.send();
  }else if(param.asInt() == 30){
      ac.setTemp(30);
      ac.send();
  }
}


BLYNK_WRITE(V2) {

  if(param.asInt() == 1){
      ac.setFan(kSamsungAcFanTurbo);
      ac.setMode(kSamsungAcCool);
      ac.send();
      Blynk.virtualWrite(V3, 0);      
      Blynk.virtualWrite(V4, 0);     
  }

}

BLYNK_WRITE(V3) {

  if(param.asInt() == 1){
      ac.setFan(kSamsungAcFanMed);
      ac.setMode(kSamsungAcFan);
      ac.send();
      Blynk.virtualWrite(V2, 0);      
      Blynk.virtualWrite(V4, 0);
  }

}

BLYNK_WRITE(V4) {

  if(param.asInt() == 1){
      ac.setFan(kSamsungAcFanMed);
      ac.setMode(kSamsungAcDry);
      ac.send();
      Blynk.virtualWrite(V3, 0);      
      Blynk.virtualWrite(V2, 0);
  }

}


BLYNK_WRITE(V5) {

  if(param.asInt() == 1){
      ac.setFan(kSamsungAcFanAuto);
      ac.send();
      Blynk.virtualWrite(V6, 0);      
  }

}

BLYNK_WRITE(V6) {

  if(param.asInt() == 1){
      ac.setFan(kSamsungAcFanHigh);
      ac.send();
      Blynk.virtualWrite(V5, 0);      
  }

}


void loop() {


  lcd.print (2, 0, "Makassar.012");
  lcd.print (1, 1, "Smart Home 1.0");

   Serial.print("ION: ");
  Serial.println(ac.getIon());
  Serial.print("Power: ");
  Serial.println(ac.getPower());
  Serial.print("Temp: ");
  Serial.println(ac.getTemp());
  Serial.print("PowerFull: ");
  Serial.println(ac.getPowerful());
  

  delay(1000);

   Blynk.run();
   timer.run();
   

}








