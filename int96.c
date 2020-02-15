#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "int96.h"

/* proste stworzenie i zainicjowanie struktury */
int96 * i96create() {
	int96 * new96 = (int96 *)malloc(sizeof(int96));
	new96->cyfry = NULL;
	new96->dlugosc = 0;
	return new96;
}

/* proste utworzenie struktury i skopiowanie << wszystkiego >> z a */
int96 * i96dup(int96 * a) {
	int96 * new96 = (int96 *)malloc(sizeof(int96));
	new96->cyfry = (char *)malloc(a->dlugosc * sizeof(char));
	memcpy(new96->cyfry, a->cyfry, a->dlugosc * sizeof(char));
	new96->dlugosc = a->dlugosc;
	return new96;
}

/* proste zwolnienie struktury */
int96 * i96free(int96 * a) {
	if(a) {
		if(a->cyfry)
			free(a->cyfry);
		free(a);
	}
	
	a = NULL;
	return a;
}

/* proste wstawienie cyfry na koniec ciagu cyfr. Bedzie to cyfra najbardziej znaczaca */
void i96insert(int96 * a, char cyfra) {
	a->cyfry = (char *)realloc(a->cyfry, (a->dlugosc + 1) * sizeof(char));
	a->cyfry[a->dlugosc] = cyfra;
	a->dlugosc++;
}

/* proste przeksztalcenie ciagu ASCII o zadanej dlugosci na cyfry */
int96 * i96convert(char * liczba, int dlugosc) {
	int96 * new96; int i, j; char cyfra;
	new96 = i96create();

	/* sprawdzamy, ile jest cyfr */
	for(i = 0; i < dlugosc; i++) {
		cyfra = liczba[i] - '0';
		if(cyfra >= 0 && cyfra <= 9)
			new96->dlugosc++;
	}
	
	/* konwertujemy */
	new96->cyfry = (char *)malloc(new96->dlugosc * sizeof(char));
	
	i = 0; j = 0;
	while(i < new96->dlugosc && j < dlugosc) {
		cyfra = liczba[j] - '0';
		
		if(cyfra >= 0 && cyfra <= 9) {
			new96->cyfry[new96->dlugosc - i - 1] = cyfra;
			i++;
		}
		j++;	
	}
	i96reduce(new96);
	return new96;
}

/* prosta zamiana wartosci dziesietnej na ciag cyfr */
int96 * i96fromdec(char cyfra) {
	int96 * new96;
	
	new96 = i96create();
	
	if(!cyfra)
		i96insert(new96, 0);
	
	while(cyfra) {
		i96insert(new96, (cyfra % 10));
		cyfra /= 10;
	}
	return new96;
}

/* proste usuniecie najbardziej znaczacych zer */
void i96reduce(int96 * a) {
	int i;
	
	for(i = a->dlugosc - 1; i > 0; i--)
		if(!a->cyfry[i])
			a->cyfry = (char *)realloc(a->cyfry, --a->dlugosc * sizeof(char));
		else
			break;
}

/* wstawienie zera na koniec ciagu cyfr - operacja rownowazna << *= 10 >> */
void i96zero(int96 * a, int ilosc) {
	int i;
	a->cyfry = (char *)realloc(a->cyfry, (a->dlugosc + ilosc) * sizeof(char));
	a->dlugosc += ilosc;
	
	for(i = a->dlugosc - 1; i >= ilosc; i--)
		a->cyfry[i] = a->cyfry[i - ilosc];
	for(i = 0; i < ilosc; i++)
		a->cyfry[i] = 0;
}

/* funkcja porownujaca dwie liczby << wazne - nie moga miec wiadacych zer >> */
int i96compare(int96 * a, int96 * b) {
	int i;
	
	if(a->dlugosc != b->dlugosc)
		return a->dlugosc - b->dlugosc;
	
	for(i = a->dlugosc - 1; i >= 0; i--)
		if(a->cyfry[i] != b->cyfry[i])
			return a->cyfry[i] - b->cyfry[i];
	return 0;
}

/* funkcja dodajaca pisemnie dwie cyfry */
int96 * i96plus(int96 * a, int96 * b) {
	int i; char cyfra; char tmp; int96 * wynik;
	
	wynik = i96create();
	
	/* dodawaj, dopoki zostaly jakies cyfry */
	for(i = 0, tmp = 0; i < a->dlugosc || i < b->dlugosc || tmp; i++) {
		cyfra = 0;
		
		if(i < a->dlugosc)
			cyfra += a->cyfry[i];
		if(i < b->dlugosc)
			cyfra += b->cyfry[i];
		cyfra += tmp;
		
		/* oblicz cyfre, a reszte << przepisz >> dalej */
		tmp = cyfra / 10;
		cyfra %= 10;
		
		/*  wstaw obliczona cyfre na koniec wyniku */
		i96insert(wynik, cyfra);
	}
	i96reduce(wynik);
	return wynik;
}

