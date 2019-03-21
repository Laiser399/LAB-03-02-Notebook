#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QSet>

#include <QDebug>

QString getNumbersFromStr(QString &str) {
    QString res;
    for (auto it = str.begin(), it_l = str.end();
         it != it_l; ++it)
    {
        if (it->isDigit())
            res += *it;
    }
    return res;
}


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
        QPushButton *bShowLastUsed = new QPushButton("Последние использованные");
        QPushButton *bShowAllRecords = new QPushButton("Все записи");

        pTable = new QTableView;
            model = new TableModel;

    setCentralWidget(w);
    w->setLayout(vLay);
        vLay->addWidget(bAddRecord, 0, 0);
        vLay->addWidget(bEditRecord, 0, 1);
        vLay->addWidget(bDeleteRecord, 0, 2);

        vLay->addWidget(bSetSearchProps, 1, 0);
        vLay->addWidget(bSearch, 1, 1);
        vLay->addWidget(bShowLastUsed, 1, 2);
        vLay->addWidget(bShowAllRecords, 1, 3);

        vLay->addWidget(pTable, 2, 0, 1, 4);
            pTable->setModel(model);



    pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTable->resizeColumnsToContents();
    pTable->resizeRowsToContents();

    connect(bAddRecord, SIGNAL(clicked()), SLOT(slot_dialogAddRecord()));
    connect(bEditRecord, SIGNAL(clicked()), SLOT(slot_dialogEditRecord()));
    connect(bDeleteRecord, SIGNAL(clicked()), SLOT(slot_dialogDeleteRecord()));

    connect(bSetSearchProps, SIGNAL(clicked()), SLOT(slot_dialogSetSearchProperties()));
    connect(bSearch, SIGNAL(clicked()), SLOT(slot_search()));
    connect(bShowAllRecords, SIGNAL(clicked()), SLOT(slot_showAllRecords()));
    connect(bShowLastUsed, SIGNAL(clicked()), SLOT(slot_showLastUsed()));

}

void MainWindow::setMenu() {
    QMenu *mFile = new QMenu("&Файл");
    mFile->addAction("&Открыть", this, SLOT(slot_menuOpen()), Qt::CTRL + Qt::Key_O);
    mFile->addAction("&Сохранить", this, SLOT(slot_menuSave()), Qt::CTRL + Qt::Key_S);
    mFile->addAction("Сохранить &как...", this, SLOT(slot_menuSaveAs()));
    mFile->addSeparator();
    mFile->addAction("&Выйти", this, SLOT(close()));

    QMenu *mEdit = new QMenu("&Правка");
    mBackModif = mEdit->addAction("&Отменить", this, SLOT(slot_modifierBack()), Qt::CTRL + Qt::Key_Z);
    mForwardModif = mEdit->addAction("&Вернуть", this, SLOT(slot_modifierForward()), Qt::CTRL + Qt::SHIFT + Qt::Key_Z);
    mBackModif->setEnabled(false);
    mForwardModif->setEnabled(false);

    QMenu *mHelp = new QMenu("&?");
    mHelp->addAction("&Автор", this, SLOT(slot_menuAuthor()));

    menuBar()->addMenu(mFile);
    menuBar()->addMenu(mEdit);
    menuBar()->addMenu(mHelp);
}

void MainWindow::refreshModel() {
    if (modelPrinted == mAll) {
        slot_showAllRecords();
    }
    else if (modelPrinted == mSearch) {
        slot_search();
    }
    else if (modelPrinted == mLastUsed) {
        slot_showLastUsed();
    }
}

void MainWindow::saveUserData(QString &filename) {
    vector<pair<void*, spUser> > allPairs = allRecords.getAllPairs();

    QFile fout(filename);
    if (!fout.open(QFile::WriteOnly)) {
        QMessageBox::information(this, "Ошибка", "Ошибка открытия файла!");
        return;
    }

    QTextStream stream(&fout);
    for (auto it = allPairs.begin(), it_l = allPairs.end();
         it != it_l; ++it)
    {
        stream << it->second->name << ", ";
        stream << it->second->surname << ", ";
        stream << it->second->patronymic << ", ";
        stream << it->second->email << ", ";
        stream << it->second->company << ", ";
        stream << it->second->position << ", ";
        for (auto it_p = it->second->phoneNumbers.begin(),
             it_p_l = it->second->phoneNumbers.end(); it_p != it_p_l; ++it_p)
        {
            stream << *it_p;
            if (it_p != it_p_l - 1)
                stream << ", ";
        }
        stream << ";\n";
    }

    fout.close();
}

