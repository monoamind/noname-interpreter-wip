#include <interpret/Runner.h>
#include <filesystem>

using Core::Runner;

// ---------------------------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
    if (argc == 2)
    {
        std::string fileName = argv[1];

        if (std::filesystem::exists(fileName))
        {
            Runner::RunFile(fileName);
        }
        else
        {
            std::cerr << std::format("Error: file {} is not found.\n", fileName);
            return -1;
        }

        return 0;
    }

    Runner::RunPrompt();
    return 0;
}