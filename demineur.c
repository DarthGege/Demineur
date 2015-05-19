#include "toolbox.h"
#define SCALE 33
#define X_MAX 30
#define Y_MAX 30

int NB_X = 0;
int NB_Y = 0;
int NB_MINE = 0;
int NB_FLAG = 0;
int NB_FLAG_MINE = 0;
int NB_HIDDEN = 0;
int MODE = 3;
int END = 0;
IMAGE images[32];

/* Déplacé dans la graphics.h
struct souris {
  POINT coord;
  int bouton;
};
typedef struct souris SOURIS;
/**/

/*
struct cellule {//BLURP
  int mine;
  int chiffre;
  int affichage;
};
typedef struct cellule CELLULE;
/**/

typedef struct {
  int mine;
  int chiffre;
  int affichage;
} CELLULE;

struct grille {//BLURP
  CELLULE cell[X_MAX][Y_MAX];
};
typedef struct grille GRILLE;

GRILLE grille;

    /* Fonctions : */

int input_entier() { //BLURP // Récupéré de toolbox.c pour y apporter des modifications d'affichage
  int n;
  fflush(stdout);
  scanf("%d",&n);
  return n;
}

void init_input() {
  images[0] = charge_image("res/0.bmp");
  images[1] = charge_image("res/1.bmp");
  images[2] = charge_image("res/2.bmp");
  images[3] = charge_image("res/3.bmp");
  images[4] = charge_image("res/4.bmp");
  images[5] = charge_image("res/5.bmp");
  images[6] = charge_image("res/6.bmp");
  images[7] = charge_image("res/7.bmp");
  images[8] = charge_image("res/8.bmp");
  images[9] = charge_image("res/mine.bmp"); // 8
  images[10] = charge_image("res/redmine.bmp"); // 6
  images[11] = charge_image("res/error.bmp"); // 9
  images[12] = charge_image("res/flag.bmp"); // 4
  images[13] = charge_image("res/hint.bmp"); // 5
  images[14] = charge_image("res/unknow.bmp"); // 0
  images[15] = charge_image("res/safe.bmp"); // 10
  images[16] = charge_image("res/danger.bmp"); // 11
  images[17] = charge_image("res/win.bmp"); // 7
  
  do {
    printf("Grille de : (9-%d) X = ? ",X_MAX);
    NB_X = input_entier();
  } while(NB_X > X_MAX || NB_X < 9);
  do {
    printf("Grille de : (9-%d) Y = ? ",Y_MAX);
    NB_Y = input_entier();
  } while(NB_Y > Y_MAX || NB_Y < 9);
  NB_HIDDEN = NB_X*NB_Y;
  do {
    printf("Nombre de mine : (10-%d) ? ",(NB_X-1)*(NB_Y-1));
    NB_MINE = input_entier();
  } while(NB_MINE > (NB_X-1)*(NB_Y-1) || NB_MINE < 10);
  do {
    printf("Mode de jeux : (1:mine possible 2:safe 3:ouverture) ? ");
    MODE = input_entier();
  } while(MODE > 3 || MODE < 1);
}

/* Déplacé dans la graphics.c
SOURIS attendre_multiclic() {
  SOURIS P;
  
  // On force l'attente du prochain clic!
  in.mouse_clic[0].x = in.mouse_clic[0].y = -1;
  in.mouse_clic[1].x = in.mouse_clic[1].y = -1;
  in.mouse_clic[2].x = in.mouse_clic[2].y = -1;

  while( in.mouse_clic[0].x==-1 && in.mouse_clic[1].x==-1 && in.mouse_clic[2].x==-1) {
    check_events();
    attendre(20);
  }
  
  if(in.mouse_clic[0].x != -1) {
    P.coord = in.mouse_clic[0];
    P.bouton = 0;
  } else if(in.mouse_clic[1].x != -1) {
    P.coord = in.mouse_clic[1];
    P.bouton = 1;
  } else {
    P.coord = in.mouse_clic[2];
    P.bouton = 2;
  }
  in.mouse_clic[0].x = in.mouse_clic[0].y = -1;
  in.mouse_clic[1].x = in.mouse_clic[1].y = -1;
  in.mouse_clic[2].x = in.mouse_clic[2].y = -1;
  
  return P;
}
/**/

