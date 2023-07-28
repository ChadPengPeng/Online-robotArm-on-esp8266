# Online-robotArm-on-esp8266
基于esp8266的一个物联网机械臂  
主控芯片：esp 8266 D1 R2  
IDE：arduino v1.8.19  
使用库：ESP8266WebServer，ESP8266WiFi，ESP8266mDNS，Math  
其他硬件：9g舵机，舵机拓展板，机械臂零件，详见淘宝：  
https://item.taobao.com/item.htm?spm=a1z09.2.0.0.43962e8d77wY8P&id=653460735801&_u=s20cqprm3dc01e  
使用方法：将mine.ino烧录至开发板，连接wifi：esp8266（密码为12345678，可在代码里修改），通过浏览器打开192.168.1.1进入开发板操作界面：  
<img width="492" alt="image" src="https://github.com/ChadPengPeng/Online-robotArm-on-esp8266/assets/123886366/3909d8d0-af84-4ba7-93b3-5e2a66504490">  
可通过输入参数或拖动窗口内机械臂进行操作，或使用wasd按键操作。  
