#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

typedef struct nodo{				//Estructura de tipo nodo en la cual se almacenan el dato y las referencias a los hijos
	char dato[7];
	struct nodo *ptrIzq;
	struct nodo *ptrDer;
}Nodo;

typedef struct NodoArbol{
	struct nodo *ptrBase;
	struct nodo *ptrVariable;
}Arbol;

typedef struct pila{
	int size;				//Pila en la cual se almacenaran las referencias a los padres
	struct nodo *ptrAnterior;
	struct pila *ptrSiguiente;
}Pila;

void AgregarArbol(Nodo **ptrRaiz,Nodo **ptrActual,Pila **ptrPila,char dato[7]);
void AgregarPila(Pila **ptrInicio,Nodo *ptrNodo);
int CalculoDigitos(char dato[7]);
float CharAFloat(char Arreglo[7]);
Nodo* CopiarArbol(Nodo *ptrRaiz);
Nodo* CrearArbol(int NivelesArbol,Nodo **ptrVariable);
Nodo* EliminarPila(Pila **ptrInicio);
void EncontrarSiguiente(Nodo **ptrActual,Pila **ptrPila);
bool EsNumero(char dato);
void ImprimirOperadorNum(char dato[7]);
void ImprimirVertical(Nodo *ptrRaiz);
int Niveles(Nodo *ptrRaiz);
int NumeroMasLargo(Nodo *ptrRaiz);
void Recorrer(Nodo **ptrActual,Pila **ptrPila,int *NivelMax,int *nivel,int *NivelAux,int digitos);
float ResolverArbol(Nodo *ptrRaiz);
char ResolverNodo(Nodo **ptrNodo);
void Reubicar(Nodo **ptrActual,Pila **ptrPila,int *NivelMax,int *nivel,int *NivelAux,int digitos);
bool Verificacion(Nodo *ptrNodo);

int main() {
	srand(time(NULL));
	int Eleccion,niveles,i,j,cantidad,indice,numerosFuncion;
	printf("Digite la cantidad de arboles a crear\n");
	scanf("%d",&cantidad);
	Arbol ArregloArboles[cantidad];
	printf("Ingrese el numero de niveles que tendran estos arboles\n");
	scanf("%d",&niveles);
	for(i=0;i<cantidad;i++){
		(ArregloArboles[i]).ptrBase=CrearArbol(niveles,&(ArregloArboles[i]).ptrVariable);
	}
	printf("Cuantos numeros de la funcion quiere evaluar\n");
	scanf("%d",&numerosFuncion);
	float matrizFuncion[cantidad][numerosFuncion];
	printf("Digite 1 para Imprimir un Arbol en especifico,2 para Resolverlos y 3 para Salir\n");
	scanf("%d",&Eleccion);
	while(Eleccion!=3){
		switch(Eleccion){
			case 1:
				printf("Digite el indice del arbol a imprimir\n");
				scanf("%d",&indice);
				ImprimirVertical((ArregloArboles[indice]).ptrBase);
				break;
			case 2:	
				for(i=0;i<cantidad;i++){
					for(j=1;j<=numerosFuncion;j++){
						char aux = j+'0';
						((ArregloArboles[i]).ptrVariable)->dato[0]=aux;
						matrizFuncion[i][j] = ResolverArbol((ArregloArboles[i]).ptrBase);	
						printf("%f ",matrizFuncion[i][j]);
					}
					((ArregloArboles[i]).ptrVariable)->dato[0]='X';
					printf("\n");
				}
				break;
		}
		printf("Digite 1 para Imprimir un Arbol en especifico,2 para Resolverlos y 3 para Salir\n");
		scanf("%d",&Eleccion);
	}	
	return 0;
}

