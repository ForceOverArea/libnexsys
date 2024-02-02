#include "shunting.hpp"

using std::function;
using std::istream_iterator;
using std::move;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

namespace nexsys
{
    /// @brief 
    /// @param op 
    /// @return 
    static char precedence(string op)
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
    static char prec_check(string o1, string o2)
    {
        bool check1 = o2 != "(";
        bool check2 = precedence(o2) > precedence(o1);
        bool check3 = precedence(o2) == precedence(o1) && o1 != "^";

        return check1 && (check2 || check3);
    }

    /// @brief 
    /// @param expr 
    /// @return 
    static string punctuate(string expr)
    {
        string output_expr;
        for (char c : expr)
        {
            switch(OPTOKENS.find(c))
            {
                case string::npos:
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

    extern bool try_parse_double(string expr, double& result);

    static vector<Token> rpnify(string expr, ContextMap ctx)
    {
        // Get space-delimited vector of words in the expression
        stringstream ss(punctuate(expr));
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> words(begin, end);

        // Initialize stack and queue
        vector<string> stack;
        vector<Token> queue;
        bool minus_is_unary = true;

        for (auto word: words)
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
                string o1 = word;

                if (minus_is_unary && o1 == "-")
                {
                    queue.push_back(Token::num(-1.0));
                    stack.push_back("*");
                    // do not specify `minus_is_unary`
                    // minus will be unary following a 
                    // unary minus for double negation.
                }
                else
                {
                    while (stack.size() != 0)
                    {
                        string o2 = stack.back();
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
                    minus_is_unary = (in_ctx->second.get_type() == Func); // minus will be unary following an individual function token 
                }
                else if (try_parse_double(word, num_literal))
                {
                    queue.push_back(Token::num(num_literal));
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

    /// @brief Helper type sized to temporarily store any value contained in a `Token`.
    union _TokenSized
    { 
        double _double; 
        Variable* _ptr;
        double (*_func)(double[]);
        size_t _uint;
    };

    /// @brief Evaluates a compiled reverse polish notation expression
    /// @param rpn_expr The reverse polish notation expression as a `std::vector<Token>`
    /// @return the value of the expression as a `double`
    static double eval_rpn_expression(vector<Token> rpn_expr)
    {
        vector<double> stack;
        vector<double> args;

        // Storage for funcs, args, etc
        _TokenSized _temp1, _temp2;

        for (Token tok: rpn_expr)
        {
            switch(tok.get_type())
            {
                case Num:
                    (void)tok.try_unwrap_num(_temp1._double);
                    stack.push_back(_temp1._double);
                    break;

                case Var:
                    (void)tok.try_unwrap_var(_temp1._ptr);
                    stack.push_back(_temp1._ptr->get_value());
                    break;

                case Plus:
                    if (stack.size() < 2)
                    {
                        throw; // TODO
                    }
                    _temp2._double = move(stack.back());
                    stack.pop_back();
                    _temp1._double = move(stack.back());
                    stack.pop_back();
                    stack.push_back(_temp1._double + _temp2._double);
                    break;

                case Minus:
                    if (stack.size() < 2)
                    {
                        throw; // TODO
                    }
                    _temp2._double = move(stack.back());
                    stack.pop_back();
                    _temp1._double = move(stack.back());
                    stack.pop_back();
                    stack.push_back(_temp1._double - _temp2._double);
                    break;

                case Mul:
                    if (stack.size() < 2)
                    {
                        throw; // TODO
                    }
                    _temp2._double = move(stack.back());
                    stack.pop_back();
                    _temp1._double = move(stack.back());
                    stack.pop_back();
                    stack.push_back(_temp1._double * _temp2._double);
                    break;

                case Div:
                    if (stack.size() < 2)
                    {
                        throw; // TODO
                    }
                    _temp2._double = move(stack.back());
                    stack.pop_back();
                    _temp1._double = move(stack.back());
                    stack.pop_back();
                    stack.push_back(_temp1._double / _temp2._double);
                    break;

                case Exp:
                    if (stack.size() < 2)
                    {
                        throw; // TODO
                    }
                    _temp2._double = move(stack.back());
                    stack.pop_back();
                    _temp1._double = move(stack.back());
                    stack.pop_back();
                    stack.push_back(powl(_temp1._double,_temp2._double));
                    break;

                case Func:
                    (void)tok.try_unwrap_func(_temp1._uint, _temp2._func);
                    if (stack.size() < _temp1._uint)
                    {
                        throw; // TODO
                    }
                    
                    while (args.size() < _temp1._uint)
                    {
                        args.push_back(move(stack.back()));
                        stack.pop_back();
                    }

                    stack.push_back(_temp2._func(args.data()));
                    args.clear();
                    break;

                default:
                    throw; // TODO
            }
        }
        if (stack.size() != 1)
        {
            throw; // TODO
        }
        return stack.back();
    }

    function<double (unordered_map<string, double>)> compile_to_function_of_umap(string expr, ContextMap ctx)
    {
        auto compiled_expr = move(rpnify(expr, ctx));
        auto arg_lookup_table = ctx;

        return [arg_lookup_table, compiled_expr](unordered_map<string, double> x)
        {
            Variable *var;
            for (auto var_val: x)
            {
                auto maybe_var = arg_lookup_table.find(var_val.first);
                if (maybe_var != arg_lookup_table.end() && 
                    maybe_var->second.try_unwrap_var(var))
                {
                    *var = var_val.second;
                }
            }
            return eval_rpn_expression(compiled_expr);
        };
    }
}
