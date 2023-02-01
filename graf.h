#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct grana {
	int broj;
	struct grana* sled;
}Grana;

typedef struct cvor {
	int broj;
	Grana* susedi;
	struct cvor* sled;
}Cvor;

typedef struct graf {
	int n;
	Cvor* cvorovi;
}Graf;

static int sledeciBroj = 0;

Cvor* dodajCvorUListu(Cvor* lst) {
	Cvor* novi = malloc(sizeof(Cvor));
	novi->broj = ++sledeciBroj;
	novi->susedi = NULL;
	novi->sled = NULL;
	if (lst == NULL)
		lst = novi;
	else {
		Cvor* posl = lst;
		while (posl->sled != NULL)
			posl = posl->sled;
		posl->sled = novi;
	}
	return lst;
}

Graf* kreirajGraf(int n) {
	if (n < 0) {
		printf("\nPocetni broj cvorova grafa mora biti >= od 0!");
		return NULL;
	}
	Graf *g = NULL;
	g = (Graf*)malloc(sizeof(Graf));
	if (g == NULL) {
		printf("\nGRESKA: Doslo je do greske prilikom kreiranja grafa.");
		return NULL;
	}
	g->n = n;
	g->cvorovi = NULL;
	for (int i = 0; i < n; i++) {
		g->cvorovi = dodajCvorUListu(g->cvorovi);
	}
	return g;
}

Grana* obrisiListuSuseda(Grana* susedi) {
	while (susedi != NULL) {
		Grana* stari = susedi;
		susedi = susedi->sled;
		free(stari);
	}
	return susedi;
}

Graf* obrisi(Graf* graf) {
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled)
		pom->susedi = obrisiListuSuseda(pom->susedi);
	Cvor* pom = graf->cvorovi;
	while (pom != NULL) {
		Cvor* stari = pom;
		pom = pom->sled;
		free(stari);
	}
	graf->cvorovi = NULL;
	free(graf);
	return NULL;
}

Graf* dodajCvor(Graf* graf) {
	graf->n++;
	graf->cvorovi = dodajCvorUListu(graf->cvorovi);
	return graf;
}

Graf* ukloniCvor(Graf* graf, int cvor) {
	int postoji = 0;
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled) {
		if (pom->broj == cvor) {
			postoji = 1;
			break;
		}
	}
	if (postoji == 0) {
		printf("\nCvor koji ste zeleli da obrisete ne postoji!");
		return graf;
	}
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled) {
		if (pom->broj == cvor) {
			continue;
		}
		if (pom->susedi != NULL) {
			if (pom->susedi->broj == cvor) {
				Grana* stari = pom->susedi;
				pom->susedi = stari->sled;
				free(stari);
			}
			else {
				Grana* pret = pom->susedi;
				while (pret->sled != NULL && pret->sled->broj != cvor)
					pret = pret->sled;
				if (pret->sled == NULL)
					continue;
				Grana* stari = pret->sled;
				pret->sled = stari -> sled;
				free(stari);
			}
		}
	}
	if (graf->cvorovi->broj == cvor) {
		Cvor* stari = graf->cvorovi;
		graf->cvorovi = stari->sled;
		obrisiListuSuseda(stari->susedi);
		free(stari);
	}
	else {
		Cvor* pret = graf->cvorovi;
		while (pret->sled != NULL && pret->sled->broj != cvor)
			pret = pret->sled;
		Cvor* stari = pret->sled;
		pret->sled = stari->sled;
		obrisiListuSuseda(stari->susedi);
		free(stari);
	}
	graf->n--;
	return graf;
}

Graf* dodajGranu(Graf* graf, int cvor1, int cvor2) {
	int nasao1 = 0, nasao2 = 0;
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled) {
		if (pom->broj == cvor1) nasao1 = 1;
		if (pom->broj == cvor2) nasao2 = 1;
	}
	if (nasao1 != 1 || nasao2 != 1) {
		printf("Greska, neki od cvorova ne postoji!");
		return graf;
	}
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled) {
		if (pom->broj == cvor1) {
			Grana* nova = malloc(sizeof(Grana));
			nova->broj = cvor2;
			nova->sled = NULL;
			Grana* g = pom->susedi;
			if (g == NULL) {
				pom->susedi = nova;
				return graf;
			}
			else {
				while (g->sled != NULL)
					g = g->sled;
				g->sled = nova;
				return graf;
			}

		}
	}

	return graf;
}

Graf* ukloniGranu(Graf* graf, int cvor1, int cvor2) {
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled) {
		if (pom->broj == cvor1 && pom->susedi != NULL) {
			if (pom->susedi->broj == cvor2) {
				Grana* stari = pom->susedi;
				pom->susedi = pom->susedi->sled;
				free(stari);
				return graf;
			}
			else {
				Grana* pret = pom->susedi;
				while (pret->sled != NULL && pret->sled->broj != cvor2)
					pret = pret->sled;
				if (pret->sled != NULL) {
					Grana* stari = pret->sled;
					pret->sled = stari->sled;
					free(stari);
					return graf;
				}
			}
		}
	}
	printf("\nBrisanje grane nije uspelo jer ista ne postoji!");
	return graf;
}

