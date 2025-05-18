#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Stepper.h>

const int stepsPerRevolution = 2048;
#define m1IN1 19
#define m1IN2 18
#define m1IN3 5
#define m1IN4 17
Stepper myStepper1(stepsPerRevolution, m1IN1, m1IN3, m1IN2, m1IN4);
int stepper1control;
int stepper1speed;

// char ssid[] = "CHENAB";
// char pass[] = "44zMf3QqdU&KC3Mv";
char ssid[] = "WIFI NAME HERE";
char pass[] = "WIFI PASSWORD HERE";

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
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
