#ifndef LABORATOR8_SERVICE_H
#define LABORATOR8_SERVICE_H

#include "../Domain/domain.h"
#include "../Persistenta/repo.h"
#include "../Persistenta/lista_notificare.h"
#include "../Validator/validator.h"
#include "export.h"
#include <functional>
#include <map>
#include <memory>
#include "undo.h"

using std::string;
using std::vector;
using std::function;
using std::map;
using std::unique_ptr;

class Service : public Observable{
private:
    AbsRepo *repo;
    ValidatorApartament &validator;
    Notificare &notificare;

    vector<unique_ptr<ActiuneUndo>> undo_actions;

public:
    /**
     * constructorul obiectului service
     * @param repo - referinta catre o instanta a clasei de repo
     * @param validator - referinta catre o instanta a clasei de validator
     * @param notificare - referinta catre o instanta a clasei de notificare
     */
    Service(AbsRepo *repo, ValidatorApartament &validator, Notificare &notificare) : repo{repo}, validator{validator},
                                                                                  notificare{notificare} {}

    /**
     * nu permite operatia de assignment pe service
     * @param ot - o alta instanta a clasei de Service unde se intentiona a se da assign la datele serviceului
     */
    void operator=(const Service &ot) = delete;

    /**
     * nu permite copierea de obiecte Service
     * @param altul - obiectul de la care se doresc a fi copiate atributele
     */
    Service(const Service &altul) = delete;

    /**
     * coordoneaza operatia de adaugare a unui apartament in aplicatie
     * @param nr_apartament - int
     * @param nume_proprietar - string
     * @param suprafata - float
     * @param tip_apartament - string
     * @throws ApartamentRepoException("Apartament existent!\n") in cazul in care exista deja un apartament identic in aplicatie
     */
    void
    adauga_apartament(int nr_apartament, const string &nume_proprietar, float suprafata, const string &tip_apartament);

    /**
     * realizeaza operatia de undo sau arunca exceptie daca nu se mai poate face undo
     * @throws ApartamentRepoException("Nu mai exista operatii de undo") in cazul in care nu se mai poate da undo
     */
    void undo();

    /**
     * returneaza un vector ce contine toate apartamentele salvate in aplicatie
     * @return un vector cu toate apartamentele - Vector<Apartament>
     */
    [[nodiscard]] const vector<Apartament> &get_all() const noexcept;

    /**
     * returneaza numarul de apartamente salvate in aplicatie
     * @return numarul de apartamente - int
     */
    [[nodiscard]] int nr_apartamente() const noexcept;

    /**
     * coordoneaza operatia de cautare a unui apartament in aplicatie
     * @param nr_apartament - int
     * @param nume_proprietar - string
     * @param suprafata - float
     * @param tip_apartament - string
     * @return pozitia in vector a apartamentului - incepand de la 0
     */
    [[nodiscard]] int
    cauta_apartament(int nr_apartament, const string &nume_proprietar, float suprafata,
                     const string &tip_apartament) const;

    /**
     * coordoneaza operatia de actualizare a unui apartament, schimband datele apartamentului cu cele noi
     * @param nr_apartament - int
     * @param nume_proprietar - string
     * @param suprafata - float
     * @param tip_apartament - string
     * @param nr_apartament_nou - int
     * @param nume_proprietar_nou - string
     * @param suprafata_noua - float
     * @param tip_apartament_nou - string
     */
    void
    actualizare_apartament(int nr_apartament, const string &nume_proprietar, float suprafata,
                           const string &tip_apartament,
                           int nr_apartament_nou, const string &nume_proprietar_nou, float suprafata_noua,
                           const string &tip_apartament_nou);

    /**
     * coordoneaza operatia de stergere a unui apartament din aplicatie
     * @param nr_apartament - int
     * @param nume_proprietar - string
     * @param suprafata - float
     * @param tip_apartament - string
     */
    void
    sterge_apartament(int nr_apartament, const string &nume_proprietar, float suprafata, const string &tip_apartament);

    /**
     * sorteaza lista de apartamente dupa numele proprietarilor
     * @return o lista sortata crescator de apartamente
     */
    vector<Apartament> sorteaza_dupa_nume_proprietar() noexcept;

    /**
     * sorteaza lista de apartamente dupa suprafata
     * @return o lista sortata crescator de apartamente
     */
    vector<Apartament> sorteaza_dupa_suprafata() noexcept;

    /**
     * sorteaza lista de apartamente dupa tip, si in caz de egalitate, dupa suprafata
     * @return o lista sortata crescator de apartamente
     */
    vector<Apartament> sorteaza_dupa_tip_apartament_si_suprafata() noexcept;

    /**
     * filtreaza lista de apartamente astfel incat apartamentele ramase au tipul dat
     * @param tip_apartament - string - tipul apartamentului pe care trebuie sa il aiba apartamentele
     * @return o lista filtrata de apartamente
     */
    vector<Apartament> filtrare_dupa_tip_apartament(const string &tip_apartament) noexcept;

    /**
     * filtreaza lista de apartamente astfel incat apartamentele ramase au suprafata data
     * @param suprafata - float - suprafata pe care trebuie sa o aiba apartamentele
     * * @return o lista filtrata de apartamente
     */
    vector<Apartament> filtrare_dupa_suprafata(float suprafata) noexcept;

    /**
     * coorodoneaza operatia de golire a listei de notificari
     */
    void golire_lista_notificari();

    /**
     * adauga apartamentul cu numarul primit in lista de notificare
     * @param nr_apartament - int - numarul apartamentului ce trebuie adaugat
     */
    void adauga_apartament_lista_notificare(int nr_apartament);

    /**
     * returneaza un vector ce contine toate apartamentele salvate in lista de notificare
     * @return
     */
    [[nodiscard]] const vector<Apartament> &get_all_notificari() const noexcept;

    /**
     * selecteaza random din lista de apartamente a aplicatiei nr_apartamente si le adauga in lista de notificare
     * @param nr_apartamente - int - cate elemente trebuie sa aleaga random
     */
    void generare_random_apartamente(int nr_apartamente);

    /**
     * realizeaza un raport, creand o mapa in care cheile reprezinta tipul apartamentului si valorile sunt
     * vectori de apartamente care au tipul respectiv
     * @return o mapa cu chei string si valori vector de apartamente
     */
    map<string, vector<Apartament>> raport_elemente_in_fucntie_de_tip();

    /**
     * coordoneaza operatia de export a datelor din lista de notificare in format CSV in fisierul cu numele dat
     * @param file_name - numele / pathul fisierului in care se vor salva datele
     */
    void exporta_lista_notificare_CSV(const string &file_name) const;

    /**
     * coordoneaza operatia de export a datelor din lista de notificare in format HTMl in fisierul cu numele dat
     * @param file_name - numele / pathul fisierului in care se vor salva datele
     */
    void exporta_lista_notificare_HTML(const string &file_name) const;

    /**
     * coordoneaza operatia de golire a listei de apartamente
     */
    void golire_repo();

    /**
     * returneaza numarul de apartamente din lista de notificare
     * @return nr de apartamente - int
     */
    [[nodiscard]] int nr_elemenete_notificare() const noexcept;


};

void teste_service();

#endif //LABORATOR8_SERVICE_H
