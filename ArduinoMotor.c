#include <Wire.h>
#include <Servo.h>
#define motorIn1 5    //左邊馬達 +  //3 5 6 9 10 11才支援analog變速
#define motorIn2 6    //左邊馬達 -
#define motorIn3 10     //右邊馬達 +
#define motorIn4 11     //右邊馬達 -
#define motorWater1 9  //沉水馬達 +
#define motorWater2 8  //沉水馬達 -

#define motorHand 4    //夾子 +
#define motorHand2 3   //夾子 -
#define micro 12       //微動開關
#define redbuttom 7     //紅色開關

Servo myservo; // 建立Servo物件，控制伺服馬達
int temp, val, command, d, one, buttom, grab;

void motorstop() {    //馬達完全停止
 digitalWrite(motorIn1, LOW);
 digitalWrite(motorIn2, LOW);
 digitalWrite(motorIn3, LOW);
 digitalWrite(motorIn4, LOW);
}
void forward() {      //馬達前進
analogWrite(motorIn1, 255);//左馬達
analogWrite(motorIn2, 12);
analogWrite(motorIn3, 255);//右馬達
analogWrite(motorIn4, 0);
}
void backward() {     //馬達後退
 digitalWrite(motorIn1, LOW);
 digitalWrite(motorIn2, HIGH);
 digitalWrite(motorIn3, LOW);
 digitalWrite(motorIn4, HIGH);
}
void right() {        //右轉
 digitalWrite(motorIn1, HIGH);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, 0);
 analogWrite(motorIn4, 255);
}
void left() {         //左轉
 analogWrite(motorIn1, 0);
 analogWrite(motorIn2, 255);
 analogWrite(motorIn3, 255);
 analogWrite(motorIn4, 0);
}
void selftest() {     //測試亂跑
 forward();
 delay(1000);
 motorstop(); delay(500);
 
 backward();
 delay(1000);
 motorstop(); delay(500);
 
 right();
 delay(1000);
 motorstop(); delay(500);
 
 left();
 delay(1000);
 motorstop(); delay(500);  
}
void watering() {     //沉水馬達澆水
 digitalWrite(motorWater1, HIGH);
 digitalWrite(motorWater2, LOW);
}
void waterstop() {    //沉水馬達停止
 digitalWrite(motorWater1, LOW);
 digitalWrite(motorWater2, LOW);
}
void handStop() {     //抓抓停止
 digitalWrite(motorHand, LOW);
 digitalWrite(motorHand2, LOW);
}
void handDown() {     //手臂向下
 analogWrite(motorHand, 180);
 analogWrite(motorHand2, 0);
}
void handUp() {       //手臂向上
 analogWrite(motorHand, 0);
 analogWrite(motorHand2, 180);
}
void microswitch() {  //微動開關
 val = digitalRead(micro);
 if (val == HIGH) {
   if (grab == 0) {
     delay(3150);
     grabb();
     grab = 1;
     d = 5;
     delay(1000);
   }
   else if (grab == 1) {
     delay(3250);
     grab = 2;
     putdown();
   }
 }
}
void redswitch() {    //澆水判斷是否跳過紅色按鈕
 buttom = digitalRead(redbuttom);
 if (buttom == HIGH) {
   if (one == 0) {
     d = 4;
     one = 1;
   }
 }
 else if (buttom == LOW) {
   if (one == 0) {
     d = 0;
     one = 1;
   }
 }
}
void grabb() {        //夾盆
 motorstop();delay(200);
 for(int i = 90; i >= 35; i-=1){ //張開
   myservo.write(i); // 使用write，傳入角度，從0度轉到90度
   delay(10);
 }
 delay(300);
 handDown();delay(2000);
 handStop();delay(300);
 for(int i = 35; i <= 60; i+=1){ //夾住
   myservo.write(i); // 使用write，傳入角度，從180度轉到90度
   delay(10);
 }
 for(int i = 60; i <= 75; i+=1){ //夾住
   myservo.write(i); // 使用write，傳入角度，從180度轉到90度
   delay(40);
 }
 delay(500);
 handUp();delay(3100);
 handStop();delay(500);
 forward();delay(1000);
}
void putdown() {      //放盆
 motorstop();delay(200);
 handDown();delay(2000);
 handStop();delay(500);
 for(int i = 75; i >= 50; i-=1){   //張開
   myservo.write(i); // 使用write，傳入角度，從0度轉到90度
   delay(10);
  }
 delay(500);
 handUp();delay(2000);
 handStop();delay(300);
}
void action() {       //所有來自超音波的指令動作
  switch (command) {
   case 0:  //前進
     forward();delay(50);
     break;
   case 1:  //往左微調
     analogWrite(motorIn4, 150);           //140
     analogWrite(motorIn2, 40);delay(300);
     break;
   case 2:  //往右微調
     analogWrite(motorIn2, 150);
     analogWrite(motorIn4, 40);delay(300);
     break;
   case 3:  //垂直左轉
     right();delay(2450);
     motorstop();delay(500);
     break;
   case 4:  //垂直右轉
     left();delay(2450);
     motorstop();delay(500);
     break;
   case 5:  //澆水囉
     if (d < 3) {
       motorstop();delay(1500);   //先停一下
       watering();delay(2000);    //澆水6.5秒
       waterstop();delay(400);   //停止澆水延遲一秒
       forward();delay(1500);      //向前走0.5秒
       d += 1;
       break;
     }
     else if (d == 3) {
       d = 4;break;
     }
   case 7:  //sin波往右微調
     analogWrite(motorIn4, 65);
     analogWrite(motorIn2, 0);
     delay(50);
     break;
   case 8:  //sin波往左微調
     analogWrite(motorIn4, 0);
     analogWrite(motorIn2, 65 );
     delay(50);
     break;
   case 9:  //終點線停止
     motorstop();delay(20000);
     break;
 }
}
void setup() {
 myservo.attach(2); // 連接數位腳位2，伺服馬達的訊號線
 pinMode(redbuttom, INPUT);
 pinMode(micro, INPUT);
 pinMode(motorIn1, OUTPUT);
 pinMode(motorIn2, OUTPUT);
 pinMode(motorIn3, OUTPUT);
 pinMode(motorIn4, OUTPUT);
 Wire.begin(); // join i2c bus (address optional for master)
 Wire.begin(8);                // join i2c bus with address #8
 Wire.onReceive(receiveEvent); // register event
 Serial.begin(9600);           // start serial for output
}
void loop() {   
 forward();
 redswitch();
 microswitch();
 action();
 
 Wire.beginTransmission(9); // transmit to device #9
 Wire.write(d);       
 Wire.endTransmission();
}
void receiveEvent(int howMany) {  //溝通的function
 while (1 < Wire.available()) { // loop through all but the last
   char c = Wire.read();
 }
 command = Wire.read();    // receive byte as an integer
}
