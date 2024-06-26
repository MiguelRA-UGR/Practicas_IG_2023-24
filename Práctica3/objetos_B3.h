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
vector<_vertex3f> colores_vertices;
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
void 	draw(_modo modo, float r, float g, float b, float grosor);

/* asignación de colores */
void gradiente_vertical(vector<_vertex3f>* vertices=nullptr, int vueltas=NULL);
void 	colors_random();
void 	colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);


vector<_vertex3i> caras;
vector<_vertex3f> colores_caras;
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

class _pie: public _triangulos3D{
       //por rotacion
       public:
       _pie();
};

class _pata: public _triangulos3D{
       public:
       _pata();
       protected:
       _cubo cuerpo_superior;
       _cubo cuerpo_inferior;
       _cilindro articulacion_superior;
       _cilindro articulacion_inferior;
       _pie pie;
};

class _torreta: public _triangulos3D{
       public:
       _torreta();
       protected:
       _esfera montura_torreta;
       _cubo base_torreta;
       _cilindro cañon;
};

class _cabina_mando: public _triangulos3D{
       public:
       _cabina_mando();
};

class _cabeza: public _triangulos3D{
       public:
       _cabeza();

       protected:
       _cabina_mando cabina;
       _torreta torreta_izquierda;
       _torreta torreta_derecha;
       _cilindro cañon_derecho;
       _cilindro cañon_izquierdo;

};

class _modulo_base: public _triangulos3D
{
public:

	_modulo_base(float tam=0.5);
};

class _modulo_central: public _triangulos3D
{
public:

	_modulo_central(float tam=0.5);
};

class _cuerpo: public _triangulos3D{
       public:
       _cuerpo();
       

       protected:
       _cilindro cuello;
       _modulo_base delantero;
       _modulo_central central;
       _modulo_base trasero;
       _cubo base;

};

class _atat: public _triangulos3D
{
       public:
              _atat();
              
       void  draw(_modo modo, float r, float g, float b, float grosor);

       protected:
       _cuerpo cuerpo;
       _cabeza cabeza;
       _pata delantera_dcha;
       _pata delantera_izqda;
       _pata trasera_dcha;
       _pata trasera_izqda;
};
