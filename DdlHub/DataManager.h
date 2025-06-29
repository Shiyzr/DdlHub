#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include "models.h"

class DataManager {
public:
    bool saveUser(const User &user);
    bool loadUser(QString &username, User &userOut);
};

#endif // DATAMANAGER_H
