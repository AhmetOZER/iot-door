/*
* https://ahmetozer.org
*/



#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>


const char* ssid = "Your SSID";
const char* password = "Your PASS";
  
  int durum = 0;
  int lamba0 = 0;
  int hirsiz = 1;
  int hirsiz0 = 0;
  int kapiacik = 0;
  int wificheck = 0;
  
void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  /*while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
*/
  delay(1000);
  pinMode(5, INPUT); // Kapı sensörü D1
  pinMode(14, INPUT); // Yakınlık sensörü D5
  pinMode(2, OUTPUT); // Lamba yok D4 ama D7 gpio 13 yapılacak 
  pinMode(15, INPUT); // OTA modu D8

  
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("Sokak-Kapisi");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(2, LOW);
}

void loop() {

if (WiFi.waitForConnectResult() != WL_CONNECTED) {
  wificheck = wificheck + 1;
  }

  if (wificheck > 5000 ){
    Serial.println("Connection Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }

  
  //if (digitalRead(15) == HIGH) {
    ArduinoOTA.handle();
  //} else {
  
      if (lamba0 > 2 ) { // Lamba say 
        lamba0 = lamba0 + 1;
      }
  
     /* if(digitalRead(5) == HIGH) { //Kapı sensörü 
        //Kapi kapali
        if (durum > 2) {
          lamba0 = 180001; //Kapı Kapanınca Lambayı Kapat
         }
      } else {
            digitalWrite(2, HIGH); //Kapı açılınca lambayı aç
           // hirsiz = 1;
            if (durum < 3){
            kapi("Kapi-Acildi");
            }
            durum = 3; // Kapa kapnınca lamba kapanmasını tetikle
            kapiacik = kapiacik + 1;
      }
  */
      /*if (digitalRead(14) == LOW) { // Yakınlık sensörü
          if (lamba0 < 3) {
          digitalWrite(2, HIGH);
          lamba0 = 3;
          }
         // hirsiz = hirsiz + 1 ;
      }*/
      if(digitalRead(5) == HIGH) {
         if (lamba0 < 3) {
            digitalWrite(2, HIGH);
            lamba0 = 3;
          }
       }

      if(digitalRead(14) == LOW) { //Kapı sensörü 
        //Kapi kapali
        if (durum > 2) {
          lamba0 = 180001; //Kapı Kapanınca Lambayı Kapat
         }
      } else {
            digitalWrite(2, HIGH); //Kapı açılınca lambayı aç
           // hirsiz = 1;
            if (durum < 3){
            kapi("Kapi-Acildi");
            }
            durum = 3; // Kapa kapnınca lamba kapanmasını tetikle
            kapiacik = kapiacik + 1;
      }
      

      if (lamba0 > 18000) { // 18 saniye sonra lambayı kapat
          digitalWrite(2, LOW);
          durum = 2;
          lamba0 = 0;
          kapiacik = 0;
      }

     /* if (hirsiz == 17000) {
        kapi("Kapidaki-uyarildi");
          //uyar  
      }

      if (hirsiz == 25000) {
        kapi("Kapi-zorlaniyor");
          //Kapı zorlanıyor.  
      }
    */
     /* if (hirsiz > 1){
        hirsiz0 = 5;
      }
      
      if (hirsiz0 > 1) {
        hirsiz0 = hirsiz0 + 1;
      }

      if (hirsiz0 > 300000) {
        hirsiz0 = 0;
        hirsiz = 1;
      }
      */
      if (kapiacik > 300000) {
        kapi("Kapi-acik-unuttunuz.");  //You forget the door open
        kapiacik = 150000;
      }

       
      delay(1);
	  
  /*  //DEBUG
  Serial.print("Lamba0 =");
  Serial.println(lamba0);
  Serial.print("durum =");
  Serial.println(durum);
  Serial.print("Gpio5 + D1 + Kapı sensoru =");
  Serial.println(digitalRead(5));
  Serial.print("Gpio14 + D5 + Yakınlık sensoru =");
  Serial.println(digitalRead(14));
  Serial.print("Gpio2 + D4 + Lamba =");
  Serial.println(digitalRead(2));
  */
}


void kapi(String durum2) {
   if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     HTTPClient http;  //Declare an object of class HTTPClient
     http.begin("IFTTT"+durum2);  //Specify request destination  . Replace "IFTTT" with your ifttt app url. Exmaple "http://maker.ifttt.com/trigger/sokak_kapisi/with/key/----------------?value1="
    int httpCode = http.GET();                                                                  //Send the request
     if (httpCode > 9) { //Check the returning code
       String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
     }
     http.end();   //Close connection
   }
   delay(2000);    //Send a request every 30 seconds
 }

