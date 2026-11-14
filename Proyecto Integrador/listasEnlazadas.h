#ifndef LISTAS_ENLAZADAS_H
#define LISTAS_ENLAZADAS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAXCARACTERES 25

/* INTERFAZ */
/* Declaracion de tipos de datos personalizados */
typedef char tString[MAXCARACTERES]; /* Tipo de dato cadena */
typedef tString tVectorStrings[];

/* Estructura de registro almacenada en Productos.dat (48 bytes en x86/x64) */
typedef struct {
	int codProducto;
	tString descripcion;
	int cantidad;
	int codCategoria; /* 1-Comestibles, 2-Tecnologia, 3-Productos del Hogar, 4-Farmacia, 5-Ropa, 6-Otros */
	float costoDelProducto;
	float precio;
} tDato;

typedef struct nodo {
	tDato dato;
	struct nodo * siguiente;
} tLista;

/* Declaracion de prototipos */
void inicializarLista( tLista ** );
bool listaVacia( tLista * );
void insertarElem( tDato, tLista ** );
void eliminarElemPrimer( tLista ** );
void visualizarElem( tLista * );
void imprimirProducto( const tDato * );
void insertarElemPos( tDato, int, tLista ** );
void eliminarElemPos( int, tLista ** );
int contarNodos( tLista * );
void liberarLista( tLista ** );

/* IMPLEMENTACION */
void inicializarLista( tLista ** pLista ) {
	*pLista = NULL;
	printf("Lista inicializada.\n");
}

bool listaVacia( tLista * pLista ) {
	return pLista == NULL;
}

void insertarElem( tDato pDato, tLista ** pLista ) {
	tLista * nuevoNodo;

	/* Se reserva memoria para el nodo y se asigna esa memoria al puntero */
	nuevoNodo = ( tLista * ) malloc(sizeof( tLista ));
	if( nuevoNodo == NULL ) {
		printf("Error: no hay memoria suficiente para insertar el producto.\n");
		return;
	}

	/* Se asignan los valores al componente correspondiente al dato */
	nuevoNodo->dato = pDato;

	if( listaVacia( *pLista ) ) {
		nuevoNodo->siguiente = NULL;
		*pLista = nuevoNodo;
		printf("Primer elemento insertado!\n");
	} else {
		/* Se enlaza al inicio de la lista actual */
		nuevoNodo->siguiente = *pLista;
		*pLista = nuevoNodo;
		printf("Elemento insertado!\n");
	}
}

void eliminarElemPrimer( tLista ** pLista ) {
	tLista * nodoSuprimir;

	if( !listaVacia( *pLista ) ) {
		nodoSuprimir = *pLista;
		*pLista = (*pLista)->siguiente;
		printf("Se elimino el producto: %s\n", nodoSuprimir->dato.descripcion);
		free(nodoSuprimir); /* Libera el espacio que estaba siendo ocupado */
		nodoSuprimir = NULL;
	} else {
		printf("No hay nada para eliminar!\n");
	}
}

void imprimirProducto( const tDato * pDato ) {
	if( pDato == NULL ) {
		return;
	}
	printf(" %-3d | %-6d | %-27s | %-4d | $%-8.2f | $%.2f\n",
		pDato->codCategoria,
		pDato->codProducto,
		pDato->descripcion,
		pDato->cantidad,
		pDato->costoDelProducto,
		pDato->precio);
}

void visualizarElem( tLista * pLista ) {
	if( pLista != NULL ) {
		imprimirProducto( &pLista->dato );
		visualizarElem( pLista->siguiente );
	}
}

void insertarElemPos( tDato pDato, int pos, tLista ** pLista ) {
	int cantProductos;
	tLista * aux;
	tLista * nuevoNodo;
	int i;

	cantProductos = contarNodos( *pLista );
	aux = *pLista;

	if( pos == 1 ) {
		insertarElem( pDato, pLista );
	} else {
		if( (pos >= 1) && (pos <= cantProductos + 1) ) {
			/* Nos desplazamos hasta el nodo anterior a la posicion deseada */
			for( i = 1; i <= pos - 2; i++ ) {
				aux = aux->siguiente;
			}

			nuevoNodo = ( tLista * ) malloc(sizeof( tLista ));
			if( nuevoNodo == NULL ) {
				printf("Error: no hay memoria suficiente para insertar el producto.\n");
				return;
			}

			nuevoNodo->dato = pDato;
			nuevoNodo->siguiente = aux->siguiente;
			aux->siguiente = nuevoNodo;
			printf("Elemento insertado en la posicion %d.\n", pos);
		} else {
			printf("La posicion ingresada no existe o excede la cantidad de productos listados!\n");
		}
	}
}

void eliminarElemPos( int pos, tLista ** pLista ) {
	int cantProductos;
	tLista * aux;
	tLista * nodoSuprimir;
	int i;

	if( !listaVacia( *pLista ) ) {
		cantProductos = contarNodos( *pLista );
		aux = *pLista;

		if( pos == 1 ) {
			eliminarElemPrimer( pLista );
		} else {
			if( (pos >= 1) && (pos <= cantProductos) ) {
				/* Nos desplazamos hasta el nodo anterior al que se desea eliminar */
				for( i = 1; i <= pos - 2; i++ ) {
					aux = aux->siguiente;
				}
				nodoSuprimir = aux->siguiente;
				aux->siguiente = nodoSuprimir->siguiente;

				printf("Se elimino %s de la posicion %d!\n", nodoSuprimir->dato.descripcion, pos);
				free(nodoSuprimir);
				nodoSuprimir = NULL;
			} else {
				printf("La posicion ingresada no existe o excede la cantidad de productos listados!\n");
			}
		}
	} else {
		printf("No hay nada para eliminar!\n");
	}
}

int contarNodos( tLista * pLista ) {
	tLista * aux;
	int contadorNodos;

	aux = pLista;
	contadorNodos = 0;

	while( aux != NULL ) {
		contadorNodos++;
		aux = aux->siguiente;
	}
	return contadorNodos;
}

void liberarLista( tLista ** pLista ) {
	tLista * nodoActual;
	tLista * nodoSiguiente;

	nodoActual = *pLista;
	while( nodoActual != NULL ) {
		nodoSiguiente = nodoActual->siguiente;
		free(nodoActual);
		nodoActual = nodoSiguiente;
	}
	*pLista = NULL;
}

#endif
