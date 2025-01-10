#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// WiFi ayarları
const char* ssid = "";
const char* password = "";

void setup() {
  Serial.begin(115200);

  // WiFi'ye bağlan
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi'ye bağlanılıyor...");
  }
  Serial.println("WiFi bağlantısı başarılı!");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP()); // ESP'nin aldığı yerel IP adresini yazdır
}

void loop() {
  // Bu örnekte loop fonksiyonunda bir işlem yapılmıyor
}
