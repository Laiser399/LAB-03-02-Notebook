#include "UserDataDialog.h"
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>

#include <QDebug>


QString internationalFormat(const QString &phoneNumber, bool &ok) {
    ok = false;

    int numDigits = 0;
    // +0(123)456-78-90
    QString result("+");
    for (int i = 0; i < phoneNumber.length(); i++) {
        if (phoneNumber[i].isDigit()) {
            numDigits++;
            result += phoneNumber[i];

            if (numDigits == 1) {
                result += '(';
            }
            else if (numDigits == 4) {
                result += ')';
            }
            else if ((numDigits == 7) || (numDigits == 9)) {
                result += '-';
            }

            if (numDigits > 11)
                break;
        }
    }

    if (numDigits != 11) {
        ok = false;
        return phoneNumber;
    }

    ok = true;
    return result;
}


//-------------|
//   private   |
//-------------|
void UserDataDialog::setWidgets(){
    ok_pressed = false;

    QGridLayout * gLay = new QGridLayout;
    for (int i = 0; i < 6; i++) {
        edits[i] = new QLineEdit;
    }

    phoneNumbers = new QListWidget;

    setLayout(gLay);

    gLay->addWidget(new QLabel("Имя:"), 0, 0, Qt::AlignRight);
    gLay->addWidget(new QLabel("Фамилия:"), 0, 2, Qt::AlignRight);
    gLay->addWidget(new QLabel("Отчество:"), 1, 0, Qt::AlignRight);
    gLay->addWidget(new QLabel("Эл. почта:"), 1, 2, Qt::AlignRight);
    gLay->addWidget(new QLabel("Компания:"), 2, 0, Qt::AlignRight);
    gLay->addWidget(new QLabel("Должность:"), 2, 2, Qt::AlignRight);

    gLay->addWidget(edits[0], 0, 1);
    gLay->addWidget(edits[1], 0, 3);
    gLay->addWidget(edits[2], 1, 1);
    gLay->addWidget(edits[3], 1, 3);
    gLay->addWidget(edits[4], 2, 1);
    gLay->addWidget(edits[5], 2, 3);

    gLay->addWidget(phoneNumbers, 3, 0, 1, 4);

    QPushButton *bAddNumber = new QPushButton("Добавить номер");
    connect(bAddNumber, SIGNAL(clicked()), SLOT(slot_addPhoneNumber()));
    QPushButton *bDelNumber = new QPushButton("Удалить номер");
    connect(bDelNumber, SIGNAL(clicked()), SLOT(slot_delPhoneNumber()));


    QPushButton *bOk = new QPushButton("Ок");
    connect(bOk, SIGNAL(clicked()), SLOT(slot_OK()));
    QPushButton *bCancel = new QPushButton("Отмена");
    connect(bCancel, SIGNAL(clicked()), SLOT(close()));


    QGridLayout *buttonsLay = new QGridLayout;
    buttonsLay->setSpacing(40);

    QGridLayout *phoneButtons = new QGridLayout;
    phoneButtons->setSpacing(4);
    phoneButtons->addWidget(bAddNumber, 0, 0);
    phoneButtons->addWidget(bDelNumber, 0, 1);
    buttonsLay->addLayout(phoneButtons, 0, 0);

    QGridLayout *contolButtons = new QGridLayout;
    contolButtons->setSpacing(4);
    contolButtons->addWidget(bOk, 0, 0);
    contolButtons->addWidget(bCancel, 0, 1);
    buttonsLay->addLayout(contolButtons, 0, 1);

    gLay->addLayout(buttonsLay, 4, 0, 1, 4);

    gLay->setRowStretch(3, 1);

    connect(phoneNumbers, SIGNAL(itemChanged(QListWidgetItem*)),
            SLOT(slot_itemChanged(QListWidgetItem*)));

}

//------------|
//   public   |
//------------|
UserDataDialog::UserDataDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Редактирование данных");

    setWidgets();
}

