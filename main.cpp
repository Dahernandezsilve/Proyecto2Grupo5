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
 * tiro parabolico dependiendo de la velocity_Thread y angulo ingresado por el usuario.
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
pthread_barrier_t barrier_Time;
pthread_barrier_t barrier_InitialCalcs;
pthread_mutex_t controlVelocityTab;
string graphVelocity[11][11];
vector<string> tableVelocity={" "};

struct Properties {
    long velocidad = 0;
    long velocidadTemRealY=0;
    long distanciax = 0;
    long altura=0;
    long velocidadInicialX = 0;
    long velocidadInicialY = 0;
    long velocidadFinalX = 0;
    long velocidadFinalY = 0;
    long steps=1;
    int point =0;
    long angulo = 0;
    long tiempo = 0;
    long tiempoReal=0;
    long alcanceMaximo = 0;
    long alturaInicial = 0;
    long alturaMaxima = 0;
    long gravedad = 10;
    bool notfinish= true;
};
void preparedGraphs(){
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            graphVelocity[j][i]=" ";
            graphVelocity[0][0]=">";
        }
    }
}

void putPoint(long x, long y,string put, int type=0){
    if(type==0){
        graphVelocity[y][x]=put;
    }
}

long proportion(long data_to_proportion, long proportion){
    long proportionData = data_to_proportion/proportion;
    return proportionData;
}
void *velocity_Thread(void *values){
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->velocidadInicialX = propertie->velocidad * (cos(propertie->angulo * RAD));
    propertie->velocidadInicialY = propertie->velocidad * (sin(propertie->angulo * RAD));
    propertie->velocidadFinalX = propertie->velocidadInicialX;
    propertie->velocidadFinalY = -propertie->velocidadInicialY;
    propertie->velocidadTemRealY=propertie->velocidadInicialY;
    pthread_barrier_wait(&barrier_InitialCalcs);
    while (propertie->point<10){
        pthread_barrier_wait(&barrier_Time);
        pthread_mutex_lock(&controlVelocityTab);
        putPoint(0,propertie->point,"X");
        putPoint(propertie->point,propertie->point,"*");
        pthread_mutex_unlock(&controlVelocityTab);
        propertie->velocidadTemRealY=propertie->velocidadInicialY-(propertie->gravedad*propertie->tiempoReal);
    }
    for(auto i =0; i<tableVelocity.size();i++){
        cout<<tableVelocity[i]<<endl;
    }
    cout<<"Rutina: Velocidad"<<endl;
    cout<<"VelocidadInicialX: "<<propertie->velocidadInicialX<<endl;
    cout<<"VelocidadInicialY: "<<propertie->velocidadInicialY<<endl;
    cout<<"VelocidadFinalX: "<<propertie->velocidadFinalX<<endl;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
}
void *time_Thread(void *values){
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->tiempo = 2* propertie->velocidadInicialY / propertie->gravedad;
    propertie->steps= proportion(propertie->tiempo,10);

    pthread_barrier_wait(&barrier_InitialCalcs);
    while (propertie->point<11){
        sleep(1);
        pthread_mutex_lock(&controlVelocityTab);
        propertie->tiempoReal+=propertie->steps;
        propertie->point++;
        if(propertie->point<=10){
            putPoint(propertie->point, 0, "#");
            pthread_mutex_unlock(&controlVelocityTab);
        }
        pthread_barrier_wait(&barrier_Time);
    }

    cout<<"Rutina: Tiempo"<<endl;
    cout<<"Tiempo: "<<propertie->tiempo<<endl;
    cout<<"VelocidadFinalY: "<<propertie->velocidadFinalY<<endl;
    cout<<""<<endl;
    cout<<"Fin de subrutina"<<endl;
    cout<<""<<endl;
    pthread_exit(NULL);
}

void *graphics(void* values){
    Properties * propertie;
    propertie = (Properties*)values;
    preparedGraphs();
    pthread_barrier_wait(&barrier_InitialCalcs);
    string graphV;

    while (propertie->point<11 ){
        pthread_barrier_wait(&barrier_Time);
        pthread_mutex_lock(&controlVelocityTab);
        if(propertie->point<=10){
            cout<<"///////////Graphic Velocity in y vs time///////////"<<endl;
            cout<<"# = "<<propertie->steps<<" segundos"<<endl;
            cout<<"< = "<<propertie->velocidadInicialY<<" m/s"<<endl;
            cout<<"X = "<<10<<" m/s"<<endl;
            for (int i = propertie->point; i >=0; --i) {
                for (int j = 0; j <propertie->point ; ++j) {
                    cout<<graphVelocity[i][j];
                }
                cout<<endl;
            }
        }
        pthread_mutex_unlock(&controlVelocityTab);
    }


}

/*
void *tiempo(void *values){
    Properties * propertie;
    propertie = (Properties*)values;
    propertie->tiempo = propertie->velocidadInicialY / propertie->gravedad;
    propertie->velocidadFinalY = (propertie->velocidadInicialY) - (propertie->gravedad * propertie->tiempo);
    cout<<"Rutina: Tiempo"<<endl;
    cout<<"Tiempo: "<<propertie->time_Thread<<endl;
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
    pthread_barrier_wait(&barrier_InitialCalcs);
    cout<<"Estas en el punto: ("<<propertie->distanciax<<","<<propertie->altura<<")"<<endl;
    while (propertie->distanciax!=propertie->alcanceMaximo){
        pthread_barrier_wait(&barrier_Time);
        propertie->distanciax=propertie->velocidadInicialX*propertie->tiempoReal;
        propertie->altura=propertie->velocidadInicialY*propertie->tiempoReal-(propertie->gravedad* pow(propertie->tiempoReal,2)/2);
       // cout<<"Estas en el punto: ("<<propertie->distanciax<<","<<propertie->altura<<")"<<endl;

    }
    cout<<"Rutina: Alcance"<<endl;
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
    cout<<"| Para comenzar, ingrese el valor de velocity_Thread inicial al que desea lanzar su objeto:              |"<<endl;
    cin>>properties.velocidad;
    cout<<"| A que angulo desea lanzar su objeto:                                                             |"<<endl;
    cin>>properties.angulo;

    pthread_t th;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_barrier_init(&barrier_Time, nullptr, 4);
    pthread_barrier_init(&barrier_InitialCalcs, nullptr, 4);
    if (pthread_mutex_init(&controlVelocityTab, nullptr) != 0)//inicializacion de mutex no completada
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    for(int i=0; i<THREADS; i++){
        pthread_create(&th, &attr, velocity_Thread, (void *) &properties);
        pthread_create(&th,&attr,alcance,(void *)&properties);
        pthread_create(&th, &attr, time_Thread, (void *) &properties);
        pthread_create(&th, &attr, graphics,(void *)&properties);
        pthread_join(th, nullptr);
    }
    return 0;
}
