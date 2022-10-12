#include "src/abb.h"
#include <stdio.h>
#include <stdlib.h>
#include "pa2m.h"
#include <string.h>


int mi_strcmp(void *A, void *B)
{
	return strcmp(A,B);
}

void abb_se_creo_vacio()
{
	abb_t *abb = abb_crear(mi_strcmp);
	pa2m_afirmar(abb_vacio(abb), "El arbol se creo vacio");
	pa2m_afirmar(abb_tamanio(abb) == 0, "El arbol tiene cero elemento");
	abb_destruir(abb);
}

void insertar_elementos_no_retorna_NULL()
{
	abb_t *abb = abb_crear(mi_strcmp);
	pa2m_afirmar(abb_insertar(abb,"J") != NULL, "Se inserto \"J\" correctamente");
	pa2m_afirmar(abb_tamanio(abb) == 1, "El tamaño del arbol es 1");
	pa2m_afirmar(abb_insertar(abb,"O") != NULL, "Se inserto \"O\" correctamente");
	pa2m_afirmar(abb_tamanio(abb) == 2, "El tamaño del arbol es 2");
	pa2m_afirmar(abb_insertar(abb,"S") != NULL, "Se inserto \"S\" correctamente");
	pa2m_afirmar(abb_tamanio(abb) == 3, "El tamaño del arbol es 3");
	pa2m_afirmar(abb_insertar(abb,"E") != NULL, "Se inserto \"E\" correctamente");
	pa2m_afirmar(abb_tamanio(abb) == 4, "El tamaño del arbol es 4");
	pa2m_afirmar(abb_insertar(abb,"P") != NULL, "Se inserto \"P\" correctamente");
	pa2m_afirmar(abb_tamanio(abb) == 5, "El tamaño del arbol es 5");
	pa2m_afirmar(abb_insertar(abb,"H") != NULL, "Se inserto \"H\" correctamente");
	pa2m_afirmar(abb_tamanio(abb) == 6, "El tamaño del arbol es 6");
	
	abb_destruir(abb);
}

void insertar_en_arbol_NULL_retorna_NULL()
{
	pa2m_afirmar(abb_insertar(NULL,"hola") == NULL, "Insertar en arbol NULL retorna NULL");
}

int comparar_char(void *A, void *B)
{

	if (*((char *)A) == *((char *)B))
		return 0;
	return (*((char *)A) < *((char *)B)) ? -1 : 1;
}
abb_t *crear_abb_prueba(char *cadena)
{
	abb_t *abb = abb_crear(mi_strcmp);
	char *iterador = cadena;
	while (*iterador != '\0')
	{
		abb_insertar(abb,iterador);
		iterador++;
	}
	return abb;
}

void quitar_todos_los_elemento_deja_abb_vacio()
{
	abb_t *abb = crear_abb_prueba("josephvaler");

}



int main()
{
	pa2m_nuevo_grupo("Pruebas de creación de abb");
	abb_se_creo_vacio();

	pa2m_nuevo_grupo("Prueba insertar elementos no retorna error");
	insertar_elementos_no_retorna_NULL();

	pa2m_nuevo_grupo("Insertar elementos en arbol NULL da error");
	insertar_en_arbol_NULL_retorna_NULL();

	return pa2m_mostrar_reporte();
}
