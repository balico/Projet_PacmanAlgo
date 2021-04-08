#ifndef JEU_H
#define JEU_H

#include <list>
using namespace std;

typedef enum {VIDE, MUR, VITRE, GOMME, POWER, SPAWN} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS, RIEN} Direction;

class Jeu;

class Fantome
{
    friend class Jeu;

  protected:
    int posX, posY;
    Direction dir;
    Direction dirPrec;
    bool vivant;

  public:
    Fantome();
    int getPosX() const;
    int getPosY() const;
};

class Pacman
{
    friend class Jeu;

  protected:
    int posPacmanX, posPacmanY;
    Direction dir, dirFutur;
    int score;

  public:
    Pacman();
    int getPacmanX() const;
    int getPacmanY() const;
};

class Jeu
{
  protected:
    Case *terrain;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPacmanX, posPacmanY;

  public:
    list<Fantome> fantomes;
    Pacman pacmanJ1;
    Pacman pacmanJ2;

  public:
    Jeu();
    Jeu(const Jeu &)=delete;
    ~Jeu();

    Jeu &operator=(const Jeu &)=delete;

    bool init();
    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Retourne la case à une position donnée
    Case getCase(int, int) const;

    // Indique si la case à une position donnée existe et est vide
    // (Pacman ou les fantomes peuvent l'occuper)
    bool posValide(int, int) const;

    // Déplace Pacman dans une direction (si la case à atteindre est valide)
    bool deplacePacman(Direction, Pacman &);

    // Gestion des fantomes
    void AjouterFantome();
    void SupprFantome();

    // Return si pacman se fait manger (true) ou non
    // bool FantomeMangePacman(Fantome) const;
    bool FantomeMangePacman(int, int, Pacman &) const;

    //return la direction que prend un fantome pour poursuivre pacman
    Direction Poursuite(int, int, Direction);

    Direction MouvFantome(int , int ,Direction);
    Direction Retour(int , int );
};

#endif
