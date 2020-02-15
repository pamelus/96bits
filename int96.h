
/* struktura sluzaca do przechowywania liczb 96bitowych (i wiekszych) */

typedef struct {
	char * cyfry;
	unsigned int dlugosc;
} int96;

/* funckja tworzaca nowa, pusta liczbe */
int96 * i96create();

/* funkcja tworzaca kopie liczby */
int96 * i96dup(int96 *);

/* funckja zwalniajaca pamiec po liczbie, zwaraca NULL */
int96 * i96free(int96 *);

/* funkcja wstawiajaca cyfre << najbardziej znaczaca >> do liczby */
void i96insert(int96 *, char);

/* funkcja konwertujaca ciag znakow o danej dlugosci na liczbe */
int96 * i96convert(char *, int);

/* funkcja konwertujaca liczbe dziesietna (max 8 bitowa) na liczbe */
int96 * i96fromdec(char cyfra);

/* funckja usuwajaca << prowadzace >> zera */
void i96reduce(int96 *);

/* funckja wstawiajaca zero na koniec liczby (dziala jak mnozenie przez 10) */
void i96zero(int96 *, int);

/* funckja porownujaca dwie liczby */
int i96compare(int96 *, int96 *);

/* funkcja dodajaca dwie liczby, zwraca wynik jako nowa liczbe */
int96 * i96plus(int96 *, int96 *);

/* funckja odejmujaca dwie liczby, zwraca wynik jako nowa liczbe */
int96 * i96minus(int96 *, int96 *);

/* funkcja mnozaca dwie liczby, zwraca wynik jako nowa liczbe */
int96 * i96multiply(int96 *, int96 *);

/* funkcja dzielaca dwie liczby w sposob << naiwny >>, skuteczna jedynie dla malych liczb */
int96 * i96dividesim(int96 *, int96 *);

/* funckja obliczajaca podulo w sposob << naiwny >>, skuteczna tylko dla malych liczb */
int96 * i96modulosim(int96 *, int96 *);

/* funkcja dzielaca - dzielenie pisemne z wykorzystaniem dzielenia naiwnego i modulo naiwnego */
int96 * i96divide(int96 *, int96 *);

/* funkcja modulo - dzielenie pisemnie z wykorzystaniem modulo naiwnego */
int96 * i96modulo(int96 *, int96 *);

/* funkcja wypisujaca liczbe na standardowe wyjscie */
void i96printf(int96 *);
