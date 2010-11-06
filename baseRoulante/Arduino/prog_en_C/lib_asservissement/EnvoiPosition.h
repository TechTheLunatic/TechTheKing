#ifndef EnvoiPosition_h
#define EnvoiPosition_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include "lib_avr/HardwareSerial.h"
#include "Manager.h"

#define TRIGGER_BLOCAGE	50

class EnvoiPosition {
	public:
		EnvoiPosition();
		
		void	boucle();
		void 	active();
		void	desactive();
	
	private:
		void 	intToHex(unsigned char *);
		
		bool	actif;
};

extern EnvoiPosition envoiPosition;

#endif
