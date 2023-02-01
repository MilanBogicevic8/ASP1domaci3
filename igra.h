#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "graf.h";

typedef struct put {
	int polje;
	int potez;
	struct put* sled;
}Put;

typedef struct putanje {
	Put* put;
	int zavrsen; // -1 : ne moze se doci do cilja, 0 : nije jos zavrsen, 1 : zavrsen
	struct putanje* sled;
}Putanje;

Graf* unosParametara(char* igrac1, char* igrac2, int* start, int* cilj) {
	Graf* tabla = NULL;

	printf("\nUnesite ime prvog igraca: ");
	scanf(" %s", igrac1);
	printf("\nUnesite ime drugog igraca: ");
	scanf(" %s", igrac2);

	int brojPolja;
	printf("\nUnesite broj polja na tabli: ");
	scanf(" %d", &brojPolja);
	tabla = kreirajGraf(brojPolja);

	int opcija;
	printf("\nUnesite puteve izmedju dva polja");
	do {
		printf("\n1 - Dodaj put");
		printf("\n0 - Kraj unosa");
		printf("\nIzbor: ");
		scanf(" %d", &opcija);
		if (opcija == 1) {
			printf("\nUnesite broj prvog i drugog polja:\nMoguca polja:");
			ispisCvorova(tabla);
			printf("\nVas izbor: ");
			int c1, c2;
			scanf(" %d %d", &c1, &c2);
			tabla = dodajGranu(tabla, c1, c2);
		}
	} while (opcija != 0);

	int dobro;
	do {
		dobro = 1;
		printf("\nUnesite broj startnog i ciljnog polja:\nMoguca polja:");
		ispisCvorova(tabla);
		printf("\nVas izbor: ");
		scanf(" %d %d", start, cilj);
		if (cvorUGrafu(tabla, *start) == 0 || cvorUGrafu(tabla, *cilj) == 0) {
			ispis(tabla);
			dobro = 0;
			printf("\nLos unos, polja %d i %d se ne nalaze na tabli!", *start, *cilj);
		}
	} while (dobro != 1);

	return tabla;

}

Putanje* init(int start) {
	Put* p = NULL;
	p = (Put*)malloc(sizeof(Put));
	p->polje = start;
	p->potez = 0;
	p->sled = NULL;
	Putanje* lP = NULL;
	lP = (Putanje*)malloc(sizeof(Putanje));
	lP->put = p;
	lP->zavrsen = 0;
	lP->sled = NULL;
	return lP;
}

Put* dodajPoljeUPut(Put* put, int polje, int potez) {
	Put* novi = NULL;
	novi = (Put*)malloc(sizeof(Put));
	novi->polje = polje;
	novi->potez = potez;
	novi->sled = NULL;
	if (put == NULL) {
		put = novi;
	}
	else {
		Put* posl = put;
		while (posl->sled != NULL)
			posl = posl->sled;
		posl->sled = novi;
	}
	return put;
}
Put* kopirajPut(Put* put) {
	Put* kopija = NULL;
	for (Put* pom = put; pom != NULL; pom = pom->sled) {
		kopija = dodajPoljeUPut(kopija, pom->polje, pom->potez);
	}
	return kopija;
}
Put* nadoveziPut(Put* put1, Put* put2) {
	if (put2 == NULL)
		return put1;
	if (put1 == NULL)
		return put2;
	Put* posl = put1;
	while (posl->sled != NULL)
		posl = posl->sled;
	posl->sled = put2;
	return put1;
}
int poslednjePolje(Put* put) {
	if (put == NULL)
		return -1;
	Put* posl = put;
	while (posl->sled != NULL) {
		posl = posl->sled;
	}
	return posl->polje;
}
int postojiPoljeUPutu(Put* put, int polje) {
	for (Put* pom = put; pom != NULL; pom = pom->sled)
		if (pom->polje == polje)
			return 1;
	return 0;
}
int poljeUPutuNaNeparnomPotezu(Put* put, int polje) {
	for (Put* pom = put; pom != NULL; pom = pom->sled)
		if (pom->polje == polje && pom->potez % 2 != 0)
			return 1;
	return 0;
}
int poljeUPutuNaIzlazuParnogPoteza(Put* put, int polje) {
	for (Put* pom = put; pom != NULL; pom = pom->sled)
		if (pom->potez != 0 && pom->polje == polje && pom->potez % 2 == 0 && (pom->sled == NULL || pom->sled->potez % 2 != 0))
			return 1;
	return 0;
}
Put* izbrisiPut(Put* put) {
	while (put != NULL) {
		Put* stari = put;
		put = put->sled;
		free(stari);
	}
	return NULL;
}
int brojPoslednjegPoteza(Put* put) {
	if (put == NULL)
		return -1;
	Put* posl = put;
	while (posl->sled != NULL) {
		posl = posl->sled;
	}
	return posl->potez;
}
void ispisiPut(Put* put) {
	if (put != NULL) {
		printf("\nPut: %d", put->polje);
		Put* pom = put->sled;
		while (pom != NULL) {
			printf(" -> %d", pom->polje);
			pom = pom->sled;
		}
	}
	else {
		printf("\nPut nepostoji!");
	}
}

