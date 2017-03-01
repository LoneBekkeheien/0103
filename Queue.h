#include "Elevmodule.h"

int nOrders;

typedef struct order {
	int floor;
	int direction;
} order;

#define orderButtons 1+3*(N_FLOORS-1)

order orders[orderButtons]; 



void queue_init();

void queue_add_order();

void queue_delete_all_orders();

void queue_delete_order();

order queue_next_order();
