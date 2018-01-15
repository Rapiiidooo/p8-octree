#include "head.h"
#include <limits.h>

static Pixel * _img_cpy;
static int _size = 0;
static int _sizeXimg = 0;
static int profondeur_max = 10;
static int taille_nb_pixel = 2048;
static int 	_taux = 1;
GLubyte * im;
int _nb = 0;
int nbcoul_max = 0;
int tableau[255][255][255];

// **********************************
// DEBUT FONCTION TABLEAU D'INDEXAGE
// **********************************

//DEBUG
void affiche_index (Tab_index * tab, int a)
{
	int i;
	printf("Taille table : %d\n", tab->nb_pixel);
	printf("Taille image_index : %d\n", tab->nb_index);
	//Print de la table
	if(a == 0)
		for (i = 0; i < tab->nb_pixel; ++i)
		{
			printf("Num : %d / RGB = {%d, %d, %d} / NB = %d / indice_img = %d / moy_p = %d; \n"
				, i, tab->pixel[i].r, tab->pixel[i].g, tab->pixel[i].b, tab->pixel[i].nb, tab->pixel[i].indice_img, tab->pixel[i].moy_p);
		}
	
	// Print du tableau d'index de l'image
	if(a == 1)
		for (i = 0; i < tab->nb_index; ++i)
			printf("I = %d / tab->index_image[i] = %d\n", i, tab->index_image[i]);
	printf("affiche_index() : done\n");
}

Tab_index * init_tab (Tab_index * tab, Image * image)
{
	if(!image)
	{
		fprintf(stderr, "ERREUR !!!!!! init_tab\n");
		exit(1);
	}
	_size = image->sizeX * image->sizeY;
	_sizeXimg = image->sizeX;
	_img_cpy = (Pixel *) malloc(_size * sizeof(Pixel));
	assert(_img_cpy);
	im = (GLubyte*) malloc (_size*3 * sizeof(*im));
	assert(im);
	tab = (Tab_index *) malloc(sizeof(Tab_index));
	assert(tab);
	tab->pixel = (Pixel *) malloc(taille_nb_pixel * sizeof(tab->pixel[0]));
	assert(tab->pixel);
	tab->index_image = (int *) malloc(_size * sizeof(int));
	assert(tab->index_image);
	tab->nb_pixel = 0;
	tab->nb_index = 0;
	tab->pixel[0].nb = 0;
	tab->pixel[0].moy_p = 0;
	tab->pixel[0].indice_img = 0;
	return tab;
}
	
Tab_index * my_realloc (Tab_index * tab)
{
	if(!tab)
	{
		printf("ERREUR !!!! src.c my_realloc\n");
		exit(1);
	}
	taille_nb_pixel *= 2;
	tab->pixel = (Pixel *) realloc(tab->pixel, taille_nb_pixel * sizeof(tab->pixel[0]));
	assert(tab->pixel);
	return tab;
}
//-------------------------------
// Fonctions pour le tableau de pixel
int pixel_egal (Pixel a, Pixel b)
{
	if(a.r == b.r && a.g == b.g && a.b == b.b)
		return 1;
	return 0;
}
Pixel empty (Pixel b)
{
	Pixel a;
	a.r = 0;
	a.g = 0;
	a.b = 0;
	a.indice_img = b.indice_img;
	a.nb = -1;
	return a;
}
int isempty (Pixel a)
{
	if (a.nb == -1)
		return 1;
	return 0;
}
int nb_null (Tab_index * tab)
{
	int i, nb;
	nb = 0;
	for (i = 0; i < tab->nb_pixel; ++i)
	{
		if(isempty(tab->pixel[i]))
			nb++;
	}
	return nb;
}

int find_indice_by_pix (Tab_index * tab, Pixel p)
{
	int i;
	for (i = 0; i < tab->nb_pixel; ++i)
	{
		if (pixel_egal(p, tab->pixel[i]))
			return i;
	}
	return -1;
}

