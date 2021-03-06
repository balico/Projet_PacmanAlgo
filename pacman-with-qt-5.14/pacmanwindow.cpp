#include <iostream>
#include "pacmanwindow.h"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
  DansMenu = true; //de base nous sommes dans un menu
  jeu.multiOnOff = false; //pas de multi
  // On check si toutes les images sont ok et on init
  verif_images();
  Init_menu_principal();
  Init_menu_enJeu();
  Init_fin_partie();
}

void PacmanWindow::lancement_jeu(){
  // Taille des cases en pixels
  int largeurCase, hauteurCase;

  //On regarde quelle carte est jouée
  if (jeu.getCarte() == 2) {
    jeu.init2();
  }
  else if (jeu.getCarte() == 3) {
    jeu.init3();
  }
  else{
    jeu.init1();
  }

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
  timer->start(250); //Temps en ms entre chaque action des fantomes

  largeurCase = pixmapMur.width();
  hauteurCase = pixmapMur.height();

  resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase);

  menu_enJeu->setVisible(true);
}

void PacmanWindow::Init_menu_principal(){

  menu_principal = new QWidget(this);

  layout_menu_principal = new QVBoxLayout(menu_principal);

  // Bouton lancer le jeu
  btnStartGame = new PacmanButton(menu_principal);
  btnStartGame->setText("Start Game");
  connect(btnStartGame, &QPushButton::clicked, this, &PacmanWindow::btnGestionStartGame);
  layout_menu_principal->addWidget(btnStartGame);

  // Bouton activer ou non le multijoueur
  btnMulti = new PacmanButton(menu_principal);
  btnMulti->setText("Multijoueur");
  connect(btnMulti, &QPushButton::clicked, this, &PacmanWindow::btnGestionMulti);
  layout_menu_principal->addWidget(btnMulti);

  //Affichage nb de joueur
  affi_multiOnOff = new QLabel(menu_principal);
  affi_multiOnOff->setFrameStyle(QFrame::Box | QFrame::Raised);
  affi_multiOnOff->setAlignment(Qt::AlignCenter);
  affi_multiOnOff->setText(" Nombre de Joueurs \n1");
  layout_menu_principal->addWidget(affi_multiOnOff);

  //Gestion menu des lvl
  btnSelec_lvl = new PacmanButton(menu_principal);
  menu_selec_lvl = new QMenu(menu_principal);
  //text de base
  btnSelec_lvl->setText(" niveau 1 ");
  //different choix du menu
  niveau_1 = new QAction(" niveau 1 ", this);
  connect(niveau_1, &QAction::triggered, this, &PacmanWindow::Fct_lvl1);
  menu_selec_lvl->addAction(niveau_1);

  niveau_2 = new QAction(" niveau 2 ", this);
  connect(niveau_2, &QAction::triggered, this, &PacmanWindow::Fct_lvl2);
  menu_selec_lvl->addAction(niveau_2);

  niveau_3 = new QAction(" niveau 3 ", this);
  connect(niveau_3, &QAction::triggered, this, &PacmanWindow::Fct_lvl3);
  menu_selec_lvl->addAction(niveau_3);
  //on accroche le menu au bouton puis au layout_menu_principal
  btnSelec_lvl->setMenu(menu_selec_lvl);
  layout_menu_principal->addWidget(btnSelec_lvl);

  menu_principal->setVisible(true);
}

