#include "toolbox.h"
#define SCALE 30
#define X_MAX 30
#define Y_MAX 30

int NB_X = X_MAX;
int NB_Y = Y_MAX;
int NB_MINE = (X_MAX*Y_MAX-9);
int MODE = 3;

IMAGE inactif = charge_image("inactif.bmp");
IMAGE mine = charge_image("mine.bmp");
IMAGE chiff[9] = {
	charge_image("0.bmp"),charge_image("1.bmp"),charge_image("2.bmp"),
	charge_image("3.bmp"),charge_image("4.bmp"),charge_image("5.bmp"),
	charge_image("6.bmp"),charge_image("7.bmp"),charge_image("8.bmp")
};

struct cellule
{
	int mine;
	int chiffre;
	int affichage;
	//negatif : 
  
};
typedef struct cellule CELLULE;

struct grille
{
	CELLULE cell[X_MAX][Y_MAX];
};
typedef struct grille GRILLE;

/* Fonctions : */

int input_entier() // Récupéré de toolbox.c pour y apporter des modifications d'affichage
{
	int n;
	fflush(stdout);
	scanf("%d",&n);
	return n;
}

POINT convert_coord1(POINT p)
{
	p.x = p.x/SCALE; p.y = p.y/SCALE;
	return p;
}

POINT convert_coord2(POINT p)
{
	p.x = p.x*SCALE; p.y = p.y*SCALE;
	return p;
}

CELLULE init_cell()
{
	CELLULE cellule;
	cellule.mine = 0;
	cellule.chiffre = 0;
	cellule.affichage = 0;
	return cellule;
}

GRILLE init_grille()
{
	GRILLE grille;
	int i,j;
	for(i=0;i<NB_X;i++)
		for(j=0;j<NB_Y;j++)
			grille.cell[i][j] = init_cell();
	return grille;
}

GRILLE generation_grille(GRILLE grille, POINT p)
{
	POINT mine;
	int i=0, i1, j1;
	while(i<NB_MINE) {
		mine.x = entier_aleatoire(NB_X); mine.y = entier_aleatoire(NB_Y);
		if(grille.cell[mine.x][mine.y].mine==1) {}
		else if(mine.x==p.x && mine.y==p.y && MODE>=2) {}
		else if(mine.x >= p.x-1 && mine.x <= p.x+1 && mine.y >= p.y-1 && mine.y <= p.y+1 && MODE>=3) {}
		else {
			grille.cell[mine.x][mine.y].mine = 1;
			for(i1 = mine.x - 1; i1 < mine.x + 2; i1++)
				for(j1 = mine.y - 1; j1 < mine.y + 2; j1++)
					if(i1 >= 0 && i1 < NB_X && j1 >= 0 && j1 < NB_Y)
						grille.cell[i1][j1].chiffre++;
			i++;
		}
	}
	return grille;
}

/* void affiche_console(GRILLE grille)
{
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

void affiche_cell_inactif(GRILLE grille)
{
	POINT p;
	int i,j;
	for(j=0;j<NB_Y;j++)
		for(i=0;i<NB_X;i++)
			if(grille.cell[i][(NB_Y-1)-j].affichage == 0) {
				p.x = i*SCALE; p.y = ((NB_Y-1)-j)*SCALE;
				dessine_image(inactif, p);
			}
	affiche_tout();
}

void affiche_cell_active(GRILLE grille)
{
	POINT p;
	int i,j;
	for(j=0;j<NB_Y;j++)
		for(i=0;i<NB_X;i++) {
			p.x = i*SCALE; p.y = ((NB_Y-1)-j)*SCALE;
			if(grille.cell[i][(NB_Y-1)-j].mine==1)
				dessine_image(mine, p);
			else
				dessine_image(chiff[grille.cell[i][(NB_Y-1)-j].chiffre], p);
		}
	affiche_tout();
}

GRILLE modif_grille(GRILLE grille, int i, int j)
{
	if(grille.cell[i][j].chiffre > 0) {
		grille.cell[i][j].affichage = 1
		return grille;
	}   
	else if(grille.cell[i][j].chiffre == 0) {
		grille.cell[i][j].affichage = 1
		grille = modif_grille(grille,i,j-1);
		grille = modif_grille(grille,i,j+1);
		grille = modif_grille(grille,i-1,j);
		grille = modif_grille(grille,i+1,j);
		return grille;
	}
	else if(grille.cell[i][j].mine == 1)
	{
			
	}
}

/* GRILLE modif_grille(GRILLE grille)
{
	int i,j;
	for(i=1; i<C-1; i++)
		for(j=1; j<L-1; j++)
			grille.cell[i][j] = modif_cell(t1,i,j);
	return grille;
} */

/* Programme : */
int main(int argc,  char** argv)
{
	/* Initialisation */
	do {
		printf("Grille de : (9-%d) X = ? ",X_MAX);
		NB_X = input_entier();
	} while(NB_X > X_MAX || NB_X < 9);

	do {
		printf("Grille de : (9-%d) Y = ? ",Y_MAX);
		NB_Y = input_entier();
	} while(NB_Y > Y_MAX || NB_Y < 9);
	int MINE_MAX = (NB_X-1)*(NB_Y-1);
	do {
		printf("Nombre de mine : (10-%d) ? ",MINE_MAX);
		NB_MINE = input_entier();
	} while(NB_MINE > MINE_MAX || NB_MINE < 10);
	do {
		printf("Mode de jeux : ? ");
		MODE = input_entier();
	} while(MODE > 3 || MODE < 0);
	// verification entrées utilisateur 
	printf("\nLa grille ferra : %d * %d avec %d mines avec le mode n_%d\n\n",NB_X,NB_Y,NB_MINE,MODE);
	GRILLE grille = init_grille(); // grille grille... grille grille grille ? grille grille le grillon. grille ?
	POINT fenetre; fenetre.x = (NB_X*SCALE); fenetre.y = (NB_Y*SCALE);
	initialiser_fenetre(fenetre.x, fenetre.y, "Demineur");
	affiche_auto_off();
	/* Corps du programme */
	affiche_cell_inactif(grille);
	POINT p;
	p = attendre_clic();
	p = convert_coord1(p);
	printf("\nPremier clic en : %d,%d\n",p.x,p.y);
	grille = generation_grille(grille,p);
	grille = modif_grille(grille,p.x,p.y)
	// affiche_console(grille);
	affiche_cell_active(grille);
	attendre_echap();
	quitter(0);
}
