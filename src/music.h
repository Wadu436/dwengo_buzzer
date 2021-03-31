#ifndef MUSIC_H
#define MUSIC_H

#include "stdlib.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "audio.h"

typedef struct beat
{
    uint8_t note;
    uint8_t time;
} beat;

typedef struct track
{
    int count;
    uint16_t top; 
    uint16_t ticks;
    beat *beats;
} track;

int setup_music(track *t);

#endif