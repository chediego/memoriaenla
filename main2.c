#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXAPELL "ZZZ"
#define MINAPELL	"aaa"
#define MAXRUT 99999999
#define MINRUT 11111111
#define MAXSUELDO 9999999
#define MINSUELDO 0
#define MAXARCH 1
#define LARGONOMB 25
#define SALTOLINEA 67
#define SALTO 37
struct nodo
{	
	char rut[11];
	char nombre[11];
	char apellido[11];
	int sueldo;
	int pos;//posicion donde se encuentra el trabajador en el archivo
	struct nodo *sgte[2]; // 0:apellidos 1:sueldos
};

void Cerrar_Archivos(FILE **idarchivos, int nroarchivos);
int Comprobar_Existencia_Archivos(FILE **idarchivos, char nombres[MAXARCH][LARGONOMB], int nroarchivos);
struct nodo *Inicializa_Lista(char *valor1, char *valor2, int valor3, int valor4);
void Cargar_Datos(FILE *idarchivo, struct nodo *inicio);
//void Imprimir(struct nodo *inicio,int i);
void Imprimir_V2(struct nodo *inicio,int i , FILE *idarchivo);
struct nodo *Liberar_Lista(struct nodo *inicio);

main()
{
	FILE *archivos[MAXARCH];
	struct nodo *cabecera;
	char nombresarchivos[MAXARCH][LARGONOMB] = {"empleados.dat"};

	if (Comprobar_Existencia_Archivos(archivos, nombresarchivos, 1))
	{
		cabecera = Inicializa_Lista(MINAPELL, MAXAPELL, MINSUELDO,MAXSUELDO);
		Cargar_Datos(archivos[0], cabecera);
	  	Imprimir_V2(cabecera,0,archivos[0]);
	  	printf("\n\n\n");
	   	Imprimir_V2(cabecera,1,archivos[0]);
	   	cabecera = Liberar_Lista(cabecera);
	}
	else
	{

	}
	
}

void Cerrar_Archivos(FILE **idarchivos, int nroarchivos)
{	int i;
	for(i=0; i<nroarchivos; i++)
	{	fclose(idarchivos[i]);
	}
}

int Comprobar_Existencia_Archivos(FILE **idarchivos, char nombres[MAXARCH][LARGONOMB], int nroarchivos)
{	int i;
	i=0;
	while((i<nroarchivos)&& ((idarchivos[i]=fopen(nombres[i], "r"))!=NULL))
	{	i++;
	};
	if(i == nroarchivos)
	{	return(1);
	}
	else
	{	printf("El archivo %s no existe!\n",nombres[i]);
		Cerrar_Archivos(idarchivos, i);
		return(0);
	}
}
struct nodo *Inicializa_Lista(char *valor1, char *valor2, int valor3, int valor4)
{	struct nodo *auxtemp;

	auxtemp = malloc(sizeof(struct nodo));

	strcpy(auxtemp->apellido, valor1);
	auxtemp->sueldo = valor4;


	auxtemp->sgte[0] = malloc(sizeof(struct nodo));
	auxtemp->sgte[1] = auxtemp->sgte[0];

	strcpy(auxtemp->sgte[0]->apellido, valor2);
	auxtemp->sgte[1]->sueldo = valor3;


	auxtemp->sgte[0]->sgte[0] = NULL;
	auxtemp->sgte[1]->sgte[1] = NULL;
	return(auxtemp);
}
void Cargar_Datos(FILE *idarchivo, struct nodo *inicio)
{	struct nodo *previo, *auxtemp;
	int posicion;
	posicion=0;

	auxtemp = malloc(sizeof(struct nodo));
	fscanf(idarchivo, "%10s", auxtemp->rut);
	while(!feof(idarchivo))
	{	fscanf(idarchivo, "%10s%10s%7d", auxtemp->nombre, auxtemp->apellido,&auxtemp->sueldo);
		//auxtemp->pos = posicion;
		previo = inicio;
		while(strcasecmp(auxtemp->apellido, previo->sgte[0]->apellido) > 0)
		{	
			previo = previo->sgte[0];
		}
		while( (strcasecmp(auxtemp->apellido, previo->sgte[0]->apellido) == 0)&&( strcasecmp(auxtemp->nombre,previo->sgte[0]->nombre) > 0) )
		{
			previo = previo->sgte[0];
		}
		while(   (strcasecmp(auxtemp->apellido, previo->sgte[0]->apellido) == 0)&&( strcasecmp(auxtemp->nombre,previo->sgte[0]->nombre) == 0)&& (auxtemp->sueldo > previo->sgte[0]->sueldo) )
		{
			previo = previo->sgte[0];
		}
		auxtemp->sgte[0] = previo->sgte[0];
		previo->sgte[0] = auxtemp;
		previo->sgte[0]->pos = posicion;

		previo = inicio;
		while(auxtemp->sueldo < previo->sgte[1]->sueldo)
		{	previo = previo->sgte[1];
		}
		while((auxtemp->sueldo == previo->sgte[1]->sueldo)&&(strcasecmp(auxtemp->rut, previo->sgte[1]->rut) > 0))
		{	previo = previo->sgte[1];
		}		
		auxtemp->sgte[1] = previo->sgte[1];
		previo->sgte[1] = auxtemp;
		previo->sgte[1]->pos = posicion;


		auxtemp = malloc(sizeof(struct nodo));
		fseek(idarchivo,30,SEEK_CUR);
		fscanf(idarchivo, "%10s", auxtemp->rut);
		posicion++;
	}
}
/*void Imprimir(struct nodo *inicio,int i)
{	inicio = inicio->sgte[i];
	while(inicio->sgte[i] != NULL)
	{	printf("%-10s %10s %7d %10s\n", inicio->apellido, inicio->nombre, inicio->sueldo, inicio->rut);
		inicio = inicio->sgte[i];
	}
}*/
void Imprimir_V2(struct nodo *inicio,int i , FILE *idarchivo){
	char experiencia[16],area[11],lenguaje[6];

	inicio = inicio->sgte[i];
	while(inicio->sgte[i] != NULL)
	{	
		rewind(idarchivo);
		fseek(idarchivo,(SALTOLINEA*(inicio->pos))+SALTO,SEEK_CUR);
		fscanf(idarchivo,"%15s%10s%5s",experiencia,area,lenguaje);
		printf("%-10s %10s %7d %10s %-15s %-10s %-5s\n", inicio->apellido, inicio->nombre, inicio->sueldo, inicio->rut,experiencia,area,lenguaje);
		inicio = inicio->sgte[i];

	}
}
struct nodo *Liberar_Lista(struct nodo *inicio)
{	struct nodo *auxtemp;

	auxtemp = inicio;
	while(auxtemp != NULL)
	{	inicio = inicio->sgte[0];
		free(auxtemp);
		auxtemp = inicio;
	}
	free(inicio);
	return(NULL);
}