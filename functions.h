#ifndef FUNKCIJE_H
#define FUNKCIJE_H
typedef struct korisnik {
	int id;
	char ime[20];
	char prezime[20];
	int IBAN;
	float stanje;
}KORISNIK;

void uvod();
void KreiranjeDatoteke();
void podizbornik();
int izbornik();
void dodavanjeKorisnika();
void* ucitavanjeKorisnika();
void ispisivanjeKorisnika(const KORISNIK* const);
void* pretrazivanjeKorisnika(KORISNIK* const);
void modificiranjeRacuna(const KORISNIK* const, KORISNIK* const);
void* sortiranjeKorisnika(KORISNIK* const);
void brisanjeKorisnika(KORISNIK** const, const KORISNIK* const);
void brisanjeDatoteke();
int izlazIzPrograma(KORISNIK*);

#endif // FUNCKIJE_H