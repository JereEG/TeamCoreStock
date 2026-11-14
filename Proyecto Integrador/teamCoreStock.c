/* Librerias */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "listasEnlazadas.h"

#define NOMBREARCHIVO "Productos.dat"
#define CANT_CATEGORIAS 6

/* Declaracion de prototipos */
static void limpiarBufferEntrada(void);
static bool leerEntero( const char *, int *, int, int );
static bool leerFlotante( const char *, float *, float );
static const char * obtenerNombreCategoria( int );
static void imprimirCabeceraTabla(void);
static void escribirListaEnArchivo( tLista *, const char *, const char * );

bool abrirArchivo( const char * ); /* Abre el archivo en el modo indicado; devuelve false si falla */
void cerrarArchivo(void);
void grabarProductosEnArchivo( tLista * );
void anadirProductosAlArchivo( tLista * );

void ingresarDatos(void);
void menu(void);
void buscarProductoYMostrar( int, tLista * );
void eliminarProducPorCod( int, tLista ** ); /* Elimina un producto por su codigo */
void verProductosGuardados(void);

void informeDeDatos(void);
void inicializarCorteControl(void);
void finalizarCorteControl(void);
void principioCorte(void);
void unProducto(void);
void finCorte(void);

/* Declaracion de variables globales */
FILE * archivoProductos = NULL;

tLista * listaProductos = NULL;
tDato rCorteProductos;
bool hayDatosCorte = false;
int codCampoControl;
tVectorStrings categorias = {
	"",
	"Comestibles",
	"Tecnologia",
	"Productos del Hogar",
	"Farmacia",
	"Ropa",
	"Otros"
};
int codigoProducConMenorCant, menorCantProduc;
char productConMenorCant[32]; /* Descripcion del producto con menor cantidad */
float porcentajeDeGananciaTotal, totalGanancia, costoTotal, costoPorCategoria, valorDelStockTotal, valorDelStockPorCateg;

/* Bloque principal */
int main(void) {
	inicializarLista( &listaProductos );
	menu();
	liberarLista( &listaProductos );
	return 0;
}

/* Helpers de entrada y formato */
static void limpiarBufferEntrada(void) {
	int c;
	while( (c = getchar()) != '\n' && c != EOF ) {
	}
}

static bool leerEntero( const char * pMensaje, int * pDestino, int pMin, int pMax ) {
	int resultado;
	while( true ) {
		printf("%s", pMensaje);
		resultado = scanf("%d", pDestino);
		if( resultado == EOF ) {
			return false;
		}
		limpiarBufferEntrada();
		if( resultado == 1 && *pDestino >= pMin && *pDestino <= pMax ) {
			return true;
		}
		printf("Valor invalido. Ingrese un numero entero entre %d y %d.\n", pMin, pMax);
	}
}

static bool leerFlotante( const char * pMensaje, float * pDestino, float pMin ) {
	int resultado;
	while( true ) {
		printf("%s", pMensaje);
		resultado = scanf("%f", pDestino);
		if( resultado == EOF ) {
			return false;
		}
		limpiarBufferEntrada();
		if( resultado == 1 && *pDestino >= pMin ) {
			return true;
		}
		printf("Valor invalido. Ingrese un numero mayor o igual a %.2f.\n", pMin);
	}
}

static const char * obtenerNombreCategoria( int pCodCategoria ) {
	if( pCodCategoria >= 1 && pCodCategoria <= CANT_CATEGORIAS ) {
		return categorias[pCodCategoria];
	}
	return "Categoria desconocida";
}

static void imprimirCabeceraTabla(void) {
	printf(" Cat | Codigo | Descripcion                 | Cant | Costo     | Precio\n");
	printf("--------------------------------------------------------------------------\n");
}

