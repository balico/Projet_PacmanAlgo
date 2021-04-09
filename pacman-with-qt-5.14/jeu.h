#ifndef JEU_H
#define JEU_H

#include <list>
using namespace std;

typedef enum {VIDE, MUR, VITRE, GOMME, POWER, SPAWN, TPG, TPD} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS, RIEN} Direction;
typedef enum {ROUGE, ORANGE, VERT, ROSE} Couleur;

class Jeu;

class Fantome
{
    friend class Jeu;

  protected:
    int posX, posY;
    Direction dir, dirPrec;
    bool vivant;
    Couleur col;

  public:
    Fantome();
    int getPosX() const;
    int getPosY() const;

    bool getvivant() const;
    Couleur getCouleur() const;
    Direction getDir() const;
};

class Pacman
{
    friend class Jeu;

  protected:
    int posPacmanX, posPacmanY;
    Direction dir, dirFutur;
    int score, vie;
    bool SuperG;
    int tempsSup;

  public:
    Pacman();
    int getPacmanX() const;
    int getPacmanY() const;
    int getScore() const;
    int getVie() const;
    bool getSuperG() const;
    Direction getDir() const;
};

class Jeu
{
  protected:
    Case *terrain;
    int carte; //quelle carte
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPacmanX, posPacmanY;
    bool victoire, defaite;

  public:
    list<Fantome> fantomes;
    Pacman pacmanJ1;
    Pacman pacmanJ2;

    //Var pour savoir si il y a 1 ou 2 joueurs
    bool multiOnOff; //false = solo, true = multi

  public:
    Jeu();
    Jeu(const Jeu &)=delete;
    ~Jeu();

    Jeu &operator=(const Jeu &)=delete;

    bool init();
    void generationCarte();
    void evolue();
    void restart_manche(Pacman &); //relance une manche
    void stop_partie(); //stop la partie

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    void setLargeur(int);
    void setHauteur(int);
    void setCarte(int);

    bool getvictoire();
    bool getdefaite();

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

    //gere les super gommes
    void SuperPacman(Pacman &);

    //fonction qui enleve une vie a pacman quand il est touche et remet tout en place
    void PerteVie(Pacman &);

    // Return si pacman se fait manger (true) ou non
    // bool FantomeMangePacman(Fantome) const;
    bool FantomeMangePacman(int, int, Pacman &) const;
    bool PacmanMangeFantome(int, int, Pacman &) const;

    //return la direction que prend un fantome pour poursuivre pacman
    Direction Poursuite(int, int, Direction);

    Direction MouvFantome(int , int ,Direction);
    Direction Retour(int , int );

    //fonction gerant la detection de la fin de partie
    void TestFin();
};

#endif
