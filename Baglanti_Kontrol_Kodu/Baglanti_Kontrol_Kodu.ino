// ESP8266 Dahili LED Blink Kodu
#define LED_PIN 2  // ESP8266'da dahili LED genellikle GPIO 2'ye bağlıdır

void setup() {
  pinMode(LED_PIN, OUTPUT);  // LED pinini çıkış olarak ayarla
}

void loop() {
  digitalWrite(LED_PIN, LOW);   // LED'i aç (LOW, dahili LED'de yanar)
  delay(1000);                  // 1 saniye bekle
  digitalWrite(LED_PIN, HIGH);  // LED'i kapat (HIGH, dahili LED'de söner)
  delay(1000);                  // 1 saniye bekle
}