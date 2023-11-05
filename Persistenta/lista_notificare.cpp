#include "lista_notificare.h"
#include <algorithm>
#include <cassert>
#include <random>
#include <chrono>

void Notificare::golire_lista() {
    lista_notificare.clear();
    // notificam cand se schimba ceva in lista
//    notify();
}

void Notificare::adauga_apartament(const Apartament &apartament) {
    if (std::find_if(lista_notificare.begin(), lista_notificare.end(),
                     [apartament](const Apartament &a) { return a == apartament; }) != lista_notificare.end())
        throw NotificareRepoException("Apartament existent deja in lista de notificare!\n");
    lista_notificare.push_back(apartament);
//    notify();
}


const vector<Apartament> &Notificare::get_all() const noexcept {
    return lista_notificare;
}


int Notificare::numar_apartamente_lista() const noexcept {
    return (int) lista_notificare.size();
}

void Notificare::umple(int nr_apartamente) {
    int seed = (int)std::chrono::system_clock::now().time_since_epoch().count();
    vector<Apartament> all = repo->get_all();
    /* o metoda de generare element random
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<> dist(0, (int) lista_apartamente.size() - 1);
    int rndNr = dist(mt);
    */
    std::shuffle(all.begin(), all.end(), std::default_random_engine(seed));
    while(lista_notificare.size() < nr_apartamente && !all.empty()){
        lista_notificare.push_back(all.back());
        all.pop_back();
    }
//    notify();
}

ostream &operator<<(ostream &out, const NotificareRepoException &ex) {
    out << ex.msg;
    return out;
}

void teste_lista_notificare() {
    AbsRepo* repo = new RepoFile{"Teste/teste.txt"};
    Notificare notificare{repo};
    Apartament ap{12, "Paul", 70, "2 cam"};
    Apartament ap1{13, "Kyle", 65, "2 cam"};
    notificare.adauga_apartament(ap);
    notificare.adauga_apartament(ap1);
    assert(notificare.numar_apartamente_lista() == 2);
    try {
        notificare.adauga_apartament(ap1);
    }
    catch (const NotificareRepoException &ex) {
        cout << ex << '\n';
    }
    vector<Apartament> lista_apartamente = notificare.get_all();
    assert(lista_apartamente[0] == ap);
    assert(lista_apartamente[1] == ap1);
    notificare.golire_lista();
    assert(notificare.numar_apartamente_lista() == 0);

    delete repo;
}