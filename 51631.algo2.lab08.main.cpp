// ALGO2 IS1 20C LAB08
// Łukasz Stefański
// sl51631@zut.edu.pl

#include "51631.algo2.lab02.main.h"

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

public:
    Graf() : wezly(TablicaDynamiczna<Wezel>()), krawedzie(TablicaDynamiczna<Krawedz>()) {
    }

    void dodajWezel(Wezel wezel) {
        wezly.dodajElement(wezel);
    }

    void dodajKrawedz(Krawedz krawedz) {
        krawedzie.dodajElement(krawedz);
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

    // c) znajdowanie reprezentanta zbioru, arg indeks wezla, wynik indeks reprezentanta
    // dwa sposoby - bez i z kompresja sciezki
    int znajdzReprezentanta(int indeks) {
        while (indeks != rodzic(indeks)) {
            indeks = rodzic(indeks);
        }

        return indeks;
    }

    int znajdzReprezentantaPathCompression(int indeks) {
        if (indeks != rodzic(indeks)) {
            indeksyRodzicow[indeks] = znajdzReprezentantaPathCompression(rodzic(indeks));
        }

        return rodzic(indeks);
    }
};

int main() {

    return 0;
}
