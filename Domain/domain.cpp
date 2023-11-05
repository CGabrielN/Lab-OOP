#include "domain.h"
#include <cassert>
#include <sstream>

int Apartament::get_nr_apartament() const {
    return nr_apartament;
}

string Apartament::get_nume_proprietar() const {
    return nume_proprietar;
}

float Apartament::get_suprafata() const {
    return suprafata;
}

string Apartament::get_tip_apartament() const {
    return tip_apartament;
}

bool Apartament::operator==(const Apartament &alta) const {
    double eps = 0.0000000000001;
    if (nr_apartament == alta.get_nr_apartament() && nume_proprietar == alta.get_nume_proprietar() &&
        tip_apartament == alta.get_tip_apartament() && suprafata - alta.get_suprafata() < eps)
        return true;
    return false;
}

std::ostream &operator<<(std::ostream &os, const Apartament &apartament) {
    os << apartament.get_nr_apartament() << ". " << apartament.get_nume_proprietar() << " : "
       << apartament.get_suprafata() << " - " << apartament.get_tip_apartament() << '\n';
    return os;
}

string Apartament::to_string() const {
    string apartament_string;
    apartament_string = std::to_string(nr_apartament) + ". " + nume_proprietar + " : ";
    std::stringstream suprafataStringStream;
    suprafataStringStream << suprafata;
    string suprafataString = suprafataStringStream.str();
    apartament_string += suprafataString;

    apartament_string+= " - " + tip_apartament;
    return apartament_string;
}

bool compara_nume_proprietar(const Apartament &apartament1, const Apartament &apartament2) {
    return apartament1.get_nume_proprietar() < apartament2.get_nume_proprietar();
}

bool compara_suprafata(const Apartament &apartament1, const Apartament &apartament2) {
    return apartament1.get_suprafata() - apartament2.get_suprafata() < 0.0000000000001;
}


void teste_domain() {
    Apartament apartament{12, "Bob John", 60, "2 cam"};
    assert(apartament.get_nr_apartament() == 12);
    assert(apartament.get_nume_proprietar() == "Bob John");
    assert(apartament.get_tip_apartament() == "2 cam");
    assert(apartament.get_suprafata() - 60 < 0.00000001);
    Apartament &apartament1{apartament};
    assert(apartament1.get_nr_apartament() == 12);
    assert(apartament1.get_nume_proprietar() == "Bob John");
    assert(apartament1.get_tip_apartament() == "2 cam");
    assert(apartament1.get_suprafata() - 60 < 0.00000001);
    assert(apartament == apartament1);
    Apartament apartament2{12, "Bob John", 60, "3 cam"};
    assert(!(apartament == apartament2));
    Apartament apartament3{20, "Kyle", 80, "3 cam"};
    assert(compara_nume_proprietar(apartament1, apartament3) == true);
    assert(compara_suprafata(apartament1, apartament3) == true);
    Apartament apartament4{5, "Anton", 30, "garsoniera"};
    Apartament apartament5{apartament4};
    Apartament apartament6;
    assert(apartament5.get_nr_apartament() == apartament4.get_nr_apartament());
    assert(compara_nume_proprietar(apartament1, apartament4) == false);
    assert(compara_suprafata(apartament1, apartament4) == false);
    cout << apartament;
}