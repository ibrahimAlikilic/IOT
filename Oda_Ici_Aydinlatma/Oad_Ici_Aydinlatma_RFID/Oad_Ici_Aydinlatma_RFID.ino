

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
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

// bilgisayar ile bağlantı kurmak için
const char* serverUrl = "http:///wake"; // Flask sunucu adresi


// rfid'leri tanımlama
MFRC522 rfid1(SS_PIN_1, RST_PIN);

// kartların tanımlanması
String allowedUID1 = ""; // sadece ışıkların açılıp kapanması
String allowedUID2 = ""; // ışıklar ile birlikte bilgisayardan müziğin de başlaması
WiFiUDP udp;
WiFiClient wifiClient;  // WiFiClient nesnesi oluşturuluyor // bilgisayarda müzik gönderme için lazım

// rfid etrafını ışıklandırmak için 2 tane led bağladım onların ataması
const int ledPins[]={D3,D4};
const int totalLeds = sizeof(ledPins) / sizeof(ledPins[0]);

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

  // ledleri yak
  for (int i = 0; i < totalLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // LED'leri başlangıçta kapat
  }
  Hepsini_Yak();
}

void loop() {
  // hangi rfid'nin okuduğunun kontrolü
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
  if (uid == allowedUID1) { // birinci kart okunduysa mesajı gönder ve sadece ışıklar harekete geçsin
    Serial.printf("AUTHORIZED (%s)\n", readerName);
    String text = "AUTHORIZED_" + String(readerName);
    sendMessage(text.c_str());
  } 
  else if(uid == allowedUID2){ // eğer ikinci kart okunduysa mesajı gönderdikten sonra müziği de başlat
    Serial.printf("AUTHORIZED (%s)\n", readerName);
    String text = "AUTHORIZED_" + String(readerName);
    sendMessage(text.c_str());
    sendHttpRequest();
  }
  else {
    Serial.printf("UNAUTHORIZED (%s)\n", readerName);
    Uyari_Isiklari();
  }
}

void sendMessage(const char* message) {
  udp.beginPacket(LightIP, udpPort);
  udp.write(message);
  udp.endPacket();
  Serial.println("Mesaj gönderildi: " + String(message));
}

// bilgisayarda müziği başlatan method
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


// Ledler için 
void Hepsini_Yak() {
  for (int i = 0; i < totalLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}

void Hepsini_Kapat() {
  for (int i = 0; i < totalLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}
void Uyari_Isiklari(){
  for(int i=0;i<3;i++){
    Hepsini_Yak();
    delay(1000);
    Hepsini_Kapat();
    delay(1000);
  }
  Hepsini_Yak();
}