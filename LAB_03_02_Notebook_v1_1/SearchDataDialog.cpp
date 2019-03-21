#include "SearchDataDialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <QDebug>

//-------------|
//   private   |
//-------------|
void SearchDataDialog::setWidgets() {
    QGridLayout *gLay = new QGridLayout;
    for (int i = 0; i < 7; i++) {
        edits[i] = new QLineEdit;
    }
    email_check = new QCheckBox("Точный поиск");
    phone_check = new QCheckBox("Точный поиск");
    QPushButton *bOk = new QPushButton("Ок");
    QPushButton *bCancel = new QPushButton("Отмена");

    // расстановка
    gLay->addWidget(new QLabel("Имя:"), 0, 0, Qt::AlignRight);
    gLay->addWidget(new QLabel("Фамилия:"), 0, 2, Qt::AlignRight);
    gLay->addWidget(new QLabel("Отчество:"), 1, 0, Qt::AlignRight);
    gLay->addWidget(new QLabel("Компания:"), 1, 2, Qt::AlignRight);

    gLay->addWidget(new QLabel("Должность:"), 2, 0, Qt::AlignRight);

    gLay->addWidget(new QLabel("Эл. почта:"), 3, 0, Qt::AlignRight);
    gLay->addWidget(new QLabel("Телефон:"), 3, 2, Qt::AlignRight);

    gLay->addWidget(edits[0], 0, 1);
    gLay->addWidget(edits[1], 0, 3);
    gLay->addWidget(edits[2], 1, 1);
    gLay->addWidget(edits[4], 1, 3);

    gLay->addWidget(edits[5], 2, 1);

    gLay->addWidget(edits[3], 3, 1);
    gLay->addWidget(edits[6], 3, 3);

    setLayout(gLay);

    gLay->addWidget(email_check, 4, 0, 1, 2, Qt::AlignRight);
    gLay->addWidget(phone_check, 4, 2, 1, 2, Qt::AlignRight);

    gLay->addWidget(bOk, 5, 2);
    gLay->addWidget(bCancel, 5, 3);

    connect(bOk, SIGNAL(clicked()), SLOT(slot_buttonOk()));
    connect(bCancel, SIGNAL(clicked()), SLOT(close()));
}

//------------|
//   public   |
//------------|
SearchDataDialog::SearchDataDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Параметры поиска");

    setWidgets();
}

searchStruct SearchDataDialog::getSearchData() {
    // выставление начальных параметров
    for (int i = 0; i < 7; i++) {
        edits[i]->setText("");
    }
    email_check->setCheckState(Qt::Unchecked);
    phone_check->setCheckState(Qt::Unchecked);

    searchStruct result;
    result.isValid = true;
    exec();
    if (!ok_pressed) {
        result.isValid = false;
        return result;
    }

    result.name = edits[0]->text().simplified();
    result.surname = edits[1]->text().simplified();
    result.patronymic = edits[2]->text().simplified();
    result.email = edits[3]->text().simplified();
    result.company = edits[4]->text().simplified();
    result.position = edits[5]->text().simplified();
    result.phoneNumber = edits[6]->text().simplified();
    if (email_check->checkState() == Qt::Checked)
        result.email_is_accurate = true;
    else
        result.email_is_accurate = false;
    if (phone_check->checkState() == Qt::Checked)
        result.ph_is_accurate = true;
    else
        result.ph_is_accurate = false;

    return result;
}

searchStruct SearchDataDialog::getSearchData(searchStruct &startData) {
    // выставление начальных параметров
    edits[0]->setText(startData.name);
    edits[1]->setText(startData.surname);
    edits[2]->setText(startData.patronymic);
    edits[3]->setText(startData.email);
    edits[4]->setText(startData.company);
    edits[5]->setText(startData.position);
    edits[6]->setText(startData.phoneNumber);
    if (startData.email_is_accurate)
        email_check->setCheckState(Qt::Checked);
    else
        email_check->setCheckState(Qt::Unchecked);
    if (startData.ph_is_accurate)
        phone_check->setCheckState(Qt::Checked);
    else
        phone_check->setCheckState(Qt::Unchecked);


    searchStruct result;
    result.isValid = true;
    exec();
    if (!ok_pressed) {
        result.isValid = false;
        return result;
    }


    result.name = edits[0]->text().simplified();
    result.surname = edits[1]->text().simplified();
    result.patronymic = edits[2]->text().simplified();
    result.email = edits[3]->text().simplified();
    result.company = edits[4]->text().simplified();
    result.position = edits[5]->text().simplified();
    result.phoneNumber = edits[6]->text().simplified();
    if (email_check->checkState() == Qt::Checked)
        result.email_is_accurate = true;
    else
        result.email_is_accurate = false;
    if (phone_check->checkState() == Qt::Checked)
        result.ph_is_accurate = true;
    else
        result.ph_is_accurate = false;

    return result;
}

//-------------------|
//   private slots   |
//-------------------|
void SearchDataDialog::slot_buttonOk() {
    ok_pressed = true;
    close();
}

//-----------------------|
//   virtual functions   |
//-----------------------|
void SearchDataDialog::showEvent(QShowEvent *) {
    ok_pressed = false;
}
