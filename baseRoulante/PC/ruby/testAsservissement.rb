require 'libChessUp'
include LibChessUp

InterfaceAsservissement = InterfaceAsservissement.Instance(50);
LibChessUp::initialisation();
#InterfaceAsservissement.avancer(1050);
#InterfaceAsservissement.goTo(LibChessUp::Point.new(2000,700),80);
#sleep(2);
#InterfaceAsservissement.goTo(Point.new(2000,1500),50);
#InterfaceAsservissement.debugConsignes();
#InterfaceAsservissement.debugGraphique();
InterfaceAsservissement.goTo(Point.new(1500,1050),50);
InterfaceAsservissement.debugConsignes();
InterfaceAsservissement.debugGraphique();
