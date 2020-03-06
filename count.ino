#include <MicroGear.h>
#include <ESP8266WiFi.h>
#define WIFI_SSID "true_home2G_907"
#define WIFI_PASSWORD "AXNPJAPN"

#define APPID "countnum"
#define KEY "3HfJRBUfU0EKkM2"
#define SECRET "cn4rCGR54rMY7c1J6fRbqxYka"
#define ALIAS "nodemcu"
#define Taget "num"


int echo1 = D0;
int trig1 = D1;
int echo2 = D7;
int trig2 = D8;


int s=0;
int num=0;
int iin=0;
int iout=0,uplim=94,lowlim=90,timer=0,u1,u2;

long duration , cm;

WiFiClient client;

MicroGear microgear(client);

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}

void setup() {
  microgear.on(CONNECTED,onConnected);
  
  Serial.begin(115200);
  Serial.println("Starting...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(250);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address");
  Serial.println(WiFi.localIP());
  
  microgear.init(KEY,SECRET,ALIAS);
  microgear.connect(APPID);
}
void loop()
{
    u1 = getLength(echo1, trig1); // วัดระยะทาง Ultrasonic ตัวที่ 1
    u2 = getLength(echo2, trig2); // วัดระยะทาง Ultrasonic ตัวที่ 2
    Serial.print(u1);
    Serial.print(" : ");
    Serial.println(u2);
    if(condition(u2)){
    iout=1;
    //Serial.print("do 6");
    }
    if(condition(u1)){
    iin=1;
    //Serial.print("do 5");
    }
    if(condition(u1) && condition(u2)){
      timer+=1;
      if(timer>=80){
        Serial.println("---reset---");
        iin=1;
        iout=1;
        s=0;
        timer=0;
        if (microgear.connected()){
          Serial.println("connected");
        }
        else {
          Serial.println("connection lost, reconnect...");
         microgear.connect(APPID);
        }
        microgear.loop();
        microgear.chat(Taget,num);
      }
    }
    else{
      timer=0;
      check();
    }
    Serial.print(iin);
    Serial.print(" : ");
    Serial.print(iout);
    Serial.print(" : ");
    Serial.println(s);
    Serial.println(num);
}

long getLength(int echo, int trig) {
  pinMode(echo, OUTPUT);
  digitalWrite(echo, 0);
  delayMicroseconds(2);
  digitalWrite(echo, 1);
  delayMicroseconds(5);
  digitalWrite(echo, 0);
  pinMode(trig, INPUT);
  duration = pulseIn(trig, 1);
  cm = microsecondsToCentimeters(duration);
  delay(60);
  return cm;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds/58.2;
}
long check(){
  if(!condition(u1) && s>=0 && iin==1){
    num+=1;
    s+=1;
    iin=0;
    Serial.println("do 1");
    }
  if(!condition(u1) && s<0 && iin==1){
    s+=1;
    iin=0;
    Serial.println("do 2");
    }
  if(!condition(u2) && s<=0 && iout==1){
    num-=1;
    s-=1;
    iout=0;
    Serial.println("do 3");
    }
  if(!condition(u2) && s>0 && iout==1){
    s-=1;
    iout=0;
    Serial.println("do 4");
    }
}
boolean condition(int raya){
  if(raya<=uplim && raya>=lowlim){
    return true;
    }
  else{return false;} 
  }
