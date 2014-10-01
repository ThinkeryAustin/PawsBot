#include <Servo.h>
#include <TimedAction.h>

Servo frontServo, backServo; //create two "servo" objects to control the servos
 
const int frontServoPin = 9; //pin on arduino where front servo is connected
const int backServoPin = 10; //pin on arduino where back servo is connected
const int pingPin = 7;       //pin on arduino where PING sensor is connected
 
int frontDefault = 70;  //these values represent the Servos "default" angle,
int backDefault = 65; //perpendicular to the body of the robot
 
int stepAngle = 20;     //angle each set of legs will travel from "default"
int stepDelay = 300;    //delay in milliseconds between each step
 
int backOff = 12;       //distance in inches read by PING sensor before the rebot moves backwards
 
TimedAction danceTimer = TimedAction(60000, dance);
char dancing = 0;
const int danceInterval = 5;  // minutes between dances
const int danceStepInterval = 2000;  // milliseconds between dance steps
 
void setup(){
  frontServo.attach(frontServoPin);
  backServo.attach(backServoPin);
 
  frontServo.write(frontDefault); //set the servos to their default positions
  backServo.write(backDefault);
 
  Serial.begin(9600);
  Serial.println("paws");  // identify the robot to any listening devices
 
  delay(5000);    //wait 5 seconds before walking
}

void loop(){
  long duration, inches;
 
  danceTimer.check();
 
  if (!dancing) {
    //the PING is triggered by a HIGH pulse of 2 or more microseconds
    //give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
   
    //read PING signal to find out how long the echo took (in microseconds)
    //fun fact: there are 1 MILLION microseconds in a second
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);

    //translate time into distance
    inches = microsecondsToInches(duration);
   
    if (inches <= backOff) //if something is closer or equal to the back off distance
      moveBackward();      //move backward
    else                   //if not
      moveForward();       //move forward
  }
 
}
 
//----------------------------MEASURMENT---------------------------
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING, there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
//----------------------------MEASURMENT---------------------------
 
//----------------------------MOVEMENT---------------------------
void moveForward(){
  // 1
  frontServo.write(frontDefault - 30);
  backServo.write(backDefault   + 30);
  delay(400);

  // 2
  frontServo.write(frontDefault + 30);
  backServo.write(backDefault   + 30);
  delay(400);
 
  // 3
  frontServo.write(frontDefault + 30);
  backServo.write(backDefault   - 30);
  delay(400);
 
  // 4
  frontServo.write(frontDefault - 30);
  backServo.write(backDefault   - 30);
  delay(400);
 
  /*
  1: \-/
  2: /-/
  3: /-\
  4: \-\
  */
}
 
void moveBackward(){
  // 1
  frontServo.write(frontDefault + 30);
  backServo.write(backDefault   - 30);
  delay(300);
 
  // 2
  frontServo.write(frontDefault - 30);
  backServo.write(backDefault   + 30);
  delay(300);
 
  // 3
  // frontServo.write(frontDefault + 50);
  // backServo.write(backDefault   - 25);
  // delay(300);
 
  // 4
  // frontServo.write(frontDefault - 30);
  // backServo.write(backDefault   - 25);
  // delay(300);
 
  /*
  1: /-\
  2: /-/
  3: \-/
  4: \-\
  */
}
//----------------------------MOVEMENT---------------------------

void dance() {
  static int minCount = 1;
	
  minCount++;
  if (minCount == danceInterval) {
    dancing = 1;

    Serial.println("d1b");

    // do the dance
    for (int i = 0; i < 10; i++) {
      moveForward();
      delay(danceStepInterval);
      moveBackward();
      delay(danceStepInterval);
    }

    // reset flags
    Serial.println("d1e");
    dancing = 0;
    minCount = 1;
  }
}