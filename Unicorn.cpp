#include "Unicorn.h"

int main() {
    Unicorn u1("Sparkle");
    Unicorn u2("Sparkle");   // error
    Unicorn u3("Twilight");

    {
        Unicorn u4("Sparkle"); // still error
    } // u4 dies, name freed

    Unicorn u5("Sparkle"); // now allowed

    return 0;
}
