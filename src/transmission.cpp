#include "transmission.h"

/* Variables */
CRGB visualize_fl[1]; // front left visualizer
CRGB visualize_fm[1]; // front middle visualizer
CRGB visualize_fr[1]; // front right visualizer
CRGB visualize_strip[LED_STRIP_LENGTH]

// Constructor for transcieve_message struct
transcieve_message::transcieve_message()
{
	deviceNum = 0;
	distance1 = 0;
	distance2 = 0;
	distance3 = 0;
}


// Globals
transcieve_message dataDevice1, dataDevice2;

void vLEDVisualizeTask(void * parameter)
{

	for(;;)
	{
		FastLED.clear();
		visualize_fl[0] = CRGB(255,255,255);
		FastLED.show();
		vTaskDelay(250);		
		FastLED.clear();
		visualize_fl[0] = CRGB(255,150,0);
		FastLED.show();
		vTaskDelay(250);		
		FastLED.clear();
		visualize_fl[0] = CRGB(255,0,0);
		FastLED.show();
		vTaskDelay(250);		
	}
}

void vSerialVisualizeTask(void * parameter)
{
	for(;;)
	{
		vTaskDelay(250 / portTICK_PERIOD_MS);
		printf("\rVisualizing received sensor data: \n");
		printf("\r-----------------------\n");
		printf("\r\t CURRENT DISTANCES\n");
		printf("\r F.L. --> %d cm \n", dataDevice1.distance1);
		printf("\r F.M. --> %d cm \n", dataDevice1.distance2);
		printf("\r F.R. --> %d cm \n", dataDevice1.distance3);
		printf("\r Rear --> %d cm \n", dataDevice2.distance1);
		printf("\r-----------------------\n");
		fflush(stdout);
	}
	
}

// When a message comes in from ESP-NOW, handle storage of most recent data packet
void receiveCallback(const uint8_t *macAddr, const uint8_t *data, int dataLen)
{
	transcieve_message tempData;

	// check to see if the incomming message
	memcpy(&tempData, data, dataLen);
	if(tempData.deviceNum == 1)
		memcpy(&dataDevice1, &tempData, sizeof(tempData));
	else if(tempData.deviceNum == 2)
		memcpy(&dataDevice2, &tempData, sizeof(tempData));

	#if DEBUG
		printf("-- Recent Data Packet Debug Stats --- \n");
		printf("Bytes Recieved: %d \n", dataLen);
		printf("Distance: %d \n", tempData.distance1);
		printf("Distance: %d \n", tempData.distance2);
		printf("Distance: %d \n", tempData.distance3);
		printf("Device Number: %d \n", tempData.deviceNum);
	#endif
}

void LED_Open()
{
	printf("Setting up LEDs: \n");
	FastLED.addLeds<WS2812B, LED_STRIP, GRB>(visualize_strip, LED_STRIP_LENGTH);
	// FastLED.addLeds<WS2812B, LED_FM, GRB>(visualize_fm, LED_STRIP_LENGTH);
	// FastLED.addLeds<WS2812B, LED_FR, GRB>(visualize_fr, LED_STRIP_LENGTH);

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
