#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include "Domain/domain.h"
#include "Persistenta/repo.h"
#include "Persistenta/lista_notificare.h"
#include "Business/service.h"
#include "Interfata/UI.h"
#include "Validator/validator.h"
#include "Interfata/GUI.h"


void run_all_tests() {
    teste_validator();
    teste_domain();
    teste_repo();
    teste_lista_notificare();
    teste_service();
    system("cls");
}

int main(int argc, char *argv[]) {
    run_all_tests();
    {
        QApplication a(argc, argv);
        QApplication::setFont(QFont{"Calibri", 18});
        ValidatorApartament validator;
        AbsRepo *repo = new RepoFile{"apartamente.txt"};
        Notificare notificare{repo};
        Service service{repo, validator, notificare};
        ApartamentUI ui{service};
        ui.show();
        a.exec();
//        ui.destroy_windows();
        delete repo;
    }

    return 0;
}

