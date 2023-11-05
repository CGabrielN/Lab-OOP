#include "UI.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cin;

/**
 * afiseaza in consola headerul tabelului pentru afisarea apartamentelor
 */
void print_header_tabel() {
    std::cout.width(11);
    std::cout << "Nr. Ap";
    std::cout.width(24);
    std::cout << "Nume Proprietar";
    std::cout.width(15);
    std::cout << "Suprafata";
    std::cout.width(20);
    std::cout << "Tip Apartament";
    std::cout << std::endl;
}

void UI::ui_afisare_apartamente(const string &titlu, const vector<Apartament> &apartamente) {
    cout << '\n' << titlu << "(" << apartamente.size() << "):\n";
    print_header_tabel();
    for (const auto &apartament: apartamente) {
        std::cout.width(10);
        std::cout << apartament.get_nr_apartament();
        std::cout.width(22);
        std::cout << apartament.get_nume_proprietar();
        std::cout.width(15);
        std::cout << apartament.get_suprafata();
        std::cout.width(20);
        std::cout << apartament.get_tip_apartament();
        std::cout << std::endl;
    }
    cout << "-------------------------------------------------------------------------------------------------------\n";
}

void UI::preluare_date(int &nr_apartament, string &nume_proprietar, float &suprafata, string &tip_apartament) {
    cout << "Nr apartament: ";
    cin >> nr_apartament;
    cin.get();
    cout << "Nume proprietar: ";
    getline(cin, nume_proprietar);
    cout << "Suprafata: ";
    cin >> suprafata;
    cin.get();
    cout << "Tip apartament: ";
    getline(cin, tip_apartament);
}

void UI::ui_adauga() {
    string nume_proprietar, tip_apartament;
    float suprafata;
    int nr_apartament;
    preluare_date(nr_apartament, nume_proprietar, suprafata, tip_apartament);
    service.adauga_apartament(nr_apartament, nume_proprietar, suprafata, tip_apartament);
    cout << "Apartament adaugat cu succes!\n";
}

void UI::ui_cauta_apartament() {
    string nume_proprietar, tip_apartament;
    float suprafata;
    int nr_apartament;
    preluare_date(nr_apartament, nume_proprietar, suprafata, tip_apartament);
    int poz = service.cauta_apartament(nr_apartament, nume_proprietar, suprafata, tip_apartament);
    cout << "Apartament exista in aplicatie si este pe pozitia " << poz + 1 << " in lista!\n";
}

void UI::ui_actualizare_apartament() {
    string nume_proprietar, tip_apartament;
    float suprafata;
    int nr_apartament;
    cout << "Introduceti datele apartamentului pe care doriti sa il modificati\n";
    preluare_date(nr_apartament, nume_proprietar, suprafata, tip_apartament);

    cout << "Introduceti datele apartamentului nou\n";
    string nume_proprietar_nou, tip_apartament_nou;
    float suprafata_noua;
    int nr_apartament_nou;
    preluare_date(nr_apartament_nou, nume_proprietar_nou, suprafata_noua, tip_apartament_nou);
    service.actualizare_apartament(nr_apartament, nume_proprietar, suprafata, tip_apartament, nr_apartament_nou,
                                   nume_proprietar_nou, suprafata_noua, tip_apartament_nou);
    cout << "Apartament acualizat cu succes!\n";
}

void UI::ui_sterge_apartament() {
    string nume_proprietar, tip_apartament;
    float suprafata;
    int nr_apartament;
    cout << "Introduceti datele apartamentului pe care doriti sa il stergeti\n";
    preluare_date(nr_apartament, nume_proprietar, suprafata, tip_apartament);
    service.sterge_apartament(nr_apartament, nume_proprietar, suprafata, tip_apartament);
    cout << "Apartament sters cu succes!\n";
}

void UI::ui_filtrare_dupa_tip_apartament() {
    string tip_apartament;
    cout << "Introduceti tipul apartamentului: ";
    getline(cin, tip_apartament);
    ui_afisare_apartamente("Apartamente", service.filtrare_dupa_tip_apartament(tip_apartament));
}

void UI::ui_filtrare_dupa_suprafata() {
    float suprafata;
    cout << "Introduceti suprafata: ";
    cin >> suprafata;
    cin.get();
    ui_afisare_apartamente("Apartamente", service.filtrare_dupa_suprafata(suprafata));
}

void UI::ui_adauga_apartament_lista_notificari() {
    int nr_apartament;
    cout << "Numarul apartamentului: ";
    cin >> nr_apartament;
    cin.get();
    service.adauga_apartament_lista_notificare(nr_apartament);
    cout << "Apartament adaugat cu succes in lista!\n";
}

void UI::ui_generare_lista() {
    int nr_apartamente;
    cout << "Numarul de apartamente de generat: ";
    cin >> nr_apartamente;
    cin.get();
    service.generare_random_apartamente(nr_apartamente);
    cout << "Apartamente generate cu succes!\n";
}

void UI::ui_afisare_raport(const map<string, vector<Apartament>> &mapa_raport) {
    for (const auto &pereche: mapa_raport) {
        cout << pereche.first << ":\n";
        for (const auto &apartament: pereche.second) {
            cout << "\t" << apartament;
        }
        cout << '\n';
    }
}

