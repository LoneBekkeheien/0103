#include <time.h>
#include <stdio.h>
#include "Eventmanager.h"
#include "Elevmodule.h"
#include "Timer.h"
#include "Queue.h"

int ev_emergency_button_pushed(){
    return elev_get_stop_signal();
}

int ev_time_door_out(){
    return timer_3sec();
}

int ev_check_buttons(){ 
    int x = 0;
    for(int i=0; i< N_FLOORS; i++) {
        if(elev_get_button_signal(BUTTON_COMMAND,i)==1){
           x = 1;
        }
    }
    
    
    for(int i = 0; i < N_FLOORS-1; i++){
        if(elev_get_button_signal(BUTTON_CALL_UP,i)==1){
            x = 1; 
        }
    }
    
    for(int i = 1; i < N_FLOORS; i++){
        if(elev_get_button_signal(BUTTON_CALL_DOWN,i)==1){      
           x = 1;
        }
    }
    
    return x;
    //nOrders;
}

int ev_order_same_floor(){

    for(int i=0; i<nOrders+1; i++){
        if(orders[i].floor==elev_get_floor_sensor_signal()){
            //if(orders[i].direction==0||orders[i].direction==direction()){
                //return 1;
            //}
            return 1;
        }
    } 

    return 0;
}

int direction(){
    if(orders[0].floor>elev_get_floor_sensor_signal()){
        return 1;
    }

    if(orders[0].floor<elev_get_floor_sensor_signal()){
        return -1;
    }   
    return 0;
}

