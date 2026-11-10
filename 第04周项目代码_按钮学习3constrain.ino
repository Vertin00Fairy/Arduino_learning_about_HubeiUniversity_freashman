/*按住按键1则绿灯渐亮红灯渐暗，按住按键2则相反。两个都按则两个都变暗，两个都不按则在2s后回复原样。
给程序15ms的缓冲时间。  按键50ms的消抖。  灯每步变化5。
不能用delay，学着用constrain*/
const uint8_t Btn1 = 11;
const uint8_t Btn2 = 10;
const uint8_t rled = 6;
const uint8_t gled = 3;
int r_bts = 255;             //设置红灯的初始亮度为255
int g_bts = 0;               //设置绿灯的初始亮度为0
int step = 5;                //灯变化的幅度
const unsigned long bufferTime = 15;         //缓冲时间为15ms
const unsigned long itvTime = 2000;          //按钮都没按住超过2s就回到最初的样子（InTerValTime）
unsigned long lastTime = 0;  //记录上一次变化时的时间用来50ms的消抖
unsigned long buttonTime = 0;//记录上一次按钮变化的时间
void setup() {
  pinMode(Btn1, INPUT_PULLUP);
  pinMode(Btn2, INPUT_PULLUP);
  pinMode(rled,OUTPUT);
  pinMode(gled,OUTPUT);
  analogWrite(rled, r_bts);
  analogWrite(gled, g_bts);  //先让灯亮起来不然像卡了一样
  Serial.begin(9600);
  Serial.println(F("开始"));
}

void loop() {
  unsigned long now = millis();
  if (now - lastTime < bufferTime) { return; }  //如果没到15ms的缓冲时间则跳过后面的loop开始新的
  lastTime = now;
  bool b1 = (digitalRead(Btn1) == LOW);  //储存按键信息
  bool b2 = (digitalRead(Btn2) == LOW);  //按下时判断返回为HIGH（只是符合习惯而已）
  if (b1 && !b2) {                       //按了1不按2绿灯渐亮红灯渐熄
    buttonTime = now;
    g_bts += step;
    r_bts -= step;
    g_bts = constrain(g_bts , 0, 255);
    r_bts = constrain(r_bts , 0, 255);  //通过constrain让数值不要溢出
    analogWrite(gled, g_bts);
    analogWrite(rled, r_bts);
  }
  else if (!b1 && b2) {  //按了2没按1红灯亮
    buttonTime = now;
    r_bts += step;
    g_bts -= step;
    r_bts = constrain(r_bts, 0, 255);
    g_bts = constrain(g_bts, 0, 255);
    analogWrite(rled, r_bts);
    analogWrite(gled, g_bts);
  }
  else if (b1 && b2) {  //两个都按则全部变暗
    buttonTime = now;
    r_bts -= step;
    g_bts -= step;
    r_bts = constrain(r_bts, 0, 255);
    g_bts = constrain(g_bts, 0, 255);
    analogWrite(rled, r_bts);
    analogWrite(gled, g_bts);
  } 
  else {
  if (now - buttonTime >= itvTime) {
    if (r_bts != 255 || g_bts != 0) {      // ← 只有真的需要复位才做
      r_bts = 255;
      g_bts = 0;
      analogWrite(rled, r_bts);
      analogWrite(gled, g_bts);
      Serial.println(F("已复位"));
    }
    buttonTime = now;
    }
  return;
  }

}
//这次没把按键消抖做出来(lll￢ω￢)，因为觉得15ms的缓冲时间已经能够滤掉大部分案件抖动问题，下一条专门练习下。
