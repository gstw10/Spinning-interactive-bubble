#include <Gesture.h>
#include <Wire.h>

unsigned long time;
int startTime = 0;
int timeGap = 50; //時間長短
int lastTime = 0;

int testPin = 2;
unsigned char cmd;
Gesture myGesture;

const int pingPin = 8;
const byte speed = 250;

const byte ENA = 12;
const byte INA1 = 10;
const byte INA2 = 11;
#include <Stepper.h>
const int stepsPerRevolution = 1024;
Stepper myStepper(stepsPerRevolution, 3, 5, 4, 6);
boolean spin = false;


void setup() {
  //定義腳位輸出
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);

  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);
  myStepper.setSpeed(10);
  pinMode(testPin, INPUT);
  Serial.write("3D Gesture sensor is now running....\r\n");
}


void loop() {
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(5);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  pinMode(7, INPUT);
  duration = pulseIn(7, HIGH);
  inches = microsecondsToInches(duration);    //duration/74/2
  cm = microsecondsToCentimeters(duration);    //duration/29/2


  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  Serial.print("Time: ");
  time = millis();
  Serial.println(time);


  //判斷距離範圍

  lastTime = ( time - startTime ) / 1000;
  Serial.print("Last Time:");
  Serial.println(lastTime);

  if (cm < 40 and spin == false) {
    startTime = time;
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    spin = true;
    //Serial.print("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOSSSSSSSSSSSSSSSSSSSSSSSSSSTART Time: ");
    //Serial.println(startTime);
  }
  if ( lastTime >= timeGap ) {
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    spin = false;
  }
  /*
    else {

    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    }
  */
  

  if (digitalRead(testPin) == 0) {
    Serial.println("yaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    myGesture.I2C1_MasterRead(md.buf, 26, 0x42); //The address is:0x42
    Serial.println("-----------------------------------");
    cmd = myGesture.mgcProcMsg();     //process the message
    if (cmd != GI_NOGESTURE )
    {
      switch (cmd)
      {   case 1:
          Serial.println("WAVE");
          if (cm < 40 and spin == false) {
            startTime = time;
            spin = true;
          }
          if ( lastTime >= timeGap ) {
            spin = false;
          }
          digitalWrite(10, HIGH);
          digitalWrite(11, LOW);
          delay(2000);
          digitalWrite(10, LOW);
          digitalWrite(11, HIGH);
          delay(500);
          digitalWrite(10, HIGH);
          digitalWrite(11, LOW);
         Serial.println("YOU CAN DO IT");
         
        case GI_FLICK_R:
          Serial.println("RIGHT");
          Serial.println("clockwise");
          myStepper.step(stepsPerRevolution);
          delay(100);

        case GI_FLICK_L:
          Serial.println("LEFT");
          Serial.println("clockwise");
          myStepper.step(stepsPerRevolution);
          delay(100);

        case GI_FLICK_D:
          Serial.println("DOWN");
           if (cm < 40 and spin == false) {
            startTime = time;
            spin = true;
          }
          if ( lastTime >= timeGap ) {
            spin = false;
          }
          digitalWrite(10, HIGH);
          digitalWrite(11, LOW);
          delay(2000);
          digitalWrite(10, LOW);
          digitalWrite(11, HIGH);
          delay(500);
          digitalWrite(10, HIGH);
          digitalWrite(11, LOW);
          Serial.println("YOU CAN DO IT");
          myStepper.step(stepsPerRevolution);

        case GI_FLICK_U:
          Serial.println("UP");
          myStepper.step(stepsPerRevolution);


        case GI_AIRWHEEL_CW://Clockwise in circles
          Serial.println("CW");
           if (cm < 40 and spin == false) {
            startTime = time;
            spin = true;
          }
          if ( lastTime >= timeGap ) {
            spin = false;
          }
          digitalWrite(10, HIGH);
          digitalWrite(11, LOW);

        case GI_AIRWHEEL_CCW://Counterclockwise circles
          Serial.println("CCW");
           if (cm < 40 and spin == false) {
            startTime = time;
            spin = true;
          }
          if ( lastTime >= timeGap ) {
            spin = false;
          }
          digitalWrite(10, LOW);
          digitalWrite(11, HIGH);

        default: break;
      }
    }
  }
  else  {};
}





long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
