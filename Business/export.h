#ifndef LABORATOR9_EXPORT_H
#define LABORATOR9_EXPORT_H

#include <string>
#include <vector>
#include "../Domain/domain.h"

using std::vector;
using std::string;

/**
 * exporta in format CSV datele din lista de apartamente citita in fisierul cu numele/pathul primit ca si parametru
 * @param file_name - numele/pathul fisierului in care se vor salva datele
 * @param lista_apartamente - vectorul ce contine toate apartamentele ce trebuie salvate in fisier
 */
void export_to_CSV(const string &file_name, const vector<Apartament> &lista_apartamente);

/**
 * exporta in format HTML sub forma unui tabel datele din lista de apartamente citita in fisierul cu numele/pathul primit ca si parametru
 * @param file_name - numele/pathul fisierului in care se vor salva datele
 * @param lista_apartamente - vectorul ce contine toate apartamentele ce trebuie salvate in fisier
 */
void export_to_HTML(const string &file_name, const vector<Apartament> &lista_apartamente);

#endif //LABORATOR9_EXPORT_H
