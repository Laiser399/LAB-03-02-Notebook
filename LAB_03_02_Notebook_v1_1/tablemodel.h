#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "common/UserStruct.h"

//   typedef QSharedPointer<User> spUser;   //

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
signals:

public slots:

public:
    explicit TableModel(QObject *parent = 0);

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool hasChildren(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    // functions
    void clear();
    void append(spUser newUser);
    void setValues(const std::vector<spUser> &valArr);


private:
    QVector<spUser> tableArr;
};

#endif // TABLEMODEL_H
