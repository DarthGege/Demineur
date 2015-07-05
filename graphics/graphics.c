/****************************************
 ****************************************
 ***   NE PAS MODIFIER CE FICHIER...  ***
 ****************************************
 ****************************************/

// ###############################################
// Franck QUESSETTE -- Franck.Quessette@uvsq.fr
// Version de novembre 2009
//
// Adapté pour l'ISN par E.Villani (2012-2014)
// ################################################

#include "../toolbox.h"

// #################################################
// Variables et constantes pour l'display de texte
// #################################################
	#define POLICE_NAME "verdana.ttf"
	TTF_Font *police[256];
	int verdana_ok = 0;	

// ############################
// Fenêtres et gestion du temps
// ############################
// Variables d'accès à l'display
SDL_Window   *SDL_screen;
SDL_Renderer *SDL_renderer;
int soft_renderer = 0;
	
// Pour ne pas oublier l'appel à initialiser_fenetre()
int __initialiser_fenetre_is_already_called = 0;

// Si SDL_AFFICHE_AUTO vaut 1, l'display
// est automatiquement fait pour chaque objet
// Sinon il faut le fait à la main
int SDL_AFFICHE_AUTO = 1;

// Les constantes de taille max de l'écran
#define MAX_WIDTH  2048
#define MAX_HEIGHT 2048

// Initialisation de la fenêtre sur laquelle on dessine
// W et H sont la largeur et la hauteur désirée.
void init_inputs();
void _set_window(int W, int H, char *titre, Uint32 flags){
	printf("= Toolbox v2.01 =\n");

	// Initialisation d'une taille raisonnable
	if ((W>10) && (W<MAX_WIDTH )) WIDTH  = W; else WIDTH  = 600;
	if ((H>10) && (H<MAX_HEIGHT)) HEIGHT = H; else HEIGHT = 400;
	
	// Initialisation de la SDL_surface
	SDL_screen = SDL_CreateWindow(titre, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, flags);
	
	if ( SDL_screen == NULL ) {
		fprintf(stderr, "Impossible de passer en %dx%d en 32 bits: %s\n", WIDTH, HEIGHT, SDL_GetError());
		quitter(1);
	}
	
	// Creation du renderer
	SDL_renderer = SDL_CreateRenderer( SDL_screen, -1, 0 );
	if ( SDL_renderer == NULL ) {
		fprintf(stderr, "Impossible de creer le renderer... %s\n", SDL_GetError());
		quitter(1);
	}
	SDL_SetRenderDrawBlendMode(SDL_renderer, SDL_BLENDMODE_BLEND );
    
    // On note si le renderer est software pour le rendu du texte
    SDL_RendererInfo info;
    SDL_GetRendererInfo(SDL_renderer, &info);
    soft_renderer = (info.flags & SDL_RENDERER_SOFTWARE);
    printf( (soft_renderer) ? "Rendu software, pas d'accélération graphique\n" : "Accélération graphique activée\n" );
		
	__initialiser_fenetre_is_already_called = 25;
	printf("Fenetre de %d x %d\n",WIDTH,HEIGHT);

	// Ouverture de la police
	TTF_Init();
	police[10] = TTF_OpenFont(POLICE_NAME, 10);
	if (police[10])	{
		verdana_ok=1;
		// printf("police %s OK : display graphique OK.\n",POLICE_NAME);
	}
	else {
		verdana_ok = 0;
		// printf("police %s absente : display dans la console.\n",POLICE_NAME);
	}
	
	init_inputs();
	
	// Remplit la fenêtre de noir
	remplir_ecran(noir);
	affiche_auto_on();
	show_all();
	
	fflush(stdout);
	
	atexit(SDL_Quit);
}

void resize_window(int W, int H) {
	SDL_Event event;

  if((W > 10) && (W < MAX_WIDTH)) WIDTH  = W;
  else WIDTH  = 600;
  
  if((H > 10) && (H < MAX_HEIGHT)) HEIGHT = H;
  else HEIGHT = 400;
  
  SDL_SetWindowSize(SDL_screen, W, H);
  
  SDL_Delay(10);
  
  remplir_ecran(noir);
  show_all();
}

void initialiser_fenetre(int W, int H, char *titre){
	_set_window(W,H,titre,0);
}

void initialiser_ecran(int W, int H, char *titre){
	_set_window(W,H,titre,SDL_WINDOW_FULLSCREEN_DESKTOP);

	//SDL_SetHint(SDL_HINT_RendER_SCALE_QUALITY, "nearest");	  // Pixel le plus proche
	//SDL_SetHint(SDL_HINT_RendER_SCALE_QUALITY, "linear");		  // Lissage lineaire	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");			  // Lissage bilineaire

	SDL_RenderSetLogicalSize( SDL_renderer, W, H);
}



// Affichage automatique ou manuel
void affiche_auto_on () { SDL_AFFICHE_AUTO = 1; }
void disable_auto_redraw() { SDL_AFFICHE_AUTO = 0; }

