/*********************************************************************************
 * Proyecto 2 | Grupo 5. Simulador de fenómenos físicos (tiro parabólico)
 * 1.Integrantes:
 * - Diego Alexander Hernández Silvestre | 21270
 * - Kristopher Javier Alvarado Lopez | 21188
 * - Linda Inés Jiménez Vides | 21169
 * - Daniel Armando Valdez Reyes | 21240
 * main.cpp
 * 2.Catedrático: Roger Díaz
 * 3.Descripción: Software encargado de mostrar graficamente el comportamiento del
 * tiro parabolico dependiendo de la velocidad y angulo ingresado por el usuario.
 * 4.Fecha: 06/10/2022
 * Version: 1.0.0
 * *********************************************************************************/

#include <iostream>
#include <cmath>

#define THREADS 1
using namespace std;

struct Properties {
    long velocidad = 0;
    long distancia = 0;
    long velocidadInicialX = 0;
    long velocidadInicialY = 0;
    long velocidadFinalX = 0;
    long velocidadFinalY = 0;
    long angulo = 0;
    long tiempo = 0;
    long alcanceInicial = 0;
    long alcanceMaximo = 0;
    long alturaInicial = 0;
    long alturaMaxima = 0;
    long gravedad = 10;
};

void *velocidad(void *values){
    cout<<"Rutina: Velocidad"<<endl;
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->velocidadInicialX = propertie->velocidad * cos(propertie->angulo);
    propertie->velocidadInicialY = propertie->velocidad * sin(propertie->angulo);
    propertie->velocidadFinalY = propertie->velocidadInicialY - propertie->gravedad * propertie->tiempo;
    propertie->velocidadFinalX = propertie->velocidadInicialX;
    cout<<"VelocidadInicialX: "<<propertie->velocidadInicialX<<endl;
    cout<<"VelocidadInicialY: "<<propertie->velocidadInicialY<<endl;
    cout<<"Valor: ", +cos(5);
    cout<<"VelocidadFinalX: "<<propertie->velocidadFinalX<<endl;
    cout<<"VelocidadFinalY: "<<propertie->velocidadFinalY<<endl;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
}

void *tiempo(void *values){
    cout<<"Rutina: Tiempo"<<endl;
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->tiempo = propertie->velocidadInicialY/propertie->gravedad;
    cout<<"Tiempo: "<<propertie->tiempo;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
}
void *alcance(void *values){
    cout<<"Rutina: Alcance"<<endl;
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->alcanceMaximo = (pow((propertie->velocidad),2)*sin(2*(propertie->angulo)))/propertie->gravedad;
    propertie->alturaMaxima = (pow(propertie->velocidad,2)* pow(sin(propertie->angulo),2))/(2*propertie->gravedad);
    cout<<"Alcance Maximo: "<<propertie->alcanceMaximo<<endl;
    cout<<"Altura Maxina: "<<propertie->alturaMaxima  <<endl;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
}


int main() {
    Properties properties;
    cout<<"---------------------------------- Simulador de fenomenos fisicos ----------------------------------"<<endl;
    cout<<"| Este programa le permitira observar el comportamiento del tiro parabolico.                       |"<<endl;
    cout<<"| Para comenzar, ingrese el valor de velocidad inicial al que desea lanzar su objeto:              |"<<endl;
    cin>>properties.velocidad;
    cout<<"| A que angulo desea lanzar su objeto:                                                             |"<<endl;
    cin>>properties.angulo;

    pthread_t th;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&th,&attr,velocidad,(void *)&properties);
    pthread_create(&th,&attr,alcance,(void *)&properties);
    pthread_create(&th,&attr,tiempo,(void *)&properties);

    //for (int i = 0; i = THREADS;i++ ){
    //    pthread_join(th, NULL);
    //}

    return 0;
}
