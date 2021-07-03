 


#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
 
 
#define FIREBASE_HOST "esp-iot-943f9-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "DzeIovy3ZbMTtgm9KQ04uTKUYUhcVxjxuhE90sqY"
#define WIFI_SSID "idoomAdsl2020"
#define WIFI_PASSWORD "la famille louhichi"


#include <ESP32Servo.h>
Servo myservo;  
int pos = 1;
int servoPin = 18;

 int count = 0;
 int relais =13;
 int garagePin1=15;
 int garagePin2=2;
 int led2 =19;
 
 String garageEtat;
FirebaseData fbdo;
unsigned long sendDataPrevMillis = 0;
void setup() {
  Serial.begin(9600);
  

  
   

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  
  
   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
   pinMode(garagePin1,OUTPUT);
   pinMode(garagePin2,OUTPUT);
   pinMode(relais,OUTPUT);
   pinMode(led2,OUTPUT);

  digitalWrite(garagePin1,LOW);
  digitalWrite(garagePin2,LOW);
  pinMode(relais,LOW);
  //myservo.attach(servoPin, 1000, 2000);
  myservo.attach(servoPin);
  
  Firebase.getString(fbdo, "/Garage");
  garageEtat=fbdo.stringData();
   
}




void loop() {

   if (Firebase.ready())
  {
    
 if(Firebase.getString(fbdo, "/Garage")){
      if(fbdo.stringData()=="1" && garageEtat=="0" ){
        digitalWrite(garagePin1,LOW);
        digitalWrite(garagePin2,HIGH);
        delay(2000);
        digitalWrite(garagePin1,LOW);
        digitalWrite(garagePin2,LOW);
        garageEtat="1";
      }else  if(fbdo.stringData()=="0" && garageEtat=="1"){
         digitalWrite(garagePin1,HIGH);
         digitalWrite(garagePin2,LOW);
         delay(2000);
         digitalWrite(garagePin1,LOW);
         digitalWrite(garagePin2,LOW);
         garageEtat="0";
      }
 }   

 // led 
 Firebase.getString(fbdo, "/Leds/led2");
 if(fbdo.stringData()=="1"){
   digitalWrite(led2,HIGH);          
  }else{
   digitalWrite(led2,LOW);
  }
 //window 
 Firebase.getString(fbdo, "/Portes_Fenetres/fenetre");
  if(fbdo.stringData()=="1"){
    for (pos ; pos <= 94; pos += 1) { 
    myservo.write(pos); 
    delay(15);                                   
       }
 }else{
    for (pos ; pos >= 1; pos -= 1) {  
    myservo.write(pos); 
    delay(15);                               
  }               
  }
  
//  prise Connectee 
 if(Firebase.getString(fbdo, "/prisesConnectees/prise01")){
      if(fbdo.stringData()=="1"){
        digitalWrite(relais,LOW);
      }else{
          digitalWrite(relais,HIGH);
      }
 }
  }
}
