#pragma once

#include <functional>

class BossCar;

class BossAction : public std::enable_shared_from_this<BossAction> {
public:

    BossAction(BossCar &boss) : _Executed(false), _Boss(boss) {}

    BossAction(BossCar &boss, std::function<void()> action);

    virtual void execute();

    void setNextAction(std::shared_ptr<BossAction> nextAction) { _NextAction = nextAction; }

    void setParentAction(std::shared_ptr<BossAction> parentAction) { _ParentAction = parentAction; }

    void setDeleteAfterExecution(bool flag) { _DeleteAfterExecution = flag; }

    bool deleteAfterExecution() { return _DeleteAfterExecution; }

    /**
     * @return true, if the action has finished executing
     */
    bool hasBeenExecuted() { return _Executed; }

protected:
    /**
     * Sets the executed flag to true and processes _NextAction
     */
    void finishExecution();

    BossCar &_Boss;
    std::shared_ptr<BossAction> _ParentAction, _NextAction;
private:
    // Can be set to true with finishExecution()
    bool _Executed, _DeleteAfterExecution;
    std::function<void()> _Action;
};