Putanje* dodajPutUPutanje(Putanje* putanje, Put* put) {
	Putanje* posl = putanje;
	Putanje* novi = NULL;
	novi = (Putanje*)malloc(sizeof(Putanje));
	novi->put = put;
	novi->zavrsen = 0;
	novi->sled = NULL;
	if (putanje == NULL) {
		putanje = novi;
	}
	else {
		Putanje* posl = putanje;
		while (posl->sled != NULL)
			posl = posl->sled;
		posl->sled = novi;
	}
	return putanje;
}
int brojPuteva(Putanje* putanje) {
	int br = 0;
	Putanje* pom = putanje;
	while (pom != NULL) {
		br++;
		pom = pom->sled;
	}
	return br;
}
Putanje* napraviPutanjeDuzine2(int polje, Graf* tabla, int potez) {
	Putanje* putanje = NULL;
	int brSusednihPolja1 = brojSusednihCvorova(tabla, polje);
	if (brSusednihPolja1 <= 0)
		return NULL;
	int* susedi1;
	susedi1 = (int*)calloc(brSusednihPolja1, sizeof(int));
	vratiNizSusednihCvorova(tabla, polje, susedi1);
	for (int i = 0; i < brSusednihPolja1; i++) {
		int brSusednihPolja2 = brojSusednihCvorova(tabla, susedi1[i]);
		if (brSusednihPolja2 <= 0)
			continue;
		int* susedi2;
		susedi2 = (int*)calloc(brSusednihPolja2, sizeof(int));
		vratiNizSusednihCvorova(tabla, susedi1[i], susedi2);
		for (int j = 0; j < brSusednihPolja2; j++) {
			Put* p = NULL;
			p = dodajPoljeUPut(p, susedi1[i], potez);
			p = dodajPoljeUPut(p, susedi2[j], potez);
			putanje = dodajPutUPutanje(putanje, p);
		}
	}
	return putanje;
}
Putanje* izbaciNezavrsene(Putanje* putanje) {
	while (putanje != NULL && putanje->zavrsen != 1) {
		Putanje* stari = putanje;
		putanje = putanje->sled;
		stari->put = izbrisiPut(stari->put);
		free(stari);
	}
	if (putanje == NULL)
		return NULL;
	for (Putanje* pom = putanje; pom->sled != NULL;) {
		if (pom->sled->zavrsen != 1) {
			Putanje* stari = pom->sled;
			pom->sled = stari->sled;
			free(stari);
		}
		else
			pom = pom->sled;
	}
	return putanje;
}
Putanje* ostaviValidneDuzine2(Putanje* putanje, Put* put) {
	for (Putanje* pom = putanje; pom != NULL; pom = pom->sled) {
		int poslednje = poslednjePolje(pom->put);
		if (poljeUPutuNaIzlazuParnogPoteza(put, poslednje) == 0) {
			pom->zavrsen = 1;
		}
	}
	putanje = izbaciNezavrsene(putanje);
	return putanje;
}
Putanje* ostaviMoguce(Putanje* putanje) {
	if (putanje == NULL)
		return NULL;
	while (putanje != NULL && putanje->zavrsen == -1) {
		Putanje* stari = putanje;
		putanje = putanje->sled;
		stari->put = izbrisiPut(stari->put);
		free(stari);
	}
	if (putanje == NULL)
		return NULL;
	for (Putanje* pom = putanje; pom->sled != NULL;) {
		if (pom->sled->zavrsen == -1) {
			Putanje* stari = pom->sled;
			pom->sled = stari->sled;
			stari->put = izbrisiPut(stari->put);
			free(stari);
		}
		else {
			pom = pom->sled;
		}
	}
	return putanje;
}
Putanje* updateDolaska(Putanje* putanje, int cilj, int potez) {
	for (Putanje* pom = putanje; pom != NULL; pom = pom->sled) {
		int poslednji = poslednjePolje(pom->put);
		if (poslednji == cilj)
			pom->zavrsen = 1;
	}
	return putanje;
}
int postojiZavrsenPut(Putanje* putanje) {
	for (Putanje* pom = putanje; pom != NULL; pom = pom->sled)
		if (pom->zavrsen == 1)
			return 1;
	return 0;
}


