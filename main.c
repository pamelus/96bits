#include <stdio.h>
#include <string.h>
#include "int96.h"

/* Najwieksza liczba 96 bitowa 79228162514264337593543950335. Ma ona 30 cyfr, wiec bufor wejsciowy bedzie mial tylko 32 bajty */

int main() {
	char in[256]; int96 * wynik, * a, * b; int wybor;

	do {
		printf("[1] Dodawanie\n[2] Odejmowanie\n[3] Mnozenie\n[4] Dzielenie\n[5] Dzielenie z modulo\n\n[0] Koniec\n");
		scanf("%d", &wybor);
		if(wybor) {
			printf("a: "); scanf("%s", in);
			a = i96convert(in, strlen(in));
			
			printf("b: "); scanf("%s", in);
			b = i96convert(in, strlen(in));
			
			printf("\n");
			switch(wybor) {
				case 1: wynik = i96plus(a, b);
						printf("a + b = "); i96printf(wynik); printf("\n");
						i96free(wynik);
						break;
				case 2: wynik = i96minus(a, b);
						printf("a - b = "); i96printf(wynik); printf("\n");
						i96free(wynik);
						break;
				case 3: wynik = i96multiply(a, b);
						printf("a * b = "); i96printf(wynik); printf("\n");
						i96free(wynik);
						break;
				case 4: wynik = i96divide(a, b);
						printf("a / b = "); i96printf(wynik); printf("\n");
						i96free(wynik);
						break;
				case 5: wynik = i96divide(a, b);
						printf("a / b = "); i96printf(wynik); printf("\n");
						i96free(wynik);
						
						wynik = i96modulo(a, b);
						printf("a %% b = "); i96printf(wynik); printf("\n");
						i96free(wynik);
						break;
				}
			i96free(a);
			i96free(b);
		
			printf("\n");
		}
	} while(wybor);
	
	return 0;
}

