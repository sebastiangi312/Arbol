#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

typedef struct nodo{				//Estructura de tipo nodo en la cual se almacenan el dato y las referencias a los hijos
	char dato;
	struct nodo *ptrIzq;
	struct nodo *ptrDer;
}Nodo;

typedef struct pila{
	int size;				//Pila en la cual se almacenaran las referencias a los padres
	struct nodo *ptrAnterior;
	struct pila *ptrSiguiente;
}Pila;

void AgregarPila(Pila **ptrInicio,Nodo *ptrNodo);
Nodo* EliminarPila(Pila **ptrInicio);
void AgregarArbol(Nodo **ptrRaiz,Nodo **ptrActual,Pila **ptrPila,char dato);
void EncontrarSiguiente(Nodo **ptrActual,Pila **ptrPila);
void ImprimirVertical(Nodo *ptrRaiz);
void Reubicar(Nodo **ptrActual,Pila **ptrPila,int *NivelMax,int *nivel,int *NivelAux);
int Niveles(Nodo *ptrRaiz);
void ImprimirOperadorNum(char dato);
void ResolverArbol(Nodo *ptrRaiz);
Nodo* CopiarArbol(Nodo *ptrRaiz);
bool Verificacion(Nodo *ptrNodo);
char ResolverNodo(Nodo **ptrNodo);

int main() {
	Pila *ptrPila=NULL;
	Nodo *ptrRaiz=NULL;
	Nodo *ptrActual=NULL;
	int Eleccion;
	char dato;
	printf("Digite 1 para agregar un dato u operador,2 para Mostrar el arbol,3 para resolver el arbol y 4 para acabar la ejecucion\n");
	scanf("%d",&Eleccion);
	while(Eleccion!=4){
		switch(Eleccion){
			case 1:
				printf("Ingrese el dato\n");
				scanf("%s",&dato);
				AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
				break;
			case 2:
				ImprimirVertical(ptrRaiz);
				break;
			case 3:
				ResolverArbol(ptrRaiz);
				break;
		}
		printf("Digite 1 para agregar un dato,2 para Mostrar el arbol,3 para resolverlo y 4 para acabar con la ejecucion\n");
		scanf("%d",&Eleccion);
	}
	return 0;
}

void AgregarArbol(Nodo **ptrRaiz,Nodo **ptrActual,Pila **ptrPila,char dato){  //Funcion que unicamente se encarga de crear la raiz,y llenar los hijos.
	int i=0;
	if(*ptrRaiz==NULL){							
		*ptrRaiz=(Nodo*)malloc(sizeof(Nodo));			//Si la raiz no existe este la crea y hace que el puntero a la raiz y el
		(*ptrRaiz)->dato=dato;					// puntero Actual(en el que se agregara el siguiente nodo) apunten a ella.
		(*ptrRaiz)->ptrDer=NULL;
		(*ptrRaiz)->ptrIzq=NULL;
		*ptrActual=*ptrRaiz;
	}else{	
		if((*ptrActual)->ptrIzq==NULL){			//En caso de que el puntero actual no tenga hijo izquierdo este lo crea y llama la funcion ordenar.
			Nodo *ptrHijo= (Nodo*)malloc(sizeof(Nodo));
			ptrHijo->ptrDer=NULL;
			ptrHijo->ptrIzq=NULL;
			ptrHijo->dato=dato;
			(*ptrActual)->ptrIzq=ptrHijo;
			AgregarPila(ptrPila,*ptrActual);
			EliminarPila(ptrPila);
		}else{
			Nodo *ptrHijo=(Nodo*)malloc(sizeof(Nodo)); //En caso de que el puntero actual tenga hijo izquierdo este creara el hijo derecho y ejecutara la funcion para reubicar el puntero actual.
			ptrHijo->dato=dato;				
			ptrHijo->ptrDer=NULL;
			ptrHijo->ptrIzq=NULL;
			(*ptrActual)->ptrDer=ptrHijo;
			AgregarPila(ptrPila,*ptrActual);
			EliminarPila(ptrPila);
			EncontrarSiguiente(ptrActual,ptrPila);
		}
	}
}
void EncontrarSiguiente(Nodo **ptrActual,Pila **ptrPila){	//Funcion que se encarga de reubicar el puntero actual al nodo en el cual debe ingresarse el siguiente dato.

	Nodo *ptrPadre=EliminarPila(ptrPila);		//Esta funcion crea un puntero llamado puntero Padre y en el ingresara el la referencia del padre del puntero actual atravez de la pila,Aqui se pueden dar 2 casos. 
	if(ptrPadre==NULL){										
		while((*ptrActual)->ptrIzq!=NULL){			//Primer caso: Que el puntero padre apunte a NULL,lo cual quiere decir que el puntero actual es la raiz y esto solo pasara cuando todos los niveles esten llenos,
			AgregarPila(ptrPila,*ptrActual);		//Por lo tanto se ubicara en el Nodo Hoja que se encuentre mas a la izquierda(Guardando en la pila todas las posiciones por las que pasa) y desde aqui procedera a crear otro nivel cuando se le ingrese el siguiente dato.
			*ptrActual=(*ptrActual)->ptrIzq;	
		}
	}else{											//Segundo caso: Que este exista y de aqui se desembocan 2 casos:
	
		if(ptrPadre->ptrDer!=*ptrActual){			//Primer caso: Que el puntero Actual sea el hijo izquierdo de su padre y en este caso el puntero actual pasara a apuntar a su hermano(hijo derecho de su padre)
			AgregarPila(ptrPila,ptrPadre);			//y de aqui se ubicara en el Nodo hoja que se encuentre mas a la izquierda de este (Guardando en la pila todas las posiciones por las que pasa).
			*ptrActual=ptrPadre->ptrDer;
			while((*ptrActual)->ptrIzq!=NULL){
				AgregarPila(ptrPila,*ptrActual);
				*ptrActual=(*ptrActual)->ptrIzq;	
			}
			}else{
			*ptrActual=ptrPadre;					//Segundo Caso:Que el puntero Actual sea el hijo derecho de su padre, en este caso el puntero actual apuntara a su padre y llamara recursivamente la funcion.
			EncontrarSiguiente(ptrActual,ptrPila);
		}
	}
}

