#pragma once
/*
* Annealing.h
*
*  Created on: April 1, 2017
*  Authors: Amaro Nicolas, Hernando Sebastian, Wajs Ezequiel
*/

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* Defines */


#define CIUDADES //Imprime las distancias de cada ciudad a las demas
#define WINDOWS //Para compilar en entornos sin soporte para VLAs (arrays de long variables)

#define vectorLength  20 //Longitud para vectores de longitud fija

#define AJUSTE 20 //constante que gobierna la formula que regula el descenso de la temperatura. Se debe obtener experimentalmente. Cambiar este valor impactara en el calculo de la probabilidad del estado y tambien nos obligara a cambiar el umbral donde la temperatura se considera fria
#define MAX_EXEC_TIME 5000 //cantidad de lazos que se corren como maximo antes de cortar el while loop
#define MAX_NOCHANGE_SOLUTION 200 //Maxima cantidad de veces que se puede ejecutar el while loop sin que haya un cambio en la mejor solucion
#define COLD 2.4 //temperatura en la cual considero que el proceso de annealing se enfrio y se termina el bucle

/* Variable type Declarations */
typedef struct {
	int id;           // ID de la ciudad
	int* nextCity;    // Puntero al vector de ciudades conectadas
	int* distance;    // Puntero al vector con distancia entre ciudades conectadas
}city;

typedef struct {
	int energy;		//energia de la mejor solucion
	int* solution;	//estado de la mejor solucion
}solucion;


/* Functions Declarations */
void initializeCity(city* cityArray);
void populateCity(city* cityArray, char* data);
//void freeMemory(listNode*a);
void stategenerator(void);
int energyCalculator(city* cityArray);