/* funckja odejmujaca pisemnie */
int96 * i96minus(int96 * a, int96 * b) {
	int i; char cyfra; char tmp; int96 * wynik;
	
	/* jezeli b > a to nie mozna odjac */
	if(i96compare(a, b) < 0)
		return i96convert("0", 1);
	
	wynik = i96create();
	
	/* dopoki w a sa cyfry (wiemy, ze w b jest co najwyzej tyle samo, ze wzgledu na to, ze b jest niewieksze od a */
	for(i = 0, tmp = 0; i < a->dlugosc; i++) {
		cyfra = a->cyfry[i] - tmp;
		tmp = 0;
		
		/* oblicz cyfre */
		if(i < b->dlugosc) {
			while(cyfra < b->cyfry[i]) {
				cyfra += 10;
				tmp += 1;
			}
	
			cyfra -= b->cyfry[i];
		}
		
		/* wstaw cyfre */
		i96insert(wynik, cyfra);
	}
	
	i96reduce(wynik);
	return wynik;
}

/* mnozenie pisemne */
int96 * i96multiply(int96 * a, int96 * b) {
	int i, j; char cyfra; int96 * tmp1, * tmp2, * tmp3, * wynik;
	
	wynik = i96convert("0", 1);
	
	/* dopoki w b sa cyfry */
	for(i = 0; i < b->dlugosc; i++) {
		tmp1 = i96convert("0", 1);
		
		/* pomnoz a razy i-ta cyfre b */
		for(j = a->dlugosc - 1; j >= 0 ; j--) {
			cyfra = a->cyfry[j] * b->cyfry[i];
			
			tmp2 = i96fromdec(cyfra);
			i96zero(tmp2, j);
			
			tmp3 = i96plus(tmp1, tmp2);
			i96free(tmp1); tmp1 = tmp3;
			i96free(tmp2);
			
			
		}
		
		/* pomnoz wynik razy 10^i */
		i96zero(tmp1, i);
		
		/* i dodaj do wyniku ogolnego */
		tmp2 = i96plus(wynik, tmp1);
		i96free(wynik); wynik = tmp2;
		
		i96free(tmp1);
	}
	
	i96reduce(wynik);
	return wynik;
}

/* naiwne dzielenie iteracyjne */
int96 * i96dividesim(int96 * a, int96 * b) {
	int96 * wynik, * tmp1, * tmp2, * one, * zero;
	
	wynik = i96convert("0", 1);
	one = i96convert("1", 1);
	tmp1 = i96dup(a);
	
	zero = i96fromdec(0);
	if(i96compare(b, zero) > 0) 
		while(i96compare(tmp1, b) >= 0) {
			tmp2 = i96minus(tmp1, b);
			i96free(tmp1); tmp1 = tmp2;
			
			tmp2 = i96plus(wynik, one);
			i96free(wynik); wynik = tmp2;
		}
	i96free(zero);
	
	i96free(one);
	i96free(tmp1);
	
	i96reduce(wynik);
	return wynik;
}


/* naiwne modulo iteracyjne */
int96 * i96modulosim(int96 * a, int96 * b) {
	int96 * wynik, *tmp, * zero;
	wynik = i96dup(a);
	
	zero = i96fromdec(0);
	if(i96compare(b, zero) > 0) 
		while(i96compare(wynik, b) >= 0) {
			tmp = i96minus(wynik, b);
			i96free(wynik); wynik = tmp;
		}
	i96free(zero);
	
	i96reduce(wynik);
	return wynik;
}

/* dzielenie pisemne */
int96 * i96divide(int96 * a, int96 * b) {
	int i; int96 * wynik, * tmp1, * tmp2, * tmp3;
	
	tmp1 = i96convert("0", 1);
	wynik = i96convert("0", 1);
	
	for(i = a->dlugosc - 1; i >= 0; i--) {
		tmp2 = i96fromdec(a->cyfry[i]);
		i96zero(tmp1, 1);	
		
		tmp3 = i96plus(tmp1, tmp2);
		i96free(tmp1); i96free(tmp2);
		tmp1 = tmp3;
		
		tmp2 = i96dividesim(tmp1, b);
		tmp3 = i96modulosim(tmp1, b);
		
		i96free(tmp1);
		tmp1 = tmp3;
		
		i96zero(wynik, 1);
		tmp3 = i96plus(wynik, tmp2);
		i96free(wynik);
		wynik = tmp3;
	}
	
	i96reduce(wynik);
	return wynik;
}

/* modulo pisemne */
int96 * i96modulo(int96 * a, int96 * b) {
	int i; int96 * wynik, * tmp1, * tmp2;
	
	wynik = i96convert("0", 1);
	
	for(i = a->dlugosc - 1; i >= 0; i--) {
		tmp1 = i96fromdec(a->cyfry[i]);
		i96zero(wynik, 1);
		
		tmp2 = i96plus(wynik, tmp1);
		i96free(wynik); i96free(tmp1);
		wynik = tmp2;
		
		tmp2 = i96modulosim(wynik, b);
		i96free(wynik);
		wynik = tmp2;
	}
	
	i96reduce(wynik);
	return wynik;
}

/* funkcja wypisujaca liczbe na standardowe wyjscie */
void i96printf(int96 * a) {
	int i; for(i = 0; i < a->dlugosc; i++)
		printf("%c", a->cyfry[a->dlugosc - i - 1]+'0');
}
