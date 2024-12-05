#define ENA 9
#define ENB 10
#define inA1 2
#define inA2 3
#define inB1 4
#define inB2 5


#define echoPin 13
#define trigPin 12


void setup() {

  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(inA1, OUTPUT);
  pinMode(inA2, OUTPUT);
  pinMode(inB1, OUTPUT);
  pinMode(inB2, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);


}

void loop() {

  int distance = getDistance();
  if (distance > 0 && distance < 20) {
    stopMotors();
    delay(1000);
    turnRight(2000);//set the turning duration  
    delay(1000);
  }else { moveForward();}
 

  Serial.println(getDistance());


}

int getDistance(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) {
    return -1; // Sensör başarısız
  }
  int distance = duration * 0.034 / 2;
  return distance;
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
void turnRight(int turningTime){
  
  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, HIGH);
  digitalWrite(inB2, LOW);
  
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  
  delay(turningTime);
  stopMotors();

}
void stopMotors(){
  
  digitalWrite(inA1, LOW);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, LOW);
}