#pragma once
#include "Common.h"
#include "Interpreter.h"
#include "Parser.h"
#include "Resolver.h"
#include "Scanner.h"

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Runner
{
    inline static Interpreter interpreter_ = {};
    inline static bool hadError_ = false;
    inline static bool hadRuntimeError_ = false;

public:
    static Object Run(const std::string& source)
    {
        Scanner scanner(source);
        auto& tokens = scanner.ScanTokens();
        Parser parser(tokens);
        auto statements = parser.Parse();

        // Syntax error.
        if (hadError_)
            return {};

        Resolver resolver(interpreter_);
        resolver.Resolve(statements);

        // Resolution error.
        if (hadError_)
            return {};

        interpreter_.Interpret(statements);
        return interpreter_.Result();
    }

    static void RunPrompt()
    {
        std::string source = {};

        while (true)
        {
            std::cout << "> ";
            std::getline(std::cin, source);

            Run(source);

            hadError_ = false;
        }
    }

    static void Reset()
    {
        interpreter_.Reset();
    }

    static void Report(size_t line, const std::string& where, const std::string& msg)
    {
        std::cout << std::format("[line {}] Error {}: {}\n", line, where, msg);
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
        std::cout << std::format("[Line {}]: {}", err.token.Line(), err.what());
        hadRuntimeError_ = true;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core