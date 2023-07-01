

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Samsung.h>

const char* ALL_SSID[] = {"RH", "realme", "ENGINEERING.TR 7", "PUBLIC_TR7", "404 - Not Found!"};
const char* ALL_PASS[] = {"1234567890", "makassar", "TIDAKKUTAUKI", "indihom3nkri", "Makassar_2015"};

const int SSID_COUNT = sizeof(ALL_SSID) / sizeof(ALL_SSID[0]);

#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* serverIP = "192.168.1.7";
const int httpPort = 80;

#define pinLED D6

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRSamsungAc ac(kIrLed);     // Set the GPIO used for sending messages.

const int buzzerPin = D0;

ESP8266WebServer webServer(80);

void buzzBuzzer(int duration) {
  digitalWrite(buzzerPin, HIGH);  // Menghidupkan buzzer
  delay(duration);                // Menunggu durasi yang ditentukan
  digitalWrite(buzzerPin, LOW);   // Mematikan buzzer
  delay(50);                      // Jeda singkat sebelum bunyi berikutnya (opsional)
}

void handleData() {
  String data = webServer.arg("data");

  // Menampilkan data yang diterima ke Serial Monitor
  Serial.println(data);

  if (data == "ON") {
      
      ac.on();
      ac.send();
      buzzBuzzer(100);        // Bunyikan buzzer selama 200 ms
      delay(100);             // Tunggu 200 ms sebelum membunyikan buzzer lagi
      buzzBuzzer(100);
      
  }else {

    ac.off();
    ac.send();
    buzzBuzzer(100);
    delay(100);
    buzzBuzzer(100);        
  }

  webServer.send(200, "text/plain", "Data berhasil diterima oleh NodeMCU");
}


void handleDataSuhu() {
  String data = webServer.arg("data");

  // Menampilkan data yang diterima ke Serial Monitor
  Serial.println(data);

  int temperature = data.toInt();

  switch (temperature) {
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
      ac.setTemp(temperature);
      ac.send();
      break;
    default:
      // Menangani kasus jika suhu tidak valid
      break;
  }

  webServer.send(200, "text/plain", "Data berhasil diterima oleh NodeMCU");
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  ac.begin();
  pinMode(pinLED, OUTPUT);
  pinMode(D3, OUTPUT);
 pinMode(buzzerPin, OUTPUT);  // Mengatur pin sebagai output

  WiFi.hostname("NodeMCU");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Penyiapan selesai");

  connectToWiFi();

  // Mengatur rute untuk menangani permintaan data
  webServer.on("/receive-data", handleData);
  webServer.on("/data-suhu", handleDataSuhu);

  webServer.begin();
  Serial.println("Server started");
}

void loop() {
  int temp = dht.readTemperature();
  int hum = dht.readHumidity();
  
  if(temp > 35){
    buzzBuzzer(100);
    delay(100);
    buzzBuzzer(100);
    delay(100);
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

  webServer.handleClient();
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