// действия с записями
void MainWindow::removeRecord(spUser user) {
    // удаление из хеш таблиц
    allRecords.erase(user.data());

    hash_name.erase(user->name, user);
    hash_surname.erase(user->surname, user);
    hash_patronymic.erase(user->patronymic, user);
    hash_email.erase(user->email, user);
    hash_company.erase(user->company, user);
    hash_position.erase(user->position, user);
    for (auto it = user->phoneNumbers.begin(),
         it_l = user->phoneNumbers.end(); it != it_l; ++it)
    {
        hash_phoneNumber.erase(*it, user);
    }

}

void MainWindow::removeAllRecords() {
    allRecords.clear();

    hash_name.clear();
    hash_surname.clear();
    hash_patronymic.clear();
    hash_email.clear();
    hash_company.clear();
    hash_position.clear();
    hash_phoneNumber.clear();
}

void MainWindow::addRecord(spUser user) {
    allRecords.insert(user.data(), user);

    hash_name.insert(user->name, user);
    hash_surname.insert(user->surname, user);
    hash_patronymic.insert(user->patronymic, user);
    hash_email.insert(user->email, user);
    hash_company.insert(user->company, user);
    hash_position.insert(user->position, user);
    for (auto it = user->phoneNumbers.begin(),
         it_l = user->phoneNumbers.end(); it != it_l; ++it)
    {
        hash_phoneNumber.insert(*it, user);
    }

}

// последние использованные
void MainWindow::addLastUsedRecord(spUser user) {
    lastUsedArr.push_back(user);
    while (lastUsedArr.size() > N_LAST_USED)
        lastUsedArr.pop_front();
}

void MainWindow::checkLastUsedForDelete() {
    bool ok = false;
    for (auto it = lastUsedArr.begin();
         it != lastUsedArr.end(); ++it)
    {
        // если в осн таблице не найдено -> удалить
        allRecords.find(it->data(), ok);
        if (!ok) {
            it = lastUsedArr.erase(it);
            it--;
//            if (it == lastUsedArr.end())
//                break;
        }

    }
}

// последние изменения
void MainWindow::addModifier(int typeModif, spUser spChangedRecord,
                             User userBefore, User userAfter)
{
    modifierStruct newModif;
    newModif.spChangedRecord = spChangedRecord;
    newModif.typeModifier = typeModif;

    if (typeModif == modifierStruct::tmAdding) {
        newModif.beforeModif = User();
        newModif.afterModif = userAfter;
    }
    else if (typeModif == modifierStruct::tmEditing) {
        newModif.beforeModif = userBefore;
        newModif.afterModif = userAfter;
    }
    else if (typeModif == modifierStruct::tmDeletion) {
        newModif.beforeModif = userBefore;
        newModif.afterModif = User();
    }

    backModif.append(newModif);
    while (backModif.size() > M_LAST_MODIF)
        backModif.pop_front();

    forwarkModif.clear();

    mBackModif->setEnabled(true);
    mForwardModif->setEnabled(false);
}

bool MainWindow::backModifier() {
    if (backModif.size() < 1)
        return false;

    modifierStruct tm_struct = backModif.takeLast();
    if (tm_struct.typeModifier == modifierStruct::tmAdding) {
        // удалить эту запись
        removeRecord(tm_struct.spChangedRecord);
    }
    else if (tm_struct.typeModifier == modifierStruct::tmEditing) {
        // отменить изменения
        removeRecord(tm_struct.spChangedRecord);
        *tm_struct.spChangedRecord = tm_struct.beforeModif;
        addRecord(tm_struct.spChangedRecord);
    }
    else if (tm_struct.typeModifier == modifierStruct::tmDeletion) {
        // вернуть запись
        addRecord(tm_struct.spChangedRecord);
    }
    refreshModel();

    forwarkModif.append(tm_struct);
    return true;
}

