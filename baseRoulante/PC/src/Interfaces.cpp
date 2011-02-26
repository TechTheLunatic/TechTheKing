#include "Interfaces.h"

using namespace std;

InterfaceAsservissement* InterfaceAsservissement::m_instance=0;

InterfaceAsservissement* InterfaceAsservissement::instance(){
	if(!m_instance)
		cerr<<"La classe n'a pas d'instance"<<endl;
	return m_instance;
}


vector<char> getTtyUSB(){
	vector<char> portsOuverts;
	string stringTmp = exec((char*)"ls -1 /dev/ttyUSB* | cut -d '/' -f 3 | sed -e 's/ttyUSB//'");
	for(unsigned int i=0;i<stringTmp.length();i++){
		if(stringTmp[i+1]=='\n'){
			portsOuverts.push_back(stringTmp[i]);
		}
	}
	return portsOuverts;
}

string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}

void detectionSerieUsb(InterfaceAsservissement* asserv){
	vector<char> portsOuverts;
	SerialStream streamTmp;
	string stringTmp;
	char charTmp;
 	streamTmp.SetBaudRate( SerialStreamBuf::BAUD_57600 ) ;
 	streamTmp.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
 	streamTmp.SetNumOfStopBits(1) ;
	string listePorts = exec((char*)"ls -1 /dev/ttyUSB* | cut -d '/' -f 3 | sed -e 's/ttyUSB//'");
	for(unsigned int i=0;i<listePorts.length();i++){
		if(listePorts[i+1]=='\n'){
			stringTmp="/dev/ttyUSB";
			stringTmp.push_back( listePorts[i] );
			streamTmp.Open( stringTmp );
			sleep(2);
			streamTmp << "?" << endl;
			streamTmp >> charTmp ;
			cout<<charTmp<<endl;
			switch(charTmp){
				case '0':
					asserv->m_liaisonSerie.Open(stringTmp);
					cout<<"Asservissement : ok"<<endl;
					break;
				case '1':
					asserv->m_liaisonSerie.Open(stringTmp);
					cout<<"Actionneurs : ok"<<endl;
					break;
			}
		}
	}
}

void InterfaceAsservissement::creer(int precision){
	if(m_instance)
		cerr<<"Instance déjà créée"<<endl;
	else
		m_instance = new InterfaceAsservissement(precision);
}

void InterfaceAsservissement::goTo(Point depart,Point arrivee,int nbPoints){
	vector<Point> listePointsTmp=m_pathfinding.getChemin(depart,arrivee);
	vector<Point> listePointsLissee=ListePoints::lissageBezier(listePointsTmp,nbPoints);
	vector<Consigne> listeConsignes=ListePoints::convertirEnConsignes(listePointsLissee); 
    ListeConsignes::transfertSerie(listeConsignes,m_liaisonSerie);
}

InterfaceAsservissement::InterfaceAsservissement(int precision) : m_pathfinding(precision){
	m_liaisonSerie.SetBaudRate(SerialStreamBuf::BAUD_57600);
	m_liaisonSerie.SetCharSize( SerialStreamBuf::CHAR_SIZE_8);
	m_liaisonSerie.SetNumOfStopBits(1);
}

unsigned char InterfaceActionneurs::pourcentageHauteurConversion(unsigned char pourcentage){
	return (pourcentage*2.55);
}


template <typename T>  stack<unsigned char> InterfaceActionneurs::decToBin(T dec){
	stack<unsigned char> res;
	for(int i=0;i<sizeof(T);i++){
	bitset<8> charTmp;
		for(int j=0;j<8;j++){
			charTmp.set(j,dec%2);
			dec=dec>>1;
		}
		unsigned char resTmp = (charTmp.to_ulong() << 8 * i);
		if(resTmp) res.push(resTmp);
	}
	return res;
}


unsigned int InterfaceActionneurs::pourcentageAngleConversion(unsigned char pourcentage){
	return(pourcentage*10.24);
}

void InterfaceCapteurs::thread(){
	while(1){
		//Tant que le capteur ne détecte pas d'obstacle
			//traiterAbsenceObstacle()
		//traiterPresenceObstacle()
	}	
}