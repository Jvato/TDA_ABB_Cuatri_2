#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

lista_t *lista_crear()
{
	lista_t *lista = malloc(sizeof(lista_t));
	if (lista == NULL)
		return NULL;
	lista->nodo_inicio = NULL;
	lista->nodo_fin = NULL;
	lista->cantidad = 0;
	return lista;
}

nodo_t *crear_nodo(void *elemento)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL)
		return NULL;

	nodo->elemento = elemento;
	nodo->siguiente = NULL;
	return nodo;
}

/**
 * inserta un nuevo elemento en la posicion del nodo "posicion" o al final en el caso que la posicion sea mayor a la lista de nodos
 * Pre: ptr_nodo es la direccion de una "lista" de nodos
 * Post: la "lista" de nodos tiene un elemento extra o devuelve NULL en caso de error
 *
 */

nodo_t *nodo_insertar_en_posicion(nodo_t **ptr_nodo, void *elemento, size_t posicion) 
{
	if (ptr_nodo == NULL)
		return NULL;
	if (*ptr_nodo == NULL || posicion == 0) 
	{
		nodo_t *nuevo_nodo = crear_nodo(elemento);
		if (nuevo_nodo == NULL)
			return NULL;
		nuevo_nodo->siguiente = *ptr_nodo;
		*ptr_nodo = nuevo_nodo;
		return nuevo_nodo;	
	}
	return nodo_insertar_en_posicion(&((*ptr_nodo)->siguiente),elemento,posicion - 1);
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (lista == NULL)
		return NULL;
	nodo_t **ptr_nodo_final = (lista_vacia(lista) == true) ?	&lista->nodo_fin : &lista->nodo_fin->siguiente;
	nodo_t *nuevo = crear_nodo(elemento);
	if (lista_vacia(lista) == true)
		lista->nodo_inicio = nuevo;
	*ptr_nodo_final = nuevo;
	lista->nodo_fin = nuevo;
	lista->cantidad++;
	return lista;
}


lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (lista == NULL)
		return NULL;
		
	nodo_t *nuevo_nodo = nodo_insertar_en_posicion(&lista->nodo_inicio, elemento, posicion);
	if (nuevo_nodo == NULL)
		return NULL;
// la posicion inicial se actualiza en la recursividad cuando la posicion en 0	
	if (posicion >= lista_tamanio(lista))
		lista->nodo_fin = nuevo_nodo;
	lista->cantidad++;

	return lista;	
}

/*
 * borra el nodo actual y reemplaza la posicion del actual con el nodo siguiente
 * PRE: el nodo existe y ptr_nodo apunta a este nodo
 * POST: se borra el nodo actual y se reemplaza con el siguiente
 */

void *borrar_nodo_actual(nodo_t **ptr_nodo)
{
	void *elemento = (*ptr_nodo)->elemento;
	nodo_t *a_borrar = *ptr_nodo;
	*ptr_nodo = a_borrar->siguiente;
	free(a_borrar);
	return elemento;
}

/*
 * Borra el el nodo actual cuando la posicion es cero o el ultimo en cuando la posicion es mayor al tamaño de lista
 * PRE: la lista no esta vacia y existe
 * POST: se borra el nodo actual de la lista cuando la posicion es cero o el ultimo si la posicion es mayor al tamaño y retorna el nodo anterior al que fue borrado, ademas se almacena el elemento del nodo borrado en ptr_elemento
 */

nodo_t *nodo_quitar_de_posicion(nodo_t **ptr_anterior, nodo_t **ptr_actual, nodo_t **ptr_siguiente, size_t posicion, void **ptr_elemento)
{
	if (ptr_siguiente == NULL || ptr_actual == NULL)
		return NULL;
	if ((*ptr_siguiente) == NULL || posicion == 0)
	{
		*ptr_elemento = borrar_nodo_actual(ptr_actual);
		return (ptr_anterior == NULL) ? NULL : *ptr_anterior;
	}
	return nodo_quitar_de_posicion(ptr_actual, ptr_siguiente, &((*ptr_siguiente)->siguiente), posicion - 1, ptr_elemento);
}

void *lista_quitar(lista_t *lista)
{
	return (lista == NULL) ? NULL : lista_quitar_de_posicion(lista, lista_tamanio(lista) - 1);
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista) == true)
		return NULL;
	void *elemento_de_nodo_borrado;
	nodo_t *nodo_anterior = nodo_quitar_de_posicion(NULL, &(lista->nodo_inicio),&(lista->nodo_inicio->siguiente),posicion,&elemento_de_nodo_borrado);
	if (posicion >= lista_tamanio(lista) - 1)
		lista->nodo_fin = nodo_anterior;
	lista->cantidad--;
	return elemento_de_nodo_borrado;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL)
		return NULL;
	if (posicion >= lista_tamanio(lista))
		return NULL;
	nodo_t *actual = lista->nodo_inicio;
	while (posicion-- != 0)
		actual = actual->siguiente;
	return actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	for (nodo_t *actual = lista->nodo_inicio; actual != NULL; actual = actual->siguiente){
		if (comparador != NULL)
			if (comparador(actual->elemento,contexto) == 0)
				return actual->elemento;
	}
	return NULL;
}

void *lista_primero(lista_t *lista)
{
	return (lista_vacia(lista)) ? NULL : lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	return (lista_vacia(lista)) ? NULL : lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	return (lista == NULL || (lista->nodo_inicio == NULL && lista->nodo_fin == NULL && lista->cantidad == 0));
}

size_t lista_tamanio(lista_t *lista)
{
	return (lista == NULL) ? 0 : lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	lista_destruir_todo(lista,NULL);
}
void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (lista != NULL)
	{
		while (lista->nodo_inicio != NULL)
		{
			nodo_t *nodo_a_borrar = lista->nodo_inicio;
			lista->nodo_inicio = nodo_a_borrar->siguiente;
			if (funcion != NULL)
				funcion(nodo_a_borrar->elemento);
			free(nodo_a_borrar);
		}
		free(lista);
	}
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL)
		return NULL;
	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (iterador == NULL)
		return NULL;
	iterador->corriente = lista->nodo_inicio;
	iterador->lista = lista;
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	return (iterador == NULL || iterador->corriente == NULL) ? false : true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (lista_iterador_tiene_siguiente(iterador) == false)
		return false;
	iterador->corriente = iterador->corriente->siguiente;
	return (iterador->corriente != NULL);
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	return (lista_iterador_tiene_siguiente(iterador)) ? iterador->corriente->elemento : NULL;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (lista == NULL || funcion == NULL)
		return 0;
	size_t i = 0;
	for (nodo_t *actual = lista->nodo_inicio; actual != NULL; actual = actual->siguiente, i++)
		if (funcion(actual->elemento,contexto) == false)
			return ++i;
	return i;
}

