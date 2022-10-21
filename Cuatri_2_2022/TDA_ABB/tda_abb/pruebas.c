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

void abb_crear_con_comparador_NULL_devuelve_NULL()
{
	pa2m_afirmar(abb_crear(NULL) == NULL, "El arbol es NULL cuando el comparador es NULL");
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
	abb_t *abb = abb_crear(comparar_char);
	char *iterador = cadena;
	while (*iterador != '\0') {
		abb_insertar(abb,iterador);
		iterador++;
	}
	return abb;
}

void quitar_todos_los_elemento_deja_abb_vacio()
{
	char *cadena = "joseph";
	abb_t *abb = crear_abb_prueba(cadena);
	pa2m_afirmar(abb != NULL, "Se inserto los elementos \'j\'-\'o\'-\'s\'-\'e\'-\'p\'-\'h\'");
	pa2m_afirmar(abb_tamanio(abb) == 6, "El arbol tiene tamaño 6");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 0)) == 'j', "Se quito correctamente la \'j\'");
	pa2m_afirmar(abb_tamanio(abb) == 5, "El arbol tiene tamaño 5");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 1)) == 'o', "Se quito correctamente la \'o\'");
	pa2m_afirmar(abb_tamanio(abb) == 4, "El arbol tiene tamaño 4");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 2)) == 's', "Se quito correctamente la \'s\'");
	pa2m_afirmar(abb_tamanio(abb) == 3, "El arbol tiene tamaño 3");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 3)) == 'e', "Se quito correctamente la \'e\'");
	pa2m_afirmar(abb_tamanio(abb) == 2, "El arbol tiene tamaño 2");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 4)) == 'p', "Se quito correctamente la \'p\'");
	pa2m_afirmar(abb_tamanio(abb) == 1, "El arbol tiene tamaño 1");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 5)) == 'h', "Se quito correctamente la \'h\'");
	pa2m_afirmar(abb_tamanio(abb) == 0, "El arbol tiene tamaño 0");
	pa2m_afirmar(abb_vacio(abb) == true, "El arbol esta vacio");
	pa2m_afirmar(abb_quitar(abb,cadena) == NULL, "Quitar un elemento que ya no existe en el arbol devuelve NULL");
	abb_destruir(abb);	
}

void quitar_en_arbol_NULL_devuelve_NULL()
{
	pa2m_afirmar(abb_quitar(NULL,NULL) == NULL, "Quitar en arbol NULL devuelve NULL");
}

void quitar_en_arbol_vacio_devuelve_NULL()
{
	abb_t *abb = abb_crear(comparar_char);
	char a = 'a';
	pa2m_afirmar(abb_quitar(abb,&a) == NULL, "Quitar en arbol vacio devuelve NULL");
	abb_destruir(abb);
}

void insertar_y_quitar_elementos_repetidos_es_correcto()
{
	abb_t *abb = abb_crear(comparar_char);
	char *cadena = "1211";
	pa2m_afirmar(abb_insertar(abb,cadena + 0) != NULL, "Se inserto el 1 correctamente");		
	pa2m_afirmar(abb_tamanio(abb) == 1, "El arbol tiene tamaño 1");
	pa2m_afirmar(abb_insertar(abb,cadena + 1) != NULL, "Se inserto el 2 correctamente");		
	pa2m_afirmar(abb_tamanio(abb) == 2, "El arbol tiene tamaño 2");
	pa2m_afirmar(abb_insertar(abb,cadena + 2) != NULL, "Se inserto el 1 correctamente");		
	pa2m_afirmar(abb_tamanio(abb) == 3, "El arbol tiene tamaño 3");
	pa2m_afirmar(abb_insertar(abb,cadena + 3) != NULL, "Se inserto el 1 correctamente");
	pa2m_afirmar(abb_tamanio(abb) == 4, "El arbol tiene tamaño 4");

	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 3)) == '1', "El elemento quitado es \'1\'");
	pa2m_afirmar(abb_tamanio(abb) == 3, "El arbol tiene tamaño 3");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 0)) == '1', "El elemento quitado es \'1\'");
	pa2m_afirmar(abb_tamanio(abb) == 2, "El arbol tiene tamaño 2");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 1)) == '2', "El elemento quitado es \'2\'");
	pa2m_afirmar(abb_tamanio(abb) == 1, "El arbol tiene tamaño 1");
	pa2m_afirmar(*((char *)abb_quitar(abb,cadena + 2)) == '1', "El elemento quitado es \'1\'");
	pa2m_afirmar(abb_tamanio(abb) == 0, "El arbol tiene tamaño 0");

	pa2m_afirmar(abb_vacio(abb) == true, "El arbol esta vacio");
	abb_destruir(abb);
}

void insertar_en_arbol_NULL_devuelve_NULL()
{
	pa2m_afirmar(abb_quitar(NULL,"hola") == NULL, "Quitar un elemento de un arbol NULL devuelve NULL");
}

