
#include <ESP8266WebServer.h>
#include "index.h"  
#include <Servo.h>

#define servoPin 3
#define MOTOR_PWM 1 
#define MOTOR_DIR 2 

Servo myservo;  
int pos = 90; 

#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0
#define PWM_SLOW 170 // arbitrary slow speed PWM duty cycle
#define PWM_FAST 255 // fastest speed PWM duty cycle

const char* ssid = "ESP01_Rider";
const char* password = "********";

ESP8266WebServer server(80);

void setup(void) {

  pinMode(servoPin, OUTPUT);
  pinMode( MOTOR_DIR, OUTPUT );
  pinMode( MOTOR_PWM, OUTPUT );

  myservo.attach(servoPin);
  myservo.write(pos);
  
  analogWrite(MOTOR_DIR, dir_Forward );
  analogWrite(MOTOR_PWM, PWM_STOP );

  //ESP32 As access point IP: 192.168.4.1
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);    

  server.on("/", handle_OnConnect);
  server.on("/A", handle_Forward_Fast);
  server.on("/B", handle_Forward_Slow);
  server.on("/C", handle_Back_Slow);
  server.on("/D", handle_Back_Fast);
  server.on("/off", handle_Stop);

  server.on("/R", handle_Go_Righter);
  server.on("/L", handle_Go_Lefter);
  server.on("/Pos", handle_Steering_Slider_Input);
  server.onNotFound(handle_NotFound);

  server.begin();               
  }


void loop(void) {
  server.handleClient();
}

void handle_NotFound () {
}

void handle_OnConnect() {
  String s = MAIN_page; 
  server.send(200, "text/html", s); 
}

void handle_Steering_Slider_Input() {

  if(server.arg("value") ){
  int slidervalue = server.arg("value").toInt();
  pos = slidervalue;
  myservo.write(slidervalue);
  }
  server.send(200, "text/plain", "ok");
}


void handle_Forward_Fast() {
  analogWrite(MOTOR_DIR, dir_Forward ); 
  analogWrite(MOTOR_PWM, PWM_FAST ); 
  server.send(200, "text/plain", "ok");
}

void handle_Forward_Slow() {
  analogWrite(MOTOR_DIR, dir_Forward ); 
  analogWrite(MOTOR_PWM, PWM_SLOW  ); 
  server.send(200, "text/plain", "ok");
}

void handle_Back_Slow() {
  analogWrite(MOTOR_DIR, dir_Back ); 
  analogWrite(MOTOR_PWM, 320 - PWM_SLOW ); 
  server.send(200, "text/plain", "ok");
}

void handle_Back_Fast() {
  analogWrite(MOTOR_DIR, dir_Back ); 
  analogWrite(MOTOR_PWM, 255 - PWM_FAST ); 
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {
  analogWrite(MOTOR_PWM, PWM_STOP );
  analogWrite(MOTOR_DIR, dir_Forward);
  server.send(200, "text/plain", "ok");
}

void handle_Go_Lefter() { //  Depending on the position and orientation of sg90 servo could change name to Righter
  pos = pos - 7;
  if (pos < 7) {
    pos = 7;
  }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}

void handle_Go_Righter() {
  pos = pos + 7;
  if (pos > 173) {
    pos = 173;
  }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}
