/*
   Hardware abstraction level 2
   Walter Nieboer en Rob Nijssen
*/

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int16.h>

ros::NodeHandle  nh;

int sensorPin = A0;
int sensorValue = 0;  // variable to store the value coming from the sensor
int ledPin = 9;      // LED connected to digital pin 9
int newAnalog;
int ReceivedAnalogValue = 0;

void messageCb( const std_msgs::Int16& toggle_msg)
{
  ReceivedAnalogValue = toggle_msg.data;
  newAnalog = map(ReceivedAnalogValue, 0, 460 , 0, 100);
  analogWrite(ledPin, newAnalog);
}

ros::Subscriber<std_msgs::Int16> sub("data_out_arduino", messageCb );
std_msgs::Int16 Analog_msg;
std_msgs::Int16 Received_Analog_msg;
ros::Publisher pot_pub("Pot_Value", &Analog_msg); // waarde van de potentiometer naar master node

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  nh.initNode();
  nh.advertise(pot_pub);
  nh.subscribe(sub);
}

void loop()
{
  sensorValue = analogRead(sensorPin); // uitlezen potmeter op de sensorpin
  Analog_msg.data = sensorValue;
  pot_pub.publish( &Analog_msg );
  nh.spinOnce();
  delay(200); // delay van 200 ms betekent 1000/200 = 5hz
}