void AgregarArbol(Nodo **ptrRaiz,Nodo **ptrActual,Pila **ptrPila,char dato[7]){  //Funcion que unicamente se encarga de crear la raiz,y llenar los hijos.
	int i=0;
	if(*ptrRaiz==NULL){							
		*ptrRaiz=malloc(sizeof(Nodo));			//Si la raiz no existe este la crea y hace que el puntero a la raiz y el
		for(i=0;i<7;i++){					// puntero Actual(en el que se agregara el siguiente nodo) apunten a ella.
			(*ptrRaiz)->dato[i]=dato[i];
		}
		(*ptrRaiz)->ptrDer=NULL;
		(*ptrRaiz)->ptrIzq=NULL;
		*ptrActual=*ptrRaiz;
	}else{	
		if((*ptrActual)->ptrIzq==NULL){			//En caso de que el puntero actual no tenga hijo izquierdo este lo crea y llama la funcion ordenar.
			Nodo *ptrHijo=malloc(sizeof(Nodo));
			ptrHijo->ptrDer=NULL;
			ptrHijo->ptrIzq=NULL;
			for(i=0;i<7;i++){
				ptrHijo->dato[i]=dato[i];
			}
			(*ptrActual)->ptrIzq=ptrHijo;
			AgregarPila(ptrPila,*ptrActual);
			EliminarPila(ptrPila);
		}else{
			Nodo *ptrHijo=malloc(sizeof(Nodo)); //En caso de que el puntero actual tenga hijo izquierdo este creara el hijo derecho y ejecutara la funcion para reubicar el puntero actual.
			for(i=0;i<7;i++){
				ptrHijo->dato[i]=dato[i];
			}				
			ptrHijo->ptrDer=NULL;
			ptrHijo->ptrIzq=NULL;
			(*ptrActual)->ptrDer=ptrHijo;
			AgregarPila(ptrPila,*ptrActual);
			EliminarPila(ptrPila);
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

 int CalculoDigitos(char dato[7]){
	int digitos=0;
	int i;
	for(i=0;i<7;i++){
		if(EsNumero(dato[i])==true){
			digitos++;
		}else{
			break;
		}
	}
	return digitos-1;
}

float CharAFloat(char Arreglo[7]){
	int digitos=0;
	int i;
	int aux=0;
	int decimal=-1;
	float total=0;
	if(Arreglo[0]=='-'){
		for(i=1;i<7;i++){
			if(Arreglo[i]=='0' || Arreglo[i]=='1' || Arreglo[i]=='2' || Arreglo[i]=='3' || Arreglo[i]=='4' || Arreglo[i]=='5' || Arreglo[i]=='6' || Arreglo[i]=='7' || Arreglo[i]=='8' || Arreglo[i]=='9'){
				digitos++;
			}else{
				if(Arreglo[i]=='.'){
					decimal=i;
					digitos++;
				}else{
					break;	
				}		
			}		
		}	
		if(decimal==-1){
			for(i=digitos-2;i>=0;i--){
				total=total-pow(10,i)*(Arreglo[aux]-48);
				aux++;
			}
		}else{
			aux++;
			for(i=decimal-2;i>=0;i--){
				total=total-pow(10,i)*(Arreglo[aux]-48);
				aux++;
			}
			int dif=decimal-(digitos-2);
			aux++;
			for(i=-1;i>=dif;i--){
				total=total-pow(10,i)*(Arreglo[aux]-48);
				aux++;
			}
		}
		return total;	
	}else{
		for(i=0;i<7;i++){
			if(Arreglo[i]=='0' || Arreglo[i]=='1' || Arreglo[i]=='2' || Arreglo[i]=='3' || Arreglo[i]=='4' || Arreglo[i]=='5' || Arreglo[i]=='6' || Arreglo[i]=='7' || Arreglo[i]=='8' || Arreglo[i]=='9'){
				digitos++;
			}else{
				if(Arreglo[i]=='.'){
					decimal=i;
					digitos++;
				}else{
					break;	
				}		
			}		
		}	
		if(decimal==-1){
			for(i=digitos-1;i>=0;i--){
				total=total+pow(10,i)*(Arreglo[aux]-48);
				aux++;
			}
		}else{
			for(i=decimal-1;i>=0;i--){
				total=total+pow(10,i)*(Arreglo[aux]-48);
				aux++;
			}
			int dif=decimal-(digitos-1);
			aux++;
			for(i=-1;i>=dif;i--){
				total=total+pow(10,i)*(Arreglo[aux]-48);
				aux++;
			}
		}
		return total;	
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
				Recorrer(&ptrActual,&ptrPila,ptrNivelMax,ptrNivelActual,ptrNivelAux,0);
			}
		}
	}else{
		printf("El arbol esta vacio\n");
	}
	return ptrRaizNuevo;
}

Nodo* CrearArbol(int NivelesArbol,Nodo **ptrVariable){
	Pila *ptrPila=NULL;
	Nodo *ptrActual=NULL;
	Nodo *ptrRaiz=NULL;
	int i;
	int NivelesOperador = pow(2,NivelesArbol)/2-1;
	int NivelesNumeros = NivelesOperador+1;
	int variable = rand() % 2;
	bool aux;
	if(variable==0){
		aux=false;
	}else{
		aux=true;
	}
	char dato[7];
	for(i=0;i<NivelesOperador;i++){
		int numero = rand() % 4;
			switch(numero){
				case 0:
					dato[0] = '+';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 1:
					dato[0] = '-';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 2:
					dato[0] = '*';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 3:
					dato[0] = '/';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
			}	
		}	
	for(i=0;i<NivelesNumeros;i++){
		int numero = rand() % 9 + 1;
		if(aux && variable==1){
			dato[0] = 'X';
			dato[1]='P';
			Nodo *ptrAux=ptrActual;
			AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
			if(ptrAux==ptrActual){
				if(ptrAux->ptrDer==NULL){
					*ptrVariable=ptrAux->ptrIzq;
				}
			}else{
				*ptrVariable=ptrAux->ptrDer;
			}
			aux=false;
		}else{
			switch(numero){
				dato[1]='P';
				case 1:
					dato[0] = '1';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 2:
					dato[0] = '2';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 3:
					dato[0] = '3';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 4:
					dato[0] = '4';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 5:
					dato[0] = '5';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 6:
					dato[0] = '6';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 7:
					dato[0] = '7';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 8:
					dato[0] = '8';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
				case 9:
					dato[0] = '9';
					AgregarArbol(&ptrRaiz,&ptrActual,&ptrPila,dato);
					break;
			}
			if(aux || variable==1){
				aux=false;
			}else{
				if(i==NivelesOperador-2){
					variable=1;
					aux=true;
				}else{
					variable = rand() % 2;
					if(variable==0){
						aux=false;
					}else{
						aux=true;
					}	
				}		
			}
		}
	}
	return ptrRaiz;
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

bool EsNumero(char dato){
	if(dato=='-' || dato=='.' || dato=='0' || dato=='1' || dato=='2' || dato=='3' || dato=='4' || dato=='5' || dato=='6' || dato=='7' || dato=='8' || dato=='9'){
		return true;
	}else{
		return false;
	}
}

void ImprimirOperadorNum(char dato[7]){
	int i=0;
	switch(dato[0]){
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
		case'X':
			printf("X");
			break;
		default:
			for(i=0;i<7;i++){
				if(EsNumero(dato[i])==true){
					if(dato[i]!='.'){
						printf("%d",dato[i]-48);
					}else{
						printf(".");
					}
				}else{
					break;
				}
			}
			break;
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
	int digitos=NumeroMasLargo(ptrRaiz);
	if(ptrRaiz==NULL){					//Si el puntero a la Raiz no apunta a nada este imprimira que el arbol esta vacio.
		printf("El arbol esta vacio\n");
	}else{
		printf("\n");
		EspInicio=pow(2,NivelMax)-1+digitos;		//Mediante la ecuacion 2^n-1(siendo n la cantidad de niveles(Iniciales) del arbol) se calcula la primera cantidad de espacios.
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
					EspInicio=pow(2,NivelMax)-1+digitos;
					for(i=0;i<EspInicio;i++){
						printf(" ");
					}
				}
				ImprimirOperadorNum(ptrActual->ptrIzq->dato);
				EspMedio=pow(2,NivelMax+1)-1+digitos;
				if(NivelMax==0){
					EspMedio=EspMedio-digitos;
				}
				for(i=0;i<EspMedio;i++){
					printf(" ");
				}
				ImprimirOperadorNum(ptrActual->ptrDer->dato);
				for(i=0;i<EspMedio;i++){
					printf(" ");
				}
				Reubicar(&ptrActual,&ptrPila,ptrNivelMax,ptrNivelActual,ptrNivelAux,digitos);
			}
		}
		printf("\n");
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

int NumeroMasLargo(Nodo *ptrRaiz){	//Funcion encargada de imprimir de forma vertical el arbol
	Pila *ptrPila=NULL;
	Nodo *ptrActual=ptrRaiz;
	int EspInicio,i,EspMedio,nivelActual,nivelAux;
	int NivelMax=Niveles(ptrRaiz);		//Mediante la funcion Niveles se calcula la cantidad de niveles que tiene el arbol.
	nivelActual=0;
	nivelAux=0;
	int *ptrNivelAux=&nivelAux;
	int *ptrNivelMax=&NivelMax;
	int *ptrNivelActual=&nivelActual;
	int digitos=CalculoDigitos(ptrActual->dato); 	//se imprimira el dato y pasara a la siguiente linea y restara en 1 la cantidad de niveles.
	NivelMax--;
	while(ptrActual->ptrIzq!=NULL){		//Mientras el puntero Actual tenga un hijo izquierdo, este verificara si tiene hijo derecho, en caso de no tener
		if(ptrActual->ptrDer==NULL){	//se da por sentado que este es el ultimo dato del arbol. 
			if(CalculoDigitos(ptrActual->ptrIzq->dato)>digitos){
				digitos=CalculoDigitos(ptrActual->ptrIzq->dato);
			}
			break;
		}else{
			if(CalculoDigitos(ptrActual->ptrIzq->dato)>digitos){
				digitos=CalculoDigitos(ptrActual->ptrIzq->dato);
			}
			if(CalculoDigitos(ptrActual->ptrDer->dato)>digitos){
				digitos=CalculoDigitos(ptrActual->ptrDer->dato);
			}
			Recorrer(&ptrActual,&ptrPila,ptrNivelMax,ptrNivelActual,ptrNivelAux,0);
		}
	}
	return digitos;
}

void Recorrer(Nodo **ptrActual,Pila **ptrPila,int *NivelMax,int *nivel,int *NivelAux,int digitos){
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
			Recorrer(ptrActual,ptrPila,NivelMax,nivel,NivelAux,0);
		}
	}
}

