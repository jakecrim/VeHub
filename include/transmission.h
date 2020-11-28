//Includes
#include <stdio.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include "VHconfig.h"

/* Defines */
#define LED_F1 23
#define LED_F2 22


/* Variables */
struct transcieve_message {
	int deviceNum;
	int distance1, distance2, distance3;

	// Constructor
	transcieve_message();	
};

// Declarations
void LED_Open(void);
void vLEDVisualizeTask(void * parameter);
void vSerialVisualizeTask(void * parameter);
void vTransmitTask(void * parameter);
void wireless_Open(void);
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void transmit(transcieve_message data);