// Affiche tous les objets et vérifie que la fonction initialiser_fenetre 
// a été appelée précédemment et affiche un message d'erreur sinon.	
void check_events();
void show_all()
{	
	if (__initialiser_fenetre_is_already_called == 25) {
		/* SDL_Flip(SDL_screen);   */
		SDL_RenderPresent( SDL_renderer );		
	} else {
		 initialiser_fenetre(380,80,"Erreur...");
		 ecrire_texte("initialiser_fenetre() n'a pas été appelée.");
		 ecrire_texte("");
		 ecrire_texte("		   Cliquer pour terminer.");
		 fprintf(stderr,"initialiser_fenetre() n'a pas été appelée.\n");
		 fprintf(stderr,"Cliquer pour terminer.\n");
		 attendre_click();
		 quitter(1);
	}
	
	check_events();
}

// Chronomètre élémentaire, précis à la milliseconde.
inline int chrono() { return SDL_GetTicks(); }

// Attend le nombre de millisecondes passé en argument
void attendre(int millisecondes) { SDL_Delay(millisecondes); }


// ###############################
// Couleurs et structures basiques
// ###############################
Uint32	   renderer_color;

// Pas de canal alpha => canal alpha à 255
inline COULEUR couleur_RGB(int r, int g, int b)
{   return couleur_RGBA( r, g, b, 255);  }

COULEUR couleur_RGBA(int r, int g, int b, int a)
{
	renderer_color = ((a%256)<<24) + ((r%256)<<16) + ((g%256)<<8) + (b%256);
	SDL_SetRenderDrawColor(SDL_renderer, r, g, b, a);	
	return renderer_color;
}

void SetRenderColor( COULEUR couleur ){
	// On evite de changer la couleur inutilement
	if( couleur==renderer_color) return;

	// On calcule les composantes RGB
	Uint8 a = (couleur & 0xFF000000) >> 24;
	Uint8 r = (couleur & 0x00FF0000) >> 16;
	Uint8 g = (couleur & 0x0000FF00) >> 8;
	Uint8 b = (couleur & 0x000000FF);
	
	// On selectionne la couleur
	SDL_SetRenderDrawColor(SDL_renderer, r, g, b, a);
	
	// Et on sauvegarde :)
	renderer_color = couleur;
}
	


// ###############################
// Interactivité
// ###############################
typedef struct {
	unsigned char key_pressed[ SDL_NUM_SCANCODES ];		// Tableau des touches appuyées
	unsigned char key_count[ SDL_NUM_SCANCODES ];		// Tableau des nb d'appuis sur les touches

	POINT mouse, mouse_rel, mouse_click[3] ;		// Positions de la souris: absolue, relative, 
												//		derniers clicks gauche, droite et milieu
	unsigned char mouse_pressed[ 3 ];		// Tableau des buttons de souris appuyés
	unsigned char mouse_count[ 3 ];		// Tableau des nombres de clicks
} INPUT;
INPUT in;

void init_inputs(){
	// Initialement toutes les positions souris / touches sont inconnues!
	in.mouse.x = in.mouse.y = -1;	
	in.mouse_rel.x = in.mouse_rel.y = 0;  
	
	int i;
	for(i=0;i<SDL_NUM_SCANCODES;i++) in.key_pressed[i]=in.key_count[i]=0;
	for(i=0;i<3;i++){
		in.mouse_click[i].x = in.mouse_click[i].y = -1;
		in.mouse_pressed[i]=in.mouse_count[i]=0;
	}
}

void check_events(){
	SDL_Event event;
	
	SDL_PumpEvents();
	int sc,b;
	
	while(SDL_PollEvent(&event))
	{
		switch( event.type ) {
			/* Si l'utilisateur a demandé à fermer la fenêtre, on quitte */
			case SDL_QUIT:
				quitter(0);
			
			/* l'utilisateur a appuyé sur une touche */
			case SDL_KEYDOWN:
				sc=event.key.keysym.scancode;
				if( sc==KB_ECHAP ) quitter(0);  // Touche Echap, on quitte
				in.key_pressed[sc]=1;		// Sinon on note la touche
				break;
				
			/* l'utilisateur a relaché sur une touche */
			case SDL_KEYUP:
				sc=event.key.keysym.scancode;
				in.key_count[sc]++;	   // On note le relachement de la touche
				in.key_pressed[sc]=0;  
				break;
				
			case SDL_MOUSEMOTION:
				// Position absolue de la souris
				in.mouse.x = event.motion.x;
				in.mouse.y = (HEIGHT-1-event.motion.y);
				// Mouvement relatif
				in.mouse_rel.x += event.motion.xrel;
				in.mouse_rel.y += -event.motion.yrel;			
				break;

			case SDL_MOUSEBUTTONDOWN:
				b = event.button.button - 1; 				// 0 = gauche, 1 = milieu, 2 = droite
				if(0<=b && b<=2) in.mouse_pressed[b]=1;		// On note que le button est pressé
				break;
			
			case SDL_MOUSEBUTTONUP:
				b = event.button.button - 1;				// 0 = gauche, 1 = milieu, 2 = droite
				if(0<=b && b<=2) {
					in.mouse_click[b].x = event.button.x;
					in.mouse_click[b].y = HEIGHT-1-event.button.y;
					in.mouse_count[b]++;
					in.mouse_pressed[b]=0;
				}
				break;

			default:
				break;
		}
	}
}


