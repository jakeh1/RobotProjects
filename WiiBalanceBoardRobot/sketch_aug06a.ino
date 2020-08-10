/* This arduino code reads in serial data form usb.
 * The serial message contains data to control the motors
 * The message is formated as "value,value;" where value is between -255 and 255 
 * Uses seedStodio motor controler shield
*/
const int IN1 = 8;
const int EA  =9;
const int EB = 10;
const int IN2 = 11;
const int IN3 = 12;
const int IN4 = 13;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EA, OUTPUT);
  pinMode(EB, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String data = Serial.readStringUntil(';');

    int index = data.indexOf(',');
    String value1 = data.substring(0, index);
    String value2 = data.substring((index + 1));
    int speedRight = value1.toInt();
    int speedLeft = value2.toInt();
    bool dirRight = true;
    bool dirLeft = true;
    
    if(speedRight < 0){
      dirRight = false;
      speedRight = -1 * speedRight;
    }
    else{
      dirRight = true;
    }
    if(speedLeft < 0){
      dirLeft = false;
      speedLeft = -1 * speedLeft;
    }
    else{
      dirLeft = true;
    }

    rightMotor((byte)speedRight, dirRight);
    leftMotor((byte)speedLeft, dirLeft);

   
    
     
  }
  
  

}
//low level control for the left motor
void leftMotor(byte spd, bool dir){

    if(dir){
      digitalWrite(IN2, HIGH);
      digitalWrite(IN1, LOW);
    }
    else{
      digitalWrite(IN2, LOW);
      digitalWrite(IN1, HIGH);
    }
    analogWrite(EA, spd);
  }
  void rightMotor(byte spd, bool dir){
    if(dir){
      digitalWrite(IN4, LOW);
      digitalWrite(IN3, HIGH);
    }
    else{
      digitalWrite(IN4, HIGH);
      digitalWrite(IN3, LOW);
    }
    analogWrite(EB, spd);
  }