bool MainWindow::forwardModifier() {
    if (forwarkModif.size() < 1)
        return false;

    modifierStruct tm_struct = forwarkModif.takeLast();
    if (tm_struct.typeModifier == modifierStruct::tmAdding) {
        // удалить эту запись
        addRecord(tm_struct.spChangedRecord);
    }
    else if (tm_struct.typeModifier == modifierStruct::tmEditing) {
        // отменить изменения
        removeRecord(tm_struct.spChangedRecord);
        *tm_struct.spChangedRecord = tm_struct.afterModif;
        addRecord(tm_struct.spChangedRecord);
    }
    else if (tm_struct.typeModifier == modifierStruct::tmDeletion) {
        // вернуть запись
        removeRecord(tm_struct.spChangedRecord);
    }
    refreshModel();

    backModif.append(tm_struct);
    return true;
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

    modelPrinted = mAll;

}

MainWindow::~MainWindow()
{

}


//-------------------|
//   private slots   |
//-------------------|
//   MENU SLOTS   //
void MainWindow::slot_menuOpen() {
    QString filename = QFileDialog::getOpenFileName(this, "Выберите имя файла", "", "*.txt");
    if (filename.length() == 0)
        return;

    QFile fin(filename);
    if (!fin.open(QFile::ReadOnly)) {
        QMessageBox::information(this, "Ошибка", "Ошибка открытия файла!");
        return;
    }

    removeAllRecords(); // удаление всех записей
    currFilename = filename;

    QTextStream stream(&fin);
    QString fullFile = stream.readAll();
    QStringList recordsList = fullFile.split(';');

    int notAddedCount = 0;
    for (auto it_rec = recordsList.begin(), it_rec_l = recordsList.end();
         it_rec != it_rec_l; ++it_rec)
    {
        if (it_rec->simplified().length() == 0)
            continue;

        QStringList args = it_rec->split(',');
        if (args.size() < 6) {
            notAddedCount++;
            continue;
        }

        User *newUser = new User;
        spUser spNewUser(newUser);

        spNewUser->name = args[0].simplified();
        spNewUser->surname = args[1].simplified();
        spNewUser->patronymic = args[2].simplified();
        spNewUser->email = args[3].simplified();
        spNewUser->company = args[4].simplified();
        spNewUser->position = args[5].simplified();
        for (int i = 6; i < args.size(); i++)
            spNewUser->phoneNumbers.append(args[i].simplified());

        addRecord(spNewUser);
    }

    if (notAddedCount > 0)
        QMessageBox::information(this, "Внимание!",
                QString("Не было добавлено %1 записей!").arg(notAddedCount));
    fin.close();

    slot_showAllRecords();
}

void MainWindow::slot_menuSave() {
    if (currFilename.length() == 0)
        return;
    saveUserData(currFilename);
}

void MainWindow::slot_menuSaveAs() {
    QFileDialog dlg;
    dlg.setDefaultSuffix("txt");
    QString filename = dlg.getSaveFileName(this, "Выберите файл для сохранения", "", "*.txt");
    if (filename.indexOf(".txt") != filename.length() - 4)
        filename += ".txt";

    if (filename.length() == 0)
        return;

    saveUserData(filename);
}

void MainWindow::slot_menuAuthor() {
    QMessageBox::information(0, "Автор", "Студент МАИ\n"
                             "Группа: М8О-213Б-17\n"
                             "ФИО: Семенов Сергей Дмитриевич\n");
}


//   BUTTONS SLOTS   //
void MainWindow::slot_dialogAddRecord() {
    User tmUser = dlg_user_data.getUserData();
    if (tmUser.name.length() == 0)
        return;

    User *pUser = new User(tmUser);
    spUser spNewUser(pUser);
    // вставка в хеш таблицы
    addRecord(spNewUser);

    addLastUsedRecord(spNewUser);

    if (modelPrinted == mAll) {
        model->append(spNewUser);
    }
    else if (modelPrinted == mSearch) {
        slot_search();
    }
    else if (modelPrinted == mLastUsed) {
        slot_showLastUsed();
    }
    else
        Q_ASSERT(false);

    addModifier(modifierStruct::tmAdding, spNewUser, User(), *spNewUser);

    pTable->resizeColumnsToContents();
    pTable->resizeRowsToContents();
}

