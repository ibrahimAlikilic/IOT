

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// NodeMCU pinleri
#define RST_PIN D1
#define SS_PIN_1 D2


// wifi
const char* ssid = "";
const char* password = "";
// diğer esp ip bağlantı bilgileri
const char* LightIP = "";
const int udpPort = 12345;


// rfid'leri tanımlama
MFRC522 rfid1(SS_PIN_1, RST_PIN);

String allowedUID = ""; // geçerli rfid kartı udi
WiFiUDP udp;


void setup() {
  Serial.begin(115200);
  SPI.begin();

  rfid1.PCD_Init();

  Serial.println("RFID modülleri hazır!");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi'ye bağlanılıyor...");
  }
  Serial.println("WiFi bağlantısı başarılı!");
}

void loop() {
  // blynk açık kapalı ve hangi rfid'nin okuduğunun kontrolü
  if ( rfid1.PICC_IsNewCardPresent() && rfid1.PICC_ReadCardSerial()) {
    processCard(rfid1, "Reader1");
    rfid1.PICC_HaltA();
  }
}

void processCard(MFRC522& reader, const char* readerName) {
  String uid = "";
  for (byte i = 0; i < reader.uid.size; i++) {
    uid += String(reader.uid.uidByte[i], HEX);
  }
  // uid.toUpperCase();

  // doğru udi mi
  if (uid == allowedUID) {
    Serial.printf("AUTHORIZED (%s)\n", readerName);
    String text = "AUTHORIZED_" + String(readerName);
    sendMessage(text.c_str());
  } else {
    Serial.printf("UNAUTHORIZED (%s)\n", readerName);
  }
}

void sendMessage(const char* message) {
  udp.beginPacket(LightIP, udpPort);
  udp.write(message);
  udp.endPacket();
  Serial.println("Mesaj gönderildi: " + String(message));
}