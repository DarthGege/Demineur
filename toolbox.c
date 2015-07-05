#include "toolbox.h"

/****************************************
 ****************************************
 ***   NE PAS MODIFIER CE FICHIER...  ***
 ****************************************
 ****************************************/


// Fonction renvoyant un reel dans ]0;1[
float reel_aleatoire() {
  static int init_alea = 1;
  if(init_alea) {
    srand(time(NULL));
  }
  init_alea = 0;
  return (float)rand() / (float)RAND_MAX;
}

// Fonction renvoyant un entier au hasard parmi
//      0, 1, 2, ... , N-1
int random_int(int N) {
  return (int)(N*reel_aleatoire());
}


// Renvoie l'entier tapé au clavier.
// Cette fonction est bloquante
int lire_entier_clavier() {
  int n;
  printf("Donner un entier : ");
  fflush(stdout);
  scanf("%d",&n);
  return n;
}


// Fonction prenant soin de liberer la memoire
//    ouverte par les differents modules
void quitter(int code_erreur) {
#ifdef WITH_AUDIO
  quitter_audio();
#endif
#ifdef WITH_NET
  quitter_net();
#endif
#ifdef WITH_GRAPHICS
  quitter_graph();
  SDL_Quit();
#endif
  exit(code_erreur);
}