void PacmanWindow::Init_menu_enJeu(){

  menu_enJeu = new QWidget(this);

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

  // Bouton pour quitter la partie
  btnStopGame = new PacmanButton(menu_enJeu);
  btnStopGame->setText("Exit");
  connect(btnStopGame, &QPushButton::clicked, this, &PacmanWindow::Fct_btnStopGame);
  layout_menu_enJeu->addWidget(btnStopGame);

  // Boutons pour afficher les scores & les vies
  affi_scoreJ1 = new QLabel(menu_principal);
  affi_scoreJ1->setFrameStyle(QFrame::Box | QFrame::Raised);
  affi_scoreJ1->setAlignment(Qt::AlignCenter);
  affi_scoreJ1->setStyleSheet("QLabel { background-color : white }");
  affi_scoreJ1->setText(" Score J1 : \n0");
  layout_menu_enJeu->addWidget(affi_scoreJ1);

  affi_vieJ1 = new QLabel(menu_principal);
  affi_vieJ1->setFrameStyle(QFrame::Box | QFrame::Raised);
  affi_vieJ1->setAlignment(Qt::AlignCenter);
  affi_vieJ1->setStyleSheet("QLabel { background-color : white }");
  affi_vieJ1->setText(" Vie J1 : \n3");
  layout_menu_enJeu->addWidget(affi_vieJ1);

  affi_scoreJ2 = new QLabel(menu_principal);
  affi_scoreJ2->setFrameStyle(QFrame::Box | QFrame::Raised);
  affi_scoreJ2->setAlignment(Qt::AlignCenter);
  affi_scoreJ2->setStyleSheet("QLabel { background-color : white }");
  affi_scoreJ2->setText(" Score J2 : \n0");
  layout_menu_enJeu->addWidget(affi_scoreJ2);

  affi_vieJ2 = new QLabel(menu_principal);
  affi_vieJ2->setFrameStyle(QFrame::Box | QFrame::Raised);
  affi_vieJ2->setAlignment(Qt::AlignCenter);
  affi_vieJ2->setStyleSheet("QLabel { background-color : white }");
  affi_vieJ2->setText(" Vie J2 : \n3");
  layout_menu_enJeu->addWidget(affi_vieJ2);

  affi_scoreJ2->hide(); //On cache car de base pas de multi
  affi_vieJ2->hide();
  menu_enJeu->setVisible(false); //non visible de base, visible une fois la partie ON
}

void PacmanWindow::Init_fin_partie(){

    fin_partie = new QWidget(this);

    layout_fin_partie = new QVBoxLayout(fin_partie);

    // Bouton revenir au menu
    btnRetourMenu = new PacmanButton(fin_partie);
    btnRetourMenu->setText(" Retour au menu principal ");
    connect(btnRetourMenu, &QPushButton::clicked, this, &PacmanWindow::btnGestionRetourMenu);
    layout_fin_partie->addWidget(btnRetourMenu);

    //Si on gagne et que la carte 1 ou 2 est jouée, on peut choisir de continuer
    btnLvlSuivant = new PacmanButton(fin_partie);
    btnLvlSuivant->setText(" Niveau suivant ");
    connect(btnLvlSuivant, &QPushButton::clicked, this, &PacmanWindow::btnGestionLvlSuivant);
    layout_fin_partie->addWidget(btnLvlSuivant);
    btnLvlSuivant->hide();
    if (jeu.getvictoire() == true && jeu.getCarte() != 3) {
      btnLvlSuivant->show();
    }

    //affichage victoire/defaite
    affi_Condition_fin = new QLabel(menu_principal);
    affi_Condition_fin->setFrameStyle(QFrame::Box | QFrame::Raised);
    affi_Condition_fin->setAlignment(Qt::AlignCenter);
    affi_Condition_fin->setText("  \n1");
    layout_fin_partie->addWidget(affi_Condition_fin);

    //affichage score J1
    affi_ScoreJ1final = new QLabel(menu_principal);
    affi_ScoreJ1final->setFrameStyle(QFrame::Box | QFrame::Raised);
    affi_ScoreJ1final->setAlignment(Qt::AlignCenter);
    affi_ScoreJ1final->setText("  \n1");
    layout_fin_partie->addWidget(affi_ScoreJ1final);

    //affichage score J2
    affi_ScoreJ2final = new QLabel(menu_principal);
    affi_ScoreJ2final->setFrameStyle(QFrame::Box | QFrame::Raised);
    affi_ScoreJ2final->setAlignment(Qt::AlignCenter);
    affi_ScoreJ2final->setText("  \n1");
    layout_fin_partie->addWidget(affi_ScoreJ2final);

    fin_partie->setVisible(false);
}

void PacmanWindow::Fct_lvl1(){
  btnSelec_lvl->setText(" niveau 1 "); //Le texte principal du bouton est changé
  jeu.setCarte(1);
}

void PacmanWindow::Fct_lvl2(){
  btnSelec_lvl->setText(" niveau 2 "); //Le texte principal du bouton est changé
  jeu.setCarte(2);
}

void PacmanWindow::Fct_lvl3(){
  btnSelec_lvl->setText(" niveau 3 "); //Le texte principal du bouton est changé
  jeu.setCarte(3);
}

void PacmanWindow::Fct_affichageScore(){

  affi_scoreJ1->setText(" Score J1 : \n" + QString::number(jeu.pacmanJ1.getScore()));
  if (jeu.multiOnOff == true) //Si multi, on met a jour le score
    affi_scoreJ2->setText(" Score J2 : \n" + QString::number(jeu.pacmanJ2.getScore()));
}

