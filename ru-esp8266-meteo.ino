#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <Wire.h>
#include <UTFT.h>
#include <SPI.h>
#include <DS3231.h>//https://github.com/jarzebski/Arduino-DS3231/archive/master.zip
DS3231 vrem;// Связываем объект clock с библиотекой DS3231
RTCDateTime DateTime;// Определяем сущность структуры RTCDateTime (описанной в библиотеке DS3231) для хранения считанных с часов даты и времени
extern uint8_t BigFont[],SmallFont[];
UTFT myGLCD ( ILI9225B, 13, 14, 15, 2, 12 );//SDI (MOSI), SCK, CS, RESET, DC == D7 D5 D8 D3 D6
 
const char* ssid = "rolph";
const char* password = "8c3dbf6715";
char host[] = "export.yandex.ru"; 
const int port = 443;
const char fingerprint[] PROGMEM = "F3 DB F7 38 AA 71 13 15 1E E8 53 FC 0F 29 8C AB 3D F4 80 9F"; 
 
int w=1,i,w1=1,y,y01,y02;
unsigned long times;
String line;
String Link;
 
void setup (  ) { Wire.begin();  vrem.begin();// Инициализируем работу с объектом библиотеки DS3231
  myGLCD.InitLCD(0);myGLCD.fillRect(0,0,320,240);myGLCD.setFont(SmallFont);myGLCD.clrScr();myGLCD.setColor(0,0,0); // первоначальные настройки экрана
 // myGLCD.setBrightness(16);myGLCD.setContrast(64);
  //  vrem.setDateTime(__DATE__, __TIME__); // Устанавливаем время на часах, основываясь на времени компиляции скетча
    Serial.begin(9600); delay(1000);
    WiFi.mode(WIFI_STA); WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {delay(1000);
    myGLCD.setColor(0xffff);myGLCD.print("Connecting...",0,0);}
    myGLCD.print(ssid,0,15);Serial.print("IP address: ");Serial.println(WiFi.localIP());delay(3000);
    myGLCD.fillScr(0x0000);myGLCD.setColor(0x0000);myGLCD.setBackColor(0x0000);
}
 
void loop(){ to_Timer();delay(100);
  if(DateTime.minute==0&&DateTime.second==0||DateTime.minute==30&&DateTime.second==0||w==1){
    i++;
 
    WiFiClientSecure client;
    Serial.printf("Using fingerprint '%s'\n", fingerprint);
     client.setFingerprint(fingerprint);
     client.setTimeout(1000); // 5 Seconds
     delay(1000); while((!client.connect(host, port))){delay(100);}
 
     Link = "/bar/reginfo.xml?region=15090";
     client.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +               
               "Connection: close\r\n\r\n");Serial.println("request sent");delay(1000);
 
  while (client.connected()) {String line = client.readStringUntil('\n');if (line == "\r") {Serial.println("headers received");break;}}
  delay(1000);while (client.available()) {line = client.readStringUntil('\r'); 
 myGLCD.setFont(BigFont);
         myGLCD.setColor(0x0400);  
 
         int a;a =line.indexOf("<temperature class");
         myGLCD.print("T_int",1,90);myGLCD.printNumI(line.substring(a+45, a+90).toInt(),90,90,2);myGLCD.print("C",157,90);myGLCD.drawCircle(155,93,2); 
 //myGLCD.setFont(SmallFont);myGLCD.setColor(200,200,200);myGLCD.print("TEMP_1",10,65);myGLCD.printNumF(vrem.readTemperature(),1,100,65);myGLCD.print("C",190,65);}
 // }
 myGLCD.setColor(0xffff); myGLCD.setFont(SmallFont);
         myGLCD.drawLine(5,115,170,115);
 
         a =line.indexOf("<torr>"); 
         myGLCD.setColor(0xfdfd);myGLCD.print("Presiune",10,125);myGLCD.setColor(0xffff);myGLCD.printNumI(line.substring(a+6, a+15).toInt(),120,125,3);myGLCD.print("mmH",147,125);
 
         a =line.indexOf("<dampness>"); 
         myGLCD.setColor(0xcdcd);myGLCD.print("Umiditate",10,135);myGLCD.setColor(0xffff);myGLCD.printNumI(line.substring(a+10, a+15).toInt(),113,135,3);myGLCD.print("%",147,135);
 
         a =line.indexOf("<wind_speed>"); 
         myGLCD.setColor(0xeded);myGLCD.print("vint",10,145);myGLCD.setColor(0xffff);myGLCD.printNumI(line.substring(a+12, a+15).toInt(),112,145,2);myGLCD.print("m/s",140,145);
 
         a =line.indexOf("<sun_rise>"); 
         myGLCD.setColor(0xdddd);myGLCD.print("Rasarit",10,155);myGLCD.setColor(0xffff);myGLCD.print(line.substring(a+10, a+15),120,155,0);
 
         a =line.indexOf("<sunset>"); 
         myGLCD.setColor(0xdddd);myGLCD.print("Apus",10,165);myGLCD.setColor(0xffff);myGLCD.print(line.substring(a+8, a+13),120,165,0);
 
         a =line.indexOf("<observation>"); 
         myGLCD.setFont(SmallFont);myGLCD.setColor(0xbbbb);myGLCD.setColor(0xffff);myGLCD.print(line.substring(a+13, a+32),CENTER,185,0);
 
        // myGLCD.setColor(0xffff);  myGLCD.drawLine(5,280,160,280);
 
         myGLCD.setFont(SmallFont);
         myGLCD.setColor(0xf000);myGLCD.print("Iasi Romania",CENTER,200);
        myGLCD.setColor(0xcdcd);myGLCD.print("Nr. solicitari:",10,210);myGLCD.printNumI(i,135,210);
  }}w=0;
  }//loop
 
 void to_Timer(){
       DateTime = vrem.getDateTime();   // Считываем c часов текущие значения даты и времени в сущность DateTime
       myGLCD.setFont(BigFont);myGLCD.setColor(0xFFE0);
 
       if(DateTime.hour<10){myGLCD.printNumI(0,27,10);y02=15;}
       else{y02=0;}myGLCD.printNumI(DateTime.hour, 27+y02, 10);myGLCD.print(":", 60, 10,0);
       // минуты
       if(DateTime.minute<10){myGLCD.printNumI(0,75,10);y01=15;}
       else{y01=0;}myGLCD.printNumI(DateTime.minute, 75+y01, 10);myGLCD.print(":", 105, 10,0);
       //секунды
       if(DateTime.second<10){myGLCD.printNumI(0,130,10);y=15;}
       else{y=0;}myGLCD.printNumI(DateTime.second, 130+y, 10);
 
       String time_d = vrem.dateFormat("D d-M-Y", DateTime);
       if(DateTime.second==0||w==1){myGLCD.setFont(SmallFont);myGLCD.setColor(0x8010);myGLCD.print(time_d, 30,35,0);
       myGLCD.setColor(0xffff);myGLCD.drawLine(5,55,170,55);  
       myGLCD.setFont(BigFont);myGLCD.setColor(0xF800);myGLCD.print("T_loc",1,65);myGLCD.printNumF(vrem.readTemperature(),1,90,65);myGLCD.print("C",157,65);myGLCD.drawCircle(155,67,2);  }
  }
