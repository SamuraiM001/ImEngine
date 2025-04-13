#include <iostream>
#include "ConsoleLog.h"

namespace IE {

    Log& Log::Get() {
        static Log instance;
        return instance;
    }

    void Log::Hook() {
        RegisterCommand("echo", "Prints the arguments", [](const std::string& input) {
            std::string msg = input.substr(input.find(' ') + 1);
            std::cout << msg << std::endl;
            });

        RegisterCommand("clear", "Clears the debug log", [](const std::string&) {
            Log::Get().Clear();
            });

        RegisterCommand("help", "Lists all available commands", [](const std::string&) {
            std::cout << "Available commands:\n";
            for (const auto& cmd : Log::Get().GetCommands()) {
                std::cout << " - " << cmd.name << ": " << cmd.description << "\n";
            }
            });

        RegisterCommand("sayhi", "Prints a greeting message", [](const std::string&) {
            std::cout << "Hello from IE Engine!\n";
            });

        m_Original = std::cout.rdbuf(this);
    }

    void Log::RegisterCommand(const std::string& name, const std::string& description, CommandFn fn) {
        m_Commands.push_back({ name, description, fn });
    }

    const std::vector<Log::Command>& Log::GetCommands() const {
        return m_Commands;
    }

    void Log::ExecuteCommand(const std::string& input) {
        if (input.empty()) return;

        // Extract command name (first word)
        std::istringstream iss(input);
        std::string commandName;
        iss >> commandName;

        for (const auto& cmd : m_Commands) {
            if (cmd.name == commandName) {
                cmd.action(input);
                return;
            }
        }

        std::cout << "Unknown command: " << commandName << std::endl;
    }

    // Optional: constructor for built-in commands

}
