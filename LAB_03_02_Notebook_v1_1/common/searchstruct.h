#ifndef SEARCHSTRUCT_H
#define SEARCHSTRUCT_H

#include <QString>
struct searchStruct {
    QString name, surname, patronymic,
        email, company, position, phoneNumber;
    // false - поиск по части почты/ном. телефона
    bool email_is_accurate, ph_is_accurate;
    bool isValid; // false - поиск не задан
};

#endif // SEARCHSTRUCT_H
