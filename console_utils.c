#include "console_utils.h"
#include <windows.h>

void setup_console_encoding() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}