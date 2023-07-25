#include <Wire.h>
#include <Servo.h>

Servo rdR, rdB, rdL;
Servo esc1, esc2, esc3, esc4, esc5, esc6;

int relay[7] = {0,A0,A1,A2,A3,A4,A5};
#define stanby A7
#define s_voltage A8
#define buzzer 22

bool condition, st_stanby = true, send_monitor[3];
int action, angel_ball, inclinometer;
String mode;

void(*reset)(void) = 0;

void setup() {
  Serial.begin(115200); //uploader
  Serial1.begin(9600); //remote
  Serial2.begin(115200); //vision
  Serial3.begin(9600); //slave
  
  rdR.attach(9);
  rdB.attach(10);
  rdL.attach(11);

  pinMode(s_voltage,INPUT);
  pinMode(buzzer, OUTPUT);
  for(int i=1; i<8; i++){pinMode(relay[i], OUTPUT);}
 
  rdR.write(40);
  rdB.write(140); //kiri
  rdL.write(40);
  delay(1000);
  rdR.write(140);
  rdB.write(40); //kanan
  rdL.write(140);
  delay(1000);
  rdR.write(90);
  rdB.write(90);
  rdL.write(90);
  

  esc1.attach(2,2000,4000);
  esc2.attach(3,2000,4000);
  esc3.attach(4,2000,4000);
  esc4.attach(5,2000,4000);
  esc5.attach(6,2000,4000);
  esc6.attach(7,2000,4000);

  digitalWrite(buzzer,1);
  delay(300);
  digitalWrite(buzzer,0);
  delay(300);
  digitalWrite(buzzer,1);
  delay(300);
  digitalWrite(buzzer,0);
  delay(300);

  Serial3.print("X");
  Serial.println("Ready");
}

void loop() {
  get_degree(&inclinometer);
  get_image();
  
  get_data(&action, &mode);
  if(mode == "RAA,"){condition = true;}
  else if(mode == "ROF,"){condition = false; st_stanby = false;}
  
  if(analogRead(stanby)>100 && st_stanby){condition = true; st_stanby = false;}
  else if(analogRead(stanby)<100 && !st_stanby){condition = false; st_stanby = true;}
  
  float adc = analogRead(s_voltage);
  float baterai = (5*(adc/1024)/1.46)*16.1;
  Serial.print(mode);Serial.print("-");Serial.print(action);
  Serial.print("  Bat : ");Serial.print(baterai);
  Serial.print("  incl: ");Serial.print(inclinometer);
  Serial.print("  cnd: ");Serial.print(condition);
  Serial.println();

  if(condition){autonomus();}
  else if(!condition){function_remote();}
  
}
