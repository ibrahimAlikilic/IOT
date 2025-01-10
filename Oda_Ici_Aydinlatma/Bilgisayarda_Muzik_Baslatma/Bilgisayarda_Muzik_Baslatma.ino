#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// NodeMCU pinleri
#define RST_PIN D1
#define SS_PIN_1 D2


const char* ssid = "";       // WiFi adı
const char* password = ""; // WiFi şifresi
const char* serverUrl = "http://:/wake"; // Flask sunucu adresi

// RFID'yi tanımlama
MFRC522 rfid1(SS_PIN_1, RST_PIN);

String allowedUID = ""; // Geçerli RFID kartı UID'si
WiFiClient wifiClient;  // WiFiClient nesnesi oluşturuluyor

void setup() {
  Serial.begin(115200);    // Seri port başlat
  SPI.begin();             // SPI başlat (RFID için)
  rfid1.PCD_Init();        // RFID okuyucuyu başlat

  Serial.println("RFID modülü hazır!");

  // Wi-Fi bağlantısını başlat
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi'ye bağlanılıyor...");
  }
  Serial.println("WiFi bağlantısı başarılı!");
}

void loop() {
  // Eğer yeni bir kart okutulmuşsa
  if (rfid1.PICC_IsNewCardPresent() && rfid1.PICC_ReadCardSerial()) {
    processCard(rfid1);  // Kartı işle
    rfid1.PICC_HaltA();  // Kartı durdur
  }

}

void processCard(MFRC522& reader) {
  String scannedUID = "";
  
  // Okunan UID'yi string formatında tutmak için
  for (byte i = 0; i < reader.uid.size; i++) {
    scannedUID += String(reader.uid.uidByte[i], HEX);
  }


  // Eğer okunan UID, izin verilen UID'ye eşitse
  if (scannedUID == allowedUID) {
    Serial.println("AUTHORIZED");  // Kart yetkilendirilmiş
    sendHttpRequest();  // HTTP isteği gönder
  } else {
    Serial.println("UNAUTHORIZED");  // Kart yetkisiz
  }
}

void sendHttpRequest() {
  HTTPClient http;
  http.begin(wifiClient, serverUrl);  // WiFiClient ile HTTP isteği başlat
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("Sunucu yanıtı: %s\n", http.getString().c_str());
  } else {
    Serial.printf("HTTP isteği başarısız: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();  // HTTP bağlantısını sonlandır
}
