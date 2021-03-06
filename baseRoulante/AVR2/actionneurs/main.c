#include <util/delay.h>
#include "actionneurs.h"
#include "ax12.h"
#include "twi_slave.h"
#include "adc.h"
//#include "serial.h"

int main()
{
    // Pour le debug serie
    //uart_init();
    
    // Initialisation de la freq de la serie pour AX12
    writeData (0XFE, 0X04, 1, 0X09);
    
    // Initialisations diverses (et variees)
    actio_init();
    
    // Initialisation I2C
    TWI_Init();
    
    //Initialisation de l'adc
    unsigned char conf = 0;
    conf |= ( 1 << ADC_REF0 ) | ( 1 << ADC_CHOOSE ) | ADC1;
    conf &= ~( 1 << ADC_REF1 );
    adcInit(conf);
    
    // Initialisation pour l'AX12 gauche
    AX12Init (AX_ID1, 211, 811, AX_SPEED);
    // Initialisation pour l'AX12 droite
    AX12Init (AX_ID2, 211, 811, AX_SPEED);
    
    // Position initiale aimants
    SERVO_CONS1 = SERVO_PWM_UP1;
    SERVO_CONS2 = SERVO_PWM_UP2;
    
    while(1)
    {
        /*************** Asservissement ***************/
        switch (etat_asservissement) {
            case ASC_ASSERV_SYNCHRO :
                asservissement_synchro();
                break;
            case ASC_ASSERV_INDEP :
                asservissement();
                break;
            case ASC_ASSERV_STOP :
                ASC_MOTEUR1 = 0;
                ASC_MOTEUR2 = 0;
                break;
            case ASC_ASSERV_RECAL :
                PORTD |= DIR1;
                PORTD &= ~DIR2;
                ASC_MOTEUR1 = 100;
                ASC_MOTEUR2 = 100;
                break;
        }
    
    AX12GoTo (AX_ID2, ax_cons2);
    AX12GoTo (AX_ID1, ax_cons1);
    
        /**********************************************/
        
        //          /*** Pour les tests ***/
        //          _delay_ms(2000);
        //          AX12GoTo(AX_ID2, 511);
        //          _delay_ms(2000);
        //          AX12GoTo(AX_ID2, 200);
        //          _delay_ms(2000);
        //          AX12GoTo(AX_ID2,800);
        
        //         _delay_ms(2000);
        //         SERVO_CONS1 = SERVO_PWM_UP1;
        //         _delay_ms(2000);
        //         SERVO_CONS1 = SERVO_PWM_DOWN1;
        
        //         _delay_ms(250);
        //         printLong(ascenseur1);
        //         printString(" ; ");
        //         printlnLong(ascenseur2);
    }
    
    return 0;
}
