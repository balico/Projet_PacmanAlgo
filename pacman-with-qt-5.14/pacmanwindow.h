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

    QBoxLayout *layout_principal;
    QMainWindow *main_window;

    PacmanButton *btnAddFantome, *btnRmvFantome;
    PacmanButton *btnMulti;

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

    void btnAjouterFantome();
    void btnSupprFantome();
    void btnGestionMulti();
};

class PacmanButton : public QPushButton
{
    public:
      PacmanButton(QWidget *);
      void keyPressEvent(QKeyEvent *);
};

#endif
