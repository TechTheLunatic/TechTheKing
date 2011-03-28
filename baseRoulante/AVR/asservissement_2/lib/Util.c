#include "Util.h"

uint8_t buffer[8];


/*
 * Controle du courant des moteurs
 */


int16_t adc_sense1 (void)
{
    // Sélectionne ADC1 pour la lecture
    ADMUX |= 1;
    
    // Démarre la lecture
    ADCSRA |= (1 << ADSC);
    
    // On attend que ADSC passe à 0 (fin de la conversion)
    while (ADCSRA & (1 << ADSC));
    
    // On recompose le résultat et on le renvoie
    return (ADCH | ADCL);
}

int16_t adc_sense2 (void)
{
    // Sélectionne ADC0 pour la lecture
    ADMUX &= ~1;
    
    // Démarre la lecture
    ADCSRA |= (1 << ADSC);
    
    // On attend que ADSC passe à 0 (fin de la conversion)
    PCMSK1 |= (1 << PCINT11);
    while (ADCSRA & (1 << ADSC));
    
    // On recompose le résultat et on le renvoie
    return (ADCH | ADCL);
}


/*
 * Obtention des données des codeuses
 */


int32_t lireBuffer (uint8_t decalage)
{
    int32_t temp;
    int32_t temp2;
    
    temp = buffer[decalage];
    temp2 = buffer[decalage+1];
    temp += (temp2 << 8);
    temp2 = buffer[decalage+2];
    temp += (temp2 << 16);
    temp2 = buffer[decalage+3];
    temp += (temp2 << 24);

    return temp;
}

int32_t getAngle (void)
{
	uint8_t i = 0;
    i2c_requestFrom(2, 8);

    while ( i2c_available() )
    {
        buffer[i] = i2c_receive();
        i++;
    }
    
    return lireBuffer(4);
}

int32_t getDistance (void)
{
	uint8_t i = 0;
    i2c_requestFrom(2, 8);

    while ( i2c_available() )
    {
        buffer[i] = i2c_receive();
        i++;
    }

    return lireBuffer(0);
}
