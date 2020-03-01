#include <SPI.h>
#include <RH_RF95.h>

// Pin definitions.
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

#define RF95_FREQ 915.0

// Radio driver.
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup()  {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);

  delay(100);

  // Manual reset.
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed.");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info.");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed.");
    while (1);
  }
  
  rf95.setTxPower(20, false);
}

void loop() {
  delay(3000);

  // Message to send.
  char radiopacket[37] = "9959cf4f-f79d-47ab-ad31-1f64bdf99ecc";

  // Send it.
  delay(10);
  rf95.send((uint8_t *)radiopacket, 37);
  delay(10);
  rf95.waitPacketSent();
}
