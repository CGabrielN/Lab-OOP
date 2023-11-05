#include "service.h"
#include <cassert>
#include <random>

void Service::adauga_apartament(int nr_apartament, const std::string &nume_proprietar, float suprafata,
                                const std::string &tip_apartament) {
    Apartament apartament{nr_apartament, nume_proprietar, suprafata, tip_apartament};
    validator.validate(apartament);
    repo->adauga_apartament(apartament);
    undo_actions.push_back(std::make_unique<UndoAdauga>(repo, apartament));
}

const vector<Apartament> &Service::get_all() const noexcept {
    return repo->get_all();
}

int Service::nr_apartamente() const noexcept {
    return repo->nr_apartamente();
}

int Service::cauta_apartament(int nr_apartament, const std::string &nume_proprietar, float suprafata,
                              const std::string &tip_apartament) const {
    Apartament apartament{nr_apartament, nume_proprietar, suprafata, tip_apartament};
    validator.validate(apartament);
    return repo->cauta_apartament(apartament);
}

void Service::actualizare_apartament(int nr_apartament, const std::string &nume_proprietar, float suprafata,
                                     const std::string &tip_apartament, int nr_apartament_nou,
                                     const std::string &nume_proprietar_nou, float suprafata_noua,
                                     const std::string &tip_apartament_nou) {
    Apartament apartament{nr_apartament, nume_proprietar, suprafata, tip_apartament};
    validator.validate(apartament);
    Apartament apartament_nou{nr_apartament_nou, nume_proprietar_nou, suprafata_noua, tip_apartament_nou};
    validator.validate(apartament_nou);
    repo->actualizeaza_apartament(apartament, apartament_nou);
    undo_actions.push_back(std::make_unique<UndoModifica>(repo, apartament, apartament_nou));
}

void
Service::sterge_apartament(int nr_apartament, const std::string &nume_proprietar, float suprafata,
                           const std::string &tip_apartament) {
    Apartament apartament{nr_apartament, nume_proprietar, suprafata, tip_apartament};
    validator.validate(apartament);
    repo->sterge_apartament(apartament);
    undo_actions.push_back(std::make_unique<UndoSterge>(repo, apartament));
}

vector<Apartament> Service::sorteaza_dupa_nume_proprietar() noexcept {
    vector<Apartament> lista_apartamente = repo->get_all();
    std::sort(lista_apartamente.begin(), lista_apartamente.end(), compara_nume_proprietar);
    return lista_apartamente;
}

vector<Apartament> Service::sorteaza_dupa_suprafata() noexcept {
    vector<Apartament> lista_apartamente = repo->get_all();
    std::sort(lista_apartamente.begin(), lista_apartamente.end(), compara_suprafata);
    return lista_apartamente;
}

vector<Apartament> Service::sorteaza_dupa_tip_apartament_si_suprafata() noexcept {
    vector<Apartament> lista_apartamente = repo->get_all();
    std::sort(lista_apartamente.begin(), lista_apartamente.end(), [](const Apartament &a1, const Apartament &a2) {
        if (a1.get_tip_apartament() == a2.get_tip_apartament())
            return a1.get_suprafata() < a2.get_suprafata();
        return a1.get_tip_apartament() < a2.get_tip_apartament();
    });
    return lista_apartamente;
}

vector<Apartament> Service::filtrare_dupa_tip_apartament(const std::string &tip_apartament) noexcept {
    vector<Apartament> lista_apartamente = repo->get_all();
    vector<Apartament> lista_filtrara(lista_apartamente.size());
    auto it = std::copy_if(lista_apartamente.begin(), lista_apartamente.end(), lista_filtrara.begin(),
                           [tip_apartament](const Apartament &apartament) {
                               return apartament.get_tip_apartament().find(tip_apartament) != -1;
                           });
    lista_filtrara.resize(std::distance(lista_filtrara.begin(), it));
    return lista_filtrara;
}

