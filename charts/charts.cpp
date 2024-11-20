#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <cstring>
using namespace std;
#pragma warning(disable : 4996)



struct Dane_Rynkowe
{
    string data = " ";
    double open = 0;
    double high = 0;
    double low = 0;
    double close = 0;
    char body = ' ';
    int dolna_poz_cienia = 0, gorna_poz_cienia = 0, poz_ciala_open = 0, poz_ciala_close = 0;
};

void wiadomosc_powitalna()
{
    HANDLE kolor = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(kolor, FOREGROUND_BLUE || FOREGROUND_INTENSITY);

    cout << "WITAJ W PROGRAMIE RYSUJACYM JAPONSKIE SWIECE GIELDOWE NA PODSTAWIE PLIKU Z DANYMI..." << endl;
    cout << "Autor: Igor Kruszewski || indeks: 198062 || 15.01.2024" << endl;
    cout << "-kliknij q aby wyjsc z programu " << endl;
    cout << "-kliknij g aby rozpoczac program" << endl;

}

int CSV_KONWERTUJ(char*& plik_wejsciowy, Dane_Rynkowe*& swieczki, double& maksimum, double& minimum, int& MAX_WYS, int& MAX_SWIEC)
{
    fstream dane(plik_wejsciowy, ios::in);
    if (!dane.is_open())
    {
        cerr << "Error: nie mozna otworzyc pliku" << endl;
        return 1;
    }
    int licznik = 0;
    string linia;
    while (getline(dane, linia))
    {
        licznik++;
    }
    dane.close();

    dane.open("intc_us_data.csv", ios::in);
    for (int i = 0; i < licznik - MAX_SWIEC; i++)
    {
        getline(dane, linia);

    }

    for (int i = 0; i < MAX_SWIEC; i++)
    {
        getline(dane, linia, ',');
        swieczki[i].data = linia;

        getline(dane, linia, ',');
        swieczki[i].open = stod(linia);

        getline(dane, linia, ',');
        swieczki[i].high = stod(linia);

        getline(dane, linia, ',');
        swieczki[i].low = stod(linia);

        getline(dane, linia, ',');
        swieczki[i].close = stod(linia);

        maksimum = max(maksimum, swieczki[i].high);
        minimum = min(minimum, swieczki[i].low);
    }
    dane.close();
}


void Inicjalizacja_S_J(Dane_Rynkowe*& swieczki, double& jednostka, double& srednia_wynikowa, double& maksimum, double& minimum, int& MAX_WYS, int& MAX_SWIEC)
{
    for (int i = 0; i < MAX_SWIEC; i++)
    {
        swieczki[i].body = (swieczki[i].open >= swieczki[i].close) ? '#' : 'O';
    }

    jednostka = (maksimum - minimum) / (MAX_WYS - 1);
    srednia_wynikowa = maksimum;
    for (int i = 0; i < MAX_SWIEC; i++)
    {
        swieczki[i].gorna_poz_cienia = static_cast<int>((maksimum - swieczki[i].high) / jednostka);
        swieczki[i].dolna_poz_cienia = static_cast<int>((maksimum - swieczki[i].low) / jednostka);
        swieczki[i].poz_ciala_open = static_cast<int>((maksimum - swieczki[i].open) / jednostka);
        swieczki[i].poz_ciala_close = static_cast<int>((maksimum - swieczki[i].close) / jednostka);
    }
}


void G_T_S_J(Dane_Rynkowe*& swieczki, string**& przechowuj_swiece, double& jednostka, double& srednia_wynikowa, int& MAX_WYS, int& MAX_SWIEC)
{

    for (int i = 0; i < MAX_WYS; i++)
    {
        for (int j = 0; j < MAX_SWIEC + 2; j++)
        {
            przechowuj_swiece[i][j] = '.';
        }
    }
    const int maks_numer = 8;
    char konwerter_na_char[maks_numer];
    char znak_przedzialki = ' |';
    for (int i = 0; i < MAX_WYS; i++)
    {

        cout.precision(2);
        cout << fixed << setprecision(2);
        snprintf(konwerter_na_char, sizeof(konwerter_na_char), "%.2f", srednia_wynikowa);
        strcat(konwerter_na_char, "|");
        przechowuj_swiece[i][0] = konwerter_na_char;
        srednia_wynikowa = srednia_wynikowa - jednostka;
    }


    return;
}

