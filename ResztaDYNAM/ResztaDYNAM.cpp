#include <iostream>
#include <fstream>
#include <tuple>

#define en 10
#define intMax 999999

using namespace std;

tuple <int*, int>wydajReszteDynamiczny(int* banknoty, int reszta, int x, int _size,int* _b) {
    // banknoty- {0 1 1 1 4 4 4 9 9 9}, reszta - 8, x - 3 (ilość różnych monet), _size - ilość monet + 1, b- {1 4 5}
    int minBank; // Ilość banknotów potrzebnych do wydania reszty
    int* wynik = new int[x]; // Tablica z ilości różnych banknotów
    for (int i = 0; i < x; i++) // Zerowanie wynikowej tablicy
    {
        wynik[i] = 0;
    }
    int row = _size;  // Ilość wierszy w tablicy -> liczba_banktotów+1      9
    int col = reszta + 1; // Ilość kolumn w tablicy -> reszta+1             9

    int** tab = new int* [row]; // Wiersze w tablicy - pojedyncze bankonoty

    for (int i = 0; i < row; i++) // Kolumny w tablicy - 9 - możlwiość reszty do wydania
    {
        tab[i] = new int[col];
    }    

    for (int i = 0; i < row; i++)           // Inicjalizacja tablicy pierwsza kolumna - 0, środek - intMax ( ∞ )
    {
        for (int j = 0; j < col; j++)
        {
            if (j!=0 )tab[i][j] = intMax;
            else tab[i][j] = 0;
        }
    }

/*
Banknoty/Reszta
           0 1 2 3 4 5 6 7 8
          ___________________
        0| 0 ∞ ∞ ∞ ∞ ∞ ∞ ∞ ∞        tab[0][j]
        1| 0 1 ∞ ∞ ∞ ∞ ∞ ∞ ∞        tab[1][j]
        1| 0 1 2 ∞ ∞ ∞ ∞ ∞ ∞        tab[1][j]
        1| 0 1 2 3 ∞ ∞ ∞ ∞ ∞        tab[2][j]
        4| 0 1 2 3 1 2 3 4 ∞        tab[3][j]
        4| 0 1 2 3 1 2 3 4 2        tab[4][j]
        4| 0 1 2 3 1 2 3 4 2        tab[5][j]
        5| 0 1 2 3 1 1 2 3 2        tab[6][j]
        5| 0 1 2 3 1 1 2 3 2        tab[7][j]
        5| 0 1 2 3 1 1 2 3 2        tab[8][j]
*/
    // Stworzenie wypełnionej tabeli ze wszystkimi optymalnymi rozwiązaniami na wydanie reszty od [1...reszta]
    for (int i = 1; i < row; i++)
    {
        for (int j = 1; j < col; j++)       // j - aktualna reszta
        {  
            if (j - banknoty[i] < 0) {  // Przepisanie wartości z góry dla kwot mniejszych od nominałów
                tab[i][j] = tab[i - 1][j];
            }
            else
            {
                if (tab[i - 1][j - banknoty[i]] + 1 < tab[i - 1][j]) // Jeżeli dodanie nowego banknotu zmniejszy ilość banknotów potrzebnych do wydania reszty
                {                                                    // dodajemy dany banknot do tablicy.
                    tab[i][j] = tab[i - 1][j - banknoty[i]] + 1;
                }
                else                            // W przeciwnym wypadku używamy tych samych banknotów do wydania reszty
                {
                    tab[i][j] = tab[i - 1][j]; 
                }
            }
            
        }
    }
    minBank = tab[row - 1][col - 1]; // Najmniejsza możlwia liczba monet do wydania -> ostatnie pole tablicy

    /////////////////////////////////////////    Szukanie monet zapewniających reszte    /////////////////////////////////////////   
/*
Banknoty/Reszta
           0 1 2 3 4 5 6 7 8
          ___________________
        0| 0 ∞ ∞ ∞ ∞ ∞ ∞ ∞ ∞        tab[0][j]
        1| 0 1 ∞ ∞ ∞ ∞ ∞ ∞ ∞        tab[1][j]
        1| 0 1 2 ∞ ∞ ∞ ∞ ∞ ∞        tab[1][j]
        1| 0 1 2 3 ∞ ∞ ∞ ∞ ∞        tab[2][j]
        4| 0 1 2 3 1 2 3 4 ∞        tab[3][j]
        4| 0 1 2 3 1 2 3 4 2        tab[4][j]
        4| 0 1 2 3 1 2 3 4 2        tab[5][j]
        5| 0 1 2 3 1 1 2 3 2        tab[6][j]
        5| 0 1 2 3 1 1 2 3 2        tab[7][j]
        5| 0 1 2 3 1 1 2 3 2        tab[8][j]
*/
    int e= tab[row - 1][col - 1];  // e = ostatnie pole tablicy
    int* pom = new int[_size];     // Tablica przechowująca wszystkie wykorzystane banknkoty potrzebne do wydania reszty
    for (int i = 0; i < _size; i++)
    {
        pom[i] = 0;
    }

    int i = row - 1,j= col - 1, p = 0;
    if (e == intMax) {                  // Jeżeli ostatnie pole tablicy zawiera wartość intMax to oznacza, że nie można wydać reszty.
        cout << "Reszta jest niemozliwa do wydania. " << endl;
        return make_tuple(new int[3]{ 0,0,0 }, NULL);
    }
    else {
        while (e != 0) {              // Dopóki wartość e nie dojdzie do 0 (reszty równej 0 )
            if (e == tab[i - 1][j]) { // Jeżeli wartość powyżej 'e' jest równa 'e' to idziemy do góry
                e = tab[i - 1][j];
                i--;
            }
            else {
                e = tab[i][j - banknoty[i]];    // e jest równe wartości pod indeksem tego samego wiersza oraz kolumny j - wartość banknotu
                pom[p++] = banknoty[i];         // Dodanie danego banknotu do wyniku
                j -= banknoty[i];               
                e = tab[i - 1][j];              // Ustawiamy e na wiersz wyżej
            }
        }
    }
    // Zapisywanie ilości róznych banknotów do tablicy wynikowej
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            if (_b[i] == pom[j])wynik[i]++; // Jeżeli wartość danego banknotu zgadza się z tablicą pom, zwiększamy wynik o 1
        }
    }

    // Wyświetlanie tablicy z resztami
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cout << tab[i][j] << " ";
        }
        cout << endl;
    }
    return make_tuple(wynik, minBank);
}

