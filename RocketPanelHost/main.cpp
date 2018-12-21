/* 
 * File:   main.cpp
 * Author: Bob Rzadzki
 *
 * Created on October 7, 2018, 10:16 PM
 * 
 * Receives packets via serial from an Arduino running RocketPanelFirmware.
 * 
 * TODO: Send those events to Telemachus via websocket. 
 * TODO: Receive events back via websocket.
 * TODO: Send relevant event data to panel.
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bitset>
#include "SerialPort.h"

using std::cout;
using std::endl;

// TODO: Get port name on command line.
char *port_name = "\\\\.\\COM4";

byte incomingData[MAX_DATA_LENGTH];

// TODO: Get debug state from command line.
const bool DEBUG = false;

using namespace std;

unsigned int last_byte;
bool holding_byte = false;

// Open the serial connection? Defaults true, override with --no-serial
bool connect_to_serial = true;

/*
 * 
 */
int main(int argc, char** argv) {
	/* Read command line arguments */
	for(int i=0; i<argc; i++) {
		if(!strcmp(argv[i],"--no-serial")) {
			connect_to_serial = false;
			cout << "--no-serial: Serial connection is disabled." << endl;
		}
	}
	
	if(connect_to_serial) {
		// Open the serial port and report on status.
		SerialPort arduino(port_name);
		if(arduino.isConnected()) cout << "Connection Established" << endl;
		else cout << "ERROR, check port";

		while(arduino.isConnected()){
			int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
			if(read_result>0){
				for(int i=0; i<read_result; i++){
					if(DEBUG) cout << std::hex << (unsigned int)incomingData[i] << endl;
					int msg_type = (incomingData[i] & 0b10000000) >> 7;
					int state    = (incomingData[i] & 0b01000000) >> 6;
					int  id      = (incomingData[i] & 0b00111111);
					if(holding_byte) {
						holding_byte = false;
						int id = incomingData[i];
						cout << "Value of pot " << id << " is " << last_byte << endl;
						continue;
					}
					if(msg_type == 1) {
						// This byte is the value for the switch identified in the following byte.
						if(i<read_result-1) {
							state = (incomingData[i] & 0b01111111); // Drop the MSB type indicator
							int id = incomingData[++i];
							cout << "Value of pot " << id << " is " << state << endl;
						} else {
							// Error
							// cout << "Packet with MSB 1 is last in read" << endl;
							last_byte = incomingData[i] & 0b01111111;
							holding_byte = true;
						}
					} else {
						// This is the value of a switch whose ID is given.
						cout << "Type: " << (int) msg_type << " State: " << (int) state << " ID: " << id << endl;
					}
				}
			}
			Sleep(100);
		}
	}
    return 0;
}

