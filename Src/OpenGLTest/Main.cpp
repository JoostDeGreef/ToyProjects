#include <cstdlib>
#include <memory>

#include "Data.h"
#include "UI.h"
#include "Logger.h"
#include "LoggerSinks.h"

int main(int argc, char* argv[])
{
    // initialize logging
    auto logger = Logger::Logger::Instance();
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink<Logger::Sink::Console>();
#ifdef WIN32
    logger->AddSink<Logger::Sink::WinDebug>();
#endif // WIN32

    // locate the data files and read them
    Data data{ "ToyProjects.00.data","ToyProjects.01.data","ToyProjects.02.data","ToyProjects.03.data",
               "ToyProjects.04.data","ToyProjects.05.data","ToyProjects.06.data","ToyProjects.07.data",
               "ToyProjects.08.data","ToyProjects.09.data","ToyProjects.10.data" };
    data.AddDataPath("ToyProjects.11.data");

    // initialize UI
    UserInterface ui(std::move(data));
    if (!ui.Init())
    {
        return EXIT_FAILURE;
    }

    // main loop
    ui.Run();
    ui.Cleanup();

    return 0;
}
