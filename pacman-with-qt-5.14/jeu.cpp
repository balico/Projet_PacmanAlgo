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

bool Fantome::getvivant() const
{
    return vivant;
}

Couleur Fantome::getCouleur() const{
  return col;
}

Direction Fantome::getDir() const{
  return dir;
}

////////////////////////////////////////////////////////////
//                         PACMAN                         //
////////////////////////////////////////////////////////////

Pacman::Pacman()
{
    posPacmanX = posPacmanY = 0;
    score = 0;
    dir = dirFutur = DROITE;
    SuperG=false;
    tempsSup=0;
    vie=3;
}

int Pacman::getPacmanX() const
{
    return posPacmanX;
}

int Pacman::getPacmanY() const
{
    return posPacmanY;
}

int Pacman::getScore() const
{
    return score;
}

int Pacman::getVie() const
{
    return vie;
}

bool Pacman::getSuperG() const{
    return SuperG;
}

Direction Pacman::getDir() const{
  return dir;
}

////////////////////////////////////////////////////////////
//                          JEU                           //
////////////////////////////////////////////////////////////

Jeu::Jeu()
{
    terrain = NULL;
    largeur = 0; hauteur = 0;
    victoire=defaite=false;
    carte = 1;
    largeur=17;
    centreX=8;
    centreY=10;
}

Jeu::~Jeu()
{
    if (terrain!=NULL)
        delete[] terrain;
}

bool Jeu::init()
{
	int x, y, i=0;
	list<Fantome>::iterator itFantome;

  victoire = defaite = false;


  if(largeur==17)
  {
      hauteur=22;
  }


  if(largeur==23)
  {
      hauteur=17;

  }

    if(largeur==27)
    {
        hauteur=19;
    }


  terrain = new Case[largeur*hauteur];

    if(carte==2)
    {
        const char terrain_defaut[hauteur][largeur+1] = {
            "#######################",
            "#######################",
            "#P...................P#",
            "#.####.####.####.####.#",
            "#.####.##..P..##.####.#",
            "G.........###.........D",
            "####.###-------###.####",
            "####.###-##V##-###.####",
            "#......--#S-S#--......#",
            "#.#.###--#####--###.#.#",
            "#......---------......#",
            "#####.###########.#####",
            "#####.##...#...##.#####",
            "G---.....#...#.....---D",
            "####.###.##.##.###.####",
            "#.P..#....#.#....#..P.#",
            "#......##.....##......#",
            "#######################",
            };
            centreX=11;
            centreY=7;
    }else if(carte==3)
    {
        const char terrain_defaut[hauteur][largeur+1] = {
            "###########################",
            "###########################",
            "#..........#####..........#",
            "#.#.##.#.#...P...###.##.#.#",
            "#......#.##.###.##.#......#",
            "####.#...#-------#...#.####",
            "G---...#..-#####-..#...---D",
            "##-P##...#-------#...##P-##",
            "##.###.###-##V##-###.###.##",
            "#........--#S-S#--........#",
            "###.#.###--#####--###.#.###",
            "G........---------........D",
            "#.#..##.###.###.###.##..#.#",
            "#.##.##.##...#...##.##.##.#",
            "#..........#...#..........#",
            "#.#.##.##.#..#..#.##.##.#.#",
            "#.#.##P#....###....#P##.#.#",
            "G........##.....##........D",
            "###########################",
            };
            centreX=14;
            centreY=8;
    }else
    {
        const char terrain_defaut[hauteur][largeur+1] = {
            "#################",
            "#################",
            "#.......#.......#",
            "#P#.###.#.###.#P#",
            "#...............#",
            "#.#.#.#####.#.#.#",
            "#...#...#...#...#",
            "###.###.#.###.###",
            "###.#-------#.###",
            "###.#-##V##-#.###",
            "G--.--#S-S#--.--D",
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
            centreX=8;
            centreY=10;
    }




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
            else if (terrain_defaut[y][x]=='G')
                terrain[y*largeur+x] = TPG;
            else if (terrain_defaut[y][x]=='D')
                terrain[y*largeur+x] = TPD;
            else
                terrain[y*largeur+x] = VIDE;

  fantomes.resize(4);

	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        if(rand()%2==0)
        {
            x=centreX-1;
            y=centreY;
        }else
        {
            x=centreX+1;
            y=centreY;
        }

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
        itFantome->dirPrec = HAUT;
        itFantome->vivant=true;
        itFantome->col = (Couleur)(i%4);
        i++;
    }

    pacmanJ1.posPacmanX = centreX;
    pacmanJ1.posPacmanY = centreY+2;
    pacmanJ1.vie = 3;
    pacmanJ2.vie = 0;

    if (multiOnOff == true) {
      pacmanJ1.posPacmanX = centreX-3;
      pacmanJ2.posPacmanX = centreX+3;
      pacmanJ2.posPacmanY = centreY+2;
      pacmanJ2.vie = 3;
    }

    return true;
}

