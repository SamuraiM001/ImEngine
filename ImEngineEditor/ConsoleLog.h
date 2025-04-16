#pragma once
#include <streambuf>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <vector>

namespace IE {

    class Log : public std::streambuf {
    public:
        using CommandFn = std::function<void(const std::string&)>;

        struct Command {
            std::string name;
            std::string description;
            CommandFn action;
        };

        static Log& Get();
        void RegisterCommand(const std::string& name, const std::string& description, CommandFn fn);
        const std::vector<Command>& GetCommands() const;

        void ExecuteCommand(const std::string& input);
        void Hook();

        void Unhook() {
            if (m_Original)
                std::cout.rdbuf(m_Original);
        }

        const std::string& GetBuffer() const {
            return m_DebugBuffer;
        }

        void Clear() {
            m_DebugBuffer.clear();
        }

    protected:
        std::streamsize xsputn(const char* s, std::streamsize n) override {
            m_DebugBuffer.append(s, n);
            return n;
        }

        int overflow(int ch) override {
            if (ch != EOF) {
                m_DebugBuffer += static_cast<char>(ch);
            }
            return ch;
        }

    private:
        std::vector<Command> m_Commands;
        std::string m_DebugBuffer;
        std::streambuf* m_Original = nullptr;
    };
}
