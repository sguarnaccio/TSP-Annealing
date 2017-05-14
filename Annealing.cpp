// Annealing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Annealing.h"



//variables globales
int* solution; // puntero al espacio de memoria donde se va a almacenar e estado
int inicio = 1; //bandera que indica que se esta ejecutando por primera vez el lazo
int cityNum;

int main(int argc, char* argv[])
{
	int stateChange = 0, noChangesCounter = 0, executionTime = 1, stateEnergy;
	solucion bestSolution;
	double randomprob, Temp;
	

	FILE *importFile;
	char importText[1024 * 8];
	int len;
	char* delimiter;

	srand(time(NULL));

	if (argc != 2)
	{
		printf("Error: Debe pasar el archivo como parámetro\n");
		return 1;
	}
	//********** EMPIEZO A CONTAR TIEMPO DESDE ACA **********
	clock_t startTime = clock();

	importFile = fopen(argv[1], "r");
	if (!importFile)
	{
		printf("Error: No se encontró el archivo %s\n", argv[1]);
		return 0;
	}
	fgets(importText, 1024, importFile);
	delimiter = strchr(importText, ';');
	*delimiter = '\0';
	cityNum = atoi(importText);
	printf("------------------------------------------------------------\n");
	printf("Archivo %s\n", argv[1]);
	printf("------------------------------------------------------------\n\n");
#ifdef WINDOWS
	city cityArray[vectorLength];
#else
	city cityArray[cityNum];
#endif
	initializeCity(cityArray);
	fgets(importText, 1024 * 8, importFile);
	populateCity(cityArray, importText);

	solution = (int*)malloc(cityNum * sizeof(int));
	bestSolution.solution = (int*)malloc(cityNum * sizeof(int));

	//funcion que regula el descenso de la temperatura. Ha de garantizar la convergencia al minimo global, independientemente de la configuracion inicial.
	//Normalmente Temp se obtiene como una funcion del tiempo
	Temp = AJUSTE / log(1 + executionTime);


	printf("------------------------------------------------------------\n");
	printf("Generando el estado inicial\n");
	printf("------------------------------------------------------------\n\n");
	//generar estado inicial(crear una solucion)
	stategenerator();

	printf("Transicion de Estados:\n");
	for (int i = 0; i < cityNum ; i++)
	{
		printf("%d -> ", *(solution + i));
	}
	printf("%d \n", *(solution));
	printf("------------------------------------------------------------\n\n");

	// mientras haya mejores soluciones en las ultimas 1000 ejecuciones del lazo sigo ejecutando
	//caso contrario se considera que se enfrio el sistema y se sale del lazo
	while (Temp > COLD && noChangesCounter < MAX_NOCHANGE_SOLUTION && executionTime < MAX_EXEC_TIME)
	{
		
		printf("------------------------------------------------------------\n");
		printf("Calculo la Energia del estado actual\n");
		
		//calculo energia
		stateEnergy = energyCalculator(cityArray);
		printf("Energy: %d \n", stateEnergy);
		printf("------------------------------------------------------------\n\n");

		if (inicio == 1)
		{
			inicio = 0;
			bestSolution.energy = stateEnergy;
			for (int i = 0; i < cityNum; i++)
			{
				*(bestSolution.solution + i) = *(solution + i);
			}
			stateChange = 1;
		}
		else {

			printf("Best Solution Energy: %d  -  Actual State Energy: %d \n", bestSolution.energy, stateEnergy);
			printf("Temperature: %f  -  Execution Time: %d\n", Temp, executionTime);
			if (stateEnergy < bestSolution.energy)
			{
				for (int i = 0; i < cityNum; i++)
				{
					*(bestSolution.solution + i) = *(solution + i);
				}
				bestSolution.energy = stateEnergy;
				stateChange = 1;
			}
			else {

				randomprob = ((double)rand() / (RAND_MAX));
				printf("Random Probability: %f \n", randomprob);
				printf("State Probability: %f \n", exp((bestSolution.energy - stateEnergy) / Temp));
				printf("Cantidad de lazos corridos desde un ultimo cambio de estado: %d\n", noChangesCounter);
				if (exp((bestSolution.energy - stateEnergy) /  Temp) > randomprob) //probabilidad entre 0 y 1 generada aleatoriamente
				{
					for (int i = 0; i < cityNum ; i++)
					{
						*(bestSolution.solution + i) = *(solution + i);
					}
					bestSolution.energy = stateEnergy;
					stateChange = 1;
				}
			}
			printf("New Best Solution Energy: %d \n", bestSolution.energy);

		}



		if (stateChange == 0)
		{
			noChangesCounter++;// si no hubo una solucion mejor incremento el contador (empieza a enfriarse el sistema)
		}
		else {
			noChangesCounter = 0; //si hubo una solucion mejor, reinicio el contador (el sistema se mantiene caliente)
			stateChange = 0;
		}

		Temp = AJUSTE / log(1 + executionTime);
		executionTime++;

		printf("------------------------------------------------------------\n");
		printf("Generando nuevo estado\n");
		printf("------------------------------------------------------------\n\n");

		//vuelvo a generar un estado nuevo(creo nueva solucion)
		stategenerator();

		printf("Transicion de Estados:\n");
		for (int i = 0; i < cityNum ; i++)
		{
			printf("%d -> ", *(solution + i));
		}
		printf("%d \n", *(solution));
		printf("------------------------------------------------------------\n\n");
	}

	printf("Temperature: %f  -  Execution Time: %d\n", Temp, executionTime);
	//imprimo en pantalla la mejor solucion
	printf("**********************************************************************\n");
	printf("*              Mejor Transicion de Estados Obtenida                  *\n");
	printf("**********************************************************************\n");
	for (int i = 0; i < cityNum ; i++)
	{
		printf("%d -> ",*(bestSolution.solution + i));
	}
	printf("%d \n", *(bestSolution.solution));
	printf("Costo Total de la Mejor Solucion: %d", bestSolution.energy);

	free(solution);
	free(bestSolution.solution);
	return 0;
}

