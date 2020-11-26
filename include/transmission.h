//Includes
#include <stdio.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include "VHconfig.h"

/* Variables */
typedef struct transcieve_message {
	int distance1, distance2, distance3;
	int deviceNum;

	
} transcieve_message;

// Declarations
void visualizeSensors_Open(void);
void vVisualizeSensorsTask(void * parameter);
void vTransmitTask(void * parameter);
void wireless_Open(void);
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void transmit(transcieve_message data);