/* 

By : Rinto Harahap

Modul IoT 101
NodeMCU ESP8266

Date : 11/2022

*/

#define BLYNK_TEMPLATE_ID "TMPL1ov1xbJZ"
#define BLYNK_DEVICE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "bG6ROuRQ4qPRS_3TOy3l5__kOvtGR7-4"

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

const char* ALL_SSID[] = {"RH", "realme", "ENGINEERING.TR 7", "PUBLIC_TR7", "99"};
const char* ALL_PASS[] = {"makassar92", "makassar", "TIDAKKUTAUKI", "indihom3nkri", "FMSULB4GSEL"};

const int SSID_TERBACA = sizeof(ALL_SSID) / sizeof(ALL_SSID[0]);
#define trigPin D1
#define echoPin D2
#define LED_Merah D6
#define Alarm_LED D7
#define LED_Start D8
#define BUZZ D0
#define DHT_Sens D5
#define RelayA D3
#define RelayB D4

BlynkTimer timer;

WidgetLED led1(V6);
WidgetLCD lcd(V7);


long durasi;
long jarak;


void setup()
{

  boolean wifiFound = false;
  int i, n;

  // Debug console
  Serial.begin(115200);

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
  
  pinMode(RelayB, OUTPUT);
  pinMode(RelayA, OUTPUT);  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(DHT_Sens, INPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(LED_Merah, OUTPUT);
  pinMode(Alarm_LED, OUTPUT);
  pinMode(LED_Start, OUTPUT);

  Startup();

}


BLYNK_WRITE(V0) {
    if (param.asInt() == 1)
    {        
        digitalWrite(RelayA, LOW);
    }
    else
    {        
        digitalWrite(RelayA, HIGH);
    }
}

BLYNK_WRITE(V1)
{
    if (param.asInt() == 1)
    {      
        digitalWrite(RelayB, LOW);   
    }
    else
    {
        digitalWrite(RelayB, HIGH);        
    }
}

BLYNK_WRITE(V4)
{
    if (param.asInt() == 1)
    {    
        digitalWrite(LED_Merah, HIGH);
                
    }
    else
    {        
        digitalWrite(LED_Merah, LOW);   
    }
}



 BLYNK_WRITE(V8)
{
  int pinValue = param.asInt();

  if(pinValue >= 45 ){
    
    Blynk.virtualWrite(V2, pinValue);
    Blynk.setProperty(V2, "color", "#FF0000");
    Blynk.logEvent("notifku","Suhu diatas 40 Derajat celcius");

  }else{
    Blynk.virtualWrite(V2, pinValue);
    Blynk.setProperty(V2, "color", "#2EFF26");
    
  }
    Blynk.virtualWrite(V3, 70);

}


void loop() {

  if (digitalRead(LED_Merah) == HIGH)  {
 
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    
    durasi = pulseIn(echoPin, HIGH);
  
    
    jarak = durasi*0.034/2;
  
    
    Blynk.virtualWrite(V5, jarak);   

    
    lcd.print (2, 0, "Makassar.012");
    lcd.print (1, 1, "Smart Home 0.1");
    timer.run();
    logic();

    Serial.print("Durasi: ");
    Serial.println(durasi);
    Serial.print("Jarak: ");
    Serial.print(jarak);
    Serial.print (" cm");
    Serial.println (" ");
    delay(100);
  }
  else  {
    digitalWrite(LED_Merah, LOW);
    Serial.println("Sistem OFF");
    delay(1000);
  }

   Blynk.run();

}

void logic() {
  if ( jarak <= 5) {

    Blynk.setProperty(V6, "color", "#FF0000");
    Blynk.setProperty(V5, "color", "#FF0000");      
    led1.on();
      digitalWrite(BUZZ, HIGH);
      digitalWrite(Alarm_LED, HIGH);
      delay(60);
      digitalWrite(BUZZ, LOW);
      digitalWrite(Alarm_LED, LOW);
      delay(30);

    Serial.println(" -5cm");
  }

  else if (( jarak > 5 ) && ( jarak < 15)) {

    Blynk.setProperty(V6, "color", "#FFE600");
    Blynk.setProperty(V5, "color", "#FFE600");
    led1.on();
      digitalWrite(RelayA, LOW);
      digitalWrite(BUZZ, HIGH);
      digitalWrite(Alarm_LED, HIGH);
      delay(400);
      digitalWrite(BUZZ, LOW);
      digitalWrite(Alarm_LED, LOW);
      delay(400);
    
    Serial.println ( " 6 - 15cm");

  }

  else if ( jarak >= 15) {
    Blynk.setProperty(V6, "color", "#2EFF26");
    Blynk.setProperty(V5, "color", "#2EFF26");
    digitalWrite(RelayA, HIGH);
    led1.on();
    delay(200);
    Serial.println ( " 15cm keatas");

  }
}


void sendSensor() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
    
  durasi = pulseIn(echoPin, HIGH);
    
  jarak = durasi*0.034/2;
  
  Blynk.virtualWrite(V5, jarak);     
}



void Startup()  {

  for(int i = 0; i <= 3; i++){
    digitalWrite(LED_Start, HIGH);
    digitalWrite(BUZZ, HIGH);
    delay(500);
    digitalWrite(LED_Start, LOW);
    digitalWrite(BUZZ, LOW);
    delay(500);
  }
}



/* Progess
void sendDHT(){
  
  int sensorDHT = digitalRead(DHT_Sens) + 38;

}
*/






