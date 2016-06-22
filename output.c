#include <xc.h>
#include "output.h"
#include "mcc_generated_files/tmr4.h"
#include "mcc_generated_files/pin_manager.h"

#define MAX_TMR             (8000)

enum state {
    LEFT_RUDDER,
    RIGHT_RUDDER,
    LEFT_MOTOR,
    RIGHT_MOTOR,
    FETCH_DATA
};

static uint8_t state = FETCH_DATA;
static uint16_t data[OUTPUT_CHANNEL_CNT] = {
    NEUTRAL_POS, 
    NEUTRAL_POS, 
    NEUTRAL_POS,
    NEUTRAL_POS
};
static uint16_t buffer[OUTPUT_CHANNEL_CNT] = {
    NEUTRAL_POS, 
    NEUTRAL_POS, 
    NEUTRAL_POS,
    NEUTRAL_POS
};

static void OUTPUT_enable_interrupts(void)
{
    IEC0bits.T3IE = true;
}

static void OUTPUT_disable_interrupts(void)
{
    IEC0bits.T3IE = false;
}

void OUTPUT_set_pin_low(void)
{
    switch(state) {
        case LEFT_RUDDER:
            LEFT_RUDDER_SetLow();
            break;
        case RIGHT_RUDDER:
            RIGHT_RUDDER_SetLow();
            break;
        case LEFT_MOTOR:
            LEFT_MOTOR_SetLow();
            break;
        case RIGHT_MOTOR:
            RIGHT_MOTOR_SetLow();
            break;
        default:
            break;
    }
    TMR4_Stop();
}

static void OUTPUT_set_pin_high(void)
{
    switch(state) {
        case LEFT_RUDDER:
            LEFT_RUDDER_SetHigh();
            break;
        case RIGHT_RUDDER:
            RIGHT_RUDDER_SetHigh();
            break;
        case LEFT_MOTOR:
            LEFT_MOTOR_SetHigh();
            break;
        case RIGHT_MOTOR:
            RIGHT_MOTOR_SetHigh();
            break;
        default:
            break;
    }
}

/* Must only be called by TMR3 callback */
void OUTPUT_update(void)
{
    ++state;
    if (state == 5)
        state = 0;
    
    if (state == FETCH_DATA) {
        uint8_t i;
        for (i = 0; i < OUTPUT_CHANNEL_CNT; ++i)
            data[i] = buffer[i];
    } else {
        TMR4_Counter16BitSet(MAX_TMR - data[state]);
        OUTPUT_set_pin_high();
        TMR4_Start();
    }
}

void OUTPUT_set_data(uint16_t *new_data)
{
    uint8_t i;
    
    OUTPUT_disable_interrupts();
    
    for (i = 0; i < OUTPUT_CHANNEL_CNT; ++i) {
        uint16_t value = new_data[i];
        if (value < MIN_POS)
            value = MIN_POS;
        else if (value > MAX_POS)
            value = MAX_POS;
        buffer[i] = value;
    }
    
    OUTPUT_enable_interrupts();
}