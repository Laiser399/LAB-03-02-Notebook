#include "TempHashFunction.h"
#include <string>
#include <QString>

template<>
int tempHashFunc<int>(int key, int multi) {
    return key * multi;
}

template<>
int tempHashFunc<std::string>(std::string key, int multi) {
    int sum = 0;
    for (int i = 0; i < key.length(); i++) {
        sum += key[i];
    }
    return sum * multi;
}

template<>
int tempHashFunc<QString>(QString key, int multi) {
    int sum = 0;
    for (int i = 0; i < key.length(); i++) {
        sum += key[i].unicode();
    }
    return sum * multi;
}

template<>
int tempHashFunc<void*>(void *pointer, int multi) {
    return (int)pointer * multi;
}