void init_indice_from_table (Tab_index * tab)
{
	if(!tab)
	{
		printf("ERREUR !!!! src.c init_indice_from_table\n");
		exit(1);
	}
	int i;
	for (i = 0; i < tab->nb_pixel; ++i)
	{
		tab->index_image[tab->pixel[i].indice_img] = i;
	}
	
	//FONCTIONNE MAIS PREND 2MINUTES
	for (i = 0; i < tab->nb_index; ++i)
	{
		if(tab->index_image[i] == 0)
		{
			tab->index_image[i] = find_indice_by_pix(tab, _img_cpy[i]);
		}
	}
}

Tab_index * recopie_to_tab_pix (Tab_index * tab)
{
	if(!tab)
	{
		printf("ERREUR !!!! src.c recopie_to_tab_pix\n");
		exit(1);
	}
	int i, j, nb;
	Pixel * new_tab_pix;
	taille_nb_pixel = tab->nb_pixel - nb_null(tab);
	new_tab_pix = (Pixel *) malloc(taille_nb_pixel * sizeof(Pixel));
	assert(new_tab_pix);

	i = j = nb = 0;
	while (i < tab->nb_pixel)
	{
		if(!isempty(tab->pixel[i]))
		{
			new_tab_pix[j] = tab->pixel[i];
			nb++;
			j++;
		}
		i++;
	}
	tab->pixel = new_tab_pix;
	tab->nb_pixel = nb;
	return tab;
}

int mycmp_qs (const void * a, const void * b)
{
	Pixel * p1, * p2;
	p1 = (Pixel *) a;
	p2 = (Pixel *) b;

	if(p1->g < p2->g)
		return 1;
	else if (p1->g == p2->g)
	{
		if(p1->r < p2->r)
			return 1;
		else if (p1->r == p2->r)
		{
			if(p1->b < p2->b)
				return 1;
			return 0;
		}
		return 0;
	}
	return 0;
}

Tab_index * cmp_tab (Tab_index * tab)
{
	if(!tab)
	{
		printf("ERREUR !!!! src.c cmp_tab\n");
		exit(1);
	}
	int i, j;
	qsort(tab->pixel, tab->nb_pixel, sizeof(Pixel*), mycmp_qs);
	for (i = 0; i < tab->nb_pixel; i++)
	{
		if(pixel_egal(tab->pixel[i], tab->pixel[i+1]))
		{
			tab->pixel[i].nb++;
			j = i+1;
			while(pixel_egal(tab->pixel[i], tab->pixel[j+1]))
			{
				tab->pixel[i].nb++;
				//AJOUT TEST

				// FIN
				tab->pixel[j] = empty(tab->pixel[j]);
				j++;
			}
			tab->pixel[j] = empty(tab->pixel[j]);
			i = j;
		}
	}

	//recopie du tableau sans les pixels vide au lieu de le resizer
	tab = recopie_to_tab_pix(tab);
	return tab;
}

void add_pixel_to_tab (Tab_index * tab, Pixel p)
{
	if(!tab)
	{
		printf("La table n'existe pas. src.c add_pixel_to_tab\n");
		exit(1);
	}

	if(taille_nb_pixel <= tab->nb_pixel)
		tab = my_realloc(tab);

	tab->pixel[tab->nb_pixel++] = p;
}

void free_arbre (Octree * arbre)
{
	int i;

	if(!arbre)
		return;
	for (i = 0; i < 8; ++i)
	{
		free_arbre(arbre->o[i]);
	}
	free(arbre);
}
void my_free (Tab_index * tab, Octree * arbre)
{
	free_arbre(arbre);
	free(tab->pixel);
	free(tab->index_image);
	free(tab);
}

