#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "notes.h"
#include "audio.h"
#include "music.h"
#include "track.h"

#define MIN_NOTE 0
#define MAX_NOTE 88

#define PWMFREQ 80000

#define TEMPO 100

#define TOP_TIMING 25

#define BASE_N 19

#define TIME_1 8
#define TIME_1_8 1
#define TIME_1_4 2
#define TIME_1_2 4

uint32_t N = 37;

// uint8_t broeder_jacob[] = {NOTE_OFF, C4, NOTE_OFF, D4, NOTE_OFF, E4, NOTE_OFF, C4, NOTE_OFF, C4, NOTE_OFF, D4, NOTE_OFF, E4, NOTE_OFF, C4, NOTE_OFF, E4, NOTE_OFF, F4, NOTE_OFF, G4, NOTE_OFF, E4, NOTE_OFF, F4, NOTE_OFF, G4, NOTE_OFF, G4, NOTE_OFF, A4, NOTE_OFF, G4, NOTE_OFF, F4, NOTE_OFF, E4, NOTE_OFF, C4, NOTE_OFF, G4, NOTE_OFF, A4, NOTE_OFF, G4, NOTE_OFF, F4, NOTE_OFF, E4, NOTE_OFF, C4, NOTE_OFF, C4, NOTE_OFF, G3, NOTE_OFF, C4, NOTE_OFF, C4, NOTE_OFF, G3, NOTE_OFF, C4, NOTE_OFF};
// uint8_t timing[] = {16, TIME_1, TIME_1_4, TIME_1, TIME_1_4, TIME_1, TIME_1_4, TIME_1, TIME_1_2, TIME_1, TIME_1_4, TIME_1, TIME_1_4, TIME_1, TIME_1_4, TIME_1, TIME_1_2, TIME_1, TIME_1_4, TIME_1, TIME_1_4, TIME_1, TIME_1_4 + TIME_1 + TIME_1_4, TIME_1, TIME_1_4, TIME_1, TIME_1_4, TIME_1, TIME_1_4 + TIME_1 + TIME_1_4, TIME_1_2, TIME_1_8, TIME_1_2, TIME_1_8, TIME_1_2, TIME_1_8, TIME_1_2, TIME_1_8, TIME_1, TIME_1_4, TIME_1, TIME_1_8, TIME_1_2, TIME_1_8, TIME_1_2, TIME_1_8, TIME_1_2, TIME_1_8, TIME_1_2, TIME_1_8, TIME_1, TIME_1_4, TIME_1, TIME_1_8, TIME_1, TIME_1_4, TIME_1, TIME_1_4, 2 * TIME_1, TIME_1_4 * 2, TIME_1, TIME_1_4, TIME_1, TIME_1_4, 2 * TIME_1, 32};

// uint8_t note = A4;
uint8_t octaaf = 1;

int main(void)
{
    // create track
    // beat b[65];
    // for (int i = 0; i < 65; i++)
    // {
    //     b[i].note = broeder_jacob[i];
    //     b[i].time = timing[i];
    // }
    // track tr = {65, b};

    DDRA = 0xFF;
    PORTA = 0x00;

    PORTA = setup_audio();
    _enable_interrupt;
    _delay_ms(100);
    set_note(NOTE_OFF);
    enable_speaker();
    PORTA = 0xFF;
    _delay_ms(500);
    PORTA = 0x00;
    set_shift(1);

    // Enable North and South buttons input + pullup
    DDRE &= ~(_BV(PE5) | _BV(PE7) | _BV(PE4) | _BV(PE6));
    PORTE |= _BV(PE5) | _BV(PE7) | _BV(PE4) | _BV(PE6);

    unsigned char prev = PINE, falling_edge;

    setup_music(&compiled_track);
    //set_track(&tr);

    while (1)
    {
        _delay_ms(10);
        falling_edge = (PINE ^ prev) & prev; // Falling edge detection
        prev = PINE;                         // Save current button state for later
        PORTA = 0x00;

        if (falling_edge & _BV(PE5))
        {
            // note = note > MIN_NOTE ? note - 1 : MIN_NOTE; // South
            // set_note(note);
            octaaf = octaaf > 1 ? octaaf - 1 : 1; // South
            PORTA |= _BV(PC0);
            set_shift(octaaf);
        }
        if (falling_edge & _BV(PE7))
        {
            // note = note < MAX_NOTE ? note + 1 : MAX_NOTE; // North
            octaaf = octaaf < 8 ? octaaf + 1 : 8; // North
            // set_note(note);
            PORTA |= _BV(PC1);
            set_shift(octaaf);
        }
        if (falling_edge & _BV(PE4)) // W
        {
            disable_speaker();
            setup_audio();
            enable_speaker();
        }
        if (falling_edge & _BV(PE6)) // E
        {
            disable_speaker();
            setup_audio();
            enable_speaker();
        }
    }

    return 0;
}
