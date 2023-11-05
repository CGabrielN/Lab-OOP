#ifndef LABORATOR10_GUI_H
#define LABORATOR10_GUI_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QWindow>
#include <QTableWidget>
#include <QHeaderView>
#include "../Business/service.h"
#include <map>
#include "Lista_GUI.h"
#include "Lista_DesenGUI.h"
#include "myQlistView.h"

using std::map;

class MyListModel;

class ApartamentUI : public QWidget {
    friend class MyListModel;
private:
Q_OBJECT
    MyListModel* model;

    Service &service;
    QListView *listaApartamente;
    vector<Apartament> currentElements;

    QLineEdit *numarApartamentEdit;
    QLineEdit *numeProprietarEdit;
    QLineEdit *suprafataEdit;
    QLineEdit *tipApartamentEdit;

    QPushButton *addButton;
    QPushButton *updateButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;
    QPushButton *undoButton;

    QCheckBox *sortNumeCheck;
    QCheckBox *sortSuprafataCheck;
    QCheckBox *sortTipSuprafataCheck;

    QLineEdit *filterSuprafataEdit;
    QLineEdit *filterTipApartamentEdit;


    QPushButton *notificareButton;
    QPushButton *notificareDeseneButton;

    vector<QPushButton *> raportButtons;
    QVBoxLayout *vButtonRaportLayout;

    void butoaneRaport();

    void reloadList(vector<Apartament> elements);

    void connect();

    int selectedIndex();

    void initGUICmps(QWidget *widget);

    vector<ListaNotificareGUI *> windows;
    vector<ListaDesenGUI *> windows_desen;

public:
    ApartamentUI(Service &service) : service{service} {
        initGUICmps(this);
    }

    void destroy_windows(){
        for(auto w : windows){
            w->close();
            delete w;
        }
        windows.clear();
        for(auto w: windows_desen){
            w->close();
            delete w;
        }
        windows_desen.clear();
    }

    void closeEvent(QCloseEvent *event) {
        destroy_windows();
        event->accept();
    }

};

#endif //LABORATOR10_GUI_H
