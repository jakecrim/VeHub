//Includes
#include <stdio.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include "VHconfig.h"
#include <FastLED.h>

/* Defines */
//count of LED's on the current strip
#define LED_STRIP_LENGTH 13
// GPIO pin for each LED
#define LED_FL 23
#define LED_FM 24
#define LED_FR 0
#define LED_STRIP 23

#define LED_CHANNEL_BASE 0
#define FREQ_BASE 5000
#define RESOLUTION_BASE 8 


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