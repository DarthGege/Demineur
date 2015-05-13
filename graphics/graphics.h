
/****************************************
 ****************************************
 ***   NE PAS MODIFIER CE FICHIER...  ***
 ****************************************
 ****************************************/


#include <SDL.h>
#include <SDL_ttf.h>
#include "couleurs.h"
#include "keycodes.h"

// CONSTANTES
	// D�placement minimal lorsque l'on utilise les fl�ches.
	#define MINDEP 1
	// Constantes bool�ennes
	#define TRUE 1
	#define True 1
	#define true 1
	#define FALSE 0
	#define False 0
	#define false 0


// ############################
// Fen�tres et gestion du temps
// ############################
	// Taille de l'affichage
	int WIDTH;
	int HEIGHT;

	// Initialisation de la fen�tre sur laquelle on dessine
	void initialiser_fenetre(int W, int H, char *titre);
	void initialiser_ecran(int W, int H, char *titre);

	// Affichage automatique ou manuel
	//    Si l'affichage est automatique, chaque objet dessin� est automatiquement affich�.
	//    Sinon, il faut explicitement appeler la fonction affiche_tout() pour afficher les objets
	void affiche_auto_on();
	void affiche_auto_off();
	void affiche_tout();

	// Renvoie le nombre de millisecondes �coul�es depuis le lancement du programme
	int chrono();
	
	// Attend le nombre de millisecondes pass� en argument
	void attendre(int millisecondes);


// ###############################
// Couleurs et structures basiques
// ###############################
	// D�finition du type POINT
	typedef struct point {int x,y;} POINT;
	typedef struct souris {POINT coord; int bouton;} SOURIS;

	// D�finition du type COULEUR
	typedef Uint32 COULEUR;

	// Cr�ation de couleur
	// 		r, g, b et a dans l'intervalle 0 .. 255
	COULEUR couleur_RGB(int r, int g, int b);
	COULEUR couleur_RGBA(int r, int g, int b, int a);


// ###############################
// Interactivit�
// ###############################
	// Renvoie le ou les fl�ches appuy�es sous forme d'un d�placement en 
	//    x n�gatif = nombre d'appuis sur la fl�che gauche
	//    x positif = nombre d'appuis sur la fl�che droite
	//    y n�gatif = nombre d'appuis sur la fl�che bas
	//    y positif = nombre d'appuis sur la fl�che haut
	// Instruction non bloquante, si aucune fl�che n'a �t� appuy�e les champs valent 0
	POINT lit_fleches();

	// Attend que l'on tape Echap et quitte
	// Instruction bloquante
	void attendre_echap();

	// Renvoie vrai ou faux, selon que la touche du clavier
	// est enfonc�e ou pas.
	//	b peut prendre les valeurs KB_A, KB_B, etc...
	//	(voir le fichier keycodes.h)
	BOOL touche_appuyee( int b );

	// Renvoie le nombre de fois (depuis le dernier appel)
	// o� la touche a �t� press�e/relach�e
	//	b peut prendre les valeurs KB_A, KB_B, etc...
	int touche_nb_appuis( int b );

	// Renvoie d�placement de souris
	// 	  Meme s�mantique que lit_fleches()
	// Instruction non bloquante, si la souris n'a pas boug�, les champs valent 0.
	POINT lit_souris();

	// Attend que l'utilisateur clique avec le bouton gauche
	// Renvoie les coordonn�es du point cliqu�
	// Instruction bloquante
	POINT attendre_clic();
	
	// Attend que l'utilisateur clique avec un bouton
	// Renvoie les coordonn�es du point cliqu� et le bouton appuy�
	// Instruction bloquante
	SOURIS attendre_multiclic();

	// Position de la souris
	// Instruction non bloquante
	POINT position_souris();

	// Renvoie vrai ou faux, selon que le bouton de la souris est appuy� ou pas
	//	b peut prendre les valeurs CLIC_GCH, CLIC_DRT ou CLIC_MIL
	//	suivant le bouton qu'on souhaite regarder	
	BOOL bouton_appuye( int b );

	// Position du dernier clic sur le bouton pr�cis�
	//  	b peut prendre les valeurs CLIC_GCH, CLIC_DRT ou CLIC_MIL
	//	    suivant le bouton qu'on souhaite regarder
	// Instruction non bloquante!
	// S'il n'y a pas eu de clic, renvoie (-1;-1)
	POINT position_clic( int b );

	// Activer / Desactiver l'affichage de la souris � l'�cran
	void montre_souris();
	void cache_souris();

	// Empeche la souris de sortir dela fenetre
	//   Attention : desactive automatiquement l'affichage de la souris!
	void capture_souris();
	void libere_souris();


