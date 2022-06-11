#define _CRT_SECURE_NO_WARNINGS_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "functions.h"
int main(void) {
	uvod();
	KreiranjeDatoteke();
	int uvijet = 1;
	while (uvijet) {
		uvijet = izbornik();
	}
	printf("Zavrsetak programa!\n");
	return 0;
}