User UserDataDialog::getUserData() {
    for (int i = 0; i < 6; i++) {
        edits[i]->setText("");
    }
    phoneNumbers->clear();

    this->exec();
    if (ok_pressed) {
        User result;
        result.name = edits[0]->text().simplified();
        result.surname = edits[1]->text().simplified();
        result.patronymic = edits[2]->text().simplified();
        result.email = edits[3]->text().simplified();
        result.company = edits[4]->text().simplified();
        result.position = edits[5]->text().simplified();
        // добавление номеров из QListWidget
        QList<QListWidgetItem*> itemsList = phoneNumbers->findItems("", Qt::MatchContains);
        for (auto it = itemsList.begin(), it_l = itemsList.end();
             it != it_l; ++it)
        {
            if ((*it)->text().simplified().length() != 0) {
                result.phoneNumbers.append((*it)->text());
            }
        }
        return result;
    }
    else {
        return User();
    }
}

User UserDataDialog::getUserData(const User &startUser) {
    phoneNumbers->clear();

    edits[0]->setText(startUser.name);
    edits[1]->setText(startUser.surname);
    edits[2]->setText(startUser.patronymic);
    edits[3]->setText(startUser.email);
    edits[4]->setText(startUser.company);
    edits[5]->setText(startUser.position);
    for (auto it = startUser.phoneNumbers.begin(),
         it_l = startUser.phoneNumbers.end(); it != it_l; ++it)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        phoneNumbers->addItem(item);
        item->setText(*it);
    }

    this->exec();
    if (ok_pressed) {
        User result;
        result.name = edits[0]->text().simplified();
        result.surname = edits[1]->text().simplified();
        result.patronymic = edits[2]->text().simplified();
        result.email = edits[3]->text().simplified();
        result.company = edits[4]->text().simplified();
        result.position = edits[5]->text().simplified();
        // добавление номеров из QListWidget
        QList<QListWidgetItem*> itemsList = phoneNumbers->findItems("", Qt::MatchContains);
        for (auto it = itemsList.begin(), it_l = itemsList.end();
             it != it_l; ++it)
        {
            if ((*it)->text().simplified().length() != 0) {
                result.phoneNumbers.append((*it)->text());
            }
        }
        return result;
    }
    else {
        return User();
    }
}

//   private slots   //
void UserDataDialog::slot_addPhoneNumber() {
    QListWidgetItem *item = new QListWidgetItem;
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    phoneNumbers->addItem(item);
}

void UserDataDialog::slot_delPhoneNumber() {
    QListWidgetItem * currItem = phoneNumbers->currentItem();
    int row = phoneNumbers->currentRow();

    if (currItem) {
        if (QMessageBox::question(this, "?",
                QString("Удалить номер \"%1\"?").arg(currItem->text())) != QMessageBox::Yes)
        {
            return;
        }
        delete phoneNumbers->takeItem(row);
    }
}

void UserDataDialog::slot_OK() {


    // проверка edit'ов
    for (int i = 0; i < 6; i++) {
        if (edits[i]->text().simplified().length() == 0) {
            QMessageBox::information(this, "Ошибка", "Одно из полей не заполнено!");
            return;
        }
    }

    bool ok = false;
    QList<QListWidgetItem*> listItems = phoneNumbers->findItems("", Qt::MatchContains);
    for (auto it = listItems.begin(), it_l = listItems.end();
         it != it_l; ++it)
    {
        (*it)->setText(internationalFormat((*it)->text(), ok));
        if (!ok) {
            QMessageBox::information(this, "Ошибка", "Один из номеров не в международном формате!");
            return;
        }
    }



    ok_pressed = true;
    close();
}

void UserDataDialog::slot_itemChanged(QListWidgetItem *item) {
    bool ok = false;
    item->setText(internationalFormat(item->text(), ok));
    if (ok || (item->text().length() == 0)) {
        item->setBackgroundColor(QColor(255, 255, 255));
    }
    else {
        item->setBackgroundColor(QColor(255, 153, 153));
    }
}

//    QWidget interface   //
void UserDataDialog::showEvent(QShowEvent *)
{
    ok_pressed = false;
}