void Jeu::evolue()
{
	list<Fantome>::iterator itFantome;

    int depX[] = {-1, 1, 0, 0,0};
    int depY[] = {0, 0, -1, 1,0};


    //On gere les super gommes
    SuperPacman(pacmanJ1);
    SuperPacman(pacmanJ2);


    //on deplace pacman
    if (pacmanJ1.vie>0)
    {
        deplacePacman(RIEN, pacmanJ1);
    }else
    {
          pacmanJ1.posPacmanX=-1;
          pacmanJ1.posPacmanY=-1;
    }
    if (pacmanJ2.vie>0)
    {
        deplacePacman(RIEN, pacmanJ2);
    }else
    {
          pacmanJ2.posPacmanX=-1;
          pacmanJ2.posPacmanY=-1;
    }
    // on fait les test de manger
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++){
      if (FantomeMangePacman(itFantome->posX, itFantome->posY, pacmanJ1) == true) {
        PerteVie(pacmanJ1);
      }
      if (FantomeMangePacman(itFantome->posX, itFantome->posY, pacmanJ2) == true) {
        PerteVie(pacmanJ2);
      }

      if (PacmanMangeFantome(itFantome->posX, itFantome->posY, pacmanJ1) == true ) {
        itFantome->vivant=false;
        pacmanJ1.score += 200;
      }
      if (PacmanMangeFantome(itFantome->posX, itFantome->posY, pacmanJ2) == true) {
        itFantome->vivant=false;
        pacmanJ2.score += 200;
      }
    }

    //On déplace les fantomes et on test s'ils mangent ou se font manger
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        //on refait apparaitre le fantome si il est de retour au spawn apres sa mort
        if(terrain[(itFantome->posY)*largeur+itFantome->posX]==SPAWN && pacmanJ1.SuperG == 0 && pacmanJ2.SuperG == 0)
        {
            itFantome->vivant=true;
        }

        if(itFantome->vivant==true)
        {
            itFantome->dir=Poursuite(itFantome->posX,itFantome->posY,itFantome->dirPrec);//deplacement en fonction de la proximite de pacman
            if(itFantome->dir==RIEN) // si pacman loin ou pas de deplacement possible interessant
            {
                itFantome->dir=MouvFantome(itFantome->posX,itFantome->posY,itFantome->dirPrec);//deplacement aleatoire de base
            }
        }else
        {
            itFantome->dir=Retour(itFantome->posX,itFantome->posY); // deplacement du fantome vers le spawn si il est mort
        }
        //on effectue le deplacement
        if(terrain[(itFantome->posY)*largeur+itFantome->posX]==TPG && itFantome->dir==GAUCHE)
        {
            itFantome->posX=largeur-1;
        }else if(terrain[(itFantome->posY)*largeur+itFantome->posX]==TPD && itFantome->dir==DROITE)
        {
            itFantome->posX=0;
        }else
        {
            itFantome->posX = itFantome->posX + depX[itFantome->dir];
            itFantome->posY = itFantome->posY + depY[itFantome->dir];
            itFantome->dirPrec=itFantome->dir;
        }


        // on fait les test de manger
        if (FantomeMangePacman(itFantome->posX, itFantome->posY, pacmanJ1) == true) {
            PerteVie(pacmanJ1);
        }
        if (FantomeMangePacman(itFantome->posX, itFantome->posY, pacmanJ2) == true) {
            PerteVie(pacmanJ2);
        }

        if (PacmanMangeFantome(itFantome->posX, itFantome->posY, pacmanJ1) == true ) {
            itFantome->vivant=false;
            pacmanJ1.score += 200;
        }
        if (PacmanMangeFantome(itFantome->posX, itFantome->posY, pacmanJ2) == true) {
            itFantome->vivant=false;
            pacmanJ2.score += 200;
        }
    }
    TestFin();
}