POINT convert_coord1(POINT p) {
  p.x = p.x/SCALE; p.y = p.y/SCALE;
  return p;
}

POINT convert_coord2(POINT p) {
  p.x = p.x*SCALE; p.y = p.y*SCALE;
  return p;
}

CELLULE init_cell() {//BLURP
  CELLULE cellule;
  cellule.mine = 0;
  cellule.chiffre = 0;
  cellule.affichage = 0;
  return cellule;
}

void init_grille() {//BLURP
  int i,j;
  for(i=0;i<NB_X;i++)
    for(j=0;j<NB_Y;j++)
      grille.cell[i][j] = init_cell();
}

void generation_grille(POINT p) {
  POINT mine;
  int i=0, i1, j1;
  while(i<NB_MINE) {
    mine.x = entier_aleatoire(NB_X); mine.y = entier_aleatoire(NB_Y);
    if(grille.cell[mine.x][mine.y].mine==1) {}//BLURP
    else if(mine.x==p.x && mine.y==p.y && MODE>=2) {}//BLURP
    else if(mine.x >= p.x-1 && mine.x <= p.x+1 && mine.y >= p.y-1 && mine.y <= p.y+1 && MODE>=3) {}//BLURP
    else {
      grille.cell[mine.x][mine.y].mine = 1;
      for(i1 = mine.x - 1; i1 < mine.x + 2; i1++)
        for(j1 = mine.y - 1; j1 < mine.y + 2; j1++)
          if(i1 >= 0 && i1 < NB_X && j1 >= 0 && j1 < NB_Y)
            grille.cell[i1][j1].chiffre++;
      i++;
    }
  }
}

/* void affiche_console() {
  int i,j;
  for(j=0;j<NB_Y;j++) {
    for(i=0;i<NB_X;i++) {
      if(grille.cell[i][(NB_Y-1)-j].mine==1) {
        printf(" X");
      } else if(grille.cell[i][(NB_Y-1)-j].chiffre == 0) {
        printf(" .");
      } else {
        printf(" %d",grille.cell[i][(NB_Y-1)-j].chiffre);
      }
    }
    printf("\n");
  }
} */

void affiche_cell_active() {
  POINT p;
  int i,j;
  for(j=0;j<NB_Y;j++)
    for(i=0;i<NB_X;i++) {
      p.x = i*SCALE; p.y = j*SCALE;
      if(grille.cell[i][j].affichage == 0) //BLURP // 0=unknow
        dessine_image(images[14], p);
      else if(grille.cell[i][j].affichage == 4) //BLURP // 4=flag
        dessine_image(images[12], p);
      else if(grille.cell[i][j].affichage == 5) //BLURP // 5=hint
        dessine_image(images[13], p);
      else if(grille.cell[i][j].affichage == 6) //BLURP // 6=redmine
        dessine_image(images[10], p);
      else if(grille.cell[i][j].affichage == 7) //BLURP // 7=win
        dessine_image(images[17], p);
      else if(grille.cell[i][j].affichage == 8) //BLURP // 8=mine
        dessine_image(images[9], p);
      else if(grille.cell[i][j].affichage == 9) //BLURP // 9=error
        dessine_image(images[11], p);
      else if(grille.cell[i][j].affichage == 10) //BLURP // 10=safe
        dessine_image(images[15], p);
      else if(grille.cell[i][j].affichage == 11) //BLURP // 11=danger
        dessine_image(images[16], p);
      else /* 0=caché 
            * 1=affiché
            * 2=à calculer
            * 3=à reporter au prochain cycle
            */            
        dessine_image(images[grille.cell[i][j].chiffre], p);
    }
}

