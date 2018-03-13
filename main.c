///
///  Projet ARENE - Version 1.9 - original / 11 mars 2018
///  Equipe "Les demons de midi"
///     Julie Neige
///     Sandra Keutcha
///     Josia Douniama Okana
///     François Nguyen
///     Alia ali Cherif
///     Charlotte Bourgogne
///

/// en Mode DEMO
/// déplacement joueur 1 : touches Up, Down, Left et Right
/// déplacement joueur 2 : touches E, D, S et F

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

const int SCREEN_BPP = 32;
int fenetre_largeur ;
int fenetre_hauteur ;
#define taille_bloc  32 //en pixels
#define nb_blocs_hauteur  16
#define nb_blocs_largeur  33              ///************* minimum 33 !
#define FILES {"labyrinthe_carte_1.txt","labyrinthe_multi_joueurs_1.txt"}

/// variables Mode DEMO
///
int joueur1_score = 0;
int joueur2_score = 0;
int mode_demo;
int num_gagnant;

/// variables Mode IA
#define max_joueurs 25
#define nb_joueurs  6
#define max_tours 8
#define max_coups_ia 6
int scores       [max_joueurs];
int pos_actuel_i [max_joueurs];
int pos_actuel_j [max_joueurs];
int joueur_actif [max_joueurs];
char code_joueur [max_joueurs];
char code_equipe [max_joueurs];
int continuer = 0;

///
/// Arborescence des appels
///
void Menu();
     void labyrinthe_demo (SDL_Surface *ecran);
     void labyrinthe_multi (SDL_Surface *ecran);
           void effacer_carte (char carte[nb_blocs_hauteur][nb_blocs_largeur]) ;
           void remplir_carte_avec_fichier (char carte[nb_blocs_hauteur][nb_blocs_largeur]);
                void afficher_carte (char carte[nb_blocs_hauteur][nb_blocs_largeur]);

            void deplacement_joueur_demo (char carte[nb_blocs_hauteur][nb_blocs_largeur], SDL_Surface*ecran,
                         int joueur1_actuel_i ,int joueur1_actuel_j,
                         int joueur2_actuel_i, int joueur2_actuel_j,
                         SDL_Surface*bonhomme_1, SDL_Surface*bonhomme_2 ,
                         SDL_Surface *blanc);

            void deplacement_joueur_multi (char action_joueur, int num_joueur,
                         char carte[nb_blocs_hauteur][nb_blocs_largeur], SDL_Surface*ecran,
                         int pos_actuel_i [max_joueurs] ,int pos_actuel_j [max_joueurs],
                         SDL_Surface*bonhomme_1, SDL_Surface*bonhomme_2 , SDL_Surface *blanc);

      void afficher_scores_demo (SDL_Surface *ecran, int gagnant);
      void afficher_scores (SDL_Surface *ecran, int gagnant, int scores[max_joueurs]);

///
/// Les IA du Projet n°4 renverront un entier : int ia_0 (carte, hauteur, largeur)
///
/// Ici, les IA de test envoient, à chaque tours, la valeur du type d'action : U(p), D(own), L(fet) ou R(ight)
///
char ia_0 (char carte[nb_blocs_hauteur][nb_blocs_largeur], int nb_tours);
char ia_1 (char carte[nb_blocs_hauteur][nb_blocs_largeur], int nb_tours);
char ia_2 (char carte[nb_blocs_hauteur][nb_blocs_largeur], int nb_tours);
char ia_3 (char carte[nb_blocs_hauteur][nb_blocs_largeur], int nb_tours);


int main(int argc, char *argv[])
{
    fenetre_hauteur = taille_bloc * nb_blocs_hauteur ;         /// en pixels
    fenetre_largeur = taille_bloc * (nb_blocs_largeur + 7);    /// prévoir de la place pour les scores
    SDL_Surface *ecran = NULL, *Image_Fond = NULL;          /// Le pointeur qui va stocker la surface de l'écran
    SDL_Init(SDL_INIT_VIDEO);                               /// initialisation de la sdl

    Menu(ecran);

    SDL_FreeSurface(Image_Fond);                            /// On libère la surface */
    SDL_Quit();                                             /// arret de la sdl et des sous systeme SDL_INIT_VIDEO ...

    return EXIT_SUCCESS;
}