// Renvoie le ou les flèches appuyées sous forme d'un déplacement en 
//	x négatif = nombre d'appuis sur la flèche gauche
//	x positif = nombre d'appuis sur la flèche droite
//	y négatif = nombre d'appuis sur la flèche bas
//	y positif = nombre d'appuis sur la flèche haut
// Instruction non bloquante, si aucune flèche n'a été appuyée les champs valent 0
POINT lit_fleches()
{
	check_events();

	POINT dep = {0,0};
	// dep.x = dep.y = 0;
	
	// Touches pressées
	if (in.key_pressed[KB_GAUCHE])	dep.x -= MINDEP;
	if (in.key_pressed[KB_DROITE])	dep.x += MINDEP;
	if (in.key_pressed[KB_HAUT])	dep.y += MINDEP;
	if (in.key_pressed[KB_BAS])		dep.y -= MINDEP;

	// Touches relachées
	dep.x -= MINDEP*in.key_count[KB_GAUCHE];	in.key_count[KB_GAUCHE] = 0;
	dep.x += MINDEP*in.key_count[KB_DROITE];	in.key_count[KB_DROITE] = 0;
	dep.y += MINDEP*in.key_count[KB_HAUT];		in.key_count[KB_HAUT] = 0;
	dep.y -= MINDEP*in.key_count[KB_BAS];		in.key_count[KB_BAS] = 0;
		
	return dep;
}

// Renvoie déplacement de souris
//	Meme sémantique que lit_fleches()
// Instruction non bloquante, si la souris n'a pas bougé, les champs valent 0.
POINT lit_souris()
{
	POINT dep = in.mouse_rel;
	
	in.mouse_rel.x = 0;
	in.mouse_rel.y = 0;
	
	// On filtre les mouvements trop importants (sorties de fenetres?)
	if (abs(dep.x)>150 || abs(dep.y)>150)
		dep.x = dep.y = 0;
	
	return dep;
}

// Attend que l'on tape Echap et quitte
// Instruction bloquante
void attendre_echap()
{
	POINT p;
	p.x = WIDTH/2 - 170;
	p.y = 25;
	//dessine_texte("Appuyer sur Echap pour terminer",20,p,gris);
	show_all();
	
	while ( 1 )
	{
			check_events();
			attendre(20);
	}
}

BOOL touche_appuyee( int b ) {
	if (0<=b && b<SDL_NUM_SCANCODES) return in.key_pressed[b];
	return 0;
}

int touche_nb_appuis( int b )  {
	if (b<0 || b>=SDL_NUM_SCANCODES) return 0;
	int r = in.key_count[b];
	in.key_count[b] = 0;
	return r;
}

// Attend que l'utilisateur clique avec le button gauche
// Renvoie les coordonnées du point cliqué
// Instruction bloquante
POINT attendre_click()
{
	POINT P;
	
	// On force l'attente du prochain click!
	in.mouse_click[0].x = in.mouse_click[0].y = -1;

	while( in.mouse_click[0].x==-1 )
	{
		check_events();
		attendre(20);
	}
	
	P = in.mouse_click[0];
	in.mouse_click[0].x = in.mouse_click[0].y = -1; 
	
	printf("Clic en %4d %4d\n",P.x,P.y);
	fflush(stdout);
	return P;
}

MOUSE wait_mouse_click()
{
	MOUSE P;
	
	// On force l'attente du prochain click!
	in.mouse_click[0].x = in.mouse_click[0].y = -1;
	in.mouse_click[1].x = in.mouse_click[1].y = -1;
	in.mouse_click[2].x = in.mouse_click[2].y = -1;
	while(in.mouse_click[0].x==-1 && in.mouse_click[1].x==-1 && in.mouse_click[2].x==-1)
	{
		check_events();
		attendre(20);
	}
	
	if(in.mouse_click[0].x != -1) {
		P.coord = in.mouse_click[0];
		P.button = 0;
	} else if(in.mouse_click[1].x != -1) {
		P.coord = in.mouse_click[1];
		P.button = 1;
	} else {
		P.coord = in.mouse_click[2];
		P.button = 2;
	}
	in.mouse_click[0].x = in.mouse_click[0].y = -1;
	in.mouse_click[1].x = in.mouse_click[1].y = -1;
	in.mouse_click[2].x = in.mouse_click[2].y = -1;
	return P;
}

POINT position_souris(){
	return in.mouse;
}

BOOL button_appuye( int b ){
	if (b<0 || b>2) return 0;
	return in.mouse_pressed[b];
}

POINT position_click( int b ){
	POINT P={-1,-1};
	if (b>=0 && b<=2) {
		P = in.mouse_click[b];
		in.mouse_click[b].x = in.mouse_click[b].y = -1;
	}
	return P;
}

inline void cache_souris() { SDL_ShowCursor(0); }
inline void montre_souris() { SDL_ShowCursor(1); }
inline void capture_souris() { SDL_SetRelativeMouseMode(1); }
inline void libere_souris() { SDL_SetRelativeMouseMode(0); }

