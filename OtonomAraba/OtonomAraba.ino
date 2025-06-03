
//deneyap renk ve cizgi sensorleri icin
//SDA A4 
//SCL A5

//TRİG A0 
//ECHO A1 




#include <AFMotor.h>
#include <Wire.h>
#include <Deneyap_HareketIsikRenkAlgilayiciMesafeOlcer.h>   // Renk sensörü
#include <Deneyap_CiftKanalliCizgiAlgilama.h>               // Çizgi sensörü

// APDS9960 renk sensörü tanımı
APDS9960 Renk;
uint16_t r, g, b, c;

// Çift kanallı çizgi sensör tanımı
LineFollower Cizgi;

//motor tanimlari
AF_DCMotor motor1(1); //sol on
AF_DCMotor motor2(2); //sol arka
AF_DCMotor motor3(3); //sag on
AF_DCMotor motor4(4); //sag arka

//isik oranlari(mekana gore degisir)
const double kirmiziSinir = 0.58;
const double yesilSinir = 0.5;

// HC-SR04 pin tanımı
const int trigPin = A0;
const int echoPin = A1;


//deneyap cizgi sensoru olmadiklari icin tanim
const int solDisSensor = A2;
const int sagDisSensor = A3;

//mesafe icin gerekli degiskenler
long duration;
int distance;

//cizgiAlgilama fonksiyonu icine girmeye karar vermek icin 
//baslangicta hareket eder
bool hareketDurumM = true;
bool hareketDurumR = true;

void setup() {
  Serial.begin(115200);

  pinMode(sagDisSensor,INPUT);
  pinMode(solDisSensor,INPUT);

  //motorlarin hiz degerleri
  motor1.setSpeed(69); 
  motor2.setSpeed(69); 
  motor3.setSpeed(120); 
  motor4.setSpeed(120); 


  // HC-SR04 pin ayarları
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Renk sensörü başlatılıyor
  if (!Renk.begin(0x39)) {
    Serial.println("Renk sensörü I2C bağlantısı başarısız!");
    while (1);
  }
  Renk.enableColor(true);

  // Çizgi sensörü başlatılıyor
  if (!Cizgi.begin(0x07)) {
    Serial.println("Çizgi sensörü I2C bağlantısı başarısız!");
    while (1);
  }
}

void loop() {
  distance = mesafeAl();
  //Serial.print("Mesafe:");
  //Serial.println(distance);
  

  
  renkKontrol();
  mesafeKontrol();

  if(hareketDurumM && hareketDurumR){
    cizgiAlgila();
  }
  else{
    dur();
  }
  
}

// HC-SR04 mesafe hesaplama fonksiyonu
int mesafeAl() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);  // 30ms timeout

  int mesafe = duration * 0.034 / 2;
  return mesafe;
}

void mesafeKontrol() {
  
  distance = mesafeAl();

  if(distance>1 && distance < 17){
    hareketDurumM = false;
  }

  else {
    hareketDurumM = true;  
  }
}



void renkKontrol() {
  Renk.getColorData(&r, &g, &b, &c);
  double kirmiziOran = (c > 0) ? ((double)r / c) : 0;
  Serial.print("Esik");
  Serial.println(kirmiziSinir);
  Serial.print("değer"); 
  Serial.println(kirmiziOran);
  delay(100);


  if (kirmiziOran > kirmiziSinir ) {
    //Serial.println("KIRMIZI algılandı - DUR");
    hareketDurumR = false;
  }

  else if (g > 50) {
    //Serial.println("YESIL algılandı - DEVAM");
    hareketDurumR = true;
  }
  else if( b > 30 ){
    dur();
    delay(1000);
    sagaKay();
    delay(1250);
    dur();
    delay(3000);
    while(!Cizgi.ReadLineFollowerDigital2()){
      solaKay();
    }
  }
  else{
    hareketDurumR = true;
  }
}

//cizgi algilama ve donuse karar verme fonksiyonu
void cizgiAlgila() {

  bool solIc = !Cizgi.ReadLineFollowerDigital1();  // çizgi varsa LOW döndüğü için tersledim
  bool sagIc = !Cizgi.ReadLineFollowerDigital2();  // çizgi varsa LOW döndüğü için tersledim

  bool solDis = !digitalRead(solDisSensor);  // Sol dış sensör
  bool sagDis = !digitalRead(sagDisSensor);  // Sağ dış sensör


  if (solIc && sagIc ) {
    ileri();
  }
  else if (solIc && !sagIc) {
    solaDon();
  }
  else if (!solIc && sagIc) {
    sagaDon();
  }
  else if(solDis){
    while(solDis){
      solDis = !digitalRead(solDisSensor);  
      solaDon();
    }
  }
  else if(sagDis){
    while(sagDis){
      sagDis = !digitalRead(sagDisSensor);
      sagaDon();
    }
  }
}


//hareket fonksiyonları
void ileri(){
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
void geri(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void dur(){
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

}
void sagaDon(){
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  

}
void solaDon(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

}  

//mecanum tekerlekler icin hareket fonksiyonları
void sagaKay(){
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);

}
void solaKay(){
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

