/*右侧车轮（IN1/2)(黑out2红out1)正转朝车后（1 = 1，2 = 0）反转朝车前（1 = 0，2 = 1）
左侧车轮（IN3/4)(红out4灰out3)正转朝车后（3 = 1，4 = 0）反转朝车前（3 = 0，4 = 1）
及在out组线不变的情况下简述为在IN1/3都为0时，灰色IN2 = 1控制右侧朝前，黑色IN4 = 1控制左侧朝前
*/
/*1.0版本：用串口输入的方式控制方向，F 向前，B 向后，L 向左，R 向右，S 向后。*/
/*2.0版本：用旋钮和按钮做外置控制，两个旋钮分别控制左右轮子的速度， 五个按钮分别代表五个状态*/
//----电机的接口
const uint8_t IN1 = 4;
const uint8_t IN2 = 5;
const uint8_t IN3 = 7;
const uint8_t IN4 = 8;
const uint8_t ENA = 3;//右轮调PWM
const uint8_t ENB = 9;//左轮调PWM
//----按钮和旋钮的接口
const uint8_t CSL = A0; //control speed left  这是旋钮控制左侧电机的速度
const uint8_t CSR = A1; //control speed right  旋钮，控制右侧电机的速度
const uint8_t Btf = A2; //button forward
const uint8_t Btb = A3; //button backward
const uint8_t Btl = A4; //button turnlLeaft
const uint8_t Btr = A5; //button right
const uint8_t Bts = 12; //button stop
//----左右电机的速度
int rightS = 0;//右侧电机初始速度为0
int leftS = 0;//左侧电机初始速度为0
//------按键消抖
unsigned long lastTime_b = 0;
const unsigned long button_interval = 50;//设置了50ms的间隔时间

//————电机方向函数
void Forward (){
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);}
void Backward (){
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);}
void turnLeft (){//左转则右轮前左轮后
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);}
void turnRight(){//右转则右轮后左轮前
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);}
void stopCar (){
  digitalWrite(IN1,LOW); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);}

//————————————数字枚举，字替换为数字（方便switch case中直接打字而不是输入数字
enum Carstate { STOP, FORWARD, BACKWARD, LEFT, RIGHT };//给这些字符以数字枚举的方式变为数字，STOP是0，FORWARD是1往后类推
Carstate current_state = STOP;

//——————按键函数
void pinOn(){
  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT); pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);
  pinMode(Btf,INPUT_PULLUP);pinMode(Btb,INPUT_PULLUP);pinMode(Btl,INPUT_PULLUP);pinMode(Btr,INPUT_PULLUP);pinMode(Bts,INPUT_PULLUP);
  }

//——————状态更新函数：1.读取A0 A1的数字模拟输入为多少 2.计算map映射 3.把pwm读取给ENA ENB 4.用switch case写方向 
void updateState (){
 leftS = constrain(map(analogRead(CSL),0,1023,0,255),0,255);
 rightS = constrain(map(analogRead(CSR),0,1023,0,255),0,255);
 if (rightS < 15) {rightS = 0;}
 if (leftS  < 15) {leftS  = 0;} //这两行最开始露了，并不知道有电机死区这一说法
 analogWrite(ENB,leftS); analogWrite(ENA,rightS);
 switch (current_state){
  case STOP :
  stopCar();break;
  case FORWARD :
  Forward();break;
  case BACKWARD :
  Backward() ; break ;
  case LEFT :
  turnLeft(); break;
  case RIGHT :
  turnRight(); break;
  default : break;
 }
}

void setup() {
  pinOn();
  Serial.begin(115200);
  Serial.println(F("开始"));
}

void loop() {
  unsigned long now = millis();
  if (now - lastTime_b >= button_interval){//只有当间隔时间为50ms时才运行下面的
    lastTime_b = now;
    if (digitalRead(Btf) == LOW){
      current_state = FORWARD;//更新状态为FORWARD
      updateState();
      Serial.println(F("按钮前进！")) ; }
    else if (digitalRead(Btb) == LOW){
      current_state = BACKWARD;
      updateState();
      Serial.println(F("按钮后撤步！")) ; }
    else if (digitalRead(Btl) == LOW){
      current_state = LEFT;
      updateState();
      Serial.println(F("按钮左转！")) ; }
    else if (digitalRead(Btr) == LOW){
      current_state = RIGHT;
      updateState();
      Serial.println(F("按钮右转!"));}
    else if (digitalRead(Bts) == LOW){
      current_state = STOP;
      updateState();
      Serial.println(F("按钮刹车！"));}
    //不能再写else了，不然就是按钮没按的时候了
  }

  //对串口通讯功能仍然保留
  if(Serial.available() > 0){
    char a = Serial.read();
   if (a == '\r' || a == '\n') return;
    switch (a){
        case 'F' : current_state = FORWARD; Serial.println(F("串口前进")); break;
        case 'B' : current_state = BACKWARD; Serial.println(F("串口后退")); break;
        case 'S' : current_state = STOP; Serial.println(F("串口停")); break;
        case 'L' : current_state = LEFT; Serial.println(F("串口左转")); break;
        case 'R' : current_state = RIGHT; Serial.println(F("串口右转")); break;
        default : break;
    }
  }
   updateState(); //最开始忘记这个了，加上这个让每轮都可以执行这个指令而不是按钮或者串口有变化的时候才运行
  
}
//因为没有接在arduino的显示器所以按钮那一块的Serial.prinln相当于没有😅，但是万一以后有了呢