// ##############
// Dessin basique
// ##############
	
// Remplissage de tout l'écran
void remplir_ecran(COULEUR couleur)
{		
	SetRenderColor( couleur );
	SDL_RenderClear(SDL_renderer);
	
	if (SDL_AFFICHE_AUTO) show_all();
}

// Affichage un pixel
void _pixel(int x, int y, COULEUR couleur)
{
	SetRenderColor( couleur );
	SDL_RenderDrawPoint( SDL_renderer, x, HEIGHT-y-1 );

	if (SDL_AFFICHE_AUTO) show_all();
}
inline void dessine_pixel(POINT p, COULEUR couleur) { _pixel(p.x, p.y, couleur); }

// Dessine un segment
void _ligne(int x1, int y1, int x2, int y2, COULEUR couleur)
{
	SetRenderColor( couleur );
	SDL_RenderDrawLine( SDL_renderer, x1, HEIGHT-y1-1, x2, HEIGHT-y2-1);
	
	if (SDL_AFFICHE_AUTO) show_all();
}
inline void dessine_ligne(POINT p1, POINT p2, COULEUR couleur) { _ligne( p1.x, p1.y, p2.x, p2.y, couleur ); }

// Dessine un rectangle non rempli
// Les deux points sont deux points quelconques 
// non adjacents du rectangle
void _rectangle(int x1, int y1, int x2, int y2, COULEUR couleur) {
	SDL_Rect rect;
	
	if(x1<x2) {	 rect.x = x1;	rect.w = x2-x1+1;	 }
	else	  {	 rect.x = x2;	rect.w = x1-x2+1;	 }
	
	if(y2<y1) {	 rect.y = HEIGHT - y1 - 1;	   rect.h = y1-y2+1;	 }
	else	  {	 rect.y = HEIGHT - y2 - 1;	   rect.h = y2-y1+1;	 }
	
	SetRenderColor( couleur );
	SDL_RenderDrawRect( SDL_renderer, &rect );
	
	if (SDL_AFFICHE_AUTO) show_all();
}
inline void dessine_rectangle(POINT p1, POINT p2, COULEUR couleur) { _rectangle( p1.x, p1.y, p2.x, p2.y, couleur ); }

// Dessine un rectangle rempli
// Les deux point sont deux points quelconques 
// non adjacents du rectangle
void _rectangle_plein(int x1, int y1, int x2, int y2, COULEUR couleur)
{
	SDL_Rect rect;
		
	if(x1<x2) {	 rect.x = x1;	rect.w = x2-x1+1;	 }
	else	  {	 rect.x = x2;	rect.w = x1-x2+1;	 }
	
	if(y2<y1) {	 rect.y = HEIGHT - y1 - 1;	   rect.h = y1-y2+1;	 }
	else	  {	 rect.y = HEIGHT - y2 - 1;	   rect.h = y2-y1+1;	 }
	
	SetRenderColor( couleur );
	SDL_RenderFillRect( SDL_renderer, &rect );
	
	if (SDL_AFFICHE_AUTO) show_all();
}
inline void dessine_rectangle_plein(POINT p1, POINT p2, COULEUR couleur) { _rectangle_plein( p1.x, p1.y, p2.x, p2.y, couleur ); }

// Dessine un cercle non rempli
void _cercle(int cx, int cy, int rayon, COULEUR couleur)
{
	int x,y,d;

	x = 0;
	y = rayon;
	d = rayon-1;

	while(x<=y)
	{
		_pixel( cx+x, cy+y, couleur );
		_pixel( cx-x, cy+y, couleur );
		_pixel( cx+x, cy-y, couleur );
		_pixel( cx-x, cy-y, couleur );
		_pixel( cx+y, cy+x, couleur );
		_pixel( cx-y, cy+x, couleur );
		_pixel( cx+y, cy-x, couleur );
		_pixel( cx-y, cy-x, couleur );
		if( d>2*(x-1) )
			{	d -= 2*x;		x++;	}
		else if( d<= 2*(rayon-y) )
			{	d += 2*y-1;		y--;	}
		else
			{	d += 2*(y-x-1);	y--;	x++;	}
	}

	if (SDL_AFFICHE_AUTO) show_all();	
}
inline void dessine_cercle(POINT centre, int rayon, COULEUR couleur) { _cercle(centre.x,centre.y,rayon,couleur); }

