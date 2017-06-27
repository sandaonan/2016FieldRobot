#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

#define trigpinleft 4 //左邊前面的超音波
#define echopinleft 5
#define trigpinleft2 6 //左邊後面的超音波
#define echopinleft2 7
#define trigpinfront 8 //車頭前面的超音波
#define echopinfront 9
#define trigpinright 11 //右邊前面的超音波
#define echopinright 10
#define trigpinright2 13 //右邊後面的超音波
#define echopinright2 12
#define redswitchVolt 3
#define microswitchVolt 2

//陀螺儀宣告
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int hillax, hillay, hillax1;
#define OUTPUT_BINARY_ACCELGYRO

//各種變數宣告
int number, count, d, ending;
float temp;

int hill_1() {              //sin波ax
 accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 return ax;
}
int hill_2() {              //sin波ay
 accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 return ay;
}
float leftdistance() {      //計算左邊後面的距離
 digitalWrite(trigpinleft, LOW);
 delayMicroseconds(2);
 digitalWrite(trigpinleft, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigpinleft, LOW);
 return pulseIn(echopinleft, HIGH) / 58.;
 delay(10);
}
float leftdistance2() {     //計算左邊後面的距離
 digitalWrite(trigpinleft2, LOW);
 delayMicroseconds(2);
 digitalWrite(trigpinleft2, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigpinleft2, LOW);
 return pulseIn(echopinleft2, HIGH) / 58.;
 delay(10);
}
float rightdistance() {     //計算右邊前面的距離
 digitalWrite(trigpinright, LOW);
 delayMicroseconds(2);
 digitalWrite(trigpinright, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigpinright, LOW);
 return pulseIn(echopinright, HIGH) / 58.;
 delay(10);
}
float rightdistance2() {    //計算右邊後面的距離
 digitalWrite(trigpinright2, LOW);
 delayMicroseconds(2);
 digitalWrite(trigpinright2, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigpinright2, LOW);
 return pulseIn(echopinright2, HIGH) / 58.;
 delay(10);
}
int frontdistance() {       //計算車頭的距離
 digitalWrite(trigpinfront, LOW);
 delayMicroseconds(2);
 digitalWrite(trigpinfront, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigpinfront, LOW);
 return pulseIn(echopinfront, HIGH) / 58;
 delay(10);
}

//條件
int condition() {
 //最後終點線停止
 if (d == 5) {
   if (frontdistance() < 37) {
     if (frontdistance() < 37) {
       ending = 9;
       return 9;
     }
   }
 }
 //右邊判斷澆水
 if ((rightdistance() < 60) && (d < 4)) {
   if (rightdistance() < 60) {
     if (rightdistance() < 60) {
       if (count > 20) {
         return 5;
       }
     }
   }
 }
 //垂直左轉
 if ( (frontdistance() < 37) && (hillay / 131 > -15) ) {
   if ( (frontdistance() < 37) && (hillay / 131 > -15) ) {
     if ((rightdistance2() < 50.) || (rightdistance() < 50.)) {
       if ((rightdistance2() < 50.) || (rightdistance() < 50.)) {
         return 4;
       }
     }
   }
 }
 //垂直右轉
 if (d < 5) {
   if ( (frontdistance() < 37) && (hillay / 131 > -15) ) {
     if ( (frontdistance() < 37) && (hillay / 131 > -15) ) {
       if ( (leftdistance2() < 50.) || (leftdistance() < 50.)) {
         if ( (leftdistance2() < 50.) || (leftdistance() < 50.)) {
           return 3;
         }
       }
     }
   }
 }
 // 陀螺儀向左微調
 //if ((hillax / 131 >= 19) && (hillax / 131 <= 28) && (hillay / 131 <= -3) && (hillay / 131 >= -11)) {
 //    return 8;
 //}
 if ((hillax / 131) > 19) {
   return 8;
 }
 // 陀螺儀向右微調
 //if ((hillax / 131 >= 10) && (hillax / 131 <= 12) && (hillay / 131 <= -13) && (hillay / 131 >= -14)) {
//   return 7;
 //}
 if ((hillax / 131) < -13) {
   return 7;
 }
 //左邊判斷微調
 if ((leftdistance() < 50.) && (leftdistance2() < 50.) && (frontdistance() > 30 )) {
   if ((leftdistance2() - leftdistance()) > 1.3) {     //往右微調
     if ((leftdistance2() - leftdistance()) > 1.3) {
       return 1;
     }
   }
   else if ((leftdistance() - leftdistance2()) > 1.3) {  //往左微調
     if ((leftdistance() - leftdistance2()) > 1.3) {
       return 2;
     }
   }
 }
 //右邊判斷微調
 if (d >= 4) {
   if ((rightdistance() < 50.) && (rightdistance2() < 50.) && (frontdistance() > 30 )) {
     if ((rightdistance2() - rightdistance()) > 1.3) {   //往左微調
       if ((rightdistance2() - rightdistance()) > 1.3) {
         return 2;
       }
     }
     else if ((rightdistance() - rightdistance2()) > 1.3) {   //往右微調
       if ((rightdistance() - rightdistance2()) > 1.3) {
         return 1;
       }
     }
   }
   if (rightdistance() < 50.) {
     temp = rightdistance();
     if (rightdistance() < temp) {
       if (rightdistance() < temp) {
         return 2;
       }
     }
   }
 }
 return 0;
}

void setup() {
 #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
   Wire.begin();
 #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
   Fastwire::setup(400, true);
 #endif

 accelgyro.initialize(); //陀螺儀宣告
 pinMode(redswitchVolt, OUTPUT);
 digitalWrite(redswitchVolt, HIGH);
 pinMode(microswitchVolt, OUTPUT);
 digitalWrite(microswitchVolt, HIGH);
 
 pinMode(trigpinleft2, OUTPUT);
 pinMode(echopinleft2, INPUT);
 pinMode(trigpinleft, OUTPUT);
 pinMode(echopinleft, INPUT);
 pinMode(trigpinfront, OUTPUT);
 pinMode(echopinfront, INPUT);
 pinMode(trigpinright, OUTPUT);
 pinMode(echopinright, INPUT);
 pinMode(trigpinright2, OUTPUT);
 pinMode(echopinright2, INPUT);

 Wire.begin(); // join i2c bus (address optional for master)
 Wire.begin(9);                // join i2c bus with address #8
 Wire.onReceive(receiveEvent); // register event
 Serial.begin(9600);           // start serial for output

}

void loop() {
 
 hillax = hill_1();
 hillay = hill_2();
 count += 1;
 
 if (ending == 9) {
   Wire.beginTransmission(8); // transmit to device #8
   Wire.write(ending);
   Wire.endTransmission();
 }
 Wire.beginTransmission(8); // transmit to device #8
 Wire.write(condition());
 Wire.endTransmission();    // stop transmitting


 
}
void receiveEvent(int howMany) {   //溝通的function
 while (1 < Wire.available()) { // loop through all but the last
   char c = Wire.read();
 }
 d = Wire.read();    // receive byte as an integer
}
