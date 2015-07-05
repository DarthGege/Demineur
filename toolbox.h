// ###############################################
// Franck QUESSETTE -- Franck.Quessette@uvsq.fr
// Version de novembre 2009
//
// Adapté pour l'ISN par E.Villani (2012-2013)
// ################################################


/****************************************
 ****************************************
 ***   NE PAS MODIFIER CE FICHIER...  ***
 ****************************************
 ****************************************/


#ifndef __TOOLBOX_H__
	// Creation d'un type booléen
	typedef int BOOL;

	// Constantes booléennes
	#define TRUE 1
	#define True 1
	#define true 1
	#define FALSE 0
	#define False 0
	#define false 0

	// Renvoie un float dans l'intervalle [0;1[
	float reel_aleatoire();

	// Renvoie un int dans l'intervalle [0..N[
	// càd N valeurs différentes de 0 à N-1
	int random_int(int N);

	// Renvoie l'entier tapé au clavier.
	// Cette fonction est bloquante
	int lire_entier_clavier();

	// Fonction prenant soin de liberer la memoire
	//    ouverte par les differents modules
	void quitter( int code_erreur )  __attribute__((noreturn));

	/* Librairies générales *************/
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <time.h>
	#include <math.h>
	/************************************/

	/* Librairies facultatives **********/
	#ifdef WITH_GRAPHICS
		#include "graphics/graphics.h"
	#endif

	#ifdef WITH_AUDIO
		#include "audio/audio.h"
	#endif

	#ifdef WITH_NET
		#include "net/net.h"
	#endif
	/************************************/

	#define __TOOLBOX_H__
#endif