// Dessine un cercle rempli
void _cercle_plein(int cx, int cy, int rayon, COULEUR couleur)
{
	int x,y,d;
	int x1,x2,yh;

	x = 0;
	y = rayon;
	d = rayon -1;

	SetRenderColor( couleur );
	
	while(x<=y)
	{   // Algo => http://fr.wikipedia.org/wiki/Algorithme_de_tracé_de_cercle_d%27Andres
		//   P => A, B, ou C
		//		dans le cas A on ne trace pas l'original, mais les symétriques
		//		dans le cas B on trace pas les symétriques, mais l'original
		//		dans le cas C on trace les deux! 
		
		if( d>2*(x-1) )				 // CAS A
		{
			// Tracé des "symétriques"
			x1 = cx-y;	 x2 = cx+y;
			yh = HEIGHT-(cy+x)-1;	   SDL_RenderDrawLine( SDL_renderer, x1,yh, x2,yh );
			if (x!=0)
			{ yh = HEIGHT-(cy-x)-1;	 SDL_RenderDrawLine( SDL_renderer, x1,yh, x2,yh ); }
			
			d -= 2*x;
			x++;
		}
		else if( d<= 2*(rayon-y) )	  // CAS B
		{
			// Tracé des "originaux"
			x1 = cx-x;	 x2 = cx+x;
			yh = HEIGHT-(cy+y)-1;	   SDL_RenderDrawLine( SDL_renderer, x1,yh, x2,yh );
			yh = HEIGHT-(cy-y)-1;	   SDL_RenderDrawLine( SDL_renderer, x1,yh, x2,yh );
			d += 2*y-1;
			y--;
		}
		else							// CAS C
		{	// On trace tout
			if(x!=y)
			{
				x1 = cx-y;	 x2 = cx+y;
				yh = HEIGHT-(cy+x)-1;   SDL_RenderDrawLine( SDL_renderer, x1,yh, x2,yh );
				yh = HEIGHT-(cy-x)-1;   SDL_RenderDrawLine( SDL_renderer, x1,yh, x2,yh );
			}
			x1 = cx-x;	 x2 = cx+x;
			yh = HEIGHT-(cy+y)-1;	   SDL_RenderDrawLine( SDL_renderer, x1,yh, x2,yh );
			yh = HEIGHT-(cy-y)-1;	   SDL_RenderDrawLine( SDL_renderer, x1,yh, x2,yh );		
			d += 2*(y-x-1);
			y--;
			x++;
		}
	}

	if (SDL_AFFICHE_AUTO) show_all();
}

inline void dessine_cercle_plein(POINT centre, int rayon, COULEUR couleur) { _cercle_plein(centre.x, centre.y, rayon, couleur); }

// Dessine un triangle
void dessine_triangle(POINT p1, POINT p2, POINT p3, COULEUR couleur)
{
	dessine_ligne(p1,p2,couleur);
	dessine_ligne(p2,p3,couleur);
	dessine_ligne(p3,p1,couleur);
}

// Dessine un triangle rempli
	// Fonction annexe qui calcule le min de 3 valeurs
int min3(int a, int b, int c)
{
	if ( (a<b) && (a<c) ) return a;
	if ( (b<a) && (b<c) ) return b;
	return c;
}

	// Fonction annexe qui calcule le max de 3 valeurs
int max3(int a, int b, int c)
{
	if ( (a>b) && (a>c) ) return a;
	if ( (b>a) && (b>c) ) return b;
	return c;
}

void dessine_triangle_plein(POINT p1, POINT p2, POINT p3, COULEUR couleur)
{
	float a12, b12, a23, b23, a31, b31;
	float s1, s2, s3;
	// La droite passant par les point pi et pj
	// a pour équation : y = aij x  +  bij
	a12 = (p1.y-p2.y)/(float)(p1.x-p2.x);   b12 = p1.y - a12*p1.x;
	a23 = (p2.y-p3.y)/(float)(p2.x-p3.x);   b23 = p2.y - a23*p2.x;
	a31 = (p3.y-p1.y)/(float)(p3.x-p1.x);   b31 = p3.y - a31*p3.x;
	// Le signe de sk détermine de quel coté pk est de la droite [pi,pj]
	s3 = p3.y - (a12*p3.x + b12);
	s1 = p1.y - (a23*p1.x + b23);
	s2 = p2.y - (a31*p2.x + b31);
	
	int minx, maxx, miny, maxy;
	minx = min3(p1.x,p2.x,p3.x); maxx = max3(p1.x,p2.x,p3.x);
	miny = min3(p1.y,p2.y,p3.y); maxy = max3(p1.y,p2.y,p3.y);
	
	int i,j; 
	int ok;
	for (i=minx;i<maxx;i++)
		for (j=miny;j<maxy;j++)
		{
			ok = 1;
			// On vérifie que le point (i,j) est du bon coté
			// des 3 droites du triangle
			if (s3 * (j - (a12*i + b12)) < 0) ok = 0;
			if (s1 * (j - (a23*i + b23)) < 0) ok = 0;
			if (s2 * (j - (a31*i + b31)) < 0) ok = 0;
			if (ok) _pixel(i,j,couleur);
		}
}


// #####
// Texte
// #####

