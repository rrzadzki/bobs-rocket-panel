import serial

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

while reading:
    # Wait for stop byte
    byte = None
    while ser.read(1) != b'\xff':
        pass
    #print("Found delimiter")
    serial_state = ser.read(2)
    #reading = False

    print(serial_state.hex())

ser.close()
