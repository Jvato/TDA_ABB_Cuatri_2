#include "abb.h"
#include <stddef.h>
#include <stdlib.h>

abb_t *abb_crear(abb_comparador comparador)
{
	abb_t *abb = calloc(1,sizeof(abb_t));
	if (abb == NULL)
		return NULL;
	abb->comparador = comparador;
	return abb;
}

/*
 *	Crea un nodo del arbol con los hijos apuntando a NULL
 *	PRE: Recibe un elemento que sera almacenado en el nodo
 *	POST: devuelve un nuevo nodo del arbol que almacena el elemento, el hijo izquierdo e hijo derecho apuntan a NULL, o devuelve NULL en caso de error.
 */

nodo_abb_t *nodo_crear(void *elemento)
{
	nodo_abb_t *nodo = calloc(1,sizeof(nodo_abb_t));
	if (nodo == NULL)
		return NULL;
	nodo->elemento = elemento;
	return nodo;
}

/*
 * Agrega un nuevo elemento al arbol
 * PRE: el nodo y la funcion comparador existe o el nodo es NULL.
 * POST: Se agrega un elemento, en el orden correcto dado por la funcion comparador, o devuelve NULL en caso de error. 
 */

bool nodo_insertar(nodo_abb_t **ptr_nodo,abb_comparador comparador, void *elemento)
{
	if (*ptr_nodo == NULL)
	{
		*ptr_nodo = nodo_crear(elemento);
		return (*ptr_nodo == NULL) ? false : true;
	}
	
	nodo_abb_t **ptr_siguiente = (comparador((*ptr_nodo)->elemento,elemento) < 0) ? &((*ptr_nodo)->derecha) : &((*ptr_nodo)->izquierda);
	return nodo_insertar(ptr_siguiente,comparador,elemento);
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if ( arbol == NULL || arbol->comparador == NULL)
		return NULL;

	if ( !nodo_insertar(&(arbol->nodo_raiz),arbol->comparador,elemento) )
		return NULL;
	
	arbol->tamanio++;
	return arbol;
}

/*
 * 
 */

void *nodo_quitar_con_1_hijo(nodo_abb_t **ptr_nodo_actual)
{
	if (*ptr_nodo_actual == NULL)
		return NULL;
	nodo_abb_t *nodo_a_borrar = *ptr_nodo_actual;
		*ptr_nodo_actual = ( nodo_a_borrar->derecha )? ((*ptr_nodo_actual)->derecha) : ((*ptr_nodo_actual)->izquierda);
	void *elemento = nodo_a_borrar->elemento;
	free(nodo_a_borrar);
	return elemento;
}


void swap_elementos_nodo_arbol(nodo_abb_t *nodo_origen, nodo_abb_t *nodo_destino)
{
	void *elemento_aux = nodo_origen->elemento;
	nodo_origen->elemento = nodo_destino->elemento;
	nodo_destino->elemento = elemento_aux;
}

void *nodo_quitar_actual(nodo_abb_t **ptr_nodo_actual)
{
	if (*ptr_nodo_actual == NULL)
		return NULL;

	nodo_abb_t **ptr_nodo_a_borrar;
	bool tiene_2_hijos = ((*ptr_nodo_actual)->derecha != NULL && (*ptr_nodo_actual)->izquierda != NULL);

	if ( tiene_2_hijos )
	{
		ptr_nodo_a_borrar = &((*ptr_nodo_actual)->izquierda);
		while ((*ptr_nodo_a_borrar)->derecha != NULL)
			ptr_nodo_a_borrar = &((*ptr_nodo_a_borrar)->derecha);
		swap_elementos_nodo_arbol(*ptr_nodo_actual,*ptr_nodo_a_borrar);
	}
	else
		ptr_nodo_a_borrar = ptr_nodo_actual;

	return nodo_quitar_con_1_hijo(ptr_nodo_a_borrar);
}