void AgregarPila(Pila **ptrInicio,Nodo *ptrNodo){			//Pila en la cual se guardaran las direcciones de los Nodos padres.

	if(*ptrInicio==NULL){									//Si la Pila esta vacia este le agrega el primer dato, almacena la direccion del Nodo Padre y actualiza el puntero a la pila.
		Pila *ptrAux=(Pila*)malloc(sizeof(Pila));
		ptrAux->ptrAnterior=ptrNodo;
		ptrAux->ptrSiguiente=NULL;
		*ptrInicio=ptrAux;
	}else{													//En caso de que esta no esta vacia agrega un dato,hace que este guarde la referencia al que hay al lado de el y actualiza el puntero a la pila.
		Pila *ptrAux=(Pila*)malloc(sizeof(Pila));
		ptrAux->ptrAnterior=ptrNodo;
		ptrAux->ptrSiguiente=*ptrInicio;
		*ptrInicio=ptrAux;
	}
}
Nodo* EliminarPila(Pila **ptrInicio){				//Si la Pila tiene datos devuelve el ultimo y en caso de estar vacia devuelve NULL.
	if(*ptrInicio!=NULL){
		Nodo *ptrAux=(*ptrInicio)->ptrAnterior;
		*ptrInicio=(*ptrInicio)->ptrSiguiente;
		return ptrAux;
	}else{
		return NULL;
	}
}

void ImprimirVertical(Nodo *ptrRaiz){	//Funcion encargada de imprimir de forma vertical el arbol
	Pila *ptrPila=NULL;
	Nodo *ptrActual=ptrRaiz;
	int EspInicio,i,EspMedio,nivelActual,nivelAux;
	int NivelMax=Niveles(ptrRaiz);		//Mediante la funcion Niveles se calcula la cantidad de niveles que tiene el arbol.
	nivelActual=0;
	nivelAux=0;
	int *ptrNivelAux=&nivelAux;
	int *ptrNivelMax=&NivelMax;
	int *ptrNivelActual=&nivelActual;
	if(ptrRaiz==NULL){					//Si el puntero a la Raiz no apunta a nada este imprimira que el arbol esta vacio.
		printf("El arbol esta vacio\n");
	}else{
		printf("\n");
		EspInicio=pow(2,NivelMax)-1;		//Mediante la ecuacion 2^n-1(siendo n la cantidad de niveles(Iniciales) del arbol) se calcula la primera cantidad de espacios.
		for(i=0;i<EspInicio;i++){
			printf(" ");
		}
		ImprimirOperadorNum(ptrActual->dato); 	//se imprimira el dato y pasara a la siguiente linea y restara en 1 la cantidad de niveles.
		printf("\n");
		NivelMax--;
		while(ptrActual->ptrIzq!=NULL){		//Mientras el puntero Actual tenga un hijo izquierdo, este verificara si tiene hijo derecho, en caso de no tener
			if(ptrActual->ptrDer==NULL){	//se da por sentado que este es el ultimo dato del arbol. 
				ImprimirOperadorNum(ptrActual->ptrIzq->dato);
				printf("\n");
				break;
			}else{
				if(nivelActual==0){
					EspInicio=pow(2,NivelMax)-1;
					for(i=0;i<EspInicio;i++){
						printf(" ");
					}
				}
				ImprimirOperadorNum(ptrActual->ptrIzq->dato);
				EspMedio=pow(2,NivelMax+1)-1;
				for(i=0;i<EspMedio;i++){
					printf(" ");
				}
				ImprimirOperadorNum(ptrActual->ptrDer->dato);
				for(i=0;i<EspMedio;i++){
					printf(" ");
				}
				Reubicar(&ptrActual,&ptrPila,ptrNivelMax,ptrNivelActual,ptrNivelAux);
			}
		}
		printf("\n");
	} 
}

