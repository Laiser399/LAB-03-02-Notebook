#ifndef USERDATADIALOG_H
#define USERDATADIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>

#include "common/UserStruct.h"

class UserDataDialog : public QDialog
{
    Q_OBJECT
private:
    bool ok_pressed;
    QLineEdit *edits[6];
    QListWidget *phoneNumbers;


    void setWidgets();
public:
    explicit UserDataDialog(QWidget *parent = 0);

    User getUserData();
    User getUserData(const User &startUser);

signals:

public slots:

private slots:
    void slot_addPhoneNumber();
    void slot_delPhoneNumber();
    void slot_OK();
    void slot_itemChanged(QListWidgetItem*);

    // QWidget interface
protected:
    void showEvent(QShowEvent *);

};

#endif // USERDATADIALOG_H
