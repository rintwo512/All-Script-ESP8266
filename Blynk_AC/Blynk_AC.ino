/* 

By : Rinto Harahap

Modul IoT 101
NodeMCU ESP8266

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

char auth[] = BLYNK_AUTH_TOKEN;

const char* ALL_SSID[] = {"RH", "realme", "ENGINEERING.TR 7", "PUBLIC_TR7", "404 - Not Found!"};
const char* ALL_PASS[] = {"1234567890", "makassar", "TIDAKKUTAUKI", "indihom3nkri", "Makassar_2015"};
const int SSID_TERBACA = sizeof(ALL_SSID) / sizeof(ALL_SSID[0]);
const int IR_LED_PIN = 4;

IRsend irsend(IR_LED_PIN);

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

// void setTime(int timezone)
// {
  
//   Serial.print("[TIME] : Setting time using SNTP \n");

//   configTime(timezone * 3600, 0, "id.pool.ntp.org");
//   time_t now = time(nullptr);
//   while (now < 1000)
//   {
//     delay(500);
//     Serial.print(".");
//     now = time(nullptr);
//   }
//   struct tm timeinfo;
//   gmtime_r(&now, &timeinfo);
//   Serial.printf("\n[TIME] : Current time: %s \n", asctime(&timeinfo));
// }


void setup()
{

  pinMode(IR_LED_PIN, OUTPUT);
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
  
 uint16_t rawData[349] = {
    648, 17784, 3032, 8932, 534, 460, 528, 1462, 552, 444, 556, 464, 524, 470, 530, 436, 552, 442, 558, 436,
    552, 442, 558, 1432, 558, 464, 524, 470, 530, 1432, 558, 438, 550, 470, 530, 1458, 530, 1458, 554, 1436,
    554, 1434, 556, 1434, 554, 440, 560, 462, 526, 468, 532, 462, 526, 468, 532, 462, 528, 466, 534, 460,
    528, 466, 522, 472, 528, 466, 522, 472, 588, 408, 556, 436, 552, 442, 558, 464, 526, 442, 558, 436,
    552, 442, 558, 436, 552, 442, 558, 464, 524, 470, 530, 464, 524, 442, 558, 438, 550, 470, 530, 464,
    524, 470, 530, 464, 524, 470, 554, 440, 548, 1440, 560, 1428, 560, 1428, 560, 1428, 560, 2934, 3034,
    8904, 550, 1438, 550, 444, 556, 438, 550, 472, 528, 466, 524, 470, 530, 464, 582, 412, 552, 442,
    558, 1430, 560, 436, 552, 442, 558, 1430, 560, 436, 552, 1436, 554, 1436, 554, 1436, 554, 1462, 528,
    1462, 528, 1462, 550, 444, 556, 438, 550, 444, 556, 438, 550, 444, 556, 438, 550, 444, 556, 438, 552,
    442, 556, 438, 550, 470, 530, 438, 550, 444, 556, 464, 524, 470, 530, 464, 524, 470, 578, 416, 584,
    410, 554, 440, 560, 434, 554, 440, 560, 434, 554, 440, 560, 434, 554, 440, 560, 434, 554, 440, 560,
    434, 554, 440, 560, 434, 554, 438, 560, 460, 528, 466, 534, 460, 528, 466, 558, 2910, 3058, 8906,
    560, 1430, 560, 434, 554, 440, 560, 434, 554, 440, 560, 434, 552, 440, 560, 434, 554, 468, 532, 1430,
    558, 462, 528, 466, 532, 462, 526, 1462, 586, 1404, 550, 1438, 550, 444, 556, 1432, 556, 1432, 556,
    1432, 556, 1432, 558, 1432, 558, 1458, 530, 1460, 530, 1460, 588, 406, 582, 412, 554, 440, 560, 1430,
    560, 1430, 560, 1430, 560, 434, 554, 440, 560, 434, 554, 468, 532, 434, 554, 468, 532, 462, 526, 468,
    532, 462, 526, 1462, 586, 408, 592, 1396, 558, 1432, 558, 436, 552, 444, 556, 438, 550, 442, 558, 436,
    586, 434, 566, 428, 560, 406, 594, 1396, 558, 1458, 530, 1458, 590, 1400, 554
};


  // Jumlah elemen dalam array rawData
  int rawDataSize = sizeof(rawData) / sizeof(rawData[0]);

  // Kirim data IR
  irsend.sendRaw(rawData, rawDataSize, 38);

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

  uint16_t rawData[349] = {
    648, 17794, 3034, 8930, 524, 470, 530, 1460, 552, 442, 558, 436, 552, 442, 558, 436, 552, 442, 560, 434,
    552, 442, 558, 1432, 558, 436, 552, 442, 558, 1430, 560, 1430, 558, 462, 526, 1464, 526, 1464, 550, 1440,
    550, 1438, 550, 1438, 550, 444, 556, 438, 550, 444, 556, 438, 550, 444, 556, 438, 550, 444, 556, 438,
    550, 470, 530, 464, 524, 470, 530, 464, 582, 412, 552, 442, 558, 436, 552, 442, 558, 436, 552, 442,
    558, 436, 552, 442, 558, 436, 554, 440, 560, 434, 552, 442, 558, 436, 552, 442, 558, 462, 526, 468,
    532, 462, 526, 468, 592, 402, 550, 444, 556, 438, 550, 444, 556, 1432, 556, 1434, 556, 2940, 3030,
    8908, 558, 1432, 558, 436, 552, 442, 558, 436, 552, 470, 530, 464, 524, 470, 530, 464, 584, 410, 554,
    1436, 554, 440, 560, 434, 554, 1436, 554, 440, 560, 1430, 560, 1430, 560, 1428, 560, 1430, 560, 1456,
    534, 1456, 534, 460, 586, 408, 556, 438, 550, 444, 556, 438, 550, 444, 558, 436, 550, 444, 556, 438,
    550, 444, 556, 438, 550, 444, 556, 438, 550, 444, 556, 438, 550, 470, 530, 464, 524, 470, 530, 464,
    582, 412, 554, 440, 560, 434, 554, 440, 560, 434, 556, 438, 560, 434, 554, 440, 560, 434, 554, 440,
    560, 434, 554, 440, 560, 434, 554, 440, 560, 460, 528, 466, 534, 460, 552, 2916, 3052, 8914, 552,
    1436, 552, 442, 558, 436, 552, 442, 558, 436, 552, 442, 558, 436, 552, 442, 558, 436, 552, 1436, 552,
    442, 558, 436, 552, 468, 532, 462, 526, 468, 592, 402, 586, 1404, 562, 1428, 550, 1440, 550, 1440,
    550, 1440, 550, 1440, 550, 1440, 550, 1440, 550, 1466, 522, 472, 528, 466, 522, 472, 552, 1438, 552,
    1438, 552, 1438, 552, 442, 558, 436, 552, 442, 558, 436, 552, 442, 558, 436, 552, 442, 558, 436, 552,
    442, 558, 1458, 532, 464, 524, 1464, 550, 1440, 562, 432, 556, 438, 562, 434, 554, 438, 562, 432,
    556, 438, 562, 432, 556, 438, 562, 434, 554, 438, 562, 1428, 550, 1466, 558
};


  // Jumlah elemen dalam array rawData
  int rawDataSize = sizeof(rawData) / sizeof(rawData[0]);

  // Kirim data IR
  irsend.sendRaw(rawData, rawDataSize, 38);

    Serial.println("OFF"); 
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
  if (param.asInt() == 17) {
    uint16_t rawData[233] = {
      644, 17868, 3090, 8874, 532, 462, 548, 1440, 586, 408, 590, 404, 584, 410, 556, 440, 584, 410, 580, 414,
      586, 410, 554, 1434, 590, 404, 584, 436, 564, 1426, 564, 430, 522, 472, 526, 1462, 552, 1438, 586, 1402,
      586, 1402, 586, 1402, 588, 408, 558, 436, 588, 406, 580, 414, 586, 408, 580, 440, 560, 434, 554, 440,
      524, 470, 530, 462, 526, 470, 554, 440, 548, 446, 578, 416, 584, 410, 590, 404, 584, 410, 590, 404,
      584, 410, 590, 404, 584, 410, 590, 404, 584, 410, 590, 432, 558, 436, 564, 430, 558, 436, 528, 466,
      522, 474, 550, 442, 580, 414, 586, 408, 580, 1408, 580, 1408, 580, 1408, 594, 1396, 580, 2914, 3054,
      8884, 582, 1434, 554, 438, 562, 434, 532, 462, 528, 468, 520, 474, 550, 444, 580, 414, 586, 408, 590,
      1398, 580, 414, 584, 410, 590, 1398, 580, 414, 586, 1404, 584, 1432, 558, 436, 552, 1438, 528, 1460,
      552, 1438, 588, 1402, 586, 1402, 588, 1402, 586, 1402, 588, 1402, 588, 406, 582, 412, 588, 406, 582,
      1434, 532, 1458, 532, 1458, 532, 462, 548, 446, 578, 416, 584, 410, 590, 404, 584, 1406, 584, 410,
      556, 438, 550, 446, 590, 1400, 590, 404, 584, 1432, 556, 1432, 558, 436, 528, 466, 546, 448, 586, 408,
      546, 448, 586, 408, 580, 414, 588, 406, 580, 1408, 580, 1408, 580, 1408, 580, 1408, 558
    };

    int rawDataSize = sizeof(rawData) / sizeof(rawData[0]);
    irsend.sendRaw(rawData, rawDataSize, 38);
  } else if (param.asInt() == 18) {
    uint16_t rawData[233] = {
      642, 15408,  3028, 8910,  558, 438,  550, 1466,  524, 470,  530, 466,  582, 412,  530, 464,  548, 446,  554, 440,  560, 434,  554,
      1436,  554, 440,  558, 436,  554, 1434,  554, 440,  560, 434,  552, 1436,  554, 1436,  554, 1462,  526, 1462,  526, 1462,  550, 444,  556, 438,  550, 444,  
      556, 440,  550, 444,  554, 438,  550, 444,  556, 438,  550, 444,  556, 438,  550, 444,  556, 438,  550, 444,  556, 466,  522, 472,  528, 466,  524, 470,  
      530, 464,  558, 436,  552, 442,  558, 436,  552, 442,  558, 436,  552, 442,  558, 436,  552, 442,  558, 436,  552, 442,  558, 436,  552, 442,  558, 436,  
      552, 468,  532, 1456,  532, 1456,  532, 1456,  556, 1434,  556, 2940,  3030, 8910,  556, 1432,  556, 438,  550, 444,  556, 438,  550, 444,  556, 438,  550,
       444,  556, 464,  522, 472,  528, 1460,  530, 466,  546, 448,  552, 1436,  554, 442,  558, 1430,  558, 1432,  558, 436,  552, 1436,  552, 1436,  554, 1434, 
        554, 1464,  526, 1462,  526, 1462,  550, 1438,  550, 1438,  550, 444,  554, 440,  548, 444,  556, 1434,  556, 1434,  556, 1434,  556, 438,  550, 444,  556, 
        438,  550, 472,  528, 466,  522, 472,  528, 1460,  552, 442,  558, 436,  552, 1436,  554, 442,  558, 1430,  558, 1430,  558, 436,  552, 442,  558, 436,  552, 
        442,  558, 436,  552, 468,  532, 462,  584, 410,  530, 1458,  556, 1434,  556, 1434,  556, 1434,  556
    };

    int rawDataSize = sizeof(rawData) / sizeof(rawData[0]);
    irsend.sendRaw(rawData, rawDataSize, 38);
  }
}


void loop() {


  lcd.print (2, 0, "Makassar.012");
  lcd.print (1, 1, "Smart Home 1.0");

  // time_t now = time(nullptr);
  // struct tm* p_timeinfo = localtime(&now);
  // Serial.printf("[NOW]  : Time = %s \n", asctime(p_timeinfo));
  
  // if((p_timeinfo->tm_hour == 17) && (p_timeinfo->tm_min == 59)) {
    
  // }

  // if((p_timeinfo->tm_hour == 5) && (p_timeinfo->tm_min == 59)) {
   
    
  // }

  // if((p_timeinfo->tm_hour == 18) && (p_timeinfo->tm_min == 59)) {
   
  // }

  // if((p_timeinfo->tm_hour == 19) && (p_timeinfo->tm_min == 59)) {
   
    
  // }

  delay(1000);

   Blynk.run();
   timer.run();
   

}