void Menu(SDL_Surface *ecran, SDL_Surface *Image_Fond)
{
    int continuer = 1;

    SDL_Event event;
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;

    while (continuer)
    {
    ecran = SDL_SetVideoMode(960, 700, 32, SDL_HWSURFACE);  /// SDL_RESIZABLE | SDL_FULLSCREEN | SDL_DOUBLEBUF
                                                        /// affiche d'une fenetre avec ses dimensions et nombre de couleur qui peut avoir
    if (ecran == NULL)
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    num_gagnant = 0;
    SDL_WM_SetCaption("Le labyrinthe de l'enfer !", NULL);

    /* Chargement d'une image Bitmap dans une surface */
    Image_Fond = SDL_LoadBMP("menu_bmp.bmp");
    SDL_BlitSurface(Image_Fond, NULL, ecran, &positionFond);
    SDL_Flip (ecran);
    SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT :
                continuer = 0;
                break;
            case SDL_KEYDOWN :
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE :
                    continuer = 0 ;
                    break;
                case SDLK_1 :
                    SDL_WM_SetCaption("Le labyrinthe de l'enfer !    ***  Version DEMO  ***     ", NULL);
                    ecran = SDL_SetVideoMode(fenetre_largeur, fenetre_hauteur, 32, SDL_HWSURFACE);
                    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
                    mode_demo = 1;
                    Labyrinthe_demo(ecran);
                    afficher_scores_demo (ecran, num_gagnant);
                    SDL_WaitEvent(&event);
                    break;
                case SDLK_2 :
                    SDL_WM_SetCaption("Le labyrinthe de l'enfer !    ***  Version IA  ***     ", NULL);
                    ecran = SDL_SetVideoMode(fenetre_largeur, fenetre_hauteur, 32, SDL_HWSURFACE);
                    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
                    mode_demo = 0;
                    labyrinthe_multi(ecran);
                    afficher_scores (ecran, num_gagnant, scores);    // revoir SDF_TTF
                    SDL_WaitEvent(&event);
                    break;
                case SDLK_3 :
                    Image_Fond = SDL_LoadBMP("regles.bmp");
                    SDL_BlitSurface(Image_Fond, NULL, ecran, &positionFond);
                    SDL_WaitEvent(&event);
                    SDL_Flip(ecran);
                    SDL_WaitEvent(&event);
                    break;
                 case SDLK_4 :
                    Image_Fond = SDL_LoadBMP("credits.bmp");
                    SDL_BlitSurface(Image_Fond, NULL, ecran, &positionFond);
                    SDL_WaitEvent(&event);
                    SDL_Flip(ecran);
                    SDL_WaitEvent(&event);
                    break;
                default :
                    break;
                }
        }
    }
}

