#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <cstring>

using namespace std;

const int MAX_PKT = 20;				// poczatkowa liczba punktow graczy
const int LK = 5;					// liczba kosci w grze


void czekaj_na_enter()
{
	system("pause");
}


//wstawianie zer do tablicy kosci
void zeruj_kosci(int k[])
{
	for (int i = 0; i < LK; i++)
	{
		k[i] = 0;
	}
}


// losowanie kosci
void rzut_koscmi(int k[])
{
	for (int i = 0; i < LK; i++)
	{
		if (k[i] == 0)
		{
			k[i] = rand() % 6 + 1;
		}
	}
}


// zwraca imie gracza
string podaj_imie_gracza()
{
	string imie;
	cout << "Podaj imie: __________\b\b\b\b\b\b\b\b\b\b";
	cin >> imie;
	return imie.substr(0, 10);		// zwroci pierwsze 10 znakow 
}


void wyswietl_imie_10_znakow(string imie)
{
	int ile_spacji;
	cout << imie;
	ile_spacji = 10 - imie.length();
	for (int i = 0; i < ile_spacji; i++)
	{
		cout << " ";
	}
}

// naglowek do wyswietlenia
void ekran_naglowek(string gracz_A, string gracz_B, int punkty_A, int punkty_B, int pula)
{
	cout << " ____________   ____________   ____________" << endl;
	cout << "| ";
	wyswietl_imie_10_znakow(gracz_A);
	cout << " | | ";
	wyswietl_imie_10_znakow(gracz_B);
	cout << " | |            |" << endl;
	cout << "|____________| |____________| |    pula    |" << endl;
	cout << "| punkty: ";
	cout << setw(2) << punkty_A;
	cout << " | | punkty: ";
	cout << setw(2) << punkty_B;
	cout << " | |     ";
	cout << setw(2) << pula;
	cout << "     |" << endl;
	cout << "|____________| |____________| |____________|" << endl << endl;
}


void wyswietl_kosci(int k[])
{
	cout << "   1      2      3      4      5" << endl;
	cout << "  ---    ---    ---    ---    --- " << endl;
	for (int i = 0; i < LK; i++)
	{
		if (k[i] == 0)
			cout << " | " << " " << " | ";
		else
			cout << " | " << k[i] << " | ";
	}
	cout << endl;
	cout << "  ---    ---    ---    ---    --- " << endl;
	cout << endl;
}


void ekran_linia()
{
	cout << "____________________________________________" << endl;
}


int minimum(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}


int maximum(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}


void zamiana(int& a, int& b)
{
	int temp;
	temp = a;
	a = b;
	b = temp;
}


// sortowanie babelkowe
void posortuj_kosci(int kosci[], int kosci_sort[])
{
	// kopiujemy do nowej tablicy, bez zmiany tablicy oryginalnej
	for (int i = 0; i < LK; i++)
	{
		kosci_sort[i] = kosci[i];
	}

	int liczba_zamian = 0;
	do
	{
		liczba_zamian = 0;
		for (int i = 0; i < LK - 1; i++)		// wykonujemy do przedostatniego elementu
		{
			if (kosci_sort[i] > kosci_sort[i + 1])
			{
				zamiana(kosci_sort[i], kosci_sort[i + 1]);
				liczba_zamian++;
			}
		}
	} while (liczba_zamian != 0);
}


// sprawdza, czy mamy pokera
// true = mamy poker, false = nie mamy pokera
// zwraca 'wartosc' = liczbe przypisana do danego pokera
bool to_jest_poker(int k[], int& wartosc)
{
	if (k[0] == k[1] && k[1] == k[2] && k[2] == k[3] && k[3] == k[4])
	{
		wartosc = 9000 + k[0];
		return true;
	}
	else
	{
		wartosc = 0;
		return false;
	}
}


// sprawdza, czy mamy karete
// true = mamy karete, false = nie mamy karete
// wartosc = liczba przypisana do danej karety
bool to_jest_kareta(int kosci[], int& wartosc)
{
	int ks[LK];

	posortuj_kosci(kosci, ks);

	if (ks[0] == ks[1] && ks[1] == ks[2] && ks[2] == ks[3] && ks[3] != ks[4])  // kareta typu XXXXY
	{
		wartosc = 8000 + ks[0] * 100 + ks[4];			// cyfra setek oznacza powtarzajace sie liczby oczek
		return true;								// cyfra jednosci to inna liczba wyrzuconych oczek 
	}
	else
		if (ks[0] != ks[1] && ks[1] == ks[2] && ks[2] == ks[3] && ks[3] == ks[4])	// kareta typu YXXXX
		{
			wartosc = 8000 + ks[1] * 100 + ks[0];
			return true;
		}
		else
		{
			wartosc = 0;
			return false;
		}
}


