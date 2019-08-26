#!/usr/bin/env python
# license removed for brevity
# import cherrypy
import threading
import time
import serial
import rospy
from std_msgs.msg import *

nanoSerial = serial.Serial("/dev/ttyUSB0", 9600) 
pm10_int=0
pm25_int=0
pm100_int=0
temp_int=0
hum_int=0
def arduino_driver():
    while True:
        global pm10
        global pm10_int
        c=''
        global pm25
        global pm100
        global temp
        global hum
        global pm25_int
        global pm100_int
        global temp_int
        global hum_int
        pm10=''
        pm25=''
        pm100=''
        temp=''
        hum=''
        c=nanoSerial.read()
        if c=='S':
            c=nanoSerial.read()
            if c=='S':
                c=nanoSerial.read(2)
                pm10=c[0:2]
                pm10_int=int(pm10,16)
                c=nanoSerial.read(2)
                pm25=c[0:2]
                pm25_int=int(pm25,16)
                # print type(pm25)
                c=nanoSerial.read(2)
                pm100=c[0:2]
                pm100_int=int(pm100,16)
                # print type(pm100)
                c=nanoSerial.read(2)
                temp=c[0:2]
                temp_int=int(temp,16)
                # print type(temp)
                c=nanoSerial.read(2)
                hum=c[0:2]
                hum_int=int(hum,16)
                # print hum
       
                    

    
        
def talker():
    rospy.init_node('particulate_sensor', anonymous=True)
    particulate_sensor_pm10 = rospy.Publisher('particulate_sensor_pm10', Int16, queue_size=10)
    
    particulate_sensor_pm25 = rospy.Publisher('particulate_sensor_pm25', Int16, queue_size=10)
    # rospy.init_node('particulate_sensor_pm25', anonymous=True)
    particulate_sensor_pm100= rospy.Publisher('particulate_sensor_pm100', Int16, queue_size=10)
    # rospy.init_node('particulate_sensor_pm100', anonymous=True)
    temp_sensor= rospy.Publisher('temp_sensor', Int16, queue_size=10)
    # rospy.init_node('temp_sensor', anonymous=True)
    hum_sensor= rospy.Publisher('hum_sensor', Int16, queue_size=10)
    # rospy.init_node('hum_sensor', anonymous=True)
    
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        particulate_sensor_pm10.publish(pm10_int)
        particulate_sensor_pm25.publish(pm25_int)
        particulate_sensor_pm100.publish(pm100_int)
        # print type(temp)
        temp_sensor.publish(temp_int)
        hum_sensor.publish(hum_int)
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