void Labyrinthe_demo (SDL_Surface*ecran)
{
   char carte [nb_blocs_hauteur][nb_blocs_largeur] ;
   int i , j ;

   SDL_EnableKeyRepeat(10, 50);   /// répétition automatique des touches

   effacer_carte (carte);
   mode_demo = 1;
   remplir_carte_avec_fichier(carte);

   SDL_Surface *mur = NULL ;
   SDL_Surface *piece = NULL ;
   SDL_Surface *bonhomme_1 = NULL ;
   SDL_Surface *bonhomme_2 = NULL ;
   SDL_Surface *blanc = NULL ;
   SDL_Surface *piege = NULL ;

   mur = SDL_LoadBMP("mur3.bmp");
   piece = SDL_LoadBMP ("piece_bmp2.bmp");
   bonhomme_1 = SDL_LoadBMP ("bonhomme_1_down_bmp.bmp");
   bonhomme_2 = SDL_LoadBMP ("bonhomme_2_down_bmp.bmp");
   blanc = SDL_LoadBMP ("Blanc1.bmp");
   piege = SDL_LoadBMP ("piege_bmp.bmp");

   SDL_Rect position;
   int joueur1_actuel_i = 0;
   int joueur1_actuel_j = 0;
   int joueur2_actuel_i = 0;
   int joueur2_actuel_j = 0;

   for (i=0; i<nb_blocs_hauteur; i=i+1)
   {
         for (j=0; j<nb_blocs_largeur; j=j+1)
        {
          position.x = j * taille_bloc;
          position.y = i * taille_bloc;
          switch (carte [i][j])
          {
            case '1' :
                SDL_BlitSurface (mur, NULL, ecran , &position);
                break;
            case '2' :
                SDL_BlitSurface (piece, NULL, ecran , &position);
                break;
            case '3' :
                SDL_BlitSurface (bonhomme_1, NULL, ecran , &position);
                joueur1_actuel_i = i;
                joueur1_actuel_j = j;
                break;
            case '4' :
                SDL_BlitSurface (bonhomme_2, NULL, ecran , &position);
                joueur2_actuel_i = i;
                joueur2_actuel_j = j;
                break;
            case '5' :
                SDL_BlitSurface (piege, NULL, ecran , &position);
                break;
            default :
                break;
         }
        }
    }

    SDL_Flip (ecran);

    deplacement_joueur_demo (carte, ecran, joueur1_actuel_i, joueur1_actuel_j, joueur2_actuel_i, joueur2_actuel_j,
                             bonhomme_1, bonhomme_2, blanc);
    SDL_FreeSurface(mur);
    SDL_FreeSurface(piece);
    SDL_FreeSurface(blanc);
    SDL_FreeSurface(bonhomme_1);
    SDL_FreeSurface(bonhomme_2);
    SDL_FreeSurface(piege);
//    SDL_FreeSurface(gagnant);

return;
}

