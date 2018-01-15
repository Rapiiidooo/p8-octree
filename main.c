#include <unistd.h>     
#include <math.h>
#include "head.h"

#ifndef __OSX__
#include <GL/glut.h>
#include <GL/glu.h>	
#endif

Image *image;
Image *image_or;
Tab_index * tab_index;
Octree * arbre;

#define ESCAPE 27

void Keyboard(unsigned char key, int x, int y)  {
  switch(key){
  case ESCAPE :
    exit(0);                   
    break;
  case 'c':
    tab_index = init_tab(tab_index, image);
    arbre = init_octree(arbre);

    timer("\0");
    table_index(image, tab_index, arbre, 0);
    timer("Algo Octree rapide");
    timer("\0");
    avecperte(image, tab_index, arbre);
    timer("Subdiviser et redessiner d'après la tab");
    Display();
    break;
  default:
    fprintf(stderr, "Unused key\n");
  }
}

void Mouse(int button, int state, int x, int y) {

  switch(button){
  case GLUT_LEFT_BUTTON:
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;    
  }
  glutPostRedisplay();
}

int Init(char *s) {

  image = (Image *) malloc(sizeof(Image));
  image_or = (Image *) malloc(sizeof(Image));
  if (image == NULL || image_or == NULL) {
    fprintf(stderr, "Out of memory\n");
    return(-1);
  }
  if (ImageLoad_PPM(s, image)==-1) 
	return(-1);
  if (ImageLoad_PPM(s, image_or)==-1) 
  return(-1);
  printf("tailles %d %d\n",(int) image->sizeX, (int) image->sizeY);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  
  return (0);
}
int ReInit() {

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  
  return (0);
}

int Init_decompress (char * s)
{
  image = (Image *) malloc(sizeof(Image));
  image_or = (Image *) malloc(sizeof(Image));
  if (image == NULL || image_or == NULL) {
    fprintf(stderr, "Out of memory\n");
    return(-1);
  }
  if (my_imageload(s, tab_index, image) ==-1) 
    return(-1);

  printf("tailles %d %d\n",(int) image->sizeX, (int) image->sizeY);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  return (1);
}

void Display(void) {
  
  //GLint w, h;
  glClear(GL_COLOR_BUFFER_BIT);
  //w = glutGet(GLUT_WINDOW_WIDTH);
  //h = glutGet(GLUT_WINDOW_HEIGHT);
  glDrawPixels(image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, 
	       image->data);

  glFlush();
}

void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble)h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void menuFunc(int item) {
  char s[256];

  switch(item){
  case 0:
    free(image);
    exit(0);
    break;
  case 1:
    timer("\0");
    table_index(image, tab_index, arbre, 0);
    timer("Tab index done");
    break;
  case 2:
    //COMPRESS RAPIDE
    timer("\0");
    table_index(image, tab_index, arbre, 0);
    timer("Algo Octree rapide");
    timer("\0");
    avecperte(image, tab_index, arbre);
    timer("Subdiviser et redessiner d'après la tab");

    Display();
    break;
  case 3:
    //COMPRESS LENTE
    timer("\0");
    table_index(image, tab_index, arbre, 1);
    timer("Algo Octree lent");
    timer("\0");
    avecperte(image, tab_index, arbre);
    timer("Subdiviser et redessiner d'après la tab");
    Display();
    break;
  case 4:
    image = memcpy(image, image_or, sizeof(Image));
    ReInit();
    Display();
    break;
  case 5:
    printf("Tout gris\n");
    gris_uniforme(image);
	  Display();
    break;
  case 6:
    printf("Entrer le nom pour l'image dans cette taille\n");
    if(!scanf("%s", &s[0]))
      printf("Err main.c l.117\n");
    if(!tab_index)
      imagesave_PPM(s, image);
    else
      my_imagesave(s, tab_index, image);
    break;
  case 7:
    printf("Taille de l image : %ld %ld\n", (long int) image->sizeX, (long int) image->sizeY);
    break;
  default:
    break;
  }
}

int main(int argc, char **argv) {  

  if (argc<2) {
    fprintf(stderr, "Usage : octree nom_de_fichier\n");
    exit(0);
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(640,480);  
  glutInitWindowPosition(100, 100);  
  glutCreateWindow("VPUP8");  

  if(strstr(argv[1], ".ppm"))
  {
    Init(argv[1]);
    tab_index = init_tab(tab_index, image);
    arbre = init_octree(arbre);
  }
  else
    Init_decompress(argv[1]);

  glutCreateMenu(menuFunc);
  glutAddMenuEntry("Quit", 0);
  glutAddMenuEntry("Sans perte", 1);
  glutAddMenuEntry("Compress", 2);
  glutAddMenuEntry("Compress lente", 3);
  glutAddMenuEntry("Back to Normal", 4);
  glutAddMenuEntry("50 nuances de grey", 5);
  glutAddMenuEntry("Sauver", 6);
  glutAddMenuEntry("Informations", 7);
  glutAttachMenu(GLUT_LEFT_BUTTON);

  glutDisplayFunc(Display);  
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  
  glutMouseFunc(Mouse);

  glutMainLoop();  

  my_free(tab_index, arbre);

  return 0;
}
