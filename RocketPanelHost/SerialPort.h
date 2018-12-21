/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SerialPort.h
 * Author: Bob
 *
 * Created on October 7, 2018, 10:16 PM
 */

#ifndef SERIALPORT_H
#define SERIALPORT_H

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 32

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialPort {
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    SerialPort(char const *portName);
    //SerialPort(const SerialPort& orig);
    virtual ~SerialPort();
    
    int readSerialPort(byte *buffer, unsigned int buf_size);
    bool writeSerialPort(char *buffer, unsigned int buf_size);
    bool isConnected();
    
private:

};

#endif /* SERIALPORT_H */

