#include "harness.hpp"
#include "variable.hpp"
using nexsys::Variable;
INIT_HARNESS

TEST(add_assign_does_not_go_past_bound)
{
    Variable var(0, 0, 10);
    var += 20;
    ASSERT_EQ(var.get_value(), 10)

    var -= 20;
    ASSERT_EQ(var.get_value(), 0)
}

RUN_TESTS