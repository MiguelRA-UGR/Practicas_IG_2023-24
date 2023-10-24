//**************************************************************************
// Práctica 2 
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>


const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS,ROJOS,VERDES,AZULES} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
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
/*************
	0 --> metodo asignacion color
	1 --> mascara rojo
	2 --> máscara verde
	3 --> máscara azul
***************/

void 	draw(_modo modo, float r, float g, float b, float grosor);

/* asignación de colores */
void gradiente_vertical(vector<_vertex3f>* vertices=nullptr, int vueltas=NULL);
void 	colors_random();
void 	colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);

vector<_vertex3i> caras;
vector<_vertex3f> colores_caras;
};


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
       _cilindro(float radio=1.0, float altura=2.0, int num=12);
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