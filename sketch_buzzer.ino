//#define buzzer=8;
const int buzzer=8;


void setup(){
      pinMode(buzzer,OUTPUT);
      for(int i=0;i<8;i++)
      {
        tone(buzzer,1000);
        delay(1000);
        noTone(buzzer);
        delay(1000);
        
      }
      noTone(buzzer);
}

void loop(){
      
}