void add_moyenne_pond (Tab_index * tab)
{
	if(!tab)
	{
		printf("ERREUR !!!! src.c add_moyenne_pond\n");
		return;
	}
	int i;
	for (i = 0; i < tab->nb_pixel; ++i)
	{
		tab->pixel[i].moy_p = ((tab->pixel[i].g * 10000) 
						+ (tab->pixel[i].r * 1000) 
						+ (tab->pixel[i].b * 100)) 
						/ 11100;
	}
}

void draw_from_index_image (Tab_index * tab, Image * i)
{
	if(!tab || !i)
	{
		printf("ERREUR !!!! src.c draw_from_index_image\n");
		return;
	}
	int j;
  	GLubyte * im;

  	im = i->data;
  	for (j = 0; j < tab->nb_index; j++) 
  	{
		*im++ = tab->pixel[tab->index_image[j]].r;
		*im++ = tab->pixel[tab->index_image[j]].g;
		*im++ = tab->pixel[tab->index_image[j]].b;
  	}
}

//RAPIDE MAIS GOURMAND!

void tab_surmem (Tab_index * tab, int tableau[255][255][255], Pixel p, int i)
{
	if(tableau[p.r][p.g][p.b] == -1)
	{
		tableau[p.r][p.g][p.b] = _nb;
		
		if(taille_nb_pixel <= _nb)
			tab = my_realloc(tab);
		
		tab->pixel[_nb].r = p.r;
		tab->pixel[_nb].g = p.g;
		tab->pixel[_nb].b = p.b;
		tab->index_image[i] = _nb;
		_nb++;
		tab->nb_pixel = _nb;
	}
	else if(tableau[p.r][p.g][p.b] > -1)
	{
		tab->index_image[i] = tableau[p.r][p.g][p.b];
	}
}

void algo_rapide (Image * img, Tab_index * tab, Octree * arbre)
{
	if(!img || !tab || !arbre)
	{
		printf("ERREUR !!!! src.c algo_rapide\n");
		return;
	}
	int i;
	
	Pixel p;

	memset(tableau, -1, sizeof(tableau));
	p.nb = 0;
	p.moy_p = 0;
	p.indice_img = 0;
	im = img->data;
	for (i = 0; i < _size; ++i)
	{
		p.r = *im++;
		p.g = *im++;
		p.b = *im++;
		tab_surmem(tab, tableau, p, i);
	}
	tab->nb_index = i;
}

//LENT MAIS FONCTIONNE AVEC "PEU" DE MEMOIRE !
void algo_lent (Image * img, Tab_index * tab, Octree * arbre)
{
	if(!img || !tab || !arbre)
	{
		printf("ERREUR !!!! src.c algo_lent\n");
		return;
	}
	int i;
	Pixel p;
	
	p.nb = 0;
	p.moy_p = 0;
	p.indice_img = 0;
	im = img->data;
	for (i = 0; i < _size; ++i)
	{
		p.r = *im++;
		p.g = *im++;
		p.b = *im++;
		add_pixel_to_tab(tab, p);
		_img_cpy[i] = p;
		tab->pixel[i].indice_img = i;
	}
	tab->nb_index = i;
	tab = cmp_tab(tab); // Tris et efface les doublons + incremente les nb
	init_indice_from_table(tab); // Initialise index_image
}

// void set_all (Tab_index * tab, int val, int new_val)
// {
// 	int i;
// 	for (int i = 0; i < tab->nb_index; ++i)
// 	{
// 		if(tab->index_image[i] == val)
// 			tab->index_image[i] = new_val;
// 	}
// }

void avecperte (Image * img, Tab_index * tab, Octree * arbre)
{
	add_moyenne_pond(tab);
	subdiviser(tab, arbre, img->sizeX, img->sizeY, 0, 0, 0);
	redraw_with_octree(tab, arbre, img->sizeX, img->sizeY, 0, 0);
	draw_from_index_image(tab, img);


	//AFFICHAGE NB DE COULEURS SUR IMAGE
	if(nbcoul_max == 0)
		fprintf(stdout, "Nombre de couleurs depart : %d\n", tab->nb_pixel);
	else
		fprintf(stdout, "Nombre de couleurs depart : %d\n", nbcoul_max);
	nbcoul_max = 0;
	nb_dernier_enfant(arbre); //Modify nbcoul_max
	fprintf(stdout, "Nombre de couleurs restantes : %d\n", nbcoul_max);
}

