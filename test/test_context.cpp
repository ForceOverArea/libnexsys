#include "harness.hpp"
#include "context.hpp"

using nexsys::ContextMap;
using nexsys::tokenize_with_context;
using nexsys::Token;
using nexsys::TokenType;
using nexsys::try_tokenize;
using nexsys::Variable;

INIT_HARNESS

// TODO: should probably add a getter for tokens added to the context for convenience...
TEST(contextmap_can_add_tokens_as_expected)
{
    ContextMap ctx;

    ctx.add_var_to_ctx("x");
    auto result = ctx.find("x");

    ASSERT_NE(result, ctx.end())

    Variable value;
    Variable* valptr;
    result->second.try_unwrap_var(valptr);

    ASSERT_EQ(value.get_value(), 1.0)
}

TEST(try_tokenize_creates_correct_tokens) // TODO: Something smells undefined here... test fails on different lines w/ no changes...
{
    Token tok;
    try_tokenize("+", tok);
    ASSERT_EQ(tok.get_type(), TokenType::Plus)

    Token tok2;
    try_tokenize("-", tok2);
    ASSERT_EQ(tok.get_type(), TokenType::Minus)

    try_tokenize("2.0", tok);
    ASSERT_EQ(tok.get_type(), TokenType::Num)
}

RUN_TESTS