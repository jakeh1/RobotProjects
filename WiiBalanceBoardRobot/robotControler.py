'''
This script allows for a tank drive robot to be controled by a wii balance bord.
This scrip allows for serial comuncation with an arduino that has the sketch_aug06a
running on it. 

In order to use this code the wii balance board must alreay be conected to the divce 
along with the arduino pluged into a usb port. 

The script will output values that are read in from the balance board and display them in the counsol.
It will also display the calculated vector and angle along with the final moter values sent over serial.

'''


import pygame
import math
import time
import serial 
NUM_OF_AXES = 4


#reads in the values of the four axes 
def read_in(joystick):
	axes = []
	for i in range(NUM_OF_AXES):
		axes.append(joystick.get_axis(i))
	return axes

#converts the information given by the wii balance board into vector angle form.
def convert_to_poler(axes):
	temp1 = -1 * ((axes[0] + axes[1])/2)
	temp2 = -1 * ((axes[2] + axes[3])/2)
	x_vec = temp1 - temp2 #positive value right negiteve value left
	print "::: " + str(x_vec)
	
	temp3 = -1 * ((axes[0] + axes[2])/2)
	temp4 = -1 * ((axes[1] + axes[3])/2)
	y_vec = temp3 - temp4 #positive value forward negiteve value backword
	print "::: " + str(y_vec)
	
	side_1 = 0
	side_2 = 0
	
	if(x_vec < 0):
		side_1 = -1 * x_vec
	else:
		side_1 = x_vec
	if(y_vec < 0):
		side_2 = -1 * y_vec
	else:
		side_2 = y_vec
		
	result_vec = math.sqrt((side_1**2) + (side_2**2))
	if(side_2 == 0):
		angle = 0
	else:
		angle = math.degrees(math.atan(side_1/side_2))
	
	quadrent = -1
	if(x_vec == 0 and y_vec > 0): #directly backward
		quadrent = 'b'
	elif(x_vec == 0 and y_vec < 0): #direcly forword
		quadrent = 'f'
	elif(x_vec > 0 and y_vec == 0): #direcly left
		quadrent = 'l'
	elif(x_vec < 0 and y_vec == 0): #directly right
		quadrent = 'r'
	elif(x_vec > 0 and y_vec > 0): 
		quadrent = 1
	elif(x_vec > 0 and y_vec < 0):
		quadrent = 2
	elif(x_vec < 0 and y_vec > 0):
		quadrent = 4
	elif(x_vec < 0 and y_vec < 0):
		quadrent = 3
	
	return [result_vec, angle, quadrent] #added quadrent since arctan is a value 0-90
#converts a given vector into motor speed
def convert_vec_to_motor(vec):
		MAX_VEC = 0.06
		ratio = 255 / MAX_VEC
		temp = 0
		if(vec > MAX_VEC):
			temp = 0.06
		else:
			temp = vec
		return temp * ratio
#will change the motor speed to traval at the right angle
def convert_angle_to_motor(angle, vec):
	return vec - ((vec/90) * angle)		

def convert_to_motor(poler):
	vec = poler[0]
	angle = poler[1]
	quadrent = poler[2]
	print angle
	right_motor = -1
	left_motor = -1
	if(vec < 0.025):
		right_motor = 0
		left_motor = 0
	else:
		if(quadrent == 'f'): #stright forward
			temp = convert_vec_to_motor(vec)
			right_motor = temp
			left_motor = temp
		elif(quadrent == 'b'): #stright backward
			temp = convert_vec_to_motor(vec)
			right_motor = -1 * temp
			left_motor = -1 * temp
		elif(quadrent == 'l'):
			temp = convert_vec_to_motor(vec) #to the left 90
			right_motor = temp
			left_motor = 0
		elif(quadrent == 'r'): #to the right 90
			temp = convert_vec_to_motor(vec)
			right_motor = 0
			left_motor = temp
		elif(quadrent == 1): # backward left
			right_motor = -1 * convert_vec_to_motor(vec)
			left_motor = -1* convert_vec_to_motor(convert_angle_to_motor(angle, vec))
		elif(quadrent == 2): # forward left
			right_motor = convert_vec_to_motor(vec)
			left_motor =  convert_vec_to_motor(convert_angle_to_motor(angle, vec))
		elif(quadrent == 3): # forward right
			right_motor =  convert_vec_to_motor(convert_angle_to_motor(angle, vec))
			left_motor =  convert_vec_to_motor(vec)
		elif(quadrent == 4): # backward right
			right_motor = -1 * convert_vec_to_motor(convert_angle_to_motor(angle, vec))
			left_motor = -1 * convert_vec_to_motor(vec)
		#speed cap
		if(right_motor > 255):
			right_motor = 255
		elif(right_motor < -255):
			right_motor = -255
		if(left_motor > 255):
			left_motor = 255
		elif(left_motor < -255):
			left_motor = -255
			
	return [int(left_motor), int(right_motor)]
#sends a message over serial	
def send_message(motors, ser):
	right_motor = motors[0]
	left_motor = motors[1]
	message = str(right_motor) + "," + str(left_motor) + ";"
	message.encode()
	ser.write(message)
	#line = ser.readline().decode('utf-8').rstrip()
	#print("serial message::: " + line)
	




def main():
	
	done = False 
	pygame.init()
	pygame.joystick.init()
	joystick = pygame.joystick.Joystick(0)
	joystick.init()
	ser = serial.Serial("/dev/ttyUSB0", 9600, timeout = 1)
	ser.flush()
    
	while(not done):
		
		for event in pygame.event.get(): # User did something.
			if event.type == pygame.JOYBUTTONDOWN: #if user presses the button on the wii balance board the program stops
				print "Program has stoped"
				done = True
		axes = read_in(joystick)
		print axes
		poler = convert_to_poler(axes)
		print poler
		m = convert_to_motor(poler)
		print m
		send_message(m, ser)
		time.sleep(.5)
		
	
		
	
	
	

main()