vector<Apartament> Service::filtrare_dupa_suprafata(float suprafata) noexcept {
    vector<Apartament> lista_apartamente = repo->get_all();
    vector<Apartament> lista_filtrara(lista_apartamente.size());
    auto it = std::copy_if(lista_apartamente.begin(), lista_apartamente.end(), lista_filtrara.begin(),
                           [suprafata](const Apartament &apartament) {
                               return std::to_string(apartament.get_suprafata()).find(std::to_string(suprafata)) != string::npos;
                           });
    lista_filtrara.resize(std::distance(lista_filtrara.begin(), it));
    return lista_filtrara;
}

void Service::golire_lista_notificari() {
    notificare.golire_lista();
    notify();
}

void Service::adauga_apartament_lista_notificare(int nr_apartament) {
    vector<Apartament> lista_apartamente = repo->get_all();
    auto it = std::find_if(lista_apartamente.begin(), lista_apartamente.end(),
                           [nr_apartament](const Apartament &a) { return a.get_nr_apartament() == nr_apartament; });
    if (it != lista_apartamente.end()) {
        notificare.adauga_apartament(lista_apartamente[it - lista_apartamente.begin()]);
        notify();
        return;
    }
    throw ApartamentRepoException("Nu exista apartament inregistrat cu acest numar!\n");
}

const vector<Apartament> &Service::get_all_notificari() const noexcept {
    return notificare.get_all();
}

void Service::generare_random_apartamente(int nr_apartamente) {
    notificare.umple(nr_apartamente);
    notify();
}

map<string, vector<Apartament>> Service::raport_elemente_in_fucntie_de_tip() {
    vector<Apartament> lista_apartamente = repo->get_all();
    map<string, vector<Apartament>> mapa_raport;
    for (const auto &apartament: lista_apartamente) {
        auto it = mapa_raport.find(apartament.get_tip_apartament());
        if (it != mapa_raport.end()) {
            mapa_raport.at(apartament.get_tip_apartament()).push_back(apartament);
        } else {
            mapa_raport.emplace(apartament.get_tip_apartament(), vector<Apartament>{apartament});
        }
    }
    return mapa_raport;
}

void Service::exporta_lista_notificare_CSV(const string &file_name) const {
    export_to_CSV(file_name, notificare.get_all());
}

void Service::exporta_lista_notificare_HTML(const string &file_name) const {
    export_to_HTML(file_name, notificare.get_all());
}

void Service::golire_repo() {
    repo->golire_repo();
}

int Service::nr_elemenete_notificare() const noexcept {
    return notificare.numar_apartamente_lista();
}

void Service::undo() {
    if (undo_actions.empty()) {
        throw ApartamentRepoException("Nu mai exista operatii de undo");
    }
    //obs: la obiecte unique_ptr nu putem face copie
    undo_actions.back()->doUndo();
    undo_actions.pop_back();
}


