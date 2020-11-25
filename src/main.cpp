#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "transmission.h"
#include "visualizeSensors.h"
#include "VHconfig.h"

/* Variables */

//Declarations
esp_now_recv_cb_t receive_message(void);
esp_now_send_cb_t sent_message(void);
void transmit(transcieve_message data);
void tasks_Open();


int main(void)
{
	printf("VeHub System Start: \n");
	tasks_Open();
	visualizeSensors_Open();
	wireless_Open();
	for(;;)
	{
		delay(10000);
		printf("Scheduled FreeRTOS Tasks Running... \n");
	}
}

void tasks_Open()
{
	xTaskCreate(vTransmitTask, "Transmit Task", TRANSMIT_TASK_STACK_SIZE, NULL, TRANSMIT_TASK_PRIO, NULL);
	// xTaskCreate(vVisualizeSensorsTask, "Visualize Sensors Task", VISUALIZE_SENSORS_TASK_STACK_SIZE, NULL, VISUALIZE_SENSORS_TASK_PRIO, NULL);
}

void setup() 
{
	Serial.begin(115200);
	delay(1000);
	main();
}

void loop() 
{
	Serial.println("YOOOO");
	delay(1000);
}