

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>



const char* ALL_SSID[] = {"RH", "realme", "ENGINEERING.TR 7", "PUBLIC_TR7", "404 - Not Found!"};
const char* ALL_PASS[] = {"1234567890", "makassar", "TIDAKKUTAUKI", "indihom3nkri", "Makassar_2015"};

const int SSID_COUNT = sizeof(ALL_SSID) / sizeof(ALL_SSID[0]);

#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* serverIP = "192.168.1.7";
const int httpPort = 80;

#define pinLED D6

const int buzzerPin = D0;


const int relayPin = D3; // Pin kontrol relay

const uint16_t kIrLed = D1;  // Pin output IR LED (sesuaikan dengan pin yang Anda gunakan)
IRsend irsend(kIrLed);

const uint16_t kRecvPin = D2;  // Pin input IR receiver (sesuaikan dengan pin yang Anda gunakan)
IRrecv irrecv(kRecvPin);


void buzzBuzzer(int duration) {
  digitalWrite(buzzerPin, HIGH);  // Menghidupkan buzzer
  delay(duration);                // Menunggu durasi yang ditentukan
  digitalWrite(buzzerPin, LOW);   // Mematikan buzzer
  delay(50);                      // Jeda singkat sebelum bunyi berikutnya (opsional)
}

void receivLaravel() {
  WiFiClient client;

  if (!client.connect(serverIP, httpPort)) {
    Serial.println("Gagal terhubung ke web server!");
    return;
  }

  String LinkRelay;
  HTTPClient httpRelay;

  LinkRelay = "http://" + String(serverIP) + "/newApp/enertrack/bacarelay";
  httpRelay.begin(client, LinkRelay);

  // AMBIL ISI DARI LARAVEL
  httpRelay.GET();

  // BACA STATUS RESPONSE
  String responseRelay = httpRelay.getString();

  Serial.println(responseRelay);

  // WAJIB, SETELAH MELAKUKAN REQUEST
  httpRelay.end();

  client.stop();

// if (responseRelay.toInt() == 1) {
//     digitalWrite(relayPin, LOW);
//     Serial.println("AC ON");    
//   }else{
//      digitalWrite(relayPin, HIGH); // Mengaktifkan relay  
//     Serial.println("AC OFF");
//   }

if (responseRelay.toInt() == 1) {
   unsigned char data[] = {
    0x4B, 0x85, 0x58, 0xE,
    0x80, 0x1C, 0x88, 0xCB,
    0x70, 0xF6, 0x87, 0x88,
    0x5F, 0xBA, 0x46, 0x8,
    0x8F, 0x5D, 0xD, 0x41
  };
  irsend.sendDaikin(data, sizeof(data));
  } else {
    digitalWrite(relayPin, HIGH);
    Serial.println("AC OFF");
  }

}


void setup() {
  Serial.begin(115200);

  irrecv.enableIRIn();

  dht.begin();
  pinMode(pinLED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
 pinMode(buzzerPin, OUTPUT);  // Mengatur pin sebagai output

  WiFi.hostname("NodeMCU");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Penyiapan selesai");

  connectToWiFi();

  Serial.println("Server started");

  

}

void loop() {

  receivLaravel();

   decode_results results;
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    irrecv.resume();  // Melanjutkan menerima sinyal IR berikutnya
  }

  int temp = dht.readTemperature();
  int hum = dht.readHumidity();
  
  if(temp > 35){
     buzzBuzzer(100);
    delay(100);
    buzzBuzzer(100);
  }else{
    digitalWrite(buzzerPin, LOW);
  }

  Serial.println("Suhu: " + String(temp));
  Serial.println("Kelembapan: " + String(hum));

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    if (!client.connect(serverIP, httpPort)) {
      Serial.println("Gagal terhubung ke web server!");
      return;
    }

    String url = "/newApp/enertrack/update/" + String(temp) + "/" + String(hum);
    Serial.print("URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + serverIP + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(100);

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.println(line);
    }

    client.stop();
  }

  delay(1000);


 

}

void connectToWiFi() {
  boolean wifiFound = false;

  Serial.println("Pencarian jaringan WiFi...");

  int numVisibleNetworks = WiFi.scanNetworks();

  if (numVisibleNetworks == 0) {
    Serial.println("Tidak ada jaringan WiFi yang ditemukan!");
    while (true);
  }

  Serial.print("Jumlah jaringan WiFi yang terdeteksi: ");
  Serial.println(numVisibleNetworks);

  for (int i = 0; i < numVisibleNetworks; i++) {
    for (int j = 0; j < SSID_COUNT; j++) {
      if (WiFi.SSID(i) == ALL_SSID[j]) {
        wifiFound = true;
        Serial.print("Terhubung ke jaringan WiFi: ");
        Serial.println(WiFi.SSID(i));
        Serial.print("Kata sandi: ");
        Serial.println(ALL_PASS[j]);

        WiFi.begin(ALL_SSID[j], ALL_PASS[j]);
        int attempt = 0;

        while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
          attempt++;

          if (attempt >= 20) {
            wifiFound = false;
            break;
          }
        }

        if (wifiFound) {
          Serial.println("\nKoneksi WiFi berhasil!");
          Serial.print("Alamat IP: ");
          Serial.println(WiFi.localIP());
          digitalWrite(pinLED, HIGH);
          return;
        }
      }
    }
  }

  if (!wifiFound) {
    Serial.println("Tidak dapat terhubung ke jaringan WiFi yang dikenal!");
    while (true);
  }
}