void UI::meniu_lista_apartamente() {
    int op;
    while (true) {
        cout << "Optiuni:\n1. Adauga\n2. Tipareste\n3. Cauta\n4. Actualizare\n5. Sterge\n"
                "6. Afisare raport dupa fiecare tip de apartament\n0. Back\n";
        cout << "Dati optiunea:";
        cin >> op;
        cin.get();
        switch (op) {
            case 1:
                ui_adauga();
                return;

            case 2:
                ui_afisare_apartamente("Apartamente", service.get_all());
                return;

            case 3:
                ui_cauta_apartament();
                return;

            case 4:
                ui_actualizare_apartament();
                return;

            case 5:
                ui_sterge_apartament();
                return;

            case 6:
                ui_afisare_raport(service.raport_elemente_in_fucntie_de_tip());
                return;

            case 0:
                return;

            default:
                cout << "Comanda invalida!\n";
        }
    }
}

void UI::sortari_filtrari() {
    int op;
    while (true) {
        cout << "Optiuni:\n1. Sorteaza dupa nume proprietar\n2. Sorteaza dupa suprafata\n"
                "3. Sorteaza dupa tip apartament si suprafata\n"
                "4. Filtrare dupa tip apartament\n5. Filtrare dupa suprafata\n0. Back\n";
        cout << "Dati optiunea:";
        cin >> op;
        cin.get();
        switch (op) {
            case 1:
                ui_afisare_apartamente("Apartamente", service.sorteaza_dupa_nume_proprietar());
                return;

            case 2:
                ui_afisare_apartamente("Apartamente", service.sorteaza_dupa_suprafata());
                return;

            case 3:
                ui_afisare_apartamente("Apartamente", service.sorteaza_dupa_tip_apartament_si_suprafata());
                return;

            case 4:
                ui_filtrare_dupa_tip_apartament();
                return;

            case 5:
                ui_filtrare_dupa_suprafata();
                return;

            case 0:
                return;

            default:
                cout << "Comanda invalida!\n";
        }
    }
}

void UI::meniu_lista_notificare() {
    int op;
    while (true) {
        cout << "Optiuni:\n1. Golire lista notificare\n2. Adauga apartament lista notificare\n"
                "3. Afisare lista notificari\n4. Generare lista random\n0. Back\n";
        cout << "Dati optiunea:";
        cin >> op;
        cin.get();
        switch (op) {
            case 1:
                service.golire_lista_notificari();
                cout << "Lista golita cu succes!\n";
                cout << "Elemente in lista de notificare: " << service.nr_elemenete_notificare() << "\n\n";
                return;

            case 2:
                ui_adauga_apartament_lista_notificari();
                cout << "Elemente in lista de notificare: " << service.nr_elemenete_notificare() << "\n\n";
                return;

            case 3:
                ui_afisare_apartamente("Lista notificare", service.get_all_notificari());
                cout << "Elemente in lista de notificare: " << service.nr_elemenete_notificare() << "\n\n";
                return;

            case 4:
                ui_generare_lista();
                cout << "Elemente in lista de notificare: " << service.nr_elemenete_notificare() << "\n\n";
                return;

            case 0:
                return;

            default:
                cout << "Comanda invalida!\n";
        }
    }
}

void UI::start() {
    int op;
    string file_name;
    while (true) {
        cout << "Meniu:\n";
        cout << "1. Optiuni pe lista de apartamente\n2. Sortari/Filtrari pe lista de apartamente\n"
                "3. Optiuni pe lista de notificare\n4. Export to CSV\n5. Export to HTML\n6. Undo\n0. Iesire\n"
                "Dati comanda:";
        cin >> op;
        cin.get();
        try {
            switch (op) {
                case 1:
                    meniu_lista_apartamente();
                    break;

                case 2:
                    sortari_filtrari();
                    break;

                case 3:
                    meniu_lista_notificare();
                    break;

                case 4:
                    cout << "Nume fisier fara extensie:";
                    getline(cin, file_name);
                    file_name += ".txt";
                    service.exporta_lista_notificare_CSV(file_name);
                    cout << "Elemente in lista de notificare: " << service.nr_elemenete_notificare() << "\n\n";
                    break;

                case 5:
                    cout << "Nume fisier fara extensie:";
                    getline(cin, file_name);
                    file_name += ".html";
                    service.exporta_lista_notificare_HTML(file_name);
                    cout << "Elemente in lista de notificare: " << service.nr_elemenete_notificare() << "\n\n";
                    break;

                case 6:
                    service.undo();
                    ui_afisare_apartamente("Apartamente", service.get_all());
                    break;

                case 0:
                    return;

                default:
                    cout << "Comanda invalida!\n";
            }
        }
        catch (const ApartamentRepoException &ex) {
            cout << ex << '\n';
        }
        catch (const ValidateException &ex) {
            cout << ex << '\n';
        }
        catch (const NotificareRepoException &ex) {
            cout << ex << '\n';
        }
        catch (const ApartamentException &ex) {
            cout << ex.get_msg() << '\n';
        }
    }
}