// Affiche du texte avec
//	  Le texte est passé dans l'argument "a_ecrire" 
//	  la police est celle définie par la constante POLICE_NAME 
//	  la taille est passée en argument
//	  l'argument p est le point en haut à gauche où le texte s'affiche
//	  la COULEUR C passée en argument est la couleur d'display
void dessine_texte(char *a_ecrire, int taille, POINT p, COULEUR C)
{
	int i;
	SDL_Color couleur;
	SDL_Surface *surf = NULL;
	
	static int premiere_fois = 1;
	static TTF_Font *police[256];
	TTF_Font *pol;
	
	// Initialisation de la TTF
	if (premiere_fois) {
							for (i=0;i<256;i++) police[i] = NULL;
							premiere_fois = 0;	}
	// Ouverture de la police
	if (taille>=256)
		pol = TTF_OpenFont(POLICE_NAME, taille);
	else {
		if (police[taille]==NULL) police[taille] = TTF_OpenFont(POLICE_NAME, taille);
		pol = police[taille];
	}
	
	couleur.a = (C & 0xFF000000) >> 24;
	couleur.r = (C & 0x00FF0000) >> 16;
	couleur.g = (C & 0x0000FF00) >> 8;
	couleur.b = (C & 0x000000FF);

	// Ecriture du texte dans la SDL_Surface "texte" en mode Blended (optimal)
	if (pol) {
		surf = ( soft_renderer )
            ? TTF_RenderText_Solid(pol, a_ecrire, couleur)      // Renderer software = rendu solide
            : TTF_RenderText_Blended(pol, a_ecrire, couleur);   // Renderer hardware = rendu alpha
	}
		
	if (surf)  {
		// Conversion en texture
		SDL_Texture *texture = SDL_CreateTextureFromSurface( SDL_renderer, surf);
		
		SDL_Rect dest;
		dest.x = p.x;
		dest.y = HEIGHT - p.y;
		dest.w = surf->w;
		dest.h = surf->h;
		
		SDL_FreeSurface(surf);
		
		SDL_RenderCopy(SDL_renderer, texture,					NULL, &dest);
	  	if (SDL_AFFICHE_AUTO) show_all();
				
		SDL_DestroyTexture( texture );		
	} else
		printf("%s\n",a_ecrire);
}

// Affiche un entier
//	  Meme sémantique que dessine_texte()
void dessine_entier(int n,  int taille, POINT p, COULEUR C)
{
	char s[32];
	sprintf(s,"%d",n);
	dessine_texte(s,taille,p,C);
}

// Affiche dans la fenêtre graphique comme dans une fenêtre
// shell mais sans déroulement. Commence en haut et se termine en bas.
void ecrire_texte(char *a_ecrire)
{
	static char s[256];
	static int fin = 0;
	static int premier = 1;
	static POINT position;
	
	if (premier) {position.x = 10; position.y = HEIGHT; premier = 0;}

	if (verdana_ok)
	{
		if (a_ecrire)
		{
			if (fin+strlen(a_ecrire) > 256) {fprintf(stderr,"Chaine de caractère trop longue\n"); return;}
				strcat(s+fin,a_ecrire);
				dessine_texte(s,20,position,blanc);
			fin += strlen(a_ecrire);
			s[fin] = '\0';
		}
		else 
		{
			position.y -= 20;
			fin = 0;
			s[fin] = '\0';
		}
	}
	else { if (a_ecrire) {printf("%s",a_ecrire); fflush(stdout);} else printf("\n"); }
}

void ecrire_entier(int n)
{
	char s[32];
	sprintf(s,"%d",n);
	ecrire_texte(s);
}

void ecrire_booleen(BOOL b) { if (b) ecrire_texte("Vrai"); else ecrire_texte("Faux"); }

void retour_a_la_ligne() { ecrire_texte(NULL); }



// #####################
// Manipulation d'img
// #####################
// Tableau pour suivre l'allocation d'img
IMAGE tab_img[1000];
int   nb_imgs = 0;  

// Chargement d'une image
IMAGE load_img( char* fichier ) {
	IMAGE im = malloc( sizeof(IMAGE) );
 
	// Chargement du fichier
	im->surface = SDL_LoadBMP( fichier );
	
	// Erreur (mauvais format? Fichier non trouvé?)
	if ( im->surface == NULL) {
		printf( "(*) Erreur lors du chargement de l'image '%s': %s\n",fichier , SDL_GetError() );
		quitter(1);
	}
	
	im->texture = NULL;

	// On note la reference...
	tab_img[ nb_imgs++ ] = im;
	
	return im;
}

// Liberation de la memoire
void libere_image( IMAGE im ) {
	if (im->surface != NULL )
		SDL_FreeSurface(im->surface);
	if (im->texture != NULL )
		SDL_DestroyTexture( im->texture );		
	im->surface = NULL;
	im->texture = NULL;
}

// Renvoir la largeur et la hauteur de l'image
inline int largeur_image( IMAGE im )  { return im->surface->w; }
inline int hauteur_image( IMAGE im )  { return im->surface->h; }

// Afficher une image en entier aux coordonnees du point P
void draw_img( IMAGE im, POINT P ) {
	if (im->texture == NULL )
	{   // Il est necessaire de regenerer la "texture" pour rendu
		im->texture = SDL_CreateTextureFromSurface( SDL_renderer, im->surface );	 
	}
	
	SDL_Rect dest;	
	dest.x = P.x;
	dest.y =  HEIGHT - (im->surface->h + P.y);
	dest.w = im->surface->w;
	dest.h = im->surface->h;

	SDL_RenderCopy(SDL_renderer, im->texture,					NULL, &dest);
	
	if (SDL_AFFICHE_AUTO) show_all();
}

