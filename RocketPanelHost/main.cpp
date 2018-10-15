/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Bob
 *
 * Created on October 7, 2018, 10:16 PM
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

char *port_name = "\\\\.\\COM4";

byte incomingData[MAX_DATA_LENGTH];

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    SerialPort arduino(port_name);
    if(arduino.isConnected()) cout << "Connection Established" << endl;
    else cout << "ERROR, check port";
    
    while(arduino.isConnected()){
        int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
		if(read_result>0){
			for(int i=0; i<read_result; i++){
				cout << std::hex << (unsigned int)incomingData[i] << endl;
				int msg_type = (incomingData[i] & 0b10000000) >> 7;
				int state    = (incomingData[i] & 0b01000000) >> 6;
				int  id      = (incomingData[i] & 0b00111111);
				cout << "Type: " << (int) msg_type << " State: " << (int) state << " ID: " << id << endl;
			}
		}
        //puts(incomingData);
        /*
		if(read_result > 2) {
			for(int i=2; i<read_result; i++) {
				if((int)(incomingData[i])==255){
					cout << "Switch " << (int)incomingData[i-2] << ": " << (int)incomingData[i-1] << endl;
				}
			}
		}
		*/
		Sleep(100);
    }
    return 0;
}

