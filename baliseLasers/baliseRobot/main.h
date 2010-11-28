#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
//#include <util/delay.h>
#include "../../common-AVR/usart.h"
#include "lib/temps.h"
#include "lib/lasers.h"


/**
 * tout d'abord quelques defines pour écrire et lire nos bits plus simplement.
 */
#ifndef sbi
#define sbi(port,bit) (port) |= (1 << (bit))
#endif

#ifndef cbi
#define cbi(port,bit) (port) &= ~(1 << (bit))
#endif

#ifndef rbi
#define rbi(port,bit) ((port & (1 << bit)) >> bit)
#endif

/**
 * ensuite les pins du moteur.
 */
#ifndef PINMOT
#define portTop PORTD
#define pinTop PORTD2 //pin2 de l'arduino
#define portMot1 PORTB 
#define pinMot1 PORTB0 //pin8 de l'arduino
#define portMot2 PORTB 
#define pinMot2 PORTB1 //pin9 de l'arduino
#define portMot3 PORTB 
#define pinMot3 PORTB2 //pin10 de l'arduino
#endif

/**
 * ensuite les pins pour le sélecteur de sens
 */
#ifndef PINSENS
#define portSens PORTC
#define pinSens PINC1 //pin 1 analog de l'arduino
#endif

#endif