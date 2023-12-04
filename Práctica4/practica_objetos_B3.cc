//**************************************************************************
// Práctica 3 
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B3.h"
#include "include/irrKlang.h"
#include <thread>
#include <chrono>
#include "include/CImg.h"

using namespace irrklang;
using namespace std;
using namespace cimg_library;

// FUNCIÓN QUE PREPARA LA TEXTURA
void prepara_textura(char *file, GLuint *tex_id);

// FUNCIÓN QUE DIBUJA
void dibuja(void);

// FUNCIÓN QUE LIBERA LA TEXTURA DE GPU
void libera_textura(GLuint *tex_id);


// IDENTIFICADOR DE TEXTURA
GLuint textura_id;

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



GLfloat vertices[] = {
// Suelo
-250, 0, -250,   250, 0, -250,   250, 0, 250,
-250, 0, -250,   250, 0, 250,   -250, 0, 250,

// Frontal
250, 300, -250,   -250, 300, -250,   250, -200, -250,
250, -200, -250,   -250, 300, -250,   -250, -200, -250,

// Techo
-250, 300, 250,   -250, 300, -250,   250, 300, -250,
-250, 300, 250,   250, 300, 250,   250, 300, -250,

// Izquierdo
-250, 300, 250,   -250, 300, -250,   -250, -200, -250,
-250, 300, 250,   -250, -200, 250,     -250, -200, -250,

// Derecho
250, 300, 250,   250, 300, -250,   250, -200, -250,
250, 300, 250,   250, -200, 250,   250, -200, -250,

// Posterior
-250, -200, 250,   250, -200, 250,   250, 300, 250,
-250, -200, 250,   -250, 300, 250,   250, 300, 250


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


int numDivisions = 3;


ISoundEngine *sonido = createIrrKlangDevice();
    
// tipos
typedef enum{CUBO, PIRAMIDE, OBJETO_PLY, ROTACION, CILINDRO, CONO, ESFERA, EXTRUSION, EXCAVADORA,ATAT} _tipo_objeto;
typedef enum{UNO,DOS,TRES,CUATRO} _turno_pata;
typedef enum{FEMUR_UP,TIBIA_UP,PATA_DOWN} _turno_parte;
_tipo_objeto t_objeto=CUBO;
_modo   modo=POINTS;
_turno_pata turno_pata =UNO;
_turno_parte turno_parte =FEMUR_UP;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=650,Window_y=50,Window_width=650,Window_high=650;


// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_objeto_ply  ply; 
_rotacion rotacion;
_cilindro cilindro(1,2,30); 
_cono cono(1,2,30);
_esfera esfera(1,30,30);
_excavadora excavadora;
_rotacion_PLY perfil_ply;
_extrusion *extrusion;
_atat atat;

// _objeto_ply *ply;

bool arriba=false;
bool derecha=false;
bool canion=true;
bool sonido_on=false;
bool disparo_habilitado=false;
bool empezar_pasos=false;

float giro1=0, giro2=0, giro3=0, giro4=0;
float retro=0;
float velocidad_disparo=10;
float velocidad=0.25;
int pulsar=0;
int paso_cabeza=0;
int paso_piernas=0;
int paso_canion=0;

std::chrono::steady_clock::time_point ultimo_disparo=std::chrono::steady_clock::now();

//**************************************************************************
//
//***************************************************************************

void clean_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
//  plano_delantero>0  plano_trasero>PlanoDelantero)
glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

// posicion del observador
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance);
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	
glDisable(GL_LIGHTING);
glLineWidth(2);
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

