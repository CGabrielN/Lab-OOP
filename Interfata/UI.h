#ifndef LABORATOR8_UI_H
#define LABORATOR8_UI_H

#include "../Business/service.h"

class UI {
private:
    Service &service;

public:
    /**
     * constructorul obiectului de UI
     * @param service - referinta catre o instanta a clasei de service
     */
    UI(Service &service) : service{service} {};

    /**
     * nu permite copierea de obiecte UI
     * @param altul - obiectul de la care se doresc a fi copiate atributele
     */
    UI(const UI &altul) = delete;

    /**
     * afiseaza meniul si preia comenzile de la tastatura
     */
    void start();

    /**
     * citeste de la tastatura datele despre un apartament in variabilele primite ca si parametru
     * @param nr_apartament - int
     * @param nume_proprietar - string
     * @param suprafata - float
     * @param tip_apartament - string
     */
    void preluare_date(int &nr_apartament, string &nume_proprietar, float &suprafata, string &tip_apartament);

    /**
     * initiaza operatia de adaugare a unui apartament, afisand realizarea cu succes a operatiei
     */
    void ui_adauga();

    /**
     * afiseaza pe ecran toate apartamentele aflate in vectorul de apartamente primit ca si parametru sub forma unui tabel
     * cu titlul primit ca si parametru
     * @param apartamente - vector de apartamente
     * @param titlu - titlul tabelului - string
     */
    void ui_afisare_apartamente(const string &titlu, const vector<Apartament> &apartamente);

    /**
     * initiaza operatia de cautare a unyi apartament, afisand pozitia apartamentului in vector, in cazul in care el exista
     */
    void ui_cauta_apartament();

    /**
     * initiaza operatia de actualizare a unui apartament, afisand realizarea cu succes a operatiei, in cazul in care el exista
     */
    void ui_actualizare_apartament();

    /**
     * initiaza operatia de stergere a unui apartament, afisand realizarea cu succes a operatiei, in cazul in care el exista
     */
    void ui_sterge_apartament();

    /**
     * cere tipul apartamentului dupa care sa se realizeze filtrarea si apoi apeleaza functia de afisare a listei filtrate de functia din service
     */
    void ui_filtrare_dupa_tip_apartament();

    /**
     * cere suprafata dupa care sa se realizeze filtrarea si apoi apeleaza functia de afisare a listei filtrate de functia din service
     */
    void ui_filtrare_dupa_suprafata();

    /**
     * cere numarul apartamentului care sa fie adaugat in lista de notificare si afiseaza finalizarea cu succes a operatiei
     */
    void ui_adauga_apartament_lista_notificari();

    /**
     * cere numarul de apartamente care sa fie selectate random pentru a fi salvate in lista de notificare si apeleaza functia de
     * service de generare, afisand pe ecran finalizarea cu succes a generarii
     */
    void ui_generare_lista();

    /**
     * afiseaza pe ecran datele raportului realizat pe baza apartamentelor, repartizate dupa tipul apartamentului
     * @param mapa_raport - o mapa unde cheia este tipul apartamentului si value un vector ce contine obiectele
     * de tip apartament ce au tipul de apartament respectiv
     */
    void ui_afisare_raport(const map<string, vector<Apartament>> &mapa_raport);

    void meniu_lista_apartamente();

    void sortari_filtrari();

    void meniu_lista_notificare();
};

#endif //LABORATOR8_UI_H
