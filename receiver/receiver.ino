#include <SPI.h>
#include <RH_RF95.h>

// Pin definitions.
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define LED 13

#define RF95_FREQ 915.0

// Radio driver.
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(LED, OUTPUT);

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  
  delay(100);

  // Reset radio.
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed.");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info.");
    while (1);
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed.");
    while (1);
  }

  rf95.setTxPower(20, false);
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      Serial.print("Got: ");
      Serial.println((char*)buf);
      
      Serial.print("Signal strength: ");
      Serial.println(rf95.lastRssi(), DEC);

      // If this is the message we're looking for, flash the onboard LED.
      if(strcmp((char*)buf, "9959cf4f-f79d-47ab-ad31-1f64bdf99ecc") == 0) {
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
      }
      
    } else {
      Serial.println("Receive failed.");
    }
    
  }
}
