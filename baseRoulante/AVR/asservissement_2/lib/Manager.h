#ifndef Manager_h
#define Manager_h

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h> 
#include "Asservissement.h"
#include "Util.h"
#include "serial.h"
#include "Cos.h"

#define PWM_MAX 255
#define NB_MAX_CONSIGNES 100
#define PRESCALER 64
//#define TEMPS_ASS 20000000/(2^16*PRESCALER)
#define TEMPS_ASS 5

/*
 *  Valeurs de comparaison pour le fast PWM
 *    entre 0 et 255
 */
#define MOTEUR2     OCR0A
#define MOTEUR1     OCR2B

/*
 *  Pins de direction des PH
 */
#define PINDIR1 (1 << PORTD4)
#define PINDIR2 (1 << PORTB0)

enum Couleur{ROUGE,BLEU};

extern Couleur CouleurRobot;

class Manager {
    public:
        Manager();
        
        void    init();

        void    pushConsigneAngle(int32_t );
        void    pushConsigneDistance(int32_t );
        void    changeIemeConsigne(int32_t , int32_t ,int16_t i);       
        void    setNbConsignes(int16_t nbConsignesDonne);
        void    changeIemeConsigneDistance (int32_t,int16_t i);
        void    changeIemeConsigneAngle (int32_t,int16_t i);

        void    assPolaire();
        
        void    switchAssDistance();
        void    switchAssAngle();

        void    test();


        void    reset();

        void    stop();
        
        Asservissement  assRotation;
        Asservissement  assTranslation;

        
    
        // Activation de l'asservissement
    
        // Activation de l'asservissement
        bool        activationAssDistance;
        bool        activationAssAngle;

        int32_t distanceTotale;
        int32_t angleTotal;

        volatile int32_t    angleBkp;

        volatile int32_t    distanceBkp;

        typedef struct {
            int32_t distance;
            int32_t angle;
        }Consigne;

        typedef struct {
            uint16_t nbConsignes;
            Consigne listeConsignes[NB_MAX_CONSIGNES] ;
        }TableauConsignes;

        uint16_t consigneActuelle;
        TableauConsignes tableauConsignes;

        void resetListeConsignes();
};

/*
 * Coordonnees du robot
 */ 
 

extern volatile double x;
extern volatile double y;

extern Manager  manager;

#endif