nodo_abb_t **nodo_buscar(nodo_abb_t **ptr_nodo_actual, abb_comparador comparador, void *elemento)
{
	if (*ptr_nodo_actual == NULL)
		return ptr_nodo_actual;
	int resultado_comparar = comparador((*ptr_nodo_actual)->elemento,elemento);
	if (resultado_comparar == 0)
		return ptr_nodo_actual;
	nodo_abb_t **ptr_nodo_siguiente = (comparador((*ptr_nodo_actual)->elemento,elemento) < 0) ? &((*ptr_nodo_actual)->derecha) : &((*ptr_nodo_actual)->izquierda);
	return nodo_buscar(ptr_nodo_siguiente,comparador,elemento);
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if ( abb_vacio(arbol) || arbol->comparador == NULL)
		return NULL;

	nodo_abb_t **ptr_nodo = nodo_buscar(&(arbol->nodo_raiz),arbol->comparador,elemento);
	if (*ptr_nodo != NULL)
	{
		arbol->tamanio--;
		return nodo_quitar_actual(ptr_nodo);
	}
	return NULL;
}


void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (abb_vacio(arbol) || arbol->comparador == NULL)
		return NULL;
	nodo_abb_t **ptr_nodo = nodo_buscar(&(arbol->nodo_raiz),arbol->comparador,elemento);
	return (*ptr_nodo == NULL) ? NULL : (*ptr_nodo)->elemento;
}

bool abb_vacio(abb_t *arbol)
{
	return (arbol == NULL || (arbol->tamanio == 0 && arbol->nodo_raiz == NULL));
}

size_t abb_tamanio(abb_t *arbol)
{
	return ( arbol ) ? arbol->tamanio : 0;
}

void nodo_destruir_todo(nodo_abb_t *nodo,void (*destructor)(void *))
{
	if (nodo == NULL)
		return;
	nodo_abb_t *izquierda = nodo->izquierda;
	nodo_abb_t *derecha = nodo->derecha;

	if (destructor != NULL)
		destructor(nodo->elemento);
	free(nodo);

	nodo_destruir_todo(izquierda,destructor);
	nodo_destruir_todo(derecha,destructor);
}

void abb_destruir(abb_t *arbol)
{
	nodo_destruir_todo(arbol->nodo_raiz,NULL);
	free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	nodo_destruir_todo(arbol->nodo_raiz,destructor);
	free(arbol);
}

size_t nodo_con_cada_elemento_inorder(nodo_abb_t *nodo_actual, bool (*funcion)(void *, void *),void *aux)
{
	if (nodo_actual == NULL)
		return 0;
	size_t cantidad_elementos_recorrido = 0;
	cantidad_elementos_recorrido += nodo_con_cada_elemento_inorder(nodo_actual->izquierda,funcion,aux);
	if (funcion(nodo_actual->elemento,aux) == true)
		cantidad_elementos_recorrido += 1;
	else
		return cantidad_elementos_recorrido;
	cantidad_elementos_recorrido += nodo_con_cada_elemento_inorder(nodo_actual->derecha,funcion,aux);
	return cantidad_elementos_recorrido;
}

size_t nodo_con_cada_elemento_preorden(nodo_abb_t *nodo_actual, bool (*funcion)(void *, void *),void *aux)
{
	if (nodo_actual == NULL)
		return 0;
	size_t cantidad_elementos_recorrido = 0;
	if (funcion(nodo_actual->elemento,aux) == true)
		cantidad_elementos_recorrido += 1;
	else
		return cantidad_elementos_recorrido;
	cantidad_elementos_recorrido += nodo_con_cada_elemento_preorden(nodo_actual->izquierda,funcion,aux);
	cantidad_elementos_recorrido += nodo_con_cada_elemento_preorden(nodo_actual->derecha,funcion,aux);
	return cantidad_elementos_recorrido;
}

