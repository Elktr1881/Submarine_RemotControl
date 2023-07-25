int running_step = 0;
unsigned time_auto, save_auto;
void autonomus()
{
  esc2.write(180); digitalWrite(relay[2], LOW); //FL
  esc3.write(180); digitalWrite(relay[3], LOW); //ML 
  esc5.write(180); digitalWrite(relay[5], LOW); //MR
  esc6.write(180); digitalWrite(relay[6], LOW); //FR
            
  switch(running_step)
  {
    case 0: delay(5000); swing_rudder(90); maju(180); delay(1000); 
            save_auto=millis(); running_step++; break;
    
    case 1: if(angel_ball == 90){maju(160);} else{maju(120);} 
            rudder_smooth(angel_ball,30); swing_rudder(90);
            if(sum_red == 0 && sum_green > 10){running_step = 2;} //belok kanan
            else if(sum_green == 0 && sum_red > 10){running_step = 4;} //belok kiri
            time_auto=millis()-save_auto;
            break;

    case 2: if(time_auto >=5000){rem(120,90);delay(2000);}swing_rudder(90);
             running_step++; break;
    case 3: belok(140,140,0.5,1000); swing_rudder(70);
            if(sum_red > 0){running_step = 1; save_auto=millis();} 
            break;
            
    case 4: if(time_auto >=5000){rem(120,90);delay(2000);}swing_rudder(90);
             running_step++; break;        
    case 5: belok(140,40,0.5,1000); swing_rudder(115);
            if(sum_green > 0){running_step = 1; save_auto=millis();} 
            break;
  }
  
  
//  motors_stabilization(140);
//  rudder_stabilization();
  Serial.print(" RN: ");Serial.print(running_step);
}
