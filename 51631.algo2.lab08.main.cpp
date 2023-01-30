// ALGO2 IS1 20C LAB08
// Łukasz Stefański
// sl51631@zut.edu.pl

#include "51631.algo2.lab02.main.h"
#include <fstream>

struct Wezel {
    float x;
    float y;

    Wezel() : x(0.0f), y(0.0f) {
    }

    Wezel(float x, float y) : x(x), y(y) {
    }
};

struct Krawedz {
    int indeksA;
    int indeksB;
    float koszt;

    Krawedz() : indeksA(0), indeksB(0), koszt(0.0f) {
    }

    Krawedz(int a, int b, float koszt) : indeksA(a), indeksB(b), koszt(koszt) {
    }
};

class Graf {
private:
    TablicaDynamiczna<Wezel> wezly;
    TablicaDynamiczna<Krawedz> krawedzie;

    void zaladujGrafZPliku(const std::string &nazwaPliku) {
        std::ifstream plik(nazwaPliku);
        if (plik.is_open()) {
            std::string linia;
            std::getline(plik, linia);
            int liczbaWezlow = std::stoi(linia);
            for (int i = 0; i < liczbaWezlow; i++) {
                std::string x;
                std::string y;
                std::getline(plik, x, ' ');
                std::getline(plik, y);
                dodajWezel(Wezel(std::stof(x), std::stof(y)));
            }

            std::getline(plik, linia);
            int liczbaKrawedzi = std::stoi(linia);
            for (int i = 0; i < liczbaKrawedzi; i++) {
                std::string a;
                std::string b;
                std::string koszt;
                std::getline(plik, a, ' ');
                std::getline(plik, b, ' ');
                std::getline(plik, koszt);
                dodajKrawedz(Krawedz(std::stoi(a), std::stoi(b), std::stof(koszt)));
            }

            plik.close();
        } else {
            std::cerr << "Nie mozna otworzyc pliku";
        }
    }

public:
    Graf() : wezly(TablicaDynamiczna<Wezel>()), krawedzie(TablicaDynamiczna<Krawedz>()) {
    }

    explicit Graf(const std::string &nazwaPliku) : Graf() {
        zaladujGrafZPliku(nazwaPliku);
    }

    void dodajWezel(Wezel wezel) {
        wezly.dodajElement(wezel);
    }

    void dodajKrawedz(Krawedz krawedz) {
        krawedzie.dodajElement(krawedz);
    }

    void sortujKrawedzie() {
        auto komparatorWag = [](const Krawedz &lewa, const Krawedz &prawa) {
            return lewa.koszt <= prawa.koszt;
        };
        krawedzie.sortujMerge(komparatorWag);
    }

    Wezel pobierzWezel(int indeks) {
        if (indeks < 0 || indeks > wezly.pobierzIloscElementow()) {
            throw std::invalid_argument("Wezel poza zakresem.");
        }

        return wezly.zwrocDane(indeks);
    }

    Krawedz pobierzKrawedz(int indeks) {
        if (indeks < 0 || indeks > krawedzie.pobierzIloscElementow()) {
            throw std::invalid_argument("Wezel poza zakresem.");
        }

        return krawedzie.zwrocDane(indeks);
    }

    int pobierzIloscWezlow() {
        return wezly.pobierzIloscElementow();
    }

    int pobierzIloscKrawedzi() {
        return krawedzie.pobierzIloscElementow();
    }

};

class UnionFind {
private:
    int *indeksyRodzicow;
    int *rangi;
    int iloscWezlow;

    int rodzic(int indeks) {
        if (indeks < 0 || indeks >= iloscWezlow) {
            throw std::invalid_argument("Indeks poza zakresem.");
        }

        return indeksyRodzicow[indeks];
    }

public:
    explicit UnionFind(int iloscWezlow) : indeksyRodzicow(new int[iloscWezlow]), rangi(new int[iloscWezlow]),
                                          iloscWezlow(iloscWezlow) {
        for (int i = 0; i < iloscWezlow; i++) {
            // utworzenie jednoelementowego zbioru dla kazdego wezla, gdzie reprezentantem jest on sam
            indeksyRodzicow[i] = i;
            rangi[i] = 0;
        }
    }

    ~UnionFind() {
        delete[] indeksyRodzicow;
        delete[] rangi;
    }

    // b) laczenie dwoch zbiorow, argumenty dwa indeksy wezlow
    // zrobic na dwa sposoby - zwykle oraz union-by-rank
    void scalZbiory(int reprezentantA, int reprezentantB) {
        if (reprezentantA == reprezentantB) {
            throw std::invalid_argument("Zbiory nie sa rozlaczne.");
        }

        indeksyRodzicow[reprezentantB] = reprezentantA;
    }

    void scalZbioryUnionByRank(int reprezentantA, int reprezentantB) {
        if (reprezentantA == reprezentantB) {
            throw std::invalid_argument("Zbiory nie sa rozlaczne.");
        }

        if (rangi[reprezentantA] < rangi[reprezentantB]) {
            indeksyRodzicow[reprezentantA] = reprezentantB;
        } else if (rangi[reprezentantA] > rangi[reprezentantB]) {
            indeksyRodzicow[reprezentantB] = reprezentantA;
        } else {
            indeksyRodzicow[reprezentantB] = reprezentantA;
            rangi[reprezentantA]++;
        }
    }

    int licznikWyszukan = 0;

