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
IMAGE images[17];

/* Déplacé dans la graphics.h
struct souris
{
	POINT coord;
	int bouton;
};
typedef struct souris SOURIS;
/**/

struct cellule
{
	int mine;
	int chiffre;
	int affichage;
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

void init_input()
{
	images[0] = charge_image("res/0.bmp");
	images[1] = charge_image("res/1.bmp");
	images[2] = charge_image("res/2.bmp");
	images[3] = charge_image("res/3.bmp");
	images[4] = charge_image("res/4.bmp");
	images[5] = charge_image("res/5.bmp");
	images[6] = charge_image("res/6.bmp");
	images[7] = charge_image("res/7.bmp");
	images[8] = charge_image("res/8.bmp");
	images[9] = charge_image("res/mine.bmp");
	images[10] = charge_image("res/redmine.bmp");
	images[11] = charge_image("res/error.bmp");
	images[12] = charge_image("res/flag.bmp");
	images[13] = charge_image("res/hint.bmp");
	images[14] = charge_image("res/unknow.bmp");
	images[15] = charge_image("res/safe.bmp");
	images[16] = charge_image("res/danger.bmp");
	
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
SOURIS attendre_multiclic()
{
	SOURIS P;
	
	// On force l'attente du prochain clic!
	in.mouse_clic[0].x = in.mouse_clic[0].y = -1;
	in.mouse_clic[1].x = in.mouse_clic[1].y = -1;
	in.mouse_clic[2].x = in.mouse_clic[2].y = -1;

	while( in.mouse_clic[0].x==-1 && in.mouse_clic[1].x==-1 && in.mouse_clic[2].x==-1)
	{
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

void affiche_cell_active(GRILLE grille)
{
	POINT p;
	int i,j;
	for(j=0;j<NB_Y;j++)
		for(i=0;i<NB_X;i++) {
			p.x = i*SCALE; p.y = ((NB_Y-1)-j)*SCALE;
			if(grille.cell[i][(NB_Y-1)-j].affichage == 0)
				dessine_image(images[14], p);
			else if(grille.cell[i][(NB_Y-1)-j].affichage == 4)
				dessine_image(images[12], p);
			else if(grille.cell[i][(NB_Y-1)-j].affichage == 5)
				dessine_image(images[13], p);
			else if(grille.cell[i][(NB_Y-1)-j].mine == 1)
				dessine_image(images[9], p);
			else
				dessine_image(images[grille.cell[i][(NB_Y-1)-j].chiffre], p);
		}
}

/* fonction générant trop d'iterations
GRILLE modif_grille(GRILLE grille, int i, int j)
{
	if(grille.cell[i][j].affichage == 0) {
		grille.cell[i][j].affichage = 1;
		if(grille.cell[i][j].chiffre == 0 && grille.cell[i][j].mine == 0) {
			int i1, j1;
			for(i1 = i - 1; i1 < i + 2; i1++)
				for(j1 = j - 1; j1 < j + 2; j1++)
					if(i1 >= 0 && i1 < NB_X && j1 >= 0 && j1 < NB_Y)
						grille = modif_grille(grille,i1,j1);
		}
	}
	return grille;
}
/**/

GRILLE modif_grille(GRILLE grille, int i, int j)
{
	if(grille.cell[i][j].affichage == 0 || grille.cell[i][j].affichage == 5) {
		grille.cell[i][j].affichage = 2;
		int modif = 0;
		do {
			modif = 0;
			for(j=0;j<NB_Y;j++)
				for(i=0;i<NB_X;i++)
					if(grille.cell[i][j].affichage == 3)
						grille.cell[i][j].affichage = 2;
			for(j=0;j<NB_Y;j++)
				for(i=0;i<NB_X;i++)
					if(grille.cell[i][j].affichage == 2) {
						grille.cell[i][j].affichage = 1;
						NB_HIDDEN--;
						modif = 1;
						if(grille.cell[i][j].mine == 1)
							END = 2;
						else if(grille.cell[i][j].chiffre == 0) {
							int i1, j1;
							for(i1 = i - 1; i1 <= i + 1; i1++)
								for(j1 = j - 1; j1 <= j + 1; j1++)
									if(i1 >= 0 && i1 < NB_X && j1 >= 0 && j1 < NB_Y && (grille.cell[i1][j1].affichage == 0 || grille.cell[i1][j1].affichage > 3))
										grille.cell[i1][j1].affichage = 3;
						}
					}
			affiche_cell_active(grille);
			affiche_tout();
			attendre(15);
		} while(modif==1);
	}    
	return grille;
}

GRILLE modif_flag(GRILLE grille, int i, int j)
{
	if(grille.cell[i][j].affichage == 0) {
		NB_FLAG++;
		if(grille.cell[i][j].mine == 1)
			NB_FLAG_MINE++;
		grille.cell[i][j].affichage = 4;
	} else if(grille.cell[i][j].affichage == 4) {
		NB_FLAG--;
		if(grille.cell[i][j].mine == 1)
			NB_FLAG_MINE--;
		grille.cell[i][j].affichage = 5;
	} else if(grille.cell[i][j].affichage == 5) {
		grille.cell[i][j].affichage = 0;
	}
	affiche_cell_active(grille);
	return grille;
}
void win()
{
}
void lose()
{
}

		/* Programme : */
int main(int argc,  char** argv)
{
		/* Initialisation */
	init_input();
	// verification entrées utilisateur 
	printf("\nLa grille ferra : %d * %d avec %d mines avec le mode n_%d\n\n",NB_X,NB_Y,NB_MINE,MODE);
	GRILLE grille = init_grille(); // grille grille... grille grille grille ? grille grille le grillon. grille ?
	POINT fenetre; fenetre.x = (NB_X*SCALE); fenetre.y = (NB_Y*SCALE);
	initialiser_fenetre(fenetre.x, fenetre.y, "Demineur");
	affiche_auto_off();
	/* Corps du programme */
	affiche_cell_active(grille);
	int prem_clic_g = 0;
	SOURIS p;
	while(END == 0) {
		affiche_tout();
		p = attendre_multiclic();
		p.coord = convert_coord1(p.coord);
		if(prem_clic_g == 0 && p.bouton == 0) {
			grille = generation_grille(grille,p.coord);
			prem_clic_g++;
		}
		if(p.bouton == 0) {
			grille = modif_grille(grille,p.coord.x,p.coord.y);
		} else if(p.bouton == 2) {
			grille = modif_flag(grille,p.coord.x,p.coord.y);
		}
		if((NB_FLAG == NB_FLAG_MINE && NB_FLAG_MINE == NB_MINE) || (NB_HIDDEN == NB_MINE))
			END = 1;
	}
	if(END == 1)
		win();
	if(END == 2)
		lose();
	attendre_echap();
	quitter(0);
}
