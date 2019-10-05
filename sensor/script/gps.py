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
        gps_time=''

        c=nanoSerial.read()
        if c=='S':
            
            c=nanoSerial.read()
            if c=='G':
                c=nanoSerial.read(32)
                #print(c[31])
                if c[31]=='E':
                    gps_time=c[0:10]
                    Lat=c[10:19]
                    uLat=c[19:20]
                    Lon=c[20:30]
                    uLon=c[30:31]
                    gps_sensor.publish(gps_time)
                    gps_sensor.publish(Lat)
                    gps_sensor.publish(uLat)
                    gps_sensor.publish(Lon)
                    gps_sensor.publish(uLon)
                    
                    #co2_int=int(co2,10)
                    
                    #co2_sensor.publish(co2_int)
                    

    
        
# def talker():
    
    
    
   
    
#     rate = rospy.Rate(1) # 10hz
#     while not rospy.is_shutdown():
#         co2_sensor.publish(co2_int)
#         rate.sleep()

if __name__ == '__main__':
    
    rospy.init_node('gps_sensor', anonymous=False)
    gps_sensor= rospy.Publisher('gps_sensor',String, queue_size=10)
    try:
        arduino_driver()
    except rospy.ROSInterruptException:
        print 'failed'
        pass

    rospy.spin()