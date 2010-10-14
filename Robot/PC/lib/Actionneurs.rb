# encoding: utf-8

# Ce fichier définit la classe pour la gestion des actionneurs du robot

require "Log"

# Cette classe contient les fonctions permettant de gérer les actionneurs
class Actionneurs
	
	# Initialise l'asservissement à partir d'un périphérique série
	def initialize peripherique
		@log = Logger.instance
		if peripherique == nil
			raise "Pas de carte pour Actionneurs" 
		end
		@log.debug "Actionneurs sur " + peripherique
		@interface = InterfaceActionneurs.new peripherique
	end
	
	# fonction à décrire
	def fonction1
		# à coder	
	end

	# fonction à décrire
	def fonction2
		# à coder	
	end
end