/*--------------------------------------------------------------------------------
* Function:    stategenerator
*
* @brief       A partir del estado(solucion) inicial, crea otros estados validos 
*			   invirtiendo la posicion de las ciudades de forma aleatoria.
*
* @param[in]	NONE
*                               
*
*
*
* @return 	NONE
--------------------------------------------------------------------------------*/
void stategenerator()
{
	int aux, swapLocation;
	if (inicio == 1)
	{
		for (int i = 0; i < cityNum; i++)
		{
			// creo una solucion que consiste en una sucesion ordenada, ascendente, de ciudades 
			*(solution + i) = i + 1;
		}
	}
	else {
		for (int i = 0; i < cityNum; i++)
		{
			//desordeno el vector creando soluciones nuevas aleatorias
			swapLocation = rand() % cityNum; //obtengo un numero aleatorio entre 0 y cityNum - 1
			aux = *(solution + i);
			*(solution + i) = *(solution + swapLocation);
			*(solution + swapLocation) = aux;
		}
	}

}

/*--------------------------------------------------------------------------------
* Function:    energyCalculator
*
* @brief       calcula la energia del estado(solucion) actual en funcion del costo 
*			   total de la solucion.
*
* @param[in]	city* cityArray	- Array donde se va a guardar info de todas las
*                                ciudades.
*
*
*
* @return 	NONE
--------------------------------------------------------------------------------*/
int energyCalculator(city* cityArray)
{
	int costo = 0;
	
	for (int i = 0; i < (cityNum - 1); i++)
	{
		// sumo los costos de mi solucion recorriendo las distancias entre las ciudades
		if ((*(solution + i) - 1) > (*(solution + i + 1) - 1))
		{
			costo += cityArray[*(solution + i) - 1].distance[*(solution + i + 1) - 1];//en cityarray los id de ciudades van de 0 a citynum - 1 y cada ciudad tiene citynum - 1 vecinos
			printf("Costo de Ciudad %d a Ciudad %d: ", *(solution + i), *(solution + i + 1));
			printf("%d \n", cityArray[*(solution + i) - 1].distance[*(solution + i + 1) - 1]);
		}
		else {
			costo += cityArray[*(solution + i) - 1].distance[*(solution + i + 1) - 2];
			printf("Costo de Ciudad %d a Ciudad %d: ", *(solution + i), *(solution + i + 1));
			printf("%d \n", cityArray[*(solution + i) - 1].distance[*(solution + i + 1) - 2]);
		}
		
	}

	if ((*(solution + (cityNum - 1)) - 1) > (*(solution)-1))
	{
		costo += cityArray[*(solution + (cityNum - 1)) - 1].distance[*(solution)-1];//sumo el costo del regreso al estado inicial
		printf("Costo de Ciudad %d a Ciudad %d: ", *(solution + (cityNum - 1)), *(solution));
		printf("%d \n", cityArray[*(solution + (cityNum - 1)) - 1].distance[*(solution)-1]);
	}
	else {
		costo += cityArray[*(solution + (cityNum - 1)) - 1].distance[*(solution)-2];//sumo el costo del regreso al estado inicial
		printf("Costo de Ciudad %d a Ciudad %d: ", *(solution + (cityNum - 1)), *(solution));
		printf("%d \n", cityArray[*(solution + (cityNum - 1)) - 1].distance[*(solution)-2]);
	}

	return costo;
}