void PacmanWindow::Fct_affichageVie(){

  affi_vieJ1->setText(" Vie J1 : \n" + QString::number(jeu.pacmanJ1.getVie()));
  if (jeu.multiOnOff == true) //Si multi, on met a jour la vie
    affi_vieJ2->setText(" Vie J2 : \n" + QString::number(jeu.pacmanJ2.getVie()));
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
  jeu.multiOnOff = !jeu.multiOnOff; //On swap la valeur de multijoueur
  if (jeu.multiOnOff == false){ //Si pas de multi, on efface le score de J2
    affi_multiOnOff->setText(" Nombre de Joueurs \n1");
    affi_scoreJ2->hide();
    affi_vieJ2->hide();
  }
  if (jeu.multiOnOff == true){ //Si multi, on affiche le score de J2
    affi_multiOnOff->setText(" Nombre de Joueurs \n2");
    affi_scoreJ2->show();
    affi_vieJ2->show();
  }
}

void PacmanWindow::verif_images(){

  //Pacman Joueur 1
  if (pixmapPacmanJ1D.load("./data/pacmans/pacmanJ1.png")==false)
  {
    cout<<"Impossible d'ouvrir pacmanJ1.png"<<endl;
    exit(-1);
  }
  if (pixmapPacmanJ1B.load("./data/pacmans/pacmanJ1B.png")==false)
  {
    cout<<"Impossible d'ouvrir pacmanJ1.png"<<endl;
    exit(-1);
  }
  if (pixmapPacmanJ1G.load("./data/pacmans/pacmanJ1G.png")==false)
  {
    cout<<"Impossible d'ouvrir pacmanJ1.png"<<endl;
    exit(-1);
  }
  if (pixmapPacmanJ1H.load("./data/pacmans/pacmanJ1H.png")==false)
  {
    cout<<"Impossible d'ouvrir pacmanJ1.png"<<endl;
    exit(-1);
  }
  //Pacman Joueur 2
  if (pixmapPacmanJ2D.load("./data/pacmans/pacmanJ2.png")==false)
  {
    cout<<"Impossible d'ouvrir pacmanJ2.png"<<endl;
    exit(-1);
  }
  if (pixmapPacmanJ2B.load("./data/pacmans/pacmanJ2B.png")==false)
  {
    cout<<"Impossible d'ouvrir pacmanJ2.png"<<endl;
    exit(-1);
  }
  if (pixmapPacmanJ2G.load("./data/pacmans/pacmanJ2G.png")==false)
  {
    cout<<"Impossible d'ouvrir pacmanJ2.png"<<endl;
    exit(-1);
  }
  if (pixmapPacmanJ2H.load("./data/pacmans/pacmanJ2H.png")==false)
  {
    cout<<"Impossible d'ouvrir pacmanJ2.png"<<endl;
    exit(-1);
  }
  //FANTOMES
  //No direction
  if (pixmapFantomeFear.load("./data/fantomes/fantome_peur.png")==false)
  {
    cout<<"Impossible d'ouvrir fantome_peur.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeDead.load("./data/fantomes/fantome_mort.png")==false)
  {
    cout<<"Impossible d'ouvrir fantome_mort.png"<<endl;
    exit(-1);
  }
  //Direction DROITE
  if (pixmapFantomeRedD.load("./data/fantomes/fantomeR.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeR.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomePinkD.load("./data/fantomes/fantomeRose.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeRose.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeGreenD.load("./data/fantomes/fantomeG.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeG.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeOrangeD.load("./data/fantomes/fantomeO.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeO.png"<<endl;
    exit(-1);
  }
  //Direction BAS
  if (pixmapFantomeRedB.load("./data/fantomes/fantomeRBas.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeR.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomePinkB.load("./data/fantomes/fantomeRoseBas.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeRose.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeGreenB.load("./data/fantomes/fantomeGBas.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeG.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeOrangeB.load("./data/fantomes/fantomeOBas.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeO.png"<<endl;
    exit(-1);
  }
  //Direction GAUCHE
  if (pixmapFantomeRedG.load("./data/fantomes/fantomeRGauche.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeR.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomePinkG.load("./data/fantomes/fantomeRoseGauche.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeRose.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeGreenG.load("./data/fantomes/fantomeGGauche.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeG.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeOrangeG.load("./data/fantomes/fantomeOGauche.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeO.png"<<endl;
    exit(-1);
  }
  //Direction HAUT
  if (pixmapFantomeRedH.load("./data/fantomes/fantomeRHaut.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeR.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomePinkH.load("./data/fantomes/fantomeRoseHaut.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeRose.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeGreenH.load("./data/fantomes/fantomeGHaut.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeG.png"<<endl;
    exit(-1);
  }
  if (pixmapFantomeOrangeH.load("./data/fantomes/fantomeOHaut.png")==false)
  {
    cout<<"Impossible d'ouvrir fantomeO.png"<<endl;
    exit(-1);
  }
  //Block
  if (pixmapMur.load("./data/block.png")==false)
  {
    cout<<"Impossible d'ouvrir block.png"<<endl;
    exit(-1);
  }
  if (pixmapVitre.load("./data/gate.png")==false)
  {
    cout<<"Impossible d'ouvrir gate.png"<<endl;
    exit(-1);
  }
  //Manger
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
    if(itFantome->getvivant()==false)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeDead);
    else if(jeu.pacmanJ1.getSuperG() == true || jeu.pacmanJ2.getSuperG() == true)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeFear);
    //ROUGE
    else if(itFantome->getCouleur() == ROUGE && itFantome->getDir() == DROITE)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeRedD);
    else if(itFantome->getCouleur() == ROUGE && itFantome->getDir() == BAS)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeRedB);
    else if(itFantome->getCouleur() == ROUGE && itFantome->getDir() == GAUCHE)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeRedG);
    else if(itFantome->getCouleur() == ROUGE && itFantome->getDir() == HAUT)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeRedH);
    else if(itFantome->getCouleur() == ROUGE && itFantome->getDir() == RIEN)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeRedD);
    //ROSE
    else if(itFantome->getCouleur() == ROSE && itFantome->getDir() == DROITE)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomePinkD);
    else if(itFantome->getCouleur() == ROSE && itFantome->getDir() == BAS)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomePinkB);
    else if(itFantome->getCouleur() == ROSE && itFantome->getDir() == GAUCHE)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomePinkG);
    else if(itFantome->getCouleur() == ROSE && itFantome->getDir() == HAUT)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomePinkH);
    else if(itFantome->getCouleur() == ROSE && itFantome->getDir() == RIEN)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomePinkD);
    //VERT
    else if(itFantome->getCouleur() == VERT && itFantome->getDir() == DROITE)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeGreenD);
    else if(itFantome->getCouleur() == VERT && itFantome->getDir() == BAS)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeGreenB);
    else if(itFantome->getCouleur() == VERT && itFantome->getDir() == GAUCHE)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeGreenG);
    else if(itFantome->getCouleur() == VERT && itFantome->getDir() == HAUT)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeGreenH);
    else if(itFantome->getCouleur() == VERT && itFantome->getDir() == RIEN)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeGreenD);
    //ORANGE
    else if(itFantome->getCouleur() == ORANGE && itFantome->getDir() == DROITE)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeOrangeD);
    else if(itFantome->getCouleur() == ORANGE && itFantome->getDir() == BAS)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeOrangeB);
    else if(itFantome->getCouleur() == ORANGE && itFantome->getDir() == GAUCHE)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeOrangeG);
    else if(itFantome->getCouleur() == ORANGE && itFantome->getDir() == HAUT)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeOrangeH);
    else if(itFantome->getCouleur() == ORANGE && itFantome->getDir() == RIEN)
    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantomeOrangeD);
  }

  // Dessine Pacman si on n'est pas dans le menu principal
  if (DansMenu == false) {
    if(jeu.pacmanJ1.getDir() == DROITE)
    painter.drawPixmap(jeu.pacmanJ1.getPacmanX()*largeurCase, jeu.pacmanJ1.getPacmanY()*hauteurCase, pixmapPacmanJ1D);
    else if(jeu.pacmanJ1.getDir() == BAS)
    painter.drawPixmap(jeu.pacmanJ1.getPacmanX()*largeurCase, jeu.pacmanJ1.getPacmanY()*hauteurCase, pixmapPacmanJ1B);
    else if(jeu.pacmanJ1.getDir() == GAUCHE)
    painter.drawPixmap(jeu.pacmanJ1.getPacmanX()*largeurCase, jeu.pacmanJ1.getPacmanY()*hauteurCase, pixmapPacmanJ1G);
    else if(jeu.pacmanJ1.getDir() == HAUT)
    painter.drawPixmap(jeu.pacmanJ1.getPacmanX()*largeurCase, jeu.pacmanJ1.getPacmanY()*hauteurCase, pixmapPacmanJ1H);

    if (jeu.multiOnOff == true) { //Dessine pacmanJ2 si le multi est ON
      if(jeu.pacmanJ2.getDir() == DROITE)
      painter.drawPixmap(jeu.pacmanJ2.getPacmanX()*largeurCase, jeu.pacmanJ2.getPacmanY()*hauteurCase, pixmapPacmanJ2D);
      else if(jeu.pacmanJ2.getDir() == BAS)
      painter.drawPixmap(jeu.pacmanJ2.getPacmanX()*largeurCase, jeu.pacmanJ2.getPacmanY()*hauteurCase, pixmapPacmanJ2B);
      else if(jeu.pacmanJ2.getDir() == GAUCHE)
      painter.drawPixmap(jeu.pacmanJ2.getPacmanX()*largeurCase, jeu.pacmanJ2.getPacmanY()*hauteurCase, pixmapPacmanJ2G);
      else if(jeu.pacmanJ2.getDir() == HAUT)
      painter.drawPixmap(jeu.pacmanJ2.getPacmanX()*largeurCase, jeu.pacmanJ2.getPacmanY()*hauteurCase, pixmapPacmanJ2H);
    }
  }
}

