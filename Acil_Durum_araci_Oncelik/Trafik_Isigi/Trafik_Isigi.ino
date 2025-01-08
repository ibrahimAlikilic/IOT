#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// WiFi ayarları
const char* ssid = "";
const char* password = "";
const int udpPort = 12345;  // UDP port numarası

// LED pinleri
const int redLedPin1 = D1;
const int greenLedPin1 = D2;
const int redLedPin2 = D3;
const int greenLedPin2 = D4;
const int redLedPin3 = D5;
const int greenLedPin3 = D6;
const int redLedPin4 = D7;
const int greenLedPin4 = D8;

WiFiUDP udp;
char incomingPacket[255];
bool overrideTraffic = false;

// bekleme süreleri
const int yesil_sure=1000;
const int kirmizi_sure=1000;
void setup() {



  Serial.begin(115200);

  pinMode(redLedPin1, OUTPUT);
  pinMode(greenLedPin1, OUTPUT);
  pinMode(redLedPin2, OUTPUT);
  pinMode(greenLedPin2, OUTPUT);
  pinMode (redLedPin3, OUTPUT); 
  pinMode (greenLedPin3, OUTPUT); 
  pinMode (redLedPin4, OUTPUT); 
  pinMode (greenLedPin4, OUTPUT);

  baslangic_Durumu(); // tüm kırmızılar yanar ve tüm yeşiller söner

  // WiFi'ye bağlan
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi'ye bağlanılıyor...");
  }
  Serial.println("WiFi bağlantısı başarılı!");

  // UDP başlat
  udp.begin(udpPort);
  Serial.println("UDP dinleniyor...");
  Serial.println(WiFi.localIP());
}

void loop() {
  trafficCycle(redLedPin1, greenLedPin1,yesil_sure,kirmizi_sure);
  trafficCycle(redLedPin2, greenLedPin2,yesil_sure,kirmizi_sure);
  trafficCycle(redLedPin3, greenLedPin3,yesil_sure,kirmizi_sure);
  trafficCycle(redLedPin4, greenLedPin4,yesil_sure,kirmizi_sure);
}

void controlTrafficLights1(int redClose1, int greenOpen1, int redOpen2, int greenClose2, int redOpen3, int greenClose3, int redOpen4, int greenClose4) { // gelen mesajın hangi yoldan geldiği parametreden alınarak ışıklar ayarlanır.
  // Durum: redLedPin1 ve greenLedPin2 HIGH, diğerleri LOW
  digitalWrite(redClose1, LOW);
  digitalWrite(greenOpen1, HIGH);
  digitalWrite(redOpen2, HIGH);
  digitalWrite(greenClose2, LOW);
  digitalWrite(redOpen3, HIGH);
  digitalWrite(greenClose3, LOW);
  digitalWrite(redOpen4, HIGH);
  digitalWrite(greenClose4, LOW);
  delay(5000);
}

void trafficCycle(int red, int green ,int kirmizi_sure,int yesil_sure) {
  // Kırmızı LED
  digitalWrite(red, HIGH);
  for (int i=0;i<2;i++){
    mesajKontrol();
    delay(kirmizi_sure);
  }
  digitalWrite(red, LOW);
  

  // Yeşil LED
  digitalWrite(green, HIGH);
  for (int i=0;i<2;i++){
    mesajKontrol();
    delay(yesil_sure);
  }
  digitalWrite(green, LOW);
  for (int i=0;i<2;i++){
    mesajKontrol();
    delay(yesil_sure/2);
  }

  // Yeşil LED yanıp sönme
  for (int i = 0; i < 3; i++) {
    digitalWrite(green, HIGH);
    // mesaj alınıp acil durum aracı geçtikten sonra tüm yollara kırmızı ışık yandığından bu düzenleme bulunmaktadır.
    for (int i=0;i<2;i++){ 
      mesajKontrol();
      digitalWrite(red,LOW);
      delay(yesil_sure/2);
    }
    digitalWrite(green, LOW);
    for (int i=0;i<2;i++){
      mesajKontrol();
      digitalWrite(red,LOW);
      delay(yesil_sure/2);
    }
  }

  digitalWrite(red, HIGH);
}
void baslangic_Durumu(){
  digitalWrite(redLedPin1, HIGH);
  digitalWrite(greenLedPin1, LOW);
  digitalWrite(redLedPin2, HIGH);
  digitalWrite(greenLedPin2, LOW);
  digitalWrite(redLedPin3, HIGH);
  digitalWrite(greenLedPin3, LOW);
  digitalWrite(redLedPin4, HIGH);
  digitalWrite(greenLedPin4, LOW);
}

void mesajKontrol() {
  // Mesaj kontrolü
  int packetSize = udp.parsePacket();
  if (packetSize) {    
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0';
    }
    String message = String(incomingPacket);
    Serial.println("Mesaj alındı: " + message);

    // Mesaja göre işlem yap
    if (message == "AUTHORIZED_Reader1") {
      Serial.println("03011977 1");
      overrideTraffic = true;
      // hangi rfid okuduğu kontrolü
      controlTrafficLights1(redLedPin1, greenLedPin1, redLedPin2, greenLedPin2,redLedPin3, greenLedPin3, redLedPin4, greenLedPin4);
      message = "";
    }
    else if (message == "AUTHORIZED_Reader2") {
      Serial.println("içerdeyiiiz 2");
      overrideTraffic = true;
      // hangi rfid okuduğu kontrolü
      controlTrafficLights1(redLedPin2, greenLedPin2, redLedPin1, greenLedPin1,redLedPin3, greenLedPin3, redLedPin4, greenLedPin4);
      message = "";
    }
    else if (message == "AUTHORIZED_Reader3") {
      Serial.println("içerdeyiiiz  3");
      overrideTraffic = true;
      // hangi rfid okuduğu kontrolü
      controlTrafficLights1(redLedPin3, greenLedPin3, redLedPin1, greenLedPin1,redLedPin2, greenLedPin2, redLedPin4, greenLedPin4);
      message = "";
    }
    else if (message == "AUTHORIZED_Reader4") {
      Serial.println("içerdeyiiiz  4");
      overrideTraffic = true;
      // hangi rfid okuduğu kontrolü
      controlTrafficLights1(redLedPin4, greenLedPin4, redLedPin1, greenLedPin1,redLedPin2, greenLedPin2, redLedPin3, greenLedPin3);
      message = "";
    }
    else{
      Serial.println("Sıkıntı var !");
    }
  }

  // Acil durum aracı geçtiyse tüm yolların kırmızı yanması için
  if (overrideTraffic) {
      overrideTraffic = false;
      baslangic_Durumu();
}
}