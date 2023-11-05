#include "repo.h"
#include <cassert>
#include <algorithm>
#include <fstream>

using std::ifstream;
using std::ofstream;

bool Repo::exista(const Apartament &apartament) const {
    try {
        cauta_apartament(apartament);
        return true;
    } catch (const ApartamentRepoException &) {
        return false;
    }
}

void Repo::adauga_apartament(const Apartament &apartament) {
    if (exista(apartament)) {
        throw ApartamentRepoException{"Apartament existent!\n"};
    }
    lista_apartamente.push_back(apartament);
}

const vector<Apartament> &Repo::get_all() const noexcept {
    return lista_apartamente;
}

int Repo::nr_apartamente() const noexcept {
    return (int) lista_apartamente.size();
}

int Repo::cauta_apartament(const Apartament &apartament) const {
    auto rez = std::find_if(lista_apartamente.begin(), lista_apartamente.end(),
                            [apartament](const Apartament &a) { return a == apartament; });
    if (rez != lista_apartamente.end()) {
        return (int) (rez - lista_apartamente.begin());
    }
    throw ApartamentRepoException{"Apartament inexistent!\n"};
}

void Repo::actualizeaza_apartament(const Apartament &apartament, const Apartament &apartament_nou) {
    int poz = cauta_apartament(apartament);
    lista_apartamente.at(poz) = apartament_nou;
}

void Repo::sterge_apartament(const Apartament &apartament) {
    int poz = cauta_apartament(apartament);
    lista_apartamente.erase(lista_apartamente.begin() + poz, lista_apartamente.begin() + poz + 1);
    // sau alt mod de a folosi erase
    // auto rez = std::find_if(lista_apartamente.begin(), lista_apartamente.end(), [apartament](const Apartament& a){return a == apartament;});
    // lista_apartamente.erase(rez);
}

ostream &operator<<(ostream &out, const ApartamentRepoException &ex) {
    out << ex.msg;
    return out;
}


void RepoFile::load_from_file() {
    //golire_repo();
    ifstream in(("../Fisiere/" + file_name));
    if (!in.is_open())
        throw ApartamentRepoException("Nu s-a putut deschide fisierul" + file_name);
    while (!in.eof()) {
        string nume_proprietar, tip_apartament;
        int nr_apartament;
        float suprafata;
        in >> nr_apartament;
        in.get();
        if (in.eof())
            break;
        getline(in, nume_proprietar);
        in >> suprafata;
        in.get();
        if(in.eof())
            break;
        getline(in, tip_apartament);
        Apartament apartament{nr_apartament, nume_proprietar, suprafata, tip_apartament};
        Repo::adauga_apartament(apartament);
    }
    in.close();

}

void RepoFile::write_to_file() {
    ofstream out("../Fisiere/" + file_name);
    //if(!out.is_open())
    //    throw ApartamentRepoException("Nu s-a putut deschide fisierul" + file_name);

    for (auto &apartament: get_all()) {
        out << apartament.get_nr_apartament() << '\n';
        out << apartament.get_nume_proprietar() << '\n';
        out << apartament.get_suprafata() << '\n';
        out << apartament.get_tip_apartament() << '\n';
    }

    out.close();
}

void teste_RepoFile() {
    ofstream out("../Teste/teste.txt", std::ios::trunc);
    out.close();
    AbsRepo* repoFile = new RepoFile{"Teste/teste.txt"};
    repoFile->adauga_apartament(Apartament{12, "John", 45, "2 cam"});

    AbsRepo* repoFile2 = new RepoFile{"Teste/teste.txt"};
    int poz_apartament = repoFile2->cauta_apartament(Apartament{12, "John", 45, "2 cam"});
    assert(poz_apartament == 0);
    try {
        repoFile2->cauta_apartament(Apartament{12, "John", 45, "cam"});
    }
    catch (ApartamentRepoException &) {
        assert(true);
    }
    repoFile2->actualizeaza_apartament(Apartament{12, "John", 45, "2 cam"}, Apartament{15, "John", 45, "2 cam"});
    repoFile2->sterge_apartament(Apartament{15, "John", 45, "2 cam"});
    assert(repoFile2->get_all().empty());
    try {
        repoFile2->sterge_apartament(Apartament{15, "John", 45, "cam"});
    }
    catch (ApartamentRepoException &) {
        assert(true);
    }

    AbsRepo* repoFile3 = new RepoFile{"Teste/teste.txt"};
    assert(repoFile3->get_all().empty());

    //fisierul nu exista si nu se poate crea (nu se creaza si directoare)
    //ar trebui sa arunce exceptie
    try {
        RepoFile repoFile4{"Teste/tte/ste.txt"};}
    catch (ApartamentRepoException &) {
        assert(true);
    }
    repoFile3->golire_repo();

    delete repoFile;
    delete repoFile2;
    delete repoFile3;
}

void teste_repo_normal() {
    AbsRepo* repo = new Repo();
    // Teste nr_oferte si get_all
    Apartament apartament1{12, "Bob John", 60, "2 cam"};
    Apartament apartament2{13, "Kyle", 80, "3 cam"};
    assert(repo->nr_apartamente() == 0);
    repo->adauga_apartament(apartament1);
    repo->adauga_apartament(apartament2);
    try {
        repo->adauga_apartament(apartament1);
    }
    catch (const ApartamentRepoException &) {
        assert(true);
    }
    assert(repo->nr_apartamente() == 2);
    const vector<Apartament> &lista_apartamente = repo->get_all();
    assert(lista_apartamente[0] == apartament1);
    assert(lista_apartamente[1] == apartament2);

    // Teste cauta
    assert(repo->cauta_apartament(apartament1) == 0);
    Apartament apartament3{5, "Paul", 45, "2 cam"};
    try {
        repo->cauta_apartament(apartament3);
    }
    catch (const ApartamentRepoException &) {
        assert(true);
    }

    // Teste actualizeaza
    repo->actualizeaza_apartament(apartament1, apartament3);
    try {
        repo->actualizeaza_apartament(apartament1, apartament3);
    }
    catch (const ApartamentRepoException &) {
        assert(true);
    }

    // Teste stergere
    repo->sterge_apartament(apartament3);
    assert(repo->nr_apartamente() == 1);
    const vector<Apartament> &lista_apartamente2 = repo->get_all();
    assert(lista_apartamente2[0] == apartament2);
    repo->sterge_apartament(apartament2);
    assert(repo->nr_apartamente() == 0);
    try {
        repo->sterge_apartament(apartament1);
    }
    catch (const ApartamentRepoException &ex) {
        cout << ex;
        const auto& msg = ex.get_msg();
        assert(msg == "Apartament inexistent!\n");
        assert(true);
    }
    delete repo;
}

void teste_repo() {
    teste_repo_normal();
    teste_RepoFile();
}
