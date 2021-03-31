#ifndef AUDIO_H
#define AUDIO_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "notes.h"
#include "lut_sin.h"

#define _enable_interrupt SREG |= _BV(SREG_I)
#define _disable_interrupt SREG &= ~_BV(SREG_I)

int setup_audio();
int enable_speaker();
int disable_speaker();
int set_note(uint8_t note);
int set_shift(uint8_t sh);
int reset_counter();

#endif