/*--------------------------------------------------------------------------------
* Function:    initializeCity
*
* @brief       Reserva lugar en memoria del vector de ciudades y guarda su id.
*
* @param[in]	city* cityArray	- Array donde se va a guardar info de todas las
*                                ciudades.
*
*
*
* @return 	NONE
--------------------------------------------------------------------------------*/
void initializeCity(city* cityArray)
{
	for (int i = 0; i < cityNum; i++)
	{
		cityArray[i].nextCity = (int*)malloc((cityNum - 1) * sizeof(int));
		cityArray[i].distance = (int*)malloc((cityNum - 1) * sizeof(int));
		cityArray[i].id = i;
	}
	return;
}


/*--------------------------------------------------------------------------------
* Function:    populateCity
*
* @brief       Completa el array de ciudades con la informacion pasada en data.
*
* @param[in]	city* cityArray	- Array a completar con la info de todas las ciudades.
* @param[in]	int cityNum     - Cantidad de ciudades.
* @param[in]	char* data      - Puntero a inicio de informacion a guardar.
*
*
* @return 	NONE
--------------------------------------------------------------------------------*/
void populateCity(city* cityArray, char* data)
{

	int distances;
	char* delimiter;

	int connections = 0;
	for (int i = 0; i < cityNum - 1; i++)
	{
		for (int j = i + 1; j < cityNum; j++)
		{
			delimiter = strchr(data, ';');
			*delimiter = '\0';
			distances = atoi(data);
			data = delimiter + 1;
			cityArray[i].distance[j - 1] = distances;
			cityArray[i].nextCity[j - 1] = j;

			cityArray[j].distance[i] = distances;
			cityArray[j].nextCity[i] = i;

		}
	}

#ifdef CIUDADES
	printf("\n********************  Ciudades  ********************\n");
	for (int i = 0; i < cityNum; i++)
	{
		printf("\nCiudad %d:\n", cityArray[i].id + 1);
		for (int j = 0; j < cityNum - 1; j++)
		{
			printf("\t Distancia a Ciudad %d: %d\n", cityArray[i].nextCity[j] + 1, cityArray[i].distance[j]);
		}
	}
#endif //CIUDADDES

	return;

}