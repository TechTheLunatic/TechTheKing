#ifndef OBSTACLES_H
#define OBSTACLES_H

/*!
 * \file Obstacles.h
 * \brief La gestion des obstacles du décor : aussi bien les pions que les morceaux de bois sur la table.
 * \author Philippe Tillet.
 * \version 1.0
 */

#include "config.h"
#include "Constantes.h"
#include "Point.h"
#ifdef DEBUG_GRAPHIQUE
#include <Magick++.h>
#endif



using namespace std;



/*!
 * \class Obstacle
 * \brief Classe représentant un obstacle
 *
 * La classe abstraite obstacle de laquelle découleront les obstacles circulaires (pions cette année) et rectangulaires(morceaux de bois cette année).
 */

class Obstacle : public Point{
    
    public:

        /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe Obstacle
         *
         *  \param x : Abscisse du centre de masse de l'obstacle.
         *  \param y : Ordonnée du centre de masse de l'obstacle.
         *  \param couleur : A qui est le pion.
         */
        Obstacle(double x=0,double y=0,Couleur couleur=NEUTRE);

        /*!
         * \brief contientCercle
         *
         * L'obstacle est il en collision avec un cercle?
         *
         * \return true si le cercle donné chevauche l'obstacle.
x         */
        virtual bool contientCercle(int centreX, int centreY, int rayon) = 0;

#ifdef DEBUG_GRAPHIQUE
        /*!
         * \brief draw
         *
         * Dessine l'e pion obstacle dans une image ImageMagick.
         */
        virtual void draw(Magick::Image* image) = 0;
#endif
        
        /*!
         * \brief deplacer
         *
         * Déplace un obstacle en (newX,newY)
         */
        void deplacer(double newX,double newY) { m_x=newX; m_y=newY; };

        /*!
         * \brief couleurPlusProche
         *
         * L'obstacle est-il à quelqu'un? Si oui, à qui?
         *
         * \return la couleur du robot à qui l'obstacle est, ou neutre si il n'est à personne.
         */
        virtual Couleur couleurPlusProche();

        /*!
         * \brief Accesseurs
         */
        virtual Couleur getCouleur()  { return m_couleur; };
        void setCouleur(Couleur couleur){ m_couleur=couleur; };
        
        
    protected:
        
        Couleur m_couleur;
        
};

/*!
 * \class CercleObstacle
 *
 * \brief classe représentant un obstacle circulaire.
 *
 * Cette classe correspond à priori uniquement aux pions cette année.
 */

class CercleObstacle : public Obstacle{
    
    public:
        
        CercleObstacle(double x,double y,Couleur couleur=NEUTRE,int rayon=TAILLE_ROBOT_ADVERSE);

        
#ifdef DEBUG_GRAPHIQUE
        void draw(Magick::Image* image);
#endif
        
        
        bool contientCercle(int centreX, int centreY, int rayon);
        
        Couleur couleurPlusProche();
        
    private:
        
        int m_rayon;
};

/*!
 * \class RectangleObstacle
 *
 * \brief classe représentant un obstacle rectangulaire.
 *
 * Cette classe correspond à priori uniquement aux planches de bois cette annee.
 */

class RectangleObstacle : public Obstacle{
    
    public:
        
        RectangleObstacle(double x,double y,int demiCoteX,int demiCoteY);

#ifdef DEBUG_GRAPHIQUE
        void draw(Magick::Image* image);
#endif

        bool contientCercle(int centreX, int centreY, int rayon);
        
        Couleur couleurPlusProche();
        
    private:
        
        int m_demiCoteX;
        int m_demiCoteY;
};

class RobotAdverse : public CercleObstacle{
	public:	
		static RobotAdverse* Instance();
		void setCoords(int x,int y);
	private:
		RobotAdverse();
		static RobotAdverse* m_instance;
};

extern std::vector< std::pair<Obstacle*,int> > listeObstacles; /*!< La liste d'obstacles dans la mémoire du robot*/

/*!
 * \namespace ListeObstacles
 *
 * Toutes les fonctions concernant à la liste d'obstacles. (elle est unique)
 */
namespace ListeObstacles{

/*!
 * \brief contientCercle
 *
 * Existe-t-il dans la liste d'obstacles un obstacle de la couleur donnée contenant le cercle donné?
 *
 * \return Si il existe, l'obstacle en question le plus proche du centre du cercle. NULL sinon.
 */
Obstacle* contientCercle(int centreX,int centreY,int rayon, Couleur couleur);

/*!
 * \brief setCouleursAuto
 *
 * Parcourt la liste d'obstacles et attribue à chacun la couleur de la case la plus proche.
 *
 */
void setCouleursAuto();



void erasePions();
	
/*!
 * \brief refreshPositions
 *
 * Recharge les coordonnees de la liste d'obstacles depuis le fichier ou la liste de pions spécifié.
 *
 */
 
 void refreshPions(std::vector< std::pair<Obstacle*,int> > listePions);
 
 
void refreshPions(const char nomFichier[]);

void ajoutPion();
/*!
 * \brief
 * 
 * Ajout des planches de bois
 * 
 */
void initialisation();

void ajouterPions();

void addRobotAdverse();

}

#endif
