#include "main.h"

volatile uint8_t transmetteur = 0b10000000;//bit 0 : flag bit reçu, bit 1 : flag init trame, bits 2-7 : pointeur
Trame message = 0;
uint8_t init = 0;
uint8_t mesure[3]={0,0,0};

int main() {
	//on initialise la transmission série (pour le debug)
	uart_init();

	//On active la sortie sur le bon bit/port
	cbi(OUT_DDR,OUT_BIT);
	//Activation des interruptions d'entree
	sbi(EICRA,ISC01);
	sbi(EICRA,ISC00);
	sbi(EIMSK,INT0);
	while(1) {
		if (transmetteur & 1){
			//Desactivation des interruptions d'entree
			cbi(EIMSK,INT0);
			uint8_t ptn = transmetteur >> 2;
			mesure[0] = (OUT_PIN >> OUT_BIT) & 1;
			_delay_us(PERIOD_RATE/3);
			mesure[1] = (OUT_PIN >> OUT_BIT) & 1;
			_delay_us(PERIOD_RATE/3);
			mesure[2] = (OUT_PIN >> OUT_BIT) & 1;
			if (((mesure[0]+mesure[1]+mesure[2])/2) & 1) {
				sbi(message,ptn);//ce 2 est une grosse magouille...
			}
			else {
				cbi(message,ptn);
			}
			if (transmetteur & (1 << 1)){
				if (ptn>0) {
					transmetteur = ((ptn-1) << 2);
				}
				else {
					transmetteur = (32 << 2);
					printUInt((message >> 19) & 0xFFF);
					printString(" ");
					printUInt((message >> 7) & 0xFFF);
					printString(" ");
					printUShort(message & 0xFF);
					printString(" ");
					uint8_t tmp = checksum((message >> 8) & 0xFFFFFFFF);
					printUShort(tmp);
					printString(" ");
					printlnUShort(tmp == (message & 0xFF));
					message = 0;
				}
			}
			else {		
				if (ptn>24) {
					transmetteur = ((ptn-1) << 2);
				}
				else {
					transmetteur = (32<<2) + 1;
					if ((message >> 24) == SYNC_BYTE) {
						sbi(transmetteur,1);
					}
				}
			}

			//Reactivation des interruptions d'entree
			sbi(EIMSK,INT0);
		}
	}
	return 0;
}

//Interruptions overflow timer2 -> transmission
ISR(INT0_vect) {
	sbi(transmetteur,1);
}

uint8_t checksum(uint32_t data) {
	return ((uint8_t)data + (uint8_t)(data << 7) + (uint8_t)(data << 15) + (uint8_t)(data << 23));
}

/* utilite faible, calcul lourd?, le checksum a été préféré
   uint8_t crc(uint32_t message) {

#define POLYNOMIAL 0xD8000000  // 11011 followed by 0's 
uint32_t  remainder;	
// Initially, the dividend is the remainder.
remainder = message;
// For each bit position in the message....
for (uint8_t bit = 32; bit > 0; --bit)
{
// If the uppermost bit is a 1...
if (remainder & 0x80)
{
// XOR the previous remainder with the divisor.
remainder ^= POLYNOMIAL;
}
// Shift the next bit of the message into the remainder.
remainder = (remainder << 1);
}
// Return only the relevant bits of the remainder as CRC.
return (remainder >> 24);

}*/

