
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
	// Déplacement minimal lorsque l'on utilise les flèches.
	#define MINDEP 1
	// Constantes booléennes
	#define TRUE 1
	#define True 1
	#define true 1
	#define FALSE 0
	#define False 0
	#define false 0


// ############################
// Fenêtres et gestion du temps
// ############################
	// Taille de l'display
	int WIDTH;
	int HEIGHT;

	// Initialisation de la fenêtre sur laquelle on dessine
	void initialiser_fenetre(int W, int H, char *titre);
	void initialiser_ecran(int W, int H, char *titre);

	// Affichage automatique ou manuel
	//    Si l'display est automatique, chaque objet dessiné est automatiquement affiché.
	//    Sinon, il faut explicitement appeler la fonction show_all() pour afficher les objets
	void affiche_auto_on();
	void disable_auto_redraw();
	void show_all();

	// Renvoie le nombre de millisecondes écoulées depuis le lancement du programme
	int chrono();
	
	// Attend le nombre de millisecondes passé en argument
	void attendre(int millisecondes);


// ###############################
// Couleurs et structures basiques
// ###############################
	// Définition du type POINT
	typedef struct point {int x,y;} POINT;
	typedef struct souris {POINT coord; int button;} MOUSE;

	// Définition du type COULEUR
	typedef Uint32 COULEUR;

	// Création de couleur
	// 		r, g, b et a dans l'intervalle 0 .. 255
	COULEUR couleur_RGB(int r, int g, int b);
	COULEUR couleur_RGBA(int r, int g, int b, int a);


// ###############################
// Interactivité
// ###############################
	// Renvoie le ou les flèches appuyées sous forme d'un déplacement en 
	//    x négatif = nombre d'appuis sur la flèche gauche
	//    x positif = nombre d'appuis sur la flèche droite
	//    y négatif = nombre d'appuis sur la flèche bas
	//    y positif = nombre d'appuis sur la flèche haut
	// Instruction non bloquante, si aucune flèche n'a été appuyée les champs valent 0
	POINT lit_fleches();

	// Attend que l'on tape Echap et quitte
	// Instruction bloquante
	void attendre_echap();

	// Renvoie vrai ou faux, selon que la touche du clavier
	// est enfoncée ou pas.
	//	b peut prendre les valeurs KB_A, KB_B, etc...
	//	(voir le fichier keycodes.h)
	BOOL touche_appuyee( int b );

	// Renvoie le nombre de fois (depuis le dernier appel)
	// où la touche a été pressée/relachée
	//	b peut prendre les valeurs KB_A, KB_B, etc...
	int touche_nb_appuis( int b );

	// Renvoie déplacement de souris
	// 	  Meme sémantique que lit_fleches()
	// Instruction non bloquante, si la souris n'a pas bougé, les champs valent 0.
	POINT lit_souris();

	// Attend que l'utilisateur clique avec le button gauche
	// Renvoie les coordonnées du point cliqué
	// Instruction bloquante
	POINT attendre_click();
	
	// Attend que l'utilisateur clique avec un button
	// Renvoie les coordonnées du point cliqué et le button appuyé
	// Instruction bloquante
	MOUSE wait_mouse_click();

	// Position de la souris
	// Instruction non bloquante
	POINT position_souris();

	// Renvoie vrai ou faux, selon que le button de la souris est appuyé ou pas
	//	b peut prendre les valeurs CLIC_GCH, CLIC_DRT ou CLIC_MIL
	//	suivant le button qu'on souhaite regarder	
	BOOL button_appuye( int b );

	// Position du dernier click sur le button précisé
	//  	b peut prendre les valeurs CLIC_GCH, CLIC_DRT ou CLIC_MIL
	//	    suivant le button qu'on souhaite regarder
	// Instruction non bloquante!
	// S'il n'y a pas eu de click, renvoie (-1;-1)
	POINT position_click( int b );

	// Activer / Desactiver l'display de la souris à l'écran
	void montre_souris();
	void cache_souris();

	// Empeche la souris de sortir dela fenetre
	//   Attention : desactive automatiquement l'display de la souris!
	void capture_souris();
	void libere_souris();


// ##############
// Dessin basique
// ##############
	// Remplissage de tout l'écran
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
	// 		Le texte est passé dans l'argument "a_ecrire" 
	// 		la police est celle définie par la constante POLICE_NAME 
	// 		la taille est passée en argument
	// 		l'argument p est le point en haut à gauche où le texte s'affiche
	// 		la COULEUR C passée en argument est la couleur d'display
	void dessine_texte(char *a_ecrire, int taille, POINT p, COULEUR C);

	// Affiche un entier
	// 		Meme sémantique que dessine_texte()
	void dessine_entier(int n, int taille, POINT p, COULEUR C);

	// Affiche dans la fenêtre graphique comme dans une fenêtre
	// shell mais sans déroulement. Commence en haut et se termine en bas.
	void ecrire_texte(char *a_ecrire);
	void ecrire_entier(int n);
	void ecrire_booleen(BOOL b);
	void retour_a_la_ligne() ;


// #####################
// Manipulation d'img
// #####################
    typedef struct {
        SDL_Surface* surface;   // L'image en mémoire, updateiable
        SDL_Texture* texture;   // L'image transformée pour le rendu
    }* IMAGE;

    // Charge une image au format BMP en mémoire
    IMAGE load_img( char* fichier );
    
    // Libere la mémoire utilisée par l'image
    void libere_image( IMAGE image );

    // Récupérer les dimensions d'une image
    int largeur_image( IMAGE image );
    int hauteur_image( IMAGE image );

    // Copie l'image dans la fenêtre d'display.
    //		Le coin inférieur gauche de l'image sera aux coordonnees du point P.
    void draw_img( IMAGE image, POINT P );

    // Copie l'image dans la fenêtre d'display.
    //		Le coin inférieur gauche de l'image sera aux coordonnees du point P.
    typedef enum { PAS_SYM = SDL_FLIP_NONE, SYM_H = SDL_FLIP_HORIZONTAL, SYM_V = SDL_FLIP_VERTICAL, SYM_HV = SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL } SYMETRIE;
    void draw_img_rzs( IMAGE im, POINT P, double angle, double k, SYMETRIE sym );

    // Récupére la couleur d'un pixel
    COULEUR lit_pixel_image( IMAGE image, POINT P );

    // Modifie la couleur d'un pixel
    void dessine_pixel_image( IMAGE image, POINT P, COULEUR c );

    // Permet d'obtenir une deuxième copie d'une image en mémoire
    IMAGE copie_image( IMAGE im );

    // Permet d'obtenir un image vide de dimension fixée
    IMAGE image_vide( int largeur, int hauteur );


// ######
// Divers
// ######
    // Helper: appelé automatiquement par quitter( error_code );
    void quitter_graph();


// #########
// Renvoie la distance entre deux points
// #########

	int distance(POINT P1, POINT P2);