Putanje* izbaciPut(Putanje* putanje, Putanje* zaBrisanje) {
	if (putanje == NULL)
		return NULL;
	if (putanje == zaBrisanje) {
		putanje = putanje->sled;
		zaBrisanje->put = (zaBrisanje->put);
		free(zaBrisanje);
		return putanje;
	}
	Putanje* stari = putanje;
	while (stari->sled != NULL && stari->sled != zaBrisanje)
		stari = stari->sled;
	if (stari->sled != NULL) {
		stari->sled = stari->sled->sled;
		zaBrisanje->put = izbrisiPut(stari->put);
		free(zaBrisanje);
	}
	return putanje;
}


Putanje* odigrajPotezA(Putanje* putanje, int rbPoteza, int cilj, Graf* tabla, int* zavrsio) {
	int brPuteva = brojPuteva(putanje);
	Putanje* pom = putanje;
	for (int i = 0; i < brPuteva; i++) {
		if (pom->zavrsen == -1) {
			pom = pom->sled;
			continue;
		}
		int trenutnoPolje = poslednjePolje(pom->put);
		int brojSusednihPolja = brojSusednihCvorova(tabla, trenutnoPolje);
		if (brojSusednihPolja <= 0) {
			pom->zavrsen = -1;
			pom = pom->sled;
			continue;
		}
		int* susedi;
		susedi = (int*)calloc(brojSusednihPolja, sizeof(int));
		vratiNizSusednihCvorova(tabla, trenutnoPolje, susedi);
		for (int j = 0; j < brojSusednihPolja; j++)
			if (postojiPoljeUPutu(pom->put, susedi[j]) == 1) {
				susedi[j] = susedi[brojSusednihPolja - 1];
				brojSusednihPolja--;
				j--;
			}
		if (brojSusednihPolja > 0) {
			for (int j = 1; j < brojSusednihPolja; j++) {
				Put* kopija = kopirajPut(pom->put);
				kopija = dodajPoljeUPut(kopija, susedi[j], rbPoteza);
				putanje = dodajPutUPutanje(putanje, kopija);
			}
			pom->put = dodajPoljeUPut(pom->put, susedi[0], rbPoteza);
		}
		else {
			pom->zavrsen = -1;
		}
		free(susedi);
		pom = pom->sled;
	}
	putanje = updateDolaska(putanje, cilj, rbPoteza);
	putanje = ostaviMoguce(putanje);
	*zavrsio = postojiZavrsenPut(putanje);
	return putanje;
}
Putanje* neparanPotezB(Putanje* putanje, int rbPoteza, int cilj, Graf* tabla, int* zavrsio) {
	int brPuteva = brojPuteva(putanje);
	Putanje* pom = putanje;
	for (int i = 0; i < brPuteva; i++) {
		if (pom->zavrsen == -1) {
			pom = pom->sled;
			continue;
		}
		int trenutnoPolje = poslednjePolje(pom->put);
		int brojSusednihPolja = brojSusednihCvorova(tabla, trenutnoPolje);
		if (brojSusednihPolja <= 0) {
			pom->zavrsen = -1;
			pom = pom->sled;
			continue;
		}
		int* susedi;
		susedi = (int*)calloc(brojSusednihPolja, sizeof(int));
		vratiNizSusednihCvorova(tabla, trenutnoPolje, susedi);
		for (int j = 0; j < brojSusednihPolja; j++)
			if (poljeUPutuNaNeparnomPotezu(pom->put, susedi[j]) == 1) {
				susedi[j] = susedi[brojSusednihPolja - 1];
				brojSusednihPolja--;
				j--;
			}
		if (brojSusednihPolja > 0) {
			for (int j = 1; j < brojSusednihPolja; j++) {
				Put* kopija = kopirajPut(pom->put);
				kopija = dodajPoljeUPut(kopija, susedi[j], rbPoteza);
				putanje = dodajPutUPutanje(putanje, kopija);
			}
			pom->put = dodajPoljeUPut(pom->put, susedi[0], rbPoteza);
		}
		else {
			pom->zavrsen = -1;
		}
		free(susedi);
		pom = pom->sled;
	}
	putanje = updateDolaska(putanje, cilj, rbPoteza);
	putanje = ostaviMoguce(putanje);
	*zavrsio = postojiZavrsenPut(putanje);
	return putanje;
}
Putanje* paranPotezB(Putanje* putanje, int rbPoteza, int cilj, Graf* tabla, int* zavrsio) {
	int brPuteva = brojPuteva(putanje);
	Putanje* pom = putanje;
	for (int i = 0; i < brPuteva; i++) {
		if (pom->zavrsen == -1) {
			pom = pom->sled;
			continue;
		}
		int trenutnoPolje = poslednjePolje(pom->put);
		Putanje* putevi2 = napraviPutanjeDuzine2(trenutnoPolje, tabla, rbPoteza);
		putevi2 = ostaviValidneDuzine2(putevi2, pom->put);
		int brojPuteva2 = brojPuteva(putevi2);
		if (brojPuteva2 == 0) {
			pom->zavrsen = -1;
		}
		else {
			Putanje* t = putevi2->sled;
			for (int j = 1; j < brojPuteva2; j++) {
				Put* kopija = kopirajPut(pom->put);
				kopija = nadoveziPut(kopija, t->put);
				putanje = dodajPutUPutanje(putanje, kopija);
				t = t->sled;
			}
			pom->put = nadoveziPut(pom->put, putevi2->put);
		}
		pom = pom->sled;
	}
	putanje = updateDolaska(putanje, cilj, rbPoteza);
	putanje = ostaviMoguce(putanje);
	*zavrsio = postojiZavrsenPut(putanje);
	return putanje;
}
Putanje* odigrajPotezB(Putanje* putanje, int rbPoteza, int cilj, Graf* tabla, int* zavrsio) {
	if (rbPoteza % 2 == 0)
		return paranPotezB(putanje, rbPoteza, cilj, tabla, zavrsio);
	else
		return neparanPotezB(putanje, rbPoteza, cilj, tabla, zavrsio);
}

