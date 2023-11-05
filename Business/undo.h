#ifndef LABORATOR9_UNDO_H
#define LABORATOR9_UNDO_H

#include "../Domain/domain.h"
#include "../Persistenta/repo.h"

class ActiuneUndo {
public:
    /**
     * functia de baza de undo, care va fi modificata de fiecare subclasa
     */
    virtual void doUndo() = 0;

    /**
     * destructorul e virtual pentru a ne asigura ca daca dau delete se apeleaza destructorul
     * din clasa care trebuie
     */
    virtual ~ActiuneUndo() = default;
};

class UndoAdauga : public ActiuneUndo {
    Apartament apartament_adaugat;
    AbsRepo *repo;
public:
    /**
     * constructorul clasei de UndoAdauga
     * @param rep - repoul pe care se va realiza undoul
     * @param apartament - apartamentul care a fost adaugat in repo
     */
    UndoAdauga(AbsRepo *rep, const Apartament &apartament) : repo{rep}, apartament_adaugat{apartament} {};

    /**
     * face undo la operatia de adaugare a apartamentului
     */
    void doUndo() override {
        repo->sterge_apartament(apartament_adaugat);
    }
};

class UndoSterge : public ActiuneUndo {
    Apartament apartament_sters;
    AbsRepo *repo;
public:
    /**
     * constructorul clasei de UndoSterge
     * @param rep - repoul pe care se va realiza undoul
     * @param apartament - apartamentul care a fost sters
     */
    UndoSterge(AbsRepo *rep, const Apartament &apartament) : repo{rep}, apartament_sters{apartament} {};

    /**
     * face undo la operatia de stergere a apartamentului
     */
    void doUndo() override {
        repo->adauga_apartament(apartament_sters);
    }
};

class UndoModifica : public ActiuneUndo {
    Apartament apartament_vechi;
    Apartament apartament_nou;
    AbsRepo *repo;
public:
    /**
     * constructorul clasei de UndoModificare
     * @param rep - repoul pe care se va face undo
     * @param apartament_v - apartamentul vechi care a fost modificat la actualizare
     * @param apartament_n - datele apartamentului nou care au fost puse la apartamentul vechi
     */
    UndoModifica(AbsRepo *rep, const Apartament &apartament_v, const Apartament &apartament_n) :
            repo{rep}, apartament_vechi{apartament_v}, apartament_nou{apartament_n} {};

    /**
     * face undo la operatia de modificare a unui apartament
     */
    void doUndo() override {
        repo->actualizeaza_apartament(apartament_nou, apartament_vechi);
    }
};

#endif //LABORATOR9_UNDO_H