// sprawdza, czy mamy fulla
// true = mamy fulla, false = nie mamy fulla
// wartosc = liczba przypisana do danego fulla
bool to_jest_full(int kosci[], int& wartosc)
{
	int ks[LK];		// przechowuje posortowane kosci

	posortuj_kosci(kosci, ks);		// sortuje kosci i zwraca w tablicy ks[]

	if (ks[0] == ks[1] && ks[1] == ks[2] && ks[2] != ks[3] && ks[3] == ks[4])  // full typu XXXYY, np. 11122
	{
		wartosc = 7000 + ks[0] * 100 + ks[3] + ks[4]; 	// Zakodowana wartosc liczbowa fulla. Cyfra setek to cyfra 'trojki'.
		return true;									// 'Dwojki' dodajemy, zeby odroznic np. 22255 od 22211.
	}
	else
		if (ks[0] == ks[1] && ks[1] != ks[2] && ks[2] == ks[3] && ks[3] == ks[4])	// full typu XXYYY, np. 11222		
		{
			wartosc = 7000 + ks[2] * 100 + ks[0] + ks[1];
			return true;
		}
		else
		{
			wartosc = 0;
			return false;
		}
}


// sprawdza, czy mamy duzy strit
// true = mamy duzy strit, false = nie mamy duzego strita
// wartosc = liczba przypisana do duzego strita
bool to_jest_duzy_strit(int kosci[], int& wartosc)
{
	int ks[LK];

	posortuj_kosci(kosci, ks);

	if (ks[0] == 2 && ks[1] == 3 && ks[2] == 4 && ks[3] == 5 && ks[4] == 6)	// uklad kosci: 23456
	{
		wartosc = 6000;
		return true;
	}
	else
	{
		wartosc = 0;
		return false;
	}
}


// sprawdza, czy mamy maly strit
// true = mamy maly strit, false = nie mamy malego strita
// wartosc = liczba przypisana do malego strita
bool to_jest_maly_strit(int kosci[], int& wartosc)
{
	int ks[LK];

	posortuj_kosci(kosci, ks);

	if (ks[0] == 1 && ks[1] == 2 && ks[2] == 3 && ks[3] == 4 && ks[4] == 5)
	{
		wartosc = 5000;
		return true;
	}
	else
	{
		wartosc = 0;
		return false;
	}
}


// sprawdza, czy mamy trojke
// true = mamy trojke, false = nie mamy trojki
// wartosc = liczba przypisana do trojki
bool to_jest_trojka(int kosci[], int& wartosc)
{
	int ks[LK];

	posortuj_kosci(kosci, ks);

	if (ks[0] == ks[1] && ks[1] == ks[2] && ks[2] != ks[3] && ks[3] != ks[4])	// trojka typu: XXXYZ
	{
		wartosc = 4000 + ks[0] * 100 + ks[3] + ks[4];
		return true;
	}
	else
		if (ks[0] != ks[1] && ks[1] == ks[2] && ks[2] == ks[3] && ks[3] != ks[4])	// trojka typu: YXXXZ
		{
			wartosc = 4000 + ks[1] * 100 + ks[0] + ks[4];
			return true;
		}
		else
			if (ks[0] != ks[1] && ks[1] != ks[2] && ks[2] == ks[3] && ks[3] == ks[4])	// trojka typu: YZXXX
			{
				wartosc = 4000 + ks[2] * 100 + ks[0] + ks[1];
				return true;
			}
			else
			{
				wartosc = 0;
				return false;
			}
}


// sprawdza, czy mamy dwie pary
// true = mamy dwie pary, false = nie mamy dwoch par
// wartosc = liczba przypisana do dwoch par
bool to_jest_dwie_pary(int kosci[], int& wartosc)
{
	int ks[LK];

	posortuj_kosci(kosci, ks);

	if (ks[0] == ks[1] && ks[1] != ks[2] && ks[2] == ks[3] && ks[3] != ks[4])	// pary typu: XXYYZ
	{
		wartosc = 3000 + (ks[0] * 2 + ks[1] * 2) * 10 + ks[4];
		return true;
	}
	else
		if (ks[0] != ks[1] && ks[1] == ks[2] && ks[2] != ks[3] && ks[3] == ks[4])	// pary typu: XYYZZ
		{
			wartosc = 3000 + (ks[1] * 2 + ks[3] * 2) * 10 + ks[0];
			return true;
		}
		else
			if (ks[0] == ks[1] && ks[1] != ks[2] && ks[2] != ks[3] && ks[3] == ks[4])	// pary typu: XXYZZ
			{
				wartosc = 3000 + (ks[0] * 2 + ks[3] * 2) * 10 + ks[2];
				return true;
			}
			else
			{
				wartosc = 0;
				return false;
			}
}


