#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define __OSX__
#ifndef __OSX__
#include <GL/gl.h>
#else
#include <GL/glut.h>
#endif

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    GLubyte *data;
};
typedef struct Image Image;
typedef unsigned short utab [3][3][3];

struct Pixel 
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    int nb; // Inutile pour algo rapide
    int moy_p;
    int indice_img; // Inutile pour algo rapide
};
typedef struct Pixel Pixel;

struct Tab_index
{
   	struct Pixel * pixel;
   	int * index_image;
    int nb_pixel;
    int nb_index;
};
typedef struct Tab_index Tab_index;

struct Octree
{
	struct Octree * o[8];
};
typedef struct Octree Octree;

int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);
void Display(void);

//Table d'indexage
void table_index (Image * img, Tab_index * tab, Octree * arbre, int bool);
void add_pixel_to_tab (Tab_index * tab, Pixel p);
void add_moyenne_pond (Tab_index * tab);
void draw_from_index_image (Tab_index * tab, Image * i);
void avecperte (Image * img, Tab_index * tab, Octree * arbre);
Tab_index * my_realloc (Tab_index * tab);
Tab_index * init_tab (Tab_index * tab, Image * image);

void algo_rapide (Image * img, Tab_index * tab, Octree * arbre);
void tab_surmem (Tab_index * t, int tab[255][255][255], Pixel p, int i);

void algo_lent (Image * img, Tab_index * tab, Octree * arbre);
void init_indice_from_table (Tab_index * tab);
Tab_index * recopie_to_tab_pix (Tab_index * tab);
Tab_index * cmp_tab (Tab_index * tab);
Pixel empty (Pixel b);
int pixel_egal (Pixel a, Pixel b);
int isempty (Pixel a);
int nb_null (Tab_index * tab);
int find_indice_by_pix (Tab_index * tab, Pixel p);
int mycmp_qs (const void * a, const void * b);

//Octree
Octree * init_octree (Octree * a);
void my_free (Tab_index * tab, Octree * arbre);
void free_arbre (Octree * arbre);
void affiche_arbre (Octree * a, int ind);
void subdiviser (Tab_index * tab, Octree * arbre, int sizeX, int sizeY, int xo, int yo, int profondeur);
void redraw_with_octree (Tab_index * tab, Octree * arbre, int x, int y, int xo, int yo);
void nb_dernier_enfant (Octree * arbre);
int ifenfant (Octree * arbre);
int comp_moy_pond (Tab_index * tab, int x, int y, int xo, int yo);
int ifenfant (Octree * arbre);

//Autre
void gris_uniforme (Image * i);
void timer (char  * str);
void my_imagesave(char *filename, Tab_index * tab, Image * img);
int my_imageload(char *filename, Tab_index * tab, Image * img);
