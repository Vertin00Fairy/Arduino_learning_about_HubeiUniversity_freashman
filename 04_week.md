# **这周准备小车！**
 <strong>干了什么</strong> 
1. 学习`consttrain` `unit8_t` `analogRead` `enum`
2. 学习了markdown让文本更好看 😄
3. 第一次实际尝试旋钮和`map`来控制pwm
4. 通过了*串口输入*来让小车按照预期动起来
5. 通过按钮实现状态改变从而改变方向，以此不用连接电脑👍👍
<details>
  <summary><strong>踩到什么坑/遇到了什么麻烦:</strong></summary>
  
1. 被 `constrain` 的宏坑了，我直接在宏中进行 `+=  -=` 导致进行了3次运算又致使了数据溢出  😑
2. 电机的死区开始并未有注意导致旋钮扭到某个区间电机会嗡嗡响但是不转
3. 线没有插紧，因此排查了很久💡
4. 给TT马达焊线的时候把马达烧坏了，刚好拆开来看了眼😁
5. **代码问题之**只在状态改变的时候运行了电机运转函数（updateState（）），导致状态不变时不运转

</details>
<details>
  <summary><strong>演示视频链接</strong></summary>

  1. [按钮学习3_constrain篇](https://www.bilibili.com/video/BV1puWT6XET2?vd_source=54b4267072f384e837e5b0820e6a9d28)
  2. [小车演示视频](https://www.bilibili.com/video/BV11usS62EbV/?share_source=copy_web&vd_source=b9b2726705af9eb190cd9165d21e1e1c)

</details>
