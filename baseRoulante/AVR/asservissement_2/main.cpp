/**
 * \file MainAsservissement.cpp
 * \brief Permet de controler l'asservissement
 *
 * exécute les ordres qui lui sont passées, voici la liste des ordres :
 * "?" pour demaner quelle carte est-tu (ie 0)
 * "a" avance en
 * "b" tourne jusqu'a
 * "e" réinitialise la position
 * "f" f recule en
 * "g" tourne jusqu'a negatif
 * "h" change AssAngle d'état
 * "i" change AssRotation d'état
 * "j" reset
 * "k" change l'accélération maximale en translation
 * "l" change le Vmax pour la translation
 * "m" change le Kp pour la translation
 * "n" stop tout mouvement
 * "o" s'asservit sur place
 * "p" change le PWM pour la transtation
 * "q" change l'accélération maximale en rotation
 * "r" change le Vmax pour la rotation
 * "s" change le Kp pour la rotation 
 * "t" change le PWM pour la rotation
 * "u" change le Kd pour la translation
 * "v" change le Kd pour la rotation
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib/Asservissement.h"
#include "lib/LectureSerie.h"
#include "lib/Manager.h"
#include "lib/Util.h"
#include "lib/twi_master.h"
#include "lib/serial.h"

int main( void )
{   
    // Série
    uart_init();
    // I2C
    TWI_init();
    //Manager
    manager.init();
    // Interruptions
    sei();

    while(1)
    {
		manager.assPolaire();
        //_delay_ms(TEMPS_ASS);
        lectureSerie.traitement();
    }

}
