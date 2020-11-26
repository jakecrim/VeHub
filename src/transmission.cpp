#include "transmission.h"

// Globals
transcieve_message dataDevice1, dataDevice2;

void vVisualizeSensorsTask(void * parameter)
{
	
	for(;;)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		printf("Visualizing received sensor data: \n");
		printf("-----------------------\n");
		printf("\t CURRENT DISTANCES\n");
		printf("F.L. --> %d cm \n", dataDevice1.distance1);
		// printf("F.M. --> %d cm \n", dataDevice1.distance2);
		// printf("F.R. --> %d cm \n", dataDevice1.distance3);
		printf("Rear --> %d cm \n", dataDevice2.distance1);
		printf("-----------------------\n");
	}
}

// When a message comes in from ESP-NOW, handle storage of most recent data packet
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen)
{
	// transcieve_message recieved;
	// memcpy(&recieved, data, sizeof(data));
	// printf("Bytes Recieved: %d \n", dataLen);
	// printf("Distance: %d \n", recieved.distance1);

	transcieve_message tempData;
	// check to see if the incomming message
	memcpy(&tempData, data, dataLen);
	if(tempData.deviceNum == 1)
		memcpy(&dataDevice1, &tempData, sizeof(tempData));
	else if(tempData.deviceNum == 2)
		memcpy(&dataDevice2, &tempData, sizeof(tempData));

	printf("-- Recent Data Packet Debug Stats --- \n");
	printf("Bytes Recieved: %d \n", dataLen);
	printf("Distance: %d \n", tempData.distance1);
	printf("Distance: %d \n", tempData.distance2);
	printf("Distance: %d \n", tempData.distance3);
	printf("Device Number: %d \n", tempData.deviceNum);
	

}

void wireless_Open()
{
	WiFi.mode(WIFI_STA);
	Serial.print("Station Mode: \n");
	Serial.println("MAC Addr: ");
	Serial.print(WiFi.macAddress());
	WiFi.disconnect();
	if (esp_now_init() == ESP_OK)
	{
		printf("\n ESP-NOW Initialized \n");
		esp_now_register_recv_cb(receiveCallback);
		// esp_now_register_send_cb(sent_message);
	}
	else
	{
		printf("ESP-NOW Init Failed...restarting \n");
		delay(500);
		ESP.restart();
	}
	
}

// Main Transmitting Task
void vTransmitTask(void * parameter)
{
	transcieve_message message1;
	for(;;)
	{
		vTaskDelay(2500 / portTICK_PERIOD_MS);
		printf("Transmitting data: \n");
		message1.distance1 = 12;
		transmit(message1);
	}
	vTaskDelete(NULL);
}

void transmit(transcieve_message data)
{
	// Transmits to any device in range
	uint8_t transmitAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	esp_now_peer_info_t peer = {};
	memcpy(&peer.peer_addr, transmitAddr, 6);
	if (!esp_now_is_peer_exist(transmitAddr))
	{
		esp_now_add_peer(&peer);
	}
	esp_err_t result = esp_now_send(transmitAddr, (uint8_t *) &data , sizeof(data));

	if (result == ESP_OK)
	{
		Serial.println("Broadcast message success");
	}
	else if (result == ESP_ERR_ESPNOW_NOT_INIT)
	{
		Serial.println("ESPNOW not Init.");
	}
	else if (result == ESP_ERR_ESPNOW_ARG)
	{
		Serial.println("Invalid Argument");
	}
	else if (result == ESP_ERR_ESPNOW_INTERNAL)
	{
		Serial.println("Internal Error");
	}
	else if (result == ESP_ERR_ESPNOW_NO_MEM)
	{
		Serial.println("ESP_ERR_ESPNOW_NO_MEM");
	}
	else if (result == ESP_ERR_ESPNOW_NOT_FOUND)
	{
		Serial.println("Peer not found.");
	}
	else
	{
		Serial.println("Unknown error");
	}
}
