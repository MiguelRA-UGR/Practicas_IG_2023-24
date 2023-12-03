#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

const float AxisSize=1000000;
// ----------------------------------------------------------------------
// NECESARIO PARA CARGAR LAS IMÁGENES DE DISCO
#include "CImg.h"

using namespace cimg_library;
// ----------------------------------------------------------------------

using namespace std;

//Functions declarations
void Init();
void OnDraw();
void SpecialKeys(int key,int x,int y);

// FUNCIÓN QUE PREPARA LA TEXTURA
void prepara_textura(char *file, GLuint *tex_id);

// FUNCIÓN QUE DIBUJA
void dibuja(void);

// FUNCIÓN QUE LIBERA LA TEXTURA DE GPU
void libera_textura(GLuint *tex_id);


// IDENTIFICADOR DE TEXTURA
GLuint textura_id;
GLuint textura_id2;

// OBJETO A DIBUJAR(TRIANGULO A TRIANGULO)
/*
GLfloat vertices[] = {0, 0,0,   10, 0, 0,   0, 5, 0,  //primer triangulo
                      10,0,0,   10,5,0,     0,5,0};   //segundo triangulo


GLfloat texVertices[] = {0.0,1.0, 1.0,1.0, 0.0,0.0, 
                         1.0,1.0, 1.0,0.0, 0.0,0.0};
*/

// Coordenadas de los vértices de la pirámide
/*
GLfloat vertices[] = {-4, 0, 4,   4, 0, 4,   0, 6, 0,   // Triángulo base frontal
                      4, 0, 4,   4, 0, -4,   0, 6, 0,  // Triángulo lateral derecho
                      4, 0, -4,   -4, 0, -4,   0, 6, 0, // Triángulo base trasero
                      -4, 0, -4,   -4, 0, 4,   0, 6, 0, // Triángulo lateral izquierdo
                      
                      0, 0, 2,   2, 0, 2,   0, 2, 2,   // Cara frontal inferior
                          2, 0, 2,   2, 2, 2,   0, 2, 2,   // Cara frontal superior
                          0, 0, 0,   2, 0, 0,   0, 2, 0,   // Cara trasera inferior
                          2, 0, 0,   2, 2, 0,   0, 2, 0,   // Cara trasera superior
                          0, 0, 0,   2, 0, 0,   0, 0, 2,   // Cara lateral izquierda
                          0, 0, 2,   2, 0, 0,   2, 0, 2}; 

GLfloat texVertices[] = {0.0, 0.5,  0.5, 0.5,   0.25, 0.0,  
                         0.5, 0.5,  1.0, 0.5,   0.75, 0.0,
                         0.0, 1.0,  0.5, 1.0,   0.25, 0.5, 
                         0.5, 1.0,  1.0, 1.0,   0.75, 0.5,
                         
                        0.0, 1.0,   0.5, 1.0,   0.0, 0.5,   // Cara frontal inferior
                        0.5, 1.0,   0.5, 0.5,   0.0, 0.5,   // Cara frontal superior
                        0.0, 0.0,   0.5, 0.0,   0.0, 0.5,   // Cara trasera inferior
                        0.5, 0.0,   0.5, 0.5,   0.0, 0.5,   // Cara trasera superior
                        0.0, 0.0,   0.5, 0.0,   0.0, 1.0,   // Cara lateral izquierda
                        0.0, 1.0,   0.5, 0.0,   0.5, 1.0};
*/
GLfloat vertices[] = {
// Suelo
-100, -100, -100,   100, -100, -100,   100, -100, 100,
-100, -100, -100,   100, -100, 100,   -100, -100, 100,

// Frontal
100, 100, -100,   -100, 100, -100,   100, -100, -100,
100, -100, -100,   -100, 100, -100,   -100, -100, -100,

// Techo
-100, 100, 100,   -100, 100, -100,   100, 100, -100,
-100, 100, 100,   100, 100, 100,   100, 100, -100,

// Izquierdo
-100, 100, 100,   -100, 100, -100,   -100, -100, -100,
-100, 100, 100,   -100, -100, 100,     -100, -100, -100,

// Derecho
100, 100, 100,   100, 100, -100,   100, -100, -100,
100, 100, 100,   100, -100, 100,   100, -100, -100,

// Posterior
-100, -100, 100,   100, -100, 100,   100, 100, 100,
-100, -100, 100,   -100, 100, 100,   100, 100, 100

};

GLfloat texVertices[] = {
   // Suelo
   0.25, 0.5,    0.5, 0.5,    0.5, 0.75,
   0.25, 0.5,    0.25, 0.75,  0.5, 0.75,

   // Frontal
   0.5, 0.25,    0.25, 0.25,  0.5, 0.5,
   0.5, 0.5,     0.25, 0.25,  0.25, 0.5,

   // Techo
   0.25, 0.0,    0.25, 0.25,  0.5, 0.25,
   0.25, 0.0,    0.5, 0.0,    0.5, 0.25,

   // Izquierdo
   0.0, 0.25,    0.25, 0.25,    0.25, 0.5,
   0.0, 0.25,    0.0, 0.5,    0.25, 0.5,

   // Derecho
   0.75, 0.25,   0.5, 0.25,   0.5, 0.5,
   0.75, 0.25,   0.75, 0.5,    0.5, 0.5,

   // Posterior
   1.0, 0.5,     0.75, 0.5,   0.75, 0.25,   
   1.0, 0.5,    1, 0.25,   0.75, 0.25
};



