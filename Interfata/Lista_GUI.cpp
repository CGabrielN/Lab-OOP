#include "Lista_GUI.h"
#include <sstream>

void ListaNotificareGUI::reloadListaNotificare(const vector<Apartament> &elements) {

    this->currentElementsNotificare = elements;
    this->tabel_apartamente_notificare->clearContents();
    this->tabel_apartamente_notificare->setRowCount(0);

    QHeaderView *headerView = this->tabel_apartamente_notificare->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);
    int nr_rand = 0;

    for (const auto &el: elements) {
        auto nr_ap = new QTableWidgetItem;
        auto nume = new QTableWidgetItem;
        auto suprafata = new QTableWidgetItem;
        auto tip_ap = new QTableWidgetItem;

        this->tabel_apartamente_notificare->insertRow(nr_rand);

        nr_ap->setText(QString::fromStdString(std::to_string(el.get_nr_apartament())));
        nume->setText(QString::fromStdString(el.get_nume_proprietar()));
        std::stringstream suprafataStringStream;
        suprafataStringStream << el.get_suprafata();
        string suprafataString = suprafataStringStream.str();
        suprafata->setText(QString::fromStdString(suprafataString));
        tip_ap->setText(QString::fromStdString(el.get_tip_apartament()));

        this->tabel_apartamente_notificare->setItem(nr_rand, 0, nr_ap);
        this->tabel_apartamente_notificare->setItem(nr_rand, 1, nume);
        this->tabel_apartamente_notificare->setItem(nr_rand, 2, suprafata);
        this->tabel_apartamente_notificare->setItem(nr_rand, 3, tip_ap);

        nr_rand++;
    }
}

void ListaNotificareGUI::initGUI() {
    this->setFixedHeight(600);
    this->setFixedWidth(900);

    auto mainVLayout = new QVBoxLayout(this);
    auto hLayout = new QVBoxLayout;

    this->tabel_apartamente_notificare = new QTableWidget;
    this->tabel_apartamente_notificare->setColumnCount(4);
    QStringList labels;
    labels << "Nr.ap" << "Nume" << "Suprafata" << "Tip ap";
    this->tabel_apartamente_notificare->setHorizontalHeaderLabels(labels);
    hLayout->addWidget(this->tabel_apartamente_notificare);

    this->golesteNotificareButton = new QPushButton("Golire lista");
    this->adaugaNotificareButton = new QPushButton("Adauga apartament");
    this->generareNotificariButton = new QPushButton("Adauga random");
    this->numarApartamentNEdit = new QLineEdit;
    this->numarApartamentNEdit->setPlaceholderText("Nr. ap. pt add");
    this->numarApartamentNEdit->setFixedHeight(36);
    this->numarApartamentNEdit->setFixedWidth(256);

    this->nrApGeneratEdit = new QLineEdit;
    this->nrApGeneratEdit->setPlaceholderText("Nr. ap. pt generat");
    this->nrApGeneratEdit->setFixedHeight(36);
    this->nrApGeneratEdit->setFixedWidth(256);

    auto vAddLayout = new QHBoxLayout;
    auto vGenerareLayout = new QHBoxLayout;

    vAddLayout->addWidget(this->numarApartamentNEdit);
    vAddLayout->addWidget(this->adaugaNotificareButton);
    vGenerareLayout->addWidget(this->nrApGeneratEdit);
    vGenerareLayout->addWidget(this->generareNotificariButton);

    hLayout->addLayout(vAddLayout);
    hLayout->addLayout(vGenerareLayout);
    this->exportButton = new QPushButton("Exporta");

    hLayout->addWidget(this->golesteNotificareButton);
    hLayout->addWidget(this->exportButton);

    mainVLayout->addLayout(hLayout);

}

