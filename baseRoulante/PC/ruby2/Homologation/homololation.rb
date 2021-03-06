require '../libChessUp'
include LibChessUp



# Couleur du robot
COULEUR_ROBOT = BLEU;

if (COULEUR_ROBOT == ROUGE)
	puts "Couleur: rouge"
else 
	puts "Couleur: bleu"
end

LibChessUp.ouvrir_adaptateur_i2c();

#Création des interfaces
InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;
InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
InterfaceCapteurs = LibChessUp::InterfaceCapteurs.Instance();

#initialisation des obstacles
LibChessUp::initialisation();


#Initialisation des bras.
puts "Initialisation de la hauteur des bras";
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);

sleep(2);

puts "Repli des bras";
InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

puts "Abaissement de l'aimant";
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);
InterfaceActionneurs.positionAimantGauche(LibChessUp::BAS);

puts "Recalage";
require 'recalage.rb'

puts "Attente du jumper";
InterfaceCapteurs.gestionJumper();

#Démarrage des capteurs
InterfaceCapteurs.ouvrirThread();

puts "Démarrage";

puts "Go To 1500 1050";
InterfaceAsservissement.goTo(Point.new(1500,1050),80);

puts "Prise de pion";
sleep(0.5);

InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU);

sleep(0.5);

puts "Go To 1500 350";
InterfaceAsservissement.tourner(Math::PI/2))
InterfaceAsservissement.goTo(Point.new(1400,350),80);

puts "Lacher pion";
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);

sleep(2);

InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);
InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT);

sleep(2);

InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

sleep(2);

puts "Arrêt du robot"
#InterfaceAsservissement.stopAll();
InterfaceActionneurs.arret();
