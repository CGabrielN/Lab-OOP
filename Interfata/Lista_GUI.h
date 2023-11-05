#ifndef LABORATOR14_LISTA_GUI_H
#define LABORATOR14_LISTA_GUI_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWindow>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>
#include <QFormLayout>
#include "../Business/service.h"
#include "observer.h"

class ListaNotificareGUI : public QWidget, public Observer {
private:
    Service &service;
    QTableWidget *tabel_apartamente_notificare;
    vector<Apartament> currentElementsNotificare;
    QPushButton *golesteNotificareButton;
    QLineEdit *numarApartamentNEdit;
    QLineEdit *nrApGeneratEdit;
    QPushButton *adaugaNotificareButton;
    QPushButton *generareNotificariButton;

    QLineEdit *fileNameText;
    QPushButton *exportButton;
    QPushButton *exportCSVButton;
    QPushButton *exportHTMLButton;
    QWidget *window_export;

    void reloadListaNotificare(const vector<Apartament> &elements);

    void initGUI();

    void connect();

public:
    ListaNotificareGUI(Service &serv) : service{serv} {
//        service.addObserver(this);
        initGUI();
        connect();
        reloadListaNotificare(service.get_all_notificari());
    }

    void update() override{
        reloadListaNotificare(service.get_all_notificari());
    }

    ~ListaNotificareGUI(){
        service.removeObserver(this);
    }

};

#endif //LABORATOR14_LISTA_GUI_H
