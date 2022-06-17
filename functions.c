#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
static int brojKorisnika = 0;
void uvod() {
	printf("\t\t\tBANK ACCOUNT PROJECT\t\t\t\n");
	printf("\tFakultet elektrotehnike, racunarstva i informacijskih tehnologija\t\t\n");
	printf("\t\t\t    Osijek\t\t\t\n");
	printf("\t\t\tSmjer: RACUNARSTVO\t\t\n");
	printf("\t\t\t   JOVANA PAPRIC\t\t\t\n");
}
void KreiranjeDatoteke() {
	FILE* fp;
	if (!(fp = fopen("banka.bin", "rb"))) {
		fp = fopen("banka.bin", "wb");
		int priv = 0;
		fwrite(&priv, sizeof(int), 1, fp);
	}
	fclose(fp);
}
void podizbornik() {
	printf("Odaberite zeljenu radnju: \n");
	printf("\n\n\t1: UPLATA NOVCA");
	printf("\n\n\t2: ISPLATA NOVCA\n");
}
int izbornik() {
	printf("**********************************************************************\n");
	printf("Odaberite zeljenu opciju:");
	printf("**************************************\n");
	printf("\t\t\t1: DODAVANJE NOVOG KORISNIKA\n");
	printf("\t\t\t2: UCITAVANJE KORISNIKA\n");
	printf("\t\t\t3: ISPISIVANJE KORISNIKA\n");
	printf("\t\t\t4: PRETRAZIVANJE KORISNIKA\n");
	printf("\t\t\t5: MODIFIKCIJA RACUNA KORISNIKA\n");
	printf("\t\t\t6: SORTIRANJE KORISNIKA\n");
	printf("\t\t\t7: BRISANJE KORISNIKA\n");
	printf("\t\t\t8: BRISANJE DATOTEKE\n");
	printf("\t\t\t9: IZLAZ IZ PROGRAMA!\n");
	printf("***********************************************************************\n");

	int uvijet = 0;
	static KORISNIK* poljeKorisnika = NULL;
	static KORISNIK* pronadeniKorisnik = NULL;
	scanf("%d", &uvijet);

	switch (uvijet) {
	case 1:
		dodavanjeKorisnika("banka.bin");
		break;
	case 2:
		if (poljeKorisnika != NULL) {
			free(poljeKorisnika);
			poljeKorisnika = NULL;
		}
		poljeKorisnika = (KORISNIK*)ucitavanjeKorisnika("banka.bin");
		if (poljeKorisnika == NULL) {
			exit(EXIT_FAILURE);
		}
		break;
	case 3:
		ispisivanjeKorisnika(poljeKorisnika);
		break;
	case 4:
		pronadeniKorisnik = (KORISNIK*)pretrazivanjeKorisnika(poljeKorisnika);
		break;
	case 5:
		modificiranjeRacuna(poljeKorisnika, pronadeniKorisnik);
		break;
	case 6:
		poljeKorisnika = (KORISNIK*)sortiranjeKorisnika(poljeKorisnika);
		break;
	case 7:
		brisanjeKorisnika(&pronadeniKorisnik, poljeKorisnika);
		break;
	case 8:
		brisanjeDatoteke("banka.bin");
		break;
	case 9:
		uvijet = izlazIzPrograma(poljeKorisnika);
		break;
	default:
		uvijet = 0;
	}
	return uvijet;
}
char* toLower(char* str)
{
	for (int i = 0; str[i]; i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}
void dodavanjeKorisnika() {
	FILE* fp = fopen("banka.bin", "rb+");

	if (fp == NULL) {
		perror("Dodavanje korisnika u datoteku banka.bin");
		exit(EXIT_FAILURE);
	}

	fread(&brojKorisnika, sizeof(int), 1, fp);
	printf("Broj korisnika: %d\n", brojKorisnika);
	KORISNIK temp = { 0 };
	temp.id = brojKorisnika;
	printf("Unesite ime korisnika:\n");
	getchar();
	scanf("%19[^\n]", temp.ime);
	strcpy(temp.ime, toLower(temp.ime));

	printf("Unesite prezime korisnika:\n");
	getchar();
	scanf("%19[^\n]", temp.prezime);
	strcpy(temp.prezime, toLower(temp.prezime));
	srand((unsigned)time(NULL));
	temp.IBAN = (rand() % 9999 - 1000 + 1) + 1000;
	temp.stanje = 0;
	fseek(fp, sizeof(KORISNIK) * brojKorisnika, SEEK_CUR);
	fwrite(&temp, sizeof(KORISNIK), 1, fp);
	rewind(fp);
	brojKorisnika++;
	fwrite(&brojKorisnika, sizeof(int), 1, fp);
	fclose(fp);
}
void* ucitavanjeKorisnika() {
	FILE* fp = fopen("banka.bin", "rb");
	if (fp == NULL) {
		perror("Ucitavanje korisnika iz datoteke banka.bin");
		return NULL;
		//exit(EXIT_FAILURE);
	}

	fread(&brojKorisnika, sizeof(int), 1, fp);
	printf("Broj korisnika: %d\n", brojKorisnika);
	KORISNIK* poljeKorisnika = (KORISNIK*)calloc(brojKorisnika, sizeof(KORISNIK));
	if (poljeKorisnika == NULL) {
		perror("Zauzimanje memorije za korisnike");
		return NULL;
		//exit(EXIT_FAILURE);
	}

	fread(poljeKorisnika, sizeof(KORISNIK), brojKorisnika, fp);
	fclose(fp);
	return poljeKorisnika;
}
void ispisivanjeKorisnika(const KORISNIK* const poljeKorisnika) {
	if (poljeKorisnika == NULL) {
		printf("Polje korisnika je prazno!\n");
		return;
	}
	int i;
	for (i = 0; i < brojKorisnika; i++)
	{
		printf("\n\nKorisnik broj %d\nID: %d\nIme: %s\nPrezime: %s\nIBAN: %d\nStanje racuna: %f\n",
			i + 1,
			(poljeKorisnika + i)->id,
			(poljeKorisnika + i)->ime,
			(poljeKorisnika + i)->prezime,
			(poljeKorisnika + i)->IBAN,
			(poljeKorisnika + i)->stanje);
	}
}
void* pretrazivanjeKorisnika(KORISNIK* const poljeKorisnika) {
	if (poljeKorisnika == NULL) {
		printf("Polje korisnika je prazno!\n");
		return NULL;
	}
	int i;
	char trazenoIme[20] = { '\0' };
	printf("Unesite ime trazenog korisnika\n");
	getchar();
	scanf("%19[^\n]", trazenoIme);
	strcpy(trazenoIme, toLower(trazenoIme));
	for (i = 0; i < brojKorisnika; i++)
	{
		if (!strcmp(trazenoIme, (poljeKorisnika + i)->ime)) {
			printf("Korisnik je pronaden!\n");
			return (poljeKorisnika + i);
		}
	}
	return NULL;
}
void upisNaRacun(KORISNIK* const trazeniKorisnik) {
	FILE* fp = fopen("banka.bin", "rb+");
	if (fp == NULL) {
		perror("Modifikacija racuna nije moguca!\n");
		exit(EXIT_FAILURE);
	}
	fseek(fp,sizeof(int),SEEK_SET);
	fseek(fp, sizeof(KORISNIK) * trazeniKorisnik->id, SEEK_CUR);
	fwrite(trazeniKorisnik, sizeof(KORISNIK), 1, fp);
	rewind(fp);
	fwrite(&brojKorisnika, sizeof(int), 1, fp);
	fclose(fp);
}
void modificiranjeRacuna(const KORISNIK* const poljeKorisnika, KORISNIK* const trazeniKorisnik) {
	float uplata;
	float isplata;
	int priv;
	int i;
	for (i = 0; i < brojKorisnika; i++)
	{
		if ((poljeKorisnika + i) == trazeniKorisnik) {
			printf("\n\tTRAZENI RACUN JE PRONADJEN!\n\t");
			printf("\t%d\t%s\t%s\t\t%d\t\t%.2f\n\n", trazeniKorisnik->id, trazeniKorisnik->ime,
				trazeniKorisnik->prezime, trazeniKorisnik->IBAN, trazeniKorisnik->stanje);
		}
		else if (trazeniKorisnik == NULL) {
			printf("\n\tTRAZENI KORISNIK NIJE PRONADJEN!\n\t");
			return;
		}
	}
	podizbornik();
	scanf("%d", &priv);


	switch (priv) {
	case 1:
		printf("\n\tUNESITE KOLICINU NOVCA KOJU ZELITE UPLATITI NA RACUN\n\t");
		scanf("%f", &uplata);

		if ((uplata < 0)) {
			printf("\n\tUNOS UPLATE NE MOZE BITI MANJI OD 0:\n\t");
			getchar();
			scanf("%f", &uplata);

		} 
		trazeniKorisnik->stanje += uplata;
		upisNaRacun(trazeniKorisnik);
		
		break;

	case 2:
		printf("\n\tUNESITE KOLICINU NOVCA KOJU ZELITE ISPLATITI S RACUNA\n\t");

		scanf("%f", &isplata);

		if (trazeniKorisnik->stanje > 0 && isplata <= trazeniKorisnik->stanje ) {
			trazeniKorisnik->stanje -= isplata;
			upisNaRacun(trazeniKorisnik);
		}
		else
			printf("\n\tPOGRESAN UNOS ILI NEDOVOLJNO STANJE NA RACUNU!\n\t");
		break;

	}

	return;
}
void* sortiranjeKorisnika(KORISNIK* const poljeKorisnika) {
	int i, j;
	KORISNIK temp;
	for (int i = 0; i <= brojKorisnika - 1; i++) {
		for (int j = i + 1; j <= brojKorisnika - 1; j++) {
			int comp = strcmp(poljeKorisnika[i].ime, poljeKorisnika[j].ime);
			if (comp > 0) {
				temp = *(poljeKorisnika + i);
				*(poljeKorisnika + i) = *(poljeKorisnika + j);
				*(poljeKorisnika + j) = temp;
			}
		}
	}

	return poljeKorisnika;
}
void brisanjeKorisnika(KORISNIK** const trazeniKorisnik, const KORISNIK* const poljeKorisnika) {
	FILE* fp = fopen("banka.bin", "wb");
	if (fp == NULL) {
		perror("Brisanje korisnika iz datoteke banka.bin");
		exit(EXIT_FAILURE);
	}

	fseek(fp, sizeof(int), SEEK_SET);
	int i;
	int noviBrojacKorisnika = 0;
	for (i = 0; i < brojKorisnika; i++)
	{
		if (*trazeniKorisnik != (poljeKorisnika + i)) {
			fwrite((poljeKorisnika + i), sizeof(KORISNIK), 1, fp);
			noviBrojacKorisnika++;
		}
	}
	rewind(fp);
	fwrite(&noviBrojacKorisnika, sizeof(int), 1, fp);
	fclose(fp);
	printf("Korisnik je uspjesno obrisan!\n");
	*trazeniKorisnik = NULL;
	ucitavanjeKorisnika();
}
void brisanjeDatoteke() {
	printf("Zelite li uistinu obrisati datoteku banka.bin ? \n");
	printf("Utipkajte \"da\" ako uistinu zelite obrisati datoteku u suprotno utipkajte\"ne\"!\n");
	char potvrda[3] = { '\0' };
	scanf("%2s", potvrda);
	if (!strcmp("da", potvrda)) {
		remove("banka.bin") ? printf("Neuspjesno brisanje datoteke banka.bin!\n") : printf("Uspjesno obrisana datoteka banka.bin!\n");
	}
}
int izlazIzPrograma(KORISNIK* poljeKorisnika) {
	free(poljeKorisnika);
	return 0;
}