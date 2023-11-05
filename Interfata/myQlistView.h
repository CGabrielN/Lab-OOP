#ifndef LABORATOR14_MYQLISTVIEW_H
#define LABORATOR14_MYQLISTVIEW_H

#include "QWidget"
#include "QVariant"
#include "QAbstractListModel"
#include "GUI.h"

class ApartamentUI;

class MyListModel : public QAbstractListModel {
    Q_OBJECT
private:
    friend class ApartamentUI;
    ApartamentUI* gui;
public:
    MyListModel(QObject *parent, ApartamentUI* ui) : QAbstractListModel(parent), gui{ui} {}

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void emitDataChanged(){
        QModelIndex topLeft = createIndex(0,0);
        QModelIndex bottomRight = createIndex(rowCount(),0);

        emit dataChanged(topLeft,bottomRight);
    }
};

#endif //LABORATOR14_MYQLISTVIEW_H
