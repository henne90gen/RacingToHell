#pragma once

#include <functional>

class BossAction {
public:

    BossAction() : _Executed(false) {}
    BossAction(std::function<void()> action);

    virtual void execute();

    /**
     * @return true, if the action has finished executing
     */
    bool hasBeenExecuted() { return _Executed; }

protected:
    bool _Executed;

private:
    std::function<void()> _Action;
};
