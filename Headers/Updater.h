//
// Created by henne on 27.11.16.
//

#ifndef RACINGTOHELL_UPDATER_H
#define RACINGTOHELL_UPDATER_H

/**
 * Forward declarations
 */
class Framework;

class Updater {
public:

    Updater(Framework &framework);

    void update(float frameTime);

private:
    Framework &_FW;
};


#endif //RACINGTOHELL_UPDATER_H
