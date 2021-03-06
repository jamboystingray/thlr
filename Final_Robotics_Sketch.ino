#include <AFMotor.h>
#include "DHT.h"
//////////////////////////////////////////////////////////////////////
#define trigPin 12 // define the pins of your sensor
#define echoPin 13 
#define headlightsOne 4
#define headlightsTwo 5
#define DHTPIN 3
#define DHTTYPE DHT22 
#define togglePin 9
////////////////////////////////////////////////////////////////////
DHT dht(DHTPIN, DHTTYPE);
AF_DCMotor Motor1(1);
AF_DCMotor Motor2(2);
/////////////////////////////////////////////////////////////////////////
char toggleMode;
//////////////////////////First Function////////////////////////////////
void setup() {
  Motor1.setSpeed(255);
  Motor2.setSpeed(255);
  Serial.begin(9600);
  pinMode(headlightsOne, OUTPUT);
  pinMode(headlightsTwo, OUTPUT);
  digitalWrite(headlightsOne, HIGH);
  digitalWrite(headlightsTwo, HIGH);
  toggleMode = 'M';
  pinMode(trigPin, OUTPUT);// set the trig pin to output (Send sound waves)
  pinMode(echoPin, INPUT);// set the echo pin to input (recieve sound waves)
  pinMode(togglePin, INPUT);
  dht.begin();
}
////////////////////////////Main Loop//////////////////////////////
void loop() {
  transmit(); //runs thorughout
  
   if (digitalRead(togglePin) == LOW){ //toggle between  manual and automatic
      toggleMode = 'A';
   }
   if (digitalRead(togglePin) == HIGH){
      toggleMode = 'M'; 
   }       
   if (toggleMode == 'M'){
       manual(); 
   }
   if (toggleMode == 'A'){
       automatic(); 
   }
}

///////////////////////////Manual Mode////////////////////////////
void manual(){
      while (toggleMode == 'M'){
          transmit();
          int c = Serial.read();
          if (c == 49) {
            transmit();
            Motor1.run(FORWARD);
            Motor2.run(FORWARD);
             
            
          }
          else if (c == 50) {
            Motor1.run(RELEASE);
            Motor2.run(RELEASE);
            transmit();
          }
          
          else if (c == 51) {
            transmit();
            Motor1.run(BACKWARD);
            Motor2.run(BACKWARD);
            
          }
          else if (c == 52) {
            transmit();
            Motor1.run(RELEASE);
            Motor2.run(RELEASE);
            
          }
          else if (c == 53) {
            transmit();
            Motor1.run(BACKWARD);
            Motor2.run(FORWARD);
            
          } 
          else if (c == 54) {
            transmit();
            Motor1.run(RELEASE);
            Motor2.run(RELEASE);
           
          }
          else if (c == 55) {
            transmit();
            Motor1.run(FORWARD);
            Motor2.run(BACKWARD);
            
          }
          else if (c == 56) {
            transmit();
            Motor1.run(RELEASE);
            Motor2.run(RELEASE);
            
          }
          else if (c == 9){
             transmit();
            digitalWrite(headlightsOne, HIGH);
            digitalWrite(headlightsTwo, HIGH);
           
           
          }
          else if (c == 10){
            transmit();
            digitalWrite(headlightsOne, LOW);
            digitalWrite(headlightsTwo, LOW);
            
           
          }
          if (digitalRead(togglePin) == LOW){
            break;
          
          }
   }
}
////////////////////////////Auto Mode//////////////////////////////
void automatic(){

  while (toggleMode == 'A'){
      
      long duration, distance; // start the scan
      digitalWrite(trigPin, LOW);  
      delayMicroseconds(2); // delays are required for a succesful sensor operation.
      digitalWrite(trigPin, HIGH);

      delayMicroseconds(10); //this delay is required as well!
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration/2) / 29.1;// convert the distance to centimeters.
      if (distance < 25)/*if there's an obstacle 25 centimers, ahead, do the following: */ 
      {   
        Motor1.run(FORWARD);  // Turn as long as there's an obstacle ahead.
        Motor2.run (BACKWARD);
        transmit();
      }
  
      else {
        delay (15);
        Motor1.run(FORWARD); //if there's no obstacle ahead, Go Forward! 
        Motor2.run(FORWARD);
        transmit();  
      }
      if (toggleMode == 'M'){
        
        Motor1.run(RELEASE); //if there's no obstacle ahead, Go Forward! 
        Motor2.run(RELEASE);
        break;
      }
   }
}
/////////////////////////Temperature Transmit///////////////////////
void transmit(){ //transmitting mode
              
        float t_c = dht.readTemperature();
        Serial.print(t_c);
        
        
        
}



