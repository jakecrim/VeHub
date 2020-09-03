#include "sensors.h"

void sensors_Open()
{
	printf("Initializing Hardware: \n");
}

void vSensorTask(void * parameter)
{
	for(;;)
	{
		printf("sEnSInG sTuFf \n");
		vTaskDelay(2500 / portTICK_PERIOD_MS);
	}
}