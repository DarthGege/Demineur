// Codes de touches
enum
{
	KB_INCONNUE = 0,

	KB_A = 4,
	KB_B = 5,
	KB_C = 6,
	KB_D = 7,
	KB_E = 8,
	KB_F = 9,
	KB_G = 10,
	KB_H = 11,
	KB_I = 12,
	KB_J = 13,
	KB_K = 14,
	KB_L = 15,
	KB_M = 16,
	KB_N = 17,
	KB_O = 18,
	KB_P = 19,
	KB_Q = 20,
	KB_R = 21,
	KB_S = 22,
	KB_T = 23,
	KB_U = 24,
	KB_V = 25,
	KB_W = 26,
	KB_X = 27,
	KB_Y = 28,
	KB_Z = 29,

	KB_1 = 30,
	KB_2 = 31,
	KB_3 = 32,
	KB_4 = 33,
	KB_5 = 34,
	KB_6 = 35,
	KB_7 = 36,
	KB_8 = 37,
	KB_9 = 38,
	KB_0 = 39,

	KB_ENTREE = 40,
	KB_ECHAP = 41,
	KB_RET_ARRIERE = 42,
	KB_TAB = 43,
	KB_ESPACE = 44,

	KB_MOINS = 45,
	KB_EGALE = 46,
	KB_CROCHET_GCH = 47,
	KB_CROCHET_DRT = 48,
	KB_BACKSLASH = 49,
	KB_DIESE = 50,
	KB_POINT_VIRGULE = 51,
	KB_APOSTROPHE = 52,
	KB_GRAVE = 53,
	KB_VIRGULE = 54,
	KB_POINT = 55,
	KB_DIVISE = 56,
	KB_VERROU_MAJ = 57,
	KB_F1 = 58,
	KB_F2 = 59,
	KB_F3 = 60,
	KB_F4 = 61,
	KB_F5 = 62,
	KB_F6 = 63,
	KB_F7 = 64,
	KB_F8 = 65,
	KB_F9 = 66,
	KB_F10 = 67,
	KB_F11 = 68,
	KB_F12 = 69,
	KB_IMPRIM_ECRAN = 70,
	KB_VERROU_DEFIL = 71,
	KB_PAUSE = 72,
	KB_INSERE = 73,
	KB_DEBUT = 74,
	KB_PAGE_HAUT = 75,
	KB_SUPPRIME = 76,
	KB_FIN = 77,
	KB_PAGE_BAS = 78,
	KB_DROITE = 79,
	KB_GAUCHE = 80,
	KB_BAS = 81,
	KB_HAUT = 82,
	KB_VERROU_NUM = 83,
	KB_DIVISE_NUM = 84,
	KB_ETOILE_NUM = 85,
	KB_MOINS_NUM = 86,
	KB_PLUS_NUM = 87,
	KB_ENTREE_NUM = 88,
	KB_1_NUM = 89,
	KB_2_NUM = 90,
	KB_3_NUM = 91,
	KB_4_NUM = 92,
	KB_5_NUM = 93,
	KB_6_NUM = 94,
	KB_7_NUM = 95,
	KB_8_NUM = 96,
	KB_9_NUM = 97,
	KB_0_NUM = 98,
	KB_POINT_NUM = 99,

	KB_00_NUM = 176,
	KB_000_NUM = 177,
	KB_SEP_MILLIER = 178,
	KB_SEP_DIZAINE = 179,
	KB_MONNAIE = 180,
	KB_SOUS_MONNAIE = 181,
	KB_PARENT_GCH_NUM = 182,
	KB_PARENT_DRT_NUM = 183,
	KB_ACCOLADE_GCH_NUM = 184,
	KB_ACCOLADE_DRT_NUM = 185,
	KB_TAB_NUM = 186,
	KB_RET_ARRIERE_NUM = 187,
	KB_A_NUM = 188,
	KB_B_NUM = 189,
	KB_C_NUM = 190,
	KB_D_NUM = 191,
	KB_E_NUM = 192,
	KB_F_NUM = 193,
	KB_XOR_NUM = 194,
	KB_CHAPEAU_NUM = 195,
	KB_POURCENT_NUM = 196,
	KB_INFERIEUR_NUM = 197,
	KB_SUPERIEUR_NUM = 198,
	KB_AND_NUM = 199,
	KB_DOUBLE_AND_NUM = 200,
	KB_OR_NUM = 201,
	KB_DOUBLE_OR_NUM = 202,
	KB_DEUX_POINTS_NUM = 203,
	KB_DIESE_NUM = 204,
	KB_ESPACE_NUM = 205,
	KB_ARROBAS_NUM = 206,
	KB_EXCLAM_NUM = 207,
	KB_MemStore_NUM = 208,
	KB_MemRecall_NUM = 209,
	KB_MemClear_NUM = 210,
	KB_MemAdd_NUM = 211,
	KB_MemSubtract_NUM = 212,
	KB_MemMultiply_NUM = 213,
	KB_MemDivide_NUM = 214,
	KB_PLUS_MOINS_NUM = 215,
	KB_Clear_NUM = 216,
	KB_ClearEntry_NUM = 217,
	KB_Binary_NUM = 218,
	KB_Octal_NUM = 219,
	KB_Decimal_NUM = 220,
	KB_Hexadecimal_NUM = 221,

	KB_CTRL_GCH = 224,
	KB_MAJ_GCH = 225,
	KB_ALT_GCH = 226,   /**< alt, option */
	KB_WIN_GCH = 227,   /**< windows, command (apple), meta */
	KB_CTRL_DRT = 228,
	KB_MAJ_DRT = 229,
	KB_ALT_DRT = 230,   /**< alt gr, option */
	KB_WIN_DRT = 231,   /**< windows, command (apple), meta */

	KB_ModeSwitch = 257,
	KB_AudioNext= 258,
	KB_AudioPrev = 259,
	KB_AudioStop = 260,
	KB_AudioPlay = 261,
	KB_AudioMute = 262,
	KB_MediaSelect = 263,
	KB_WWW = 264,
	KB_Mail = 265,
	KB_Calculator = 266,
	KB_Computer = 267,
	KB_AC_Search = 268,
	KB_AC_Home = 269,
	KB_AC_Back = 270,
	KB_AC_Forward = 271,
	KB_AC_Stop = 272,
	KB_AC_Refresh = 273,
	KB_AC_Bookmarks = 274,

	KB_BrightnessDown = 275,
	KB_BrightnessUp = 276,
	KB_DisplaySwitch = 277,
	KB_KBDIllumToggle = 278,
	KB_KBDIllumDown = 279,
	KB_KBDIllumUp = 280,
	KB_Eject = 281,
	KB_Sleep = 282,

	KB_APP1 = 283,
	KB_APP2 = 284,
};

extern char* KB_CODES[SDL_NUM_SCANCODES];

// Boutons de la souris
enum{ CLIC_GCH, CLIC_MIL, CLIC_DRT };
