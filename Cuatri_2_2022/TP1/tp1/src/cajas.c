#include "cajas.h"
#include "pokemon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define FACTOR_AUMENTO 2
#define CAPACIDAD_INICIAL 10U
#define MAX_LINEA 1048
#define CARACTER_DELIMITADOR ';'

#define ARCHIVO_OUT "Salida.txt"

struct _caja_t {
	pokemon_t ** caja;
	int cantidad;
	int capacidad;
};

caja_t *_crear_caja(void) {
	caja_t * caja = malloc(sizeof(caja_t));
	if (caja == NULL)
			return NULL;
	
	caja->caja = malloc(sizeof(pokemon_t*) * CAPACIDAD_INICIAL);
	if (caja->caja == NULL) {
			free(caja);
			return NULL;
	}
	
	caja->cantidad = 0;
	caja->capacidad = CAPACIDAD_INICIAL;
	
	return caja;
}

bool _redimensionar_caja(caja_t *caja) {
	pokemon_t **nueva_caja = realloc(caja->caja,(size_t)caja->capacidad * FACTOR_AUMENTO * sizeof(pokemon_t*));
	if (nueva_caja == NULL)
		return false;
	
	caja->caja = nueva_caja;
	caja->capacidad = caja->capacidad * FACTOR_AUMENTO;
	
	return true;
}

bool _caja_guardar_pokemon(caja_t *caja, pokemon_t *pokemon) {
	if (caja->cantidad >= caja->capacidad)
		if (_redimensionar_caja(caja) == false)
			return false;
	
	caja->caja[caja->cantidad] = pokemon;
	caja->cantidad++;
	
	return true;	

}

void _swap_pokemon_caja(caja_t *caja, int poscA, int poscB) {
	pokemon_t *pokemon_aux = caja->caja[poscA];
	caja->caja[poscA] = caja->caja[poscB];
	caja->caja[poscB] = pokemon_aux;
}

bool _ordenar_caja_por_nombre(caja_t *caja) {

	for (int final = caja->cantidad; final > 0; final--) {
		int maximo = final;
		const char *nombre_max = pokemon_nombre(caja_obtener_pokemon(caja,maximo - 1));
		for (int iterador = 1; iterador < final; iterador++) {
			const char *nombre = pokemon_nombre(caja_obtener_pokemon(caja,iterador - 1));
			if (strcmp(nombre,nombre_max) > 0) {
				maximo = iterador;
				nombre_max = pokemon_nombre(caja_obtener_pokemon(caja,maximo - 1));
			}
		}
		_swap_pokemon_caja(caja,maximo - 1,final - 1);
	}
	return true;
}

caja_t *caja_cargar_archivo(const char *nombre_archivo)
{
	if (nombre_archivo == NULL)
		return NULL;
	FILE * archivo = fopen(nombre_archivo,"r");
	if (archivo == NULL)
		return NULL;
	
	caja_t *caja = _crear_caja();
	if (caja == NULL) {
			fclose(archivo);
			return NULL;
	}

	char linea[MAX_LINEA];
	while (fgets(linea, MAX_LINEA,archivo) != NULL) {
			
		pokemon_t *pokemon = pokemon_crear_desde_string(linea);
		if (pokemon == NULL) {
			fclose(archivo);
			caja_destruir(caja);
			return NULL;
		}
	
		if (_caja_guardar_pokemon(caja,pokemon) == false) {
			pokemon_destruir(pokemon);
			fclose(archivo);
			caja_destruir(caja);
			return false;
		}
	}
	fclose(archivo);

	_ordenar_caja_por_nombre(caja);
	return caja;
}

void _crear_informacion_pokemon_con_formato (char *cadena, const char * nombre, int nivel, int ataque, int defensa) {
	sprintf(cadena,"%s%c%d%c%d%c%d\n", nombre,CARACTER_DELIMITADOR, nivel, CARACTER_DELIMITADOR, ataque, CARACTER_DELIMITADOR, defensa);
}

int caja_guardar_archivo(caja_t *caja, const char *nombre_archivo)
{
	if (nombre_archivo == NULL)
		return 0;
	FILE * archivo = fopen(nombre_archivo, "w");
	if (archivo == NULL)
		return 0;
	
	int iterador;
	for (iterador = 0; iterador < caja->cantidad; iterador++) {
		char cadena_aux[MAX_LINEA];
		const char *nombre = pokemon_nombre(caja->caja[iterador]);	
		int nivel = pokemon_nivel(caja->caja[iterador]);
		int ataque = pokemon_ataque(caja->caja[iterador]);
		int defensa = pokemon_defensa(caja->caja[iterador]);

		_crear_informacion_pokemon_con_formato(cadena_aux, nombre, nivel, ataque, defensa);
		if (fputs(cadena_aux,archivo) == 0)
			break;	
	}
	fclose(archivo);
	return iterador;
}


bool _caja_agregar_caja(caja_t *caja_destino, caja_t *caja_origen) {	
	for (size_t iterador = 0; iterador < caja_origen->cantidad; iterador++) {
		const char *nombre = pokemon_nombre(caja_origen->caja[iterador]);	
		int nivel = pokemon_nivel(caja_origen->caja[iterador]);
		int ataque = pokemon_ataque(caja_origen->caja[iterador]);
		int defensa = pokemon_defensa(caja_origen->caja[iterador]);

		char cadena_aux[MAX_LINEA];
		_crear_informacion_pokemon_con_formato(cadena_aux, nombre, nivel, ataque, defensa);
		pokemon_t *nuevo_pokemon = pokemon_crear_desde_string(cadena_aux);
		if (nuevo_pokemon == NULL)
				return false;

		if (_caja_guardar_pokemon(caja_destino, nuevo_pokemon) == false) {
				pokemon_destruir(nuevo_pokemon);
				return false;
		}
	}
	return true;
}

caja_t *caja_combinar(caja_t *caja1, caja_t *caja2)
{
	if (caja1 == NULL || caja2 == NULL)
		return NULL;
	caja_t *nueva_caja = _crear_caja();
	if (nueva_caja == NULL)
		return NULL;

	if (_caja_agregar_caja(nueva_caja,caja1) == false) {
		caja_destruir(nueva_caja);
		return NULL;
	}
	if (_caja_agregar_caja(nueva_caja,caja2) == false) {
		caja_destruir(nueva_caja);
		return NULL;
	}

	_ordenar_caja_por_nombre(nueva_caja);
	return nueva_caja;
		
}

int caja_cantidad(caja_t *caja)
{
	return (caja == NULL) ? 0 : caja->cantidad;
}

pokemon_t *caja_obtener_pokemon(caja_t *caja, int n)
{
	if (caja == NULL || n >= caja->cantidad || n < 0)
		return NULL;
	return caja->caja[n];
}

int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t *))
{
	if (caja == NULL || funcion == NULL)
		return 0;
	for (size_t i = 0; i < caja->cantidad; i++) {
		funcion(caja->caja[i]);
	}
	return (int)caja->cantidad;
}

void caja_destruir(caja_t *caja)
{
	if (caja != NULL) {
		caja_recorrer(caja,pokemon_destruir);
		free(caja->caja);
		free(caja);
	}
}
