#include "Parser.h"
#include "Runner.h"
#include <iostream>

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

Ptr<ExprAst> Parser::Parse()
{
    try
    {
        return Expression();
    }
    catch (TypeError& e)
    {
        std::cerr << e.what();
        return nullptr;
    }
}

TypeError Parser::RaiseError(const Token& token, const std::string& msg)
{
    Runner::Error(token, msg);
    return TypeError(msg);
}