void Reubicar(Nodo **ptrActual,Pila **ptrPila,int *NivelMax,int *nivel,int *NivelAux){
	Nodo *ptrPadre=EliminarPila(ptrPila);	
	int NE,i;
	if(ptrPadre==NULL){
		*NivelMax=*NivelMax-1;
		*nivel=*nivel+1;
		for(i=0;i<*nivel;i++){
			AgregarPila(ptrPila,*ptrActual);
			*ptrActual=(*ptrActual)->ptrIzq;
			*NivelAux=*NivelAux+1;
		}
		printf("\n");
		NE=pow(2,*NivelMax)-1;
		for(i=0;i<NE;i++){
			printf(" ");
		}
	}else{
		if(ptrPadre->ptrDer!=*ptrActual){			
			AgregarPila(ptrPila,ptrPadre);			
			*ptrActual=ptrPadre->ptrDer;
			for(i=0;i<*nivel-*NivelAux;i++){
				AgregarPila(ptrPila,*ptrActual);
				*ptrActual=(*ptrActual)->ptrIzq;
			}
			*NivelAux=*nivel;
		}else{
			*ptrActual=ptrPadre;					
			*NivelAux=*NivelAux-1;
			Reubicar(ptrActual,ptrPila,NivelMax,nivel,NivelAux);
		}
	}
}
int Niveles(Nodo *ptrRaiz){		//funcion que calcula y devuelve el numero de niveles.
	Nodo *ptrAux=ptrRaiz;
	int Nivel=0;
	if(ptrAux!=NULL){			//Mientras el puntero en cuestion tenga hijo izquiero el puntero apuntara a este y aumentara en 1 a la variable nivel.
		while(ptrAux->ptrIzq!=NULL ){
			ptrAux=ptrAux->ptrIzq;
			Nivel++;
		}
	}
	return Nivel;
}


void ResolverArbol(Nodo *ptrRaiz){
	Pila *ptrPila = NULL;
	Nodo *ptrRaizCopiado= CopiarArbol(ptrRaiz);
	if(ptrRaizCopiado!=NULL){
		Nodo *ptrActualCopiado=ptrRaizCopiado;
		ImprimirVertical(ptrRaizCopiado);
		while(ptrActualCopiado->ptrIzq!=NULL){
			AgregarPila(&ptrPila,ptrActualCopiado);
			ptrActualCopiado=ptrActualCopiado->ptrIzq;
		}
		Nodo *ptrPadre=EliminarPila(&ptrPila);
		if(ptrPadre==NULL){
			printf("El Arbol es de un solo nivel\n");
		}else{
			ptrActualCopiado=ptrPadre;
			if(ptrActualCopiado!=NULL){
				while(ptrActualCopiado!=ptrRaizCopiado){
					bool Verificar = Verificacion(ptrActualCopiado);
					if(Verificar==true){
						ResolverNodo(&ptrActualCopiado);
						EncontrarSiguiente(&ptrActualCopiado,&ptrPila);
						Nodo *ptrPadre=EliminarPila(&ptrPila);
						if(ptrPadre!=NULL){
							ptrActualCopiado=ptrPadre;
						}
					}else{
						printf("El Arbol es Invalido\n");
						break;
						break;
					}
				}
				bool Verificar = Verificacion(ptrActualCopiado);
					if(Verificar==true){
						ResolverNodo(&ptrActualCopiado);
						EncontrarSiguiente(&ptrActualCopiado,&ptrPila);
						Nodo *ptrPadre=EliminarPila(&ptrPila);
						if(ptrPadre!=NULL){
							ptrActualCopiado=ptrPadre;
						}
					}
				printf("El Arbol da como resultado %c\n",ptrActualCopiado->dato);
			}	
		}
		
	}
	
}


