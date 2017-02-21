
#include "stdafx.h"
#include "Framework/Framework.h"

int main() {
    Framework _Framework;
    _Framework.run();

    return 0;
}

#ifdef SFML_SYSTEM_WINDOWS

// FIXME Do we still need this?
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    return main();
}

#endif
