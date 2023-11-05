#ifndef LABORATOR8_REPO_H
#define LABORATOR8_REPO_H

#include "../Domain/domain.h"
#include <ostream>
#include <vector>

using std::vector;
using std::ostream;

class AbsRepo {
public:

    virtual void adauga_apartament(const Apartament &apartament) = 0;

    [[nodiscard]] virtual int cauta_apartament(const Apartament &apartament) const = 0;

    [[nodiscard]] virtual const vector<Apartament> &get_all() const noexcept = 0;

    [[nodiscard]] virtual int nr_apartamente()const noexcept = 0;

    virtual void actualizeaza_apartament(const Apartament &apartament, const Apartament &apartament_nou) = 0;

    virtual void sterge_apartament(const Apartament &apartament) = 0;

    virtual void golire_repo() = 0;

    virtual ~AbsRepo() = default;
};

class Repo : public AbsRepo{
    vector<Apartament> lista_apartamente;

    /**
     * verifica daca apartamentul exista in lista de apartamente sau nu
     * @param apartament - apartamentul cautat
     * @return true daca el exista, false altfel
     */
    bool exista(const Apartament &apartament) const;

public:
    Repo() = default;

    virtual ~Repo() = default;

    /**
     * nu permite copierea de obiecte repo
     * @param r - obiectul de la care se doresc a fi copiate atributele
     */
    Repo(const Repo &r) = delete;

    /**
     * adauga pe ultima pozitie din vector un apartament nou, primit ca si parametru, daca acesta nu exista deja in lista
     * @param apartament - instanta a clasei de Apartament ce trebuie adaugata in lista
     */
    void adauga_apartament(const Apartament &apartament) override;

    /**
     * cauta apartamentul primit ca si parametru si ii returneaza pozitia
     * @param apartament - apartamentul ce este cautat
     * @return pozitia in lista a apartamentului daca acesta exista
     * @throws OfertaRepoException("Apartament inexistent!\n") daca apartamentul cautata nu exista
     */
    [[nodiscard]] int cauta_apartament(const Apartament &apartament) const override;

    /**
     * returneaza vectorul ce contine toate apartamentele din aplicatie
     * @return Vector<Apartament> cu toate apartamentele din aplicatie
     */
    [[nodiscard]] const vector<Apartament> &get_all() const noexcept override;

    /**
     * returneaza numarul de apartamente salvate in aplicatie
     * @return int
     */
    [[nodiscard]] int nr_apartamente() const noexcept override;

    /**
     * modifica datele apartamentului cu cele ale apartamentului nou primite ca si parametru
     * @param apartament - apartament ale carei valori trebuie modificate
     * @param apartament_nou - noile valori pe care apartamentul le va avea
     */
    void actualizeaza_apartament(const Apartament &apartament, const Apartament &apartament_nou) override;

    /**
     * sterge din vectorul de apartamente apartamentul primit ca si parametru
     * @param apartament - o instanta a clasei de Apartament reprezentand apartamentul ce trebuie sters
     */
    void sterge_apartament(const Apartament &apartament) override;

    /**
     * goleste lista de apartamente
     */
     void golire_repo() override{
        lista_apartamente.clear();
    }
};

class ApartamentRepoException {
    string msg;
public:
    ApartamentRepoException(const string &m) : msg{m} {}

    [[nodiscard]] const string& get_msg() const {
        return msg;
    }

    friend ostream &operator<<(ostream &out, const ApartamentRepoException &ex);
};

ostream &operator<<(ostream &out, const ApartamentRepoException &ex);

class RepoFile : public Repo {
private:
    string file_name;

    /**
     * preia continutul fisierului ce are salvata locatia in file_name si il incarca in lista de apartamente
     */
    void load_from_file();

    /**
     * scrie continutul listei de apartamente in fisierul ce are locatia salvata in file_name
     */
    void write_to_file();

public:
    /**
     * Constructorul clasei de RepoFile ce apeleaza constructorul clasei de baza si salveaza numele fisierului cu
     * care va lucra
     * @param f_name - string
     */
    RepoFile(const string& f_name) : Repo(), file_name{f_name} {
        load_from_file(); // incarcam datele din fisier la pornire
    };

    /**
     * adauga in lista de apartamente apartamentul primit si salveaza in fisier datele
     * @param apartament - apartamentul ce trebuie adaugat in lista
     */
    void adauga_apartament(const Apartament &apartament) override {
        Repo::adauga_apartament(apartament);
        write_to_file();
    }

    /**
     * sterge din lista de apartamente apartamentul dat
     * @param apartament - apartamentul ce trebuie sters
     */
    void sterge_apartament(const Apartament &apartament) override {
        Repo::sterge_apartament(apartament);
        write_to_file();
    }

    /**
     * actualizeaza datele primului apartament cu ale celui de al doilea
     * @param apartament - apartamentul are carui date vor fi modificate
     * @param apartament_nou - datele noi pe care le va avea apartamentul
     */
    void actualizeaza_apartament(const Apartament &apartament, const Apartament &apartament_nou) override {
        Repo::actualizeaza_apartament(apartament, apartament_nou);
        write_to_file();
    }

    /**
     * goleste lista de apartamente si fisierul in care sunt retinute acestea
     */
    void golire_repo() override {
        Repo::golire_repo();
        write_to_file();
    }
};

void teste_repo();

#endif //LABORATOR8_REPO_H