float latitud=0.0,longitud=0.0,radio=24;

void Init(int argc, char **argv){
   //Initialize GLUT windows manager
   //Create the window
   glutInit(&argc, argv);
   glutInitWindowSize(800,600);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow("My World");

   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_COLOR_MATERIAL);
   glMatrixMode(GL_PROJECTION);
   gluPerspective( 40.0, 800/600.0f, 0.1, 500);
  
   prepara_textura("skybox.jpeg", &textura_id);
}

void draw_axis(){
   glLineWidth(2);
   glBegin(GL_LINES);
       // eje X, color rojo
       glColor3f(1,0,0);
       glVertex3f(-AxisSize,0,0);
       glVertex3f(AxisSize,0,0);
       // eje Y, color verde
       glColor3f(0,1,0);
       glVertex3f(0,-AxisSize,0);
       glVertex3f(0,AxisSize,0);
       // eje Z, color azul
       glColor3f(0,0,1);
       glVertex3f(0,0,-AxisSize);
       glVertex3f(0,0,AxisSize);
   glEnd();
   glLineWidth(1);
   glColor3f(1,1,1);
}

void OnDraw(void){

   // cámara orbital
   float observador[3];
   float phir,thetar;
   phir=latitud*M_PI/180.0; 
   thetar=longitud*M_PI/180.0;
   observador[0]=radio*sin(thetar)*cos(phir);
   observador[1]=radio*sin(phir);
   observador[2]=radio*cos(phir)*cos(thetar);

   //cleaning the screen
   glClearColor(0.5,0.5,0.5,1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //Define view
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // eye position -> observador
   // target -> (0,0,0)
   // define positive Y axis  -> (0.0, 1.0, 0.0)		
   gluLookAt (observador[0],observador[1],observador[2],0.0,0.0,0.0,0.0,1.0*cos(phir),0.0);

   //draw_axis();
   //Put your code here to draw objects
   dibuja();

   //no delete this line
   glutSwapBuffers();
}


void SpecialKeys(int key,int x,int y) {
    switch (key){
	case GLUT_KEY_LEFT:longitud +=2.0;break;
	case GLUT_KEY_RIGHT:longitud -=2.0;break;
	case GLUT_KEY_UP:latitud -=2.0;break;
	case GLUT_KEY_DOWN:latitud +=2.0;break;
	case GLUT_KEY_PAGE_UP:radio +=1.0;break;
	case GLUT_KEY_PAGE_DOWN:radio -=1.0;if (radio<1) radio=1;break;
	}
    glutPostRedisplay();
}

int main(int argc, char** argv) {
   Init(argc, argv);
   //Enter the callbacks
   glutDisplayFunc(OnDraw);
   glutSpecialFunc(SpecialKeys);

   glutMainLoop(); // begin the loop

   // LIBERA LA TEXTURA
   libera_textura(&textura_id);
   return 0;
}


// ----------------------------------------------------------------------
void prepara_textura (char *file, GLuint *tex_id )
{
   vector<unsigned char> data; 
   CImg<unsigned char> image;

   image.load(file);

   // empaquetamos bien los datos
   for (long y = 0; y < image.height(); y ++)
      for (long x = 0; x < image.width(); x ++)
      {
	 unsigned char *r = image.data(x, y, 0, 0);
	 unsigned char *g = image.data(x, y, 0, 1);
	 unsigned char *b = image.data(x, y, 0, 2);
	 data.push_back(*r);
	 data.push_back(*g);
	 data.push_back(*b);
      }

   glGenTextures(1, tex_id);
   glBindTexture(GL_TEXTURE_2D, *tex_id);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // TRASFIERE LOS DATOS A GPU
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
		0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
}


void dibuja (void)
{
   glEnable(GL_TEXTURE_2D);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);

   glBindTexture(GL_TEXTURE_2D, textura_id);
   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
   glDrawArrays(GL_TRIANGLES, 0, 36);

/*
glBindTexture(GL_TEXTURE_2D, textura_id);

   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
   glDrawArrays(GL_TRIANGLES, 0, 12);

   glPushMatrix();
   glTranslatef(0,4.0,0);
   glRotatef(90,0,1,0);
   glScalef(0.8,0.8,0.8);
   glDrawArrays(GL_TRIANGLES, 0, 12);
   glPopMatrix();

   glBindTexture(GL_TEXTURE_2D, textura_id2);
   glPushMatrix();
   glTranslatef(6,0,6);
   glRotatef(75,0,1,0);
   glScalef(0.5,0.5,0.5);
   glDrawArrays(GL_TRIANGLES, 12, 36);
   glPopMatrix();


   glDisableClientState(GL_VERTEX_ARRAY);
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_TEXTURE_2D);
*/
   
   
}

void libera_textura (GLuint *tex_id)
{
   glDeleteTextures(1, tex_id);
}

