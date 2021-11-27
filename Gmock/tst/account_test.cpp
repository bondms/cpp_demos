// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include "Gmock/lib/account.h"

namespace {

class Notifier_Mock :
    public Notifier_Interface {
 public:
    MOCK_METHOD1(deposit_made, void(unsigned int));
    MOCK_METHOD1(withdrawal_made, void(unsigned int));
};

}  // namespace

TEST(AccountTests, simple) {
    Notifier_Mock notifier{};

    testing::InSequence in_sequence{};

    Account account{notifier};
    EXPECT_EQ(0, account.balance());

    EXPECT_CALL(notifier, deposit_made(5));
    account.deposit(5);
    EXPECT_EQ(5, account.balance());

    EXPECT_CALL(notifier, deposit_made(10));
    account.deposit(10);
    EXPECT_EQ(15, account.balance());

    EXPECT_CALL(notifier, withdrawal_made(12));
    account.withdraw(12);
    EXPECT_EQ(3, account.balance());

    EXPECT_THROW(account.withdraw(5), std::exception);
    EXPECT_EQ(3, account.balance());
}