static void escribirListaEnArchivo( tLista * pLista, const char * pModo, const char * pMensajeExito ) {
	tLista * aux;

	aux = pLista;
	if( listaVacia( aux ) ) {
		printf("No hay productos que guardar!\n");
		return;
	}
	if( abrirArchivo( pModo ) ) {
		while( aux != NULL ) {
			fwrite(&aux->dato, sizeof(tDato), 1, archivoProductos);
			aux = aux->siguiente;
		}
		printf("%s\n", pMensajeExito);
		cerrarArchivo();
	}
}

/* Implementacion de funciones */

/* Funciones de lista enlazada */
void ingresarDatos(void) {
	tDato producto;
	size_t len;

	memset(&producto, 0, sizeof(tDato));

	if( !leerEntero("Ingrese codigo del producto:\n", &producto.codProducto, 1, 999999999) ) {
		return;
	}

	do {
		printf("Ingrese Descripcion del Producto (max. %d caracteres):\n", MAXCARACTERES - 1);
		if( fgets(producto.descripcion, sizeof(producto.descripcion), stdin) == NULL ) {
			return;
		}
		len = strlen(producto.descripcion);
		if( len > 0 && producto.descripcion[len - 1] == '\n' ) {
			producto.descripcion[len - 1] = '\0';
		} else {
			limpiarBufferEntrada();
		}
		if( producto.descripcion[0] == '\0' ) {
			printf("La descripcion no puede estar vacia.\n");
		}
	} while( producto.descripcion[0] == '\0' );

	if( !leerEntero("Ingrese la cantidad de producto:\n", &producto.cantidad, 0, 999999999) ) {
		return;
	}

	if( !leerEntero("Ingrese Categoria del Producto (1-Comestibles, 2-Tecnologia, 3-Productos del Hogar, 4-Farmacia, 5-Ropa, 6-Otros):\n",
			&producto.codCategoria, 1, CANT_CATEGORIAS) ) {
		return;
	}

	if( !leerFlotante("Ingrese el costo del producto:\n", &producto.costoDelProducto, 0.0f) ) {
		return;
	}

	if( !leerFlotante("Ingrese el precio del producto:\n", &producto.precio, 0.0f) ) {
		return;
	}

	insertarElem( producto, &listaProductos );
}

void menu(void) {
	int opcion;
	int codigo;
	int res;

	do {
		printf("\n==========================================================================\n");
		printf("                      TEAMCORESTOCK - MENU PRINCIPAL                      \n");
		printf("==========================================================================\n");
		printf(" 1 - Ingresar un producto a la lista\n");
		printf(" 2 - Buscar un producto por su codigo y mostrarlo\n");
		printf(" 3 - Eliminar un producto de la lista\n");
		printf(" 4 - Ver los productos en la lista (en memoria)\n");
		printf(" 5 - Anadir productos de la lista al archivo (%s)\n", NOMBREARCHIVO);
		printf(" 6 - Sobrescribir productos en el archivo (%s)\n", NOMBREARCHIVO);
		printf(" 7 - Ver productos guardados en el archivo\n");
		printf(" 8 - Ver informe de productos (Corte de control por categoria)\n");
		printf(" 0 - Finalizar programa\n");
		printf("==========================================================================\n");
		printf("Ingrese el numero para realizar la accion: ");

		res = scanf("%d", &opcion);
		if( res == EOF ) {
			break;
		}
		limpiarBufferEntrada();
		if( res != 1 ) {
			printf("Entrada invalida. Por favor ingrese un numero del 0 al 8.\n");
			opcion = -1;
			continue;
		}

		switch( opcion ) {
			case 1:
				ingresarDatos();
				break;
			case 2:
				if( leerEntero("Ingrese el codigo del producto para buscarlo:\n", &codigo, 1, 999999999) ) {
					buscarProductoYMostrar( codigo, listaProductos );
				}
				break;
			case 3:
				if( leerEntero("Ingrese el codigo de producto a eliminar:\n", &codigo, 1, 999999999) ) {
					eliminarProducPorCod( codigo, &listaProductos );
				}
				break;
			case 4:
				if( !listaVacia( listaProductos ) ) {
					printf("\n*** Productos en lista ***\n");
					imprimirCabeceraTabla();
					visualizarElem( listaProductos );
				} else {
					printf("No hay productos en la lista!\n");
				}
				break;
			case 5:
				anadirProductosAlArchivo( listaProductos ); /* Agrega al final del archivo */
				break;
			case 6:
				grabarProductosEnArchivo( listaProductos ); /* Sobrescribe el archivo */
				break;
			case 7:
				verProductosGuardados();
				break;
			case 8:
				informeDeDatos();
				break;
			case 0:
				printf("Finalizando programa...\n");
				break;
			default:
				printf("Opcion invalida. Seleccione un numero entre 0 y 8.\n");
				break;
		}
	} while( opcion != 0 );
}

