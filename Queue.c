#include "Queue.h"
#include "Elevmodule.h"

void queue_init(){
	nOrders=0;
}


void queue_add_order(){ 
	for(int i=0; i< N_FLOORS; i++) {
		if(elev_get_button_signal(BUTTON_COMMAND,i)==1){
			nOrders+=1;
			orders[nOrders].floor=i;
			orders[nOrders].direction=0;
			elev_set_button_lamp(BUTTON_COMMAND, i, 1);
		}
	}
	/*
	for(int i = 0; i < N_FLOORS-1; i++){
		if(elev_get_button_signal(BUTTON_CALL_UP,i)==1){
			nOrders+=1;
			orders[nOrders].floor=i;
			orders[nOrders].direction=1;	
		}
	}
	for(int i = 1; i < N_FLOORS; i++){
		if(elev_get_button_signal(BUTTON_CALL_DOWN,i)==1){		
			nOrders+=1;
			orders[nOrders].floor=i;
			orders[nOrders].direction=-1; 
		}
	}
	*/

}

/* DETTE ER FEIL
void queue_delete_all_orders(){
	for(int i=0; i<nOrders+1; i++){
		orders[i].floor=orderButtons+1;
		orders[i].direction=orderButtons+1;
	}
	nOrders=0;	

}

void queue_delete_order(){
	for(int i=0; i<orderButtons; i++){
		if(elev_get_floor_sensor_signal()==orders[i].floor){
			orders[i].floor=orderButtons+1;
			orders[i].direction=orderButtons+1;
			nOrders-=1;	
		}
	}
}
*/

order queue_next_order(){
	return orders[0];	
}