void draw_img_rzs( IMAGE im, POINT P, double angle, double k, SYMETRIE sym) {
	if (im->texture == NULL )
	{   // Il est necessaire de regenerer la "texture" pour rendu
		im->texture = SDL_CreateTextureFromSurface( SDL_renderer, im->surface );	 
	}
	
	SDL_Rect dest;	
	dest.x = P.x;
	dest.y = HEIGHT - (im->surface->h*k + P.y);
	dest.w = im->surface->w * k;
	dest.h = im->surface->h * k;
    
	SDL_RenderCopyEx(SDL_renderer, im->texture, NULL, &dest, -angle, NULL, sym );
	
	if (SDL_AFFICHE_AUTO) show_all();	
}

// Récupérer la couleur d'un pixel sur une image
COULEUR lit_pixel_image( IMAGE im, POINT P )
{
	if ( (P.x < im->surface->w) && (P.x >= 0) && (P.y < im->surface->h) && (P.y >= 0) )
	{
		int X = P.x;
		int Y = (im->surface->h-P.y-1);
		Uint32* ptr = im->surface->pixels;
		
		return ptr[Y * im->surface->w + X];
	} else
		return 0;
}

// Changer la couleur d'un pixel sur une image
void dessine_pixel_image( IMAGE im, POINT P, COULEUR c )
{
	if ( (P.x < im->surface->w) && (P.x >= 0) && (P.y < im->surface->h) && (P.y >= 0) )
	{
		int X = P.x;
		int Y = (im->surface->h-P.y-1);
		Uint32* ptr = im->surface->pixels;		
		ptr[Y * im->surface->w + X] = c;

		if (im->texture != NULL )
			SDL_DestroyTexture( im->texture );

		im->texture = NULL;
	}
}

// Permet d'obtenir une deuxième copie d'une image en mémoire
IMAGE copie_image( IMAGE im ){
	IMAGE dest = malloc( sizeof(IMAGE) );
	
	dest->surface = SDL_ConvertSurface( im->surface, im->surface->format,  im->surface->flags );
	dest->texture = NULL;

	// On note la reference...
	tab_img[ nb_imgs++ ] = dest;
	
	return dest;
}

