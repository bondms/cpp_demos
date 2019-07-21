#include "../app/Consumer.h"
#include "../app/SimpleEvent.h"

#include <gmock/gmock.h>

#include <stdexcept>

using namespace ConsumerProducer;

TEST(ConsumerTest, DefaultConstructed)
{
    Consumer<bool> consumer{};
}

TEST(ConsumerTest, Empty)
{
    unsigned int count{ 0 };

    Consumer<bool> consumer{ [&](bool) {
         ++count;
    } };

    EXPECT_EQ(0, count);
}

TEST(ConsumerTest, Simple)
{
    unsigned int true_count{ 0 };
    unsigned int false_count{ 0 };
    
    SimpleEvent simpleEvent{};

    Consumer<bool> consumer{ [&](bool b) {
        if (b)
        {
            ++true_count;
        }
        else
        {
            ++false_count;
            simpleEvent.Set();
        }
    } };

    consumer.push(true);
    consumer.push(true);
    consumer.push(false);

    simpleEvent.Wait();

    EXPECT_EQ(2, true_count);
    EXPECT_EQ(1, false_count);
}

// Use the --gtest_filter=ConsumerTest.DISABLED_TerminatesOnException --gtest_also_run_disabled_tests arguments to run this test.
// The test should terminate rather than complete with either a pass or fail.
TEST(ConsumerTest, DISABLED_TerminatesOnException)
{
    try
    {
        Consumer<bool> consumer{ [](bool) {
            throw std::runtime_error("Test exception");
        } };

        consumer.push(true);

        SimpleEvent simpleEvent{};
        simpleEvent.Wait();
    }
    catch (const std::exception &)
    {
    }
    ADD_FAILURE() << "Program execution should not reach here";
}