void MainWindow::slot_dialogEditRecord() {
    // получение выделенных индексов модели
    QModelIndexList listIndexes = pTable->selectionModel()->selectedRows();
    if (listIndexes.size() != 1) {
        QMessageBox::information(this, "Ошибка", "Выделено больше одной записи или не выделено ни одной!");
        return;
    }

    User *pUser = static_cast<User*>(listIndexes[0].internalPointer());
    int currRow = listIndexes[0].row();

    // новые данные
    User tmUserData = dlg_user_data.getUserData(*pUser);
    if (tmUserData.name.length() == 0)
        return;

    // удаление старых значений из хеш таблиц
    bool ok = false;
    std::pair<void*, spUser> currPair = allRecords.find((void*)pUser, ok);
    spUser currSPointer = currPair.second;
    Q_ASSERT(ok);
    removeRecord(currSPointer);

    // изменение
    User beforeChange = *currSPointer;
    *currSPointer = tmUserData;
    // добавление новых
    addRecord(currSPointer);
    addLastUsedRecord(currSPointer);
    addModifier(modifierStruct::tmEditing, currSPointer, beforeChange, *currSPointer);

    emit model->dataChanged(model->index(currRow, 0, QModelIndex()),
                            model->index(currRow, 6, QModelIndex()));

    if (modelPrinted == mAll) {
        //model->append(spNewUser);
    }
    else if (modelPrinted == mSearch) {
        slot_search();
    }
    else if (modelPrinted == mLastUsed) {
        slot_showLastUsed();
    }
    else
        Q_ASSERT(false);

    pTable->resizeColumnsToContents();
    pTable->resizeRowToContents(currRow);
}

void MainWindow::slot_dialogDeleteRecord() {
    // получение выделенных индексов модели
    QModelIndexList listIndexes = pTable->selectionModel()->selectedRows();
    if (listIndexes.size() < 1) {
        QMessageBox::information(this, "Ошибка", "Не выделено ни одной записи!");
        return;
    }

    if (QMessageBox::question(this, "?", QString("Удалить %1 выбранных элементов?").arg(listIndexes.size()))
            != QMessageBox::Yes)
        return;

    // проход по выдел индексам
    for (auto it = listIndexes.begin(), it_l = listIndexes.end();
         it != it_l; ++it)
    {
        User *pUser = static_cast<User*>(it->internalPointer());
        //int currRow = it->row();

        bool ok = false;
        std::pair<void*, spUser> currPair = allRecords.find((void*)pUser, ok);
        spUser currSpUser = currPair.second;
        Q_ASSERT(ok);
        // удаление из хеш таблиц
        removeRecord(currSpUser);

        addModifier(modifierStruct::tmDeletion, currSpUser, *currSpUser, User());
    }

    qDebug() << "in last used before: " << lastUsedArr.size();
    checkLastUsedForDelete(); // удаление из очереди последних использованных
    qDebug() << "in last used after:  " << lastUsedArr.size();


    if (modelPrinted == mAll) {
        slot_showAllRecords();
    }
    else if (modelPrinted == mSearch) {
        slot_search();
    }
    else if (modelPrinted == mLastUsed) {
        slot_showLastUsed();
    }
    else
        Q_ASSERT(false);

}

void MainWindow::slot_dialogSetSearchProperties() {
    searchStruct tm_struct;
    tm_struct = dlgSearchParam.getSearchData(currSearchParam);
    if (tm_struct.isValid) {
        currSearchParam = tm_struct;
    }
}

