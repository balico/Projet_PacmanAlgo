#ifndef PACMANWINDOW_H
#define PACMANWINDOW_H

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.h"

class PacmanButton;

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapPacmanJ1, pixmapPacmanJ2, pixmapFantome,
      pixmapMur, pixmapGomme, pixmapVitre, pixmapPower, pixmapFantomeMort;

    //Var pour savoir si le nb de joueur
    bool multiOnOff; //false = solo, true = multi

    //Affichage menu initial
    QWidget *menu_principal;
    QVBoxLayout *layout_menu_principal;
    //Bouton du menu
    PacmanButton *btnMulti, *btnStartGame;

    //Affichage durant la partie
    QWidget *menu_enJeu;
    QHBoxLayout *layout_menu_enJeu;
    //Bouton de l'affichage durant la partie
    PacmanButton *btnAddFantome, *btnRmvFantome, *btnStopGame;
    //Affichage score, vie
    QLabel *affi_scoreJ1, *affi_scoreJ2;
    QLabel *affi_vieJ1, *affi_vieJ2;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=0);
    void handleTimer();

  protected:
    void verif_images();

    void Init_menu_principal();
    //initialisation des boutons present pendant le jeu (pause, play, ajout d'ennemi...)
    void Init_bouton_jeu();
    //lance le jeu apres un appui sur play dans le menu principal
    void lancement_jeu();
    void Fct_btnStopGame();

    void Init_menu_enJeu();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

    void btnGestionMulti();
    void btnGestionStartGame();

    void Fct_btnAddFantome();
    void Fct_btnRmvFantome();
};

class PacmanButton : public QPushButton
{
    public:
      PacmanButton(QWidget *);
      void keyPressEvent(QKeyEvent *);
};

#endif
