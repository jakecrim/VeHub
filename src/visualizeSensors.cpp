#include "visualizeSensors.h"

void visualizeSensors_Open()
{
	printf("Initializing Hardware: \n");
}

void vVisualizeSensorsTask(void * parameter)
{
	for(;;)
	{
		printf("sEnSInG sTuFf \n");
		vTaskDelay(2500 / portTICK_PERIOD_MS);
	}
}