#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <algorithm>
#include <iostream>
#include <QMessageBox>
#include <QGroupBox>
#include <sstream>
#include <utility>


#include "GUI.h"

void ApartamentUI::butoaneRaport() {
    for (const auto &buton: raportButtons) {
        this->vButtonRaportLayout->removeWidget(buton);
        delete buton;
    }
    this->raportButtons.clear();
    map<string, vector<Apartament>> mapa_raport = this->service.raport_elemente_in_fucntie_de_tip();
    for (const auto &pair: mapa_raport) {
        auto *buttonNou = new QPushButton(QString::fromStdString(pair.first));
        this->raportButtons.push_back(buttonNou);
        this->vButtonRaportLayout->addWidget(raportButtons[raportButtons.size() - 1]);
        QObject::connect(buttonNou,
                         &QPushButton::clicked,
                         [this, pair]() {
                             QMessageBox::information(this, "Number of elemnts",
                                                      QString::fromStdString(std::to_string(pair.second.size())));
                         });
    }
}


void ApartamentUI::initGUICmps(QWidget *widget) {
    widget->resize(800, 700);
    auto mainVBoxLayout = new QVBoxLayout(widget);
    auto hLayoutraport_lista = new QHBoxLayout;
    this->listaApartamente = new QListView;
    this->vButtonRaportLayout = new QVBoxLayout;

    auto hLayoutFiltrareExport = new QHBoxLayout;
    auto vFilterLayout = new QVBoxLayout;

    this->filterSuprafataEdit = new QLineEdit;
    this->filterSuprafataEdit->setPlaceholderText("Filtrare dupa suprafata");
    this->filterSuprafataEdit->setFixedHeight(36);
    vFilterLayout->addWidget(this->filterSuprafataEdit);

    this->filterTipApartamentEdit = new QLineEdit;
    this->filterTipApartamentEdit->setPlaceholderText("Filtrare dupa tip apartament");
    this->filterTipApartamentEdit->setFixedHeight(36);
    vFilterLayout->addWidget(this->filterTipApartamentEdit);

    hLayoutFiltrareExport->addLayout(vFilterLayout);

    auto vNotificareLayout = new QVBoxLayout;
    this->notificareButton = new QPushButton("Notificari");
    this->notificareDeseneButton = new QPushButton("Desen");
//    vExportLayout->addWidget(this->exportButton);
    vNotificareLayout->addWidget(this->notificareButton);
    vNotificareLayout->addWidget(this->notificareDeseneButton);
    hLayoutFiltrareExport->addLayout(vNotificareLayout);

    mainVBoxLayout->addLayout(hLayoutFiltrareExport);


    hLayoutraport_lista->addWidget(this->listaApartamente);
    hLayoutraport_lista->addLayout(this->vButtonRaportLayout);
    mainVBoxLayout->addLayout(hLayoutraport_lista);

    //butoaneRaport();
    auto hLayout = new QHBoxLayout;

    auto formLayout = new QFormLayout;
    auto buttonsLayout = new QFormLayout;

    this->numarApartamentEdit = new QLineEdit;
    this->numeProprietarEdit = new QLineEdit;
    this->suprafataEdit = new QLineEdit;
    this->tipApartamentEdit = new QLineEdit;

    this->numarApartamentEdit->setFixedHeight(36);
    this->numeProprietarEdit->setFixedHeight(36);
    this->suprafataEdit->setFixedHeight(36);
    this->tipApartamentEdit->setFixedHeight(36);

    this->numarApartamentEdit->setFixedWidth(256);
    this->numeProprietarEdit->setFixedWidth(256);
    this->suprafataEdit->setFixedWidth(256);
    this->tipApartamentEdit->setFixedWidth(256);

    formLayout->setVerticalSpacing(25);
    formLayout->setHorizontalSpacing(25);
    formLayout->addRow(QString("Nr. Apartament"), this->numarApartamentEdit);
    formLayout->addRow(QString("Nume proprietar"), this->numeProprietarEdit);
    formLayout->addRow(QString("Suprafata"), this->suprafataEdit);
    formLayout->addRow(QString("Tip apartament"), this->tipApartamentEdit);

    hLayout->addLayout(formLayout);

    this->addButton = new QPushButton("Adauga");
    this->deleteButton = new QPushButton("Sterge");
    this->searchButton = new QPushButton("Cauta apartament");
    this->updateButton = new QPushButton("Modifica");
    this->undoButton = new QPushButton("Undo");
    buttonsLayout->addWidget(this->addButton);
    buttonsLayout->addWidget(this->deleteButton);
    buttonsLayout->addWidget(this->searchButton);
    buttonsLayout->addWidget(this->updateButton);
    buttonsLayout->addWidget(this->undoButton);

    hLayout->addLayout(buttonsLayout);

    mainVBoxLayout->addLayout(hLayout);

    auto butoaneSortareLayout = new QHBoxLayout;
    this->sortNumeCheck = new QCheckBox("Sorteaza nume");
    this->sortSuprafataCheck = new QCheckBox("Sorteaza suprafata");
    this->sortTipSuprafataCheck = new QCheckBox("Sorteaza tip + suprafata");
//    this->unsorted = new QPushButton("Nesortate");

    butoaneSortareLayout->addWidget(this->sortNumeCheck);
    butoaneSortareLayout->addWidget(this->sortSuprafataCheck);
    butoaneSortareLayout->addWidget(this->sortTipSuprafataCheck);
//    butoaneSortareLayout->addWidget(this->unsorted);

    mainVBoxLayout->addLayout(butoaneSortareLayout);

    this->model = new MyListModel(this->listaApartamente, this);
    this->listaApartamente->setModel(model);

    this->reloadList(this->service.get_all());

    this->connect();
}



