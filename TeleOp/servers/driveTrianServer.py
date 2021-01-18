
import asyncio
import websockets


async def sever(websockts, path):
    message = await websockets.recv()
    processed_message = message_processer(message)
    send_message_to_motors(message)


def send_message_to_motors(motors, ser):
	right_motor = motors[0]
	left_motor = motors[1]
	message = str(right_motor) + "," + str(left_motor) + ";"
	message.encode()
	ser.write(message)
	


def message_processer(message):
    forword = False
    backword = False
    right = False
    left = False
    fast = False
    right_mortor = 0
    left_motor = 0
    if(message == "[]"):
        return [0,0]
    for ch in message:
        if(ch == 'w'):
            forword = True
        if(ch == 'a'):
            left = True
        if(ch == 's'):
            backword = True
        if(ch == 'd'):
            right = True
        if(ch == 'f'):
            fast = True
        
    if(fast):
        if(forword and not backword):
            if(right and not left):
                #Motor values
            elif(left and not right):
                #Motor values
            else:
                right_mortor = 255
                left_motor = 255
        elif(backword and not forword):
            if(right and not left):
                #Motor values
            elif(left and not right):
                #Motor values
            else:
                right_mortor = -255
                left_motor = -255
        else:
            if(right and not left):
                #Motor values
            elif(left and not right):
                #Motor values
            else:
                right_mortor = 0
                left_motor = 0
    else:
        if(forword and not backword):
            if(right and not left):
                #Motor values
            elif(left and not right):
                #Motor values
            else:
                #Motor values
        elif(backword and not forword):
            if(right and not left):
                #Motor values
            elif(left and not right):
                #Motor values
            else:
                #Motor values
        else:
            if(right and not left):
                #Motor values
            elif(left and not right):
                #Motor values
            else:
                #Motor values
    
    value = [right_mortor, left_motor]
    return value
    



def main():
	
	ser = serial.Serial("/dev/ttyUSB0", 9600, timeout = 1)
	ser.flush()
    

main()