void ListaNotificareGUI::connect() {
    QObject::connect(this->golesteNotificareButton,
                     &QPushButton::clicked,
                     [this]() {
                         this->service.golire_lista_notificari();
                         this->reloadListaNotificare(this->service.get_all_notificari());
                         QMessageBox::information(this, "Lista notificari",
                                                  QString::fromStdString(
                                                          "Elemente in lista de notificare: " +
                                                          std::to_string(
                                                                  this->service.nr_elemenete_notificare())));
                     });

    QObject::connect(this->adaugaNotificareButton,
                     &QPushButton::clicked,
                     [this]() {
                         auto numar = std::stoi(this->numarApartamentNEdit->text().toStdString());
                         try {
                             this->service.adauga_apartament_lista_notificare(numar);
                             this->reloadListaNotificare(this->service.get_all_notificari());
                             QMessageBox::information(this, "Lista notificari",
                                                      QString::fromStdString(
                                                              "Elemente in lista de notificare: " +
                                                              std::to_string(
                                                                      this->service.nr_elemenete_notificare())));
                         }
                         catch (const ApartamentRepoException &ex) {
                             QMessageBox::information(this,
                                                      "Something went wrong",
                                                      QString::fromStdString(ex.get_msg()));
                         }
                         catch (const NotificareRepoException &ex) {
                             QMessageBox::information(this,
                                                      "Something went wrong",
                                                      QString::fromStdString(ex.get_msg()));
                         }
                         this->numarApartamentNEdit->clear();
                     });

    QObject::connect(this->generareNotificariButton,
                     &QPushButton::clicked,
                     [this]() {
                         auto nr_ap = this->nrApGeneratEdit->text().toStdString();
                         if (nr_ap.empty()) {
                             QMessageBox::information(this,
                                                      "Something went wrong",
                                                      "Introduceti un numar valid de apartamente");
                             this->nrApGeneratEdit->clear();
                             return;
                         }
                         int nr_ap_de_generat = std::stoi(nr_ap);
                         if (nr_ap_de_generat < 0) {
                             QMessageBox::information(this,
                                                      "Something went wrong",
                                                      "Introduceti un numar valid de apartamente");
                             this->nrApGeneratEdit->clear();
                             return;
                         }

                         this->service.generare_random_apartamente(nr_ap_de_generat);
                         this->reloadListaNotificare(this->service.get_all_notificari());
                         this->nrApGeneratEdit->clear();
                         QMessageBox::information(this, "Lista notificari",
                                                  QString::fromStdString(
                                                          "Elemente in lista de notificare: " +
                                                          std::to_string(
                                                                  this->service.nr_elemenete_notificare())));

                     });
    QObject::connect(this->exportButton,
                     &QPushButton::clicked,
                     [this]() {
                         this->window_export = new QWidget;
                         this->window_export->show();
                         this->window_export->setFixedHeight(125);
                         this->window_export->setFixedWidth(400);

                         auto mainVLayout = new QVBoxLayout(window_export);
                         auto formLayout = new QFormLayout;
                         this->fileNameText = new QLineEdit;
                         this->fileNameText->setFixedHeight(36);
                         this->fileNameText->setFixedWidth(256);
                         formLayout->addRow(QString("Nume fisier"), this->fileNameText);
                         mainVLayout->addLayout(formLayout);
                         this->exportCSVButton = new QPushButton("Export to CSV");
                         this->exportHTMLButton = new QPushButton("Export to HTML");
                         auto hButtonsLayout = new QHBoxLayout;
                         hButtonsLayout->addWidget(this->exportCSVButton);
                         hButtonsLayout->addWidget(this->exportHTMLButton);
                         mainVLayout->addLayout(hButtonsLayout);

                         QObject::connect(this->exportCSVButton,
                                          &QPushButton::clicked,
                                          [this]() {
                                              auto text = this->fileNameText->text().toStdString();
                                              if (text.empty()) {
                                                  QMessageBox::information(this->window_export, "Something went wrong",
                                                                           "File name can't be empty");
                                                  return;
                                              }
                                              text += ".txt";

                                              try {
                                                  service.exporta_lista_notificare_CSV(text);
                                                  QMessageBox::information(this->window_export, "Task completed",
                                                                           QString("Export realizat cu succes!"));
                                                  this->window_export->close();
                                              }
                                              catch (const ApartamentException &ex) {
                                                  QMessageBox::information(this->window_export, "Something went wrong",
                                                                           QString::fromStdString(ex.get_msg()));
                                              }

                                          });

                         QObject::connect(this->exportHTMLButton,
                                          &QPushButton::clicked,
                                          [this]() {
                                              auto text = this->fileNameText->text().toStdString();
                                              if (text.empty()) {
                                                  QMessageBox::information(this->window_export, "Something went wrong",
                                                                           "File name can't be empty");
                                                  return;
                                              }
                                              text += ".html";

                                              try {
                                                  service.exporta_lista_notificare_HTML(text);
                                                  QMessageBox::information(this->window_export, "Task completed",
                                                                           QString("Export realizat cu succes!"));
                                              }
                                              catch (const ApartamentException &ex) {
                                                  QMessageBox::information(this->window_export, "Something went wrong",
                                                                           QString::fromStdString(ex.get_msg()));
                                              }
                                              this->window_export->close();
                                          });
                     });
}