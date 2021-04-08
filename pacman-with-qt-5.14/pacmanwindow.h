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
      pixmapMur, pixmapGomme, pixmapVitre, pixmapPower;

    QWidget *menu_principal;
    QVBoxLayout *layout_menu_principal;
    QMainWindow *main_window;

    PacmanButton *btnMulti, *btnStartGame;
    PacmanButton *btnAddFantome, *btnRmvFantome;

    bool multiOnOff; //false = solo, true = multi

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=0);
    void handleTimer();

  protected:
    void verif_images();

    void Init_menu();
    //initialisation des boutons present pendant le jeu (pause, play, ajout d'ennemi...)
    void Init_bouton_jeu();
    //lance le jeu apres un appui sur play dans le menu principal
    void lancement_jeu();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

    void btnGestionMulti();
    void btnGestionStartGame();

    void btnAjouterFantome();
    void btnSupprFantome();
};

class PacmanButton : public QPushButton
{
    public:
      PacmanButton(QWidget *);
      void keyPressEvent(QKeyEvent *);
};

#endif