void deplacement_joueur_demo (char carte[nb_blocs_hauteur][nb_blocs_largeur], SDL_Surface*ecran,
                              int joueur1_actuel_i ,int joueur1_actuel_j,int joueur2_actuel_i, int joueur2_actuel_j,
                              SDL_Surface*bonhomme_1, SDL_Surface*bonhomme_2 , SDL_Surface *blanc)
{
    int continuer = 1;
    int joueur1_futur_i = 0;
    int joueur1_futur_j = 0;
    int joueur2_futur_i = 0;
    int joueur2_futur_j = 0;
    int numero_du_joueur;

    SDL_Rect position;
    SDL_Event event;
    while (continuer && num_gagnant == 0)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT :
                continuer = 0;
                break;
            case SDL_KEYDOWN :
                numero_du_joueur = 0;
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE :
                    continuer = 0 ;
                    break;
                case SDLK_UP :
                    joueur1_futur_i = joueur1_actuel_i - 1 ;
                    joueur1_futur_j = joueur1_actuel_j ;
                    numero_du_joueur = 1;
                    bonhomme_1 = SDL_LoadBMP ("bonhomme_1_up_bmp.bmp");
                    break;
                case SDLK_DOWN :
                    joueur1_futur_i = joueur1_actuel_i + 1 ;
                    joueur1_futur_j = joueur1_actuel_j ;
                    numero_du_joueur = 1;
                    bonhomme_1 = SDL_LoadBMP ("bonhomme_1_down_bmp.bmp");
                    break;
                case SDLK_RIGHT :
                    joueur1_futur_i = joueur1_actuel_i ;
                    joueur1_futur_j = joueur1_actuel_j + 1;
                    numero_du_joueur = 1;
                    bonhomme_1 = SDL_LoadBMP ("bonhomme_1_right_bmp.bmp");
                    break;
                case SDLK_LEFT :
                    joueur1_futur_i = joueur1_actuel_i ;
                    joueur1_futur_j = joueur1_actuel_j - 1;
                    numero_du_joueur = 1;
                    bonhomme_1 = SDL_LoadBMP ("bonhomme_1_left_bmp.bmp");
                    break;

                case SDLK_e :    /// UP JOUEUR 2   ///probleme touche z
                    joueur2_futur_i = joueur2_actuel_i - 1 ;
                    joueur2_futur_j = joueur2_actuel_j ;
                    numero_du_joueur = 2;
                    bonhomme_2 = SDL_LoadBMP ("bonhomme_2_up_bmp.bmp");
                    break;
                case SDLK_d :   /// DOWN
                    joueur2_futur_i = joueur2_actuel_i + 1 ;
                    joueur2_futur_j = joueur2_actuel_j ;
                    numero_du_joueur = 2;
                    bonhomme_2 = SDL_LoadBMP ("bonhomme_2_down_bmp.bmp");
                    break;
                case SDLK_f :   /// RIGHT
                    joueur2_futur_i = joueur2_actuel_i ;
                    joueur2_futur_j = joueur2_actuel_j + 1;
                    numero_du_joueur = 2;
                    bonhomme_2 = SDL_LoadBMP ("bonhomme_2_right_bmp.bmp");
                    break;
                case SDLK_s :   /// LEFT    ///probleme touche q
                    joueur2_futur_i = joueur2_actuel_i ;
                    joueur2_futur_j = joueur2_actuel_j - 1;
                    numero_du_joueur = 2;
                    bonhomme_2 = SDL_LoadBMP ("bonhomme_2_left_bmp.bmp");
                    break;
                }
                ///if (numero_du_joueur == 1 && joueur1_vivant == 1)
                if (numero_du_joueur == 1)

                {
                    if (carte [joueur1_futur_i][joueur1_futur_j] == '0' ||     /// '0' : case vide
                        carte [joueur1_futur_i][joueur1_futur_j] == '2' ||     /// '2' : pièce
                        carte [joueur1_futur_i][joueur1_futur_j] == '5' ||     /// '5' : piège
                        carte [joueur1_futur_i][joueur1_futur_j] == '4')       /// '4' : joueur adverse (joueur 2 pour la démo)
                    {
                        if (carte [joueur1_futur_i][joueur1_futur_j] == '2')
                        {
                            joueur1_score++;
                        }
                        if (carte [joueur1_futur_i][joueur1_futur_j] == '5')
                        {
                            joueur1_score--;
                        }

                        if (carte [joueur1_futur_i][joueur1_futur_j] == '4')
                        {
                            joueur1_score = joueur1_score + 3;
                            num_gagnant = 1;
                                                }

                        carte [joueur1_actuel_i ][joueur1_actuel_j] = '0';     /// sur la carte : Mise à zéro de l'ancienne case de joueur1
                        carte [joueur1_futur_i ][joueur1_futur_j] = '3';       /// sur la carete : Indiquer nouvelle case du joueur1

                        position.x = joueur1_actuel_j * taille_bloc;           /// Cacher avec du Blanc l'ancienne image du joueur1
                        position.y = joueur1_actuel_i * taille_bloc;
                        SDL_BlitSurface (blanc, NULL, ecran , &position);

                        position.x = joueur1_futur_j * taille_bloc;            /// déplacer le joueur
                        position.y = joueur1_futur_i * taille_bloc;
                        SDL_BlitSurface (bonhomme_1, NULL, ecran , &position);

                        joueur1_actuel_i = joueur1_futur_i;                    /// mise à jour des nouvelles coordonnées du joueur1
                        joueur1_actuel_j = joueur1_futur_j;
                        }
                }

                if (numero_du_joueur == 2)
                {
                    if (carte [joueur2_futur_i][joueur2_futur_j] == '0' ||
                        carte [joueur2_futur_i][joueur2_futur_j] == '2' ||
                        carte [joueur2_futur_i][joueur2_futur_j] == '5' ||
                        carte [joueur2_futur_i][joueur2_futur_j] == '3' )     /// on avance si case vide ou pièce ou piège
                    {
                    if (carte [joueur2_futur_i][joueur2_futur_j] == '2')
                    {
                        joueur2_score++;
                    }
                    if (carte [joueur2_futur_i][joueur2_futur_j] == '5')
                    {
                        joueur2_score--;
                    }
                    if (carte [joueur2_futur_i][joueur2_futur_j] == '3')
                        {
                            joueur2_score = joueur2_score + 3;
                            num_gagnant = 2;
                        }

                    carte [joueur2_actuel_i ][joueur2_actuel_j] = '0';       /// sur la carte : Mise à zéro de l'ancienne case de joueur1
                    carte [joueur2_futur_i ][joueur2_futur_j] = '4';         /// sur la carete : Indiquer nouvelle case du joueur1

                    position.x = joueur2_actuel_j * taille_bloc;             /// Cacher avec du Blanc l'ancienne image du joueur1
                    position.y = joueur2_actuel_i * taille_bloc;
                    SDL_BlitSurface (blanc, NULL, ecran , &position);

                    position.x = joueur2_futur_j * taille_bloc;              ///déplacer le joueur
                    position.y = joueur2_futur_i * taille_bloc;
                    SDL_BlitSurface (bonhomme_2, NULL, ecran , &position);

                    joueur2_actuel_i = joueur2_futur_i;                      /// mise à jour des nouvelles coordonnées du joueur1
                    joueur2_actuel_j = joueur2_futur_j;
                    }
                }
                }
        SDL_Flip (ecran);
    }
    return;
}

