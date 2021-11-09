#include <ServoTimer2.h>
/* 
 * This program is for a two motor drive robot with a ultrasoinc attached to a servo using an arduino microcontroller.
 * The code will allow the robot to navigate a space without ruing into objects.
 */

  

  ServoTimer2 myservo;  // create servo object to control a servo


  int pos = 0;    // variable to store the servo position

  const int TURN_SPEED = 225;
  const int DRIVE_SPEED = 255;
  const int RIGHT_45_DELY = 460;
  const int LEFT_45_DELY = 500;
  const int RIGHT_90_DELY = 750;
  const int LEFT_90_DELY = 900;
  const int RIGHT_180_DELY = 0;
  const int LEFT_180_DELY = 1600;
  const long stopDistance = 12; //The distance the robot will stop at.
  int servoPin = 5;
  int pingPin = 4;
  const int IN1 = 8;
  const int EA = 9;
  const int EB = 10;
  const int IN2 = 11;
  const int IN3 = 12;
  const int IN4 = 13;  
  int pings[5];

  
void setup() { 

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EA, OUTPUT);
  pinMode(EB, OUTPUT);

  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(1500);
  Serial.begin(9600);
}

void loop() {
  
  myservo.write(1500); //sets the servo to the forward postion.
  pings[0] = getPing();
   
  //if the ultrasonic detects an object in front on the robot.
  if(pings[0] <= stopDistance && pings[0] >= 0){ 
   Serial.print("ultrasainics: ");
   Serial.print(pings[0]);
   Serial.print(", ");
   leftMotor(0,true);
   rightMotor(0, true);
   myservo.write(750); // read in right 90
   delay(1000);
   pings[1] = getPing();
   Serial.print(pings[1]);
   Serial.print(", ");
   myservo.write(1125); //read in right 45
   delay(1000);
   pings[2] = getPing();
   Serial.print(pings[2]);
   Serial.print(", ");
   myservo.write(1875); //read in left 45
   delay(1000);
   pings[3] = getPing();
   Serial.print(pings[3]);
   Serial.print(", ");
   myservo.write(2250); //read in left 90
   delay(1000);
   pings[4] = getPing();
   Serial.print(pings[4]);
   Serial.println();
   myservo.write(1500);

   int temp = 0;
   int index = -1;
   for(int i = 0; i < 5; i++)
   {
    if(pings[i] > temp){ //finds the max
      temp = pings[i];
      index = i;
      }
    }
    Serial.print("Index: ");
    Serial.print(index);
    Serial.println(); 
    if(pings[index] <= stopDistance && pings[index] > 0){ //makes sure that it can turn around if not it backs up
      backUp();
      turnAround();
    }
    else{
    
      if(index == 0){ // if the intal read is greater than the ones from looking around
        turnAround();
      }
      else if(index == 1){ //turn right 90
        turnRight(false);
      }
      else if(index == 2){// turn right 45
        turnRight(true);
      }
      else if(index == 3){// turn left 45
        turnLeft(true);
      }
      else if(index == 4){ //turn left 90
        turnLeft(false);
      }
    }
  } 
  driveForword();
  delay(50);
  
}
//sets the motors to drive forward
void driveForword()
{
  leftMotor(255, true);
  rightMotor(255, true);
}
//sets the motors to drive backwards 
void backUp(){
  leftMotor(225, false);
  rightMotor(225, false);
  delay(100);
}
//true for 45 degrees and false for 90 degrees
void turnRight(bool angle)
{
  leftMotor(225, false);
  rightMotor(225, true);
  if(angle){
    delay(RIGHT_45_DELY);
  }
  else{
    delay(RIGHT_90_DELY);
  }
  leftMotor(0, false);
  rightMotor(0, true);
}
// true for 45 degrees and false for 90 degrees
void turnLeft(bool angle)
{
  leftMotor(225, true);
  rightMotor(225, false);
  if(angle){
    delay(LEFT_45_DELY);
  }
  else{
    delay(LEFT_90_DELY);
  }
  leftMotor(0, false);
  rightMotor(0, true);
}
//spins the robot 180
void turnAround()
{
  leftMotor(225, true);
  rightMotor(225, false);
  delay(LEFT_180_DELY);
  leftMotor(0, true);
  rightMotor(0, true);
  
}
//low level control for the left motor
void leftMotor(byte spd, bool dir)
{
  if(dir)
  {
    digitalWrite(IN2,HIGH);
    digitalWrite(IN1,LOW);
  }
  else
  {
    digitalWrite(IN2,LOW);
    digitalWrite(IN1,HIGH);
  }
  analogWrite(EA,spd);  
}
//low level control for the right motor
void rightMotor(byte spd, bool dir)
{
  if(dir)
  {
    digitalWrite(IN4,LOW);
    digitalWrite(IN3,HIGH);
  }
  else
  {
    digitalWrite(IN4,HIGH);
    digitalWrite(IN3,LOW);
  }
  analogWrite(EB,spd);  
}
// stops robot 
void hardStop()
{
  analogWrite(EB,0);
  analogWrite(EA,0);
  digitalWrite(IN4,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN1,LOW);
}
//used to turn a ping from the ultrasonic in to useable distance
int microsecondsToInches(int microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

//gets a value from the ultra sonic
int getPing()
{
  int duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  return microsecondsToInches(duration) + 1; //ultrasanic is one inch off of the acual messurement
}
