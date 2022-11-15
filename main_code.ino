#include <Adafruit_NeoPixel.h>

#define trigPin 3     //초음파 trig핀번호
#define echoPin 4     //초음파 echo핀번호
#define buzzer 7      //부저 핀번호
#define vibratePin 6  //진동센서 핀번호(PWM 핀 사용)
#define btn 8         //버튼 핀번호
#define cds A0        //조도센서 핀번호
#define neoPixel 2    //네오픽셀 핀번호
#define NumPixel 19   //네오픽셀 갯수
#define bright 255    //네오픽셀 밝기

Adafruit_NeoPixel neo(NumPixel, neoPixel, NEO_GRB + NEO_KHZ800);

double distance_now = 200;    //현재 장애물과의 거리 변수
int buzzer_time = 500;        //부저 작동 시간[ms]
double alert_distance = 100;  //알람작동 거리[cm]
int vibratePower = 255;       //진동 세기 변수(0~255)
int vibrateTime = 100;        //진동 유지 시간[ms]
int Btn;                      //버튼 변수


void setup() {
  pinMode(btn, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  neo.begin();
  neo.setBrightness(bright);
  neo.clear();
  neo.show();
}

void loop() {
  alert_sound();              //버튼 누를시 경보음
  cdsNeo(NumPixel);           //조도값 낮을 시 neopixel ON
  distance_now = distance();  //장애물과의 거리 변수

  if (distance_now < alert_distance / 5) {  //0~20cm
    analogWrite(vibratePin, vibratePower);
  } else if (distance_now < alert_distance / 5 * 2) {  //20~40cm
    analogWrite(vibratePin, vibratePower / 5 * 4);
  } else if (distance_now < alert_distance / 5 * 3) {  //40~60cm
    analogWrite(vibratePin, vibratePower / 5 * 3);
  } else if (distance_now < alert_distance / 5 * 4) {  //60~80cm
    analogWrite(vibratePin, vibratePower / 5 * 2);
  } else if (distance_now < alert_distance) {  //80~100cm
    analogWrite(vibratePin, vibratePower / 5);
  } else {
    analogWrite(vibratePin, 0);
  }
  delay(vibrateTime);
}

void alert_sound() {
  Btn = digitalRead(btn);
  if (Btn == 0) {
    digitalWrite(buzzer, HIGH);
    delay(buzzer_time);
    digitalWrite(buzzer, LOW);
  }
}

void cdsNeo(int i) {
  float val = analogRead(cds);
  if (val > 950)
    for (int j = 0; j < i; j++) neo.setPixelColor(j, bright, 0, bright);
  else
    for (int j = 0; j < i; j++) neo.setPixelColor(j, 0, 0, 0);
  neo.show();
}

double distance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  double pulseTime = pulseIn(echoPin, HIGH);
  double distance_cm = pulseTime * 17.33 / 1000;
  if (distance_cm > 350) {
    return 350;
  } else if (distance_cm < 10) {
    return 10;
  }
  return distance_cm;
}