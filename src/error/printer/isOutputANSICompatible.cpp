//
// Created by kemal on 9/16/2024.
//

#include <windows.h>

/* Checks if the output / console / terminal is ANSI compatible.
 * It isn't able to be a method of the ErrorPrinter class because it uses windows.h which creates a conflict with several
 * names in the project such as TokenType and ERROR_INDEX_OUT_OF_BOUNDS. Separating it like this seems to fix the issue
 * and allows the project to compile. Probably could have been fixed in some way by picking out certain includes in the
 * class, but it works like this so its best not to modify it.
 */
bool isOutputANSICompatible() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hConsole, &dwMode)) {
        return false;
    }

    return (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}