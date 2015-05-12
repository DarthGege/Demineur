#include "../toolbox.h"

// Une variable permettant de controler si le systeme sonore est initialisé
int __initialiser_sortie_audio = 0;

// Des variables permettant de noter les fichiers ouverts et bien les liberer...
MUSIQUE t_musiques[32];    int nb_musiques = 0;
SON t_sons[100];           int nb_sons = 0;


void initialiser_audio() {
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1 ) {
        printf("Erreur d'initialisation de la sortie sonore: %s\n", Mix_GetError());
        quitter(1);
    }
    
    Mix_AllocateChannels(16);
    __initialiser_sortie_audio = 32;
}


MUSIQUE charge_musique( char* fichier ) {
    // Si cela n'a pas encore été fait, on initialise le systeme sonore
    if ( __initialiser_sortie_audio == 0 )  initialiser_audio();

    MUSIQUE m = Mix_LoadMUS( fichier );
    if( m == NULL ) {
        printf("(*) Erreur lors du chargement du fichier de musique: %s\n", Mix_GetError());
        quitter(1);
    }

    t_musiques[ nb_musiques++ ] = m;
    
    return m;    
}




void joue_musique( MUSIQUE musique ) {
    if (musique)
        Mix_PlayMusic(musique, -1);
}


inline void pause_musique()   {   Mix_PauseMusic();    }
inline void reprise_musique() {   Mix_ResumeMusic();   }
inline void stop_musique()    {   Mix_HaltMusic();     }
    

SON charge_son( char* fichier ) {
    // Si cela n'a pas encore été fait, on initialise le systeme sonore
    if ( __initialiser_sortie_audio == 0 )  initialiser_audio();

    SON s = Mix_LoadWAV( fichier );
    if( s == NULL ) {
        printf("(*) Erreur lors du chargement du fichier son: %s\n", Mix_GetError());
        quitter(1);
    }

    t_sons[ nb_sons++ ] = s;
    
    return s;    
}



void joue_son( SON son ) {
    if ( son ) {
        if( Mix_PlayChannel(-1, son, 0)==-1)
            printf( "(*) Erreur lors de la lecture du son: %s\n",Mix_GetError() );
    }
}

void quitter_audio()
{
    // Liberation des musiques
    while( nb_musiques!=0 )
    {
        nb_musiques--;
        Mix_FreeMusic( t_musiques[ nb_musiques ] );
        t_musiques[ nb_musiques ] = NULL;
    }

    // Liberation des sons
    while( nb_sons!=0 )
    {
        nb_sons--;
        Mix_FreeChunk( t_sons[ nb_sons ] );
        t_sons[ nb_sons ] = NULL;
    }

    // Fermeture du systeme sonore
    if (__initialiser_sortie_audio!=0)
    {
        Mix_CloseAudio();
        __initialiser_sortie_audio = 0;
    }
}