void Jeu::stop_partie(){
    int x,y;
    for(y=0;y<hauteur;++y)
    {
        for(x=0;x<largeur;++x)
        {
            terrain[y*largeur+x]=VIDE;
        }
    }
    delete[] terrain;
    fantomes.clear();
}

Direction Jeu::Poursuite(int X, int Y, Direction dirPrec)
{

    //on regarde si le fantome est proche de pacman
    if (((X-pacmanJ1.posPacmanX)*(X-pacmanJ1.posPacmanX))+((Y-pacmanJ1.posPacmanY)*(Y-pacmanJ1.posPacmanY))<10)//equation de cercle
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

    int possibilite[4];//tableau pour stocker la possibilite de deplacement  0:impossible  1:vitre possible que si rien d autre 2:case vide normal 3: chance accru car meme que deplacement precedent
    int nb_2=0;
    int choix;
    int i;

    // on fait les test pour les 4 cases
    if(posValide(X-1,Y)==true || terrain[Y*largeur+X]==TPG)
    {
        possibilite[GAUCHE]=2;
    }else if(terrain[Y*largeur+X-1]==VITRE)
    {
        possibilite[GAUCHE]=1;
    }else
    {
        possibilite[GAUCHE]=0;
    }


    if(posValide(X+1,Y)==true || terrain[Y*largeur+X]==TPD)
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

    //empechement de faire demi tour
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

    //tirage si on sut meme deplacement que precedement
    if(possibilite[dirPrec]==3)
    {
        choix=1+rand()%(2+nb_2);
        if(choix<=2)
        {
            return dirPrec;
        }
    }


    //sinon tirage aux hasard des possibilite 2
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
        //sinon derniere possibilite le deplacement de possibilite 1
        for(i=0;i<4;i++)
        {
            if(possibilite[i]==1)
            {
                return (Direction)(i);
            }
        }
    }
    return RIEN;
}

