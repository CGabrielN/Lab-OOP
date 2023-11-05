#ifndef LABORATOR8_LISTA_NOTIFICARE_H
#define LABORATOR8_LISTA_NOTIFICARE_H

#include "../Domain/domain.h"
#include "../Interfata/observer.h"
#include "repo.h"
#include <utility>
#include <vector>
#include <ostream>

using std::ostream;
using std::vector;

class Notificare {
private:
    vector<Apartament> lista_notificare;
    const AbsRepo *repo;
public:
    /**
     * constructor default al clasei
     */
    Notificare(const AbsRepo *r) : repo{r} {}

    /**
     * previne copierea obiectelor de tip Notificare
     * @param altul - obiectul de la care se doresc a fi copiate atributele
     */
    Notificare(const Notificare &altul) = delete;

    /**
     * sterge toate apartamentele din lista de notificare
     */
    void golire_lista();

    /**
     * adauga un apartament in lista de notificare
     * @param apartament - apartamentul ce trebuie adaugat in lista
     */
    void adauga_apartament(const Apartament &apartament);

    /**
     * returneaza lista de apartamente
     * @return o lista de obiecte de tip apartament - lista de notificare
     */
    [[nodiscard]] const vector<Apartament> &get_all() const noexcept;

    /**
     * returneaza numarul de elmente salvate in lista de notificare
     * @return numarul apartamentelor salvate in lista - int
     */
    [[nodiscard]] int numar_apartamente_lista() const noexcept;

    void umple(int nr_apartamente);
};

class NotificareRepoException : public std::exception {
    string msg;
public:
    explicit NotificareRepoException(string m) : msg{std::move(m)} {}

    [[nodiscard]] const string &get_msg() const {
        return msg;
    }

    friend ostream &operator<<(ostream &out, const NotificareRepoException &ex);
};

ostream &operator<<(ostream &out, const NotificareRepoException &ex);

void teste_lista_notificare();

#endif //LABORATOR8_LISTA_NOTIFICARE_H
