#ifndef INTERFACES_H
#define INTERFACES_H

#include <iostream>
#include <bitset>
#include <stack>
#include "Point.h"
#include "AStar.h"
#include <SerialPort.h>
#include "Singleton.h"
#include "Thread.h"
#include "config.h"

typedef enum{positif,negatif}SensDeplacement;
typedef enum{bas,haut}ModeAimant;

std::string exec(char* cmd);
class InterfaceAsservissement;
std::vector<char> getTtyUSB();


class InterfaceAsservissement {
public:
	static InterfaceAsservissement* Instance(int precisionAStar=50);
	~InterfaceAsservissement();
    friend void detectionSerieUsb(InterfaceAsservissement* asserv); // ne devrait pas servir si on garde l'i2c
    int getDistanceRobot();
    void goTo(Point arrivee,int nbPoints);
    void avancer(unsigned int distanceMm);
    void reculer(unsigned int distanceMm);
    void tourner(int angleRadian);
	#ifdef DEBUG_GRAPHIQUE
	void debugGraphique();
	#endif
    void debugConsignes();

private:
    InterfaceAsservissement& operator=(const InterfaceAsservissement&);
    InterfaceAsservissement(const InterfaceAsservissement&){};
	InterfaceAsservissement(int precisionAStar);
    void recupPosition();
    int getXRobot();
	int getYRobot();
private:
	int m_compteurImages;
	vector<Point> m_lastTrajectory;
	vector<Consigne> m_lastListeConsignes;
	static InterfaceAsservissement* m_instance;
    AStar m_pathfinding;
    unsigned int vitesseMax;
    SerialStream m_liaisonSerie;
};

// Interface passive : capteurs. A priori, pas besoin de méthode publique autre que ouvrirThread.
class InterfaceCapteurs : public Thread {
public:
    InterfaceCapteurs();
private:
    inline void traiterAbsenceObstacle();
    inline void traiterPresenceObstacle();
    void thread();
private:
};




class InterfaceActionneurs {
    
    public:
        InterfaceActionneurs();
        ~InterfaceActionneurs();
        void hauteurBrasGauche(unsigned char pourcentageHauteur);
        void hauteurBrasDroit(unsigned char pourcentageHauteur);
        void hauteurDeuxBras(unsigned char pourcentageHauteur);
        void angleBrasGauche(unsigned char pourcentageAngle);
        void angleBrasDroit(unsigned char pourcentageAngle);
        void positionAimantGauche(ModeAimant mode);
        void positionAimantDroit(ModeAimant mode);
        
    private:
        // D'un pourcentage à une valeur entre 0 et 15000
        inline unsigned int pourcentageHauteurConversion(unsigned char pourcentage); 
        // D'un pourcentage à une valeur entre 512 et 1023 à envoyer via i2c
        inline unsigned int pourcentageAngleConversion(unsigned char pourcentage);
        
};

void ouvrir_adaptateur_i2c ();

#endif
