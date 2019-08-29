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
    global pm10
    global pm10_int
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
                    co2_sensor.publish(co2_int)
            if c=='P':
                c=nanoSerial.read(11)
                if c[10]=='E':
                    pm10=c[0:2]
                    pm10_int=int(pm10,16)
                    pm25=c[2:4]
                    pm25_int=int(pm25,16)
                    # print type(pm25)
                    pm100=c[4:6]
                    pm100_int=int(pm100,16)
                    # print type(pm100)
                    temp=c[6:8]
                    temp_int=int(temp,16)
                    print (temp_int)
                    hum=c[8:10]
                    hum_int=int(hum,16)
                    # print hum        
                    particulate_sensor_pm10.publish(pm10_int)
                    particulate_sensor_pm25.publish(pm25_int)
                    particulate_sensor_pm100.publish(pm100_int)
                    # print type(temp)
                    temp_sensor.publish(temp_int)
                    hum_sensor.publish(hum_int)
    
        
# def talker():
    
    
    
   
    
#     rate = rospy.Rate(1) # 10hz
#     while not rospy.is_shutdown():
#         co2_sensor.publish(co2_int)
#         rate.sleep()

if __name__ == '__main__':
    rospy.init_node('sensor', anonymous=False)
    co2_sensor= rospy.Publisher('co2_sensor',Int16, queue_size=10)
    particulate_sensor_pm10 = rospy.Publisher('particulate_sensor_pm10', Int16, queue_size=10)
    particulate_sensor_pm25 = rospy.Publisher('particulate_sensor_pm25', Int16, queue_size=10)
    particulate_sensor_pm100= rospy.Publisher('particulate_sensor_pm100', Int16, queue_size=10)
    temp_sensor= rospy.Publisher('temp_sensor', Int16, queue_size=10)
    hum_sensor= rospy.Publisher('hum_sensor', Int16, queue_size=10)
    try:
        arduino_driver()
    except rospy.ROSInterruptException:
        print 'failed'
        pass

    rospy.spin()