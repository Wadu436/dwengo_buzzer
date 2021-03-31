#include "audio.h"

#define TOP_PWM 200

uint16_t audio_counter = 0;
uint16_t pwmfreq = 0;
uint8_t direction = 1;
uint8_t pwmfreq_mod = 0;
uint8_t shift = 1;
volatile uint8_t _note = 0;

uint16_t abs(int16_t x) {
    return x<0 ? -x : x;
}

//See page 69 of datasheet
ISR(TIMER3_OVF_vect)
{
    audio_counter += shift*notes_lut[_note];
    if (audio_counter >= pwmfreq)
    {
        audio_counter -= pwmfreq;
    }
    OCR3A = lut_sin(audio_counter);
    // OCR3A = abs((audio_counter- 20000)/100);
}

int setup_audio()
{
    // Put Timer3 in Phase/Frequency PWM mode 8
    TCCR3B |= _BV(WGM33);  // WGM33 - 1
    TCCR3B &= ~_BV(WGM32); // WGM32 - 0
    TCCR3A &= ~_BV(WGM31); // WGM31 - 0
    TCCR3A &= ~_BV(WGM30); // WGM30 - 0
    pwmfreq = 40000;
    pwmfreq_mod = 0;
 

    // Enable Timer3 interrupts on BOTTOM
    TIMSK3 |= _BV(TOIE3);

    // Set PWM granularity to TOP
    ICR3 = TOP_PWM;
    OCR3A = 0;

    _enable_interrupt;

    // Select a clock source
    TCCR3B &= ~_BV(CS32);
    TCCR3B &= ~_BV(CS31);
    TCCR3B |= _BV(CS30);

    return 0;
}

// Enable Timer3 output to OC3A
// See page138 of the datasheet
int enable_speaker()
{
    TCCR3A |= _BV(COM3A1);
    TCCR3A &= ~_BV(COM3A0);
    DDRC |= _BV(PC6);
    return 0;
}

int disable_speaker()
{
    TCCR3A &= ~_BV(COM3A1);
    TCCR3A &= ~_BV(COM3A0);
    DDRC &= ~_BV(PC6);
    return 0;
}

int set_note(uint8_t note) {
    _note = note;
    return 0;
}

int set_shift(uint8_t sh) {
    shift = sh;
    return 0;
}

int reset_counter() {
    audio_counter = 0;
    return 0;
}