/* fonction générant trop d'iterations
void modif_grille(int i, int j) {
  if(grille.cell[i][j].affichage == 0) {
    grille.cell[i][j].affichage = 1;
    if(grille.cell[i][j].chiffre == 0 && grille.cell[i][j].mine == 0) {
      int i1, j1;
      for(i1 = i - 1; i1 < i + 2; i1++)
        for(j1 = j - 1; j1 < j + 2; j1++)
          if(i1 >= 0 && i1 < NB_X && j1 >= 0 && j1 < NB_Y)
            modif_grille(i1,j1);
    }
    affiche_cell_active();
    affiche_tout();
    attendre(15);
  }
}
/**/

// 0=caché 1=affiché 2=à calculer 3=à reporter au prochain cycle
void modif_grille(int i, int j) {
  if(grille.cell[i][j].affichage == 0 || grille.cell[i][j].affichage == 5 || grille.cell[i][j].affichage >= 10) {//BLURP 
    grille.cell[i][j].affichage = 2;//BLURP 
    if(grille.cell[i][j].mine == 1) {//BLURP 
      END = 2;
      grille.cell[i][j].affichage = 6;//BLURP 
    }
    int modif = 0;
    do {
      modif = 0;
      for(j=0;j<NB_Y;j++)
        for(i=0;i<NB_X;i++)
          if(grille.cell[i][j].affichage == 3)//BLURP 
            grille.cell[i][j].affichage = 2;//BLURP 
      for(j=0;j<NB_Y;j++)
        for(i=0;i<NB_X;i++)
          if(grille.cell[i][j].affichage == 2) {//BLURP 
            grille.cell[i][j].affichage = 1;//BLURP 
            NB_HIDDEN--;
            modif = 1;
            if(grille.cell[i][j].mine == 1)
              END = 2;
            else if(grille.cell[i][j].chiffre == 0) {
              int i1, j1;
              for(i1 = i - 1; i1 <= i + 1; i1++)
                for(j1 = j - 1; j1 <= j + 1; j1++)
                  if(i1 >= 0 && i1 < NB_X && j1 >= 0 && j1 < NB_Y && (grille.cell[i1][j1].affichage == 0 || grille.cell[i1][j1].affichage == 3 || grille.cell[i1][j1].affichage == 4))//BLURP 
                    grille.cell[i1][j1].affichage = 3;//BLURP 
            }
          }
      affiche_cell_active();
      affiche_tout();
      attendre(15);
    } while(modif==1);
  }
}

void modif_flag(int i, int j) {
  if(grille.cell[i][j].affichage == 0 || grille.cell[i][j].affichage == 10 ||grille.cell[i][j].affichage == 11) {//BLURP 
    NB_FLAG++;
    if(grille.cell[i][j].mine == 1)//BLURP 
      NB_FLAG_MINE++;
    grille.cell[i][j].affichage = 4;//BLURP 
  } else if(grille.cell[i][j].affichage == 4) {//BLURP 
    NB_FLAG--;
    if(grille.cell[i][j].mine == 1)//BLURP 
      NB_FLAG_MINE--;
    grille.cell[i][j].affichage = 5;//BLURP 
  } else if(grille.cell[i][j].affichage == 5) {//BLURP 
    grille.cell[i][j].affichage = 0;//BLURP 
  }
  affiche_cell_active();
}

void win() {
  int i,j;
  for(j=0;j<NB_Y;j++)
    for(i=0;i<NB_X;i++)
      if(grille.cell[i][j].mine == 1)//BLURP 
        grille.cell[i][j].affichage = 7;//BLURP 
  affiche_cell_active();
  affiche_tout();
  printf("WINNNNNERRRRR !!!!\n");//BLURP 
}

