#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16
from hardware_abstraction_2.msg import custom_msg
pub_arduino = rospy.Publisher('/data_out_arduino', Int16, queue_size=10)
pub_raspberry = rospy.Publisher('/data_out_raspberry', custom_msg, queue_size=10)


raspberry_data = custom_msg()


# Function for mapping (omzetten naar een andere range) 
def map_custom(x, in_min, in_max, out_min, out_max):
    return int((x-in_min) * (out_max-out_min) / (in_max-in_min) + out_min)

def subscriber():
    sub = rospy.Subscriber('Pot_Value', Int16, callback_function)
    rospy.spin()

def callback_function(message):
    rospy.loginfo("I received: %s"%message.data)
    raspberry_data.servo = message.data

    raspberry_data.led = map_custom(message.data, 0, 460, 460, 0)

    pub_arduino.publish(message.data)
    pub_raspberry.publish(raspberry_data)

if __name__ == "__main__":
    rospy.init_node("Master_Node")
    while not rospy.is_shutdown():
        
        subscriber()