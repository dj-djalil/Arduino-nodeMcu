
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>  

#define FIREBASE_HOST "esp-iot-943f9-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "DzeIovy3ZbMTtgm9KQ04uTKUYUhcVxjxuhE90sqY"
#define WIFI_SSID "idoomAdsl2020"
#define WIFI_PASSWORD "la famille louhichi"

#define DHTPIN 0    // Connect Data pin of DHT to pin 0
#define DHTTYPE    DHT11  // ----------------------------------------------- LE TYPE DE CAPTEUR UTILISE

DHT dht(DHTPIN, DHTTYPE); // Temperature

#include <Servo.h> // pour le servo Moteur
Servo myservo;


#define gaz A0 
int led_01=12; 
//int led_02=13;
int relay=13;
int led_03=15; // led 03 avec mouvement
int led_04=16;
int pos = 1; // position initial du servoMoteur
int pinMov =14; // capteur de Mov
int  _movValue=0;
 
// buzzer
int buzzer = 4;



void setup() {
  Serial.begin(9600);
  // led pins 
  pinMode(led_01,OUTPUT);
  pinMode(led_03,OUTPUT);
  pinMode(led_04,OUTPUT);
  pinMode(relay,OUTPUT);
   pinMode(gaz, INPUT);

  pinMode(buzzer, INPUT);
  
   dht.begin();  // DHT sensor begin
   myservo.attach(5);  // attaches the servo on pin 5 to the servo object

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
  
}




void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
   // set value
 Firebase.setFloat("Temperature/temp", t);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /Temperature Failed:");
      Serial.println(Firebase.error());  
      return;
  }
 
 Firebase.setFloat("Temperature/humidity", h);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /Humidity Failed:");
      Serial.println(Firebase.error());  
      return;
  }


 // ventilateur 
  
 
   if(Firebase.getString("Ventilateur/auto")=="1"){

      if(t>25.0){  Serial.println("temp>25"); digitalWrite(relay,HIGH);}
      else { Serial.println("fan auto ON"); digitalWrite(relay,LOW);  }
  }else {
      
      if(Firebase.getString("Ventilateur/etat")=="1"){Serial.println("fan is on manuel"); digitalWrite(relay,HIGH);}
      else { Serial.println("fan is off");digitalWrite(relay,LOW); }
  }

// leds
 
if(Firebase.getString("Leds/led1")=="1"){digitalWrite(led_01,HIGH);}
else {digitalWrite(led_01,LOW);}
 
if(Firebase.getString("Leds/led2")=="1"){ Serial.println("led 2 is on");}
else {Serial.println("led 2 is oFF");}
 
if(Firebase.getString("Leds/led4")=="1"){digitalWrite(led_04,HIGH);}
else { digitalWrite(led_04,LOW);}
// leds 03 avec capteur de mouvement
if(Firebase.getString("Leds/led3_auto")=="1"){
     Serial.println("led 03 is auto");
    _movValue=digitalRead(pinMov);
    if(_movValue==HIGH){
             digitalWrite(led_03,HIGH);
           // si il n'a pas de mouvement 
           } else if(_movValue==LOW){
                digitalWrite(led_03,LOW);
           }
  
}else{
  if(Firebase.getString("Leds/led3")=="1"){digitalWrite(led_03,HIGH);}
  else {digitalWrite(led_03,LOW);}
}



// Portes et Fenetres 
  if(Firebase.getString("Portes_Fenetres/porte")=="1"){
    for (pos ; pos <= 94; pos += 1) { 
    myservo.write(pos);                                    
       }
     } else{
       for (pos ; pos >= 1; pos -= 1) {  
        myservo.write(pos);              
                        
  }
    
    }

 // alarm GAZ

  if(Firebase.getString("Gaz/alarme")=="1"){
   
    int analogSensor = analogRead(gaz);
    Firebase.setInt("Gaz/gaz",analogSensor);
    if(analogSensor>400){
         pinMode(buzzer, OUTPUT);
    }else{
       pinMode(buzzer, INPUT);
    }
    
  }else{
     pinMode(buzzer, INPUT);
  }
     
}
