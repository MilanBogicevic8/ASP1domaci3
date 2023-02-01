#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "graf.h"
#include "igra.h"

int main() {

	int opcija;
	do {
		printf("\n === GLAVNI MENI ===");
		printf("\n1 - Implementacija osnovnih algoritama");
		printf("\n2 - Ne ljuti se covece");
		printf("\n0 - Kraj rada programa");
		printf("\nVas izbor: ");
		scanf("%d", &opcija);
		if (opcija == 1) {
			zadatak1();
		}
		else if (opcija == 2) {
			zadatak2();
		}
		else if (opcija == 0) {
			break;
		}
		else {
			printf("\nPogresan unos, pokusajte opet");
		}
	} while (opcija != 0);
	return 0;
}