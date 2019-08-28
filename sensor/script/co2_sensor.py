#!/usr/bin/env python
# license removed for brevity
# import cherrypy
import threading
import time
import serial
import rospy
from std_msgs.msg import *

nanoSerial = serial.Serial("/dev/ttyUSB0", 9600) 


def arduino_driver():
    while not rospy.is_shutdown():
        c=''
        co2=''
        c=nanoSerial.read()
        if c=='S':
            c=nanoSerial.read()
            if c=='C':
                c=nanoSerial.read(5)
                if c[4]=='E':
                    co2=c[0:4]
                    co2_int=int(co2,10)
                    print(co2_int)
                    

    
        
def talker():
    
    co2_sensor= rospy.Publisher('co2_sensor',Int16, queue_size=10)
    
   
    
    rate = rospy.Rate(1) # 10hz
    while not rospy.is_shutdown():
        co2_sensor.publish(co2_int)
        rate.sleep()

if __name__ == '__main__':
    rospy.init_node('co2_sensor', anonymous=False)

    try:
        arduino_driver()
    except rospy.ROSInterruptException:
        print 'failed'
        pass

    rospy.spin()