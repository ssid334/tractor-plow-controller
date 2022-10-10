#include <elapsedMillis.h>
elapsedMillis timeElapsed;
 
int RPWM = 10;
int LPWM = 11;

int DEBUG = 0;

int downPin = 12;
//int downPin = 4;


//int upPin = 13;
int upPin = 2;
//int upPin = 6;

int autoPin = 8;
int manPin = 9;
 
int sensorPin = A0;
int potPin = A1;
int potVal;
int maxpotVal;
int sensorVal;
int Buffer = 10;

int speedPin = A2;

int MAX_SPEED = 255;

int maxpotPin = A3;
 
int maxAnalogReading; 
int minAnalogReading;
 
void setup() {
  pinMode(maxpotPin, INPUT);
 
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(autoPin, INPUT_PULLUP);
  pinMode(manPin, INPUT_PULLUP);  
  pinMode(sensorPin, INPUT);
  pinMode(potPin, INPUT);

  pinMode(speedPin, INPUT);
 
  Serial.begin(9600);
  // maxAnalogReading = moveToLimit(1); // otwieracie monitor portu szeregowego i odczytujecie wartoscii poprzez sterowanie potenjometrem, w jakim zakresie ma działać was siłownik, usuwacie movetolimit(1) i zmieniacie np na 937
  maxAnalogReading = 858; // 701;
  // minAnalogReading = moveToLimit(-1) ; // otwieracie monitor portu szeregowego i odczytujecie wartoscii poprzez sterowanie potenjometrem, w jakim zakresie ma działać was siłownik, usuwacie movetolimit(-1) i zmieniacie np na 242
  // minAnalogReading = 328; //317;
  minAnalogReading = 4;
 
}


 // 328 - 690 potencjometr

 // 312 - 707 
void loop() {

  int mspeed = map(analogRead(speedPin), 0, 1023, minAnalogReading, maxAnalogReading);
  int speedval = (mspeed - 238)/1.77;
  if (speedval > 250) { speedval = 250; }


if ( DEBUG == 1) {
  Serial.print("manPin");   Serial.print(digitalRead(manPin)==LOW);   Serial.print("autoPin");   Serial.println(digitalRead(autoPin)==LOW);
  Serial.print("speed");   Serial.print(speedval);
  //delay(500);
}
  delay(47);
   
  if(digitalRead(manPin)==LOW)
  {
    if (DEBUG == 1) { 
    Serial.println("automat");
    Serial.println(digitalRead(upPin)==LOW);
    Serial.println(digitalRead(downPin)==LOW);
    //delay(100);
    }
  if(digitalRead(upPin)==LOW){ //wysuwaj jesli guzik jest wcisniety
    analogWrite(RPWM, 0);
    analogWrite(LPWM, speedval);
    
  }
 
  else if(digitalRead(downPin)==LOW){ //wsuwaj jesli guzik jest wcisniety
    analogWrite(RPWM, speedval);
    analogWrite(LPWM, 0);
  }
 
  else{ //jesli guzik nie jest wcisniety stoj
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);
  }
  }
 
  else if(digitalRead(autoPin)==LOW)
  {
  potVal = map(analogRead(potPin), 0, 1023, minAnalogReading, maxAnalogReading);
  // maxpotVal = map(analogRead(maxpotPin), 0, 1023, 330, 688);
  maxpotVal = map(analogRead(maxpotPin), 0, 1023, 340, 680);
  sensorVal = analogRead(sensorPin);

  if(potVal > (sensorVal+Buffer)){               //Dodatkowy bufor, zapobiegajacy wibracja silnika z powodu zaklocen danych wejscia
    driveActuator(1, speedval);
  }
  else if(potVal < (sensorVal-Buffer)){
    driveActuator(-1, speedval);
  }
  else{
    driveActuator(0, 0);
  }

  
 
  }
 
  else{ //jesli zaden przycisk trybu nie jest przelaczony, stoj
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);    
  }


  if ( DEBUG == 1) {
    Serial.print("potVal: "); //po ustaleniu zakresy pracy na potencjometrze można usunąć ten kod od tej linijki do delay(10)
    Serial.print(potVal);
    Serial.print("\tsensorVal: ");
    Serial.print(sensorVal);
   
    Serial.print("\tSpeed: ");
    Serial.println(speedval);

    delay(10);
  }
}


 /*
int moveToLimit(int Direction){  //po ustaleniu zakresy pracy na potencjometrze można usunąć ten kod od tej linijki do return currreading i znak } który znajduje się w linijce pod spodem
  int prevReading=0;
  int currReading=0;
  do{
    prevReading = currReading;
    driveActuator(Direction, 255);
    timeElapsed = 0;
    while(timeElapsed < 200){ delay(1);}          
   currReading = analogRead(sensorPin);
  }while(prevReading != currReading);
  return currReading;
}
*/
 
void driveActuator(int Direction, int mspeed){
  switch(Direction){
    case 1:       //wsuwanie
      if (DEBUG == 1) { Serial.println("wysuwam"); }
      analogWrite(RPWM, mspeed);
      analogWrite(LPWM, 0);
      break;
   
    case 0:       //stop
      if (DEBUG == 1) { Serial.println("STOP"); }
      analogWrite(RPWM, 0);
      analogWrite(LPWM, 0);
      break;
 
    case -1:      //wysuwanie
      if (DEBUG == 1) { Serial.println("chowie"); }
      analogWrite(RPWM, 0);
      analogWrite(LPWM, mspeed);
      break;
  }
}
