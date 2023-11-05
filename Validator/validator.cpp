#include "validator.h"
#include <cassert>
#include <sstream>

void ValidatorApartament::validate(const Apartament &apartament) {
    string msgs;
    if(apartament.get_nr_apartament() <= 0) msgs += "Apartament <= 0 !\n";
    if(apartament.get_nume_proprietar().empty()) msgs += "Nume proprietar vid!\n";
    if(apartament.get_tip_apartament().empty()) msgs += "Tip apartament vid!\n";
    if(apartament.get_suprafata() < 0) msgs += "suprafata <= 0 !\n";
    if(!msgs.empty())
        throw ValidateException(msgs);
}

ostream& operator<<(ostream& out, const ValidateException& ex) {
    out << ex.msgs;
    return out;
}

void teste_validator(){
    ValidatorApartament validatorApartament;
    Apartament apartament_valid{12,"Kyle",70, "2 cam"};
    validatorApartament.validate(apartament_valid);

    Apartament apartament_invalid{0, "", -12, ""};
    try {
        validatorApartament.validate(apartament_invalid);
    }
    catch (const ValidateException & ex) {
        std::stringstream sout;
        sout << ex;
        auto mesaj = sout.str();
        assert(mesaj.find("<= 0!") > 0);
        assert(mesaj.find("vid!") > 0);
        const auto& msg = ex.get_msgs();
        assert(msg.find("<= 0!") > 0);
        assert(msg.find("vid!") > 0);

    }
}