// Permet d'obtenir un image vide de dimension fixée
IMAGE image_vide( int largeur, int hauteur ) {
	IMAGE im = malloc( sizeof(IMAGE) );

	// Creation de la surface remplie de "noir transparent"
	im->surface = SDL_CreateRGBSurface(0, largeur, hauteur, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	SDL_FillRect(im->surface,					NULL, 0x00000000);

	// Texture initialement vide
	im->texture = NULL;

	// On note la reference...
	tab_img[ nb_imgs++ ] = im;

	return im;
}

// ######
// Divers
// ######
// Helper: appelé automatiquement par quitter( error_code );
void quitter_graph() {
	// Liberation de toutes les img ouvertes
	while(nb_imgs!=0)
	{
		nb_imgs--;
		IMAGE im = tab_img[ nb_imgs ];

		// Si l'image en fin de tableau n'est pas libérée,
		//	appeler la fonction libere_image		
		if ( im->surface!=NULL || im->texture!=NULL )
		{
			libere_image( im );
		}
	}
}

// #########
// Renvoie la distance entre deux points
// #########
	
int distance(POINT P1, POINT P2)
	{
	int d;
	d = (P1.x-P2.x)*(P1.x-P2.x) + (P1.y-P2.y)*(P1.y-P2.y);
	return (int)(sqrt(d));
	}





/* Nom des scancodes *********************************************************************************************/
char* KB_CODES[SDL_NUM_SCANCODES] = {
							NULL,					NULL,					NULL,					NULL,
	"KB_A",					"KB_B",					"KB_C",					"KB_D",					"KB_E",
	"KB_F",					"KB_G",					"KB_H",					"KB_I",					"KB_J",
	"KB_K",					"KB_L",					"KB_M",					"KB_N",					"KB_O",
	"KB_P",					"KB_Q",					"KB_R",					"KB_S",					"KB_T",
	"KB_U",					"KB_V",					"KB_W",					"KB_X",					"KB_Y",
	"KB_Z",					"KB_1",					"KB_2",					"KB_3",					"KB_4",
	"KB_5",					"KB_6",					"KB_7",					"KB_8",					"KB_9",
	"KB_0",					"KB_ENTREE",			"KB_ECHAP",				"KB_RET_ARRIERE",		"KB_TAB",
	"KB_ESPACE",			"KB_MOINS",				"KB_EGALE",				"KB_CROCHET_GCH",		"KB_CROCHET_DRT",
	"KB_BACKSLASH",			"KB_DIESE",				"KB_POINT_VIRGULE",		"KB_APOSTROPHE",		"KB_GRAVE",
	"KB_VIRGULE",			"KB_POINT",				"KB_DIVISE",			"KB_VERROU_MAJ",		"KB_F1",
	"KB_F2",				"KB_F3",				"KB_F4",				"KB_F5",				"KB_F6",
	"KB_F7",				"KB_F8",				"KB_F9",				"KB_F10",				"KB_F11",
	"KB_F12",				"KB_IMPRIM_ECRAN",		"KB_VERROU_DEFIL",		"KB_PAUSE",				"KB_INSERE",
	"KB_DEBUT",				"KB_PAGE_HAUT",			"KB_SUPPRIME",			"KB_FIN",				"KB_PAGE_BAS",
	"KB_GAUCHE",			"KB_DROITE",			"KB_BAS",				"KB_HAUT",				"KB_VERROU_NUM",
	"KB_DIVISE_NUM",		"KB_ETOILE_NUM",		"KB_MOINS_NUM",			"KB_PLUS_NUM",			"KB_ENTREE_NUM",
	"KB_1_NUM",				"KB_2_NUM",				"KB_3_NUM",				"KB_4_NUM",				"KB_5_NUM",
	"KB_6_NUM",				"KB_7_NUM",				"KB_8_NUM",				"KB_9_NUM",				"KB_0_NUM",
	"KB_POINT_NUM",			NULL,					"KB_APPLI",				"KB_ALLUMAGE",			"KB_EGALE_NUM",
	"KB_F13",				"KB_F14",				"KB_F15",				"KB_F16",				"KB_F17",
	"KB_F18",				"KB_F19",				"KB_F20",				"KB_F21",				"KB_F22",
	"KB_F23",				"KB_F24",				"KB_EXEC",				"KB_AIDE",				"KB_MENU",
	"KB_SELECT",			"KB_STOP",				"KB_ENCORE",			"KB_ANNULE",			"KB_COUPE",
	"KB_COPY",				"KB_COLLE",				"KB_CHERCHE",			"KB_MUET",				"KB_VOLUME_HAUT",
	"KB_VOLUME_BAS",		NULL,					NULL,					NULL,					"KB_VIRGULE_NUM",
	"KB_AS400_NUM",			NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					NULL,					NULL,					"KB_EFFACE_ALT",
	"KB_SysReq",			"KB_ANNULER",			"KB_EFFACER",			"KB_PRECEDENT",			"KB_RETOUR",
	"KB_SEPARATEUR",		"KB_HORS",				"KB_OPER",				"KB_NETTOIE_ENCORE",	"KB_CRSEL",
	"KB_EXSEL",				NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					"KB_00_NUM",			"KB_000_NUM",			"KB_SEP_MILLIER",
	"KB_SEP_DIZAINE",		"KB_MONNAIE",			"KB_SOUS_MONNAIE",		"KB_PARENT_GCH_NUM",	"KB_PARENT_DRT_NUM",
	"KB_ACCOLADE_GCH_NUM",	"KB_ACCOLADE_DRT_NUM",	"KB_TAB_NUM",			"KB_RET_ARRIERE_NUM",	"KB_A_NUM",
	"KB_B_NUM",				"KB_C_NUM",				"KB_D_NUM",				"KB_E_NUM",				"KB_F_NUM",
	"KB_XOR_NUM",			"KB_CHAPEAU_NUM",		"KB_POURCENT_NUM",		"KB_INFERIEUR_NUM",		"KB_SUPERIEUR_NUM",
	"KB_AND_NUM",			"KB_DOUBLE_AND_NUM", 	"KB_OR_NUM",			"KB_DOUBLE_OR_NUM",		"KB_DEUX_POINTS_NUM",
	"KB_DIESE_NUM",			"KB_ESPACE_NUM",		"KB_ARROBAS_NUM",		"KB_EXCLAM_NUM",		"KB_MemStore_NUM",
	"KB_MemRecall_NUM",		"KB_MemClear_NUM",		"KB_MemAdd_NUM",		"KB_MemSubtract_NUM",	"KB_MemMultiply_NUM",
	"KB_MemDivide_NUM",		"KB_PLUS_MOINS_NUM",	"KB_Clear_NUM",			"KB_ClearEntry_NUM",	"KB_Binary_NUM",
	"KB_Octal_NUM",			"KB_Decimal_NUM",		"KB_Hexadecimal_NUM",	NULL,					NULL,
	"KB_CTRL_GCH",			"KB_MAJ_GCH",			"KB_ALT_GCH",			"KB_WIN_GCH",			"KB_CTRL_DRT",
	"KB_MAJ_DRT",			"KB_ALT_DRT",			"KB_WIN_DRT",			NULL,					NULL,
	NULL,					NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					NULL,					NULL,					NULL,
	NULL,					NULL,					NULL,					"KB_ModeSwitch",		"KB_AudioNext",
	"KB_AudioPrev",			"KB_AudioStop",			"KB_AudioPlay",			"KB_AudioMute",			"KB_MediaSelect",
	"KB_WWW",				"KB_Mail",				"KB_Calculator",		"KB_Computer",			"KB_AC_Search",
	"KB_AC_Home",			"KB_AC_Back",			"KB_AC_Forward",		"KB_AC_Stop",			"KB_AC_Refresh",
	"KB_AC_Bookmarks",		"KB_BrightnessDown",	"KB_BrightnessUp",		"KB_DisplaySwitch",		"KB_KBDIllumToggle",
	"KB_KBDIllumDown",		"KB_KBDIllumUp",		"KB_Eject",				"KB_Sleep"
};
