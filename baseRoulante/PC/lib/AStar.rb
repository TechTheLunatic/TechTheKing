require "./Point"
require "./Obstacles"

#la classe représentant les noeuds qui seront ajoutés au fur et à mesure.
#La choucroute c'est un peu salé c'est vrai mais c'est super bon >__________<

class Noeud < Point
	attr_accessor :cout1, :cout2, :cout3, :parent
	
	def initialize x=0, y=0, cout1=0, cout2=0
		super(x,y)
		#cout 1 : distance au point de départ, cout2 : distance au point d'arrivee, cout 3 : somme des deux.
		@cout1 = cout1
		@cout2 = cout2
		@cout3=@cout1+@cout2
		#le noeud parent.
		@parent=Point.new
	end
	
	def meilleurQue noeud2
	#compare deux noeuds selon leur cout3
		if @cout3 < noeud2.cout3
		return true
		end
		return false
	end
	
	def prettyPrint
	#affiche un noeud (surtout pour du debug..)
		super
		puts " cout1 = " + @cout1.to_s + " cout2 = " + @cout2.to_s + " cout3 = " + @cout3.to_s
	end
	
end


class AStar < ListePoints
public

	def initialize depart=Point.new(0,0), arrivee=Noeud.new(1243,2785), listeObstacles=listeObstacles.new
		@depart=depart
		@arrivee=arrivee
		@listeObstacles=listeObstacles
		@listeFermee=ListePoints.new
		@listeOuverte=ListePoints.new
		rechercheChemin
	end
	
private

	def ajouterCasesAdjacentes point
	#on découpe en carrés de 100mm = 10cm.
		for i in [point.x-30,point.x,point.x+30]
			if(i<0 or i>3000)
				next
			end
			for j in [point.y-30,point.y,point.y+30]
				if(j<0 or j>3000)
					next
				end
				if(j==point.y and i==point.x)
					next
				end
				if(@listeObstacles.contientPoint(Point.new(i,j)))
					next
				end
				#ajout d'un noeud temporaire
				@tmp = Noeud.new(i,j)
				#si le point se trouve dans la liste fermée, on ne fait rien
				if (@listeFermee.elementCommun(@tmp)==nil)
					#mise à jour des caractéristiques du noeud qu'on ajoutera par la suite
					@tmp.cout1 = @depart.distance(@tmp)
					@tmp.cout2 = @arrivee.distance(@tmp)
					@tmp.cout3 = @tmp.cout1 + @tmp.cout2
					if(@tmp.cout2==0)
						@arrivee.parent = point
					else
						@tmp.parent=point
					end
					if(@listeOuverte.elementCommun(@tmp)!=nil)
						#si le noeud temporaire est mieux que celui qui existe actuellement dans la liste ouverte, on le met à jour
						(@listeOuverte.elementCommun(@tmp)).cout1 = @tmp.cout1
						(@listeOuverte.elementCommun(@tmp)).cout2 = @tmp.cout2
						(@listeOuverte.elementCommun(@tmp)).cout3 = @tmp.cout3
					else
						#sinon on l'ajoute
						@listeOuverte.push(@tmp)
					end
				end
			end
		end
	end
	
	def trouverMeilleurNoeud
	#trouve le meilleur noeud dans la liste ouverte
		@@meilleurNoeud=@listeOuverte[0]
		@listeOuverte.each do |noeud|
			if(noeud.meilleurQue(@@meilleurNoeud))
				@@meilleurNoeud=noeud
			end
		end
		return @@meilleurNoeud
	end
	
	def transfererNoeud noeud
	#transfert un noeud de la liste fermée vers la liste ouverte
		@listeFermee.push(noeud);
		@listeOuverte.delete(noeud);
	end
	
	def remonterChemin
		@@noeudCourant=@arrivee
		while(@@noeudCourant.parent!=@depart)
			self.push(Point.new(@@noeudCourant.x,@@noeudCourant.y))
			@@noeudCourant=@@noeudCourant.parent
		end
		#ne pas oublier de remettre la liste dans l'ordre, ça évitera des absurdités.
		self.reverse!
	end
	
	def rechercheChemin
	#boucle principale.
		#le noeud courant est initialisé au noeud de départ (celui ci étant un point, une conversion en noeud est nécéssaire)
		@@courant=Noeud.new(@depart.x,@depart.y,0,@depart.distance(@arrivee))
		#ajout du noeud courant à la liste ouverté
		@listeOuverte.push(@@courant)
		#transfert du noeud courant vers la liste fermée.
		transfererNoeud(@@courant)
		#on commence à remplir la liste ouverte
		ajouterCasesAdjacentes(@@courant)
		while(!(@@courant.cout2<30) and !@listeOuverte.empty?)
			@@courant = trouverMeilleurNoeud
			transfererNoeud @@courant
			ajouterCasesAdjacentes @@courant
		end
		if(@@courant.cout2<30)
			@arrivee.parent=@@courant
			remonterChemin
		else
			puts("Pas de chemin trouvé");
		end
	end
	
end

pion = Obstacle.new(150,150,100)
pion2 = Obstacle.new(300,450,100)
puts pion.contientPoint(Point.new(100,100))
listeObstacles = ListeObstacles.new
listeObstacles.push(pion)
listeObstacles.push(pion2)
AStar.new(Point.new(0,0),Noeud.new(1000,1000),listeObstacles).lissageBezier(100).prettyPrint