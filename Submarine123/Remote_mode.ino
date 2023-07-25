
int kecepatan, sudut, rov;
int r_rov = 0, r_kec = 0, r_sdt = 50, get_on = 0, gate = 0;
void function_remote()
{
  if(mode == "T,"){r_sdt = action;}
  else if(mode == "G,"){gate = action;}
  else if(mode == "S," && get_on == 1){r_rov = action;}
  else if(mode == "S," && get_on == 0){r_kec = action;}
  else if(mode == "LFA,"){get_on = 1;}
  else if(mode == "LNE,"){get_on = 1;}
  else if(mode == "LOF,"){get_on = 0;}

  if(gate == 3) //R
  {
    digitalWrite(relay[1], HIGH);
    digitalWrite(relay[4], HIGH);
  }
  else if(gate == 1) //D
  {
    digitalWrite(relay[1], LOW);
    digitalWrite(relay[4], LOW);
  }
  else if(gate == 0) //P
  {
    digitalWrite(relay[2], LOW);
    digitalWrite(relay[3], LOW);
    digitalWrite(relay[5], LOW);
    digitalWrite(relay[6], LOW);
    esc2.write(0); //FL
    esc3.write(0); //ML 
    esc5.write(0); //MR
    esc6.write(0); //FR
  }
  else if(gate == 2) //N
  {
    digitalWrite(relay[2], LOW);
    digitalWrite(relay[3], LOW);
    digitalWrite(relay[5], LOW);
    digitalWrite(relay[6], LOW);
    esc2.write(180); //FL
    esc3.write(180); //ML 
    esc5.write(180); //MR
    esc6.write(180); //FR
  }

  kecepatan = map(r_kec,0,95,0,180);
  rov = map(r_rov,0,95,0,180);
  sudut = map(r_sdt,0,99,40,140);
  
  Serial.print(" S : "); Serial.print(sudut);
  Serial.print(" K : "); Serial.print(kecepatan);
  Serial.print(" R : "); Serial.print(rov);

  rdB.write(sudut);

  int speed_L,speed_R;
  if(r_sdt > 49){speed_R = kecepatan-(kecepatan*((r_sdt-49)/50)); speed_L = 0;} //belok kiri
  else if(r_sdt < 49){speed_L = kecepatan-(kecepatan*((49-r_sdt)/49)); speed_R = 0;} //belok kanan
  else{speed_L = speed_R = kecepatan;}//maju

  esc1.write(speed_L); //BL
  esc4.write(speed_R); //BR
}


void motors_stabilization(int kec)
{
  int degreeR = kec - (inclinometer*4);
  int degreeL = kec + (inclinometer*4);
  
  if(degreeR >= 180){degreeR = 180;}
  else if(degreeR <= 0){degreeR = 0;}
  if(degreeL >= 180){degreeL = 180;}
  else if(degreeL <= 0){degreeL = 0;}
  
  esc2.write(degreeL); //FL
  esc3.write(degreeL); //ML 
  esc5.write(degreeR); //MR
  esc6.write(degreeR); //FR
}

void rudder_stabilization()
{
  int degree = 90 - (inclinometer*6);
  if(degree >= 140){degree = 140;}
  else if(degree <= 40){degree = 40;}
  rdR.write(degree);
  rdL.write(degree);
}