void buscarProductoYMostrar( int pCodigo, tLista * pLista ) {
	tLista * aux;
	bool encontrado;

	aux = pLista;
	encontrado = false;

	if( listaVacia( aux ) ) {
		printf("Lista de productos sin elementos!\n");
		return;
	}

	while( aux != NULL ) {
		if( aux->dato.codProducto == pCodigo ) {
			if( !encontrado ) {
				printf("\n*** Producto encontrado ***\n");
				imprimirCabeceraTabla();
				encontrado = true;
			}
			imprimirProducto( &aux->dato );
		}
		aux = aux->siguiente;
	}
	if( !encontrado ) {
		printf("No se encontro ningun producto con el codigo %d en la lista.\n", pCodigo);
	}
}

void eliminarProducPorCod( int pCodigo, tLista ** pLista ) {
	tLista * aux;
	tLista * siguiente;
	int posicion;
	bool eliminado;

	aux = *pLista;
	posicion = 1;
	eliminado = false;

	if( listaVacia( aux ) ) {
		printf("Lista de productos sin elementos!\n");
		return;
	}

	while( aux != NULL ) {
		siguiente = aux->siguiente;
		if( aux->dato.codProducto == pCodigo ) {
			eliminarElemPos( posicion, pLista );
			eliminado = true;
		} else {
			posicion++;
		}
		aux = siguiente;
	}
	if( !eliminado ) {
		printf("No se encontro ningun producto con el codigo %d en la lista.\n", pCodigo);
	}
}

/* Funciones de archivos */
bool abrirArchivo( const char * pModoLectura ) {
	archivoProductos = fopen( NOMBREARCHIVO, pModoLectura );
	if( archivoProductos == NULL ) {
		printf("\nError al abrir el archivo '%s'!\n", NOMBREARCHIVO);
		return false;
	}
	printf("\nArchivo de productos abierto...\n");
	return true;
}

void grabarProductosEnArchivo( tLista * pLista ) {
	escribirListaEnArchivo( pLista, "wb", "Productos guardados (archivo sobrescrito)!" );
}

void anadirProductosAlArchivo( tLista * pLista ) {
	escribirListaEnArchivo( pLista, "ab", "Productos agregados al archivo!" );
}

void cerrarArchivo(void) {
	if( archivoProductos != NULL ) {
		fclose(archivoProductos);
		archivoProductos = NULL;
		printf("\nArchivo cerrado...\n");
	}
}

void verProductosGuardados(void) {
	tDato rLeerProduc;
	int cantidadLeida;

	if( abrirArchivo( "rb" ) ) {
		cantidadLeida = 0;
		printf("\n*** Productos guardados en %s ***\n", NOMBREARCHIVO);
		imprimirCabeceraTabla();
		while( fread(&rLeerProduc, sizeof(tDato), 1, archivoProductos) == 1 ) {
			imprimirProducto( &rLeerProduc );
			cantidadLeida++;
		}
		if( cantidadLeida == 0 ) {
			printf("(No hay productos registrados en el archivo)\n");
		}
		cerrarArchivo();
	}
}