void remplir_carte_avec_fichier (char carte[nb_blocs_hauteur][nb_blocs_largeur])
{
  const char* nom_fichier [2] = FILES;
  FILE *ff = NULL;
  int j, i;
  int k = 1;
  char contenu_du_fichier [nb_blocs_largeur] ;
  if (mode_demo == 1)
  {
      k=0;
  }
  for (i=0 ; i<nb_blocs_largeur ; i=i+1)
    {
      contenu_du_fichier[i] = '0';
    }

  ff = fopen(nom_fichier[k], "r");
  if (ff == NULL)
    {
      printf("** Erreur fopen : %s \n", nom_fichier [k]);
      return 0;
    }
    i=0;
    while (fgets(contenu_du_fichier, nb_blocs_largeur, ff)!= NULL) /// Lecture fichier ligne par ligne
       {
       for (j=0 ; j<nb_blocs_largeur ; j=j+1)
       {
       ///carte [i][j] = contenu_du_fichier[j];          /// ************  PROBLEME A REVOIR
        switch (contenu_du_fichier [j])
         {
         case '0' :
            carte [i][j] = '0';
            break;
         case '1' :
            carte [i][j] = '1';
            break;
         case '2' :
            carte [i][j] = '2';
            break;
         case '3' :
            carte [i][j] = '3';
            break;
         case '4' :
            carte [i][j] = '4';
            break;
         case '5' :
            carte [i][j] = '5';
            break;
         case 'A' :
            carte [i][j] = 'A';
            break;
         case 'B' :
            carte [i][j] = 'B';
            break;
         case 'C' :
            carte [i][j] = 'C';
            break;
         case 'D' :
            carte [i][j] = 'D';
            break;
         case 'E' :
            carte [i][j] = 'E';
            break;
         case 'F' :
            carte [i][j] = 'F';
            break;
         default :
            carte [i][j] = '0';
         }
       }
       i=i+1;
      }
    fclose(ff);

    return ;
}
void effacer_carte (char carte[nb_blocs_hauteur][nb_blocs_largeur])
 {
    int i, j;

    for (i=0; i<nb_blocs_hauteur; i++)
    {
            for (j=0; j<nb_blocs_largeur; j++)
            {
                carte[i][j] = '0' ;
            }
    }
    return ;
 }

 /// /////////////////////////////////////////////////////
 ///                                                   ///
 ///                      Mode IA                      ///
 ///                                                   ///
 /// /////////////////////////////////////////////////////

 void labyrinthe_multi (SDL_Surface*ecran)
{
   char carte [nb_blocs_hauteur][nb_blocs_largeur] ;
   int i , j ;
   int num_joueur = 0 ;
   int count_tours;
   char action_joueur;

   effacer_carte (carte);
   remplir_carte_avec_fichier(carte);

   SDL_Surface *mur = NULL ;
   SDL_Surface *piece = NULL ;
   SDL_Surface *bonhomme_1 = NULL ;
   SDL_Surface *bonhomme_2 = NULL ;
   SDL_Surface *blanc = NULL ;
   SDL_Surface *piege = NULL ;

   mur = SDL_LoadBMP("mur3.bmp");
   piece = SDL_LoadBMP ("piece_bmp2.bmp");
   bonhomme_1 = SDL_LoadBMP ("bonhomme_1_down_bmp.bmp");
   bonhomme_2 = SDL_LoadBMP ("bonhomme_2_down_bmp.bmp");
   blanc = SDL_LoadBMP ("Blanc1.bmp");
   piege = SDL_LoadBMP ("piege_bmp.bmp");

   SDL_Rect position;

   for (i=0; i<nb_blocs_hauteur; i=i+1)
   {
         for (j=0; j<nb_blocs_largeur; j=j+1)
        {
          position.x = j * taille_bloc;
          position.y = i * taille_bloc;
          switch (carte [i][j])
          {
            case '0' :
                break;
            case '1' :
                SDL_BlitSurface (mur, NULL, ecran , &position);
                break;
            case '2' :
                SDL_BlitSurface (piece, NULL, ecran , &position);
                break;
            case '5' :
                SDL_BlitSurface (piege, NULL, ecran , &position);
                break;
            default :
                SDL_BlitSurface (bonhomme_2, NULL, ecran , &position);
                pos_actuel_i [num_joueur]= i;
                pos_actuel_j [num_joueur]= j;
                code_joueur [num_joueur] = carte [i][j];
                num_joueur = num_joueur + 1 ;
                break;
         }
        }
    }

    SDL_Flip (ecran);

    for (count_tours = 0; count_tours < max_tours; count_tours++)
     {
            num_joueur = 0;
            action_joueur = ia_0 (carte, count_tours);
            deplacement_joueur_multi (action_joueur, num_joueur, carte , ecran , pos_actuel_i , pos_actuel_j, bonhomme_1, bonhomme_2, blanc);
            SDL_Event event;
            SDL_WaitEvent(&event);

            num_joueur ++;
            action_joueur = ia_1 (carte, count_tours);
            deplacement_joueur_multi (action_joueur, num_joueur, carte , ecran , pos_actuel_i , pos_actuel_j, bonhomme_1, bonhomme_2, blanc);
            SDL_WaitEvent(&event);

            num_joueur ++;
            action_joueur = ia_2 (carte, count_tours);
            deplacement_joueur_multi (action_joueur, num_joueur, carte , ecran , pos_actuel_i , pos_actuel_j, bonhomme_1, bonhomme_2, blanc);
            SDL_WaitEvent(&event);

            num_joueur ++;
            action_joueur = ia_3 (carte, count_tours);
            deplacement_joueur_multi (action_joueur, num_joueur, carte , ecran , pos_actuel_i , pos_actuel_j, bonhomme_1, bonhomme_2, blanc);
            SDL_WaitEvent(&event);

            if  (count_tours > max_coups_ia)
            {
             break;
            }
        }

    SDL_FreeSurface(mur);
    SDL_FreeSurface(piece);
    SDL_FreeSurface(piege);
    SDL_FreeSurface(blanc);
    SDL_FreeSurface(bonhomme_1);
    SDL_FreeSurface(bonhomme_2);

return;
}

