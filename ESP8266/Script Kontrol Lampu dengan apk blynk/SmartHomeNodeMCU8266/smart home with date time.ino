#define BLYNK_TEMPLATE_ID "TMPLstM2b7O9"
#define BLYNK_DEVICE_NAME "kontrol lampu"
#define BLYNK_AUTH_TOKEN "Vl7XliFi11LhPUDzWS_MAIylSbcPZk8r"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

char auth[] = BLYNK_AUTH_TOKEN;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

char ssid[] = "RH";
char pass[] = "makassar92";

const long utcOffsetInSeconds = 3600 * 7;
// area saya ada di WIB Indonesia,
// dan itu harus +7jam dari GMT
// sehingga program diatas harus dikalikan dengan 7 saja

char daysOfTheWeek[7][12] = {"Minggu", "Senin ", "Selasa", "Rabu  ", "Kamis ", "Jumat ", "Sabtu "};

// Definisi tool NTP nya
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup()
{
  Serial.begin(9600);

  pinMode(2, OUTPUT); // Initialise digital pin 2 as an output pin
  pinMode(0, OUTPUT); // Initialise digital pin 2 as an output pin

  Blynk.begin(auth, ssid, pass);

  Wire.begin();
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  lcd.print("ryntooh@gmail.com");
  lcd.setCursor(00, 1);
  lcd.print("Smart Home");
  delay(2000);
  lcd.clear();
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("Sabar Yaah!");
  }
  lcd.clear();
  timeClient.begin();
}

BLYNK_WRITE(V1) // Executes when the value of virtual pin 0 changes
{
  if (param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(2, HIGH); // Set digital pin 2 HIGH
    lcd.setCursor(0, 1);
    lcd.clear();
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(2, LOW); // Set digital pin 2 LOW
    lcd.setCursor(0, 1);
    lcd.print("Lampu ON");
  }
}

BLYNK_WRITE(V2)
{
  if (param.asInt() == 1)
  {

    digitalWrite(0, HIGH);
  }
  else
  {

    digitalWrite(0, LOW);
  }
}

void loop()
{
  timeClient.update();

  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.print(",");
  if (timeClient.getHours() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(timeClient.getHours() + 1);
  lcd.print(":");
  if (timeClient.getMinutes() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(timeClient.getMinutes());
  lcd.print(":");
  if (timeClient.getSeconds() <= 9)
  {
    lcd.print("0");
  }
  lcd.println(timeClient.getSeconds());
  lcd.setCursor(0, 1);
  // lcd.print(" ");

  // Serial.println(timeClient.getFormattedTime());

  delay(1000);

  Blynk.run();
}
