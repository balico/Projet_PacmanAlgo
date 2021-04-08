#include "jeu.h"
#include <iostream>
#include <assert.h>
#include <math.h>

////////////////////////////////////////////////////////////
//                        FANTOME                         //
////////////////////////////////////////////////////////////

Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = BAS;
    dirPrec=BAS;
    vivant=true;
}

int Fantome::getPosX() const
{
    return posX;
}

int Fantome::getPosY() const
{
    return posY;
}

////////////////////////////////////////////////////////////
//                         PACMAN                         //
////////////////////////////////////////////////////////////

Pacman::Pacman()
{
    posPacmanX = posPacmanY = 0;
    score = 0;
    dir = dirFutur = DROITE;
}

int Pacman::getPacmanX() const
{
    return posPacmanX;
}

int Pacman::getPacmanY() const
{
    return posPacmanY;
}

////////////////////////////////////////////////////////////
//                          JEU                           //
////////////////////////////////////////////////////////////

Jeu::Jeu()
{
    terrain = NULL;
    largeur = 0; hauteur = 0;
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
    "---.--#S-S#--.---",
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

  fantomes.resize(4);

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
            else if (terrain_defaut[y][x]=='S')
                terrain[y*largeur+x] = SPAWN;
            else
                terrain[y*largeur+x] = VIDE;

	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]==MUR);

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
        itFantome->dirPrec = DROITE;
        itFantome->vivant=true;
    }

    do {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]==MUR);

    pacmanJ1.posPacmanX = x,
    pacmanJ1.posPacmanY = y;

    pacmanJ2.posPacmanX = 1,
    pacmanJ2.posPacmanY = 1;

    return true;
}

void Jeu::evolue()
{
    int testX, testY;
	list<Fantome>::iterator itFantome;

    int depX[] = {-1, 1, 0, 0,0};
    int depY[] = {0, 0, -1, 1,0};

    //On déplace Pacman
    deplacePacman(RIEN, pacmanJ1);
    deplacePacman(RIEN, pacmanJ2);

    //On test s'il se fait manger par n'importe quel fantome
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++){
      if (FantomeMangePacman(itFantome->posX, itFantome->posY, pacmanJ1) == true) {
        std::cout << "J1 Manger !" << '\n';
      }
      if (FantomeMangePacman(itFantome->posX, itFantome->posY, pacmanJ2) == true) {
        std::cout << "J2 Manger !" << '\n';
      }
    }

    //On déplace les fantomes et on test s'ils mangent pacman
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        if(terrain[(itFantome->posY)*largeur+itFantome->posX]==SPAWN)
        {
            itFantome->vivant=true;
        }

        if(itFantome->vivant==true)
        {
            itFantome->dir=Poursuite(itFantome->posX,itFantome->posY,itFantome->dirPrec);
            if(itFantome->dir==RIEN)
            {
                itFantome->dir=MouvFantome(itFantome->posX,itFantome->posY,itFantome->dirPrec);
            }
        }else
        {
            itFantome->dir=Retour(itFantome->posX,itFantome->posY);
        }
        itFantome->posX = itFantome->posX + depX[itFantome->dir];
        itFantome->posY = itFantome->posY + depY[itFantome->dir];
        itFantome->dirPrec=itFantome->dir;

        if (FantomeMangePacman(testX, testY, pacmanJ1) == true) {
          std::cout << "J1 Manger !" << '\n';
        }
        if (FantomeMangePacman(testX, testY, pacmanJ2) == true) {
          std::cout << "J2 Manger !" << '\n';
        }
    }


    //score
   // std::cout << "Score J1 : " << pacmanJ1.score << " | Score J2 : " << pacmanJ2.score << '\n';
}

