//Library for stepper motor                                                                                                                                                                                                                  #include<HX711_ADC.h>
#include<Stepper.h>

//Library for load cell
#include<HX711_ADC.h>

// Connects DOUT pin,SCK pin of HX711 to pin6, pin7 of Arduino
HX711_ADC LoadCell(6,7);

//Defines pins numbers of HC-SR04
#define trig 3
#define echo 4

//Defines variables
long Time;
float Distance;
int stepsPerRevolution=400;
int i=0;

//Sets pins of Arduino connected to steppermotor
Stepper myStepper(stepsPerRevolution,10,11,12,13);

//Sets pin for buzzer
#define buzzer 8

void setup() {
  Serial.begin(9600);// Starts the serial communication
  LoadCell.begin();//Starts connection to HX711
  LoadCell.start(2000);//Sets a stabilising time to improve tare precision
  LoadCell.setCalFactor(365.0);//Set calibration factor for load cell
  Serial.println("Ready for weighing:");
  pinMode(trig,OUTPUT);//Sets the trigPin of HC-SR04 as an Output
  pinMode(echo,INPUT);//Sets the echoPin as an Input
  myStepper.setSpeed(60);//Sets rotate speed for stepper motor
  pinMode(buzzer,OUTPUT);

}

void loop() {
  LoadCell.update();//Retrives data from load cell
  float weight=LoadCell.getData();//Gets output value
  Serial.print("Weight:");
  Serial.println(weight);
  //Sets the trig on High for 10 micro seconds to start working
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  //Echo records the sounds wave's traveling time
  Time=pulseIn(echo,HIGH);
  //Calculates the distance by multipling traveling time by sound speed
  Distance=Time*0.034/2;
  Serial.print("Distance: ");
  Serial.println(Distance);

//Invokes motor function when feeding is required
//The threshold set for weight of food in bowl is less than 20g
//Sets i as a stablility factor because there is some problem with the HX711 board that it may detecte and read the value of weight unstably 
//i is a multiple of 15 means that the weight sensor will detect 15 times in a row to ensure that it returns correct and stable data. 
if(weight<20&&(i%15==0)){
  myStepper.step(stepsPerRevolution);//Motor rotates clockwise
  delay(500);
  myStepper.step(-stepsPerRevolution);//Motor rotates counterclockwise
 
}
i++;

//Invokes the buzzer function when sensor detects that food in vessel is few
//Sensor detects the distance between the top of vessel and the food to estimate the amount of remaining food, the threshold we set is 10cm
if(Distance>10){
  for(int i=0;i<2;i++)//Controls numbers of voice prompts 
  {
    tone(buzzer,1000);//Sends a tone at a frequency of 1000 Hz
    delay(1000);
    noTone(buzzer);
    delay(1000);
  }
}
delay(1500);
}
