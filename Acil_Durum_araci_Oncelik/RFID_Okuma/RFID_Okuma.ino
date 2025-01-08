#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp8266.h>

// NodeMCU pinleri
#define RST_PIN D1
#define SS_PIN_1 D2
#define SS_PIN_2 D3
#define SS_PIN_3 D4
#define SS_PIN_4 D8


// wifi
const char* ssid = "";
const char* password = "";
// diğer esp ip bağlantı bilgileri
const char* trafficLightIP = "";
const int udpPort = 12345;

char auth[] = ""; // blynk

// rfid'leri tanımlama
MFRC522 rfid1(SS_PIN_1, RST_PIN);
MFRC522 rfid2(SS_PIN_2, RST_PIN);
MFRC522 rfid3(SS_PIN_3, RST_PIN);
MFRC522 rfid4(SS_PIN_4, RST_PIN);

String allowedUID = ""; // geçerli rfid kartı udi
WiFiUDP udp;

bool rfid1Enabled = true; // blynk kontrol

void setup() {
  Serial.begin(115200);
  SPI.begin();

  rfid1.PCD_Init();
  rfid2.PCD_Init();
  rfid3.PCD_Init();
  rfid4.PCD_Init();

  Serial.println("RFID modülleri hazır!");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi'ye bağlanılıyor...");
  }
  Serial.println("WiFi bağlantısı başarılı!");

  Blynk.begin(auth, ssid, password);
}

void loop() {
  Blynk.run();
  // blynk açık kapalı ve hangi rfid'nin okuduğunun kontrolü
  if (rfid1Enabled && rfid1.PICC_IsNewCardPresent() && rfid1.PICC_ReadCardSerial()) {
    processCard(rfid1, "Reader1");
    rfid1.PICC_HaltA();
  }

  if (rfid1Enabled && rfid2.PICC_IsNewCardPresent() && rfid2.PICC_ReadCardSerial()) {
    processCard(rfid2, "Reader2");
    rfid2.PICC_HaltA();
  }

  if (rfid1Enabled && rfid3.PICC_IsNewCardPresent() && rfid3.PICC_ReadCardSerial()) {
    processCard(rfid3, "Reader3");
    rfid3.PICC_HaltA();
  }

  if (rfid1Enabled && rfid4.PICC_IsNewCardPresent() && rfid4.PICC_ReadCardSerial()) {
    processCard(rfid4, "Reader4");
    rfid4.PICC_HaltA();
  }
}

void processCard(MFRC522& reader, const char* readerName) {
  String uid = "";
  for (byte i = 0; i < reader.uid.size; i++) {
    uid += String(reader.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

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
  udp.beginPacket(trafficLightIP, udpPort);
  udp.write(message);
  udp.endPacket();
  Serial.println("Mesaj gönderildi: " + String(message));
}

BLYNK_WRITE(V0) { rfid1Enabled = param.asInt(); } //blynk bağlantı butonu