switch (t_objeto){
	case CUBO: cubo.draw(modo,1.0,0.0,0.0,5);break;
	case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,5);break;
        case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,5);break;
        case ROTACION: perfil_ply.draw(modo,1.0,0.0,0.0,5);break;
        case CILINDRO: cilindro.draw(modo,1.0,0.0,0.0,5);break;
        case CONO: cono.draw(modo,1.0,0.0,0.0,5);break;
        case ESFERA: esfera.draw(modo,1.0,0.0,0.0,5);break;
        case EXCAVADORA: excavadora.draw(modo,1.0,0.0,0.0,5);break;
        case EXTRUSION: extrusion->draw(modo,1.0,0.0,0.0,5);break;
        case ATAT: atat.draw(modo,1.0,0.0,0.0,5);
        
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);

        glBindTexture(GL_TEXTURE_2D, textura_id);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        break;
	}

    

}


//**************************************************************************
//  luces
//***************************************************************************

//Lo mismo pero con una luz que se mueva

void luces()
{ 
    // Definir propiedades de la luz
    GLfloat luz_ambiental[] = {0.05, 0.05, 0.05, 1.0};
    GLfloat luz_difusa[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat luz_especular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat luz_posicion[] = {10.0, 10.0, 10.0, 1.0};

    // Habilitar iluminación y la luz específica (GL_LIGHT0)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Configurar propiedades de la luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiental);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);

    // También puedes deshabilitar la iluminación si es necesario
    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}


//**************************************************************************
//
//***************************************************************************


void draw(void)
{
clean_window();
change_observer();
//luces();
//draw_axis();
draw_objects();
glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
float Aspect_ratio;

Aspect_ratio=(float) Alto1/(float )Ancho1;
Size_y=Size_x*Aspect_ratio;
change_projection();
glViewport(0,0,Ancho1,Alto1);
glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y)
{
switch (toupper(Tecla1)){
	case 'Q':exit(0);
	case '1':modo=POINTS;break;
	case '2':modo=EDGES;break;
	case '3':modo=SOLID;break;
	case '4':modo=SOLID_COLORS;break;
    case '5':modo=SOLID_COLORS_GOURAUD;break;
    case '6':modo=SOLIDO_PHONG_FLAT;break;
    case '7':modo=SOLIDO_PHONG_GOURAUD;break;
    case '8':modo=ROJOS;break;
	case '9':modo=VERDES;break;
	case '0':modo=AZULES;break;
	
        case 'P':t_objeto=PIRAMIDE;break;
        case 'C':t_objeto=CUBO;break;
        case 'O':t_objeto=OBJETO_PLY;break;	
        case 'R':t_objeto=ROTACION;break;
        case 'L':t_objeto=CILINDRO;break;
        case 'N':t_objeto=CONO;break;
        case 'E':t_objeto=ESFERA;break;
        case 'A':t_objeto=EXCAVADORA;break;
        case 'X':t_objeto=EXTRUSION;break;
        case 'T':t_objeto=ATAT;break;
        case 'K':sonido_on=!sonido_on;
            disparo_habilitado=true; 
            if(!sonido_on)sonido->stopAllSounds();break;
	    case 'S':sonido_on=!sonido_on;
        if (pulsar==0)
                    {giro1=0.15;
                     pulsar=1;
                     retro=0.1;
                     giro2=0.25;
                     }
                 else
                    {giro1=0.0;
                     giro2=0.0;
                     giro3=0.0;
                     pulsar=0;
                     retro=0;
                     }    
                 break;
        
	}
glutPostRedisplay();
}

void caminar(float velocidad_patas){
    switch(turno_pata){
            case UNO:
                switch(turno_parte){
                    case FEMUR_UP:
                        atat.giro_pata_1+=velocidad_patas*6;
        
                        atat.giro_pata_4-=velocidad_patas/2;
                        atat.giro_pata_2-=velocidad_patas/2;
                        atat.giro_pata_3-=velocidad_patas/2;


                        atat.pata_1.giro_rodilla+=velocidad_patas*6;

                        atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;

                        if(abs(atat.giro_pata_1) > atat.giro_patas_max)
                            turno_parte=TIBIA_UP;
                    break;
                    case TIBIA_UP:
                        atat.pata_1.giro_rodilla-=velocidad_patas*24;
                        atat.pata_1.abajo.giro_pie-=velocidad_patas*24;

                        atat.giro_pata_4-=velocidad_patas/2;
                        atat.giro_pata_2-=velocidad_patas/2;
                        atat.giro_pata_3-=velocidad_patas/2;

                        atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;

                        if(atat.pata_1.giro_rodilla*(velocidad_patas) < 0)
                            turno_parte=PATA_DOWN;
                    break;
                    case PATA_DOWN:
                        atat.giro_pata_1-=velocidad_patas*6;
                        atat.pata_1.abajo.giro_pie+=velocidad_patas*6;

                        atat.giro_pata_4-=velocidad_patas/2;
                        atat.giro_pata_2-=velocidad_patas/2;
                        atat.giro_pata_3-=velocidad_patas/2;

                        atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;

                        if(abs(atat.giro_pata_1) < atat.giro_patas_max/3){
                            turno_pata=CUATRO;
                            turno_parte=FEMUR_UP;
                            if(sonido_on==true && !sonido->isCurrentlyPlaying("audio/pisadas.wav")){
                                sonido->play2D("audio/pisadas.wav");
                            }
                                
                        }
                    break;
                }
            break;

            case CUATRO:
            switch(turno_parte){
                    case FEMUR_UP:
                    atat.giro_pata_4+=velocidad_patas*6;
        
                    atat.giro_pata_1-=velocidad_patas/2;
                    atat.giro_pata_2-=velocidad_patas/2;
                    atat.giro_pata_3-=velocidad_patas/2;

                    atat.pata_4.giro_rodilla+=velocidad_patas*6;

                    atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                    atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                    atat.pata_1.abajo.giro_pie+=velocidad_patas/2;

                    if(abs(atat.giro_pata_4) > atat.giro_patas_max)
                        turno_parte=TIBIA_UP;
                    break;
                    case TIBIA_UP:
                         atat.pata_4.giro_rodilla-=velocidad_patas*24;
                        atat.pata_4.abajo.giro_pie-=velocidad_patas*24;

                        atat.giro_pata_1-=velocidad_patas/2;
                        atat.giro_pata_2-=velocidad_patas/2;
                        atat.giro_pata_3-=velocidad_patas/2;

                        atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_1.abajo.giro_pie+=velocidad_patas/2;

                        if(atat.pata_4.giro_rodilla*(velocidad_patas) < 0)
                            turno_parte=PATA_DOWN;
                    break;
                    case PATA_DOWN:
                    atat.giro_pata_4-=velocidad_patas*6;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas*6;

                        atat.giro_pata_1-=velocidad_patas/2;
                        atat.giro_pata_2-=velocidad_patas/2;
                        atat.giro_pata_3-=velocidad_patas/2;

                        atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_1.abajo.giro_pie+=velocidad_patas/2;


                        if(abs(atat.giro_pata_4) < atat.giro_patas_max/3){
                            turno_pata=DOS;
                            turno_parte=FEMUR_UP;
                        }
                    break;
                }
            break;

            case DOS:
            switch(turno_parte){
                    case FEMUR_UP:
                        atat.giro_pata_2+=velocidad_patas*6;
        
                        atat.giro_pata_1-=velocidad_patas/2;
                        atat.giro_pata_3-=velocidad_patas/2;
                        atat.giro_pata_4-=velocidad_patas/2;


                        atat.pata_2.giro_rodilla-=velocidad_patas*6;

                        atat.pata_1.abajo.giro_pie+=velocidad_patas/2;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;

                        if(abs(atat.giro_pata_2) > atat.giro_patas_max)
                            turno_parte=TIBIA_UP;
                    break;
                    case TIBIA_UP:
                        atat.pata_2.giro_rodilla+=velocidad_patas*24;
                        atat.pata_2.abajo.giro_pie+=velocidad_patas*24;

                        atat.giro_pata_1-=velocidad_patas/2;
                        atat.giro_pata_3-=velocidad_patas/2;
                        atat.giro_pata_4-=velocidad_patas/2;

                        atat.pata_1.abajo.giro_pie+=velocidad_patas/2;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;


                        if(atat.pata_2.giro_rodilla*(velocidad_patas) > 0)
                            turno_parte=PATA_DOWN;
                    break;
                    case PATA_DOWN:
                        atat.giro_pata_2-=velocidad_patas*6;
                        atat.pata_2.abajo.giro_pie-=velocidad_patas*6;

                        atat.giro_pata_1-=velocidad_patas/2;
                        atat.giro_pata_3-=velocidad_patas/2;
                        atat.giro_pata_4-=velocidad_patas/2;

                        atat.pata_1.abajo.giro_pie+=velocidad_patas/2;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;


                        if(abs(atat.giro_pata_2) < atat.giro_patas_max/3){
                            turno_pata=TRES;
                            turno_parte=FEMUR_UP;
                        }
                    break;
                }
            break;

            case TRES:
            switch(turno_parte){
                    case FEMUR_UP:
                        atat.giro_pata_3+=velocidad_patas*6;
        
                        atat.giro_pata_2-=velocidad_patas/2;
                        atat.giro_pata_1-=velocidad_patas/2;
                        atat.giro_pata_4-=velocidad_patas/2;


                        atat.pata_3.giro_rodilla-=velocidad_patas*6;

                        atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_1.abajo.giro_pie+=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;

                        if(abs(atat.giro_pata_3) > atat.giro_patas_max)
                            turno_parte=TIBIA_UP;
                    break;
                    case TIBIA_UP:
                        atat.pata_3.giro_rodilla+=velocidad_patas*24;
                        atat.pata_3.abajo.giro_pie+=velocidad_patas*24;

                        atat.giro_pata_2-=velocidad_patas/2;
                        atat.giro_pata_1-=velocidad_patas/2;
                        atat.giro_pata_4-=velocidad_patas/2;

                        atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_1.abajo.giro_pie+=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;

                        if(atat.pata_3.giro_rodilla*(velocidad_patas) > 0)
                            turno_parte=PATA_DOWN;
                    break;
                    case PATA_DOWN:
                        atat.giro_pata_3-=velocidad_patas*6;
                        atat.pata_3.abajo.giro_pie-=velocidad_patas*6;

                        atat.giro_pata_2-=velocidad_patas/2;
                        atat.giro_pata_1-=velocidad_patas/2;
                        atat.giro_pata_4-=velocidad_patas/2;

                        atat.pata_2.abajo.giro_pie-=velocidad_patas/2;
                        atat.pata_1.abajo.giro_pie+=velocidad_patas/2;
                        atat.pata_4.abajo.giro_pie+=velocidad_patas/2;

                        if(abs(atat.giro_pata_3) < atat.giro_patas_max/3){
                            turno_pata=UNO;
                            turno_parte=FEMUR_UP;
                        }
                    break;
                }
            break;
        }
}


//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1,int x,int y)
{

switch (Tecla1){
   case GLUT_KEY_LEFT:Observer_angle_y--;break;
   case GLUT_KEY_RIGHT:Observer_angle_y++;break;
   case GLUT_KEY_UP:Observer_angle_x--;break;
   case GLUT_KEY_DOWN:Observer_angle_x++;break;
   case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
   case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
    
    
    //MOVIMIENTO AT-AT
    case GLUT_KEY_F1:
    
    if (arriba){
        atat.giro_cabeza_z+=1;
        
        if(atat.giro_cabeza_z>= atat.giro_cabeza_max_z)
            arriba=false;
    }
    else{
        atat.giro_cabeza_z-=1;
        if(atat.giro_cabeza_z<= -atat.giro_cabeza_max_z)
            arriba=true;
    }
    break;

    case GLUT_KEY_F2:;
     if (derecha){
        atat.giro_cabeza_y+=1;
        
        if(atat.giro_cabeza_y>= atat.giro_cabeza_max_y)
            derecha=false;
    }
    else{
        atat.giro_cabeza_y-=1;
        if(atat.giro_cabeza_y<= -atat.giro_cabeza_max_y)
            derecha=true;
    }
    break;

    case GLUT_KEY_F3:
    if (arriba){
        atat.cabeza.giro_torretas_z+=1;
        
        if(atat.cabeza.giro_torretas_z>= atat.cabeza.giro_torreta_max)
            arriba=false;
    }
    else{
        atat.cabeza.giro_torretas_z-=1;
        if(atat.cabeza.giro_torretas_z<= -atat.cabeza.giro_torreta_max)
            arriba=true;
    }
    break;

    case GLUT_KEY_F4:
        switch (paso_canion) {
            case 0:
                if(atat.cabeza.retroceso1<0)
                    atat.cabeza.retroceso1 += 0.5;

                if (atat.cabeza.retroceso2 > -atat.cabeza.retroceso_maximo) {
                    atat.cabeza.retroceso2 -= 0.5;
                    atat.cabeza.dist2 -= velocidad_disparo;
                }
                else{
                    paso_canion = 1;
                    atat.cabeza.dist1=0;
                }

                break;

            case 1:

                if(atat.cabeza.retroceso2<0)
                    atat.cabeza.retroceso2 += 0.5;

                if (atat.cabeza.retroceso1 > -atat.cabeza.retroceso_maximo) {
                    atat.cabeza.retroceso1 -= 0.5;
                    atat.cabeza.dist1 -= velocidad_disparo;
                }
                else{
                    paso_canion = 2;
                    atat.cabeza.dist2=0;
                }

                break;
            case 2:
            if(atat.cabeza.retroceso1<0)
                    atat.cabeza.retroceso1 += 0.5;
            else
                paso_canion = 0;
            break;
        }
        //sonido_disparo();
    break;

    case GLUT_KEY_F5:
        caminar(velocidad);
    break;

    case GLUT_KEY_F6:
        caminar(-velocidad);
    break;


    /*MOVIMIENTO EXCAVADORA
         case GLUT_KEY_F1:excavadora.giro_cabina+=5;break;
   case GLUT_KEY_F2:excavadora.giro_cabina-=5;break;
   case GLUT_KEY_F3:excavadora.giro_primer_brazo+=1;
        if (excavadora.giro_primer_brazo > excavadora.giro_primer_brazo_max)
            excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_max;break;
   case GLUT_KEY_F4:excavadora.giro_primer_brazo-=1;
        if (excavadora.giro_primer_brazo < excavadora.giro_primer_brazo_min)
            excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_min;break;
   case GLUT_KEY_F5:excavadora.giro_segundo_brazo+=1;
        if (excavadora.giro_segundo_brazo > excavadora.giro_segundo_brazo_max)
            excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_max;break;
   case GLUT_KEY_F6:excavadora.giro_segundo_brazo-=1;
        if (excavadora.giro_segundo_brazo < excavadora.giro_segundo_brazo_min) 
            excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_min;break;
   case GLUT_KEY_F7:excavadora.giro_pala+=1;
        if (excavadora.giro_pala > excavadora.giro_pala_max)
            excavadora.giro_pala = excavadora.giro_pala_max;break;
   case GLUT_KEY_F8:excavadora.giro_pala-=1;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
            excavadora.giro_pala = excavadora.giro_pala_min;break;
    */
  
	}
glutPostRedisplay();
}


//***************************************************************************
// Funcion de animación automática
//***************************************************************************

void animacion(){
    
    //animacion de las patas
    caminar(giro1);
    //animacion de la cabeza
    switch (paso_cabeza){
        case 0: 
                atat.giro_cabeza_y-=giro2;
                atat.giro_cabeza_z-=giro2;

                if(atat.giro_cabeza_y < -atat.giro_cabeza_max_y)
                paso_cabeza=1;
                break;

            case 1:
                atat.giro_cabeza_y+=giro2;

                if(atat.giro_cabeza_y > atat.giro_cabeza_max_y)
                paso_cabeza=2;
                break;
            case 2:
                atat.giro_cabeza_z+=giro2;

                if(atat.giro_cabeza_z == 0)
                paso_cabeza=3;
                break;
            case 3:
                atat.giro_cabeza_y-=giro2;

                if(atat.giro_cabeza_y == 0)
                paso_cabeza=0;
                break;
    } 

        if(giro1 !=0){

            std::chrono::duration<double> tiempo_transcurrido;

            auto tiempo_actual = std::chrono::steady_clock::now();
            tiempo_transcurrido = tiempo_actual - ultimo_disparo;

            if (tiempo_transcurrido.count() >= 3) {
                if(sonido_on)sonido->play2D("audio/shot.wav");
                disparo_habilitado=true;
                ultimo_disparo = std::chrono::steady_clock::now();
            }
            
            if(disparo_habilitado){
                switch (paso_canion) {
            case 0:
                
                if(atat.cabeza.retroceso1<0)
                    atat.cabeza.retroceso1 += 0.5;

                if (atat.cabeza.retroceso2 >= -atat.cabeza.retroceso_maximo) {

                    atat.cabeza.retroceso2 -= 0.5;
                    atat.cabeza.dist2 -= velocidad_disparo;
                }
                else{
                    atat.cabeza.dist2=0;
                    paso_canion = 1;
                }

                break;

            case 1:
                if(atat.cabeza.retroceso2<0)
                    atat.cabeza.retroceso2 += 0.5;
                
                if (atat.cabeza.retroceso1 >= -atat.cabeza.retroceso_maximo) {
                    
                    atat.cabeza.retroceso1 -= 0.5;
                    atat.cabeza.dist1 -= velocidad_disparo;
                }
                else{
                    atat.cabeza.dist1=0;
                    paso_canion = 2;
                }
                break;
            case 2:
            if(atat.cabeza.retroceso1<0)
                    atat.cabeza.retroceso1 += 0.5;
            else{
                paso_canion = 0;
                disparo_habilitado=false;
            }
            break;
            }
            }
                
            
            
        }

glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
// se inicalizan la ventana y los planos de corte
Size_x=0.5;
Size_y=0.5;
Front_plane=1;
Back_plane=1000;

// se incia la posicion del observador, en el eje z
Observer_distance=6*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
change_projection();
glViewport(0,0,Window_width,Window_high);
prepara_textura("skybox.jpeg", &textura_id);
}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char *argv[] )
{

// perfil 

vector<_vertex3f> perfil, poligono;
_vertex3f aux;

aux.x=1.0; aux.y=-1.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.5; aux.y=0.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.0; aux.y=1.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.5; aux.y=1.2; aux.z=0.0;
perfil.push_back(aux);


rotacion.parametros(perfil,6,1,1,0);

aux.x=1.0; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);
aux.x=1.0; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-0.8; aux.y=0.0; aux.z=0.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);

extrusion= new _extrusion(poligono, 0.25, 1.0, 0.25);

// se llama a la inicialización de glut
glutInit(&argc, argv);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(Window_x,Window_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(Window_width,Window_high);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("PRACTICA - 3");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw);
// asignación de la funcion llamada "change_window_size" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "normal_key" al evento correspondiente
glutKeyboardFunc(normal_key);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_key);

glutIdleFunc(animacion);

	
// funcion de inicialización
initialize();

// creación del objeto ply

ply.parametros(argv[1]);
perfil_ply.parametros_PLY(argv[2],50);

//ply = new _objeto_ply(argv[1]);

// inicio del bucle de eventos
glutMainLoop();
libera_textura(&textura_id);
return 0;
}

void libera_textura (GLuint *tex_id)
{
   glDeleteTextures(1, tex_id);
}