// ##############
// Dessin basique
// ##############
	// Remplissage de tout l'�cran
	void remplir_ecran(COULEUR couleur);

	// Dessine un pixel
	void dessine_pixel(POINT p, COULEUR couleur);
	void _pixel(int x, int y, COULEUR couleur);

	// Dessine un segment
	void dessine_ligne(POINT p1, POINT p2, COULEUR couleur);
	void _ligne(int x1, int y1, int x2, int y2, COULEUR couleur);

	// Dessine un rectangle non rempli
	// Les deux points sont deux points quelconques 
	// non adjacents du rectangle
	void dessine_rectangle(POINT p1, POINT p2, COULEUR couleur);
	void _rectangle(int x1, int y1, int x2, int y2, COULEUR couleur);

	// Dessine un rectangle rempli
	// Les deux point sont deux points quelconques 
	// non adjacents du rectangle
	void dessine_rectangle_plein(POINT p1, POINT p2, COULEUR couleur);
	void _rectangle_plein(int x1, int y1, int x2, int y2, COULEUR couleur);

	// Dessine un cercle non rempli
	void _cercle(int cx, int cy, int rayon, COULEUR couleur);
	void dessine_cercle(POINT centre, int rayon, COULEUR couleur);

	// Dessine un cercle rempli
	void _cercle_plein(int cx, int cy, int rayon, COULEUR couleur);
	void dessine_cercle_plein(POINT centre, int rayon, COULEUR couleur);

	// Dessine un triangle
	void dessine_triangle(POINT p1, POINT p2, POINT p3, COULEUR couleur);

	// Dessine un triangle rempli
	void dessine_triangle_plein(POINT p1, POINT p2, POINT p3, COULEUR couleur);
    

// #####
// Texte
// #####
	// Affiche du texte avec
	// 		Le texte est pass� dans l'argument "a_ecrire" 
	// 		la police est celle d�finie par la constante POLICE_NAME 
	// 		la taille est pass�e en argument
	// 		l'argument p est le point en haut � gauche o� le texte s'affiche
	// 		la COULEUR C pass�e en argument est la couleur d'affichage
	void dessine_texte(char *a_ecrire, int taille, POINT p, COULEUR C);

	// Affiche un entier
	// 		Meme s�mantique que dessine_texte()
	void dessine_entier(int n, int taille, POINT p, COULEUR C);

	// Affiche dans la fen�tre graphique comme dans une fen�tre
	// shell mais sans d�roulement. Commence en haut et se termine en bas.
	void ecrire_texte(char *a_ecrire);
	void ecrire_entier(int n);
	void ecrire_booleen(BOOL b);
	void retour_a_la_ligne() ;


// #####################
// Manipulation d'images
// #####################
    typedef struct {
        SDL_Surface* surface;   // L'image en m�moire, modifiable
        SDL_Texture* texture;   // L'image transform�e pour le rendu
    }* IMAGE;

    // Charge une image au format BMP en m�moire
    IMAGE charge_image( char* fichier );
    
    // Libere la m�moire utilis�e par l'image
    void libere_image( IMAGE image );

    // R�cup�rer les dimensions d'une image
    int largeur_image( IMAGE image );
    int hauteur_image( IMAGE image );

    // Copie l'image dans la fen�tre d'affichage.
    //		Le coin inf�rieur gauche de l'image sera aux coordonnees du point P.
    void dessine_image( IMAGE image, POINT P );

    // Copie l'image dans la fen�tre d'affichage.
    //		Le coin inf�rieur gauche de l'image sera aux coordonnees du point P.
    typedef enum { PAS_SYM = SDL_FLIP_NONE, SYM_H = SDL_FLIP_HORIZONTAL, SYM_V = SDL_FLIP_VERTICAL, SYM_HV = SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL } SYMETRIE;
    void dessine_image_rzs( IMAGE im, POINT P, double angle, double k, SYMETRIE sym );

    // R�cup�re la couleur d'un pixel
    COULEUR lit_pixel_image( IMAGE image, POINT P );

    // Modifie la couleur d'un pixel
    void dessine_pixel_image( IMAGE image, POINT P, COULEUR c );

    // Permet d'obtenir une deuxi�me copie d'une image en m�moire
    IMAGE copie_image( IMAGE im );

    // Permet d'obtenir un image vide de dimension fix�e
    IMAGE image_vide( int largeur, int hauteur );


// ######
// Divers
// ######
    // Helper: appel� automatiquement par quitter( error_code );
    void quitter_graph();


// #########
// Renvoie la distance entre deux points
// #########

	int distance(POINT P1, POINT P2);
