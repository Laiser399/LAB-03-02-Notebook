#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include "tablemodel.h"
#include "common/HashMultiTable.h"
#include "common/HashTable.h"
#include "UserDataDialog.h"
#include "common/searchstruct.h"
#include <SearchDataDialog.h>
#include "common/ModifierStruct.h"

#define N_LAST_USED 20
#define M_LAST_MODIF 20

using std::vector;
using std::pair;





class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    struct modifierStruct {
        enum {
            tmAdding = 1,
            tmEditing,
            tmDeletion
        };

        spUser spChangedRecord;
        User beforeModif, afterModif;
        int typeModifier;
    };

    enum modelType { // что в тек момент отображается в модели
        mAll = 1,
        mSearch,
        mLastUsed
    };

    QString currFilename; // имя последнего открытого файла

    int modelPrinted; // что в тек момент отображается в модели
    TableModel *model;
    QTableView *pTable;
    UserDataDialog dlg_user_data;

    searchStruct currSearchParam;
    SearchDataDialog dlgSearchParam;

    // добавляются:
    //    Новые записи
    //    Отредактированные записи
    //    Результаты поиска
    QVector<spUser> lastUsedArr; // очередь последних использованных записей

    // добавляются в стек изменений:
    //    добавление
    //    редактирование
    //    удаление
    QVector<modifierStruct> backModif, forwarkModif; // последние изменения, возвращение изменений
    QAction *mBackModif, *mForwardModif;

    HashTable<void*, spUser> allRecords;
    HashMultiTable<QString, spUser> hash_name, hash_surname, hash_patronymic,
        hash_email, hash_company, hash_position, hash_phoneNumber;



    void setWidgets();
    void setMenu();

    void refreshModel();

    void saveUserData(QString &filename);
    void removeRecord(spUser user);
    void removeAllRecords();
    void addRecord(spUser user);

    void addLastUsedRecord(spUser user);
    void checkLastUsedForDelete();

    void addModifier(int typeModif, spUser spChangedRecord,
                     User userBefore, User userAfter);
    bool backModifier();
    bool forwardModifier();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slot_menuOpen();
    void slot_menuSave();
    void slot_menuSaveAs();
    void slot_menuAuthor();

    void slot_dialogAddRecord();
    void slot_dialogEditRecord();
    void slot_dialogDeleteRecord();

    void slot_dialogSetSearchProperties();
    void slot_search();
    void slot_showAllRecords();

    void slot_showLastUsed();

    void slot_modifierBack();
    void slot_modifierForward();
};

#endif // MAINWINDOW_H
