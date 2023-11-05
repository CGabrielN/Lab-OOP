#include "export.h"
#include <fstream>

using std::ifstream;
using std::ofstream;

void export_to_CSV(const string &file_name, const vector<Apartament> &lista_apartamente) {
    ofstream out("../Export/" + file_name, std::ios::trunc);
    // std::ios::trunc means that when the file is opened, the old contents are immediately removed
    if (!out.is_open()) {
        throw ApartamentException("Nu se poate deschide fisierul de export: " + file_name);
    }
    for (const auto &apartament: lista_apartamente) {
        out << apartament.get_nr_apartament() << ",";
        out << apartament.get_nume_proprietar() << ",";
        out << apartament.get_suprafata() << ",";
        out << apartament.get_tip_apartament() << "\n";
    }
    out.close();
}

void export_to_HTML(const string &file_name, const vector<Apartament> &lista_apartamente) {
    ofstream out("../Export/" + file_name, std::ios::trunc);
    // std::ios::trunc means that when the file is opened, the old contents are immediately removed
    if (!out.is_open()) {
        throw ApartamentException("Nu se poate deschide fisierul de export: " + file_name);
    }

    out << "<html><body>" << '\n';
    out << "<table border=\"1\" style=\"width:100%\">" << '\n';
    out << "<td> Numar apartament </td>" << '\n';
    out << "<td> Nume proprietar </td>" << '\n';
    out << "<td> Suprafata </td>" << '\n';
    out << "<td> Tip apartament </td>" << '\n';

    for (const auto &apartament: lista_apartamente) {
        out << "<tr>" << '\n';
        out << "<td>" << apartament.get_nr_apartament() << "</td>" << '\n';
        out << "<td>" << apartament.get_nume_proprietar() << "</td>" << '\n';
        out << "<td>" << apartament.get_suprafata() << "</td>" << '\n';
        out << "<td>" << apartament.get_tip_apartament() << "</td>" << '\n';
        out << "</tr>" << '\n';
    }
    out << "</table>" << '\n';
    out << "</body></html>" << '\n';
    out.close();
}