Direction Jeu::Poursuite(int X, int Y, Direction dirPrec)
{

    //on regarde si le fantome est proche de pacman
    if (((X-pacmanJ1.posPacmanX)*(X-pacmanJ1.posPacmanX))+((Y-pacmanJ1.posPacmanY)*(Y-pacmanJ1.posPacmanY))<10)
    {
        if(Y>pacmanJ1.posPacmanY && posValide(X,Y-1)==true && dirPrec!=BAS)
        {
            return HAUT;
        }else if(Y<pacmanJ1.posPacmanY && posValide(X,Y+1)==true && dirPrec!=HAUT)
        {
            return BAS;
        }else if(X>pacmanJ1.posPacmanX && posValide(X-1,Y)==true && dirPrec!=DROITE)
        {
            return GAUCHE;
        }else if(X<pacmanJ1.posPacmanX && posValide(X+1,Y)==true && dirPrec!=GAUCHE)
        {
            return DROITE;
        }
    }else if (((X-pacmanJ2.posPacmanX)*(X-pacmanJ2.posPacmanX))+((Y-pacmanJ2.posPacmanY)*(Y-pacmanJ2.posPacmanY))<10)
    {
        if(Y>pacmanJ2.posPacmanY && posValide(X,Y-1)==true && dirPrec!=BAS)
        {
            return HAUT;
        }else if(Y<pacmanJ2.posPacmanY && posValide(X,Y+1)==true && dirPrec!=HAUT)
        {
            return BAS;
        }else if(X>pacmanJ2.posPacmanX && posValide(X-1,Y)==true && dirPrec!=DROITE)
        {
            return GAUCHE;
        }else if(X<pacmanJ2.posPacmanX && posValide(X+1,Y)==true && dirPrec!=GAUCHE)
        {
            return DROITE;
        }
    }
    return RIEN;


}

Direction Jeu::MouvFantome(int X, int Y,Direction dirPrec)
{

    int possibilite[4];
    int nb_2=0;
    int choix;
    int i;


    if(posValide(X-1,Y)==true)
    {
        possibilite[GAUCHE]=2;
    }else if(terrain[Y*largeur+X-1]==VITRE)
    {
        possibilite[GAUCHE]=1;
    }else
    {
        possibilite[GAUCHE]=0;
    }


    if(posValide(X+1,Y)==true)
    {
        possibilite[DROITE]=2;
    }else if(terrain[Y*largeur+X+1]==VITRE)
    {
        possibilite[DROITE]=1;
    }else
    {
        possibilite[DROITE]=0;
    }

    if(posValide(X,Y+1)==true)
    {
        possibilite[BAS]=2;
    }else if(terrain[(Y+1)*largeur+X]==VITRE)
    {
        possibilite[BAS]=1;
    }else
    {
        possibilite[BAS]=0;
    }

    if(posValide(X,Y-1)==true)
    {
        possibilite[HAUT]=2;
    }else if(terrain[(Y-1)*largeur+X]==VITRE)
    {
        possibilite[HAUT]=1;
    }else
    {
        possibilite[HAUT]=0;
    }

    if(possibilite[dirPrec]==2)
    {
        possibilite[dirPrec]=3;
    }

    //cout<<possibilite[GAUCHE]<<possibilite[DROITE]<<possibilite[HAUT]<<possibilite[BAS]<<"\n";

    switch (dirPrec)
    {
    case GAUCHE:
        possibilite[DROITE]=0;
        break;
    case DROITE:
        possibilite[GAUCHE]=0;
        break;
    case HAUT:
        possibilite[BAS]=0;
        break;
    default:
        possibilite[HAUT]=0;
        break;
    }

    for(i=0;i<4;i++)
    {
        if(possibilite[i]==2)
        {
            nb_2 = nb_2 + 1;
        }
    }

    if(possibilite[dirPrec]==3)
    {
        choix=1+rand()%(2+nb_2);
        if(choix<=2)
        {
            return dirPrec;
        }
    }
    if(nb_2>0)
    {
        choix=1+rand()%nb_2;
        nb_2=0;
        for(i=0;i<4;i++)
        {
            if(possibilite[i]==2)
            {
                nb_2 = nb_2 + 1;
                if(nb_2==choix)
                {
                    return (Direction)(i);
                }
            }
        }
    }else
    {
        for(i=0;i<4;i++)
        {
            if(possibilite[i]==1)
            {
                return (Direction)(i);
            }
        }
    }
}