void deplacement_joueur_multi (char action_joueur, int num_joueur,char carte[nb_blocs_hauteur][nb_blocs_largeur],
                            SDL_Surface*ecran, int pos_actuel_i [max_joueurs] ,int pos_actuel_j [max_joueurs],
                            SDL_Surface*bonhomme_1, SDL_Surface*bonhomme_2 , SDL_Surface *blanc)
{
    int pos_future_i = 0;
    int pos_future_j = 0;
    int continuer = 1;
    SDL_Rect position;

    switch (action_joueur)   /// convertir le type d'action (envoyé par l'IA de test) en coordonnées futures qui devront être calculées
            {
                case 'U' :
                    pos_future_i = pos_actuel_i [num_joueur]- 1 ;
                    pos_future_j = pos_actuel_j [num_joueur];
                    break;
                case 'D' :
                    pos_future_i = pos_actuel_i [num_joueur] + 1 ;
                    pos_future_j = pos_actuel_j [num_joueur];
                    break;
                case 'R' :
                    pos_future_i = pos_actuel_i [num_joueur];
                    pos_future_j = pos_actuel_j [num_joueur] + 1;
                    break;
                case 'L' :
                    pos_future_i = pos_actuel_i [num_joueur];
                    pos_future_j = pos_actuel_j [num_joueur] - 1;
                    break;
                default :
                    continuer = 0;
                    break;
            }

            if (continuer == 1)
                {
                    if (carte [pos_future_i][pos_future_j] == '0' ||
                        carte [pos_future_i][pos_future_j] == '2' ||   /// on avance si case vide, pièce, piège
                        carte [pos_future_i][pos_future_j] == '5')     /// ajouter les adversaires
                    {
                    if (carte [pos_future_i][pos_future_j] == '2')
                    {
                        scores [num_joueur]++;
                    }

                    if (carte [pos_future_i][pos_future_j] == '5')
                    {
                        scores [num_joueur]--;
                    }
                    carte [pos_actuel_i [num_joueur] ][pos_actuel_j [num_joueur]] = '0';  /// sur la carte : Mise à zéro de l'ancienne case de joueur1

                    carte [pos_future_i ][pos_future_j] = code_joueur [num_joueur];  /// sur la carete : Indiquer nouvelle case du joueur1

                    position.x = pos_actuel_j [num_joueur] * taille_bloc;           /// Cacher avec du Blanc l'ancienne image du joueur1
                    position.y = pos_actuel_i [num_joueur] * taille_bloc;
                    SDL_BlitSurface (blanc, NULL, ecran , &position);

                    position.x = pos_future_j * taille_bloc;                       ///déplacer le joueur
                    position.y = pos_future_i * taille_bloc;

                    /// prévoir la gestion de la couleur des bonhommes fonction du jouenr
                    SDL_BlitSurface (bonhomme_2, NULL, ecran , &position);

                    pos_actuel_i [num_joueur] = pos_future_i;                      /// mise à jour des nouvelles coordonnées du joueur1
                    pos_actuel_j [num_joueur] = pos_future_j;
                    SDL_Flip (ecran);
                    }
                }
    return;
}
char ia_0 (char carte[nb_blocs_hauteur][nb_blocs_largeur], int nb_tours)
 {
    char action ;
    char simulation_action [max_tours] ={'D','D','D','L','L','R'};
    action = simulation_action [nb_tours];
    return action;
}

