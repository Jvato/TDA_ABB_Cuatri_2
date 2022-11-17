#include "hash.h"
#include "lista.h"
#include <string.h>

#define CAPACIDAD_MINIMA 3

typedef struct {
	char *clave;
	void *valor;
} par_t;

struct hash {
	lista_t **pares;
	size_t (*fh)(const char *);
	size_t cantidad;
	size_t capacidad;
};

// esta funcion de hasheo se llama FNV y la sacamos de este link
// https://www.programmingalgorithms.com/algorithm/fnv-hash/c/
// se le hizo unos cambios minimos, como cambiar el tipo del dato que devuelve a size_t 


size_t FNVHash(const char* str) {
	const unsigned int fnv_prime = 0x811C9DC5;
	size_t hash = 0;
	size_t i = 0;
	for (i = 0; i < strlen(str); str++, i++){
		hash *= fnv_prime;
		hash = hash^(size_t)(*str);
	}
	
	return hash;
}

int _strcmp(void *A, void *B) {
	return strcmp(A,B);
}

par_t *crear_par(const char *clave, void *valor) {
	char *clave_nueva = malloc(sizeof(char)*(strlen(clave) + 1));
	if (clave_nueva == NULL)
		return NULL;
	par_t *par = malloc(sizeof(par_t));
	if (par == NULL) {
		free(clave_nueva);
		return NULL;
	}
	strcpy(clave_nueva,clave);
	par->clave = clave_nueva;
	par->valor = valor;
	return par;
}

void *destruir_par(par_t *par) {
	free(par->clave);
	void *valor_a_borrar = par->valor;
	free(par);
	return valor_a_borrar;
}

hash_t *hash_crear(size_t capacidad)
{
	capacidad = (capacidad < CAPACIDAD_MINIMA)? CAPACIDAD_MINIMA : capacidad;
	lista_t **pares = calloc(capacidad,sizeof(lista_t *));
	if (pares == NULL)
		return NULL;

	hash_t *hash = malloc(sizeof(hash_t));
	if (hash == NULL) {
		free(pares);
		return NULL;
	}

	for (size_t i = 0; i < capacidad; i++) {
		pares[i] = lista_crear();
	}
	hash->pares = pares;
	hash->capacidad = capacidad;
	hash->cantidad = 0;
	hash->fh = FNVHash;

	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,void **anterior)
{
	if (hash_cantidad(hash) == 0)
		return NULL;
	size_t posc_clave = hash->fh(clave)%hash->capacidad;
	lista_iterador_t *iterador = lista_iterador_crear(hash->pares[posc_clave]);
	if (iterador == NULL)
		return NULL;

	while(lista_iterador_tiene_siguiente(iterador) == true) {
		par_t *par_actual = lista_iterador_elemento_actual(iterador);
		if (strcmp(par_actual->clave,clave) == 0) {
			lista_iterador_destruir(iterador);
			par_actual->valor = elemento;
			return hash;
		}
		lista_iterador_avanzar(iterador);
	}
	lista_iterador_destruir(iterador);
	par_t *par_nuevo = crear_par(clave,elemento);
	if (par_nuevo == NULL)
		return NULL;
	if (lista_insertar(hash->pares[posc_clave],par_nuevo) == NULL) {
		destruir_par(par_nuevo);
		return NULL;
	}
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (hash_cantidad(hash) == 0)
		return NULL;
	size_t posc_clave = hash->fh(clave)%hash->capacidad;
	
	if (lista_buscar_elemento(hash->pares[posc_clave],_strcmp,(void *)clave) == NULL)
		return NULL;

	lista_iterador_t *iterador = lista_iterador_crear(hash->pares[posc_clave]);
	if (iterador == NULL)
		return NULL;
	size_t posc_en_lista;
	for (posc_en_lista = 0; lista_iterador_tiene_siguiente(iterador) == true; posc_en_lista++) {
		if (strcmp(clave,((par_t *)lista_iterador_elemento_actual(iterador))->clave) == 0)
				break;
		lista_iterador_avanzar(iterador);
	}
		
	return lista_quitar_de_posicion(hash->pares[posc_clave],posc_en_lista);
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (hash_cantidad(hash) == 0)
		return NULL;
	size_t posc_clave = hash->fh(clave)%hash->capacidad;
	lista_iterador_t *iterador = lista_iterador_crear(hash->pares[posc_clave]);
	if (iterador == NULL)
		return NULL;

	void *valor_a_devolver = NULL;
	while(lista_iterador_tiene_siguiente(iterador) == true) {
		par_t *par_actual = lista_iterador_elemento_actual(iterador);
		if (strcmp(par_actual->clave,clave) == 0) {
			valor_a_devolver = par_actual->valor;
			break;
		}
		lista_iterador_avanzar(iterador);
	}
	lista_iterador_destruir(iterador);
	return valor_a_devolver;
}



bool hash_contiene(hash_t *hash, const char *clave)
{
	if (hash_cantidad(hash) == 0)
		return false;

	size_t posc_clave = hash->fh(clave)%hash->capacidad;
	return (lista_buscar_elemento(hash->pares[posc_clave],_strcmp,(char *)clave) == NULL) ? false : true;
}

size_t hash_cantidad(hash_t *hash)
{
	return ((hash == NULL)? 0 : hash->cantidad);
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash,NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (hash == NULL)
		return;
	for (size_t i = 0; i < hash->capacidad; i++) {
		lista_destruir_todo(hash->pares[i],destructor);
	}
	free(hash->pares);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	if (hash == NULL || f == NULL)
		return 0;
	size_t contador = 0;
	for (size_t posc = 0; posc < hash->capacidad; posc++) {
		lista_iterador_t *iterador = lista_iterador_crear(hash->pares[posc]);
		if (iterador == NULL)
			return contador;
		while (lista_iterador_tiene_siguiente(iterador) == true) {
			par_t *actual = lista_iterador_elemento_actual(iterador);
			if (actual == NULL) {
				lista_iterador_destruir(iterador);
				return contador;
			}
			contador++;
			if (f(actual->clave,actual->valor,aux) == false){
				lista_iterador_destruir(iterador);
				return contador;
			}
		lista_iterador_avanzar(iterador);
		}
		lista_iterador_destruir(iterador);
	}
	return contador;
}
