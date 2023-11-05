#include "myQlistView.h"

QVariant MyListModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole){
        if(gui->currentElements.size() <= index.row())
            return {};

        auto Ap = gui->currentElements.at(index.row());
        return QString::fromStdString(Ap.to_string());
    }
    return {};
}

int MyListModel::rowCount(const QModelIndex &parent) const {
    return (int) gui->currentElements.size();
}