void table_index (Image * img, Tab_index * tab, Octree * arbre, int boolean)
{
	//_nb = 0; // >> COMPRESS PUIS ALGO SANS PERTE PUIS SAUVER 
	if(!img || !tab || !arbre)
	{
		printf("ERREUR !!!! src.c table_index\n");
		return;
	}
	_size = img->sizeY * img->sizeX;
	_taux += 4;

	if(boolean == 0)
		algo_rapide(img, tab, arbre);
	else
		algo_lent(img, tab, arbre);
}

void nb_dernier_enfant (Octree * a)
{
	if(!a)
		return;;
	int i;
	int boolean = 1;
	for (i = 0; i < 8; ++i)
	{
		if(a->o[i] != NULL)
		{
			boolean = 0;
			nb_dernier_enfant(a->o[i]);
		}
	}
	if(boolean != 0)
		nbcoul_max++;	
}

// **********************************
// FIN FONCTION TABLEAU D'INDEXAGE
// **********************************


// **********************************
// DEBUT FONCTION SUBDIVISER
// **********************************

void affiche_arbre (Octree * a, int ind)
{
	if(!a)
		return;
	int i, j;
	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < ind; ++j)
		{
			printf("\t");
		}
		if(a->o[i])
			printf("Profondeur : %d / Enfant [%d] = exist\n", ind, i);
		else
			printf("Profondeur : %d / Enfant [%d] = no\n", ind, i);
		affiche_arbre(a->o[i], ind+1);
	}
}

Octree * init_octree (Octree * a)
{
	int i;
 	a = (Octree *) malloc(sizeof(Octree));
 	assert(a);

	for (i = 0; i < 8; ++i)
	{
		a->o[i] = (Octree *) malloc(sizeof(Octree));
		assert(a->o[i]);
		a->o[i] = NULL;
	}
	return a;
}

int comp_moy_pond (Tab_index * tab, int x, int y, int xo, int yo)
{
	int i, j;
	int initial;
	initial = tab->pixel[tab->index_image[yo * _sizeXimg + xo]].moy_p;
	for (i = yo; i < yo+y; ++i)
	{
		for (j = xo; j < xo+x; ++j)
		{
			if(tab->pixel[tab->index_image[i * _sizeXimg + j]].moy_p > initial + _taux 
				|| tab->pixel[tab->index_image[i * _sizeXimg + j]].moy_p < initial - _taux)
				return 1;
		}
	}
	return 0;
}

void subdiviser (Tab_index * tab, Octree * arbre, int sizeX, int sizeY, int xo, int yo, int profondeur)
{	
	if(!tab || !arbre)
	{
		printf("Erreur : La table d'index ou l'arbre n'existe pas.\n");
		return;
	}
	int i, x, y;

	x = sizeX/2;
	y = sizeY/4;

	if(profondeur < profondeur_max)
	{
		profondeur++;
		//Coté Gauche
		for (i= 0; i < 4; ++i)
		{
			//Verif si besoin de subdiviser
			if(comp_moy_pond(tab, x, y, xo, yo) == 1)
			{
				arbre->o[i] = init_octree(arbre->o[i]);
				subdiviser(tab, arbre->o[i], x, y, xo, yo, profondeur);
			}
			yo += sizeY/4;
		}
		//Coté Droit
		yo = sizeY/4;
		for (i= 4; i < 8; ++i)
		{
			if(comp_moy_pond(tab, sizeX, y, x, yo) == 1)
			{
				arbre->o[i] = init_octree(arbre->o[i]);
				subdiviser(tab, arbre->o[i], sizeX, y, x, yo, profondeur);
			}
			yo += sizeY/4;
		}
	}
}

