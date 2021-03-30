#include <iostream>
#include "pacmanwindow.h"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    if (pixmapPacman.load("./data/pacman.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman.png"<<endl;
        exit(-1);
    }

    if (pixmapFantome.load("./data/fantome.bmp")==false)
    {
        cout<<"Impossible d'ouvrir fantome.bmp"<<endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }

    if (pixmapVitre.load("./data/gate.png")==false)
    {
        cout<<"Impossible d'ouvrir gate.png"<<endl;
        exit(-1);
    }

    if (pixmapGomme.load("./data/dot.png")==false)
    {
        cout<<"Impossible d'ouvrir dot.png"<<endl;
        exit(-1);
    }

    if (pixmapPower.load("./data/power_ball.png")==false)
    {
        cout<<"Impossible d'ouvrir power_ball.png"<<endl;
        exit(-1);
    }

    // Bouton pour nb fantomes
    PacmanButton *btnAddFantome = new PacmanButton(this);
    btnAddFantome->setText("Ajouter fantome");
    btnAddFantome->setGeometry(10,8,100,17);
    connect(btnAddFantome, &QPushButton::clicked, this, &PacmanWindow::btnAjouterFantome);

    PacmanButton *btnRmvFantome = new PacmanButton(this);
    btnRmvFantome->setText("Enlever fantome");
    btnRmvFantome->setGeometry(10+100+10,8,100,17);
    connect(btnRmvFantome, &QPushButton::clicked, this, &PacmanWindow::btnSupprFantome);

    jeu.init();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
    timer->start(500); //Temps en ms entre chaque action des fantomes

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase);
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    list<Fantome>::const_iterator itFantome;
    int x, y;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (y=0;y<jeu.getNbCasesY();y++){
        for (x=0;x<jeu.getNbCasesX();x++){
      			if (jeu.getCase(x,y)==MUR)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapMur);
            else if (jeu.getCase(x,y)==VITRE)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapVitre);
            else if (jeu.getCase(x,y)==GOMME)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapGomme);
            else if (jeu.getCase(x,y)==POWER)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapPower);
        }
    }
    // Dessine les fantomes
    for (itFantome=jeu.fantomes.begin(); itFantome!=jeu.fantomes.end(); itFantome++)
        painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantome);

	// Dessine Pacman
	painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase, pixmapPacman);
}

void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Left)
        jeu.deplacePacman(GAUCHE);
    else if (event->key()==Qt::Key_Right)
        jeu.deplacePacman(DROITE);
    else if (event->key()==Qt::Key_Up)
        jeu.deplacePacman(HAUT);
    else if (event->key()==Qt::Key_Down)
        jeu.deplacePacman(BAS);

    update();
}

void PacmanWindow::btnAjouterFantome()
{
    jeu.AjouterFantome();
}

void PacmanWindow::btnSupprFantome()
{
    jeu.SupprFantome();
}

void PacmanWindow::handleTimer()
{
    jeu.evolue();
    update();
}

PacmanButton::PacmanButton(QWidget *p):QPushButton(p){
  ;
}

void PacmanButton::keyPressEvent(QKeyEvent *event)
{
    if (parent()!=NULL){
      QCoreApplication::sendEvent(parent(),event);
    }
}