char ia_1 (char carte[nb_blocs_hauteur][nb_blocs_largeur], int nb_tours)
 {
    char action ;
    char simulation_action [max_tours] ={'U','U','U','L','L','R'};
    action = simulation_action [nb_tours];
    return action;
}
char ia_2 (char carte[nb_blocs_hauteur][nb_blocs_largeur], int nb_tours)
 {
    char action ;
    char simulation_action [max_tours] ={'D','U','D','L','D','L'};
    action = simulation_action [nb_tours];
    return action;
}
char ia_3 (char carte[nb_blocs_hauteur][nb_blocs_largeur], int nb_tours)
 {
    char action ;
    char simulation_action [max_tours] ={'U','D','U','D','L','R'};
    action = simulation_action [nb_tours];
    return action;
}


void afficher_scores_demo (SDL_Surface * ecran,int gagnant)
{
    SDL_Event event;
    SDL_Rect position;
    position.x = 32 * taille_bloc;
    position.y = 3 * taille_bloc;

    if (num_gagnant == 1)
    gagnant = SDL_LoadBMP ("gagnant_1_bmp.bmp");
    else
    {
        gagnant = SDL_LoadBMP ("gagnant_2_bmp.bmp");
    }
    SDL_BlitSurface (gagnant, NULL, ecran , &position);
    SDL_Flip (ecran);
    SDL_WaitEvent(&event);
   return ;
}

void afficher_scores (SDL_Surface *ecran, int gagnant, int scores[max_joueurs])
{
    SDL_Event event;
    SDL_Rect position;
    position.x = 32 * taille_bloc;
    position.y = 3 * taille_bloc;

    gagnant = SDL_LoadBMP ("fin_partie_bmp.bmp");

    SDL_BlitSurface (gagnant, NULL, ecran , &position);
    SDL_Flip (ecran);
    SDL_WaitEvent(&event);
   return ;
}


/*
 static void afficher_scores (SDL_Surface * ecran )
{

   TTF_Init();

   if(TTF_Init() == -1){
    fprintf(stderr, "erreur de TTF_Init : %s", TTF_GetError());
    exit(EXIT_FAILURE);
   }

   TTF_Quit();
   return ;
}
*/
