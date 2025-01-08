
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp8266.h>

// WiFi ayarları
const char* ssid = "";
const char* password = "";

// Blynk Auth Token (Uygulamadan alınacak)
char auth[] = "";

// LED pinleri
const int ledPins[] = {D1, D2, D3, D4, D5, D6, D7, D8};
const int totalLeds = sizeof(ledPins) / sizeof(ledPins[0]);
const int delayTime = 250; // LED kayması hızı

bool ledState = false;          // LED durumu (false: kapalı, true: açık)
bool isEffectActive = true;    // Kayma efekti bayrağı

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

  Hepsini_Kapat();
}

void loop() {
  Blynk.run();

  if (ledState) {
    if (!isEffectActive) {
      // LED kayma efekti
      Dongu();
      isEffectActive = true; // Efekt sadece bir kez çalışacak
    }
    Hepsini_Yak(); // Efekt sonrası LED'ler sabit yanar
  } else {
    Hepsini_Kapat(); // LED'leri kapat
    isEffectActive = false; // Efekt bayrağını sıfırla
  }
}

void Dongu() {
  // LED kayma efekti
  for (int i=0 ; i<3 ; i++){
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
