#include "Elevmodule.h"
#include "Timer.h"
#include "Eventmanager.h"
#include "Queue.h"
#include "Statemachine.h"
#include <stdio.h>


enum state{
    IDLE=0,
    RUN = 1,
    EMERGENCY_STOP = 2,
    DOOR_OPEN = 3,
} state, nextstate;

static void elevator_initialize() {
    while(elev_get_floor_sensor_signal() == -1) {
        elev_set_motor_direction(DIRN_UP);
    }
    elev_set_motor_direction(DIRN_STOP);

    for(int i=0; i<orderButtons; i++){
        orders[i].floor=(orderButtons+1);
        orders[i].direction=(orderButtons+1); 
    }

}

void fsm_EMERGENCY_STOP() {
    elev_set_stop_lamp(1); 
    elev_set_motor_direction(DIRN_STOP); 
}

void fsm_IDLE() {
    elev_set_motor_direction(DIRN_STOP); 
    if(elev_get_floor_sensor_signal() != -1){ //stopp i etasje
        elev_set_button_lamp(BUTTON_CALL_UP, elev_get_floor_sensor_signal(), 0); //Skrur av alle lys i etasjen, kan være denne må bli mer komplisert!
        elev_set_button_lamp(BUTTON_CALL_DOWN, elev_get_floor_sensor_signal(), 0);
        elev_set_button_lamp(BUTTON_COMMAND, elev_get_floor_sensor_signal(), 0);
        
        elev_set_floor_indicator(elev_get_floor_sensor_signal());
    }
}

void fsm_RUN() {
    queue_add_order();

    /* setter knappelysene i queue_add_order
    if(queue_next_order().direction == 1){
        elev_set_button_lamp(BUTTON_CALL_UP, queue_next_order().floor, 1);
    }
    if(queue_next_order().direction == 0){
        elev_set_button_lamp(BUTTON_COMMAND, queue_next_order().floor, 1);
    }
    if(queue_next_order().direction == -1){
        elev_set_button_lamp(BUTTON_CALL_DOWN, queue_next_order().floor, 1);
    }
    */ 

    elev_set_motor_direction(direction()); 
}

void fsm_DOOR_OPEN() {
    elev_set_door_open_lamp(1);
    timer_start(); 
    
    //queue_delete_order(); 
    if (ev_time_door_out()) { //venter her helt til døren lukkes
        elev_set_door_open_lamp(0);

    }

}


int main() {
    // Initialize hardware
    

    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elevator_initialize(); //Initialiserer heisen (kjører opp til nærmeste etasje hvis den ikke allerede står i etasje)
    queue_init(); //initialiserer antall bestillinger (nOrders)
    
    elev_set_motor_direction(DIRN_UP);

    state=IDLE;
    nextstate=0;
   
    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }
        
        /*  ødelegger emergency_stop, knappen lyser ikke
        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
        */ 

        
        
        switch(state){
                    case IDLE:
                        if(ev_emergency_button_pushed()==1){
                            nextstate = EMERGENCY_STOP;
                        }
                        else if(ev_order_same_floor() == 1){
                         
                            nextstate = DOOR_OPEN;
                        }
                        else if(ev_check_buttons()==1){
                            
                            nextstate = RUN;
                        }
                        break;
                    case EMERGENCY_STOP:
                        if(ev_emergency_button_pushed()==1){
                            nextstate = EMERGENCY_STOP;
                        }
                        if(ev_emergency_button_pushed()==0){
                            if(elev_get_floor_sensor_signal() != -1){ //er i en etasje
                                //queue_delete_all_orders();
                                elev_set_stop_lamp(0);
                                elev_set_floor_indicator(elev_get_floor_sensor_signal()); //kanskje denne må inn i ev_door_open()
                                nextstate = DOOR_OPEN;
                            }
                            else{
                                //queue_delete_all_orders();
                               
                                nextstate = IDLE; 
                            }
                        }
                        break;
                    case RUN:
                        if(ev_order_same_floor() == 0){
                             
                            nextstate = RUN;
                            break;
                        }
                        if(ev_order_same_floor() == 1){
                             
                            nextstate = IDLE;
                            break;
                        }
                        if(ev_emergency_button_pushed() == 1){
                            nextstate = EMERGENCY_STOP;
                            break;
                        }
                    case DOOR_OPEN:
                    
                        nextstate = IDLE;
                        break;
                    default:
                        break;
        }

        elev_set_door_open_lamp(0);
        if(ev_emergency_button_pushed() == 1){
            nextstate = EMERGENCY_STOP;
        }
        

        if(state != nextstate){
            switch(nextstate){
                case IDLE:
                    fsm_IDLE();
                    break;
                case EMERGENCY_STOP:
                    fsm_EMERGENCY_STOP();
                    break;
                case RUN:
                    fsm_RUN();
                    break;
                case DOOR_OPEN:
                    fsm_DOOR_OPEN();
                    break;
                default:
                    break;
            }
        }
        
        state = nextstate;
        
    }

    return 0;
}
