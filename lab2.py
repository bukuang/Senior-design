import datetime
from twilio.rest import Client
import time
import serial

arduino = 0
account_sid = 'AC1468678ecaaaa8dcf8cab52e9e976277'
auth_token = 'da6827685a0fabe03a98d4d198ed4257'
fromPhone = '+16122609795'
toPhone = '+13193335351'

###################################################
def sendText(message):
    
    client = Client(account_sid, auth_token)

    message = client.messages.create(body=message,from_=fromPhone,to=toPhone)

    print(message.sid)

###################################################
def send(message):
    global arduino
    arduino.write(message.encode())
    arduino.flush()
    print(message + " sent")

def recieve():
    global arduino
    while arduino.in_waiting < 3:
        time.sleep(.001)
    b = arduino.readline()
    string_n = b.decode()
    decoded_bytes = string_n.rstrip()
    print(decoded_bytes)
    arduino.reset_input_buffer()
    return decoded_bytes

###################################################


textCounter = 0
arduino = serial.Serial('COM3', 9600, timeout=0)
time.sleep(2)
while True:
    status = str(recieve())
    if(status == "1"):
        if(textCounter == 0):
            timeNow = datetime.datetime.now()
            hour = timeNow.hour
            minute = timeNow.minute
            if(hour < 10):
                hourstr = "0" + str(hour)
            else:
                hourstr = str(hour)
            if(minute < 10):
                minutestr = "0" + str(minute)
            else:
                minutestr = str(minute)
            sendText("Critical safety event at " + hourstr + " " + minutestr)
            
            textCounter = textCounter+1

    if(status == "0"):
        textCounter = 0 