//**************************************************************************
// Práctica 3 
//**************************************************************************

#include "objetos_B3.h"
#include "file_ply_stl.hpp"
#include <time.h> 
#include <set>


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPointSize(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawArrays(GL_POINTS,0,vertices.size()); 

/*int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();*/
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawElements(GL_TRIANGLES,caras.size()*3,GL_UNSIGNED_INT,&caras[0]);

/*int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();

}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores(int modo)
{ int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);

  switch(modo){
    case 0:
       for (i=0;i<caras.size();i++){
        glColor3f(colores_caras[i].r,colores_caras[i].g,colores_caras[i].b);
        glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
      }
    break;

    case 1:
      for (i=0;i<caras.size();i++){
        glColor3f(colores_caras[i].r,colores_caras[i].g*0.2,colores_caras[i].b*0.1);
        glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
      }
    break;

    case 2:
      for (i=0;i<caras.size();i++){
        glColor3f(colores_caras[i].r*0.1,colores_caras[i].g,colores_caras[i].b*0.2);
        glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
      }
    break;

    case 3:
      for (i=0;i<caras.size();i++){
        glColor3f(colores_caras[i].r*0.1,colores_caras[i].g*0.2,colores_caras[i].b);
        glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
        glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
      }
    break;
  }
glEnd();
}


//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r, g, b, grosor);break;
	case EDGES:draw_aristas(r, g, b, grosor);break;
	case SOLID:draw_solido(r, g, b);break;
	case SOLID_COLORS:draw_solido_colores(0);break;
  case ROJOS:draw_solido_colores(1);break;
  case VERDES:draw_solido_colores(2);break;
  case AZULES:draw_solido_colores(3);break;
	}
}

//*************************************************************************
// asignación colores
//*************************************************************************


void _triangulos3D::gradiente_vertical(vector<_vertex3f>* perfil, int vueltas){
  bool es_objeto_revolucion= (perfil==nullptr||vueltas==0) ? false:true;
  colores_caras.resize(caras.size());
  double min_y = INFINITY;
  double max_y = -INFINITY;
  set <double> valores_y;
  int num_niveles=0;

  //Si no es un objeto de revolución calculamos la cantidad de niveles según el valor y de los vértices
  if(!es_objeto_revolucion){
    for(int i=0; i < vertices.size(); i++){
      if(valores_y.find(vertices[i].y)==valores_y.end()){
        valores_y.insert(vertices[i].y);
      }
    }

    num_niveles=valores_y.size();
  }

  else{
    //Calculamos los niveles como la cantidad de puntos menos el último y primero(tapas)
    num_niveles=((perfil->size())*2)-3;
  }

  min_y=0;
  max_y=num_niveles;

  double valores_gradiente[num_niveles];

  for(int i=0; i < num_niveles; i++){
    //Calculamos los valores normalizando entre 0 y 1
    valores_gradiente[i] = (i-min_y)/(max_y-min_y);
  }

  for(int i=0; i < caras.size(); i++){
    for(int j=0; j < num_niveles; j++){
      colores_caras[i].r=valores_gradiente[j];
      colores_caras[i].g=valores_gradiente[j];
      colores_caras[i].b=valores_gradiente[j];

      if(i < caras.size()-1){
        i++;
        colores_caras[i].r=valores_gradiente[j];
        colores_caras[i].g=valores_gradiente[j];
        colores_caras[i].b=valores_gradiente[j];
      }
    }

    //Tratamiento de las tapas
    if(es_objeto_revolucion){
      //tapa de arriba
      for(int i=caras.size()-vueltas-1; i < caras.size(); i++){
        colores_caras[i].r=valores_gradiente[num_niveles-1];
        colores_caras[i].g=valores_gradiente[num_niveles-1];
        colores_caras[i].b=valores_gradiente[num_niveles-1];
      }

      //tapa de abajo
      for(int i=caras.size()-2*vueltas; i < caras.size()-vueltas; i++){
        colores_caras[i].r=valores_gradiente[0];
        colores_caras[i].g=valores_gradiente[0];
        colores_caras[i].b=valores_gradiente[0];
      }
    }
  }
}

void _triangulos3D::colors_random()
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
srand (time(NULL));
for (i=0;i<n_c;i++)  
  {colores_caras[i].r=rand()%1000/1000.0;
   colores_caras[i].g=rand()%1000/1000.0;
   colores_caras[i].b=rand()%1000/1000.0;
  }
}

//*************************************************************************

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
for (i=0;i<n_c;i++)  
  {if (i%2==0) 
     {colores_caras[i].r=r1;
      colores_caras[i].g=g1;
      colores_caras[i].b=b1;
     }
   else 
     {colores_caras[i].r=r2;
      colores_caras[i].g=g2;
      colores_caras[i].b=b2;
     } 
  }
}


//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
caras[11]._0=1;caras[11]._1=5;caras[11]._2=4; 

//colores de las caras
colors_random();
}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{
int i;
//vertices 
vertices.resize(5); 
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;

//colores de las caras
colors_random();
}

//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
 
}



void _objeto_ply::parametros(char *archivo)
{
int i, n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;
 
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

//printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

vertices.resize(n_ver);
caras.resize(n_car);
// vértices

for (i=0;i<n_ver;i++)
  {vertices[i].x=ver_ply[3*i];
   vertices[i].y=ver_ply[3*i+1];
   vertices[i].z=ver_ply[3*i+2];
  }

// vértices
for (i=0;i<n_car;i++)
  {caras[i].x=car_ply[3*i];
   caras[i].y=car_ply[3*i+1];
   caras[i].z=car_ply[3*i+2];
  }

// colores
colors_random();
}


//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, 
                           int tapa_in, int tapa_su, int tipo)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;
float radio;

// tratamiento de los vértice
radio=sqrt(perfil[0].x*perfil[0].x+perfil[0].y*perfil[0].y);

num_aux=perfil.size();
if (tipo==2) num_aux=1;
vertices.resize(num_aux*num);

for (j=0;j<num;j++)
  {for (i=0;i<num_aux;i++)
     {
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
     }
  }

// tratamiento de las caras 
if (tipo<2)
{
for (j=0;j<num;j++)
  {for (i=0;i<num_aux-1;i++)
     {cara_aux._0=i+j*num_aux;
      cara_aux._1=i+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);

      cara_aux._0=i+((j+1)%num)*num_aux;
      cara_aux._1=i+1+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);
     }
   }
}
 
// tapa inferior
if (tapa_in==1)
{
  // punto central de la tapa
  vertice_aux.x=0.0;
  if (tipo==1) vertice_aux.y=-radio;
  else vertice_aux.y=perfil[0].y;
  vertice_aux.z=0.0;
  vertices.push_back(vertice_aux);
  //caras tapa inferior

  cara_aux._0=num_aux*num;
  for (j=0;j<num;j++)
    {
     cara_aux._1=((j+1)%num)*num_aux;
     cara_aux._2=j*num_aux;
     caras.push_back(cara_aux);
    }

}
 
// tapa superior

if (tapa_su==1)
{
  // punto central de la tapa
  vertice_aux.x=0.0;
  vertice_aux.y=perfil[num_aux-1].y;
  if (tipo==1) vertice_aux.y=radio;
  if (tipo==2) vertice_aux.y=perfil[1].y;
  vertice_aux.z=0.0;
  vertices.push_back(vertice_aux);

  //caras tapa superior
  if (tapa_in==1) cara_aux._0=num_aux*num+1;
  else cara_aux._0=num_aux*num;

  for (j=0;j<num;j++)
    {cara_aux._1=j*num_aux+num_aux-1;
     cara_aux._2=((j+1)%num)*num_aux+num_aux-1;
     caras.push_back(cara_aux);
    }
}

//colores de las caras
//colors_random();
gradiente_vertical(&perfil,num);
}


//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
int i;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

// tratamiento de los vértice

num_aux=poligono.size();
vertices.resize(num_aux*2);
for (i=0;i<num_aux;i++)
    {
      vertices[2*i]=poligono[i];
      vertices[2*i+1].x=poligono[i].x+x;
      vertices[2*i+1].y=poligono[i].y+y;
      vertices[2*i+1].z=poligono[i].z+z;
    }
    
// tratamiento de las caras 

caras.resize(num_aux*2);
int c=0;
for (i=0;i<num_aux;i++)         
  {
   caras[c]._0=i*2;
   caras[c]._1=(i*2+2)%(num_aux*2);
   caras[c]._2=i*2+1;    
   c=c+1;
   caras[c]._0=(i*2+2)%(num_aux*2);
   caras[c]._1=(i*2+2)%(num_aux*2)+1;
   caras[c]._2=i*2+1;    
   c=c+1;    
   }  
   
//colores de las caras
colors_random();
}

//************************************************************************
//************************************************************************

//************************************************************************
// objeto cilindro (caso especial de rotacion)
//************************************************************************

_cilindro::_cilindro(float radio, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio; aux.y=-altura/2.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=radio; aux.y=altura/2.0; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,1,1,0);
}

//************************************************************************
// objeto cono (caso especial de rotacion)
//************************************************************************

_cono::_cono(float radio, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio; aux.y=0; aux.z=0.0;
perfil.push_back(aux);
aux.x=0.0; aux.y=altura; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,1,1,2);
}

//************************************************************************
// objeto esfera (caso especial de rotacion)
//************************************************************************

_esfera::_esfera(float radio, int latitud, int longitud)
{
vector<_vertex3f> perfil;
_vertex3f aux;
int i;
for (i=1;i<latitud;i++)
  {aux.x=radio*cos(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.y=radio*sin(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.z=0.0;
   perfil.push_back(aux);
  }
parametros(perfil,longitud,1,1,1);
}


//************************************************************************
// rotacion archivo PLY (caso especial de rotacion)
//************************************************************************

_rotacion_PLY::_rotacion_PLY()
{

}

void _rotacion_PLY::parametros_PLY(char *archivo, int num)
{
int i, n_ver;

  vector<float> ver_ply;
  vector<int> car_ply;

  _file_ply::read(archivo, ver_ply,car_ply);

  n_ver=ver_ply.size()/3;

  printf("Number of vertices=d\n", n_ver);

  vector<_vertex3f> perfil;

  for(i=0; i<n_ver;i++){
    _vertex3f aux;
    aux.x = ver_ply[3*i];
    aux.y = ver_ply[3*i + 1];
    aux.z = ver_ply[3*i + 2];
    perfil.push_back(aux);
  }

  parametros(perfil, num, 1,1,0);
}

//************************************************************************
// práctica 3, objeto jerárquico articulado
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

_pala::_pala(float radio, float ancho, int num)
{
vector<_vertex3f> perfil;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int i, j;

vertice_aux.x=radio; vertice_aux.y=0; vertice_aux.z=-ancho/2.0;
perfil.push_back(vertice_aux);
vertice_aux.z=ancho/2.0;
perfil.push_back(vertice_aux);

// tratamiento de los vértices

for (j=0;j<=num;j++)
  {for (i=0;i<2;i++)	
     {
      vertice_aux.x=perfil[i].x*cos(M_PI*j/(1.0*num))-
                    perfil[i].y*sin(M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].x*sin(M_PI*j/(1.0*num))+
                    perfil[i].y*cos(M_PI*j/(1.0*num));
      vertice_aux.z=perfil[i].z;
      vertices.push_back(vertice_aux);
     }
  }
  
// tratamiento de las caras 

for (j=0;j<num;j++)
   {cara_aux._0=j*2;
    cara_aux._1=(j+1)*2;
    cara_aux._2=(j+1)*2+1;
    caras.push_back(cara_aux);
    
    cara_aux._0=j*2;
    cara_aux._1=(j+1)*2+1;
    cara_aux._2=j*2+1;
    caras.push_back(cara_aux);
   }
   
// tapa inferior
vertice_aux.x=0;
vertice_aux.y=0;
vertice_aux.z=-ancho/2.0;
vertices.push_back(vertice_aux); 

for (j=0;j<num;j++)
   {cara_aux._0=j*2;
    cara_aux._1=(j+1)*2;
    cara_aux._2=vertices.size()-1;
    caras.push_back(cara_aux);
   }
  
// tapa superior
vertice_aux.x=0;
vertice_aux.y=0;
vertice_aux.z=ancho/2.0;
vertices.push_back(vertice_aux); 

for (j=0;j<num;j++)
   {cara_aux._0=j*2+1;
    cara_aux._1=(j+1)*2+1;
    cara_aux._2=vertices.size()-1;
    caras.push_back(cara_aux);
   }
  
colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
}

//************************************************************************
// brazo
//************************************************************************

_brazo::_brazo()
{
ancho=0.6;
alto=0.1;
fondo=0.1;
colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _brazo::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glScalef(ancho, alto, fondo);
glTranslatef(0.5,0,0);
cubo.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// cabina
//************************************************************************

_cabina::_cabina()
{
ancho=0.4;
alto=0.6;
fondo=0.4;
cubo.colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _cabina::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glScalef(ancho, alto, fondo);
cubo.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// sustentación
//************************************************************************

_sustentacion::_sustentacion()
{
ancho=1.2;
alto=0.3;
fondo=0.8;
radio=0.15;
base.colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _sustentacion::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glTranslatef(2*ancho/6,-alto/2.0,0);
glRotatef(90,1,0,0);
glScalef(radio, fondo/2.2, radio);
rueda.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-2*ancho/6,-alto/2.0,0);
glRotatef(90,1,0,0);
glScalef(radio, fondo/2.2, radio);
rueda.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glScalef(ancho, alto, fondo);
base.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

_excavadora::_excavadora()
{
giro_cabina = 0.0;
giro_primer_brazo = 0.0;
giro_primer_brazo_max = 0;
giro_primer_brazo_min = -90;
giro_segundo_brazo = 0.0;
giro_segundo_brazo_max = 30;
giro_segundo_brazo_min = 0;
giro_pala = 0.0;
giro_pala_max = 50.0;
giro_pala_min = -90.0;

tamanio_pala=0.15;
};


void _excavadora::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();

sustentacion.draw(modo, r, g, b, grosor);

glTranslatef(0,(cabina.alto+sustentacion.alto)/2.0,0);
glRotatef(giro_cabina,0,1,0);
cabina.draw(modo, r, g, b, grosor);

glTranslatef(cabina.ancho/2.0,0,0);
glRotatef(giro_segundo_brazo,0,0,1);
brazo.draw(modo, r, g, b, grosor);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_primer_brazo,0,0,1);
brazo.draw(modo, r, g, b, grosor);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_pala,0,0,1);
glTranslatef(tamanio_pala,0,0);
glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
pala.draw(modo, r, g, b, grosor);

glPopMatrix();
};

//************************************************************************
// atat
//************************************************************************

_modulo_base::_modulo_base(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-tam*1.5;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam*1.5;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam*0.8;vertices[2].y=tam*2;vertices[2].z=tam;
vertices[3].x=-tam*0.8;vertices[3].y=tam*2;vertices[3].z=tam;
vertices[4].x=-tam*2;vertices[4].y=-tam;vertices[4].z=-tam*2;
vertices[5].x=tam*2;vertices[5].y=-tam;vertices[5].z=-tam*2;
vertices[6].x=tam;vertices[6].y=tam*3.5;vertices[6].z=-tam*2;
vertices[7].x=-tam;vertices[7].y=tam*3.5;vertices[7].z=-tam*2;

// triangulos
caras.resize(10);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
caras[4]._0=0;caras[4]._1=7;caras[4]._2=4;
caras[5]._0=0;caras[5]._1=3;caras[5]._2=7;
caras[6]._0=3;caras[6]._1=2;caras[6]._2=7;
caras[7]._0=2;caras[7]._1=6;caras[7]._2=7;
caras[8]._0=0;caras[8]._1=1;caras[8]._2=4;
caras[9]._0=1;caras[9]._1=5;caras[9]._2=4; 
colors_random();
}

_modulo_central::_modulo_central(float tam)
{
  //vertices
  vertices.resize(8);
vertices[0].x=-tam*2;vertices[0].y=-tam;vertices[0].z=-tam*2;
vertices[1].x=tam*2;vertices[1].y=-tam;vertices[1].z=-tam*2;
vertices[2].x=-tam*1.2;vertices[2].y=tam*4;vertices[2].z=-tam*2;
vertices[3].x=tam*1.2;vertices[3].y=tam*4;vertices[3].z=-tam*2;

vertices[4].x=-tam*2;vertices[4].y=-tam;vertices[4].z=-tam*6;
vertices[5].x=tam*2;vertices[5].y=-tam;vertices[5].z=-tam*6;
vertices[6].x=-tam*1.2;vertices[6].y=tam*4;vertices[6].z=-tam*6;
vertices[7].x=tam*1.2;vertices[7].y=tam*4;vertices[7].z=-tam*6;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=2;caras[0]._2=3;
caras[1]._0=0;caras[1]._1=1;caras[1]._2=3;
caras[2]._0=1;caras[2]._1=3;caras[2]._2=7;
caras[3]._0=1;caras[3]._1=5;caras[3]._2=7;
caras[4]._0=2;caras[4]._1=3;caras[4]._2=7;
caras[5]._0=2;caras[5]._1=6;caras[5]._2=7;
caras[6]._0=4;caras[6]._1=2;caras[6]._2=6;
caras[7]._0=0;caras[7]._1=4;caras[7]._2=2;
caras[8]._0=4;caras[8]._1=5;caras[8]._2=7;
caras[9]._0=4;caras[9]._1=7;caras[9]._2=6;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
caras[11]._0=4;caras[11]._1=1;caras[11]._2=5;



colors_random();
}