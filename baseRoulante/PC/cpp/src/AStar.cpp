#include "AStar.h"
#include "Constantes.h"
#include "Util.hpp"

/*!
 *\def RAYON_DE_DETECTION
 *
 * le périmetre dans lequel le robot se déplacera pour aller bouger un pion à l'adversaire...
 */
int RAYON_DE_DETECTION = 350;

/*!
 * \def EMPIETEMENT
 * l'empietement du robot sur le pion adverse, lorsque celui ci n'est pas sur le chemin le plus court
 */
int EMPIETEMENT = 100 ;
/*!
 * \class Noeud
 *
 * La classe Noeud, indispensable pour tout algorithme de pathfinding.
 */
 
 int compteurImages=0;
 
/*ce qui concerne les noeuds*/


Noeud::Noeud(int x,int y,double cout1,double cout2) : Point(x,y), m_cout1(cout1), m_cout2(cout2){
	m_cout3=cout1+cout2;
	m_distancePionAdverse=0;
	m_collision=false;
	m_parent=NULL;
	CHECK_INVARIANTS;
}

bool Noeud::operator>(Noeud noeud2){
	if(noeud2.m_collision==true)
		return true;
	if(m_collision==true)
		return false;
	if(m_distancePionAdverse > TAILLE_PION+TAILLE_ROBOT-EMPIETEMENT && noeud2.m_distancePionAdverse > TAILLE_PION+TAILLE_ROBOT-EMPIETEMENT)
		return  (m_distancePionAdverse<noeud2.m_distancePionAdverse);
	return(m_cout3<noeud2.m_cout3);
}

void Noeud::operator=(Point pointDonne){
	m_x=pointDonne.getX();
	m_y=pointDonne.getY();
}

double Noeud::getCout1(){
	return m_cout1;
}

double Noeud::getCout2(){
	return m_cout2;
}

double Noeud::getCout3(){
	return m_cout3;
}


