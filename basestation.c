#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "dev/leds.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

/* Declare our "main" process, the basestation_process */
PROCESS(basestation_process, "Clicker basestation");
PROCESS(ledsoff_process, "led basestation");
/* The basestation process should be started automatically when
 * the node has booted. */
AUTOSTART_PROCESSES(&basestation_process,&ledsoff_process);

/* Holds the number of packets received. */
static int count = 0;
static struct etimer et;
static int8_t event = 0;

/* Callback function for received packets.
 *
 * Whenever this node receives a packet for its broadcast handle,
 * this function will be called.
 *
 * As the client does not need to receive, the function does not do anything
 */
static void recv(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest) {
       count++;

    char t[len];
    memcpy(t,data,len);

    if(t[0] == 'b'){
		
                if (event == 2){
		leds_on(0b0111);
		event = 3;}
                else{
                leds_on(0b0010);
                event = 1;}
	 process_poll(&ledsoff_process);}
   
   else if(t[0] == 't'){
		
		if (event == 1){
		leds_on(0b0111);
		event = 3;}
		else{
		leds_on(0b0001);
                event = 2;}
	 process_poll(&ledsoff_process);
}
  
     
    
}

/* Our main process. */
PROCESS_THREAD(basestation_process, ev, data) {
	PROCESS_BEGIN();
	/* Initialize NullNet */
	nullnet_set_input_callback(recv);
	PROCESS_END();
}
PROCESS_THREAD(ledsoff_process, ev, data) {
      PROCESS_BEGIN();
while(1){
      PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_POLL);
      etimer_set(&et,CLOCK_SECOND *10);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
      leds_off(LEDS_ALL);

}
	PROCESS_END();
}