void ApartamentUI::reloadList(vector<Apartament> elements) {
    this->currentElements = std::move(elements);
    this->model->emitDataChanged();
}

void ApartamentUI::connect() {
    QObject::connect(this->addButton,
                     &QPushButton::clicked,
                     [this]() {
                         auto numar_apartament = this->numarApartamentEdit->text().toStdString();
                         auto nume_proprietar = this->numeProprietarEdit->text().toStdString();
                         auto suprafata = this->suprafataEdit->text().toStdString();
                         auto tip_apartament = this->tipApartamentEdit->text().toStdString();


                         if (numar_apartament.empty() || nume_proprietar.empty() || suprafata.empty() ||
                             tip_apartament.empty()) {
                             QMessageBox::information(this, "Something went wrong", "No field can be empty");
                             return;
                         }
                         try {
                             this->service.adauga_apartament(std::stoi(numar_apartament), nume_proprietar,
                                                             std::stof(suprafata), tip_apartament);
                         }
                         catch (const ApartamentRepoException &ex) {
                             QMessageBox::information(this, "Something went wrong",
                                                      QString::fromStdString(ex.get_msg()));
                             this->reloadList(this->service.get_all());
                             return;
                         }
                         catch (const ValidateException &ex) {
                             QMessageBox::warning(this, "Something went wrong",
                                                  QString::fromStdString(ex.get_msgs()));
                             this->reloadList(this->service.get_all());
                             return;
                         }

                         this->reloadList(this->service.get_all());

                         this->numarApartamentEdit->clear();
                         this->numeProprietarEdit->clear();
                         this->suprafataEdit->clear();
                         this->tipApartamentEdit->clear();
                     });


    QObject::connect(this->deleteButton,
                     &QPushButton::clicked,
                     [this]() {
                         auto numar_apartament = this->numarApartamentEdit->text().toStdString();
                         auto nume_proprietar = this->numeProprietarEdit->text().toStdString();
                         auto suprafata = this->suprafataEdit->text().toStdString();
                         auto tip_apartament = this->tipApartamentEdit->text().toStdString();


                         if (numar_apartament.empty() || nume_proprietar.empty() || suprafata.empty() ||
                             tip_apartament.empty()) {
                             QMessageBox::information(this, "Something went wrong", "No field can be empty");
                             return;
                         }

                         try {
                             this->service.sterge_apartament(std::stoi(numar_apartament), nume_proprietar,
                                                             std::stof(suprafata), tip_apartament);
                         }
                         catch (const ValidateException &ex) {
                             QMessageBox::warning(this, "Something went wrong",
                                                  QString::fromStdString(ex.get_msgs()));
                             this->reloadList(this->service.get_all());
                             return;
                         }
                         catch (const ApartamentRepoException &ex) {
                             QMessageBox::information(this, "Something went wrong",
                                                      QString::fromStdString(ex.get_msg()));
                             this->reloadList(this->service.get_all());
                             return;
                         }

                         this->reloadList(this->service.get_all());

                         this->numarApartamentEdit->clear();
                         this->numeProprietarEdit->clear();
                         this->suprafataEdit->clear();
                         this->tipApartamentEdit->clear();
                     });


    QObject::connect(this->updateButton,
                     &QPushButton::clicked, [this]() {
                int index = this->selectedIndex();
                if (index == -1) {
                    QMessageBox::information(this, "Nothing selected", "Nothing selected");
                    return;
                }
                auto element = this->currentElements.at(index);

                auto numar_apartament = this->numarApartamentEdit->text().toStdString();
                auto nume_proprietar = this->numeProprietarEdit->text().toStdString();
                auto suprafata = this->suprafataEdit->text().toStdString();
                auto tip_apartament = this->tipApartamentEdit->text().toStdString();


                if (numar_apartament.empty() || nume_proprietar.empty() || suprafata.empty() ||
                    tip_apartament.empty()) {
                    QMessageBox::information(this, "Something went wrong", "No field can be empty");
                    return;
                }
                try {
                    this->service.actualizare_apartament(element.get_nr_apartament(), element.get_nume_proprietar(),
                                                         element.get_suprafata(), element.get_tip_apartament(),
                                                         std::stoi(numar_apartament), nume_proprietar,
                                                         std::stof(suprafata), tip_apartament);
                }
                catch (const ValidateException &ex) {
                    QMessageBox::warning(this, "Something went wrong",
                                         QString::fromStdString(ex.get_msgs()));
                    this->reloadList(this->service.get_all());
                    return;
                }

                this->reloadList(this->service.get_all());

                this->numarApartamentEdit->clear();
                this->numeProprietarEdit->clear();
                this->suprafataEdit->clear();
                this->tipApartamentEdit->clear();
            });


    QObject::connect(this->searchButton,
                     &QPushButton::clicked,
                     [this]() {
                         auto numar_apartament = this->numarApartamentEdit->text().toStdString();
                         auto nume_proprietar = this->numeProprietarEdit->text().toStdString();
                         auto suprafata = this->suprafataEdit->text().toStdString();
                         auto tip_apartament = this->tipApartamentEdit->text().toStdString();


                         if (numar_apartament.empty() && nume_proprietar.empty() && suprafata.empty() &&
                             tip_apartament.empty()) {
                             this->reloadList(this->service.get_all());
                             return;
                         }

                         try {
                             if (numar_apartament.empty() || suprafata.empty()) {
                                 numar_apartament = suprafata = "-1";
                             }
                             int index = this->service.cauta_apartament(std::stoi(numar_apartament), nume_proprietar,
                                                                        std::stof(suprafata), tip_apartament);
                             this->reloadList({this->service.get_all()[index]});
                         }
                         catch (const ApartamentRepoException &ex) {
                             QMessageBox::information(this, "Something went wrong",
                                                      QString::fromStdString(ex.get_msg()));
                             this->reloadList(this->service.get_all());
                             return;
                         }
                         catch (const ValidateException &ex) {
                             QMessageBox::warning(this, "Something went wrong",
                                                  QString::fromStdString(ex.get_msgs()));
                             this->reloadList(this->service.get_all());
                             return;
                         }

                         this->numarApartamentEdit->clear();
                         this->numeProprietarEdit->clear();
                         this->suprafataEdit->clear();
                         this->tipApartamentEdit->clear();
                     });


    QObject::connect(this->undoButton,
                     &QPushButton::clicked,
                     [this]() {
                         try {
                             this->service.undo();
                         }
                         catch (const ApartamentRepoException &ex) {
                             QMessageBox::information(this, "Something went wrong",
                                                      QString::fromStdString(ex.get_msg()));
                         }

                         this->reloadList(this->service.get_all());

                         this->numarApartamentEdit->clear();
                         this->numeProprietarEdit->clear();
                         this->suprafataEdit->clear();
                         this->tipApartamentEdit->clear();
                     });


    QObject::connect(this->filterSuprafataEdit,
                     &QLineEdit::textChanged,
                     [this](const QString &text) {
                         if (text.isEmpty()) {
                             this->reloadList(this->service.get_all());
                             return;
                         }
                         auto suprafata = std::stof(text.toStdString());
                         auto filtered = this->service.filtrare_dupa_suprafata(suprafata);

                         this->reloadList(filtered);
                     });

    QObject::connect(this->filterTipApartamentEdit,
                     &QLineEdit::textChanged,
                     [this](const QString &text) {
                         if (text.isEmpty()) {
                             this->reloadList(this->service.get_all());
                             return;
                         }
                         auto tip_apartament = text.toStdString();
                         auto filtered = this->service.filtrare_dupa_tip_apartament(tip_apartament);

                         this->reloadList(filtered);
                     });


    QObject::connect(this->sortNumeCheck, &QCheckBox::stateChanged, [this](int checkState) {

        if (checkState == Qt::Checked) {
            this->sortSuprafataCheck->setCheckState(Qt::Unchecked);
            this->sortTipSuprafataCheck->setCheckState(Qt::Unchecked);
            this->reloadList(this->service.sorteaza_dupa_nume_proprietar());
        } else {
            this->reloadList(this->service.get_all());
        }
    });

    QObject::connect(this->sortSuprafataCheck, &QCheckBox::stateChanged, [this](int checkState) {
        if (checkState == Qt::Checked) {
            this->sortNumeCheck->setCheckState(Qt::Unchecked);
            this->sortTipSuprafataCheck->setCheckState(Qt::Unchecked);
            this->reloadList(this->service.sorteaza_dupa_suprafata());
        } else {
            this->reloadList(this->service.get_all());
        }
    });

    QObject::connect(this->sortTipSuprafataCheck, &QCheckBox::stateChanged, [this](int checkState) {
        if (checkState == Qt::Checked) {
            this->sortSuprafataCheck->setCheckState(Qt::Unchecked);
            this->sortSuprafataCheck->setCheckState(Qt::Unchecked);
            this->reloadList(this->service.sorteaza_dupa_tip_apartament_si_suprafata());
        } else {
            this->reloadList(this->service.get_all());
        }
    });


    QObject::connect(this->notificareButton,
                     &QPushButton::clicked,
                     [this]() {
                        auto lista = new ListaNotificareGUI{this->service};
                        lista->show();
                        windows.push_back(lista);
                        service.addObserver(lista);
    });


    QObject::connect(this->notificareDeseneButton,
                     &QPushButton::clicked,
                     [this](){
                        auto window_desen = new ListaDesenGUI{this->service};
                        window_desen->show();
                        windows_desen.push_back(window_desen);
                        service.addObserver(window_desen);
    });

    QObject::connect(this->listaApartamente->selectionModel(),
                     &QItemSelectionModel::selectionChanged,
                     [this](){
                         if (this->listaApartamente->selectionModel()->selectedIndexes().isEmpty() || this->listaApartamente->selectionModel()->selectedIndexes().at(0).row() >= service.nr_apartamente()) {
                             this->numarApartamentEdit->clear();
                             this->numeProprietarEdit->clear();
                             this->suprafataEdit->clear();
                             this->tipApartamentEdit->clear();
                             return;
                         }
                         auto selIndex = this->listaApartamente->selectionModel()->selectedIndexes().at(0);
                         auto date = selIndex.data(Qt::DisplayRole).toString();
                         string nr, nume,suprafata,tip;
                         std::stringstream X(date.toStdString());
                         std::getline(X,nr,'.');
                         std::getline(X,nume,':');
                         nume.erase(nume.begin());
                         nume.erase(nume.end() - 1);
                         std::getline(X,suprafata,'-');
                         suprafata.erase(suprafata.begin());
                         suprafata.erase(suprafata.end()- 1);
                         std::getline(X,tip,'\n');
                         tip.erase(tip.begin());
                         this->numarApartamentEdit->setText(QString::fromStdString(nr));
                         this->numeProprietarEdit->setText(QString::fromStdString(nume));
                         this->suprafataEdit->setText(QString::fromStdString(suprafata));
                         this->tipApartamentEdit->setText(QString::fromStdString(tip));
    });
}

int ApartamentUI::selectedIndex() {
    auto index = this->listaApartamente->selectionModel()->currentIndex();
    return index.row();
}