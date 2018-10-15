import serial

ser = serial.Serial(port='COM4',
                    baudrate=9600,
                    timeout=2,
                    bytesize=serial.EIGHTBITS)
