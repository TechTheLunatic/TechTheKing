#include "AStar.h"

/*ce qui concerne les noeuds*/

Noeud::Noeud(int x,int y,double cout1,double cout2){
	m_x=x;
	m_y=y;

	m_cout1=cout1;
	m_cout2=cout2;
	m_cout3=cout1+cout2;
	
	m_distancePionAdverse=TAILLE_PION+RAYON_DE_DETECTION;
	
	m_parent=this;
}

bool Noeud::operator>(Noeud noeud2) const{
	if(m_distancePionAdverse < TAILLE_PION+RAYON_DE_DETECTION && m_distancePionAdverse > TAILLE_PION+TAILLE_ROBOT-EMPIETEMENT)
		return  (m_distancePionAdverse<noeud2.m_distancePionAdverse);
	return(m_cout3<noeud2.m_cout3);
}

double Noeud::getCout1() const{
	return m_cout1;
}

double Noeud::getCout2() const{
	return m_cout2;
}

double Noeud::getCout3() const{
	return m_cout3;
}


Noeud* Noeud::getParent() const{
	return m_parent;
}

void Noeud::setCout1(double cout1){
	m_cout1=cout1;
}

void Noeud::setCout2(double cout2){
	m_cout2=cout2;
}

void Noeud::setCout3(double cout3){
	m_cout3=cout3;
}

void Noeud::setDistancePionAdverse(double distance){
	m_distancePionAdverse=distance;
}

void Noeud::setParent(Noeud* noeud){
	m_parent=noeud;
}

Noeud::~Noeud(){
}



/* ce qui concerne AStar */



vector <Point> AStar::getChemin(){
	return m_chemin;
}


AStar::AStar(int precision, Noeud depart, Noeud arrivee, vector<Obstacle*>listeObstacles){
	m_precision=precision;
	m_depart=depart;
	m_arrivee=arrivee;
	m_listeObstacles=listeObstacles;

	trouverChemin();
};


void AStar::ajouterCasesAdjacentes(Noeud noeud){
	for(int i=noeud.getX()-m_precision;i<=noeud.getX()+m_precision;i+=m_precision){
		if(i<0 || i>3000)
			continue; //Si on est en dehors de la carte, on ignore...
		for (int j=noeud.getY()-m_precision;j<=noeud.getY()+m_precision;j+=m_precision){
			//si on est en dehors de la carte, on ignore...
			if(j<0 || j>2100)
				continue; 
			 //on est à l'étape actuelle, on ignore...
			if(i==noeud.getX() && j==noeud.getY())
				continue;
			 // si le point est dans un obstacle de la couleur du robot, on ignore pour ne pas le déplacer
			if(contientCercle(noeud,TAILLE_ROBOT,m_listeObstacles,COULEUR_ROBOT)!=NULL)
				continue;
			Noeud tmp(i,j);
			if(estDansListe(m_listeFermee,tmp)==-1){ //si le a déjà été étudié, on ne fait rien, sinon...
				Obstacle* estSurPionAdverse=contientCercle(tmp,RAYON_DE_DETECTION,m_listeObstacles,COULEUR_ADVERSE);
				if(estSurPionAdverse!=NULL){
					/*si pousser le pion adverse nous fait pousser le notre aussi*/
					if(contientCercle(*estSurPionAdverse,TAILLE_PION+50,m_listeObstacles,COULEUR_ROBOT)!=NULL)
						continue;
					else
					{
						tmp.setDistancePionAdverse(tmp.rayon(*estSurPionAdverse));
					}

				}
				Noeud* parent = new Noeud;
				(*parent)=noeud;
				tmp.setCout1(tmp.rayon(m_depart));
				tmp.setCout2(tmp.rayon(m_arrivee));
				tmp.setCout3(tmp.getCout1()+tmp.getCout2());
				tmp.setParent(parent);
				int indiceCommun = estDansListe(m_listeOuverte,tmp);
				if(indiceCommun>-1){//si le noeud est déjà présent dans la liste ouverte...
					if(tmp>m_listeOuverte[indiceCommun]){	//si le noeud actuel est meilleur, on le remplace
						m_listeOuverte[indiceCommun]=tmp;
					}
				}
				else
					m_listeOuverte.push_back(tmp); //si le noeud n'était pas dans la liste ouverte, on le rajoute...
			}
		}
	}
}
					
			
/*
 * trouve le meilleur noeud dans la liste ouverte
 */