void ImprimirOperadorNum(char dato){
	switch(dato){
		case '+':
			printf("+");
			break;
		case '-':
			printf("-");
			break;
		case '*':
			printf("*");
			break;
		case '/':
			printf("/");
			break;
		default:
			printf("%d",dato-48);
			break;
	}
}


Nodo* CopiarArbol(Nodo *ptrRaiz){
	Pila *ptrPila=NULL;
	Pila *ptrPilaNueva=NULL;
	Nodo *ptrActual=ptrRaiz;
	int nivelActual,nivelAux;
	int NivelMax=Niveles(ptrRaiz);		//Mediante la funcion Niveles se calcula la cantidad de niveles que tiene el arbol.
	nivelActual=0;
	nivelAux=0;
	int *ptrNivelAux=&nivelAux;
	int *ptrNivelMax=&NivelMax;
	int *ptrNivelActual=&nivelActual;
	Nodo *ptrRaizNuevo=NULL;
	Nodo *ptrActualNuevo=ptrRaizNuevo;
	if(ptrRaiz!=NULL){
		AgregarArbol(&ptrRaizNuevo,&ptrActualNuevo,&ptrPilaNueva,ptrActual->dato);
		NivelMax--;
		while(ptrActual->ptrIzq!=NULL){		//Mientras el puntero Actual tenga un hijo izquierdo, este verificara si tiene hijo derecho, en caso de no tener
			if(ptrActual->ptrDer==NULL){	//se da por sentado que este es el ultimo dato del arbol. 
				AgregarArbol(&ptrRaizNuevo,&ptrActualNuevo,&ptrPilaNueva,ptrActual->ptrIzq->dato);
				break;
			}else{
				AgregarArbol(&ptrRaizNuevo,&ptrActualNuevo,&ptrPilaNueva,ptrActual->ptrIzq->dato);
				AgregarArbol(&ptrRaizNuevo,&ptrActualNuevo,&ptrPilaNueva,ptrActual->ptrDer->dato);
				Reubicar(&ptrActual,&ptrPila,ptrNivelMax,ptrNivelActual,ptrNivelAux);
			}
		}
	}else{
		printf("El arbol esta vacio\n");
	}
	return ptrRaizNuevo;
}

bool Verificacion(Nodo *ptrNodo){
	bool Padre=true;
	bool Der=true;
	bool Izq=true;
	if(ptrNodo->ptrDer==NULL || ptrNodo->ptrIzq==NULL){
		return false;
	}else{
		switch(ptrNodo->dato){
			case '+':
				Padre=false;
				break;
			case '-':
				Padre=false;
				break;
			case '*':
				Padre=false;
				break;
			case '/':
				Padre=false;
				break;
			default:
				break;
		}
		switch(ptrNodo->ptrDer->dato){
			case '+':
				Der=false;
				break;
			case '-':
				Der=false;
				break;
			case '*':
				Der=false;
				break;
			case '/':
				Der=false;
				break;
			default:
				break;
		}
		switch(ptrNodo->ptrIzq->dato){
			case '+':
				Izq=false;
				break;
			case '-':
				Izq=false;
				break;
			case '*':
				Izq=false;
				break;
			case '/':
				Izq=false;
				break;
			default:
				break;
		}
		if(Padre==false && Izq==true && Der==true){
			return true;
		}else{
			return false;
		}
	}
}

char ResolverNodo(Nodo **ptrNodo){
	int Izq = (*ptrNodo)->ptrIzq->dato;
	int Der = (*ptrNodo)->ptrDer->dato;
	switch((*ptrNodo)->dato){
		case '+':
			(*ptrNodo)->dato=(Izq-48)+(Der-48)+48;
			break;
		case '-':
			(*ptrNodo)->dato=(Izq-48)-(Der-48)+48;
			break;
		case '*':
			(*ptrNodo)->dato=(Izq-48)*(Der-48)+48;
			break;
		case '/':
			(*ptrNodo)->dato=(Izq-48)/(Der-48)+48;
			break;
	}
	Nodo *ptrAux=(*ptrNodo)->ptrIzq;
	(*ptrNodo)->ptrIzq=NULL;
	free(ptrAux);
	ptrAux=(*ptrNodo)->ptrDer;
	(*ptrNodo)->ptrDer=NULL;
	free(ptrAux);
}