void teste_service_functii_normale() {
    ValidatorApartament validatorApartament;
    AbsRepo* repo = new RepoFile{"Teste/teste.txt"};
    Notificare notificare{repo};
    Service service{repo, validatorApartament, notificare};
    // Teste nr_oferte si get_all
    assert(service.nr_apartamente() == 0);
    service.adauga_apartament(12, "Bob John", 60, "2 cam");
    service.adauga_apartament(13, "Kyle", 80, "3 cam");
    service.adauga_apartament_lista_notificare(12);
    try {
        service.adauga_apartament_lista_notificare(2);
    }
    catch (const ApartamentRepoException &) {
        assert(true);
    }

    service.adauga_apartament_lista_notificare(13);
    vector<Apartament> lista_notificare = service.get_all_notificari();
    assert(lista_notificare[0].get_nr_apartament() == 12);
    assert(lista_notificare[1].get_nr_apartament() == 13);
    assert(service.nr_elemenete_notificare() == 2);
    try {
        service.adauga_apartament(13, "Kyle", 80, "3 cam");
    }
    catch (const ApartamentRepoException &ex) {
        assert(true);
    }
    assert(service.nr_apartamente() == 2);
    const vector<Apartament> &lista_apartamente = service.get_all();
    assert(lista_apartamente[0].get_nume_proprietar() == "Bob John");
    assert(lista_apartamente[1].get_nume_proprietar() == "Kyle");

    // Teste cauta
    assert(service.cauta_apartament(12, "Bob John", 60, "2 cam") == 0);
    try {
        auto poz = service.cauta_apartament(15, "Sabin", 25, "1 cam");
    }
    catch (const ApartamentRepoException &) {
        assert(true);
    }

    // Teste actualizeaza
    service.actualizare_apartament(12, "Bob John", 60, "2 cam",
                                   15, "Sabin", 25, "1 cam");
    try {
        service.actualizare_apartament(12, "Bob John", 60, "2 cam",
                                       15, "Sabin", 25, "1 cam");
    }
    catch (const ApartamentRepoException &) {
        assert(true);
    }

    // Teste stergere
    service.generare_random_apartamente(4);
    service.golire_lista_notificari();
    service.sterge_apartament(15, "Sabin", 25, "1 cam");
    assert(service.nr_apartamente() == 1);
    service.generare_random_apartamente(1);
    const vector<Apartament> &lista_notificari_2 = service.get_all_notificari();
    assert(lista_notificari_2[0].get_nume_proprietar() == "Kyle");
    const vector<Apartament> &lista_apartamente2 = service.get_all();
    assert(lista_apartamente2[0].get_nume_proprietar() == "Kyle");
    service.sterge_apartament(13, "Kyle", 80, "3 cam");
    assert(service.nr_apartamente() == 0);
    try {
        service.sterge_apartament(13, "Kyle", 80, "3 cam");
    }
    catch (const ApartamentRepoException &ex) {
        cout << ex;
        assert(true);
    }

    // Teste sortari
    service.adauga_apartament(12, "Paul", 60, "2 cam");
    service.adauga_apartament(13, "Anton", 56, "2 cam");
    service.adauga_apartament(22, "Dylan", 80, "3 cam");
    service.adauga_apartament(32, "Cristian", 45, "1 cam");
    vector<Apartament> lista_sortate = service.sorteaza_dupa_nume_proprietar();

    service.generare_random_apartamente(4);
    assert(lista_sortate[0].get_nume_proprietar() == "Anton");
    assert(lista_sortate[1].get_nume_proprietar() == "Cristian");
    assert(lista_sortate[2].get_nume_proprietar() == "Dylan");
    assert(lista_sortate[3].get_nume_proprietar() == "Paul");

    lista_sortate = service.sorteaza_dupa_suprafata();
    assert(lista_sortate[0].get_nume_proprietar() == "Cristian");
    assert(lista_sortate[1].get_nume_proprietar() == "Anton");
    assert(lista_sortate[2].get_nume_proprietar() == "Paul");
    assert(lista_sortate[3].get_nume_proprietar() == "Dylan");

    lista_sortate = service.sorteaza_dupa_tip_apartament_si_suprafata();
    assert(lista_sortate[0].get_tip_apartament() == "1 cam");
    assert(lista_sortate[1].get_suprafata() - 56 < 0.00001);
    assert(lista_sortate[2].get_suprafata() - 60 < 0.00001);
    assert(lista_sortate[3].get_tip_apartament() == "3 cam");

    // Teste filtrari
    vector<Apartament> lista_filtate = service.filtrare_dupa_tip_apartament("2 cam");
    assert(lista_filtate.size() == 2);
    assert(lista_filtate[0].get_nume_proprietar() == "Paul");
    assert(lista_filtate[1].get_nume_proprietar() == "Anton");

    service.adauga_apartament(2, "Garret", 60, "2 cam");
    lista_filtate = service.filtrare_dupa_suprafata(60);
    assert(lista_filtate.size() == 2);
    assert(lista_filtate[0].get_nume_proprietar() == "Paul");
    assert(lista_filtate[1].get_nume_proprietar() == "Garret");

    service.golire_repo();

    // teste raport
    service.adauga_apartament(12, "Paul", 60, "2 cam");
    service.adauga_apartament(13, "Anton", 56, "2 cam");
    service.adauga_apartament(22, "Dylan", 80, "3 cam");
    service.adauga_apartament(32, "Cristian", 45, "1 cam");
    auto mapa_raport = service.raport_elemente_in_fucntie_de_tip();
    assert(mapa_raport.find("2 cam") != mapa_raport.end());
    assert(mapa_raport["2 cam"].size() == 2);
    assert(mapa_raport.find("3 cam") != mapa_raport.end());
    assert(mapa_raport["3 cam"].size() == 1);
    assert(mapa_raport.find("2 cam") != mapa_raport.end());
    assert(mapa_raport["1 cam"].size() == 1);

    service.golire_repo();
    delete repo;
}

