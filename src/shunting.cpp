#include "shunting.hpp"

/// @brief 
/// @param op 
/// @return 
char precedence(std::string op)
{
    if (op.size() != 1)
    {
        return 0;
    }

    switch(*(op.c_str()))
    {
        case '^':
            return 4;
        case '*':
        case '/':
            return 3;
        case '+':
        case '-':
            return 2;
        default:
            return 1;
    }
}

/// @brief 
/// @param o1 
/// @param o2 
/// @return 
char prec_check(std::string o1, std::string o2)
{
    bool check1 = o2 != "(";
    bool check2 = precedence(o2) > precedence(o1);
    bool check3 = precedence(o2) == precedence(o1) && o1 != "^";

    return check1 && (check2 || check3);
}

/// @brief 
/// @param expr 
/// @return 
std::string punctuate(std::string expr)
{
    std::string output_expr;
    for (char c : expr)
    {
        switch(OPTOKENS.find(c))
        {
            case std::string::npos:
                output_expr.push_back(c);
                break;

            default:
                output_expr.push_back(' ');
                output_expr.push_back(c);
                output_expr.push_back(' ');
                break;
        }
    }

    return output_expr;
}

/// @brief 
/// @param expr 
/// @param result 
/// @return 
bool try_parse_double(std::string expr, double& result)
{
    result = strtod(expr.c_str(), NULL);
    if (result == 0)
    {
        for (char c : expr)
        {
            if (c != '0' && c != '.')
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<Token> rpnify(std::string expr, ContextMap ctx)
{
    // Get space-delimited vector of words in the expression
    std::stringstream ss(punctuate(expr));
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> words(begin, end);

    // Initialize stack and queue
    std::vector<std::string> stack;
    std::vector<Token> queue;
    bool minus_is_unary = true;

    for (auto word : words)
    {
        if (word == ",")
        {
            while (stack.size() != 0)
            {
                if (stack.back() == "(")
                {
                    stack.pop_back();
                    break;
                }
                queue.push_back(tokenize_with_context(move(stack.back()), ctx));
                stack.pop_back();
            }
            minus_is_unary = true;
        }
        else if (word == "(")
        {
            stack.push_back(word);
            minus_is_unary = false;
        }
        else if (word == ")")
        {
            while (stack.size() != 0)
            {
                if (stack.back() == "(")
                {
                    stack.pop_back();
                    break;
                }
                else
                {
                    queue.push_back(tokenize_with_context(word, ctx));
                    stack.pop_back();
                }
            }
            minus_is_unary = true;
        }
        else if (word == "+" || word == "-" || word == "*" || word == "/" || word == "^")
        {
            std::string o1 = word;

            if (minus_is_unary && o1 == "-")
            {
                queue.push_back(num(-1.0));
                stack.push_back("*");
                // do not specify `minus_is_unary`
                // minus will be unary following a 
                // unary minus for double negation.
            }
            else
            {
                while (stack.size() != 0)
                {
                    std::string o2 = stack.back();
                    stack.pop_back();
                    if (prec_check(o1, o2))
                    {
                        queue.push_back(tokenize_with_context(o2, ctx));
                    }
                    else
                    {
                        stack.push_back(o2);
                        break;
                    }
                }
                minus_is_unary = true;
            }
        }
        else // Token is either a yet-to-be-parsed number literal, a ctx-specified token, or garbage
        {
            auto in_ctx = ctx.find(word);
            double num_literal;

            if (in_ctx != ctx.end())
            {
                queue.push_back(in_ctx->second);
                minus_is_unary = (in_ctx->second.type == Func); // minus will be unary following an individual function token 
            }
            else if (try_parse_double(word, num_literal))
            {
                queue.push_back(num(num_literal));
            }
            else
            {
                throw; // TODO
            }
        }
    }

    while (stack.size() != 0)
    {
        if (stack.back() == "(" || stack.back() == ")")
        {
            throw; // TODO
        }
        queue.push_back(tokenize_with_context(stack.back(), ctx));
        stack.pop_back();
    }

    return queue;
}

