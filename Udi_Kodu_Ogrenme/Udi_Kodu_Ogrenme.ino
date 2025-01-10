#include <SPI.h>
#include <MFRC522.h>

// RFID modülü için pin tanımları
#define RST_PIN D1  // Reset pin
#define SS_PIN D2  // Slave Select (CS) pin

MFRC522 rfid(SS_PIN, RST_PIN); // RFID nesnesi

void setup() {
  Serial.begin(115200); // Seri haberleşmeyi başlat
  SPI.begin();        // SPI haberleşmesini başlat
  rfid.PCD_Init();    // RFID modülünü başlat
  Serial.println("RFID modülü hazır!");
}

void loop() {
  // RFID kart okunup okunmadığını kontrol et
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    Serial.println("11.");
    delay(1000); // 1 saniye bekle
  if (!rfid.PICC_IsNewCardPresent()) {
    Serial.println("Yeni kart algılanmadı!");
    delay(1000);
    return;
  }
  if (!rfid.PICC_ReadCardSerial()) {
    Serial.println("Kart seri numarası okunamadı!");
    delay(1000);
    return;
  }
  }

  // RFID UID'yi oku
  String rfidUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    rfidUID += String(rfid.uid.uidByte[i], HEX); // UID değerlerini stringe çevir
  }

  // Seri porta UID'yi yazdır
  Serial.print("RFID Kart Numarası: ");
  Serial.println(rfidUID);
  
  delay(1000); // 1 saniye bekle
}