Noeud AStar::trouverMeilleurNoeud(){
	Noeud meilleurNoeud=m_listeOuverte[0];
	for(unsigned int i=0;i<m_listeOuverte.size();i++){
		if(m_listeOuverte[i]>meilleurNoeud)
			meilleurNoeud=m_listeOuverte[i];
	}
	return meilleurNoeud;
}

/*
 * transfert un noeud de la liste ouverte vers la liste fermee
 */

void AStar::transfererNoeud(Noeud noeud){
	m_listeFermee.push_back(noeud);
	int indiceNoeud = estDansListe(m_listeOuverte,noeud);
	if(indiceNoeud==-1)
		cerr << "le noeud n'apparait pas dans la liste ouverte" << endl;
	else
		m_listeOuverte.erase(m_listeOuverte.begin()+indiceNoeud);
}


void AStar::remonterChemin(){
	
	m_chemin.clear();
	
	/*
	 * on remonte le chemin dans les noeuds
	 */
	
	
	vector <Point> listePointsTmp; //liste de points temporaire (avec des points trop proches entre eux)
	
	Noeud noeudCourant=m_arrivee;
	listePointsTmp.push_back(noeudCourant);
	while(!(*(noeudCourant.getParent())==m_depart)){
		noeudCourant=*(noeudCourant.getParent());
		listePointsTmp.push_back(noeudCourant);
	}
	
	
	/*
	 * On garde pas tous les noeuds pour alléger le futur calcul de la trajectoire de bézier, et pour réduire la courbure de la courbe . (le robot n'aime pas trop les grosses courbures)
	 */
	

	
	
	unsigned int distanceAGarder = 3; //on garde un point sur trois
	
	for(unsigned int i=0;i<listePointsTmp.size();i+=distanceAGarder)
		m_chemin.push_back(listePointsTmp[i]);
	
	m_chemin.push_back(m_depart); // on ajoute le point de départ du robot

	reverse(m_chemin.begin(), m_chemin.end()); //ne pas oublier d'inverser la liste ... mieux vaut le faire maintenant qu'avant la recopie. 
}

/*
 * C'est la boucle principale de recherche de AStar
 */

void AStar::trouverChemin(){
	if(contientCercle(m_arrivee,TAILLE_ROBOT,m_listeObstacles,COULEUR_ROBOT)!=NULL || contientCercle(m_depart,TAILLE_ROBOT,m_listeObstacles,COULEUR_ROBOT)!=NULL ) 
		cerr<<"Le robot est bloqué ou croit être bloqué dans un obstacle"<<endl;
	else
	{
		Noeud courant(m_depart.getX(),m_depart.getY(),0,m_depart.rayon(m_arrivee)); //initialisation du noeud courant..
		m_listeOuverte.push_back(courant);
		ajouterCasesAdjacentes(courant);
		transfererNoeud(courant);
		while(courant.getCout2()>m_precision && !(m_listeOuverte.empty())){
			courant = trouverMeilleurNoeud();
			transfererNoeud(courant);
			ajouterCasesAdjacentes(courant);
		}
		/* on libère la mémoire inutile correspondant aux parents de la liste ouverte */
		if(courant.getCout2()<=m_precision){
			m_arrivee.setParent(&courant);
			remonterChemin();
		}
		else{
			cerr<<"pas de chemin"<<endl;
			m_chemin.push_back(m_depart);
		}
		for(unsigned int i=0;i<m_listeOuverte.size();i++){
			delete m_listeOuverte[i].getParent();
		}
		for(unsigned int i=1;i<m_listeFermee.size();i++){
			delete m_listeFermee[i].getParent();
		}
	}
}

/*
 * cette fonction teste la présence d'un élement dans la liste. Si il est présent, elle renvoie son indice, sinon, elle renvoie -1
 */

int estDansListe(const vector<Noeud>& listeNoeuds, Point point){
	for(unsigned int i=0;i<listeNoeuds.size();i++){
		if(listeNoeuds[i]==point){
			return i;
		}
	}
	return -1;
}