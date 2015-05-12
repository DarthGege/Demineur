
/****************************************
 ****************************************
 ***   NE PAS MODIFIER CE FICHIER...  ***
 ****************************************
 ****************************************/

#include <SDL_mixer.h>

// Deux type pour stocker les sons
typedef Mix_Music* MUSIQUE;
typedef Mix_Chunk* SON;


// Charger un fichier de musique (format WAV, MP3, OGG)
MUSIQUE charge_musique( char* fichier );

// Lecture du fichier (en boucle)
void joue_musique( MUSIQUE musique );

// Controle de la sortie musicale
void pause_musique();
void reprise_musique();
void stop_musique();

    
// Charger un fichier son (format WAV, MP3, OGG)
SON charge_son( char* fichier );

// Lecture du fichier (1 fois)
void joue_son( SON son );


// Met fin au module sonore
void quitter_audio();