void lose() {
  int i,j;
  for(j=0;j<NB_Y;j++)
    for(i=0;i<NB_X;i++) {
      if(grille.cell[i][j].affichage == 4 && grille.cell[i][j].mine == 0)//BLURP 
        grille.cell[i][j].affichage = 9;//BLURP 
      else if(grille.cell[i][j].affichage != 4 && grille.cell[i][j].mine == 1 && grille.cell[i][j].affichage != 6)//BLURP 
        grille.cell[i][j].affichage = 8;//BLURP 
      else if(grille.cell[i][j].affichage == 4 && grille.cell[i][j].mine == 1)//BLURP 
        grille.cell[i][j].affichage = 7;//BLURP 
    }
  affiche_cell_active();
  affiche_tout();
  printf("LOOOOOOOSSSSERRRRRRR !!!!\n");//BLURP 
}

void reset_solver() {
  int i,j;
  for(i=0;i<NB_X;i++)
    for(j=0;j<NB_Y;j++)
      if(grille.cell[i][j].affichage == 10 || grille.cell[i][j].affichage == 11)//BLURP 
        grille.cell[i][j].affichage = 0;//BLURP 
}

void solver() {
  reset_solver();
  int i,j,i1,j1,hide;
  for(i=0;i<NB_X;i++)
    for(j=0;j<NB_Y;j++) {
      hide = 0;
      if(grille.cell[i][j].affichage == 1) {//BLURP*
        for(i1 = i - 1; i1 < i + 2; i1++)
          for(j1 = j - 1; j1 < j + 2; j1++)
            if(grille.cell[i1][j1].affichage == 0 || grille.cell[i1][j1].affichage > 3)//BLURP
              hide++;
        if(hide == grille.cell[i][j].chiffre)
          for(i1 = i - 1; i1 < i + 2; i1++)
            for(j1 = j - 1; j1 < j + 2; j1++)
              if(grille.cell[i1][j1].affichage == 0) //BLURP
                grille.cell[i1][j1].affichage = 11; //BLURP
        }
    }
  for(i=0;i<NB_X;i++)
    for(j=0;j<NB_Y;j++) {
      hide = 0;
      if(grille.cell[i][j].affichage == 1) {//BLURP*
        for(i1 = i - 1; i1 < i + 2; i1++)
          for(j1 = j - 1; j1 < j + 2; j1++)
            if(grille.cell[i1][j1].affichage == 11)//BLURP
              hide++;
        if(hide == grille.cell[i][j].chiffre)
          for(i1 = i - 1; i1 < i + 2; i1++)
            for(j1 = j - 1; j1 < j + 2; j1++)
              if(grille.cell[i1][j1].affichage == 0) //BLURP
                grille.cell[i1][j1].affichage = 10; //BLURP
        }
    }
  affiche_cell_active();
  affiche_tout();
  printf("TRICHEUR !!!!\n");
}

    /* Programme : */
int main(int argc,  char** argv) {
    /* Initialisation */
  init_input();
  // verification entrées utilisateur 
  printf("\nLa grille ferra : %d * %d avec %d mines avec le mode n_%d\n\n",NB_X,NB_Y,NB_MINE,MODE);
  init_grille();
  POINT fenetre; fenetre.x = (NB_X*SCALE); fenetre.y = (NB_Y*SCALE);//BLURP
  initialiser_fenetre(fenetre.x, fenetre.y, "Demineur");
  affiche_auto_off();
  /* Corps du programme */
  affiche_cell_active();
  int prem_clic_g = 0;
  SOURIS p;
  while(END == 0) {
    affiche_tout();
    p = attendre_multiclic();
    p.coord = convert_coord1(p.coord);
    if(prem_clic_g == 0 && p.bouton == 0) {
      generation_grille(p.coord);
      prem_clic_g++;
    }
    if(p.bouton == 0) {
      modif_grille(p.coord.x,p.coord.y);
    } else if(p.bouton == 1) {
      solver();
    } else if(p.bouton == 2) {
      modif_flag(p.coord.x,p.coord.y);
    }
    if((NB_FLAG == NB_FLAG_MINE && NB_FLAG_MINE == NB_MINE) || (NB_HIDDEN == NB_MINE))
      END = 1;
  }
  if(END == 1)
    win();
  else if(END == 2)
    lose();
  attendre_echap();
  quitter(0);
}