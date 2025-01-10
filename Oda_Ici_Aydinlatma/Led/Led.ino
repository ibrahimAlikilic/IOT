

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp8266.h>

// WiFi ayarları
const char* ssid = "";
const char* password = "";

// Blynk Auth Token (Uygulamadan alınacak)
char auth[] = "";

// UDP ayarları
WiFiUDP udp;
const int udpPort = 12345;
char incomingPacket[255]; // Gelen UDP mesajı için buffer

// LED pinleri
const int ledPins[] = {D1, D2, D3, D4, D5, D6, D7, D8};
const int totalLeds = sizeof(ledPins) / sizeof(ledPins[0]);
const int delayTime = 250; // LED kayması hızı

bool ledState = false;          // LED durumu (false: kapalı, true: açık)
bool isEffectActive = true;    // Kayma efekti bayrağı
int tek_cift = 1; // okunma sayısı tekse tamamını aç çiftse tamamını kapat

void setup() {
  Serial.begin(115200);

  // WiFi'ye bağlan
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi'ye bağlanılıyor...");
  }
  Serial.println("WiFi bağlantısı başarılı!");

  Blynk.begin(auth, ssid, password);

  udp.begin(udpPort); // UDP sunucusunu başlat
  Serial.printf("UDP bağlantısı %d portunda dinleniyor...\n", udpPort);

  // LED pinlerini çıkış olarak ayarla
  // LED pinlerini OUTPUT olarak ayarla
  for (int i = 0; i < totalLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // LED'leri başlangıçta kapat
  }
  Hepsini_Kapat();
}

void loop() {
  Blynk.run();
  
  // UDP mesajlarını dinle
  int packetSize = udp.parsePacket();

  if (ledState) {
    if (!isEffectActive) { // telefondan açık durumdaysa ve kart okutulduysa 
      Dongu();
      isEffectActive = true; // Efekt sadece bir kez çalışacak
    }
    tek_cift=0;
    Hepsini_Yak();
  }
  else {
    Hepsini_Kapat();
    isEffectActive = false;
    tek_cift=1;
  }

  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0'; // Gelen mesajı null-terminate et
    }
    Serial.printf("UDP mesajı alındı: %s\n", incomingPacket);

    // Mesajı işle
    // tek_cift ilk başta 1 bu sayede tek ise girece ve en sonlarında arttırma işlemi gerçekleştiğinden kontrol esnasında yanma ya da sönme durumu kontrol edilebiliyor.
    if (String(incomingPacket) == "AUTHORIZED_Reader1" && tek_cift%2 != 0) { // tek sayısa giriş sağlandıysa 
      if (!isEffectActive) {
        Dongu();
        isEffectActive = true; // Efekt sadece bir kez çalışacak
        tek_cift++;
      }
      // uygulama ile bağlantı kurmazsak ışıklar bir açılıp bir kapanıyor.
      ledState = true; // LED durumu açık
      Blynk.virtualWrite(V0, 1); // Butonu aç

      Hepsini_Yak();
      Serial.println("RFID yetkili, LED'ler açıldı.");
    } 
    else if (String(incomingPacket) == "AUTHORIZED_Reader1" && tek_cift%2 == 0) {
      // uygulama ile bağlantı kurmazsak ışıklar bir açılıp bir kapanıyor.
      ledState = false; // LED durumu kapalı
      Blynk.virtualWrite(V0, 0); // Butonu kapat

      Hepsini_Kapat();
      isEffectActive = false;
      tek_cift++;
    }
    else{
      Uyari();
    }
  }
}

void Dongu() {
  for (int i = 0; i < 3; i++) {
    for (int i = 0; i < totalLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(delayTime);
      digitalWrite(ledPins[i], LOW);
    }
    for (int i = totalLeds - 1; i >= 0; i--) {
      digitalWrite(ledPins[i], HIGH);
      delay(delayTime);
      digitalWrite(ledPins[i], LOW);
    }
  }
}
void Uyari_Dongu(){
  for(int i=0;i<2;i++){
    Hepsini_Yak();
    Hepsini_Kapat();
  }
}

void Uyari(){ // olması gerekenin haricinde kart okutulduğu vakit 
  Uyari_Dongu();
  Hepsini_Kapat();
}

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

// Blynk uygulamasındaki bir butonla LED'leri açıp kapatmak için
BLYNK_WRITE(V0) {
  ledState = param.asInt(); // Buton durumu (1: Açık, 0: Kapalı)
}
