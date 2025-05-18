#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Stepper.h>
#include <EEPROM.h>
#define EEPROM_SIZE 2

#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 5*60*60 + 30*60;
const int   daylightOffset_sec = 3600;
struct tm timeinfo;

const int stepsPerRevolution = 2048;
#define m1IN1 19
#define m1IN2 18
#define m1IN3 5
#define m1IN4 17
Stepper myStepper1(stepsPerRevolution, m1IN1, m1IN3, m1IN2, m1IN4);
int stepper1control;
int stepper1speed;
int hr,mn;

char ssid[] = "IITRPR";
char pass[] = "V#6qF?pyM!bQ$%NX";

TaskHandle_t Task1;

BLYNK_WRITE(V0)
{   
  int value = param.asInt(); // Get value as integer

  if(value)
  digitalWrite(2,HIGH);
  else
  digitalWrite(2,LOW);
}

BLYNK_WRITE(V1)
{   
  stepper1control = param.asInt();
}
BLYNK_WRITE(V2)
{   
  stepper1speed = param.asInt();
  myStepper1.setSpeed(stepper1speed);
}
BLYNK_WRITE(V3) {
  String timeString = param.asStr();
  timeConvert(timeString);
  Serial.print("Scheduled Time - Hour : ");
  Serial.print(hr);
  Serial.print(" Min : ");
  Serial.println(mn);
  EEPROM.write(0,hr);
  EEPROM.write(1,mn);
  EEPROM.commit();
}

void motorHalfRev() {
  myStepper1.step(stepsPerRevolution/2);
}

void timeConvert(String timeString) {
  String seconds = "";
    for(int i=0;i<timeString.length();i++){
      char c = timeString[i];
      if(c>='0' && c<='9')
      seconds = seconds + c;
      else
      break;
    }
  int sec = seconds.toInt();
    hr = sec/3600;
    mn = (sec%3600)/60;
}

void loop2(void * parameter) {
  for( ; ;) {
    int h,m;
  if(!getLocalTime(&timeinfo))
    Serial.println("Failed to obtain time");
  else{
    h = timeinfo.tm_hour;
    m = timeinfo.tm_min;
  }
  if(h==hr && m==mn){
  Serial.println("Its Time");
  motorHalfRev();
  delay(60000);
  }
  delay(5000);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);
  hr = EEPROM.read(0);
  mn = EEPROM.read(1);
  Serial.print("Scheduled Time - Hour : ");
  Serial.print(hr);
  Serial.print(" Min : ");
  Serial.println(mn);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while(!getLocalTime(&timeinfo)){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(100);
  }
  
  xTaskCreatePinnedToCore(
      loop2, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */

  stepper1control = 0;
  stepper1speed = 10;
  Blynk.virtualWrite(V2, stepper1speed);
  myStepper1.setSpeed(stepper1speed);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  if(stepper1control)
  myStepper1.step(stepsPerRevolution/360);
}
