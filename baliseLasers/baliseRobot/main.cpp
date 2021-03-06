#include "main.h"

/*
 * définissons quelques variables globales ou define (selon les 
 * besoins) qu'on va utiliser pour le moteur.
 */

//pour la mesure des periodes de rotation du moteur
volatile uint32_t temps[2]={65535,65535};	//contient les périodes de rotation mesurées
volatile uint32_t milAP=0;	//micros après top
volatile uint32_t milAV=0;	//micros avant top

//pour la commande du moteur
uint32_t periode = 70000;	//periode de commande du moteur, en microsecondes
uint16_t incPeriode = 5000;	//periode d'incrémentation de periode pour le démarrage, en microsecondes
uint32_t incTemps = 0;	//stocke le temps écoulé depuis le début pour le démarrage
uint32_t micTemps = 0;	//stocke le temps écoulé depuis le début pour les commutions du moteur
uint8_t ind = 0;	//code la position dans le chronogramme de commande du moteur
/*
   uint8_t pas[4][4] = {
   {1,0,0,1},
   {1,0,1,0},
   {0,1,1,0},
   {0,1,0,1}
   };
   */
uint8_t pas[4][4] = {
	{1,0,0,1},
	{0,1,1,0},
	{1,0,0,1},
	{0,1,1,0}
};


/**
 * et on y va pour la fonction main
 */
int main() {
	//on initialise la transmission série (pour le debug)
	uart_init();
	printlnString("Start");

	//on initialise la notion temporelle sur l'AVR
	temps_init();

	//on initialise l'alimentation des lasers (par pwm)
	lasers_init();//!!!ce n'est que l'initialisation des timers, ça n'allume pas les lasers pour autant!!!

	//on initialise le pwm du moteur
	pwm_init();

	//on initialise les ports de commande du moteur en sortie
	sbi(ddrMot1,pinMot1);
	sbi(ddrMot2,pinMot2);
	sbi(ddrMot3,pinMot3);
	sbi(ddrMot4,pinMot3);

	//on initialise les interruptions du top tour sur front descendant
	sbi(EICRA,ISC11);
	cbi(EICRA,ISC10);
	sbi(EIMSK,INT1);//pour activer les interruptions sur INT1

	//on initialise la pin du sélecteur de sens en input avec pull-up
	cbi(DDRC,pinSens);//input
	sbi(portSens,pinSens);//pull-up

	//et on y va ;)
	printlnString("Go");

	//démarrage : 
	while(42) {

		//calcul de la valeur de periode à utiliser ici (en gros pour le démarrage, on se contente pour l'instant d'une simple rampe) : 
		if (periode > 35000) {
			lasers_start();
			if (micros() > incTemps + incPeriode) {
				incTemps = micros();
				periode -= 30;
			}
			pwmMot1=180;
			pwmMot2=180;
		}
		else if (periode > 20000) {
			if (micros() > incTemps + incPeriode) {
				incTemps = micros();
				periode -= 15;
			}
			pwmMot1=210;
			pwmMot2=210;
		}
		else if (periode > 10000) {
			if (micros() > incTemps + incPeriode) {
				incTemps = micros();
				periode -= 7;
			}
			pwmMot1=220;
			pwmMot2=220;
		}
		else if (periode > 7695) {
			lasers_stop();
			pwmMot1=192;
			pwmMot2=192;
			periode = 7695;
		}
		//commutation des bobines du moteur : 
		if (micros() > micTemps + periode) {
			micTemps = micros();

			printULong(periode);
			printString("\t");
			printUShort(ind);
			printString("\t");
			printlnString("");	

			// et on commute
			commuter(pas[ind]);
			if (ind<3) 		ind++;
			else 			ind = 0;
		}
	}
	return 0;
}

/**
 * fonction de commutation
 */
void commuter ( uint8_t pas_commuter[4] ) {
	if (pas_commuter[0] == 1) 
		sbi(portMot1,pinMot1);
	else 
		cbi(portMot1,pinMot1);
	if (pas_commuter[1] == 1) 
		sbi(portMot2,pinMot2);
	else 
		cbi(portMot2,pinMot2);
	if (pas_commuter[2] == 1) 
		sbi(portMot3,pinMot3);
	else 
		cbi(portMot3,pinMot3);
	if (pas_commuter[3] == 1) 
		sbi(portMot4,pinMot4);
	else 
		cbi(portMot4,pinMot4);

	return;
}

/**
 * fonction d'initialisation du pwm du pont en H. 
 */
void pwm_init() {
	sbi(ddrMotPWM1,pinMotPWM1);	//définie la sortie A du timer0
	sbi(ddrMotPWM2,pinMotPWM2);	//définie la sortie B du timer0

	/*positionner le mode pwm*/
	sbi(TCCR0A,WGM00);	//
	sbi(TCCR0A,WGM01);	//fast-pwm
	cbi(TCCR0B,WGM02);	//

	/*paramétrer le comportement sur comparaisons*/
	sbi(TCCR0A,COM0A1);	//fast-pwm -> OC2A mis à 0 lors d'une comparaison
	cbi(TCCR0A,COM0A0);	//réussie et mis à 1 à passage par BOTTOM
	sbi(TCCR0A,COM0B1);	//fast-pwm -> OC2B mis à 0 lors d'une comparaison
	cbi(TCCR0A,COM0B0);	//réussie et mis à 1 à passage par BOTTOM

	/*sélection de la source du timer*/
	//sbi(TCCR0B,CS02);	//
	//cbi(TCCR0B,CS01);	//prescaler à 256, soit 300Hz (clock à 20MHz)
	//cbi(TCCR0B,CS00);	//
	cbi(TCCR0B,CS02);	//
	sbi(TCCR0B,CS01);	//prescaler à 8, soit 9,76kHz (clock à 20MHz)
	cbi(TCCR0B,CS00);	//

	//régler des valeurs des comp
	pwmMot1=192;
	pwmMot2=192;
	return ;
}

/**
 * fonction du top-tour
 */
ISR(INT1_vect) {
	cli();
	milAP=micros();
	temps[0]=temps[1];
	temps[1]=(milAP-milAV);
	milAV=milAP;
	sei();
}