float ResolverArbol(Nodo *ptrRaiz){
	int i;
	Pila *ptrPila = NULL;
	Nodo *ptrRaizCopiado= CopiarArbol(ptrRaiz);
	if(ptrRaizCopiado!=NULL){
		Nodo *ptrActualCopiado=ptrRaizCopiado;
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
					return CharAFloat(ptrActualCopiado->dato);
				}else{
					printf("El Arbol da fallo\n");
					return 0;
				}
			}	
		}
	}
	
}

char ResolverNodo(Nodo **ptrNodo){
	float Izq = CharAFloat((*ptrNodo)->ptrIzq->dato);
	float Der = CharAFloat((*ptrNodo)->ptrDer->dato);
	char aux[7];
	int i;
	switch((*ptrNodo)->dato[0]){
		case '+':
			sprintf(aux,"%f",Izq+Der);
			break;
		case '-':
			sprintf(aux,"%f",Izq-Der);
			break;
		case '*':
			sprintf(aux,"%f",Izq*Der);
			break;
		case '/':
			sprintf(aux,"%f",Izq/Der);
			break;
	}
	for(i=0;i<7;i++){
		(*ptrNodo)->dato[i]=aux[i];
	}
	Nodo *ptrAux=(*ptrNodo)->ptrIzq;
	(*ptrNodo)->ptrIzq=NULL;
	free(ptrAux);
	ptrAux=(*ptrNodo)->ptrDer;
	(*ptrNodo)->ptrDer=NULL;
	free(ptrAux);
}

void Reubicar(Nodo **ptrActual,Pila **ptrPila,int *NivelMax,int *nivel,int *NivelAux,int digitos){
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
		NE=pow(2,*NivelMax)-1+digitos;
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
			Reubicar(ptrActual,ptrPila,NivelMax,nivel,NivelAux,0);
		}
	}
}

bool Verificacion(Nodo *ptrNodo){
	bool Padre=true;
	bool Der=true;
	bool Izq=true;
	if(ptrNodo->ptrDer==NULL || ptrNodo->ptrIzq==NULL){
		return false;
	}else{
		switch(ptrNodo->dato[0]){
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
		switch(ptrNodo->ptrDer->dato[0]){
			case '+':
				Der=false;
				break;
			case '-':
				if(!EsNumero(ptrNodo->ptrDer->dato[1])){
					Der=false;
				}
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
		switch(ptrNodo->ptrIzq->dato[0]){
			case '+':
				Izq=false;
				break;
			case '-':
				if(!EsNumero(ptrNodo->ptrIzq->dato[1])){
					Izq=false;	
				}
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