// sprawdza, czy mamy pare
// true = mamy pare, false = nie mamy pary
// wartosc = liczba przypisana do pary
bool to_jest_para(int kosci[], int& wartosc)
{
	int ks[LK];

	posortuj_kosci(kosci, ks);

	if (ks[0] == ks[1] && ks[1] != ks[2] && ks[2] != ks[3] && ks[3] != ks[4])	// para typu: XXABC
	{
		wartosc = 2000 + ks[0] * 100 + ks[2] + ks[3] + ks[4];
		return true;
	}
	else
		if (ks[0] != ks[1] && ks[1] == ks[2] && ks[2] != ks[3] && ks[3] != ks[4])	// para typu: AXXBC
		{
			wartosc = 2000 + ks[1] * 100 + ks[0] + ks[3] + ks[4];
			return true;
		}
		else
			if (ks[0] != ks[1] && ks[1] != ks[2] && ks[2] == ks[3] && ks[3] != ks[4])	// para typu: ABXXC
			{
				wartosc = 2000 + ks[2] * 100 + ks[0] + ks[1] + ks[4];
				return true;
			}
			else
				if (ks[0] != ks[1] && ks[1] != ks[2] && ks[2] != ks[3] && ks[3] == ks[4])	// para typu: ABCXX
				{
					wartosc = 2000 + ks[3] * 100 + ks[0] + ks[1] + ks[2];
					return true;
				}
				else
				{
					wartosc = 0;
					return false;
				}
}


// sprawdza, czy mamy Nic
// true = mamy Nic, false = nie mamy Nic
// wartosc = liczba przypisana do Nic
bool to_jest_nic(int kosci[], int& wartosc)
{
	int ks[LK];

	posortuj_kosci(kosci, ks);

	if ((ks[0] == 1 && ks[1] == 2 && ks[2] == 3 && ks[3] == 4 && ks[4] == 6)		// 1 2 3 4 6
		|| (ks[0] == 1 && ks[1] == 2 && ks[2] == 3 && ks[3] == 5 && ks[4] == 6)		// 1 2 3 5 6
		|| (ks[0] == 1 && ks[1] == 2 && ks[2] == 4 && ks[3] == 5 && ks[4] == 6)		// 1 2 4 5 6
		|| (ks[0] == 1 && ks[1] == 3 && ks[2] == 4 && ks[3] == 5 && ks[4] == 6))	// 1 3 4 5 6
	{
		wartosc = 1000 + ks[0] + ks[1] + ks[2] + ks[3] + ks[4];
		return true;
	}
	else
	{
		wartosc = 0;
		return false;
	}
}


// Funkcja zwraca wartosc (liczbe) reprezentujaca dany uklad kosci.
// 9xxx - poker, poker na szostkach to 9006 (9000 + 6), poker na jedynkach to 9001 (9000 + 1)
// 8xxx - kareta, np. kareta 44445 bedzie zakodowana jako liczba 8405 (8000 + 4*100 + 5)
// 7xxx - full, np. full 11122 bedzie zakodowany jako liczba 7104 (7000 + 1*100 + 2 + 2)
// 6000 - duzy strit, wartosc 6000
// 5000 - maly strit, wartosc 5000
// 4xxx - trojka, np. 33356 bedzie zakodowane jako liczba 4311 (4000 + 3*300 + 5 + 6)
// 3xxx - dwie pary, np. 11336 bedzie zakodowane jako liczba 3086  (3000 + (2*1 + 2*3)*10 + 6)
// 2xxx - para, np. 11456 bedzie zakodowane jako liczba 2115  (2000 + 1*100 + 4 + 5 + 6)
// 1xxx - nic, np. 13456 bedzie zakodowane jako liczba 1019 (1000 + 1+3+4+5+6)
int wartosc_kosci(int kosci[])
{
	int wartosc;
	if (to_jest_poker(kosci, wartosc));
	else if (to_jest_kareta(kosci, wartosc));
	else if (to_jest_full(kosci, wartosc));
	else if (to_jest_duzy_strit(kosci, wartosc));
	else if (to_jest_maly_strit(kosci, wartosc));
	else if (to_jest_trojka(kosci, wartosc));
	else if (to_jest_dwie_pary(kosci, wartosc));
	else if (to_jest_para(kosci, wartosc));
	else (to_jest_nic(kosci, wartosc));

	return wartosc;
}


