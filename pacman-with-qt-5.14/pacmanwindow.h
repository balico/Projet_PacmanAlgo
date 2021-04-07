#ifndef PACMANWINDOW_H
#define PACMANWINDOW_H

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.h"

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapPacmanJ1, pixmapPacmanJ2, pixmapFantome, pixmapMur, pixmapGomme, pixmapVitre, pixmapPower;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=0);
    void handleTimer();

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void btnAjouterFantome();
    void btnSupprFantome();
};

class PacmanButton : public QPushButton
{
    public:
      PacmanButton(QWidget *);

    protected:
      void keyPressEvent(QKeyEvent *);
};

#endif
