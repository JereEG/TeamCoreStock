/* Librerias */
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>

/* Declaracion de tipos de datos personalizados */
typedef char tString[25]; /* Tipo de dato cadena */
typedef struct {
	int codigo;
	tString descripcion;
} tDato;
typedef struct nodo {
	tDato dato;
	struct nodo * siguiente;
} tLista;

/* Declaracion de prototipos */
void inicializarPila();
bool pilaVacia( tLista * );
/* Elem = Elemento */
void insertarProducPrimer( tDato );
void insertarProducEncima( tDato );
void insertarProducto( tDato );

void eliminarUltimoProducto();

void visualizarElem( tLista * );


void ejecutarPrograma(); 
/* Declaracion de variables */
tLista * pilaProductos;

/* Bloque principal */
int main() {
	inicializarPila();
	ejecutarPrograma(); 
	return 0;
}

/* Implementacion de funciones */
void inicializarPila() {
	pilaProductos = NULL;
	printf("Pila inicializada!\n");
}
bool pilaVacia( tLista *  pLista) { /* Verifica si la pila esta vacia */
	return pLista == NULL;
}

void insertarProducPrimer( tDato pDato ) {
	tLista * nuevoNodo;
	
	/* Se reserva memoria para el nodo y se asigna esa memoria al puntero */
	nuevoNodo = ( tLista * ) malloc(sizeof( tLista ));
	
	/* Se asigna valores al componente correspondiente al dato */
	nuevoNodo->dato = pDato;
	
	/* Se asigna una direccion al componente corresponiente al siguiente nodo */
	nuevoNodo->siguiente = NULL;

	pilaProductos = nuevoNodo;
	
	printf("Primer elemneto insertado! ...\n");
}
void insertarProducEncima( tDato pDato ) {
	tLista * nuevoNodo;
	
	/* Se reserva memoria para el nodo y se asigna esa memoria al puntero */
	nuevoNodo = ( tLista * ) malloc(sizeof( tLista ));
	
	/* Se asigna valores al componente correspondiente al dato */
	nuevoNodo->dato = pDato;
	
	/* Se asigna una direccion al componente corresponiente al siguiente nodo */
	nuevoNodo->siguiente = pilaProductos;
	/* Como nuevoNodo tiene la pilaProductos completa entoces le asigno la misma a la pilaProductos global */
	pilaProductos = nuevoNodo;
	printf("Elemneto insertado! ...\n");
}
void insertarProducto( tDato pDato) {
	if( pilaVacia( pilaProductos ) ) {
		insertarProducPrimer(pDato);
	} else {
		insertarProducEncima(pDato);
	}
}

void eliminarUltimoProducto() {
	if( !pilaVacia( pilaProductos ) ) {
	/* Es posible eliminar el primero nodo */
		tLista * nodoSuprimir;
		
		nodoSuprimir = pilaProductos;
		
		pilaProductos = pilaProductos->siguiente;
		
		printf(" Se elimino el producto: %s ...\n", nodoSuprimir->dato.descripcion);
		
		free(nodoSuprimir); //Libera el espasio que estaba siendo ocupado
		
		nodoSuprimir = NULL;
	} else {
		printf("No hay nada para eliminar!\n");
	}
}

void visualizarElem( tLista * pLista ) {
	tLista* aux;
	
	aux = pLista;
	
	if( !pilaVacia( aux ) ) {
		printf("Productos disponibles\n");
		printf("----------------------\n");
		printf("Codigo | Descripcion\n");
		while( aux != NULL ) {
			printf("%d | %s\n", aux->dato.codigo, aux->dato.descripcion);
			aux = aux->siguiente; /* Se pierde el anterior nodo para poder visualizar el siguiente (por eso usamos una variable auxiliar) */
		}
		
	} else {
		printf("Lista sin elementos\n");
	}
}


void ejecutarPrograma() {
	tDato producto;
	int opcion;
	printf("Ingrese un numero para realizar acion, 0 para finalizar programa\n1-Igresar Producto\t2-Eliminar ultimo producto\t3-Visualizar productos\n");
	scanf("%d", &opcion);
	if( !(opcion == 0) ) { /* Si opcion es 0 el programa termina */
		if( opcion == 1) {
			printf("Ingrese el codigo del producto:\n");
			fflush(stdin);
			scanf("%d", &producto.codigo);
			printf("Ingrese el nombre del producto:\n");
			fflush(stdin);
			scanf("%[^\n]s", producto.descripcion);
			insertarProducto( producto );
		}
		if( opcion == 2) {
			eliminarUltimoProducto();
		}
		if( opcion == 3) {
			visualizarElem( pilaProductos );
		}
		
		ejecutarPrograma(); /* Como la opcion no es 0 se puede llamar de nuevo a esta misma funcion */
	}
	
}