Direction Jeu::Retour(int X, int Y)
{
    float distance1,distance2;
    int x,y;
    //on calcul de quelle case spawn le fantome est le plus proche pour l y envoyer
    distance1=sqrt((X-centreX-1)*(X-centreX-1)+(Y-centreY)*(Y-centreY));
    distance2=sqrt((X-centreX+1)*(X-centreX+1)+(Y-centreY)*(Y-centreY));
    //on renvoie le deplacement sur y ou x en fct de quelle est le plus loin
    if (abs(distance1)>abs(distance2))
    {
        x=X-centreX-1;
        y=Y-centreY;
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
        x=X-centreX+1;
        y=Y-centreY;
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

void Jeu::SuperPacman(Pacman &pac)
{
    if(pac.tempsSup>0)
    {
        pac.tempsSup-=1;
    }else
    {
        pac.SuperG=false;
    }
}

void Jeu::PerteVie(Pacman &pac)
{
    //on enleve une vie
    pac.vie-=1;
    restart_manche(pac);
}

void Jeu::restart_manche(Pacman &pac){
    //on remet le jeu en place
    list<Fantome>::iterator itFantome;
    int x,y,i=0;

    //reset fantomes
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        if(rand()%2==0)
        {
            x=centreX-1;
            y=centreY;
        }else
        {
            x=centreX+1;
            y=centreY;
        }
        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
        itFantome->dirPrec = DROITE;
        itFantome->vivant=true;
        itFantome->col = (Couleur)(i%4);
        i++;
    }

    pac.posPacmanX = centreX;
    pac.posPacmanY = centreY+2;

}

bool Jeu::deplacePacman(Direction dir, Pacman &pac)
{
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};
    int testX, testY;

    if (dir != RIEN){ //Si l'utilisateur change de direction via les boutons
      //on save la valeur pour le prochain evolue()
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
            pac.SuperG=true;
            pac.tempsSup+=30;
            //fct manger fantomes
          }
          pac.dir = pac.dirFutur;
          return false;
      }
      //on test si il est sur un TP
      else if(terrain[pac.posPacmanY*largeur+pac.posPacmanX]==TPG && pac.dirFutur==GAUCHE)
      {
          pac.posPacmanX=largeur-1;
      }else if(terrain[pac.posPacmanY*largeur+pac.posPacmanX]==TPD && pac.dirFutur==DROITE)
      {
          pac.posPacmanX=0;
      }
      //Sinon Evolue() il garde sa direction
      else{
        testX = pac.posPacmanX + depX[pac.dir];
        testY = pac.posPacmanY + depY[pac.dir];

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
              pac.SuperG=true;
              pac.tempsSup+=30;
              pac.score += 50;

              //fct manger fantomes
            }
        return true;
        }
        else if(terrain[pac.posPacmanY*largeur+pac.posPacmanX]==TPG && pac.dirFutur==GAUCHE)
        {
            pac.posPacmanX=largeur-1;
        }else if(terrain[pac.posPacmanY*largeur+pac.posPacmanX]==TPD && pac.dirFutur==DROITE)
        {
            pac.posPacmanX=0;
        }
      return false;
      }
    }
    return true;
}


void Jeu::setCarte(int c){
  if (c>0 && c<3)
    carte = c;
}

void Jeu::setLargeur(int l){
  if (l>0)
    largeur = l;
}


int Jeu::getNbCasesX() const
{
    return largeur;
}


int Jeu::getNbCasesY() const
{
    return hauteur;
}

bool Jeu::getvictoire()
{
    return victoire;
}

bool Jeu::getdefaite()
{
    return defaite;
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
  f.posY = 10;
  f.dir = HAUT;
  f.dirPrec = HAUT;
  f.vivant = true;
  f.col = (Couleur)(rand()%4);

  Jeu::fantomes.push_back(f);
}

void Jeu::SupprFantome(){
    if (fantomes.size() > 0) {
        fantomes.pop_back();
    }
}

bool Jeu::FantomeMangePacman(int posX, int posY, Pacman &pac) const {
  if (posX == pac.posPacmanX and posY == pac.posPacmanY && pac.SuperG==false){ //pacman se fait manger
    return true;
  }
  return false;
}

bool Jeu::PacmanMangeFantome(int posX, int posY, Pacman &pac) const {
  if (posX == pac.posPacmanX && posY == pac.posPacmanY && pac.SuperG==true){ //le fantome se fait manger
    return true;
  }
  return false;
}

void Jeu::TestFin()
{
    victoire=true;
    int x,y;
    for(y=0;y<hauteur;++y)
    {
        for(x=0;x<largeur;++x)
        {
            if(terrain[y*largeur+x]==GOMME || terrain[y*largeur+x]==POWER)
                victoire=false;
        }
    }

    if(pacmanJ1.vie<1 && pacmanJ2.vie<1)
        defaite=true;

    if(victoire==true || defaite==true)
    {
        stop_partie();
    }

}
