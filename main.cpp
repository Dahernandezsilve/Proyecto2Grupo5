#include <iostream>

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
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS 1
#define PI 3.141592654
#include <vector>
#define RAD PI / 180
using namespace std;
pthread_barrier_t barreraTiempo;
pthread_mutex_t controlVelocityTab;

struct Properties {
    long velocidad = 0;
    long velocidadTemRealY=0;
    long velocidadTemReal=0;
    long distanciax = 0;
    long altura=0;
    long velocidadInicialX = 0;
    long velocidadInicialY = 0;
    long velocidadFinalX = 0;
    vector<string> tableVelocity={""};
    long velocidadFinalY = 0;
    long angulo = 0;
    long tiempo = 0;
    int tiempoReal=0;
    long alcanceMaximo = 0;
    long alturaInicial = 0;
    long alturaMaxima = 0;
    long gravedad = 10;
};

void *velocidad(void *values){
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->velocidadInicialX = propertie->velocidad * (cos(propertie->angulo * RAD));
    propertie->velocidadInicialY = propertie->velocidad * (sin(propertie->angulo * RAD));
    propertie->velocidadFinalX = propertie->velocidadInicialX;
    propertie->velocidadFinalY = -propertie->velocidadInicialY;
    propertie->velocidadTemRealY=propertie->velocidadInicialY;
    while (propertie->velocidadTemRealY!=propertie->velocidadFinalY){
        pthread_barrier_wait(&barreraTiempo);
        propertie->velocidadTemRealY=propertie->velocidadInicialY-(propertie->gravedad*propertie->tiempoReal);
        cout<<"Velocidad instantanea: "<<propertie->velocidadTemRealY<<" m/s"<<endl;
        propertie->velocidadTemReal = sqrt(pow(propertie->velocidadTemRealY,2) + pow(propertie->velocidadInicialX,2));
        cout<<"Velocidad general: "<<propertie->velocidadTemReal<<" m/s"<<endl;
        /*pthread_mutex_lock(&controlVelocityTab);
        if(propertie->velocidadTemRealY/10 >propertie->tableVelocity.size()){
            int o = propertie->tableVelocity.size();
            int i = propertie->velocidadTemRealY/10 - propertie->tableVelocity.size();
            propertie->tableVelocity.resize(i);
            for(o;o<i;o++){
                propertie->tableVelocity[o]="#"+' '*(propertie->tiempoReal-1)+'*';
            }

        } else{
            int position= propertie->velocidadTemRealY;
            propertie->tableVelocity[position][propertie->tiempoReal-1]='*';

        }
        pthread_mutex_unlock(&controlVelocityTab);
         */
    }
    for(auto i =0; i<propertie->tableVelocity.size();i++){
        cout<<propertie->tableVelocity[i]<<endl;
    }
    cout<<"Rutina: Velocidad"<<endl;
    cout<<"VelocidadInicialX: "<<propertie->velocidadInicialX<<" m/s"<<endl;
    cout<<"VelocidadInicialY: "<<propertie->velocidadInicialY<<" m/s"<<endl;
    cout<<"VelocidadFinalX: "<<propertie->velocidadFinalX<<" m/s"<<endl;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
}
void *tiempo(void *values){
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->tiempo = 2* propertie->velocidadInicialY / propertie->gravedad;

    while (propertie->tiempoReal<propertie->tiempo){
        propertie->tiempoReal++;


        sleep(1);
        pthread_barrier_wait(&barreraTiempo);
        cout<<"tiempo en este instante: "<<propertie->tiempoReal<<" segundos"<<endl;
        pthread_mutex_lock(&controlVelocityTab);
        for(auto i=0; i<propertie->tableVelocity.size();i++){
            if(i==0){
                propertie->tableVelocity[i]+="#";
                cout<<propertie->tableVelocity[0]<<endl;
            } else{
                propertie->tableVelocity[i]+=" ";
            }
        }
        pthread_mutex_unlock(&controlVelocityTab);

    }
    cout<<"Rutina: Tiempo"<<endl;
    cout<<"Tiempo: "<<propertie->tiempo<<" segundos"<<endl;
    cout<<"VelocidadFinalY: "<<propertie->velocidadFinalY<<" m/s"<<endl;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
    return NULL;
}

/*
void *tiempo(void *values){
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->tiempo = propertie->velocidadInicialY / propertie->gravedad;
    propertie->velocidadFinalY = (propertie->velocidadInicialY) - (propertie->gravedad * propertie->tiempo);
    cout<<"Rutina: Tiempo"<<endl;
    cout<<"Tiempo: "<<propertie->tiempo<<endl;
    cout<<"VelocidadFinalY: "<<propertie->velocidadFinalY<<endl;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
}*/



void *alcance(void *values){
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->alcanceMaximo = ((pow(propertie->velocidad,2))/propertie->gravedad) * (sin(2 * (propertie->angulo * RAD)));
    propertie->alturaMaxima = (pow((propertie->velocidad * sin(propertie->angulo * RAD)),2))/ (2 * propertie->gravedad);
    cout<<"Estas en el punto: ("<<propertie->distanciax<<","<<propertie->altura<<")"<<endl;
    while (propertie->distanciax!=propertie->alcanceMaximo){
        pthread_barrier_wait(&barreraTiempo);
        propertie->distanciax=propertie->velocidadInicialX*propertie->tiempoReal;
        propertie->altura=propertie->velocidadInicialY*propertie->tiempoReal-(propertie->gravedad* pow(propertie->tiempoReal,2)/2);
        cout<<"Estas en el punto: ("<<propertie->distanciax<<","<<propertie->altura<<")"<<endl;

    }
    cout<<"Rutina: Alcance"<<endl;
    cout<<"Alcance Maximo: "<<propertie->alcanceMaximo<<" metros"<<endl;
    cout<<"Altura Maxina: "<<propertie->alturaMaxima  <<" metros"<<endl;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
}

int main() {
    string p;
    p[100];
    p[0]=' ';
    Properties properties;
    cout<<"---------------------------------- Simulador de fenomenos fisicos ----------------------------------"<<endl;
    cout<<"| Este programa le permitira observar el comportamiento del tiro parabolico.                       |"<<endl;
    cout<<"| Para comenzar, ingrese el valor de velocidad inicial al que desea lanzar su objeto:              |"<<endl;
    cin>>properties.velocidad;
    cout<<"| A que angulo desea lanzar su objeto?:                                                             |"<<endl;
    cin>>properties.angulo;

    pthread_t th;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_barrier_init(&barreraTiempo, NULL, 3);
    if (pthread_mutex_init(&controlVelocityTab, NULL) != 0)//inicializacion de mutex no completada
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    for(int i=0; i<THREADS; i++){
        pthread_create(&th,&attr,velocidad,(void *)&properties);
        pthread_create(&th,&attr,alcance,(void *)&properties);
        pthread_create(&th,&attr,tiempo,(void *)&properties);
        pthread_join(th, NULL);
    }
    return 0;
}