void R_W_S_J(Dane_Rynkowe*& swieczki, string**& przechowuj_swiece, int& MAX_WYS, int& MAX_SWIEC, double& maksimum, double& minimum)
{
    int licznik_dni = 0;
    HANDLE kolor = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < MAX_SWIEC; i++)
    {
        for (int j = swieczki[i].gorna_poz_cienia; j <= swieczki[i].dolna_poz_cienia; j++)
        {
            przechowuj_swiece[j][i + 2] = '|';
        }
    }
    for (int i = 0; i < MAX_SWIEC; i++)
    {
        for (int j = min(swieczki[i].poz_ciala_open, swieczki[i].poz_ciala_close); j <= max(swieczki[i].poz_ciala_open, swieczki[i].poz_ciala_close); j++)
        {
            przechowuj_swiece[j][i + 2] = swieczki[i].body;
        }
    }
    for (int i = 0; i < MAX_WYS; i++)
    {
        for (int j = 0; j < MAX_SWIEC; j++)
        {
            SetConsoleTextAttribute(kolor, 2);
            cout << przechowuj_swiece[i][j];

        }
        Sleep(1);
        cout << endl;
    }

    int miesiac = 1;
    int licznik = 0;
    cout << "XXXXX|";
    for (int i = 0; i < MAX_SWIEC - 1; i++)
    {
        cout << "_";
    }
    cout << endl;
    cout << "Wartość maksymalna: " << maksimum << endl;
    cout << "Watosc minimalna: " << minimum << endl;
    cout << endl;


}

int zapis_do_chart(char*& plik_wyjsciowy, string**& przechowuj_swiece, int& MAX_WYS, int& MAX_SWIEC)
{
    ofstream chart(plik_wyjsciowy);
    if (!chart.is_open())
    {
        cerr << "Error: nie mozna otworzyc pliku" << endl;
        return 1;
    }
    for (int i = 0; i < MAX_WYS; i++)
    {
        for (int j = 0; j < MAX_SWIEC; j++)
        {
            chart << przechowuj_swiece[i][j];
        }
        chart << endl;
    }
    cout << "Zapisano wykres do pliku --> " << plik_wyjsciowy << endl;
    chart.close();
    return 1;


}

void odczyt_logs(const string& input)
{
    ofstream akcje_uzytkownika("odczyt_akcji_uzytkownika.log", ios_base::app);
    if (!akcje_uzytkownika.is_open())
    {
        cout << "ERROR: nie ma pliku do zapisu..." << endl;
    }
    else
    {
        time_t start = time(0);
        tm* czas = localtime(&start);
        akcje_uzytkownika << asctime(czas) << ":" << input << endl;
        akcje_uzytkownika.close();
    }
}


int main()
{
    odczyt_logs("WCZYTANO PROGRAM...");
    bool status = true;
    int MAX_WYS;
    int MAX_SWIEC;
    char input;
    double maksimum = 0;
    double minimum = 1000000;
    double jednostka = 0;
    double srednia_wynikowa = 0;
    char* plik_wejsciowy = new char[20];
    char* plik_wyjsciowy = new char[20];
    wiadomosc_powitalna();
    cin >> input;


    while (status)
    {



        if (input == 'g' || input == 'G')
        {
            odczyt_logs("WCZYTANO KLAWISZ G...");
            cout << "Wybierz szerokość i wysokosć wykresu..." << endl;
            cin >> MAX_WYS;
            odczyt_logs("WCZYTANO MAX_WYS...");
            cin >> MAX_SWIEC;
            odczyt_logs("WCZYTANO MAX_SWIEC...");
            cout << "Wybierz plik wejsciowy i wyjciowy..." << endl;
            cout << "Plik wejsciowy:";
            cin >> plik_wejsciowy;
            odczyt_logs("WCZYTANO PLIK_WEJSCIOWY...");
            cout << "\nPlik wyjsciowy:";
            cin >> plik_wyjsciowy;
            odczyt_logs("WCZYTANO PLIK WYJSCIOWY...");
            cout << endl;
            Dane_Rynkowe* swieczki = new Dane_Rynkowe[MAX_SWIEC + 2];
            string** przechowuj_swiece = new string * [MAX_WYS];
            for (int i = 0; i < MAX_WYS; i++)
            {
                przechowuj_swiece[i] = new string[MAX_SWIEC + 2];
            }
            cout << "Rysowanie rozpoczete..." << endl;
            Sleep(1000);
            CSV_KONWERTUJ(plik_wejsciowy, swieczki, maksimum, minimum, MAX_WYS, MAX_SWIEC);
            Inicjalizacja_S_J(swieczki, jednostka, srednia_wynikowa, maksimum, minimum, MAX_WYS, MAX_SWIEC);
            G_T_S_J(swieczki, przechowuj_swiece, jednostka, srednia_wynikowa, MAX_WYS, MAX_SWIEC);
            R_W_S_J(swieczki, przechowuj_swiece, MAX_WYS, MAX_SWIEC, maksimum, minimum);
            zapis_do_chart(plik_wyjsciowy, przechowuj_swiece, MAX_WYS, MAX_SWIEC);
            status = false;
        }
        else if (input == 'q' || input == 'Q')
        {
            odczyt_logs("WCZYTANO KLAWISZ Q...");
            exit(0);
            status = false;
        }
        else
        {
            cin >> input;
            odczyt_logs("WCZYTANO KLAWISZ...");
        }
    }
    odczyt_logs("ZAMKNIETO PROGRAM...");
    cout << "Zapisano akcje użytkownika do pliku --> odczyt_akcji_uzytkownika.log" << endl;
    return 0;

}