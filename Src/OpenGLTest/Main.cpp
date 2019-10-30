#include <cstdlib>
#include <memory>

#include "UI.h"

int main(int argc, char* argv[])
{
    // initialize UI
    UserInterface ui;
    if (!ui.Init())
    {
        return EXIT_FAILURE;
    }

    // main loop
    ui.Run();
    ui.Cleanup();

    return 0;
}