int ifenfant (Octree * arbre)
{
	if(!arbre)
		return 0;
	int i;
	for (i = 0; i < 8; ++i)
	{
		if(arbre->o[i] != NULL)
			return 1;
	}
	return 0;
}

void redraw_with_octree (Tab_index * tab, Octree * arbre, int x, int y, int xo, int yo)
{
	if(!tab)
		return;
	int i, j, k, tmp, ytmp;

	ytmp = yo;
	//Coté Gauche
	for (k = 0; k < 4; ++k)
	{
		if(ifenfant(arbre->o[k]) == 0)
		{
			tmp = tab->index_image[yo * _sizeXimg + xo];
			for (i = yo; i < yo+y/4; ++i)
			{
				for (j = xo; j < xo+x/2; ++j)
				{	
					tab->index_image[i * _sizeXimg + j] = tmp;
				}
			}
		}
		else
		{
			redraw_with_octree(tab, arbre->o[k], x/2, y/4, xo, yo);
		}
		yo += y/4;
	}

	yo = ytmp;
	//Coté Droit
	for (k = 4; k < 8; ++k)
	{
		if(ifenfant(arbre->o[k]) == 0)
		{
			tmp = tab->index_image[yo * _sizeXimg + xo + x/2];
			for (i = yo; i < yo+y/4; ++i)
			{
				for (j = xo+(x/2); j < xo+x; ++j)
				{	
					tab->index_image[i * _sizeXimg + j] = tmp;
				}
			}
		}
		else
		{
			redraw_with_octree(tab, arbre->o[k], x/2, y/4, xo+x/2, yo);
		}
		yo += y/4;
	}
}

// **********************************
// FIN FONCTION SUBDIVISER
// **********************************

void my_imagesave(char *filename, Tab_index * tab, Image * img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) 
    {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }
    if (!tab) 
    {
         fprintf(stderr, "Unable to read Tab index '%s'\n", filename);
         exit(1);
    }
    fwrite(&tab->nb_pixel, (size_t) 4, (size_t) 1, fp);
    fwrite(&img->sizeX, (size_t) 4, (size_t) 1, fp);
    fwrite(&img->sizeY, (size_t) 4, (size_t) 1, fp);
    // index data
    fwrite(tab->index_image, (size_t) 1, (size_t) tab->nb_index, fp);
    fwrite(tab->pixel, (size_t) sizeof(*tab->pixel), (size_t) tab->nb_pixel, fp);

    fclose(fp);
}

int my_imageload(char *filename, Tab_index * tab, Image * img)
{
    FILE *fp;
    size_t voi_d;
    //open file for output
    fp = fopen(filename, "rb");
    if (!fp)
    {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         return (1);
    }
    voi_d = fread(&tab->nb_pixel, (size_t) 4, (size_t) 1, fp);
    voi_d = fread(&img->sizeX, (size_t) 4, (size_t) 1, fp);
    voi_d = fread(&img->sizeY, (size_t) 4, (size_t) 1, fp);
    tab->nb_index = img->sizeX * img->sizeY;
    // index data
    voi_d = fread(tab->index_image, (size_t) 1, (size_t) tab->nb_index, fp);
    voi_d = fread(tab->pixel, (size_t) sizeof(*tab->pixel), (size_t) tab->nb_pixel, fp);
    printf("%lu\n", voi_d);
    fclose(fp);
    return (1);
}

//Made by JJB
void gris_uniforme(Image * i) {
  int j, _size;
  GLubyte * im, val;
  val = 128;
  _size = 3 * i->sizeY * i->sizeX;
  im = i->data;
  for (j = 0; j < _size; j ++) {
	*im++ = val;
  }
}