    // c) znajdowanie reprezentanta zbioru, arg indeks wezla, wynik indeks reprezentanta
    // dwa sposoby - bez i z kompresja sciezki
    int znajdzReprezentanta(int indeks) {
        while (indeks != rodzic(indeks)) {
            licznikWyszukan++;
            indeks = rodzic(indeks);
        }
        licznikWyszukan++;
        return indeks;
    }

    int znajdzReprezentantaPathCompression(int indeks) {
        licznikWyszukan++;
        if (indeks != rodzic(indeks)) {
            indeksyRodzicow[indeks] = znajdzReprezentantaPathCompression(rodzic(indeks));
        }

        return rodzic(indeks);
    }
};

TablicaDynamiczna<Krawedz> algorytmKruskala(Graf graf, bool unionByRank, bool pathCompression) {
    UnionFind unionFind = UnionFind(graf.pobierzIloscWezlow());

    clock_t czasStartuSortowania = clock();
    graf.sortujKrawedzie();
    clock_t czasKoncaSortowania = clock();
    long czasSortowaniaMs = (czasKoncaSortowania - czasStartuSortowania) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Sortowanie zajelo:  " << czasSortowaniaMs << "ms.\n";

    TablicaDynamiczna<Krawedz> wynikowyPodzbiorKrawedzi = TablicaDynamiczna<Krawedz>();
    clock_t czasStartuGlownejPetli = clock();
    for (int i = 0; i < graf.pobierzIloscKrawedzi(); i++) {
        int reprezentantWezlaA;
        int reprezentantWezlaB;
        if (pathCompression) {
            reprezentantWezlaA = unionFind.znajdzReprezentantaPathCompression(graf.pobierzKrawedz(i).indeksA);
            reprezentantWezlaB = unionFind.znajdzReprezentantaPathCompression(graf.pobierzKrawedz(i).indeksB);
        } else {
            reprezentantWezlaA = unionFind.znajdzReprezentanta(graf.pobierzKrawedz(i).indeksA);
            reprezentantWezlaB = unionFind.znajdzReprezentanta(graf.pobierzKrawedz(i).indeksB);
        }

        if (reprezentantWezlaA != reprezentantWezlaB) {
            wynikowyPodzbiorKrawedzi.dodajElement(graf.pobierzKrawedz(i));
            if (unionByRank) {
                unionFind.scalZbioryUnionByRank(reprezentantWezlaA, reprezentantWezlaB);
            } else {
                unionFind.scalZbiory(reprezentantWezlaA, reprezentantWezlaB);
            }
        }
    }
    clock_t czasKoncaGlownejPetli = clock();
    long czasObliczenGlownejPetli = (czasKoncaGlownejPetli - czasStartuGlownejPetli) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Glowna petla w algorytmie Kruskala zajela: " << czasObliczenGlownejPetli << "ms.\n";

    std::cout << "Liczba wykonan operacji find: " << unionFind.licznikWyszukan << '\n';

    return wynikowyPodzbiorKrawedzi;
}

void testMergeSort() {
    TablicaDynamiczna<int> tablica = TablicaDynamiczna<int>();
    for (int i = 9; i > 0; i = i - 2) {
        tablica.dodajElement(i);
    }
    for (int i = 0; i < 10; i = i + 2) {
        tablica.dodajElement(i);
    }
    auto wyswietlInt = [](const int &i) { std::cout << i; };
    tablica.wyswietlInfo(10, wyswietlInt);
    auto komparatorInt = [](const int &lewy, const int &prawy) { return lewy <= prawy; };
    tablica.sortujMerge(komparatorInt);
    tablica.wyswietlInfo(10, wyswietlInt);
}

void testLadowaniaGrafuZPliku() {
    Graf graf = Graf("g1.txt");
    std::cout << "Pierwszy wezel : " << graf.pobierzWezel(0).x << ' ' << graf.pobierzWezel(0).y << '\n';
    std::cout << "Ostatni wezel : " << graf.pobierzWezel(19).x << ' ' << graf.pobierzWezel(19).y << '\n';
    std::cout << "Pierwsza krawedz : " << graf.pobierzKrawedz(0).indeksA << ' ' << graf.pobierzKrawedz(0).indeksB
              << ' ' << graf.pobierzKrawedz(0).koszt << '\n';
    std::cout << "Ostatnia krawedz : " << graf.pobierzKrawedz(64).indeksA << ' ' << graf.pobierzKrawedz(64).indeksB
              << ' ' << graf.pobierzKrawedz(64).koszt << '\n';

}

void eksperyment(bool unionByRank, bool pathCompression) {
    std::string nazwyPlikow[3] = {"g1.txt", "g2.txt", "g3.txt"};

    for (const auto &nazwaPliku: nazwyPlikow) {
        std::cout << "--------------------------------------\nGraf: " << nazwaPliku
                  << "\nUnionByRank: " << unionByRank << " PathCompression: " << pathCompression << '\n'
                  << "\n--------------------------------------\n";

        Graf graf = Graf(nazwaPliku);
        TablicaDynamiczna<Krawedz> wynik = algorytmKruskala(graf, unionByRank, pathCompression);
        std::cout << "Liczba krawedzi: " << wynik.pobierzIloscElementow() << '\n';
        float sumaWag = 0.0f;
        for (int j = 0; j < wynik.pobierzIloscElementow(); j++) {
            sumaWag += wynik[j].koszt;
        }
        std::cout << "Suma wag: " << sumaWag << '\n';

    }

}

int main() {
    eksperyment(false, false);
    eksperyment(true, false);
    eksperyment(false, true);
    eksperyment(true, true);
//    testMergeSort();
//    testLadowaniaGrafuZPliku();


    return 0;
}

