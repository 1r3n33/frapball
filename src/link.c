#include <gb/gb.h>
#include "link.h"
#include "graphics.h"

volatile UINT8 lk_connected;
volatile UINT8 lk_connection_state; // LK_SLAVE, LK_MASTER

volatile UINT8 lk_buffer_in[16];
volatile UINT8 lk_buffer_out[16];
volatile UINT8 lk_buffer_len;

volatile UINT8 lk_buffer_cur_in;
volatile UINT8 lk_buffer_cur_out;

void lk_connection_isr() __critical __interrupt(0)
{
    UINT8 r;
 
    r = SB_REG;
 
    if (!lk_connected)
    {
        if (r == LK_MASTER && lk_connection_state == LK_SLAVE)
        {
            //SB_REG = 0;
            //delay(1);
            //SC_REG = 0x80;
            lk_connected = 1;
        }
        else if (r == LK_SLAVE && lk_connection_state == LK_MASTER)
        {
            //SB_REG = 0;
            lk_connected = 1;
        }
        else
        {
            lk_connection_state = 0;
        }
    }
    else
    {
        if (lk_connection_state == LK_MASTER)
        {
            lk_buffer_in[lk_buffer_cur_in] = SB_REG;
            SB_REG = lk_buffer_out[lk_buffer_cur_out];
            lk_buffer_cur_in++;
            lk_buffer_cur_out++;
            if (lk_buffer_cur_in < lk_buffer_len)
            {
                delay(1);
                SC_REG = 0x81;
            }
        }
        else if (lk_connection_state == LK_SLAVE)
        {
            lk_buffer_in[lk_buffer_cur_in] = SB_REG;
            SB_REG = lk_buffer_out[lk_buffer_cur_out];
            lk_buffer_cur_in++;
            lk_buffer_cur_out++;
            if (lk_buffer_cur_in < lk_buffer_len)
            {
                delay(1);
                SC_REG = 0x80;
            }
        }
    }
}

void lk_init()
{
    disable_interrupts();
    add_SIO(lk_connection_isr);
    enable_interrupts();

    lk_disconnect();
}

void lk_disconnect()
{
    IE_REG &= ~SIO_IFLAG;
    lk_connected = 0;
    lk_connection_state = 0;
    lk_buffer_len = 0;
    lk_buffer_cur_in = 0;
    lk_buffer_cur_out = 0;
    SB_REG = 0;
    SC_REG = 0;
}

void lk_slave_connect()
{
    if (lk_connection_state != LK_SLAVE)
    {
        IE_REG |= SIO_IFLAG;
        lk_connection_state = LK_SLAVE;
        lk_buffer_len = 0;
        lk_buffer_cur_in = 0;
        lk_buffer_cur_out = 0;
        SB_REG = LK_SLAVE;
        SC_REG = 0x80;
    }
}

void lk_master_connect()
{
    if (lk_connection_state != LK_MASTER)
    {
        IE_REG |= SIO_IFLAG;
        lk_connection_state = LK_MASTER;
        lk_buffer_len = 0;
        lk_buffer_cur_in = 0;
        lk_buffer_cur_out = 0;
        SB_REG = LK_MASTER;
        SC_REG = 0x81;
    }
}

UINT8 lk_is_connected()
{
    return lk_connected ? lk_connection_state : 0;
}

void lk_transmit()
{
    SB_REG = lk_buffer_out[lk_buffer_cur_out];
    lk_buffer_cur_out++;
    if (lk_connection_state == LK_MASTER)
    {
        SC_REG = 0x81;
    }
    else
    {
        SC_REG = 0x80;
    }
}

void lk_wait_sio_done()
{
    while (lk_buffer_cur_in < lk_buffer_len); // LK_SLAVE only
}

void lk_set_data_buffer_out(UINT8* data, UINT8 len)
{
    UINT8 i;

    for (i=0; i<len; i++)
    {
        lk_buffer_out[i] = data[i];
    }
    for (; i<16; i++)
    {
        lk_buffer_out[i] = 0;
    }

    lk_buffer_len = len;

    lk_buffer_cur_in = 0;
    lk_buffer_cur_out = 0;
}

UINT8 *lk_get_data_buffer_in()
{
    return lk_buffer_in;
}