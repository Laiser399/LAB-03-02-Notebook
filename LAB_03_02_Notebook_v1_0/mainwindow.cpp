#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QGridLayout>

#include <QPushButton>
#include <QMessageBox>

#include <QDebug>

//-------------|
//   private   |
//-------------|
void MainWindow::setWidgets() {
    QWidget *w = new QWidget;

    QGridLayout *vLay = new QGridLayout;
        QPushButton *bAddRecord = new QPushButton("Добавить запись");
        QPushButton *bEditRecord = new QPushButton("Редактировать запись");
        QPushButton *bDeleteRecord = new QPushButton("Удалить запись");

        QPushButton *bSetSearchProps = new QPushButton("Параметры поиска");
        QPushButton *bSearch = new QPushButton("Поиск");
        QPushButton *bResetSearch = new QPushButton("Сбросить поиск");
        QPushButton *bShowLastUsed = new QPushButton("Последние использованные");

        pTable = new QTableView;
            model = new TableModel;
        QPushButton *bTest = new QPushButton("Click for test");

    setCentralWidget(w);
    w->setLayout(vLay);
        vLay->addWidget(bAddRecord, 0, 0);
        vLay->addWidget(bEditRecord, 0, 1);
        vLay->addWidget(bDeleteRecord, 0, 2);

        vLay->addWidget(bSetSearchProps, 1, 0);
        vLay->addWidget(bSearch, 1, 1);
        vLay->addWidget(bResetSearch, 1, 2);
        vLay->addWidget(bShowLastUsed, 1, 3);

        vLay->addWidget(pTable, 2, 0, 1, 4);
            pTable->setModel(model);
        vLay->addWidget(bTest);



    pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTable->resizeColumnsToContents();
    pTable->resizeRowsToContents();

    connect(bTest, SIGNAL(clicked()), SLOT(slot_test()));
    connect(bAddRecord, SIGNAL(clicked()), SLOT(slot_buttonAddRecord()));
    connect(bEditRecord, SIGNAL(clicked()), SLOT(slot_buttonEditRecord()));
    connect(bDeleteRecord, SIGNAL(clicked()), SLOT(slot_buttonDeleteRecord()));

    connect(bSetSearchProps, SIGNAL(clicked()), SLOT(slot_buttonSetSearchProperties()));
    connect(bSearch, SIGNAL(clicked()), SLOT(slot_buttonSearch()));
    connect(bResetSearch, SIGNAL(clicked()), SLOT(slot_buttonResetSearch()));
    connect(bShowLastUsed, SIGNAL(clicked()), SLOT(slot_buttobShowLastUsed()));

}

void MainWindow::setMenu() {
    QMenu *mFile = new QMenu("&Файл");
    mFile->addAction("&Открыть", this, SLOT(slot_menuOpen()), Qt::CTRL + Qt::Key_O);
    mFile->addAction("&Сохранить", this, SLOT(slot_menuSave()), Qt::CTRL + Qt::Key_S);
    mFile->addAction("Сохранить &как...", this, SLOT(slot_menuSaveAs()));
    mFile->addSeparator();
    mFile->addAction("&Выйти", this, SLOT(close()));

    QMenu *mHelp = new QMenu("&?");
    mHelp->addAction("&Автор", this, SLOT(slot_menuAuthor()));



    menuBar()->addMenu(mFile);
    menuBar()->addMenu(mHelp);
}


//------------|
//   public   |
//------------|
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Записная книжка");
    resize(740, 400);

    setMenu();
    setWidgets();

}

MainWindow::~MainWindow()
{

}


//-------------------|
//   private slots   |
//-------------------|
//   MENU SLOTS   //
void MainWindow::slot_menuOpen() {
    qDebug() << "open";
}

void MainWindow::slot_menuSave() {
    qDebug() << "save";
}

void MainWindow::slot_menuSaveAs() {
    qDebug() << "save as";
}

