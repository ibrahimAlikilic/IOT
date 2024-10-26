#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
char auth [] = "";  // Blynk uygulaması tarafından, mailinize gelen token key    
char ssid [] = "";  // Bağlantı yapacağınız Wi‐Fi adı 
char pass [] = ""; // Wi‐Fi şifreniz 
// Sanal pin oluşturma
BLYNK_WRITE(V0) { 
  digitalWrite(D0, param.asInt()); 
}
/*
BLYNK_WRITE(V0): Blynk platformunda bir sanal pin (V0) üzerine gelen veriyi almak için kullanılan özel bir fonksiyondur.
V0, Blynk uygulamasındaki sanal bir pin olarak çalışır ve Arduino ile uygulama arasında veri aktarımı sağlar.
*/
void setup() 
{ 
  Serial.begin(9600); 
  pinMode(D0,OUTPUT); 
  Blynk.begin(auth, ssid, pass, "blynk.cloud",80); 
}
/*
Serial.begin(9600);: Seri iletişimi başlatır ve haberleşme hızını 9600 baud olarak ayarlar. Bu, hata ayıklama veya sensör verilerini görmek için kullanılır.
pinMode(D0, OUTPUT);: D0 pinini çıkış (OUTPUT) modunda ayarlar. Bu sayede bu pine bağlı bir LED’i açıp kapatabilirsiniz. // D0 pini + ucu
Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);: Blynk sunucusuna bağlanır. Bu komutun parametreleri:
auth: Blynk uygulaması tarafından verilen kimlik doğrulama kodu (auth token).
ssid: Wi-Fi ağının adı.
pass: Wi-Fi şifresi.
"blynk.cloud": Blynk’in sunucu adresi (varsayılan bulut adresi).
80: Kullanılan port numarasıdır.
*/
void loop() 
{ 
  Blynk.run(); 
}
/*
Blynk.run();: Blynk kütüphanesinin sürekli çalışmasını sağlar.
Bu, Blynk platformu ile Arduino arasında sürekli iletişim sağlar.
Diğer işlevler gibi çalışması gereken Blynk ile ilgili tüm işlemleri burada işler.*/