void PacmanWindow::btnGestionStartGame(){
  menu_principal->setVisible(false); //on efface le menu principal
  DansMenu = false; //On indique que l'on sort du menu principal (pour l'affichage)
  lancement_jeu();
}

void PacmanWindow::Fct_btnAddFantome(){
  jeu.AjouterFantome();
}

void PacmanWindow::Fct_btnRmvFantome(){
  jeu.SupprFantome();
}

void PacmanWindow::Fct_btnStopGame(){
  menu_principal->setVisible(true); //On affiche le menu principal
  menu_enJeu->setVisible(false); //On cache le menu in-game
  DansMenu = true; //On indique que l'on retourne dans le menu principal (pour l'affichage)
  jeu.stop_partie();
  timer->stop();
  menu_principal->resize(1000,1000); //resize pour effacer les case (bug d'affichage ?)
  menu_principal->resize(150,135);
}

void PacmanWindow::btnGestionRetourMenu(){
  fin_partie->setVisible(false); //on efface l'affichage de fin
  menu_principal->setVisible(true); //On affiche le menu principal
}

void PacmanWindow::btnGestionLvlSuivant(){
  fin_partie->setVisible(false); //on efface l'affichage de fin
  menu_enJeu->setVisible(true); //On affiche le menu en jeu
  DansMenu = false;
  //On lance la carte suivante
  if (jeu.getCarte() == 1) {
    jeu.setCarte(2);
    lancement_jeu();
  }
  else if (jeu.getCarte() == 2) {
    jeu.setCarte(3);
    lancement_jeu();
  }
}