void MainWindow::slot_menuAuthor() {
    QMessageBox::information(0, "Автор", "Студент МАИ\n"
                             "Группа: М8О-213Б-17\n"
                             "ФИО: Семенов Сергей Дмитриевич\n");
}


//   BUTTONS SLOTS   //
void MainWindow::slot_buttonAddRecord() {
    User tmUser = dlg.getUserData();
    if (tmUser.name.length() == 0)
        return;

    User *pUser = new User(tmUser);
    spUser spNewUser(pUser);
    // вставка в хеш таблицы
    allRecords.insert((void*)pUser, spNewUser);

    hash_name.insert(spNewUser->name, spNewUser);
    hash_surname.insert(spNewUser->surname, spNewUser);
    hash_patronymic.insert(spNewUser->patronymic, spNewUser);
    hash_email.insert(spNewUser->email, spNewUser);
    hash_company.insert(spNewUser->company, spNewUser);
    hash_position.insert(spNewUser->position, spNewUser);

    for (int i = 0; i < spNewUser->phoneNumbers.size(); i++) {
        hash_phoneNumber.insert(spNewUser->phoneNumbers[i], spNewUser);
    }

    model->append(spNewUser);
    pTable->resizeColumnsToContents();
    pTable->resizeRowsToContents();
}

void MainWindow::slot_buttonEditRecord() {
    QModelIndexList listIndexes = pTable->selectionModel()->selectedRows();
    if (listIndexes.size() != 1) {
        QMessageBox::information(this, "Ошибка", "Выделено больше одной записи!");
        return;
    }

    User *pUser = static_cast<User*>(listIndexes[0].internalPointer());
    int currRow = listIndexes[0].row();

    User tmUserData = dlg.getUserData(*pUser);
    if (tmUserData.name.length() == 0)
        return;

    // удаление старых значений из хеш таблиц
    bool ok = false;
    std::pair<void*, spUser> currPair = allRecords.find((void*)pUser, ok);
    spUser currSPointer = currPair.second;
    Q_ASSERT(ok);
    hash_name.erase(currSPointer->name, currSPointer);
    hash_surname.erase(currSPointer->surname, currSPointer);
    hash_patronymic.erase(currSPointer->patronymic, currSPointer);
    hash_email.erase(currSPointer->email, currSPointer);
    hash_company.erase(currSPointer->company, currSPointer);
    hash_position.erase(currSPointer->position, currSPointer);
    for (int i = 0; i < currSPointer->phoneNumbers.size(); i++) {
        hash_phoneNumber.erase(currSPointer->phoneNumbers[i], currSPointer);
    }
    // изменение
    *currSPointer = tmUserData;
    // добавление новых
    hash_name.insert(currSPointer->name, currSPointer);
    hash_surname.insert(currSPointer->surname, currSPointer);
    hash_patronymic.insert(currSPointer->patronymic, currSPointer);
    hash_email.insert(currSPointer->email, currSPointer);
    hash_company.insert(currSPointer->company, currSPointer);
    hash_position.insert(currSPointer->position, currSPointer);
    for (int i = 0; i < currSPointer->phoneNumbers.size(); i++) {
        hash_phoneNumber.insert(currSPointer->phoneNumbers[i], currSPointer);
    }

    emit model->dataChanged(model->index(currRow, 0, QModelIndex()),
                            model->index(currRow, 6, QModelIndex()));

    pTable->resizeColumnsToContents();
    pTable->resizeRowToContents(currRow);
}

void MainWindow::slot_buttonDeleteRecord() {
    qDebug() << "delete record";
}

void MainWindow::slot_buttonSetSearchProperties() {
    qDebug() << "set props";
}

void MainWindow::slot_buttonSearch() {
    qDebug() << "search";
}

void MainWindow::slot_buttonResetSearch() {
    qDebug() << "reset search";
}

void MainWindow::slot_buttobShowLastUsed() {
    qDebug() << "show last used";
}




void MainWindow::slot_test() {

}














