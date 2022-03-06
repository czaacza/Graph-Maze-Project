#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void zeruj_macierz(double *macierz, int n)
{
    for (int w = 0; w < n; w++)
    {
        for (int k = 0; k < n; k++)
            macierz[w * n + k] = 0;
    }
}

int wczytaj_krawedzie(int *krawedzie, FILE *in, int *ile_krawedzi)
{
    while (!feof(in))
    {
        for (int j = 0; j < 2; j++)
        {
            if (fscanf(in, "%d", &krawedzie[(*ile_krawedzi) * 2 + j]) != 1)
            {
                return 1;
            }
        }
        (*ile_krawedzi)++;
    }
    return 0;
}

void wczytaj_wagi(double *wagi, int lk)
{
    srand(time(NULL));

    for (int i = 0; i < lk; i++)
    {
        double ulamek = (double)rand() / RAND_MAX;
        double zaokraglony = ((int)(ulamek * 100 + .5) / 100.0);
        wagi[i] = zaokraglony * 10;
    }
}

void wczytaj_macierz(double *macierz, int n, int lk, int *krawedzie, double *wagi)
{
    for (int w = 0; w < n * n; w++)
    {
        for (int k = 0; k < lk; k++)
        {
            if (w == krawedzie[k * 2])
            {
                macierz[w * n + krawedzie[k * 2 + 1]] = wagi[k];
            }
        }
    }
}

void zeruj_wektor(int *odwiedzone, int liczba_krawedzi)
{
    for (int i = 0; i < liczba_krawedzi; i++)
    {
        odwiedzone[i] = 0;
    }
}

void DFS(double *macierz, int start, int n, int v, int odwiedzone[], int koniec, double *suma, int *liczba_drog, int drogi[128][128], int liczba_ruchow)
{
    odwiedzone[v] = 1;
    for (int k = 0; k < n; k++)
    {
        if (odwiedzone[k] == 0 && macierz[v * n + k] > 0)
        {
            //           printf("%d->%d\n", v, k);
            drogi[*liczba_drog][liczba_ruchow] = v;
            suma[*liczba_drog] += macierz[v * n + k];
            //            printf("suma: %g\n", suma[*liczba_drog]);
            liczba_ruchow++;
            if (k == koniec)
            {
                drogi[*liczba_drog][0] = liczba_ruchow;
                drogi[*liczba_drog][liczba_ruchow] = koniec;
                (*liczba_drog)++;
                liczba_ruchow = 1;
                suma = realloc(suma, (*liczba_drog) * sizeof *suma);
                suma[*liczba_drog] = 0;
                DFS(macierz, start, n, start, odwiedzone, koniec, suma, liczba_drog, drogi, liczba_ruchow);
            }

            DFS(macierz, start, n, k, odwiedzone, koniec, suma, liczba_drog, drogi, liczba_ruchow);
            suma[*liczba_drog] -= macierz[v * n + k];
        }
    }
}

void wyswietl_najkrostsza_droge(double *suma, int drogi[128][128], int n)
{
    printf("Liczba drog: %d\n", n);
    double mini = suma[0];
    int numer_najkrotszej_drogi = 0;
    for (int i = 0; i < n; i++)
    {
        if (suma[i] < mini && suma[i] != 0)
        {
            mini = suma[i];
            numer_najkrotszej_drogi = i;
        }
    }
    printf("Najkrotsza droga : %g\n", mini);
    printf("start %d", drogi[numer_najkrotszej_drogi][1]);
    for (int i = 2; i <= drogi[numer_najkrotszej_drogi][0]; i++)
    {
        printf(" -> %d", drogi[numer_najkrotszej_drogi][i]);
    }
    printf(" koniec\n");
}

int main(int argc, char **argv)
{
    FILE *kr = argc > 1 ? fopen(argv[1], "r") : NULL;
    int wielkosc_macierzy = argc > 2 ? atoi(argv[2]) : 3;
    int start = argc > 3 ? atoi(argv[3]) : 1;
    int koniec = argc > 4 ? atoi(argv[4]) : (wielkosc_macierzy * wielkosc_macierzy) - 2;
    if (kr == NULL)
    {
        printf("Nie wykryto pliku. Podaj nazwe pliku w pierwszym argumencie wywolania.\n");
        printf("Prawidlowy format wywolania pliku: ./a.out [nazwaPliku [wierzcholek startowy [wierzcholek koncowy]]]\n");
        return 1;
    }

    int ile_krawedzi = 0;
    int *krawedzie = malloc(256 * sizeof *krawedzie);
    double *wagi = malloc(256 * sizeof *wagi);
    double *macierz = malloc(wielkosc_macierzy * wielkosc_macierzy * wielkosc_macierzy * wielkosc_macierzy * sizeof *macierz);

    zeruj_macierz(macierz, wielkosc_macierzy * wielkosc_macierzy);
    wczytaj_krawedzie(krawedzie, kr, &ile_krawedzi);

    if (wczytaj_krawedzie(krawedzie, kr, &ile_krawedzi))
    {
        printf("%s: Nie udalo sie poprawnie wczytac krawedzi. Sprawdz plik %s.\n", argv[0], argv[1]);
        return 1;
    }
    wczytaj_wagi(wagi, ile_krawedzi);

    wczytaj_macierz(macierz, wielkosc_macierzy * wielkosc_macierzy, ile_krawedzi, krawedzie, wagi);

    int *odwiedzone = malloc(ile_krawedzi * sizeof *odwiedzone);
    zeruj_wektor(odwiedzone, ile_krawedzi);

    double *suma = malloc(sizeof *suma);
    int liczba_drog = 0;
    suma[liczba_drog] = 0;

    int drogi[128][128];
    int liczba_ruchow = 1;

    DFS(macierz, start, wielkosc_macierzy * wielkosc_macierzy, start, odwiedzone, koniec, suma, &liczba_drog, drogi, liczba_ruchow);
    free(odwiedzone);
    free(krawedzie);
    free(wagi);
    free(macierz);

    wyswietl_najkrostsza_droge(suma, drogi, liczba_drog);

    free(suma);

    return 0;
}
