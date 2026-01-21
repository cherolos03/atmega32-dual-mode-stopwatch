#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DEBOUNCE_DELAY 50

volatile unsigned char hours = 0;
volatile unsigned char minutes = 0;
volatile unsigned char seconds = 0;
volatile unsigned char countdown_mode = 0;
volatile unsigned char paused = 0;


void Timer1InitCTCMode(void) {
    TCNT1 = 0;
    TCCR1A = (1 << FOC1A) | (1 << FOC1B);
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A = 15624;
    TIMSK |= (1 << OCIE1A);
}

void output_bcd(unsigned char number, unsigned char segment) {
    PORTC = (PORTC & 0xF0) | (number & 0x0F);
    PORTA = (PORTA & 0xC0) | (1 << segment);
}

void display() {
    unsigned char i;
    unsigned char digits[6] = {
        hours / 10, hours % 10,
        minutes / 10, minutes % 10,
        seconds / 10, seconds % 10
    };
    for(i = 0; i < 6; i++) {
        output_bcd(digits[i], i);
        _delay_ms(2);
        PORTA = 0x00;
    }
}

unsigned char debounce_button(unsigned char pin) {

    if (!(PINB & (1 << pin))) {
        _delay_ms(DEBOUNCE_DELAY);
        if (!(PINB & (1 << pin))) {
            while (!(PINB & (1 << pin))) {
                _delay_ms(10);
            }
            return 1;
        }
    }
    return 0;
}

ISR(INT0_vect) {
    TCNT1 = hours = minutes = seconds = 0;
}

ISR(INT1_vect) {
    TCCR1B &= 0xF8;
    paused = 1;
}

ISR(INT2_vect) {

    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    PORTD &= ~(1 << PD0);
    paused = 0;
}

ISR(TIMER1_COMPA_vect) {
    if (countdown_mode) {
        if (seconds == 0) {
            if (minutes == 0) {
                if (hours == 0) {
                    PORTD |= (1 << PD0);
                    return;
                }
                hours--;
                minutes = 59;
                seconds = 59;
            }
            else {
                minutes--;
                seconds = 59;
            }
        }
        else {
            seconds--;
        }
    }
    else {
        PORTD |= (1 << PD4);
        PORTD &= ~(1 << PD0);
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
                if (hours >= 24) {
                    hours = 0;
                }
            }
        }
    }
}

int main(void) {
    /*  Reset Button */
    DDRD &= ~(1 <<2);
    PORTD |= (1 <<2);
    MCUCR |= (1 << ISC01);
    MCUCR &= ~(1 << ISC00);
    GICR |= (1 << INT0);

    /* Pause Button */
    DDRD &= ~(1 <<3);
    PORTD |= (1 <<3);
    MCUCR |= (1 << ISC11) | (1 << ISC10);
    GICR |= (1 << INT1);

    /* Resume Button */
    DDRB &= ~(1 <<2);
    PORTB |= (1 << 2);
    MCUCSR &= ~(1 << ISC2);
    GICR |= (1 << INT2);

    /* Toggle Button */
    DDRB &= ~(1 <<7);
    PORTB |= (1 <<7);

    /*HoursButtons */
    DDRB &= ~(1 <<1);
    PORTB |= (1 <<1);
    DDRB &= ~(1 << 0);
    PORTB |= (1 << 0);

    /*Minutes Buttons */
    DDRB &= ~(1 <<4);
    PORTB |= (1 <<4);
    DDRB &= ~(1 <<3);
    PORTB |= (1 <<3);

    /*Seconds Buttons */
    DDRB &= ~(1 <<6);
    PORTB |= (1 <<6);
    DDRB &= ~(1 <<5);
    PORTB |= (1 <<5);

    /*LEDs Output */
    DDRD |= (1 <<4);
    DDRD |= (1 <<5);

    /* Buzzer and BCD Display */
    DDRD |= (1 <<0);
    DDRC |= 0x0F;
    PORTC &= 0xF0;
    DDRA |= 0x3F;

    /* Initialize LEDs */
    PORTD |= (1 <<4);
    PORTD &= ~(1 <<5);

    /* Initialize Timer1 */
    Timer1InitCTCMode();

    /* Enable Global Interrupts */
    SREG |=(1<<7);

    while (1) {
        display();

        while(paused) {
            display();

            //Mode Toggle Button
            if (debounce_button(PB7)) {
                countdown_mode ^= 1;

                if (countdown_mode) {
                    PORTD |= (1 << PD5);
                    PORTD &= ~(1 << PD4);
                }
                else {
                    PORTD |= (1 << PD4);
                    PORTD &= ~(1 << PD5);
                }
            }

            //Hours Increment Button
            if (debounce_button(PB1)) {
                hours = (hours + 1) % 24;
            }

            //Hours Decrement Button
            if (debounce_button(PB0)) {
                if (hours == 0) {
                    hours = 23;
                } else {
                    hours--;
                }
            }

            else if (debounce_button(PB4)) {
                if (minutes == 59) {
                    minutes = 0;
                } else {
                    minutes++;
                }
            }

            else if (debounce_button(PB3)) {
                if (minutes == 0) {
                    minutes = 59;
                } else {
                    minutes--;
                }
            }

            else if (debounce_button(PB6)) {
                if (seconds == 59) {
                    seconds = 0;
                } else {
                    seconds++;
                }
            }

            else if (debounce_button(PB5)) {
                if (seconds == 0) {
                    seconds = 59;
                } else {
                    seconds--;
                }
            }
            }
        }
    }

