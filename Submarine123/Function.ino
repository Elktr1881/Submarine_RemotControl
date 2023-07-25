/*bluthoot remote*/
String int_inString = "", str_inString = "";
uint16_t integer, string;
void get_data(int* value, String* nilai)
{
  while(Serial1.available())
  {
    int inchar = Serial1.read();
    if(isDigit(inchar)){int_inString+=(char)inchar;}
    if(isPrintable(inchar) && !isDigit(inchar)){str_inString+=(char)inchar;}
     
    if(inchar == ',')
    {
      integer = int_inString.toInt();
      *value = integer;
      *nilai = str_inString;
      int_inString = "";
      str_inString = "";
    }
  }
}

/*inclinometer slave*/
String int_inString1 = "", str_inString1 = "";
uint16_t integer1, string1;
void get_degree(int* value)
{
  String nilai;
  while(Serial3.available())
  {
    int inchar = Serial3.read();
    if(isDigit(inchar)){int_inString1+=(char)inchar;}
    if(isAlpha(inchar) && !isDigit(inchar)){str_inString1+=(char)inchar;}
     
    if(inchar == ',')
    {
      nilai = str_inString1;
      integer1 = int_inString1.toInt();
      if(nilai == "N"){*value = -integer1;}
      else if(nilai == "P"){*value = integer1;}
      int_inString1 = "";
      str_inString1 = "";
    }
  }
}

/*vision image*/
String int_inString2 = "", str_inString2 = "";
uint16_t integer2, string2;

String get_green[11]={" ","AHD","BHD","CHD","DHD","EHD","FHD","GHD","HHD","IHD","JHD"}; //kanan lintasaan
String get_red[11]={" ","AOD","BOD","COD","DOD","EOD","FOD","GOD","HOD","IOD","JOD"}; //kiri lintasan
String loss_green[11]={" ","AHT","BHT","CHT","DHT","EHT","FHT","GHT","HHT","IHT","JHT"}; //kanan lintasaan
String loss_red[11]={" ","AOT","BOT","COT","DOT","EOT","FOT","GOT","HOT","IOT","JOT"}; //kiri lintasan
int green[11],red[11],sum_green, sum_red;

void get_image()
{
  while(Serial2.available())
  {
    int inchar = Serial2.read();
    if(isDigit(inchar)){int_inString2+=(char)inchar;}
    if(isAlpha(inchar) && !isDigit(inchar)){str_inString2+=(char)inchar;}

    if(inchar == ',')
    {
      integer2 = int_inString2.toInt();
      int value2 = integer2;
      String nilai2 = str_inString2;
      int_inString2 = "";
      str_inString2 = "";

      for(int i=1; i<=10; i++)
      {
        if(nilai2 == get_green[i]) {green[i] = value2;}
        else if(nilai2 == loss_green[i]) {green[i] = 0;}
        if(nilai2 == get_red[i]) {red[11-i] = value2;}
        else if(nilai2 == loss_red[i]) {red[11-i] = 0;}
      }
    }
    
  }

  
  for(int i=1; i<=10; i++){Serial.print(red[i]); Serial.print(" ");}
  for(int i=1; i<=10; i++){Serial.print(green[i]); Serial.print(" ");}

  angel_ball=90; sum_red=0; sum_green=0;
  for(int i=1; i<=10; i++)
  {
    if(red[i]>0){sum_red+=i;}
    if(green[i]>0){sum_green+=i;}
 
    if(i>3 && i<8){continue;}
    if(red[i]>0){angel_ball+=20;}
    if(green[i]>0){angel_ball-=20;}
  }
//  Serial.print(" R: ");Serial.print(sum_red);
//  Serial.print(" G: ");Serial.print(sum_green);
//  Serial.print(" S: ");Serial.print(angel_ball);
}

void Throttling()
{
  esc1.write(180); //ML
  esc2.write(180); //MR
  esc3.write(180); //FL 
  esc4.write(180); //FR
  esc5.write(180); //BR
  esc6.write(180); //BL
  delay(2500);

  esc1.write(0); //ML
  esc2.write(0); //MR
  esc3.write(0); //FL 
  esc4.write(0); //FR
  esc5.write(0); //BR
  esc6.write(0); //BL
}

int save_rudder = 90, save_time, set_time = millis();
void rudder_smooth(int sdt, unsigned int interval)
{
    if(sdt <= 40){sdt = 40;} else if(sdt >=140){sdt =140;}
    
    if(save_rudder == sdt){set_time = millis();}
    else if(save_rudder > sdt){if(save_time > interval){save_rudder--; set_time = millis();}}
    else if(save_rudder < sdt){if(save_time > interval){save_rudder++; set_time = millis();}}
    
    rdB.write(save_rudder);
    save_time = millis()-set_time;
    
//    Serial.print(save_time);
//    Serial.print(" ");
//    Serial.print(save_rudder);
//    Serial.print(" ");
}

unsigned long stamp = millis(), waktu;
void maju(int kec)
{
  digitalWrite(relay[1], LOW);
  digitalWrite(relay[4], LOW);
  
  esc1.write(kec); esc4.write(kec);
}

void belok(int kec, int sdt, double modulasi, double interval)
{ 
  digitalWrite(relay[1], LOW);
  digitalWrite(relay[4], LOW);

  if(sdt <= 40){sdt = 40;} else if(sdt >=140){sdt =140;}
  rdB.write(sdt);
  
  if(modulasi >= 1.00){modulasi = 1;}
  int time_on = interval*modulasi;

  waktu = millis()- stamp;
  if(waktu <= time_on)
  {
    if(sdt<90){esc1.write(kec); esc4.write(0);}
    else if(sdt>90){esc1.write(0); esc4.write(kec);}
  }
  else if(waktu > time_on)
  {
    esc1.write(0); esc4.write(0);
    if(waktu > interval){stamp = millis();}
  }
  
}

void rem(int kec,int sdt)
{
  digitalWrite(relay[1], HIGH);
  digitalWrite(relay[4], HIGH);
  esc1.write(kec); esc4.write(kec);
  rdB.write(sdt);
}

void swing_rudder(int sdt)
{
//  int hasil;
//  if(sdt <= 40){hasil = map(sdt,0,90,45,90);} else if(sdt >=140){hasil = map(sdt,90,180,90,135);}
  rdR.write(sdt); rdL.write(sdt);
}