size_t nodo_con_cada_elemento_postorden(nodo_abb_t *nodo_actual, bool (*funcion)(void *, void *),void *aux)
{
	if (nodo_actual == NULL)
		return 0;
	size_t cantidad_elementos_recorrido = 0;
cantidad_elementos_recorrido += nodo_con_cada_elemento_preorden(nodo_actual->izquierda,funcion,aux);
	cantidad_elementos_recorrido += nodo_con_cada_elemento_preorden(nodo_actual->derecha,funcion,aux);
	if (funcion(nodo_actual->elemento,aux) == true)
		cantidad_elementos_recorrido += 1;

	return cantidad_elementos_recorrido;
}


size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (funcion == NULL)
		return 0;
	if (recorrido == INORDEN)
		return nodo_con_cada_elemento_inorder(arbol->nodo_raiz,funcion,aux);
	if (recorrido == PREORDEN)
		return nodo_con_cada_elemento_preorden(arbol->nodo_raiz,funcion,aux);
	if (recorrido == POSTORDEN)
		return nodo_con_cada_elemento_postorden(arbol->nodo_raiz,funcion,aux);
	return 0;
}

size_t nodo_recorrer_inorden(nodo_abb_t *nodo_actual,void **array,size_t tamanio_array, size_t tamanio_ocupado)
{
	if (nodo_actual == NULL || tamanio_ocupado == tamanio_array)
		return tamanio_ocupado;
	tamanio_ocupado = nodo_recorrer_inorden(nodo_actual->izquierda,array, tamanio_array,tamanio_ocupado);
	if (tamanio_ocupado == tamanio_array)
		return tamanio_ocupado;
	array[tamanio_ocupado++] = nodo_actual->elemento;
	return nodo_recorrer_inorden(nodo_actual->derecha,array, tamanio_array,tamanio_ocupado);
}

size_t nodo_recorrer_preorden(nodo_abb_t *nodo_actual,void **array,size_t tamanio_array, size_t tamanio_ocupado)
{
	if (nodo_actual == NULL || tamanio_ocupado == tamanio_array)
		return tamanio_ocupado;
	array[tamanio_ocupado++] = nodo_actual->elemento;
	tamanio_ocupado = nodo_recorrer_preorden(nodo_actual->izquierda,array, tamanio_array,tamanio_ocupado);
	if (tamanio_ocupado == tamanio_array)
		return tamanio_ocupado;
	return nodo_recorrer_preorden(nodo_actual->derecha,array, tamanio_array,tamanio_ocupado);
}

size_t nodo_recorrer_postorden(nodo_abb_t *nodo_actual,void **array,size_t tamanio_array, size_t tamanio_ocupado)
{
	if (nodo_actual == NULL || tamanio_ocupado == tamanio_array)
		return tamanio_ocupado;
	tamanio_ocupado = nodo_recorrer_postorden(nodo_actual->izquierda,array, tamanio_array,tamanio_ocupado);
	if (tamanio_ocupado == tamanio_array)
		return tamanio_ocupado;
	tamanio_ocupado = nodo_recorrer_postorden(nodo_actual->derecha,array, tamanio_array,tamanio_ocupado);
	if (tamanio_ocupado == tamanio_array)
		return tamanio_ocupado;
	array[tamanio_ocupado++] = nodo_actual->elemento;
	return tamanio_array;
}


size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (tamanio_array == 0 || array == NULL || abb_vacio(arbol) == true)
		return 0;
	if (recorrido == INORDEN)
		return nodo_recorrer_inorden(arbol->nodo_raiz,array,tamanio_array,0);
	if (recorrido == PREORDEN)
		return nodo_recorrer_preorden(arbol->nodo_raiz,array,tamanio_array,0);
	if (recorrido == POSTORDEN)	
		return nodo_recorrer_postorden(arbol->nodo_raiz,array,tamanio_array,0);
	return 0;
}