void ispis(Graf* graf) {
	printf("\nGraf sa %d cvorova, prikazan preko liste susednosti:\n", graf->n);
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled) {
		printf("\n%d", pom->broj);
		for (Grana* g = pom->susedi; g != NULL; g = g->sled) {
			printf(" -> %d", g->broj);
		}
	}
	printf("\n");
}

void ispisCvorova(Graf* graf) {
	if (graf == NULL) return;
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled)
		printf(" %d", pom->broj);
}

void zadatak1() {
	Graf* graf = NULL;
	int opcija;
	do {
		printf("\n\t*** MENI ***\n");
		printf("\n\t1 - Kreiraj graf");
		printf("\n\t2 - Dodaj cvor");
		printf("\n\t3 - Ukloni cvor");
		printf("\n\t4 - Dodaj granu");
		printf("\n\t5 - Ukloni granu");
		printf("\n\t6 - Ispisi graf");
		printf("\n\t7 - Obrisi graf");
		printf("\n\t0 - Povratak u glavni meni");
		printf("\n\tVas izbor: ");
		scanf("%d", &opcija);
		switch (opcija) {
		case 1:
			if (graf != NULL) {
				printf("\nGraf vec postoji, morate ga obrisati da biste kreirali novi!");
			}
			else {
				int n;
				printf("\nUnesite broj cvorova: ");
				scanf("%d", &n);
				graf = kreirajGraf(n);
			}
			break;
		case 2:
			if (graf == NULL) {
				printf("\nGraf ne postoji, morate ga kreirati da biste dodali cvor!");
			}
			else
				graf = dodajCvor(graf);
			break;
		case 3:
			if (graf == NULL) {
				printf("\nGraf ne postoji, morate ga kreirati da biste obisrali cvor!");
			}
			else {
				printf("\nUnesite redni broj cvora\nCvorovi u grafu:");
				ispisCvorova(graf);
				printf("\nVas izbor: ");
				int cvor;
				scanf("%d", &cvor);
				graf = ukloniCvor(graf, cvor);
			}
			break;
		case 4:
			if (graf == NULL) {
				printf("\nGraf ne postoji, morate ga kreirati da biste dodali granu!");
			}
			else {
				printf("\nUnesite redni broj prvog i drugog cvora:\nCvorovi u grafu:");
				ispisCvorova(graf);
				printf("\nVas izbor: ");
				int cvor1, cvor2;
				scanf("%d %d", &cvor1, &cvor2);
				graf = dodajGranu(graf, cvor1, cvor2);
			}
			break;
		case 5:
			if (graf == NULL) {
				printf("\nGraf ne postoji, morate ga kreirati da biste obrisali granu!");
			}
			else {
				printf("\nUnesite redni broj prvog i drugog cvora:\nCvorovi u grafu:");
				ispisCvorova(graf);
				printf("\nVas izbor: ");
				int cvor1, cvor2;
				scanf("%d %d", &cvor1, &cvor2);
				graf = ukloniGranu(graf, cvor1, cvor2);
			}
			break;
		case 6:
			if (graf == NULL) {
				printf("\nGraf ne postoji, morate ga kreirati da biste ga ispisali!");
			}
			else {
				ispis(graf);
			}
			break;
		case 7:
			if (graf == NULL) {
				printf("\nGraf ne postoji, morate ga kreirati da biste ga obrisali!");
			}
			else {
				graf = obrisi(graf);
			}
			break;
		case 0:
			if (graf != NULL) graf = obrisi(graf);
			break;
		default:
			printf("\nNe postoji definisana operacija za uneti broj, pokusajte ponovo.");
			break;
		}

	} while (opcija != 0);
}

int brojSusednihCvorova(Graf* graf, int cvor) {
	if (graf == NULL) {
		printf("\nNema table");
	}
	Cvor* trazeni = graf->cvorovi;
	while (trazeni != NULL && trazeni->broj != cvor)
		trazeni = trazeni->sled;
	if (trazeni == NULL) {
		printf("\nNe postoji cvor %d u grafu", cvor);
		return -1;
	}
	if (trazeni->susedi == NULL)
		return 0;
	int br = 0;
	for (Grana* g = trazeni->susedi; g != NULL; g = g->sled)
		br++;
	return br;
}

void vratiNizSusednihCvorova(Graf* graf, int cvor, int* niz) {
	if (graf == NULL)
		return;
	Cvor* trazeni = graf->cvorovi;
	while (trazeni != NULL && trazeni->broj != cvor)
		trazeni = trazeni->sled;
	if (trazeni == NULL) {
		return;
	}
	int poz = 0;
	for (Grana* g = trazeni->susedi; g != NULL; g=g->sled)
			niz[poz++] = g->broj;
}

int cvorUGrafu(Graf* graf, int cvor) {
	for (Cvor* pom = graf->cvorovi; pom != NULL; pom = pom->sled) {
		if (pom->broj == cvor)
			return 1;
	}
	return 0;
}