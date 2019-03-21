#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include "tablemodel.h"
#include "HashMultiTable.h"
#include "HashTable.h"
#include "UserDataDialog.h"

using std::vector;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    TableModel *model;
    QTableView *pTable;
    UserDataDialog dlg;

    HashTable<void*, spUser> allRecords;
    HashMultiTable<QString, spUser> hash_name, hash_surname, hash_patronymic,
        hash_email, hash_company, hash_position, hash_phoneNumber;


    void setWidgets();
    void setMenu();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slot_menuOpen();
    void slot_menuSave();
    void slot_menuSaveAs();
    void slot_menuAuthor();

    void slot_buttonAddRecord();
    void slot_buttonEditRecord();
    void slot_buttonDeleteRecord();

    void slot_buttonSetSearchProperties();
    void slot_buttonSearch();
    void slot_buttonResetSearch();

    void slot_buttobShowLastUsed();

    void slot_test();
};

#endif // MAINWINDOW_H