Direction Jeu::Retour(int X, int Y)
{
    float distance1,distance2;
    int x,y;
    distance1=sqrt((X-7)*(X-7)+(Y-9)*(Y-9));
    distance2=sqrt((X-9)*(X-9)+(Y-9)*(Y-9));
    if (abs(distance1)>abs(distance2))
    {
        x=X-7;
        y=Y-9;
        if(abs(x)>abs(y))
        {
            if(x<0)
            {
                return DROITE;
            }else
            {
                return GAUCHE;
            }
        }else
        {
            if(y<0)
            {
                return BAS;
            }else
            {
                return HAUT;
            }
        }
    }else
    {
        x=X-9;
        y=Y-9;
        if(abs(x)>abs(y))
        {
            if(x<0)
            {
                return DROITE;
            }else
            {
                return GAUCHE;
            }
        }else
        {
            if(y<0)
            {
                return BAS;
            }else
            {
                return HAUT;
            }
        }
    }
    return RIEN;

}

bool Jeu::deplacePacman(Direction dir, Pacman &pac)
{
    //static Direction dir_save=DROITE; //On garde en mémoire la valeur du clavier (de base direction DROITE)
    //static Direction dir_prec=DROITE; //On garde en mémoire la valeur precedente de direction
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};
    int testX, testY;

    if (dir != RIEN){ //Si l'utilisateur change de direction via les boutons
      //dir_save = dir; //on save la valeur pour le prochain evolue()
      pac.dirFutur = dir;
    }
    else{ //Sinon c'est un appel de evolue()
      testX = pac.posPacmanX + depX[pac.dirFutur];
      testY = pac.posPacmanY + depY[pac.dirFutur]; //dir_save

      if (posValide(testX, testY)) //On test si déplacer Pacman est possible
      {
          pac.posPacmanX = testX;
          pac.posPacmanY = testY;

          if (terrain[pac.posPacmanY*largeur+pac.posPacmanX]==GOMME) {
            //replacer la case par du sol
            terrain[pac.posPacmanY*largeur+pac.posPacmanX]=VIDE;
            pac.score += 10;
          }
          else if (terrain[pac.posPacmanY*largeur+pac.posPacmanX]==POWER) {
            //replacer la case par du sol
            terrain[pac.posPacmanY*largeur+pac.posPacmanX]=VIDE;
            pac.score += 50;
            //fct manger fantomes
          }
          //dir_prec = dir_save;
          pac.dir = pac.dirFutur;
          return true;
      }
      //Sinon Evolue() il garde sa direction
      else{
        testX = pac.posPacmanX + depX[pac.dir]; //dir_prec
        testY = pac.posPacmanY + depY[pac.dir];

        if (posValide(testX, testY)) //On test si déplacer Pacman est possible
        {
            pac.posPacmanX = testX;
            pac.posPacmanY = testY;

            if (terrain[pac.posPacmanY*largeur+pac.posPacmanX]==GOMME) {
              //replacer la case par du sol
              terrain[pac.posPacmanY*largeur+pac.posPacmanX]=VIDE;
              pac.score += 10;
              std::cout << "+10" << '\n';
            }
            else if (terrain[pac.posPacmanY*largeur+pac.posPacmanX]==POWER) {
              //replacer la case par du sol
              terrain[pac.posPacmanY*largeur+pac.posPacmanX]=VIDE;
              pac.score += 50;
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

Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

bool Jeu::posValide(int x, int y) const //pour valider la pos de Pacman
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && terrain[y*largeur+x]!=MUR && terrain[y*largeur+x]!=VITRE && terrain[y*largeur+x]!=SPAWN);
}

void Jeu::AjouterFantome(){
  Fantome f;
  f.posX = 8;
  f.posY = 9;
  f.dir = HAUT;
  f.dirPrec=HAUT;
  f.vivant=true;

  Jeu::fantomes.push_back(f);
}

void Jeu::SupprFantome(){
    if (fantomes.size() > 0) {
        fantomes.pop_back();
    }
}

bool Jeu::FantomeMangePacman(int posX, int posY, Pacman &pac) const {
  if (posX == pac.posPacmanX and posY == pac.posPacmanY){ //pacman se fait manger
    return true;
  }
  return false;
}