Noeud* Noeud::getParent(){
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

void Noeud::setParent(Noeud* parent){
	m_parent = parent;
}

Noeud::~Noeud(){
}



/* ce qui concerne AStar */

#ifdef DEBUG_GRAPHIQUE
using namespace Magick;

void AStar::debugGraphique(std::vector<Point> listePoints){
    cout << "Conversion du chemin emprunté par le robot en graphique..." << endl;
    Image image( "img/table.png" );
    
    /* Affiche les obstacles */
    for(unsigned int i=0;i<listeObstacles.size();i++){
        (listeObstacles[i].first)->draw(&image);
    }
    
    /* Affiche la courbe */
    image.strokeColor(Color(MaxRGB,MaxRGB,MaxRGB,MaxRGB/2));
    //image.strokeWidth(2*TAILLE_ROBOT);
    image.strokeWidth(15);
    for(unsigned int i=0;i<listePoints.size()-1;i++)
        image.draw(DrawableLine((listePoints[i].getX())*900/3000,
        630-listePoints[i].getY()*630/2100,
        listePoints[i+1].getX()*900/3000,
        630-listePoints[i+1].getY()*630/2100));
    image.magick("png");
    std::string tmp("cheminRobot");
    image.display();
    cout<<"chemin emprunté dans le robot écrit dans cheminRobot.png"<<endl;
}

#endif

vector<Point> AStar::getChemin(Point depart, Point arrivee){
	REQUIRE(depart!=arrivee, "Le point de départ est différent du point d'arrivée du robot");
	REQUIRE(TAILLE_ROBOT<arrivee.getX() && arrivee.getX()<3000-TAILLE_ROBOT, "L'abscisse du point d'arrivee du robot est tel qu'il ne touche pas le bord de la table");
	REQUIRE(TAILLE_ROBOT<arrivee.getY() && arrivee.getY()<2100-TAILLE_ROBOT, "L'ordonnée du point d'arrivee du robot est tel qu'il ne touche pas le bord de la table");
	m_chemin.clear();
	m_depart=depart;
	m_arrivee=arrivee;
	trouverChemin();
	return m_chemin;
}


AStar::AStar(int precision){
	REQUIRE(precision>=0, "La précision est positive");
	m_precision=precision;
}

void AStar::setPrecision(int precision){
	REQUIRE(precision>=0, "La précision est positive");
	m_precision=precision;
}

void AStar::ajouterCasesAdjacentes(Noeud* noeud){
	int noeudX=noeud->getX();
	int noeudY=noeud->getY();
	Noeud noeudObj = *noeud;
	for(int i=noeudX-m_precision;i<=noeudX+m_precision;i+=m_precision){
		if(i>3000-TAILLE_ROBOT || i < TAILLE_ROBOT )
			continue;
		for (int j=noeudY-m_precision;j<=noeudY+m_precision;j+=m_precision){
			if(j>2100 - TAILLE_ROBOT || j < TAILLE_ROBOT )
				continue;
			 //on est à l'étape actuelle, on ignore...
			if(i==noeudX && j==noeudY)
				continue;
			if(estDansListe(m_listeFermee,i,j)==m_listeFermee.end()){ //si le a déjà été étudié, on ne fait rien, sinon...
				Noeud* tmp = new Noeud(i,j);
				// si le point est dans un obstacle de la couleur du robot
				// ou sur une planche de bois, on ignore pour ne pas le déplacer ou se bloquer.
				// Si cet obstacle est le point d'arrivee, ça ne s'applique pas
				if(ListeObstacles::contientCercle(i,j,TAILLE_ROBOT,NOIR)!=NULL)
				{
					continue;
				}
				if(ListeObstacles::contientCercle(i,j,TAILLE_ROBOT+m_precision+MARGE_SECURITE_PION,COULEUR_ROBOT)!=NULL){
					tmp->setCollision(true);
				}
				Obstacle* estSurPionAdverse=ListeObstacles::contientCercle(i,j,RAYON_DE_DETECTION,COULEUR_ADVERSE);
				if(estSurPionAdverse!=NULL){
					/*si pousser le pion adverse nous fait pousser le notre aussi*/
					if(ListeObstacles::contientCercle((*estSurPionAdverse).getX(),(*estSurPionAdverse).getY(),TAILLE_PION+50,COULEUR_ROBOT)){
						tmp->setCollision(true);
					}
					else
					{
						tmp->setDistancePionAdverse(tmp->rayon(*estSurPionAdverse));
						noeud->setDistancePionAdverse(tmp->rayon(*estSurPionAdverse));
					}

				}
				tmp->setCout1(tmp->rayon(m_depart)); 
				tmp->setCout2(tmp->rayon(m_arrivee));
				tmp->setParent(noeud);
				list<Noeud*>::iterator noeudCommun = estDansListe(m_listeOuverte,tmp->getX(),tmp->getY());
				if(noeudCommun!=m_listeOuverte.end()){//si le noeud est déjà présent dans la liste ouverte...
					if(*tmp>**noeudCommun){	//si le noeud actuel est meilleur, on le remplace
						delete *noeudCommun;
						m_listeOuverte.erase(noeudCommun);
						m_listeOuverte.push_back(tmp);
					}
					else{
						delete tmp;
					}
					
				}
				else{
					m_listeOuverte.push_back(tmp); //si le noeud n'était pas dans la liste ouverte, on le rajoute
				}
			}
		}
	}
}
					
			
/*
 * trouve le meilleur noeud dans la liste ouverte
 */

list<Noeud*>::iterator AStar::trouverMeilleurNoeud(){
	list<Noeud*>::iterator it;
	list<Noeud*>::iterator meilleurNoeud=m_listeOuverte.begin();
	for(it=m_listeOuverte.begin();it!=m_listeOuverte.end();it++){
		if(**it>**meilleurNoeud)
			meilleurNoeud=it;
	}
	return meilleurNoeud;
}

/*
 * transfert un noeud de la liste ouverte vers la liste fermee
 */

void AStar::transfererNoeud(Noeud* noeud){
	m_listeFermee.push_back(noeud);
	list<Noeud*>::iterator indiceNoeud=estDansListe(m_listeOuverte, noeud->getX(), noeud->getY());
	if(indiceNoeud==m_listeOuverte.end())
		cerr << "le noeud n'apparait pas dans la liste ouverte" << endl;
	else{
		m_listeOuverte.erase(indiceNoeud);
	}
}


void AStar::remonterChemin(){
	
	/*
	 * on remonte le chemin dans les noeuds
	 */
	
	vector <Point> listePointsTmp; //liste de points temporaire (avec des points trop proches entre eux)
	
	Noeud noeudCourant=m_arrivee;
	listePointsTmp.push_back(noeudCourant);
	while(!(noeudCourant==m_depart)){
		noeudCourant=*(noeudCourant.getParent());
		listePointsTmp.push_back(noeudCourant);
	}
	/*
	 * On garde pas tous les noeuds pour alléger le futur calcul de la trajectoire de bézier, et pour réduire la courbure de la courbe . (le robot n'aime pas trop les grosses courbures)
	 */
	
	unsigned int espacement = 3; //on garde un point sur trois
	
	for(unsigned int i=0;i<listePointsTmp.size();i+=espacement){
		m_chemin.push_back(listePointsTmp[i]);
	}
	
	m_chemin.push_back(m_depart); // on ajoute le point de départ du robot

	reverse(m_chemin.begin(), m_chemin.end()); //ne pas oublier d'inverser la liste ... mieux vaut le faire maintenant qu'avant la recopie.
}

/*
 * C'est la boucle principale de recherche de AStar
 */

bool AStar::trouverChemin(){
		//Supression de l'arrivee de la liste d'obstacles
		Obstacle* bloque;
		std::vector<Obstacle*> listeObstaclesBloquants;
		while((bloque=ListeObstacles::contientCercle(m_arrivee.getX(),m_arrivee.getY(),TAILLE_ROBOT,NOIR))!=NULL
			   || (bloque=ListeObstacles::contientCercle(m_arrivee.getX(),m_arrivee.getY(),TAILLE_ROBOT,COULEUR_ROBOT))!=NULL
			   || (bloque=ListeObstacles::contientCercle(m_arrivee.getX(),m_arrivee.getY(),TAILLE_ROBOT,COULEUR_ADVERSE))!=NULL
			   || (bloque=ListeObstacles::contientCercle(m_arrivee.getX(),m_arrivee.getY(),TAILLE_ROBOT,NEUTRE))!=NULL
			   || (bloque=ListeObstacles::contientCercle(m_depart.getX(),m_depart.getY(),TAILLE_ROBOT,NOIR))!=NULL
			   || (bloque=ListeObstacles::contientCercle(m_depart.getX(),m_depart.getY(),TAILLE_ROBOT,COULEUR_ROBOT))!=NULL
			   || (bloque=ListeObstacles::contientCercle(m_depart.getX(),m_depart.getY(),TAILLE_ROBOT,COULEUR_ADVERSE))!=NULL
			   || (bloque=ListeObstacles::contientCercle(m_depart.getX(),m_depart.getY(),TAILLE_ROBOT,NEUTRE))!=NULL
			 ) 
		{
			listeObstaclesBloquants.push_back(bloque);
			#ifdef DEBUG
				std::cout << "Obstacle sur le point d'arrivée : suppression de " << *bloque << std::endl;
			#endif
			for(int k = 0; k < listeObstacles.size(); k++)
			{
				if(*(listeObstacles[k].first) == *bloque){
					listeObstacles.erase(listeObstacles.begin() + k);
					#ifdef DEBUG
					std::cout << "Supprimé : " << *bloque << std::endl;
					#endif
				}
			}
		}
		#ifdef DEBUG
			std::cout << "Début AStar " << std::endl;
		#endif
		Noeud* courant = new Noeud(m_depart.getX(),m_depart.getY(),0,m_depart.rayon(m_arrivee)); //initialisation du noeud courant..
		m_listeOuverte.push_back(courant);
		transfererNoeud(courant);
		ajouterCasesAdjacentes(courant);	
		while(courant->getCout2()>m_precision && !(m_listeOuverte.empty())){
			courant = *trouverMeilleurNoeud();
			transfererNoeud(courant);
			ajouterCasesAdjacentes(courant);
		}
		if(courant->getCout2()<=m_precision){
			m_arrivee.setParent(courant);
			remonterChemin();
		}
		for(list<Noeud*>::iterator it = m_listeOuverte.begin();it!=m_listeOuverte.end();it++){
			if(*it)
				delete *it;
		}
		for(list<Noeud*>::iterator it = m_listeFermee.begin();it!=m_listeFermee.end();it++){
			if(*it){
				delete *it;
			}
				
		}
		#ifdef DEBUG
			std::cout << "Début Rajout de l'obstacle supprimé " << std::endl;
		#endif
		for(std::vector<Obstacle*>::iterator it =listeObstaclesBloquants.begin(); it!=listeObstaclesBloquants.end();it++)
		{
			if((*it)->getCouleur()==NOIR){
				listeObstacles.push_back(std::make_pair<Obstacle*,int>(*it,0));
				#ifdef DEBUG
				std::cout << "Rajouté " << std::endl;
				#endif
			}
		}
		if(m_chemin.empty())
		{
			std::cout << "Pas de chemin..." << std::endl;
			return false;
		}
		else{
			//debugGraphique(m_chemin);                                                                                
			m_listeOuverte.clear();
			m_listeFermee.clear();
			return true;
		}
}

/*
 * cette fonction teste la présence d'un élement dans la liste. Si il est présent, elle renvoie son indice, sinon, elle renvoie -1
 */

list<Noeud*>::iterator estDansListe(list<Noeud*>& listeNoeuds, int x, int y){
	list<Noeud*>::reverse_iterator it;
	for(it=listeNoeuds.rbegin() ; it!=listeNoeuds.rend() ;it++){
		if((**it).getX()==x && (**it).getY()==y){
			return (++it).base();
		}
	}
	return (++it).base();
}
