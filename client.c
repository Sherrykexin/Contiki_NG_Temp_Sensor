#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "dev/sensor/sht11/sht11-sensor.h"
#define temperature_Max 74
#define read_interval CLOCK_SECOND/10
/* Declare our "main" process, the client process*/
PROCESS(botton_process, "Clicker client");
PROCESS(temp_process, "temperature process");
/* The client process should be started automatically when
 * the node has booted. */
AUTOSTART_PROCESSES(&botton_process,&temp_process);
static struct etimer et;
static int counter = 0;
/* Callback function for received packets.
 *
 * Whenever this node receives a packet for its broadcast handle,
 * this function will be called.
 *
 * As the client does not need to receive, the function does not do anything
 */
static void recv(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest) {

}

/* Our main process. */
PROCESS_THREAD(botton_process, ev, data) {

	PROCESS_BEGIN();
        static char payload[] = "b";
	/* Activate the button sensor. */
        SENSORS_ACTIVATE(button_sensor);
	/* Initialize NullNet */
	nullnet_buf = (uint8_t *)&payload;
	nullnet_len = sizeof(payload);
	nullnet_set_input_callback(recv);
	/* Loop forever. */
	while (1) {
		/* Wait until an event occurs. If the event has
		 * occured, ev will hold the type of event, and
		 * data will have additional information for the
		 * event. In the case of a sensors_event, data will
		 * point to the sensor that caused the event.
		 * Here we wait until the button was pressed. */
		/* Copy the string "button" into the packet buffer. */
                PROCESS_WAIT_EVENT_UNTIL(data == &button_sensor);
               // printf("sky's button pressed");
		nullnet_buf = (uint8_t *)&payload;
		nullnet_len = sizeof(payload);
		memcpy(nullnet_buf, &payload, sizeof(payload));
		/* Send the content of the packet buffer using the
		 * broadcast handle. */
		NETSTACK_NETWORK.output(NULL);
	}

	PROCESS_END();
}


PROCESS_THREAD(temp_process, ev,data){
   
	PROCESS_BEGIN();
        int converted_temp=0;
	static char payload[]="t";
	int16_t temperature;

        SENSORS_ACTIVATE(sht11_sensor);
	while(1){
	       counter++;	       
 	       etimer_set(&et, CLOCK_SECOND);
               PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
               temperature = sht11_sensor.value(SHT11_SENSOR_TEMP);
               converted_temp = temperature / 100;
               //printf("Temperature %d ºC\n ", converted_temp);
        	   if(converted_temp > temperature_Max){
	   		nullnet_buf = (uint8_t *)&payload;
	       		nullnet_len = sizeof(payload);
              		memcpy(nullnet_buf,&payload,sizeof(payload));         
                        NETSTACK_NETWORK.output(NULL);
			}
                
              
		}      
	PROCESS_END();

	
 }        

