
#include "stdafx.h"
#include "Framework/Framework.h"

int main() {
    Framework _Framework;
    _Framework.run();

    return 0;
}

#ifdef SFML_SYSTEM_WINDOWS

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    main();
}

#endif
