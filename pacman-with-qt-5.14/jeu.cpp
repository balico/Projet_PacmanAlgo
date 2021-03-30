#include "jeu.h"
#include <iostream>
#include <assert.h>

Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = BAS;
}

int Fantome::getPosX() const
{
    return posX;
}

int Fantome::getPosY() const
{
    return posY;
}

Jeu::Jeu()
{
    terrain = NULL;
    largeur = 0; hauteur = 0;
    posPacmanX = 0; posPacmanY = 0;
}

Jeu::~Jeu()
{
    if (terrain!=NULL)
        delete[] terrain;
}

bool Jeu::init()
{
	int x, y;
	list<Fantome>::iterator itFantome;

  const char terrain_defaut[21][1+17] = {
    "#################",
		"#.......#.......#",
		"#P#.###.#.###.#P#",
		"#...............#",
    "#.#.#.#####.#.#.#",
    "#...#...#...#...#",
    "###.###.#.###.###",
    "###.#-------#.###",
    "###.#-##V##-#.###",
    "---.--##-##--.---",
    "###.#-#####-#.###",
    "###.#-------#.###",
    "###.#-#####-#.###",
    "#.......#.......#",
    "#.##.##.#.##.##.#",
    "#P.#.........#.P#",
    "##.#.#.###.#.#.##",
		"#....#..#..#....#",
		"#.#####.#.#####.#",
		"#...............#",
    "#################",
    };

	largeur = 17; //+1 pour le tableau !
	hauteur = 21;

	terrain = new Case[largeur*hauteur];

	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
                terrain[y*largeur+x] = MUR;
            else if (terrain_defaut[y][x]=='P')
                terrain[y*largeur+x] = POWER;
            else if (terrain_defaut[y][x]=='V')
                terrain[y*largeur+x] = VITRE;
            else if (terrain_defaut[y][x]=='.')
                terrain[y*largeur+x] = GOMME;
            else
                terrain[y*largeur+x] = VIDE;

    fantomes.resize(4);

	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]==MUR);

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
    }

    do {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]==MUR);

    posPacmanX = x,
    posPacmanY = y;

    return true;
}

void Jeu::evolue()
{
    int testX, testY;
	list<Fantome>::iterator itFantome;

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        testX = itFantome->posX + depX[itFantome->dir];
        testY = itFantome->posY + depY[itFantome->dir];
        if (terrain[testY*largeur+testX]!=MUR)
        {
            itFantome->posX = testX;
            itFantome->posY = testY;
        }
        else {
          do {
            // Changement de direction
            itFantome->dir = (Direction)(rand()%4);
            testX = itFantome->posX + depX[itFantome->dir];
            testY = itFantome->posY + depY[itFantome->dir];
          } while(terrain[testY*largeur+testX]==MUR);
          itFantome->posX = testX;
          itFantome->posY = testY;
        }
    }
    deplacePacman(RIEN); //On déplace Pacman
}

bool Jeu::deplacePacman(Direction dir)
{
    static Direction dir_save=DROITE; //On garde en mémoire la valeur du clavier (de base direction DROITE)
    static Direction dir_prec=DROITE; //On garde en mémoire la valeur precedente de direction
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};
    int testX, testY;

    if (dir != RIEN){ //Si l'utilisateur change de direction via les boutons
      dir_save = dir; //on save la valeur pour le prochain evolue()
    }
    else{ //Sinon c'est un appel de evolue()
      testX = posPacmanX + depX[dir_save];
      testY = posPacmanY + depY[dir_save];

      if (posValide(testX, testY)) //On test si déplacer Pacman est possible
      {
          posPacmanX = testX;
          posPacmanY = testY;

          if (terrain[posPacmanY*largeur+posPacmanX]==GOMME) {
            //replacer la case par du sol
            terrain[posPacmanY*largeur+posPacmanX]=VIDE;
            //fct score ?
          }
          else if (terrain[posPacmanY*largeur+posPacmanX]==POWER) {
            //replacer la case par du sol
            terrain[posPacmanY*largeur+posPacmanX]=VIDE;
            //fct manger fantomes
          }
          dir_prec = dir_save;
          return true;
      }
      else{ //sinon il garde sa direction
        testX = posPacmanX + depX[dir_prec];
        testY = posPacmanY + depY[dir_prec];

        if (posValide(testX, testY)) //On test si déplacer Pacman est possible
        {
            posPacmanX = testX;
            posPacmanY = testY;

            if (terrain[posPacmanY*largeur+posPacmanX]==GOMME) {
              //replacer la case par du sol
              terrain[posPacmanY*largeur+posPacmanX]=VIDE;
              //fct score ?
            }
            else if (terrain[posPacmanY*largeur+posPacmanX]==POWER) {
              //replacer la case par du sol
              terrain[posPacmanY*largeur+posPacmanX]=VIDE;
              //fct manger fantomes
            }
        return true;
        }
      return false;
      }
    }
    return true;
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

int Jeu::getPacmanX() const
{
    return posPacmanX;
}

int Jeu::getPacmanY() const
{
    return posPacmanY;
}

Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

bool Jeu::posValide(int x, int y) const //pour valider la pos de Pacman
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && terrain[y*largeur+x]!=MUR && terrain[y*largeur+x]!=VITRE);
}

void Jeu::AjouterFantome(){
  Fantome f;
  f.posX = 8;
  f.posY = 10;
  f.dir = HAUT;

  Jeu::fantomes.push_back(f);
}

void Jeu::SupprFantome(){
    if (fantomes.size() > 0) {
        fantomes.pop_back();
    }
}
