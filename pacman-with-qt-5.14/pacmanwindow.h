#ifndef PACMANWINDOW_H
#define PACMANWINDOW_H

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.h"

class PacmanButton;
class SimpleMenu;

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;

    //Variable qui permet de savoir quand afficher les pacmans
    //True = dans le menu principal, false = affichage pacman
    bool DansMenu;
    QTimer *timer;

    //Les images
    QPixmap pixmapPacmanJ1D, pixmapPacmanJ1B, pixmapPacmanJ1G, pixmapPacmanJ1H,
      pixmapPacmanJ2D, pixmapPacmanJ2B, pixmapPacmanJ2G, pixmapPacmanJ2H,
      pixmapFantomeRedD, pixmapFantomeRedB, pixmapFantomeRedG, pixmapFantomeRedH,
      pixmapFantomePinkD, pixmapFantomePinkB, pixmapFantomePinkG, pixmapFantomePinkH,
      pixmapFantomeGreenD, pixmapFantomeGreenB, pixmapFantomeGreenG, pixmapFantomeGreenH,
      pixmapFantomeOrangeD, pixmapFantomeOrangeB, pixmapFantomeOrangeG, pixmapFantomeOrangeH,
      pixmapFantomeFear, pixmapFantomeDead,
      pixmapMur, pixmapGomme, pixmapVitre, pixmapPower;

    //Affichage menu initial
    QWidget *menu_principal;
    QVBoxLayout *layout_menu_principal;
    //Bouton du menu
    PacmanButton *btnMulti, *btnStartGame, *btnSelec_lvl;
    //Affichage multijoueur OnOff
    QLabel *affi_multiOnOff;
    QMenu *menu_selec_lvl;
    QAction *niveau_1, *niveau_2, *niveau_3;

    //Affichage durant la partie
    QWidget *menu_enJeu;
    QHBoxLayout *layout_menu_enJeu;
    //Bouton de l'affichage durant la partie
    PacmanButton *btnAddFantome, *btnRmvFantome, *btnStopGame;
    //Affichage score, vie
    QLabel *affi_scoreJ1, *affi_scoreJ2;
    QLabel *affi_vieJ1, *affi_vieJ2;

    //Affichage fin de partie
    QWidget *fin_partie;
    QVBoxLayout *layout_fin_partie;
    //Bouton du menu
    PacmanButton *btnRetourMenu;
    //Affichage de la condition de fin et des scores
    QLabel *affi_Condition_fin, *affi_ScoreJ1final, *affi_ScoreJ2final;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=0);
    void handleTimer();


  protected:
    void verif_images();

    void Init_menu_principal();
    void Init_menu_enJeu();
    void Init_fin_partie();

    //lance le jeu apres un appui sur play dans le menu principal
    void lancement_jeu();
    void Fct_btnStopGame();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

    void btnGestionMulti();
    void btnGestionStartGame();

    void btnGestionRetourMenu();

    void Fct_btnAddFantome();
    void Fct_btnRmvFantome();

    void Fct_affichageScore();
    void Fct_affichageVie();
};

////////////////////////////////////////////////////////////
//                    PacmanButton                        //
////////////////////////////////////////////////////////////

class PacmanButton : public QPushButton
{
    public:
      PacmanButton(QWidget *);
      void keyPressEvent(QKeyEvent *);
};

////////////////////////////////////////////////////////////
//                      SimpleMenu                        //
////////////////////////////////////////////////////////////

class SimpleMenu : public QMainWindow {

  public:
    SimpleMenu(QWidget *parent = nullptr);
};

#endif
