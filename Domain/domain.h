#ifndef LABORATOR8_DOMAIN_H
#define LABORATOR8_DOMAIN_H

#include <string>
#include <iostream>

using std::string;
using std::cout;

class ApartamentException{
    string msg;
public:
    /**
     * constructorul exceptiei pentru apartamente
     * @param m - mesajul exceptiei
     */
    ApartamentException(const string& m):msg{m}{};

    /**
     * returneaza valoarea parametrului privat de mesaj
     * @return string
     */
    [[nodiscard]] string get_msg()const {return msg; }
};

class Apartament {
private:
    string nume_proprietar, tip_apartament;
    int nr_apartament;
    float suprafata;
public:
    Apartament() {
//        cout << "Apartament creat fara parametrii!\n";
    };

    /**
     * constructorul obiectului de apartament
     * @param nr_apartament - int
     * @param nume_proprietar - string
     * @param suprafata - float
     * @param tip_apartament - string
     */
    Apartament(int nr_apartament, const string &nume_proprietar, float suprafata, const string &tip_apartament) :
            nr_apartament{nr_apartament}, nume_proprietar{nume_proprietar}, suprafata{suprafata},
            tip_apartament{tip_apartament} {
//        cout << "Apartament creat cu parametrii!\n";
    }

    /**
     * construieste un obiect nou cu atributele obiectului curent, creand o copie a valorilor
     * @param of - un obiect de tip apartament
     */
    Apartament(const Apartament &ot) : nr_apartament{ot.nr_apartament}, nume_proprietar{ot.nume_proprietar},
                                 suprafata{ot.suprafata}, tip_apartament{ot.tip_apartament} {
//        cout << "Apartament creat prin copiere!!!!!\n";
    }

    /**
     * returneaza numarul apartamentului
     * @return nr_apartament - int
     */
    [[nodiscard]] int get_nr_apartament() const;

    /**
     * returneaza numele proprietarului apartamentului
     * @return nume proprietar - string
     */
    [[nodiscard]] string get_nume_proprietar() const;

    /**
     * returneaza suprafata apartamentului
     * @return suprafata - float
     */
    [[nodiscard]] float get_suprafata() const;

    /**
     * returneaza tipul apartamentului
     * @return tip apartament - string
     */
    [[nodiscard]] string get_tip_apartament() const;

    /**
     * verifica egalitatea intre 2 apartamente - suprascrierea operatorului de ==
     * @param altul - apartamentul cu care se compara apartamentul curent
     * @return true daca cele 2 apartamente au toate atributele egale, false altfel
     */
    bool operator==(const Apartament &altul) const;

    /**
     * suprascriere la operatorul de << pentru a putea afisa apartamentul, salvand in os toate datele despre un apartament sub forma unui string
     * @param os - ostream ce contine stringul ce contine datele apartamentului
     * @param apartament - apartamentul pentru care se cere afisarea
     * @return os - ostream
     */
    friend std::ostream &operator<<(std::ostream &os, const Apartament &apartament);

    string to_string() const;

    ~Apartament() {
//        cout<<"Apartament distrus!\n";
    };
};

/**
 * compara denumirea a 2 apartamente si returneaza care este mai mica lexicografic
 * @param apartament1 - primul apartament
 * @param apartament2 - cel de al 2-lea apartament
 * @return true daca apartamentul 1 este mai mic decat cel de al 2-lea, false altfel
 */
bool compara_nume_proprietar(const Apartament &apartament1, const Apartament &apartament2);

/**
 * compara destinatia a 2 apartamente si returneaza care este mai mica lexicografic
 * @param apartament1 - primul apartament
 * @param apartament2 - cel de al 2-a apartament
 * @return true daca apartamentul 1 este mai mic decat cel de al 2-lea, false altfel
 */
bool compara_suprafata(const Apartament &apartament1, const Apartament &apartament2);

void teste_domain();

#endif //LABORATOR8_DOMAIN_H
