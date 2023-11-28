//**************************************************************************
// Práctica 2 
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>


const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS,SOLID_COLORS_GOURAUD,SOLIDO_PHONG_FLAT,SOLIDO_PHONG_GOURAUD,ROJOS,VERDES,AZULES} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);
vector<_vertex3f> vertices;
vector<_vertex3f> colores_vertices;
vector<_vertex3f> normales_caras;
vector<_vertex3f> normales_vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_colores(int modo);
void 	draw_solido_colores_vertices(int modo);
void 	draw_solido_phong_flat();
void 	draw_solido_phong_gouraud();
void 	draw(_modo modo, float r, float g, float b, float grosor);

/* asignación de colores */
void gradiente_vertical(const double r1=0, const double g1=0, const double b1=0, const double r2=1, const double g2=1, const double b2=1);
void 	colors_random();
void 	colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);
void    colors_diffuse_flat (float kr, float kg, float kb,
                             float lpx, float lpy, float lpz);
                             
void    colors_diffuse_gouraud  (float kr, float kg, float kb,
                                 float lpx, float lpy, float lpz);                            
/* calcular normales */

void    calcular_normales_caras();
void    calcular_normales_vertices();

vector<_vertex3i> caras;
vector<_vertex3f> colores_caras;
vector<_vertex3f> colores_vertices;
vector<_vertex3f> normales_caras;
vector<_vertex3f> normales_vertices;

int calculadas_normales_caras;

//material
_vertex4f ambiente;
_vertex4f difuso;
_vertex4f especular;
float brillo;

};

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

void  parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************


class _rotacion: public _triangulos3D
{
public:
       _rotacion();
       
void  parametros(vector<_vertex3f> perfil, int num,  
                 int tapa_in, int tapa_su, int tipo);
};

// tapa_in=0 sin tapa, tapa_in=1 con tapa
// tapa_su=0 sin tapa, tapa_su=1 con tapa
// tipo=0 revolucion normal
// tipo=1 esfera
// tipo=2 cono

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion: public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};


//************************************************************************
// objeto por revolución especiales
//************************************************************************

 
//************************************************************************
// cilindro
//************************************************************************

class _cilindro: public _rotacion
{
public:
       _cilindro(float radio=1.0, float altura=2.0, int num=20);
};

//************************************************************************
// cono
//************************************************************************

class _cono: public _rotacion
{
public:
       _cono(float radio=1.0, float altura=2.0, int num=12);
};

//************************************************************************
// esfera
//************************************************************************

class _esfera: public _rotacion
{
public:
       _esfera(float radio=1.0, int latitud=6, int longitud=6);
};


//************************************************************************
// rotacion archivo PLY
//************************************************************************

class _rotacion_PLY: public _rotacion
{
public:
       _rotacion_PLY();
void  parametros_PLY(char *archivo, int num);
};


//************************************************************************
// práctica 3, objeto jerárquico articulado excavadora
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

class _pala: public _triangulos3D
{
public:
      _pala(float radio=1.0, float ancho=2.0, int num=8);
};

//************************************************************************
// brazo
//************************************************************************

class _brazo: public _triangulos3D
{
public:
      _brazo();
void  draw(_modo modo, float r, float g, float b, float grosor);

float ancho;
float alto;
float fondo;

protected:
_cubo cubo;
};

//************************************************************************
// cabina
//************************************************************************

class _cabina: public _triangulos3D
{
public:
       _cabina();
void  draw(_modo modo, float r, float g, float b, float grosor);

float ancho;
float alto;
float fondo;

protected:
_cubo cubo;
};

//************************************************************************
// sustentación
//************************************************************************

class _sustentacion: public _triangulos3D
{
public:
      _sustentacion();
void  draw(_modo modo, float r, float g, float b, float grosor);
       
float ancho;
float alto;
float fondo;

float radio;

protected:
_cilindro rueda;
_cubo base;
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

class _excavadora: public _triangulos3D
{
public:
       _excavadora();
       
void  draw(_modo modo, float r, float g, float b, float grosor);

float giro_cabina;
float giro_primer_brazo;
float giro_segundo_brazo;
float giro_pala;

float giro_primer_brazo_max;
float giro_primer_brazo_min;
float giro_segundo_brazo_max;
float giro_segundo_brazo_min;
float giro_pala_max;
float giro_pala_min;

float tamanio_pala;

protected:
_pala pala;
_brazo brazo;
_cabina cabina;
_sustentacion sustentacion;
};

//************************************************************************
// atat
//************************************************************************

class _pie: public _objeto_ply{

       public:
       _pie();
};

class _tibia: public _objeto_ply{
       public:
       _tibia();

};

class _femur: public _objeto_ply{
       public:
       _femur();
};

class _cabeza: public _objeto_ply{
       public:
       _cabeza();
};

class _cuerpo: public _objeto_ply{
       public:
       _cuerpo();
};

class _torreta: public _objeto_ply{
       public:
       _torreta();
};

class _boca_canion: public _objeto_ply{
       public:
       _boca_canion();
};

class _rodilla: public _triangulos3D{
       public:
       _rodilla();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float giro_pie;

       protected:
       _pie pie;
       _tibia tibia;
};

class _pata: public _triangulos3D{
       public:
       _pata();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float giro_rodilla;
       float tamanio;


       _rodilla abajo;
       protected:
       _femur arriba;
       
};

class _cabeza_artillada: public _triangulos3D{
       public:
       _cabeza_artillada();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float giro_torretas_z;
       float giro_torreta_max;
       float retroceso1;
       float retroceso2;
       float retroceso_maximo;
       float largo_pro;
       float radio_pro;
       float dist1;
       float dist2;

       protected:
       _cilindro proyectil_izq;
       _cilindro proyectil_dch;
       _cabeza cabeza;
       _torreta izquierda;
       _torreta derecha;
       _boca_canion boca_izq;
       _boca_canion boca_dch;
};

class _atat: public _triangulos3D{
       public:
       _atat();

       void  draw(_modo modo, float r, float g, float b, float grosor);

       float giro_cabeza_z;
       float giro_cabeza_max_y;
       float giro_cabeza_max_z;
       float giro_cabeza_y;
       float giro_pata_1;
       float giro_pata_2;
       float giro_pata_3;
       float giro_pata_4;
       float giro_patas_max;

       _cabeza_artillada cabeza;
       _pata pata_1;
       _pata pata_2;
       _pata pata_3;
       _pata pata_4;

       protected:
       _cuerpo cuerpo;
       
};