// zwraca string z nazwa ukladu kosci
string podaj_nazwe_ukladu(int kosci[])
{
	int wynik;

	wynik = wartosc_kosci(kosci);		// zwracamy wartosc ukladu wyrzuconych kosci
	wynik = wynik / 1000;
	switch (wynik)
	{
	case 9:
		return "Poker";
	case 8:
		return "Kareta";
	case 7:
		return "Full";
	case 6:
		return "Duzy Strit";
	case 5:
		return "Maly Strit";
	case 4:
		return "Trojka";
	case 3:
		return "Dwie Pary";
	case 2:
		return "Para";
	default:
		return "Nic";
	}
}


// wyswietlamy kosci tylko jesli wszystkie sa niezerowe
bool czy_wyswietlac_uklad(int kosci[])
{
	for (int i = 0; i < LK; i++)
	{
		if (kosci[i] == 0)
		{
			return false;
		}
	}
	return true;
}


// wyswietlanie ekranu z koscmi
void ekran_kosci(string gracz_A, string gracz_B, int k_A[], int k_B[])
{
	cout << gracz_A;
	cout << endl;
	if (czy_wyswietlac_uklad(k_A) == true)
		cout << setw(43) << podaj_nazwe_ukladu(k_A);
	cout << endl;
	wyswietl_kosci(k_A);

	cout << gracz_B;
	cout << endl;
	if (czy_wyswietlac_uklad(k_B) == true)
		cout << setw(43) << podaj_nazwe_ukladu(k_B);
	cout << endl;
	wyswietl_kosci(k_B);
}


// wyswietlanie calego ekranu gry
void ekran_gry(string gracz_A, string gracz_B, int punkty_A, int punkty_B, int pula, int kosci_A[], int kosci_B[])
{
	system("cls");

	ekran_naglowek(gracz_A, gracz_B, punkty_A, punkty_B, pula);
	ekran_kosci(gracz_A, gracz_B, kosci_A, kosci_B);
	ekran_linia();
}


//wymiana kosci i rzut koscmi, ktore gracz chce wymienic
void wymiana_kosci(string gracz_A, string gracz_B, int& punkty_A, int& punkty_B, int& pula, int kosci_A[], int kosci_B[])
{
	int n_A, n_B; 	// ilosc kosci do wymiany
	int numer_kosci;  // numer kosci do wymiany

	// gracz A:
	do {
		ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
		cout << gracz_A << ", podaj ilosc kosci do wymiany [0, " << LK << "]: ";
		cin.clear();
		cin.sync();
	} while (!(cin >> n_A) || n_A < 0 || n_A > LK);
	ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
	for (int i = 0; i < n_A; i++)
	{
		do {
			ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
			cout << gracz_A << ", podaj numer kosci [1, " << LK << "]: ";
			cin.clear();
			cin.sync();
		} while (!(cin >> numer_kosci) || numer_kosci <= 0 || numer_kosci > LK);
		kosci_A[numer_kosci - 1] = 0;
		ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
	}

	// gracz B:
	do {
		ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
		cout << gracz_B << ", podaj ilosc kosci do wymiany [0, " << LK << "]: ";
		cin.clear();
		cin.sync();
	} while (!(cin >> n_B) || n_B < 0 || n_B > LK);
	ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
	for (int i = 0; i < n_B; i++)
	{
		do {
			ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
			cout << gracz_B << ", podaj numer kosci [1, " << LK << "]: ";
			cin.clear();
			cin.sync();
		} while (!(cin >> numer_kosci) || numer_kosci <= 0 || numer_kosci > LK);
		kosci_B[numer_kosci - 1] = 0;
		ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
	}
	if (n_A > 0 || n_B > 0)
	{
		cout << "Rozpoczynamy rzut koscmi... ";
		czekaj_na_enter();
		rzut_koscmi(kosci_A);
		rzut_koscmi(kosci_B);
	}
}


