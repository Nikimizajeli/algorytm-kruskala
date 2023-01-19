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

int main() {

    return 0;
}
