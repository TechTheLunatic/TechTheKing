/*
 * recepteur.cpp
 */

#include "main.h"

volatile uint8_t transmetteur = 0b11111000;//bit 0 : flag bit reçu, bit 1 : flag init trame, bit 2 : flag fin trame, bits 3-7 : pointeur
//Trame message = 0;
uint32_t message = 0;
uint8_t init = 0;
volatile uint32_t temps[2] = {0,0};
uint32_t mesures=0;
uint32_t nb_bit=0;
uint8_t ind_mesures=0;

/*
uint8_t checksum(uint32_t data) {
	return ((uint8_t)data + (uint8_t)(data << 7) + (uint8_t)(data << 15) + (uint8_t)(data << 23));
}
*/
//et une petite macro pour la route : 
#define checksum(data) ((uint8_t)data + (uint8_t)(data << 7) + (uint8_t)(data << 15) + (uint8_t)(data << 23))

int main() {
	//on initialise la transmission série (pour le debug)
	uart_init();

	//On active la sortie sur le bon bit/port
	cbi(OUT_DDR,OUT_BIT);
	//Activation des interruptions d'entree on CHANGE
	cbi(EICRA,ISC01);
	sbi(EICRA,ISC00);
	sbi(EIMSK,INT0);
	temps[0] = micros();
	while(1) {
		// On travaille en échantillonage
		//si changement de bit 
		//analyse couche 2 ;)
		if (transmetteur & 1) {
			//on prend la main
			cbi(transmetteur,FLAG_INIT_BIT);
			//on stocke le nombre de bits lues et la position où on va les écrire
			nb_bit = (temps[0]-temps[1])/PERIOD_RATE;
			uint8_t ptn = (transmetteur >> FLAG_POINTEUR_MESSAGE);

			//On décale le pointeur de message, et si l'en-tête est bonne, on place le flag pour demarrer l'analyse de la trame.
			if ((ptn <= 24) && (((transmetteur >> FLAG_INIT_TRAME) & 1)==0)) {
				if (((message >> 24) & 0xFF) == SYNC_BYTE) {
					sbi(transmetteur,FLAG_INIT_TRAME);
				}
				transmetteur = (transmetteur & FLAG_MASQUE) + 0xF8;
			}
			else if (ptn <= nb_bit) { //!!!risque d'écraser le début de trame, mise en place d'une petite rustine!!!
				sbi(transmetteur,FLAG_FIN_TRAME);
				cbi(transmetteur,FLAG_INIT_TRAME);
				transmetteur = (transmetteur & FLAG_MASQUE) + 0xF8;
			}
			else {
				transmetteur = (transmetteur & FLAG_MASQUE) + ((ptn-nb_bit) << FLAG_POINTEUR_MESSAGE);
			}

			//on enregistre le nombre de bit correspondant, avec décodage du codage source à la volee.
			uint8_t compteur = nb_bit;
			cbi(message,ptn-compteur);
			compteur--;
			while (nb_bit>0) {
				sbi(message,ptn-compteur);
				compteur--;
			}
			cbi(transmetteur,FLAG_INIT_BIT);
		}
		//fin de trame
		//analyse couche 3 ;)
		if ((transmetteur >> FLAG_FIN_TRAME) & 1){
			uint8_t ptn = transmetteur >> FLAG_POINTEUR_MESSAGE;
			if (ptn>0) {
				transmetteur = ((ptn-1) << FLAG_POINTEUR_MESSAGE);
			}
			else {
				transmetteur = (32 << 2);
				printUInt((message >> 19) & 0xFFF);
				printString(" ");
				printUInt((message >> 7) & 0xFFF);
				printString(" ");
				printUShort(message & 0xFF);
				printString(" ");
				uint8_t tmp = checksum(((message >> 8) & 0xFFFFFFFF));
				printUShort(tmp);
				printString(" ");
				printlnUShort(tmp == (message & 0xFF));
				message = 0;
			}
		}
	}
	return 0;
}

//Interruptions overflow timer2 -> transmission
ISR(INT0_vect) {
	// On informe du changement
	sbi(transmetteur,FLAG_INIT_BIT);
	// On stoque la durée depuis le dernier bit
	temps[1] = temps[0];
	temps[0] = micros();
}

