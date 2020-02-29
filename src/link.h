#ifndef LINK_H
#define LINK_H

#include <gb/gb.h>

struct link_t
{
    volatile UINT8 control;
    volatile UINT8 pad0;
    volatile UINT8 connected; // when data are exchanged succesfully
    volatile UINT8 exchange_completed; // exchange ok

    UINT8 spr_tick;
    UINT8 spr_receive;
    UINT8 pad1;
    UINT8 pad2;

}; // make private

void init_link();

void disable_link();
void enable_link_master();
void enable_link_slave();

UINT8 is_link_connected();

UINT8 is_link_master();
UINT8 is_link_slave();

void link_exchange(UINT8 *send, UINT8* receive, UINT8 len);
UINT8 is_link_exchange_completed();

// new interface
#define LK_SLAVE  0x01
#define LK_MASTER 0x02

void  lk_init(); // init serial system and disconnect
void  lk_disconnect(); 
void  lk_master_connect(); // initialize a transfer as master
void  lk_slave_connect(); // initialize a transfer as slave
UINT8 lk_is_connected(); // return 0 (not connected), LK_SLAVE (connected as slave), LK_MASTER (connected as master) 

void  lk_master_transmit();
void  lk_slave_transmit();
void  lk_transmit();

void  lk_wait_sio_done();

void lk_set_data_buffer_out(UINT8* data, UINT8 len);
UINT8 *lk_get_data_buffer_in();


#endif