int* banknoty(int* nominaly, int* ileNom, int x, int _size) // Funkcja zwracająca wszystkie banknoty użyte w problemie ( razem z 0 na początku)
{
    int* wynik = new int[_size];     // Tablica ze wszystkimi banknotami (+0) posegregowanymi od najmniejszego do największego 0 1 1 1 4 4 4 5 5 5
    
    wynik[0] = 0;
    int w = 1;
    for (int i = 0; i < x; i++)
    {
        int j = 0;
        while (j < ileNom[i]) {
            wynik[w++] = nominaly[i];
            j++;
        }
    }
    return wynik;
}

int sizeFunc(int*_c,int _n) // Zwraca ilość wszystkich banknotów + 0
{
    int size = 0;
    for (int i = 0; i < _n; i++)
    {
        size += _c[i];
    }
    return size + 1;
}

int main() {

    int n, reszta;
    ifstream read("PKO_in_Pozniewski_8.txt");
    ofstream save("PKO_out_Pozniewski_8.txt");

    read >> n;  // Wczytanie ilości różnych nominałów
    int* b = new int[n]; // Tablica nominałów
    int* c = new int[n]; // Liczba banknotów o nominale b[i]

    for (int i = 0; i < n; i++) // Wczytanie nominałów
    {
        read >> b[i];
    }
    for (int i = 0; i < n; i++) // Wczytanie ilosci nominalow
    {
        read >> c[i];
    }
    read >> reszta; // Wczytanie reszty
    read.close();

    int lBank; // Ilość banktotów potrzebna do wydania reszty
    int* wyntab = new int[n];  // Jakie banknoty zawierają się w reszcie

    int size = sizeFunc(c, n); // Ilość wszystkich monet + 1

    int* banknotyTab = new int[size];  // Tablica ze wszystkim banknotami uporządkowanymi niemalejąco 0 1 1 1 4 4 4 5 5 5
    banknotyTab = banknoty(b, c, n, size);
                                              // Wszystkie banknoty|reszta do  wydania| ilość różnych banknotów | ilość wszystkich banknotów| wartości monet
    tie(wyntab, lBank) = wydajReszteDynamiczny(banknotyTab, reszta, n, size,b); // Jakie banknoty zostały użyte i ile

    save << lBank << endl;
    cout << "\nReszta: " << reszta << endl;
    cout << "Minimalna liczba monet potrzebna do wydania reszty: " << lBank << endl;
    cout << "Banknoty: " << endl;
    for (int i = 0; i < n; i++)
    {
        cout << b[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        cout << wyntab[i] << " ";
        save << wyntab[i] << " ";
    }
    save.close();

}