// obsluguje scenariusz licytacji
void licytacja(string gracz_A, string gracz_B, int& punkty_A, int& punkty_B, int& pula, int kosci_A[], int kosci_B[])
{
	int suma_obstawien_A;	// ilosc postawionych punktow w calej licytacji przez gracza A
	int suma_obstawien_B;	// ...przez gracza B
	int obstawione_przez_A; // pojedyncze obstawienie gracza A
	int obstawione_przez_B;	// ...gracza B
	int min, max;			// dozwolone minimalne i maksymalne obstawienie

	suma_obstawien_A = 0;
	suma_obstawien_B = 0;
	obstawione_przez_A = 0;
	obstawione_przez_B = 0;

	do
	{
		min = maximum(suma_obstawien_B - suma_obstawien_A, 1);
		max = minimum(punkty_B + suma_obstawien_B - suma_obstawien_A, punkty_A);

		do {
			ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
			cout << gracz_A << ", ile punktow obstawiasz? [" << min << ", " << max << "]: ";
			cin.clear();
			cin.sync();
		} while (!(cin >> obstawione_przez_A) || obstawione_przez_A < min || obstawione_przez_A > max);

		suma_obstawien_A = suma_obstawien_A + obstawione_przez_A;
		punkty_A = punkty_A - obstawione_przez_A;
		pula = pula + obstawione_przez_A;

		ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);

		if (suma_obstawien_A != suma_obstawien_B)
		{
			min = maximum(suma_obstawien_A - suma_obstawien_B, 1);
			max = minimum(punkty_A + suma_obstawien_A - suma_obstawien_B, punkty_B);

			do {
				ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
				cout << gracz_B << ", ile punktow obstawiasz? [" << min << ", " << max << "]: ";
				cin.clear();
				cin.sync();
			} while (!(cin >> obstawione_przez_B) || obstawione_przez_B < min || obstawione_przez_B > max);

			suma_obstawien_B = suma_obstawien_B + obstawione_przez_B;
			punkty_B = punkty_B - obstawione_przez_B;
			pula = pula + obstawione_przez_B;

			ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
		}

	} while (suma_obstawien_A != suma_obstawien_B);
}


int main()
{
	string gracz_A;						// imie gracza A
	string gracz_B;						// imie gracza B
	int kosci_A[LK];					// kosci gracza A
	int kosci_B[LK];					// kosci gracza B
	int punkty_A = MAX_PKT;				// liczba punktow gracza A
	int punkty_B = MAX_PKT;				// liczba punktow gracza B
	int pula = 0;						// aktualna wartosc puli punktow 
	int wynik_gracza_A, wynik_gracza_B;	// wartosc ukladu kosci 

	srand((unsigned int)time(NULL));		// inicjalizacja generatora liczb losowych

	cout << "Gracz 1. ";
	gracz_A = podaj_imie_gracza();
	cout << "Gracz 2. ";
	gracz_B = podaj_imie_gracza();

	while (punkty_A > 0 && punkty_B > 0)	// gra toczy sie dopoki gracze maja punkty do obstawiania
	{
		zeruj_kosci(kosci_A);
		zeruj_kosci(kosci_B);

		ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);

		licytacja(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);	// obsluga pierwszej licytacji

		cout << "Rozpoczynamy rzut koscmi... ";
		czekaj_na_enter();

		rzut_koscmi(kosci_A);		// losowanie kosci graczy
		rzut_koscmi(kosci_B);

		ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);		// pokazujemy ekran z wylosowanymi koscmi

		if (punkty_A > 0 && punkty_B > 0)		// licytujemy tylko wtedy, jesli gracze maja punkty do obstawienia 
		{									// po poprzedniej licytacji moglo sie cos zmienic
			licytacja(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);	// obsluga drugiej licytacji
		}

		wymiana_kosci(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);	// scenariusz wymiany kosci

		wynik_gracza_A = wartosc_kosci(kosci_A);		// okreslenie wartosci ukladu kosci
		wynik_gracza_B = wartosc_kosci(kosci_B);		// w zmiennych mamy liczby

		if (wynik_gracza_A > wynik_gracza_B)			// kto wygral jedna runde
		{
			punkty_A = punkty_A + pula;
			pula = 0;
			ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
			cout << "Wygrywa " << gracz_A << " ! ";
		}
		else if (wynik_gracza_A < wynik_gracza_B)
		{
			punkty_B = punkty_B + pula;
			pula = 0;
			ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
			cout << "Wygrywa " << gracz_B << " ! ";
		}
		else
		{
			punkty_A = punkty_A + pula / 2;
			punkty_B = punkty_B + pula / 2;
			pula = 0;
			ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);
			cout << "Remis! ";
		}
		czekaj_na_enter();
	}						// koniec petli while = koniec rund, jeden gracz wygrywa cala gre

	ekran_gry(gracz_A, gracz_B, punkty_A, punkty_B, pula, kosci_A, kosci_B);		// pokazujemy ekran na koniec

	if (punkty_A > punkty_B)	// kto wygrywa cala gre
	{
		cout << ">>> Rozgrywke wygrywa " << gracz_A << " ! <<< ";
	}
	else
	{
		cout << ">>> Rozgrywke wygrywa " << gracz_B << " ! <<< ";
	}

	czekaj_na_enter();
	return 0;
}