/* Corte de control */
void informeDeDatos(void) {
	if( abrirArchivo( "rb" ) ) {
		inicializarCorteControl();
		if( !hayDatosCorte ) {
			printf("El archivo esta vacio. No hay datos para generar el informe.\n");
			cerrarArchivo();
			return;
		}

		printf("__________________________________________________________________________\n");
		printf("                      *** Informe de Productos ***                        \n");
		printf("__________________________________________________________________________\n");

		while( hayDatosCorte ) {
			principioCorte();

			while( hayDatosCorte && (rCorteProductos.codCategoria == codCampoControl) ) {
				unProducto();
				hayDatosCorte = (fread(&rCorteProductos, sizeof(tDato), 1, archivoProductos) == 1);
			}
			finCorte();
		}

		finalizarCorteControl();
		cerrarArchivo();
	}
}

void inicializarCorteControl(void) {
	costoTotal = 0.0f;
	valorDelStockTotal = 0.0f;
	costoPorCategoria = 0.0f;
	valorDelStockPorCateg = 0.0f;
	hayDatosCorte = (fread(&rCorteProductos, sizeof(tDato), 1, archivoProductos) == 1);
	if( hayDatosCorte ) {
		codCampoControl = rCorteProductos.codCategoria;
	}
}

void finalizarCorteControl(void) {
	totalGanancia = valorDelStockTotal - costoTotal;
	if( valorDelStockTotal != 0.0f ) {
		porcentajeDeGananciaTotal = (totalGanancia * 100.0f) / valorDelStockTotal;
	} else {
		porcentajeDeGananciaTotal = 0.0f;
	}
	printf("El valor total del Stock completo es de: $%.2f\n", valorDelStockTotal);
	printf("El costo del Stock completo es de: $%.2f\n", costoTotal);
	printf("La ganancia del Stock completo es de: $%.2f\n", totalGanancia);
	printf("El porcentaje de ganancia es de: %.2f%%\n", porcentajeDeGananciaTotal);
	printf("__________________________________________________________________________\n");
}

void principioCorte(void) {
	valorDelStockPorCateg = 0.0f;
	costoPorCategoria = 0.0f;
	codigoProducConMenorCant = rCorteProductos.codProducto;
	menorCantProduc = rCorteProductos.cantidad;
	snprintf(productConMenorCant, sizeof(productConMenorCant), "%s", rCorteProductos.descripcion);

	printf("\nCategoria %d: %s\n", codCampoControl, obtenerNombreCategoria(codCampoControl));
	imprimirCabeceraTabla();
}

void unProducto(void) {
	if( menorCantProduc > rCorteProductos.cantidad ) {
		codigoProducConMenorCant = rCorteProductos.codProducto;
		menorCantProduc = rCorteProductos.cantidad;
		snprintf(productConMenorCant, sizeof(productConMenorCant), "%s", rCorteProductos.descripcion);
	}
	valorDelStockPorCateg += (float)rCorteProductos.cantidad * rCorteProductos.precio;
	costoPorCategoria += (float)rCorteProductos.cantidad * rCorteProductos.costoDelProducto;
	imprimirProducto( &rCorteProductos );
}

void finCorte(void) {
	costoTotal += costoPorCategoria;
	valorDelStockTotal += valorDelStockPorCateg;
	printf("--------------------------------------------------------------------------\n");
	printf("La menor cantidad corresponde al producto '%s', codigo %d, cantidad %d\n",
		productConMenorCant, codigoProducConMenorCant, menorCantProduc);
	printf("El valor del Stock de esta categoria es de: $%.2f\n", valorDelStockPorCateg);
	printf("El costo del Stock de esta categoria es de: $%.2f\n", costoPorCategoria);
	printf("La ganancia estimada de esta categoria es de: $%.2f\n", valorDelStockPorCateg - costoPorCategoria);
	printf("__________________________________________________________________________\n");
	if( hayDatosCorte ) {
		codCampoControl = rCorteProductos.codCategoria;
	}
}
