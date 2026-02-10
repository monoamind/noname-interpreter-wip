#pragma once
#include "Interpreter.h"
#include "Parser.h"
#include "Scanner.h"

#include <iostream>

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Runner
{
    inline static Interpreter interpreter = {};

    inline static bool hadError_ = false;
    inline static bool hadRuntimeError_ = false;

public:
    static void Run(const std::string& source)
    {
        Scanner scanner(source);
        auto& tokens = scanner.ScanTokens();
        Parser parser(tokens);
        auto statements = parser.Parse();

        if (hadError_)
            return;

        interpreter.Interpret(statements);
    }

    static void RunPrompt()
    {
        std::string source = {};

        while (true)
        {
            std::cout << "\n> ";
            std::getline(std::cin, source);
            Run(source);
            hadError_ = false;
        }
    }

    static void Report(size_t line, const std::string& where, const std::string& msg)
    {
        std::cerr << std::format("[line {}] Error {}: {}", line, where, msg);
        hadError_ = true;
    }

    static void Error(size_t line, const std::string& msg)
    {
        Report(line, "", msg);
    }

    static void Error(const Token& token, const std::string& msg)
    {
        if (token.IsEof())
        {
            Report(token.Line(), "at end", msg);
            return;
        }

        Report(token.Line(), "at '" + token.Lexeme() + "'", msg);
    }

    static void RuntimeError(const RuntimeError& err)
    {
        std::cout << std::format("{}\n[line {}]", err.what(), err.token.Line());
        hadRuntimeError_ = true;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core