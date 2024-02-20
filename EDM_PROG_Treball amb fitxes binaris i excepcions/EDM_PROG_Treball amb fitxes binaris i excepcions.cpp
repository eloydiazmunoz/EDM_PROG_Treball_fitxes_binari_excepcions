#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>

using namespace std;

// Definim una estructura per a les sèries de televisió
struct SerieTV {
    string titol;                   // Títol de la sèrie
    int nombre_episodis;            // Nombre d'episodis
    int personatges_per_episodi;    // Nombre de personatges per episodi
};

// Funció per desar les dades de les sèries en un fitxer binari
void desar_dades(const vector<SerieTV>& series) {
    ofstream fitxer("dades_series.bin", ios::binary);
    if (!fitxer.is_open()) {
        throw runtime_error("No s'ha pogut obrir el fitxer per desar les dades.");
    }

    // Escrivim cada estructura SerieTV al fitxer binari
    for (const SerieTV& serie : series) {
        fitxer.write(reinterpret_cast<const char*>(&serie), sizeof(SerieTV));
    }
}

// Funció per carregar les dades de les sèries des d'un fitxer binari
vector<SerieTV> carregar_dades() {
    vector<SerieTV> series;
    ifstream fitxer("dades_series.bin", ios::binary);
    if (!fitxer.is_open()) {
        return series;  // Si el fitxer no s'obre, retorna un vector buit
    }

    SerieTV serie;
    // Llegim cada estructura SerieTV del fitxer binari i l'afegim al vector de sèries
    while (fitxer.read(reinterpret_cast<char*>(&serie), sizeof(SerieTV))) {
        series.push_back(serie);
    }

    return series;
}

// Funció per afegir una nova sèrie al vector de sèries i desar les dades
void afegir_serie(vector<SerieTV>& series, const string& titol, int nombre_episodis, int personatges_per_episodi) {
    series.push_back({ titol, nombre_episodis, personatges_per_episodi });
    desar_dades(series);
}

// Funció per esborrar una sèrie del vector de sèries i desar les dades
void esborrar_serie(vector<SerieTV>& series, const string& titol) {
    series.erase(remove_if(series.begin(), series.end(),
        [&titol](const SerieTV& s) { return s.titol == titol; }),
        series.end());
    desar_dades(series);
}

// Funció per calcular estadístiques sobre les sèries
void calcular_estadistiques(const vector<SerieTV>& series) {
    if (series.empty()) {
        cout << "No hi ha dades disponibles." << endl;
        return;
    }

    // Vector per emmagatzemar el nombre de personatges per episodi de cada sèrie
    vector<int> comptes_personatges;
    for (const SerieTV& serie : series) {
        comptes_personatges.push_back(serie.personatges_per_episodi);
    }

    double suma = 0;
    for (int compte : comptes_personatges) {
        suma += compte;
    }
    double mitjana_personatges = suma / comptes_personatges.size();

    // Funció lambda per calcular la moda dels personatges per episodi
    auto moda = [&comptes_personatges]() {
        int valor_moda = 0;
        int comptat_maxim = 0;
        for (int valor : comptes_personatges) {
            int comptat = count_if(comptes_personatges.begin(), comptes_personatges.end(), [&valor](int x) { return x == valor; });
            if (comptat > comptat_maxim) {
                comptat_maxim = comptat;
                valor_moda = valor;
            }
        }
        return valor_moda;
        };

    // Calculem la mediana dels personatges per episodi
    sort(comptes_personatges.begin(), comptes_personatges.end());
    int mig = comptes_personatges.size() / 2;
    double mediana_personatges = (comptes_personatges.size() % 2 != 0) ? comptes_personatges[mig] : (comptes_personatges[mig - 1] + comptes_personatges[mig]) / 2.0;

    // Mostrem les estadístiques calculades
    cout << "Mitjana de personatges per episodi: " << mitjana_personatges << endl;
    cout << "Moda de personatges per episodi: " << moda() << endl;
    cout << "Mediana de personatges per episodi: " << mediana_personatges << endl;
}

// Funció principal
int main() {
    vector<SerieTV> series = carregar_dades(); // Carreguem les dades de les sèries des del fitxer

    while (true) {
        // Menú d'opcions per a l'usuari
        cout << "\n1. Afegir serie" << endl;
        cout << "2. Esborrar serie" << endl;
        cout << "3. Calcular estadistiques" << endl;
        cout << "4. Sortir" << endl;

        int opcio;
        cout << "Selecciona una opcio: ";
        cin >> opcio;

        // Segons l'opció seleccionada, cridem la funció corresponent
        if (opcio == 1) {
            string titol;
            int nombre_episodis, personatges_per_episodi;
            cout << "Introdueix el titol de la serie: ";
            cin.ignore();
            getline(cin, titol);
            cout << "Introdueix el nombre d'episodis: ";
            cin >> nombre_episodis;
            cout << "Introdueix el nombre de personatges per episodi: ";
            cin >> personatges_per_episodi;
            afegir_serie(series, titol, nombre_episodis, personatges_per_episodi);
        }
        else if (opcio == 2) {
            string titol;
            cout << "Introdueix el titol de la serie a esborrar: ";
            cin.ignore();
            getline(cin, titol);
            esborrar_serie(series, titol);
        }
        else if (opcio == 3) {
            calcular_estadistiques(series);
        }
        else if (opcio == 4) {
            break; // Sortim del bucle si l'usuari selecciona l'opció 4
        }
        else {
            cout << "Opcio no valida." << endl;
        }
    }

    return 0;
}
