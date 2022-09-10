#include "pokemon.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_NOMBRE_POKEMON 30
#define MAX_STRING_NUMERO 10
#define CARACTER_DELIMITADOR ';'

struct _pokemon_t {
	char nombre[MAX_NOMBRE_POKEMON];
	int nivel;
	int ataque;
	int defensa;
};

pokemon_t *pokemon_crear_desde_string(char *string)
{
	if (string == NULL)
		return NULL;
	pokemon_t *pokemon = malloc(sizeof(pokemon_t));
	if (pokemon == NULL)
		return NULL;

	size_t iterador;
	for (iterador= 0; string[iterador] != CARACTER_DELIMITADOR; iterador++)
		pokemon->nombre[iterador] = string[iterador];
	if (iterador == 0) {
		free(pokemon);
		return NULL;
	}
	pokemon->nombre[iterador] = '\0';

	char *endptr;
	char *inicio;

	inicio = string + iterador + 1;
	pokemon->nivel = (int)strtol(inicio,&endptr,10);
	if (*endptr != CARACTER_DELIMITADOR || inicio == endptr) {
		free(pokemon);
		return NULL;
	}

	inicio = endptr + 1;
	pokemon->ataque = (int)strtol(inicio, &endptr, 10);
	if (*endptr != CARACTER_DELIMITADOR || inicio == endptr) {
		free(pokemon);
		return NULL;
	}
	inicio = endptr + 1;
	pokemon->defensa = (int)strtol(inicio, &endptr, 10);
	if (inicio == endptr) {
		free(pokemon);
		return NULL;
	}
	return pokemon;
}

int pokemon_nivel(pokemon_t *pokemon)
{
	return (pokemon == NULL) ? 0 : pokemon->nivel;
}

int pokemon_ataque(pokemon_t *pokemon)
{
	return (pokemon == NULL) ? 0 : pokemon->ataque;
}

int pokemon_defensa(pokemon_t *pokemon)
{
	return (pokemon == NULL) ? 0 : pokemon->defensa;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	return (pokemon == NULL) ? NULL : pokemon->nombre;
}

void pokemon_destruir(pokemon_t *pokemon)
{
		free(pokemon);
}

void _imprimir_pokemon(pokemon_t *pokemon) {
		puts("/*****************************************************/");
		printf("\tnombre de pokemon: %s\n", pokemon->nombre);
		printf("\tNivel de pokemon: %i\n",pokemon->nivel);
		printf("\tAtaque de pokemon: %i\n",pokemon->ataque);
		printf("\tdefensa de pokemon: %i\n",pokemon->defensa);
		puts("/*****************************************************/");
		puts("");
}


void _imprimir_cadena(const char *titulo, const char *cadena_dato) {
		printf("\t/************** %s **************/\n", titulo);
		printf("\t\t\tdato: %s\n",cadena_dato);
		printf("\t/********************************************/\n");
		puts("");
}

void _imprimir_numero(const char *titulo, const int dato) {
		printf("\t/************** %s **************/\n", titulo);
		printf("\t\t\tdato: %i\n",dato);
		printf("\t/********************************************/\n");
		puts("");
}
