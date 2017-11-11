#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <math.h>
#include <stdlib.h>

using namespace std;
typedef long double typ2;
typedef long long int int2;
typ2 iloczyn_skalarny(typ2 *v1, typ2 *v2, int size);
int convert(int n, vector<int> &cont);
void Array_Swap(typ2 *a, typ2 *b, int size);
typ2 pot(typ2 a, int n);
void Wyznacznik_Chio(typ2 **matrix, int size, vector<typ2> &div, typ2 *det);
typ2 **Macierz_Transponowana(typ2 **matrix, int n, int m);
typ2 **Iloczyn_Macierzy(typ2 **matrix1, typ2 **matrix2, int Matrix1_Columns, int Matrix2_Columns, int Matrix1_Rows, int Matrix2_Rows);
double Wspolczynnik_Hadamara(typ2 **Baza, int n, int m);
typ2 **Psucie_Unimod(typ2 **unimod, int n);
typ2 **Macierz_Dopelnien(typ2 **matrix, int n);
typ2 **Macierz_Odwrotna(typ2 **matrix, int n);
typ2 **Macierz_Mnozenie_Skalar(typ2 **matrix, typ2 value, int n, int m);
typ2 **Alfa(typ2 **Baza, typ2 **punkt, int n, int m);
int main()
{
	srand(time(NULL));
	int n, m, d;
	double WSPH;
	typ2 det = 0;
	vector <typ2> div;
	cout << "Podaj ilosc wektorow oraz rozmiar przestrzeni" << endl<< "Dla prokolu GGH musi to byc krata pelnego rzedu." << endl;
	cin >> n;
	m = n;
	typ2 **Klucz_Prywatny = new typ2 *[n];
	typ2 **Klucz_Publiczny;
	typ2 **Klucz_Publiczny_Odwrotny;
	typ2 **Wiadomosc_Zaszyfrowana=new typ2 *[1];
	typ2 **unimod = new typ2*[n];
	typ2 **Wiadomosc_Odczytana;
	for (int i = 0; i < n; i++) {
		Klucz_Prywatny[i] = new typ2[m];
		unimod[i] = new typ2[m];
	}
	Wiadomosc_Zaszyfrowana[0] = new typ2[m];
	cout << endl;
	cout << "--------------------------------------------" << endl;
	cout << "Alicja" << endl;
	cout << "Podaj 'd' z zakresu ktorego bedzie tworzony klucz prywatny" << endl;
	cin >> d;
	int temp;
	temp = 2 * d;
	do {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				Klucz_Prywatny[i][j] = d;
				Klucz_Prywatny[i][j] -= rand() % (temp + 1);
				if (i == j) unimod[i][j] = 1;
				else unimod[i][j] = 0;
			}
		}
		WSPH = Wspolczynnik_Hadamara(Klucz_Prywatny, n, m);
	} while (WSPH < 0.8);
	cout << "Klucz prywatny wygenerowany dla Alicji"<< endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << Klucz_Prywatny[i][j] << " ";
		}
		cout << endl;
	}
	unimod = Psucie_Unimod(unimod, n);
	Klucz_Publiczny = Iloczyn_Macierzy(unimod, Klucz_Prywatny, n, n, n, n);
	cout << "Wygenerowany klucz publiczny"<< endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << Klucz_Publiczny[i][j] << " ";
		}
		cout << endl;
	}

	cout << "\n\n\n\n";
	cout << "--------------------------------------------" << endl;
	cout << "Bob" << endl;
	cout << "Podaj liczby do zaszyfrowania" << endl;
	
	for (int i = 0; i < m; i++) {		
		cin >> Wiadomosc_Zaszyfrowana[0][i];
	}
	cout << endl << "Wiadomosc do zaszyfrowania:"<<endl;
	for (int i = 0; i < m; i++) {
		cout << Wiadomosc_Zaszyfrowana[0][i] << " ";

	}
	Wiadomosc_Zaszyfrowana = Iloczyn_Macierzy(Wiadomosc_Zaszyfrowana, Klucz_Publiczny,m,m,1,n);
	d = 1;
	temp = 2 * d;
	for (int i = 0; i < m; i++) {	
		Wiadomosc_Zaszyfrowana[0][i] += d;
		Wiadomosc_Zaszyfrowana[0][i] -= rand() % (temp + 1);
	}
	cout << "\n";
	cout << "--------------------------------------------" << endl;
	cout << "Alicja" << endl;
	cout << "Otrzymalem szyfrogram: " << endl;

	for (int i = 0; i < m; i++) {
		cout << (int2)Wiadomosc_Zaszyfrowana[0][i] << " ";
	}
	cout << "\n";
	cout << "Wspolczynniki kombinacji liniowej, wyznaczone algorytymem Babaja:" << endl;
	Wiadomosc_Odczytana = Alfa(Klucz_Prywatny, Wiadomosc_Zaszyfrowana, n, m);
	Klucz_Publiczny_Odwrotny = Macierz_Odwrotna(unimod, n);
	Wiadomosc_Odczytana = Iloczyn_Macierzy(Wiadomosc_Odczytana, Klucz_Publiczny_Odwrotny, m, m, 1, n);
	cout << "\n";
	cout << "Po zdeszyfrowaniu szyfrogramu, otrzymane 'm':" << endl;
	for (int i = 0; i < m; i++) {
		cout << Wiadomosc_Odczytana[0][i] << " ";
	}
	cout << "\n\n\n\n";
	system("PAUSE");
	return 0;
}
typ2 iloczyn_skalarny(typ2 *v1, typ2 *v2, int size) {
	typ2 iloczyn = 0;
	typ2 kat = 1;
	for (int i = 0; i < size; i++) {
		iloczyn += v1[i] * v2[i] * kat;
	}

	return iloczyn;
}
int convert(int n, vector<int> &cont) {
	int reszta = n % 2;
	if (n > 1) convert((int)(n / 2), cont);
	cont.push_back(reszta);
	return reszta;
}
void Array_Swap(typ2 *a, typ2 *b, int size) {
	typ2 temp;
	for (int i = 0; i < size; i++) {
		temp = a[i];
		a[i] = b[i];
		b[i] = temp;
	}
}
typ2 pot(typ2 a, int n) {
	typ2 value = 1;
	typ2 temp = a;
	vector<int>cont;
	convert(n, cont);
	for (int i = cont.size() - 1; i >= 0; i--) {
		if (cont[i] != 0)
			value *= temp;
		temp *= temp;
	}
	return value;
}
void Wyznacznik_Chio(typ2 **matrix, int size, vector<typ2> &div, typ2 *det) {
	if (size == 2) {
		typ2 temp;
		temp = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		for (int i = 0; i < div.size(); i++) {
			temp /= div[i];
		}
		div.erase(div.begin(),div.end());
		*det = temp;
	}
	else {
		typ2 **mat;
		mat = new typ2 *[size - 1];
		bool Zero_Flag = false;
		for (int i = 0; i < size - 1; i++) mat[i] = new typ2[size - 1];
		if (matrix[0][0] == 0) {
			for (int i = 1; i < size; i++) {
				if (matrix[i][0] != 0) {
					Array_Swap(matrix[0], matrix[i], size);
					div.push_back(-1);
					break;
				}
				if (i == (size - 1)) Zero_Flag = true;
			}
		}
		if (!Zero_Flag) {
			for (int i = 0; i < size - 1; i++) {
				for (int j = 0; j < size - 1; j++) {
					mat[i][j] = matrix[0][0] * matrix[i + 1][j + 1] - matrix[i + 1][0] * matrix[0][j + 1];
				}
			}
			typ2 div_temp = 1;
			div_temp = pot(matrix[0][0], size - 2);
			div.push_back(div_temp);
			Wyznacznik_Chio(mat, size - 1, div, det);
		}
		else {
			*det = 0;
		}
	}
}
typ2 **Macierz_Transponowana(typ2 **matrix, int n, int m) {
	typ2 **Transpozycja = new typ2 *[m];
	for (int i = 0; i < m; i++) {
		Transpozycja[i] = new typ2[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			Transpozycja[j][i] = matrix[i][j];
		}
	}


	return Transpozycja;
}
typ2 **Iloczyn_Macierzy(typ2 **matrix1, typ2 **matrix2, int Matrix1_Columns, int Matrix2_Columns, int Matrix1_Rows, int Matrix2_Rows) {

	if (Matrix1_Columns != Matrix2_Rows)
	{
		cout << "c1:" << Matrix1_Columns << " r1:" << Matrix1_Rows << endl;
		cout << "c2:" << Matrix2_Columns << " r2:" << Matrix2_Rows << endl;
		return NULL;
	}
	else
	{
		typ2 **Iloczyn = new typ2 *[Matrix1_Rows];
		typ2 suma = 0;
		for (int i = 0; i < Matrix1_Rows; i++) {
			Iloczyn[i] = new typ2[Matrix2_Columns];
		}
		for (int i = 0; i < Matrix1_Rows; ++i) {
			for (int j = 0; j < Matrix2_Columns; ++j)
			{
				Iloczyn[i][j] = 0;
			}
		}
		for (int i = 0; i < Matrix1_Rows; i++) {
			for (int j = 0; j < Matrix2_Columns; j++) {
				for (int k = 0; k < Matrix1_Columns; k++) {
					Iloczyn[i][j] += matrix1[i][k] * matrix2[k][j];
				}
			}
		}
		return Iloczyn;
	}
}
double Wspolczynnik_Hadamara(typ2 **Baza, int n, int m) {
	typ2 **Baza_Transponowa;
	Baza_Transponowa = Macierz_Transponowana(Baza, n, m);
	typ2 **Iloczyn;
	Iloczyn = Iloczyn_Macierzy(Baza_Transponowa, Baza, n, m, m, n);
	typ2 Wyznacznik_Iloczynu = 0;
	vector <typ2> div;
	Wyznacznik_Chio(Iloczyn, m, div, &Wyznacznik_Iloczynu);
	if (Wyznacznik_Iloczynu < 0) Wyznacznik_Iloczynu *= (-1);
	typ2 Mianownik = 1;
	double temp;
	for (int i = 0; i < n; i++) {
		temp = sqrt(iloczyn_skalarny(Baza[i], Baza[i], m));
		Mianownik *= temp;
	}
	double value;
	value = sqrt(Wyznacznik_Iloczynu);
	value /= Mianownik;
	value = pow(value, 1 / (double)n);
	return value;
}
typ2 **Psucie_Unimod(typ2 **unimod, int n) {
	typ2 **U = new typ2*[n];
	for (int i = 0; i < n; i++) {
		U[i] = new typ2[n];
	}
	int los;
	int a, b;
	do {
		for (int j = 0; j < 10; j++) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					if (i == j) U[i][j] = 1;
					else U[i][j] = 0;
				}
			}
			for (int i = 0; i < n; i++) {
				a = rand() % (n - 1) + 1;
				b = rand() % a;
				los = rand() % 2;
				if (los == 0)  U[a][b] = 1;
				if (los == 1)  U[a][b] = -1;
			}
			unimod = Iloczyn_Macierzy(unimod, U, n, n, n, n);
		}

	} while (Wspolczynnik_Hadamara(unimod, n, n) > 0.1);
	return unimod;
}
typ2 **Macierz_Dopelnien(typ2 **matrix, int n) {
	typ2 **Dopelnien = new typ2*[n];
	typ2 **Pomocnicza = new typ2*[n - 1];
	typ2 det = 1;
	vector <typ2>div;
	int y = 0, z = 0;
	for (int i = 0; i<n; i++) {
		Dopelnien[i] = new typ2[n];
	}
	for (int i = 0; i<n - 1; i++) {
		Pomocnicza[i] = new typ2[n - 1];
	}
	for (int i = 0; i<n; i++) {
		for (int j = 0; j<n; j++) {
			z = 0;
			y = 0;
			for (int k = 0; k<n; k++) {
				for (int l = 0; l<n; l++) {
					if (k != i && l != j) {
						Pomocnicza[y][z] = matrix[k][l];

					}
					if (l != j) z++;

				}
				z = 0;
				if (k != i) y++;
			}
			Wyznacznik_Chio(Pomocnicza, n - 1, div, &det);
			if ((i + j) % 2 != 0) det *= -1;
			Dopelnien[i][j] = det;
		}
	}

	return Dopelnien;
}
typ2 **Macierz_Odwrotna(typ2 **matrix, int n) {
	typ2 **Odwrotna = new typ2*[n];
	typ2 **Trans;
	typ2 **Dopelnien;
	typ2 det = 1;
	vector <typ2>div;
	int temp;
	for (int i = 0; i<n; i++) {
		Odwrotna[i] = new typ2[n];
	}

	Wyznacznik_Chio(matrix, n, div, &det);
	if (det != 0) {
		if (n != 2) {
			Dopelnien = Macierz_Dopelnien(matrix, n);
			Trans = Macierz_Transponowana(Dopelnien, n, n);
			Odwrotna = Macierz_Mnozenie_Skalar(Trans, 1/(typ2)det, n, n);
		}
		else {
			Odwrotna[0][0] = matrix[1][1];
			Odwrotna[0][1] = (-1)*matrix[0][1];
			Odwrotna[1][0] = (-1)*matrix[1][0];
			Odwrotna[1][1] = matrix[0][0];
			Odwrotna = Macierz_Mnozenie_Skalar(Odwrotna, 1/(typ2)det, n, n);
		}
		return Odwrotna;
	}
	return NULL;
}
typ2 **Macierz_Mnozenie_Skalar(typ2 **matrix, typ2 value, int n, int m) {
	typ2 **Wynik = new typ2*[n];
	for (int i = 0; i<n; i++) {
		Wynik[i] = new typ2[m];
	}

	for (int i = 0; i<n; i++) {
		for (int j = 0; j<m; j++) {
			Wynik[i][j] = matrix[i][j] * value;
		}
	}
	return Wynik;
}
typ2 **Alfa(typ2 **Baza, typ2 **punkt, int n, int m) {
	typ2 **Wynik;
	typ2 **Odwrotna;
	typ2 **Punkt_Trans;
	typ2 **Baza_Trans;
	Baza_Trans = Macierz_Transponowana(Baza, n, m);
	Odwrotna = Macierz_Odwrotna(Baza_Trans, n);

	Punkt_Trans = Macierz_Transponowana(punkt, 1, m);
	Wynik = Iloczyn_Macierzy(Odwrotna, Punkt_Trans, m, 1, n, m);
	for (int i = 0; i<n; i++) {
		Wynik[i][0] = floor(Wynik[i][0] + 0.5);
		cout << Wynik[i][0]<< " ";
	}
	Wynik = Macierz_Transponowana(Wynik, n, 1);
	return Wynik;
}