void PacmanWindow::handleTimer(){
  //On met a jours les valeurs de score et la vie a chaque "tour"
  Fct_affichageScore();
  Fct_affichageVie();
  jeu.evolue();
  if(jeu.getvictoire()==true || jeu.getdefaite()==true) //test fin de la partie
  {
      if(jeu.getvictoire()==true) //affichage de la raison de fin
        affi_Condition_fin->setText(" Victoire! ");
      else
        affi_Condition_fin->setText(" Défaite! ");

      affi_ScoreJ1final->setText(" Score J1 : \n"+ QString::number(jeu.pacmanJ1.getScore()));
      affi_ScoreJ2final->hide();
      if (jeu.multiOnOff == true) {
        affi_ScoreJ2final->show();
        affi_ScoreJ2final->setText(" Score J2 : \n"+ QString::number(jeu.pacmanJ2.getScore()));
      }

    //on affiche le tout
      fin_partie->setVisible(true);
      menu_enJeu->setVisible(false);
      DansMenu = true;
      //et on arrete le timer
      timer->stop();
      menu_principal->resize(1000,1000);
      menu_principal->resize(150,135);
  }
  update();
}

////////////////////////////////////////////////////////////
//                    PacmanButton                        //
////////////////////////////////////////////////////////////

PacmanButton::PacmanButton(QWidget *p):QPushButton(p){}

void PacmanButton::keyPressEvent(QKeyEvent *event){
    if (parent()!=NULL){
      QCoreApplication::sendEvent(parent(),event);
    }
}
