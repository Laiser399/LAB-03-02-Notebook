#include "tablemodel.h"

#include <QDebug>

TableModel::TableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

//-------------|
//   virtual   |
//-------------|
QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column);

    if (parent.isValid())
        return QModelIndex();
    if ((row < 0) || (row >= tableArr.size()))
        return QModelIndex();

    return createIndex(row, column, tableArr[row].data());
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return tableArr.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 7;
}

bool TableModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid())
        return false;
    else
        return true;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        User *currUser = static_cast<User*>(index.internalPointer());
        switch (index.column()) {
        case 0: return currUser->name;
        case 1: return currUser->surname;
        case 2: return currUser->patronymic;
        case 3: return currUser->email;
        case 4: return currUser->company;
        case 5: return currUser->position;
        case 6: {
            QString phoneNumbers;
            for (int i = 0; i < currUser->phoneNumbers.size(); i++) {
                phoneNumbers += currUser->phoneNumbers[i] + ';';
                if (i < currUser->phoneNumbers.size() - 1)
                    phoneNumbers += '\n';
            }
            return phoneNumbers;
        }
        }
    }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        User *user = static_cast<User*>(index.internalPointer());
        switch (index.column()) {
        case 0: user->name = value.toString(); break;
        case 1: user->surname = value.toString(); break;
        case 2: user->patronymic = value.toString(); break;
        case 3: user->email = value.toString(); break;
        case 4: user->company = value.toString(); break;
        case 5: user->position = value.toString(); break;
        }
        return true;
    }

    return false;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case 0: return "Имя";
            case 1: return "Фамилия";
            case 2: return "Отчество";
            case 3: return "email";
            case 4: return "Компания";
            case 5: return "Должность";
            case 6: return "Телефонные номера";
            }
        }
    }
    else if (orientation == Qt::Vertical) {
        if (role == Qt::DisplayRole)
            return section + 1;
    }

    return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}


//---------------|
//   functions   |
//---------------|

void TableModel::clear() {
    if (tableArr.size() < 1)
        return;

    emit beginRemoveRows(QModelIndex(), 0, tableArr.size() - 1);
    tableArr.clear();
    emit endRemoveRows();
}

void TableModel::append(spUser newUser) {
    emit beginInsertRows(QModelIndex(), tableArr.size(), tableArr.size());
    tableArr.append(newUser);
    emit endInsertRows();
}

void TableModel::setValues(const std::vector<spUser> &valArr) {
    clear();
    if (valArr.size() < 1)
        return;

    emit beginInsertRows(QModelIndex(), 0, valArr.size() - 1);
    for (auto it = valArr.begin(), it_l = valArr.end();
         it != it_l; ++it)
    {
        tableArr.append(*it);
    }
    emit endInsertRows();
}