void MainWindow::slot_search() {
    // поиск
    if (!currSearchParam.isValid)
        return;

    std::vector<spUser> tm_arr;
    QSet<spUser> search_results[7]; // результаты поиска
    QString search_arr[7] = { currSearchParam.name, currSearchParam.surname,
            currSearchParam.patronymic, currSearchParam.email, currSearchParam.company,
            currSearchParam.position, currSearchParam.phoneNumber };
    HashMultiTable<QString, spUser> *hash_tables_arr[7] = { &hash_name, &hash_surname,
            &hash_patronymic, &hash_email, &hash_company, &hash_position, &hash_phoneNumber};

    model->clear();
    modelPrinted = mSearch; // в тек момент отобр результаты поиска

    std::vector<std::pair<void*, spUser> > allPairs = allRecords.getAllPairs();
    try {
        for (int i = 0; i < 7; i++) {
            if ((i == 3) && (!currSearchParam.email_is_accurate) && (search_arr[i].length() > 0)) {
                // поиск по часте строки    email
                for (auto it = allPairs.begin(), it_l = allPairs.end(); it != it_l; ++it) {
                    if (it->second->email.contains(search_arr[i]))
                        search_results[i].insert(it->second);
                }
                if (search_results[i].size() == 0)
                    throw 0;
            }
            else if ((i == 6) && (!currSearchParam.ph_is_accurate) &&
                     (getNumbersFromStr(search_arr[i]).length() > 0))
            {
                // поиск по часте строки    phone
                for (auto it = allPairs.begin(), it_l = allPairs.end(); it != it_l; ++it) {
                    // итератор по телефонам
                    QString search_phone = getNumbersFromStr(search_arr[i]);
                    for (auto it_p = it->second->phoneNumbers.begin(),
                         it_p_l = it->second->phoneNumbers.end(); it_p != it_p_l; ++it_p)
                    {
                        if (getNumbersFromStr(*it_p).contains(search_phone)) {
                            search_results[i].insert(it->second);
                            break;
                        }
                    }
                }
                if (search_results[i].size() == 0)
                    throw 0;
            }
            else if (search_arr[i].length() > 0) {
                // поиск точного совпадения в хеш таблицах
                tm_arr = hash_tables_arr[i]->find(search_arr[i]);
                for (auto it = tm_arr.begin(), it_l = tm_arr.end(); it != it_l; ++it)
                    search_results[i].insert(*it);
            }

        }
    }
    catch (...) {
        // не найдено в одном из пунктов
        return;
    }

    QSet<spUser> res_search; // множество пересечения результатов поиска
    bool first_not_found = true;
    for (int i = 0; i < 7; i++) {
        if (search_results[i].size() > 0) {
            if (first_not_found) {
                res_search = search_results[i];
                first_not_found = false;
            }
            else {
                res_search &= search_results[i];
            }
        }
    }

    // добавление в модель и очередь последних использованных
    for (auto it = res_search.begin(), it_l = res_search.end();
         it != it_l; ++it)
    {
        model->append(*it);
        addLastUsedRecord(*it);
    }

    pTable->resizeColumnsToContents();
    pTable->resizeRowsToContents();
}

void MainWindow::slot_showAllRecords() {
    model->clear();
    modelPrinted = mAll;
    vector<spUser> allPairs = allRecords.getAllValues();
    model->setValues(allPairs);
//    for (auto it = allPairs.begin(), it_l = allPairs.end();
//         it != it_l; ++it)
//    {

//    }

    pTable->resizeColumnsToContents();
    pTable->resizeRowsToContents();
}

void MainWindow::slot_showLastUsed() {
    model->clear();
    modelPrinted = mLastUsed;

    QSet<spUser> setOfPrinted; // мн-во уже отображенных записей
    for (auto it = lastUsedArr.end() - 1, it_l = lastUsedArr.begin() - 1;
         it != it_l; --it)
    {
        if (setOfPrinted.find(*it) == setOfPrinted.end()) {
            // данная запись еще не была отображена
            setOfPrinted.insert(*it);
            model->append(*it);
        }
        else {
            // отображена -> пропуск
            continue;
        }
    }

    pTable->resizeColumnsToContents();
    pTable->resizeRowsToContents();
}

void MainWindow::slot_modifierBack() {
    backModifier();

    if (backModif.size() > 0)
        mBackModif->setEnabled(true);
    else
        mBackModif->setEnabled(false);
    mForwardModif->setEnabled(true);

    checkLastUsedForDelete();
}

void MainWindow::slot_modifierForward() {
    forwardModifier();

    if (forwarkModif.size() > 0)
        mForwardModif->setEnabled(true);
    else
        mForwardModif->setEnabled(false);
    mBackModif->setEnabled(true);

    checkLastUsedForDelete();
}
















