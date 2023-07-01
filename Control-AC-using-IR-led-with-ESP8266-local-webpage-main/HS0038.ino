#include <Arduino.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

#ifdef ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = D3;
#else
const uint16_t kRecvPin = D3;
#endif

const uint32_t kBaudRate = 115200;


const uint16_t kCaptureBufferSize = 1024;

#if DECODE_AC

const uint8_t kTimeout = 50;
#else  

const uint8_t kTimeout = 15;
#endif  

const uint16_t kMinUnknownSize = 12;


const uint8_t kTolerancePercentage = kTolerance;  

#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  

void setup() {
#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#else 
  Serial.begin(kBaudRate, SERIAL_8N1);
#endif  
  while (!Serial) 
    delay(50);
  
  assert(irutils::lowLevelSanityCheck() == 0);

  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
 
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif 
  irrecv.setTolerance(kTolerancePercentage);  
  irrecv.enableIRIn();
}


void loop() {
 
  if (irrecv.decode(&results)) {
  
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
   
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
   
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_STR "\n");
   
    if (kTolerancePercentage != kTolerance)
      Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
  
    Serial.print(resultToHumanReadableBasic(&results));
   
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield();
#if LEGACY_TIMING_INFO
   
    Serial.println(resultToTimingInfo(&results));
    yield();
#endif 
    
    Serial.println(resultToSourceCode(&results));
    Serial.println();    
    yield();            
  }
}