void buscar_elementos_devuelve_el_valor_correcto()
{
	char *cadena = "josephjames";
	abb_t *abb = crear_abb_prueba(cadena);

	pa2m_afirmar(abb != NULL, "Se inserto los elementos \'j\'-\'o\'-\'s\'-\'e\'-\'p\'-\'h\'-\'j\'-\'a\'-\'m\'-\'e\'-\'s\'");
	pa2m_afirmar(abb_buscar(abb,cadena+0) == cadena+0, "Buscar la \'j\' devuelve el elemento correcto");
	pa2m_afirmar(abb_buscar(abb,cadena+8) == cadena+8, "Buscar la \'m\' devuelve el elemento correcto");
	char z = 'z';
	pa2m_afirmar(abb_buscar(abb,&z) == NULL, "Buscar la \'z\' devuelve NULL");

	abb_destruir(abb);
}

bool es_par(void *valor,void *extra)
{
	return ((*(int *)valor % 2) == 0);
}

int comparar_enteros(void *A, void *B)
{
	if ((*(int *)A) == (*(int *)B))
		return 0;
	if ((*(int *)A) > (*(int *)B))
		return 1;
	return -1;
}

void aplicar_con_cada_elemento_devuelve_la_cantidad_correcta()
{
	abb_t *abb = abb_crear(comparar_enteros);
	pa2m_afirmar(abb_con_cada_elemento(abb,INORDEN,es_par,NULL) == 0, "Aplicar funcion con cada elemento devuelve cero cuando el arbol esta vacio y recorro INORDEN");
	pa2m_afirmar(abb_con_cada_elemento(abb,PREORDEN,es_par,NULL) == 0, "Aplicar funcion con cada elemento devuelve cero cuando el arbol esta vacio y recorro PREORDEN");
	pa2m_afirmar(abb_con_cada_elemento(abb,POSTORDEN,es_par,NULL) == 0, "Aplicar funcion con cada elemento devuelve cero cuando el arbol esta vacio y recorro POSTORDEN");
	int a = 100, b = 36, c = 21, d = 43, e = 120, f = 110, g = 121;
	void *_a = &a, *_b = &b, *_c = &c, *_d = &d, *_e = &e, *_f = &f, *_g = &g;
	abb_insertar(abb,_a);
	abb_insertar(abb,_b);
	abb_insertar(abb,_c);
	abb_insertar(abb,_d);
	abb_insertar(abb,_e);
	abb_insertar(abb,_f);
	abb_insertar(abb,_g);
	
	pa2m_afirmar(abb_con_cada_elemento(abb,INORDEN,es_par,NULL) == 0, "Con el recorrido INORDEN, el primer elemento es inpar, entonces devuelve cero");
	pa2m_afirmar(abb_con_cada_elemento(abb,PREORDEN,es_par,NULL) == 2, "Con el recorrido PREORDEN, los 2 primero elementos son PAR");
	pa2m_afirmar(abb_con_cada_elemento(abb,POSTORDEN,es_par,NULL) == 0, "Con el recorrido POSTORDEN, el primer elemento es impar");

	pa2m_afirmar(abb_con_cada_elemento(abb,EOF,es_par,NULL) == 0, "aplicar una funcion con cada elemento y un recorrido invalido devuelve 0");
	pa2m_afirmar(abb_con_cada_elemento(abb,INORDEN,NULL,NULL) == 0, "Aplicar la funcion NULL a cada elemento devuelve 0");
	abb_destruir(abb);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creación de abb");
	abb_se_creo_vacio();

	pa2m_nuevo_grupo("Prueba insertar elementos no retorna error");
	insertar_elementos_no_retorna_NULL();

	pa2m_nuevo_grupo("Insertar elementos en arbol NULL da error");
	insertar_en_arbol_NULL_retorna_NULL();

	pa2m_nuevo_grupo("Prueba insertar en arbol NULL");
	insertar_en_arbol_NULL_retorna_NULL();

	pa2m_nuevo_grupo("quita elementos de un arbol hasta que quede vacio");
	quitar_todos_los_elemento_deja_abb_vacio();

	pa2m_nuevo_grupo("Prueba quitar elemento en arbol NULL");
	quitar_en_arbol_NULL_devuelve_NULL();

	pa2m_nuevo_grupo("Prueba quitar en arbol vacio devuelve NULL");
	quitar_en_arbol_vacio_devuelve_NULL();

	pa2m_nuevo_grupo("Insertar y quitar elementos repetidos");
	insertar_y_quitar_elementos_repetidos_es_correcto();

	pa2m_nuevo_grupo("Prueba buscar elementos en el arbol");
	buscar_elementos_devuelve_el_valor_correcto();
	
	pa2m_nuevo_grupo("Prueba con comparador NULL");
	abb_crear_con_comparador_NULL_devuelve_NULL();

	pa2m_nuevo_grupo("Prueba aplicar funcion \"es_par\" con cada elemento");
	aplicar_con_cada_elemento_devuelve_la_cantidad_correcta();

	return pa2m_mostrar_reporte();
}
