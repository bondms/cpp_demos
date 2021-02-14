#include "lib/notifier.h"

#include <gmock/gmock.h>
#include <sstream>

TEST(NotifierTests, deposit_made)
{
    std::ostringstream oss{};
    Notifier notifier{oss};

    notifier.deposit_made(123);

    EXPECT_EQ("Deposited: 123\n", oss.str());
}

TEST(NotifierTests, withdrawal_made)
{
    std::ostringstream oss{};
    Notifier notifier{oss};

    notifier.withdrawal_made(321);

    EXPECT_EQ("Withdrawn: 321\n", oss.str());
}
