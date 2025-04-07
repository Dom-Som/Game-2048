#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>

using namespace std;

int lenta[4][4];
int KrypEil[] = {1, 0, -1, 0};
int KrypStul[] = {0, 1, 0, -1};
int rez = 0;
int eil, stul;
bool Pabaiga = false, Pergale = false;

void Arlaimejo() {//tikrina ar buvo pasiektas langelis '2048'
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (lenta[i][j] == 2048)
                Pergale = true;
}

void tusciaPoz(int &eil, int &stul) {//tikrina ar yra tusciu langeliu lentoje, kad butu galima prideti nauja skaiciu
    int uzimta = 1;
    eil = -1;
    stul = -1;
    while (uzimta) {
        stul = rand() % 4;
        eil = rand() % 4;
        if (lenta[eil][stul] == 0) {
            uzimta = 0;   
        }
    }
}

void PridetiSk() {//kai ejimas padaromas prideda nauja skaiciu lentoje
    int poz1, poz2;
    tusciaPoz(eil, stul);
    poz1 = eil;
    poz2 = stul;
    lenta[poz1][poz2] = 2;
}

void NaujasZaidimas() {//sukuria nauja zaidimo lenta
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            lenta[i][j] = 0;
    PridetiSk();
    rez = 0;
}

void printMeniu() {//spaudina vaizda ekrane
    system("cls");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            if (lenta[i][j] == 0)
                cout << setw(4) << ".";
            else 
                cout << setw(4) << lenta[i][j];
        cout << endl;
    }
    cout << "n: Pradeti zaidima is naujo, b: ijungti automatini algoritma, kuris daro atsitiktinius ejimus, w: virsun, d: desinen, s: apacion, a: kairen, q: baigti zaidima" << endl;
    cout << "Taskai: " << rez << endl;

    if (Pergale == true) {
        cout << "Sveikiname iveikus zaidima !!!" << endl;
        cout << "Jus sujugete langeli 2048, jei norite galite testi zaidima ir bandyti pagerinti savo rezultata." << endl;
    }

    cout << endl;

}

bool EjimasGalimas(int i, int j, int nextEil, int nextStul) {//tikrinama ar ejimas galimas
    if (nextEil < 0 || nextStul < 0 || nextEil >= 4 || nextStul >= 4 || lenta[i][j] != lenta[nextEil][nextStul] && lenta[nextEil][nextStul] != 0 ) {
        return false;
    }
    return true;
}

void paeik(int kryptis) {//ejimas ivygdomas
    int pradEil = 0, pradStul = 0, zingsnisEil = 1, zingsnisStul = 1, kiek = 0;
    if (kryptis == 0) {
        pradEil = 3;
        zingsnisEil = -1;
    }
    if (kryptis == 1) {
        pradStul = 3;
        zingsnisStul = -1;
    }
    int imanomas, Galimaprideti = 0;
    
    do {
        imanomas = 0;
        for (int i = pradEil; i >= 0 && i < 4; i += zingsnisEil)
        {           
            for (int j = pradStul; j >= 0 && j < 4; j += zingsnisStul) {
                int nextI = i + KrypEil[kryptis], nextJ = j + KrypStul[kryptis], nextnextI = i + KrypEil[kryptis] + KrypEil[kryptis], nextnextJ = j + KrypStul[kryptis] + KrypStul[kryptis];
                int lastI = i - KrypEil[kryptis], lastJ = j - KrypEil[kryptis];
                if (lenta[i][j] != 0 && (EjimasGalimas(i, j, i + KrypEil[0], j + KrypStul[0]) || EjimasGalimas(i, j, i + KrypEil[1], j + KrypStul[1]) || EjimasGalimas(i, j, i + KrypEil[2], j + KrypStul[2]) || EjimasGalimas(i, j, i + KrypEil[3], j + KrypStul[3])))
                    kiek++;  
                    if (lenta[i][j] != 0 && EjimasGalimas(i, j, nextI, nextJ)) {
                        
                        if (lenta[nextI][nextJ] == lenta[i][j])
                            rez += lenta[nextI][nextJ] + lenta[i][j];
                        lenta[nextI][nextJ] += lenta[i][j];
                        if (lenta[nextI][nextJ] + lenta[i][j] != lenta[nextI][nextJ]) {
                            lenta[i][j] = 0;
                        }
                        imanomas = Galimaprideti = 1;
                    }                
            }
        }
        
    }while (imanomas);
    printMeniu();
    if (kiek == 0) //tikrina kiek yra imanomu ejimu, jei ju nera zaidimas pasibaigia
        Pabaiga = true;
    if (Galimaprideti)
        PridetiSk();
}

void botas() { //funkcija, kuri automatiskai zaidzia zaidima
    while (Pabaiga == false) {
        //int kryptis = rand() % 4;
        int kryptis;
        for (int i = 0; i < 20; i++) {
            if (i % 2 == 0) kryptis = 1;
            else kryptis = 0;
            paeik(kryptis);
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        kryptis = 2;
        paeik(kryptis);
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

int main()
{
    srand(time(0));
    char veiksmas[128];
    veiksmas['s'] = 0;
    veiksmas['d'] = 1;
    veiksmas['w'] = 2;
    veiksmas['a'] = 3;
    NaujasZaidimas();

    

    while (true) {//tikrina koks mygtukas buvo ivestas
        Arlaimejo();
        printMeniu();
        char komanda;
            cin >> komanda;
        if (komanda != 'n' && komanda != 'q' && komanda != 's' && komanda != 'w' && komanda != 'd' && komanda != 'a' && komanda != 'b') {
            cout << "Jus ivedete netinkama simboli, zaidimo naudojima simboliai yra nurodyti " << endl;
            break;
        }
        else if (komanda == 'n')
            NaujasZaidimas();
        else if (komanda == 'q') {
            cout << "Zaidimas Baigtas !" << endl;
            break;
        }
        else if (komanda == 'b') {
            botas();
        }
        else if (Pabaiga == true) {
            cout << "Jus pralaimejote, nera daugiau galimu ejimu " << endl;
            break;
        }   
        else {
            int kryptis = veiksmas[komanda];
            paeik(kryptis);
        }
    }
    return 0;
}
