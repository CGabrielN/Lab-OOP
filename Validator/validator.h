#ifndef LABORATOR8_VALIDATOR_H
#define LABORATOR8_VALIDATOR_H

#include "../Domain/domain.h"
#include <ostream>

using std::ostream;
using std::string;

class ValidatorApartament {
public:
    /**
     * valideaza apartamentul primita ca si parametru, acesta trebuie sa aiba numele proprietarului si tipul stringuri nevide
     * iar pretul si numarul apartamentului un numar >= 0
     * @param apartament - instanta a clasei de Apartament ce trebuie validata
     * @throws ValidateException(msgs) unde msgs e un string ce contine informatii despre ce date sunt invalide
     */
    void validate(const Apartament &apartament);
};

class ValidateException {
    string msgs;
public:
    ValidateException(const string &errors) : msgs{errors} {}

    const string& get_msgs() const {
        return msgs;
    }

    friend ostream &operator<<(ostream &out, const ValidateException &ex);
};

ostream &operator<<(ostream &out, const ValidateException &ex);

void teste_validator();

#endif //LABORATOR8_VALIDATOR_H
