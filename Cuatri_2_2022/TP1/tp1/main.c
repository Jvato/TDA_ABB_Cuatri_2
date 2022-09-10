#include "src/cajas.h"
#include "src/pokemon.h"
#include <stdio.h>



void _imprimir_pokemon(pokemon_t *pokemon);
void _imprimir_cadena(const char *titulo, const char *cadena_dato);
void _imprimir_numero(const char *titulo, const int dato);


int main(int argc, char *argv[]) {

	//En este archivo .c tenes que poner tu programa principal.

	//No se pide nada concreto, pero básicamente tenés que escribir un
	//programa de demostración que sirva para ver cómo funciona tu
	//biblioteca de cajas pokemon.

	//Por ejemplo podes leer dos o 3 archivos de cajas, mostrar su
	//contenido, combinar un par y mostrar el contenido de la caja
	//combinada.

	if (argc != 2) {
		fprintf(stderr,"ERROR: Falta ingresar el nombre del archivo\n");
		return 1;
	}
	caja_t *caja1 = caja_cargar_archivo(argv[1]);
	caja_t *caja2 = caja_cargar_archivo(argv[1]);
	caja_recorrer(caja1,_imprimir_pokemon);

	caja_guardar_archivo(caja1,"salida.txt");


	puts("--------------------------------------------");

	caja_t *caja = caja_combinar(caja1,caja2);
	caja_destruir(caja1);
	caja_destruir(caja2);

	caja_recorrer(caja,_imprimir_pokemon);

	caja_guardar_archivo(caja,"salida.txt");

	caja_destruir(caja);	
	return 0;
}



