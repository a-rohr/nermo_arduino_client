/******************************************************************************
 * NERMO-LITE driver for ESP-01s microcontroller
 * NOTE
 * When implementing on your local environments, ensure to do the following:
 * Setup the correct local network where your mouse is in
 * Set the correct IP address for the ROS-Master
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <math.h>


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SCL 2
#define SDA 0

// IP-Address of the ROS-Master PC
IPAddress server(192, 168, 249, 18);
WiFiClient client;

/******************** SERVO DRIVERS *************************/
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
/**
#define SERVOMIN  400 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  2200 // this is the 'maximum' pulse length count (out of 4096)
*/

// Servomin/max for 60Hz of the PCA - for DSM44
#define SERVOMIN  105 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  650

// Here are the zero positions set for the servos. Adjust these slightly if necessary
#define m1_fl_zero 90
#define m2_fl_zero 90
#define m1_fr_zero 90
#define m2_fr_zero 90

#define m1_rl_zero 110
#define m2_rl_zero 120
#define m1_rr_zero 70
#define m2_rr_zero 60

#define m2_spine_zero 90
#define spine_amp 3

#define tail_zero 90
#define head_zero 90

// Integers that store the values for the degrees if each servo
int m1_fl; int m2_fl; int m1_fr; int m2_fr;
int m1_rl; int m2_rl; int m1_rr; int m2_rr;
int m2_spine; int m1_tail; int m1_head;

/************************ Hardware class to interface with ROSSERIAL ***/
class WiFiHardware {

  public:
  WiFiHardware() {};

  void init() {
    // do your initialization here. this probably includes TCP server/client setup
    client.connect(server, 11411);
  }

  // read a byte from the serial port. -1 = failure
  int read() {
    // implement this method so that it reads a byte from the TCP connection and returns it
    //  you may return -1 is there is an error; for example if the TCP connection is not open
    return client.read();         //will return -1 when it will works
  }

  // write data to the connection to ROS
  void write(uint8_t* data, int length) {
    // implement this so that it takes the arguments and writes or prints them to the TCP connection
    for(int i=0; i<length; i++)
      client.write(data[i]);
  }

  // returns milliseconds since start of program
  unsigned long time() {
     return millis(); // easy; did this one for you
  }
};

// Servo s;
//float q_values[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
int q_values[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
std_msgs::Float32MultiArray temp;

char ssid[] = "LOSTLITTLEROBOT";
char pass[] = "mousewalk";

std_msgs::String return_msg;
ros::Publisher arduino_msgs("arduino", &return_msg);

std_msgs::Int32 return_q1;

std_msgs::Float32MultiArray return_q;
ros::Publisher arduino_q("arduino_q", &return_q1);
ros::NodeHandle_<WiFiHardware> nh;

unsigned long last_time_msg;

void chatterCallback(const std_msgs::Float32MultiArray& msg) {
  for(int i=0; i < 12; i++) {
      q_values[i] = (int)(msg.data[i]*180.0/M_PI);
  }
  
  // return_q1.data = millis() - last_time_msg;
  // return_q.layout.dim_length = 1;
  // return_q.data_length = 12;
  // arduino_q.publish( &return_q1); 
  // last_time_msg = millis();
  // s.write(i);
  servo_commands(); 
}

ros::Subscriber<std_msgs::Float32MultiArray> sub("q_values", &chatterCallback);

unsigned long last_time;
unsigned long last_time_servo;

void setup() {
  // Serial.begin(9600);
  // Serial.println("Setting up node");
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

  //Serial.println("Wifi sucessful");
  
  // s.attach(9);
  nh.initNode();
  delay(2000);
  nh.advertise(arduino_msgs);
  nh.advertise(arduino_q);
  delay(2000);
  return_msg.data = "Node Setup!";
  arduino_msgs.publish( &return_msg); 
  //Serial.println("Hardware node for wireless  communication enabled");
  nh.subscribe(sub);
  return_msg.data = "Subscriber Ready!";
  arduino_msgs.publish( &return_msg); 
  //Serial.println("Subscriber initalized");
  Wire.begin(0,2);
  pwm.begin();
  pwm.setPWMFreq(60);
  last_time = millis();
  last_time_servo = millis();
  last_time_msg = millis();
}

void loop() {
  nh.spinOnce();
  delay(1);
}

void servo_commands() {
      /*** Send servo commands to the pwm multi-channel controller ***/
      // Serial.println("");
      // Serial.print("m1_rl :");
      // Serial.print(q_values[4]);
      // Serial.print(" || m2_rl :");
      // Serial.print(q_values[5]);

      m1_fl = (int)max(min(180, m1_fl_zero + q_values[0]),0);
      m2_fl = (int)max(min(180, m2_fl_zero - q_values[1]),0);
      m1_fr = (int)max(min(180, m1_fr_zero - q_values[2]),0);
      m2_fr = (int)max(min(180, m2_fr_zero + q_values[3]),0);
      
      m1_rl = (int)max(min(180, m1_rl_zero + q_values[4]),0);
      m2_rl = (int)max(min(180, m2_rl_zero - q_values[5]),0);
      m1_rr = (int)max(min(180, m1_rr_zero - q_values[6]),0);
      m2_rr = (int)max(min(180, m2_rr_zero + q_values[7]),0);

      m2_spine = (int)max(min(180, m2_spine_zero + spine_amp*q_values[11]),0);

      m1_tail = (int)max(min(180, tail_zero + q_values[8]),0);
      m1_head = (int)max(min(180, head_zero + q_values[9]),0);

      /*
      Serial.println("Servo sent values || ");
      Serial.print("m1_rl :");
      Serial.print(m1_rl);
      Serial.print(" || m2_rl :");
      Serial.print(m2_rl);*/
      pwm.setPWM(6, 0, angleToPulse(m1_fl) );
      pwm.setPWM(7, 0, angleToPulse(m2_fl) );
      pwm.setPWM(8, 0, angleToPulse(m1_fr) );
      pwm.setPWM(9, 0, angleToPulse(m2_fr) );
      pwm.setPWM(10, 0, angleToPulse(m1_rl) );
      pwm.setPWM(11, 0, angleToPulse(m2_rl) );
      pwm.setPWM(12, 0, angleToPulse(m1_rr) );
      pwm.setPWM(13, 0, angleToPulse(m2_rr) );
      pwm.setPWM(14, 0, angleToPulse(m2_spine) );
      pwm.setPWM(15, 0, angleToPulse(m1_tail) );
      pwm.setPWM(5, 0, angleToPulse(m1_head) );
}

int angleToPulse(int ang){
    /*** Convert angle value to the pulse value for the servo ***/
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
   // Serial.print("Angle: ");Serial.print(ang);
   // Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}