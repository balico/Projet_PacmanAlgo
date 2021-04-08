#include <iostream>
#include "pacmanwindow.h"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
  multiOnOff = false;
  verif_images();
  Init_menu_principal();
  Init_menu_enJeu();
}

void PacmanWindow::lancement_jeu(){
  // Taille des cases en pixels
  int largeurCase, hauteurCase;
  // On check si toutes les images sont ok

  jeu.init();

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
  timer->start(500); //Temps en ms entre chaque action des fantomes

  largeurCase = pixmapMur.width();
  hauteurCase = pixmapMur.height();

  resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase);

  menu_enJeu->setVisible(true);
}

void PacmanWindow::Init_menu_principal(){

  menu_principal = new QWidget(this);
  //Définir taille window ?

  layout_menu_principal = new QVBoxLayout(menu_principal);

  // Bouton lancer le jeu
  btnStartGame = new PacmanButton(menu_principal);
  btnStartGame->setText("Start Game");
  connect(btnStartGame, &QPushButton::clicked, this, &PacmanWindow::btnGestionStartGame);
  layout_menu_principal->addWidget(btnStartGame);

  // Bouton activer ou non le multijoueur
  btnMulti = new PacmanButton(menu_principal);
  btnMulti->setText("Multijoueur (2 joueurs)");
  connect(btnMulti, &QPushButton::clicked, this, &PacmanWindow::btnGestionMulti);
  layout_menu_principal->addWidget(btnMulti);

  menu_principal->setVisible(true);
}

void PacmanWindow::Init_menu_enJeu(){

  menu_enJeu = new QWidget(this);
  //Définir taille window ?

  layout_menu_enJeu = new QHBoxLayout(menu_enJeu);

  // Bouton pour nb fantomes
  btnAddFantome = new PacmanButton(menu_enJeu);
  btnAddFantome->setText("Ajouter fantome");
  connect(btnAddFantome, &QPushButton::clicked, this, &PacmanWindow::Fct_btnAddFantome);
  layout_menu_enJeu->addWidget(btnAddFantome);

  btnRmvFantome = new PacmanButton(menu_enJeu);
  btnRmvFantome->setText("Enlever fantome");
  connect(btnRmvFantome, &QPushButton::clicked, this, &PacmanWindow::Fct_btnRmvFantome);
  layout_menu_enJeu->addWidget(btnRmvFantome);

  btnStopGame = new PacmanButton(menu_enJeu);
  btnStopGame->setText("Quitter la partie");
  connect(btnStopGame, &QPushButton::clicked, this, &PacmanWindow::Fct_btnStopGame);
  layout_menu_enJeu->addWidget(btnStopGame);

  menu_enJeu->setVisible(false);
}

void PacmanWindow::Init_bouton_jeu(){
  ;
}

void PacmanWindow::verif_images(){
  if (pixmapPacmanJ1.load("./data/pacmanJ1.bmp")==false)
  {
      cout<<"Impossible d'ouvrir pacman.png"<<endl;
      exit(-1);
  }

  if (pixmapPacmanJ2.load("./data/pacmanJ2.bmp")==false)
  {
      cout<<"Impossible d'ouvrir pacman.png"<<endl;
      exit(-1);
  }

  if (pixmapFantome.load("./data/fantome.bmp")==false)
  {
      cout<<"Impossible d'ouvrir fantome.bmp"<<endl;
      exit(-1);
  }

  if (pixmapFantomeMort.load("./data/fantome_mort.bmp")==false)
  {
      cout<<"Impossible d'ouvrir fantome Mort.bmp"<<endl;
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
}

void PacmanWindow::paintEvent(QPaintEvent *){
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
    {
        if(itFantome->getvivant()==true)
        {
            painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantome);
        }else
        {
            painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeMort);
        }
    }


	// Dessine Pacman
  painter.drawPixmap(jeu.pacmanJ1.getPacmanX()*largeurCase, jeu.pacmanJ1.getPacmanY()*hauteurCase, pixmapPacmanJ1);
  painter.drawPixmap(jeu.pacmanJ2.getPacmanX()*largeurCase, jeu.pacmanJ2.getPacmanY()*hauteurCase, pixmapPacmanJ2);
}

void PacmanWindow::keyPressEvent(QKeyEvent *event){
    if (event->key()==Qt::Key_Left)
        jeu.deplacePacman(GAUCHE, jeu.pacmanJ1);
    else if (event->key()==Qt::Key_Right)
        jeu.deplacePacman(DROITE, jeu.pacmanJ1);
    else if (event->key()==Qt::Key_Up)
        jeu.deplacePacman(HAUT, jeu.pacmanJ1);
    else if (event->key()==Qt::Key_Down)
        jeu.deplacePacman(BAS, jeu.pacmanJ1);
    else if (event->key()==Qt::Key_Q)
        jeu.deplacePacman(GAUCHE, jeu.pacmanJ2);
    else if (event->key()==Qt::Key_D)
        jeu.deplacePacman(DROITE, jeu.pacmanJ2);
    else if (event->key()==Qt::Key_Z)
        jeu.deplacePacman(HAUT, jeu.pacmanJ2);
    else if (event->key()==Qt::Key_S)
        jeu.deplacePacman(BAS, jeu.pacmanJ2);

    update();
}

void PacmanWindow::btnGestionMulti(){
  multiOnOff = !multiOnOff;
  std::cout << "multi : " << multiOnOff << '\n';
}

void PacmanWindow::btnGestionStartGame(){
  menu_principal->setVisible(false);
  lancement_jeu();
}

void PacmanWindow::Fct_btnAddFantome(){
  jeu.AjouterFantome();
}

void PacmanWindow::Fct_btnRmvFantome(){
    jeu.SupprFantome();
}

void PacmanWindow::Fct_btnStopGame(){
  ;
}

void PacmanWindow::handleTimer(){
    jeu.evolue();
    update();
}

PacmanButton::PacmanButton(QWidget *p):QPushButton(p){
  ;
}

void PacmanButton::keyPressEvent(QKeyEvent *event){
    if (parent()!=NULL){
      QCoreApplication::sendEvent(parent(),event);
    }
}
