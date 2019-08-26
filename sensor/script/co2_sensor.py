#!/usr/bin/env python
# license removed for brevity
# import cherrypy
import threading
import time
import serial
import rospy
from std_msgs.msg import *

nanoSerial = serial.Serial("/dev/ttyUSB0", 9600) 
co2_int=0

def arduino_driver():
    while True:
        global co2
        global co2_int
        c=''
        co2=''
        c=nanoSerial.read()
        if c=='S':
            c=nanoSerial.read()
            if c=='S':
                c=nanoSerial.read(4)
                co2=c[0:4]
                co2_int=int(co2,10)
                print(co2_int)
                    

    
        
def talker():
    rospy.init_node('co2_sensor', anonymous=True)
    co2_sensor= rospy.Publisher('co2_sensor',Int16, queue_size=10)
    
   
    
    rate = rospy.Rate(1) # 10hz
    while not rospy.is_shutdown():
        co2_sensor.publish(co2_int)
        rate.sleep()

if __name__ == '__main__':
    # t = threading.Thread(target=startCherry)
    # t.daemon = True
    # t.start()

    t2 = threading.Thread(target=arduino_driver)
    t2.daemon = True
    t2.start()
    while True:
        try:
            talker()
        except rospy.ROSInterruptException:
            pass