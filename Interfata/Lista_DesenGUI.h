#ifndef LABORATOR14_LISTA_DESENGUI_H
#define LABORATOR14_LISTA_DESENGUI_H

#include <QWidget>
#include <QWindow>
#include <QPainter>
#include <QPaintEvent>
#include <random>
#include <QMessageBox>
#include "../Business/service.h"
#include "observer.h"

class ListaDesenGUI : public QWidget, public Observer {
private:
    Service& service;
public:
    ListaDesenGUI(Service &serv) : service{serv} {
        this->setFixedHeight(600);
        this->setFixedWidth(900);
    }

    void update() override{
        QWidget::update();
    }

    int getRandomNumber(int min, int max) {
        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_int_distribution<> dist(min, max);
        return dist(eng);
    }

    void paintEvent(QPaintEvent* ev) override{
//        QWidget::paintEvent(ev);

        QPainter p{this};

        for (int i = 0; i < this->service.nr_elemenete_notificare(); ++i) {
            int x,y,height,width;
            QPolygon polygon;
            int figura = getRandomNumber(1,4);
            switch (figura) {
                case 1:
                    // desenam o elipsa
                    x = getRandomNumber(5, this->width() - 175);
                    y = getRandomNumber(5, this->height() - 175);
                    height = getRandomNumber(50, 150);
                    width = getRandomNumber(50, 150);
                    p.drawEllipse(x, y, width, height);
                    break;

                case 4:
                    // desenam un cerc
                    x = getRandomNumber(5, this->width() - 175);
                    y = getRandomNumber(5, this->height() - 175);
                    height = getRandomNumber(50, 150);
                    p.drawEllipse(x, y, height, height);
                    break;
                case 2:
                    // desenam un patrat
                    x = getRandomNumber(5, this->width() - 175);
                    y = getRandomNumber(5, this->height() - 175);
                    height = getRandomNumber(50, 150);
                    width = getRandomNumber(50, 150);
                    p.drawRect(x, y, width, height);
                    break;

                case 3:
                    // desenam un triunghi

                    polygon << QPoint(getRandomNumber(10,this->width() - 30), getRandomNumber(10,this->height() - 30));   // Punctul 1
                    polygon << QPoint(getRandomNumber(10,this->width() - 30), getRandomNumber(10,this->height() - 30)); // Punctul 2
                    polygon << QPoint(getRandomNumber(10,this->width() - 30), getRandomNumber(10,this->height() - 30));  // Punctul 3

                    p.drawPolygon(polygon);
                    break;

                case 5:
                    p.drawImage(10,10,QImage("../cerinta.png"));
                    break;

                default:
                    QMessageBox::warning(this,"Something is not ok",QString::fromStdString("Am ales random o figura neexistenta " + std::to_string(figura)));
                    break;
            }
        }
    }

    ~ListaDesenGUI(){
        service.removeObserver(this);
//        delete this;
    }
};


#endif //LABORATOR14_LISTA_DESENGUI_H