#include <fstream>

using std::ifstream;

void teste_export_si_undo() {
    ValidatorApartament validatorApartament;
    AbsRepo* repo = new RepoFile{"Teste/teste.txt"};
    Notificare notificare{repo};
    Service service{repo, validatorApartament, notificare};

    // teste Undo
    try {
        service.undo();
    }
    catch (const ApartamentRepoException &) {
        assert(true);
    }
    assert(service.nr_apartamente() == 0);
    service.adauga_apartament(12, "Bob John", 60, "2 cam");
    service.adauga_apartament(13, "Kyle", 80, "3 cam");
    assert(service.nr_apartamente() == 2);
    service.undo();
    assert(service.get_all()[0].get_nr_apartament() == 12);
    service.undo();
    assert(service.nr_apartamente() == 0);

    service.adauga_apartament(12, "Bob John", 60, "2 cam");
    service.sterge_apartament(12, "Bob John", 60, "2 cam");
    service.undo();
    assert(service.get_all()[0].get_nr_apartament() == 12);
    service.actualizare_apartament(12, "Bob John", 60, "2 cam", 15, "Bob John", 60, "2 cam");
    assert(service.get_all()[0].get_nr_apartament() == 15);
    service.undo();
    assert(service.get_all()[0].get_nr_apartament() == 12);
    service.golire_repo();

    // teste export
    service.adauga_apartament(12, "Paul", 60, "2 cam");
    service.adauga_apartament(13, "Anton", 56, "2 cam");
    service.adauga_apartament(22, "Dylan", 80, "3 cam");
    service.adauga_apartament(32, "Cristian", 45, "1 cam");
    service.generare_random_apartamente(4);
    assert(service.nr_elemenete_notificare() == 4);

    // export CSV
    service.exporta_lista_notificare_CSV("Teste/csv_file_test.txt");
    ifstream in("../Export/Teste/csv_file_test.txt");
    assert(in.is_open());
    int countLines = 0;
    while (!in.eof()) {
        string line;
        getline(in, line);
        countLines++;
    }
    in.close();
    assert(countLines == 5);

    // export HTML
    service.exporta_lista_notificare_HTML("Teste/html_file_test.html");
    in.open("../Export/Teste/html_file_test.html");
    assert(in.is_open());

    try {
        service.exporta_lista_notificare_CSV("/a/tdpts");
    }
    catch (const ApartamentException &ex) {
        assert(ex.get_msg() == "Nu se poate deschide fisierul de export: /a/tdpts");
    }


    try {
        service.exporta_lista_notificare_HTML("/a/tddsasadpts");
    }
    catch (const ApartamentException &) {
        assert(true);
    }

    service.golire_repo();
    delete repo;
}


void teste_service() {
    teste_service_functii_normale();
    teste_export_si_undo();
}