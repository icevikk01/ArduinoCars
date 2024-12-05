#include <Dabble.h>

#define ENA 9
#define ENB 10
#define inA1 2
#define inA2 3
#define inB1 4
#define inB2 5

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(inA1, OUTPUT);
  pinMode(inA2, OUTPUT);
  pinMode(inB1, OUTPUT);
  pinMode(inB2, OUTPUT);

  Dabble.begin(9600);

}

void loop() {
  Dabble.processInput();

  if (GamePad.isUpPressed()) {

    moveForward();

  } 
  
  else if (GamePad.isDownPressed()) {

    moveBack();

  } 
  
  else if(GamePad.isLeftPressed()){

    turnLeft();

  } 
  
  else if(GamePad.isRightPressed()){

    turnRight();

  }
  
  else {

    stopMotors();

  }

}
void moveForward(){

  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, HIGH);
  
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}
void moveBack(){//Optional
  
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, HIGH);
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, HIGH);
  
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}
void turnRight(){
  
  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, HIGH);
  digitalWrite(inB2, LOW);
  
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

}
void turnLeft(){
  
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, HIGH);
  digitalWrite(inB1, HIGH);
  digitalWrite(inB2, LOW);
  
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

}
void stopMotors(){
  
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, LOW);
}
