import serial

def launch():
	print("LAUNCH")

actions = {
	'04' : {
		'01' : launch
	}
}

try:
	ser = serial.Serial(port='COM4',
						baudrate=9600,
						timeout=2,
						bytesize=serial.EIGHTBITS)
	print("Serial connected")
except:
	print("Serial connection error")
	exit()
	
reading = True
serial_state = []

ser.reset_input_buffer()

count = 0;
while reading:
	b = ser.read(1)
	if b is not None:
		count = count + 1
		print("{} {}".format(count,b.hex()))
	'''
	b = ser.read_until(b'\xff', 32)
	#print(b.hex())
	if len(b) > 0:
		switch = b.hex()[0:2]
		pos    = b.hex()[2:4]
		print("Switch {}: {}".format(switch,pos))
		if switch in actions:
			if pos in actions[switch]:
				actions[switch][pos]()
	'''
	
ser.close()
	