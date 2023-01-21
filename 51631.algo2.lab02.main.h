//
// Created by Lukasz Stefanski on 16.12.2022.
//

#ifndef LAB06_51631_ALGO2_LAB02_MAIN_H
#define LAB06_51631_ALGO2_LAB02_MAIN_H

#include <iostream>
#include <random>

template<typename T>
class TablicaDynamiczna {
private:
    const int POCZATKOWY_ROZMIAR = 1;
    const int WSPOLCZYNNIK_POWIEKSZANIA = 2;

    T *tablica;
    int iloscElementow;
    int rozmiarTablicy;

    void merge(const int poczatekTablicy, const int srodekTablicy, const int koniecTablicy,
               bool (*komparator)(const T &, const T &)) {
        int rozmiarLewej = srodekTablicy - poczatekTablicy + 1;
        int rozmiarPrawej = koniecTablicy - srodekTablicy;

        T *lewaTablica = new T[rozmiarLewej];
        T *prawaTablica = new T[rozmiarPrawej];

        for (int i = 0; i < rozmiarLewej; i++) {
            lewaTablica[i] = tablica[poczatekTablicy + i];
        }
        for (int i = 0; i < rozmiarPrawej; i++) {
            prawaTablica[i] = tablica[srodekTablicy + i + 1];
        }

        int indeksLewej = 0;
        int indeksPrawej = 0;
        int indeksGlownej = poczatekTablicy;

        while (indeksLewej < rozmiarLewej && indeksPrawej < rozmiarPrawej) {
            if (komparator(lewaTablica[indeksLewej], prawaTablica[indeksPrawej])) {
                tablica[indeksGlownej] = lewaTablica[indeksLewej];
                indeksLewej++;
            } else {
                tablica[indeksGlownej] = prawaTablica[indeksPrawej];
                indeksPrawej++;
            }
            indeksGlownej++;
        }

        while (indeksLewej < rozmiarLewej) {
            tablica[indeksGlownej] = lewaTablica[indeksLewej];
            indeksGlownej++;
            indeksLewej++;
        }

        while (indeksPrawej < rozmiarPrawej) {
            tablica[indeksGlownej] = prawaTablica[indeksPrawej];
            indeksGlownej++;
            indeksPrawej++;
        }

        delete[] lewaTablica;
        delete[] prawaTablica;
    }

    void mergeSort(int poczatekPodtablicy, int koniecPodtablicy, bool (*komparator)(const T &, const T &)) {
        if (poczatekPodtablicy >= koniecPodtablicy) {
            return;
        }

        int srodekPodtablicy = poczatekPodtablicy + (koniecPodtablicy - poczatekPodtablicy) / 2;
        mergeSort(poczatekPodtablicy, srodekPodtablicy, komparator);
        mergeSort(srodekPodtablicy + 1, koniecPodtablicy, komparator);
        merge(poczatekPodtablicy, srodekPodtablicy, koniecPodtablicy, komparator);
    }

public:
    TablicaDynamiczna() : tablica(new T[POCZATKOWY_ROZMIAR]), iloscElementow(0), rozmiarTablicy(POCZATKOWY_ROZMIAR) {
    }

    ~TablicaDynamiczna() {
        delete[] tablica;
        tablica = nullptr;
    }

    // a) dodanie nowego elementu na koncu tablicy
    void dodajElement(const T &dane) {
        if (iloscElementow >= rozmiarTablicy) {
            int nowyRozmiar = rozmiarTablicy * WSPOLCZYNNIK_POWIEKSZANIA;
            T *nowaTablica = new T[nowyRozmiar];
            for (int i = 0; i < iloscElementow; i++) {
                nowaTablica[i] = tablica[i];
            }

            delete[] tablica;
            tablica = nowaTablica;
            rozmiarTablicy = nowyRozmiar;
        }

        tablica[iloscElementow] = dane;
        iloscElementow++;
    }

    // b) zwrocenie danych i-tego elementu
    T &zwrocDane(int index) {
        if (index < 0 || index >= iloscElementow) {
            throw std::runtime_error("Index poza zakresem.\n");
        }
        return tablica[index];
    }

    T &operator[](int index) {
        return zwrocDane(index);
    }

    // c) podmiana i-tego elementu
    void podmien(int index, const T &dane) {
        if (index < 0 || index >= iloscElementow) {
            std::cout << "[Podmien] Index poza zakresem.\n";
            return;
        }

        tablica[index] = dane;
    }

    // d) czyszczenie tablicy
    void usunWszystkie() {
        for (int i = 0; i < iloscElementow; i++) {
            tablica[i].~T();
        }

        delete[] tablica;
        tablica = new T[rozmiarTablicy];    // POCZATKOWY_ROZMIAR zamiast rozmiarTablicy?

        iloscElementow = 0;
    }

    // e) napisowa reprezentacja tablicy
    void wyswietlInfo(int iloscElementowDoWyswietlenia, void (*wyswietlElement)(const T &)) const {
        if (iloscElementowDoWyswietlenia < 0) {
            std::cout << "[wyswietlInfo] Nie mozna wyswietlic negatywnej ilosci elementow.\n";
            return;
        }

        std::cout << "------------------------\nRozmiar tablicy: " << rozmiarTablicy
                  << "\nIlosc elementow: " << iloscElementow << '\n';

        iloscElementowDoWyswietlenia = std::min(iloscElementowDoWyswietlenia, iloscElementow);
        for (int i = 0; i < iloscElementowDoWyswietlenia; i++) {
            std::cout << '[' << i << "]:\t";
            wyswietlElement(tablica[i]);
            std::cout << '\n';
        }

        std::cout << "------------------------\n";
    }

    // f) sortowanie babelkowe
    void sortuj(bool (*komparator)(const T &, const T &) = nullptr) {
        if (!komparator) {
            komparator = [](const T &lewy, const T &prawy) {
                return lewy > prawy;
            };
        }

        for (int i = 0; i < iloscElementow; i++) {
            for (int j = 1; j < iloscElementow - i; j++) {
                if (komparator(tablica[j - 1], tablica[j])) {
                    T tmp = std::move(tablica[j - 1]);
                    tablica[j - 1] = std::move(tablica[j]);
                    tablica[j] = std::move(tmp);
                }
            }
        }
    }

    // komparator true jak lewy <= prawy
    void sortujMerge(bool (*komparator)(const T &, const T &)) {
        mergeSort(0, iloscElementow - 1, komparator);
    }

    int pobierzIloscElementow() {
        return iloscElementow;
    }

    void ustawIloscElementow(int nowaIlosc) {
        iloscElementow = nowaIlosc;
    }
};

#endif //LAB06_51631_ALGO2_LAB02_MAIN_H
