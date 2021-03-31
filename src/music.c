#include "music.h"

track *_t = NULL;

volatile uint16_t _counter = 0;
volatile uint16_t _ticks = 0;

uint16_t _timing_ticks;

ISR(TIMER0_OVF_vect)
{
    if (++_ticks == _timing_ticks)
    {
        _ticks = 0;
        if (++_counter == _t->count)
            _counter = 0;
        set_note(_t->beats[_counter].note);
        reset_counter();
        _timing_ticks = _t->ticks * _t->beats[_counter].time;
    }
    //reset_counter();
    PORTA = _ticks>>3;
}

int setup_music(track *t)
{
    // Set up timer 0
    // Fast PWM - Mode 7
    // zie pagina 111 datasheet
    TCCR0B |= _BV(WGM02); // 1
    TCCR0A |= _BV(WGM01); // 1
    TCCR0A |= _BV(WGM00); // 1

    // Set TOP
    OCR0A = t->top;

    _t = t;
    _counter = 0;
    _ticks = 0;
    _timing_ticks = _t->ticks * _t->beats[_counter].time;
    set_note(_t->beats[0].note);

    // Clock select see page 112
    TCCR0B |= (_BV(CS00) | _BV(CS02));
    TCCR0B &= ~(_BV(CS01));

    // Enable overflow interrupt
    TIMSK0 |= _BV(TOIE0);

    return 0;
}