Putanje* obrisiPutanje(Putanje* p) {
	while (p != NULL) {
		Putanje* stari = p;
		p = p->sled;
		stari->put = izbrisiPut(stari->put);
		free(stari);
	}
}


void zadatak2() {
	char igrac1[20], igrac2[20];
	Graf* tabla = NULL;
	int start, cilj;
	int rbPoteza = 0;

	int zavrsioA = 0, zavrsioB = 0;


	Putanje *prvi = NULL;
	Putanje *drugi = NULL;

	tabla = unosParametara(igrac1, igrac2, &start, &cilj);

	prvi = init(start);
	drugi = init(start);

	while (zavrsioA == 0 || zavrsioB == 0) {
		rbPoteza++;
		if (zavrsioA == 0) {
			prvi = odigrajPotezA(prvi, rbPoteza, cilj, tabla, &zavrsioA);
			if (prvi == NULL)
				zavrsioA = -1;
			if (zavrsioA == 1) {
				prvi = izbaciNezavrsene(prvi);
			}
		}
		if (zavrsioB == 0) {
			drugi = odigrajPotezB(drugi, rbPoteza, cilj, tabla, &zavrsioB);
			if (drugi == NULL)
				zavrsioB = -1;
			if (zavrsioB == 1) {
				drugi = izbaciNezavrsene(drugi);
			}
		}
	}

	if (zavrsioA == -1 || zavrsioB == -1) {
		if (zavrsioA == -1 && zavrsioB == -1) {
			printf("\nU igri nema pobednika");
		}
		else if (zavrsioA == -1) {
			printf("\nIgrac %s je izgubio", igrac1);
		}
		else {
			printf("\nIgrac %s je izgubio", igrac2);
		}
	}

	if (zavrsioA == 1 && zavrsioB == 1) {
		int a = brojPoslednjegPoteza(prvi->put);
		int b = brojPoslednjegPoteza(drugi->put);
		if (a < b) {
			printf("\nPOBEDNIK IGRE JE %s", igrac1);
		}
		else if (b < a) {
			printf("\nPOBEDNIK IGRE JE %s", igrac2);
		}
		else {
			printf("\nIGRA JE ZAVRSENA NERESENIM ISHODOM");
		}
	}

	if (prvi != NULL) {
		int x = brojPoslednjegPoteza(prvi->put);
		printf("\nIgrac %s je dosao do cilja u %d poteza", igrac1, x);
		for (Putanje* pom = prvi; pom != NULL; pom = pom->sled)
			ispisiPut(pom->put);
	}


	if (drugi != NULL) {
		int x = brojPoslednjegPoteza(drugi->put);
		printf("\nIgrac %s je dosao do cilja u %d poteza", igrac2, x);
		for (Putanje* pom = drugi; pom != NULL; pom = pom->sled)
			ispisiPut(pom->put);
	}
	prvi = obrisiPutanje(prvi);
	drugi = obrisiPutanje(drugi);
	tabla = obrisi(tabla);

}