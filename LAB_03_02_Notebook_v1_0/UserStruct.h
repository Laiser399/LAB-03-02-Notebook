#ifndef USERSTRUCT_H
#define USERSTRUCT_H

#include <QSharedPointer>
struct User {
    QString name, surname, patronymic, email, company, position;
    QStringList phoneNumbers;
};
typedef QSharedPointer<User> spUser;

#endif // USERSTRUCT_H
