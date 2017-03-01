#pragma once

#include <functional>

class BossCar;

class BossAction {
public:

    BossAction(BossCar &boss) : _Executed(false), _Boss(boss) {}

    BossAction(BossCar &boss, std::function<void()> action);

    virtual void execute();

    /**
     * @return true, if the action has finished executing
     */
    bool hasBeenExecuted() { return _Executed; }

protected:
    bool _Executed;
    BossCar &_Boss;
private:
    std::function<void()> _Action;
};
