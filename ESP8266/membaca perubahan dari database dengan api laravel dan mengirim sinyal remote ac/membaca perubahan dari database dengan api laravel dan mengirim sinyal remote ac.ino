#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const char* ssid = "404 - Not Found!";
const char* password = "Makassar_2015";
const char* serverIP = "192.168.1.7";  // Ganti dengan IP server Laravel Anda
const int httpPort = 80;  // Ganti dengan port yang digunakan oleh server Laravel Anda
const int interval = 5000;  // Interval waktu antara permintaan ke server (dalam milidetik)

const uint16_t kIrLed = D1;  // Pin output IR LED (sesuaikan dengan pin yang Anda gunakan)
IRsend irsend(kIrLed);
unsigned char data[] = {0x4B, 0x85, 0x58, 0xE};

void setup() {
    Serial.begin(115200);

    // Menghubungkan ke jaringan WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        // Membuat objek HTTPClient
        HTTPClient http;
        WiFiClient client;

        // Membuat URL API endpoint
        String url = "http://" + String(serverIP) + "/newApp/api/enertrack/statuspower";

        // Mengirim permintaan GET ke API endpoint
        http.begin(client, url);
        int httpResponseCode = http.GET();

        // Memeriksa respons dari server
        if (httpResponseCode == 200) {
            String response = http.getString();  // Mendapatkan respons dari server
            Serial.println("Response: " + response);

            // Lakukan tindakan sesuai dengan respons yang diterima
            // Misalnya, Anda dapat mengubah status relay berdasarkan respons

            // Parsing respons JSON dan mengambil status
            int status = 0;
            if (response.indexOf("\"status\":1") != -1) {
                status = 1;
            }

            if (response.toInt() == 1) {
                 irsend.sendDaikin(data, 2);
                  Serial.print("AC ON: ");
                  for (int i = 0; i < 2; i++) {
                    Serial.print(data[i], HEX);
                    Serial.print(" ");
                  }
                  Serial.println();
                Serial.println("AC ON");
            } else {
                // AC OFF
                
                Serial.println("AC OFF");
            }
        } else {
            Serial.print("Error: ");
            Serial.println(httpResponseCode);
        }

        // Menutup koneksi dan membersihkan sumber daya
        http.end();
    }

    delay(interval);

}
