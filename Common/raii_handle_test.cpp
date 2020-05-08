#include "../app/raii_handle.h"

#include <gtest/gtest.h>

#include <cassert>
#include <utility>

namespace
{
    class MockClass
    {
        bool freed_{ false };
    public:
        void free() noexcept
        {
            freed_ = true;
        };

        bool freed() const noexcept
        {
            return freed_;
        }

        void reset() noexcept
        {
            freed_ = false;
        }
    };

    using MockHandle = MockClass*;

    void mockFree(MockHandle mockHandle) noexcept
    {
        assert(nullptr != mockHandle);
        EXPECT_FALSE(mockHandle->freed()) << "Double free";
        mockHandle->free();
    }

    using MockRaiiHandle = utilities::RaiiHandle<MockHandle, mockFree>;

    class RaiiHandleTestFixture :
        public testing::Test
    {
    public:
        MockClass mockClass_{};
        void SetUp() override
        {
            mockClass_.reset();
        }
        void mockAllocate(MockHandle * pMockHandle)
        {
            *pMockHandle = &mockClass_;
        }
    };

} // namespace

TEST_F(RaiiHandleTestFixture, DefaultConstructable)
{
    MockRaiiHandle mockRaiiHandle{};
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());
}

TEST_F(RaiiHandleTestFixture, Simple)
{
    MockClass mockClass{};
    EXPECT_FALSE(mockClass.freed());
    {
        MockRaiiHandle mockRaiiHandle{&mockClass};
        EXPECT_TRUE(mockRaiiHandle.isValid());
        EXPECT_FALSE(mockClass.freed());
        EXPECT_EQ(&mockClass, mockRaiiHandle.handle());
    }
    EXPECT_TRUE(mockClass.freed());
}

TEST_F(RaiiHandleTestFixture, MoveAssignment)
{
    MockClass mockClass{};
    {
        MockRaiiHandle mockRaiiHandle1{&mockClass};
        MockRaiiHandle mockRaiiHandle2{};

        EXPECT_TRUE(mockRaiiHandle1.isValid());
        EXPECT_EQ(&mockClass, mockRaiiHandle1.handle());
        EXPECT_FALSE(mockRaiiHandle2.isValid());
        EXPECT_EQ(nullptr, mockRaiiHandle2.handle());
        EXPECT_FALSE(mockClass.freed());

        mockRaiiHandle2 = std::move(mockRaiiHandle1);

        EXPECT_FALSE(mockRaiiHandle1.isValid());
        EXPECT_EQ(nullptr, mockRaiiHandle1.handle());
        EXPECT_TRUE(mockRaiiHandle2.isValid());
        EXPECT_EQ(&mockClass, mockRaiiHandle2.handle());
        EXPECT_FALSE(mockClass.freed());
    }
    EXPECT_TRUE(mockClass.freed());
}

TEST_F(RaiiHandleTestFixture, MoveAssignmentFromSelf)
{
    MockClass mockClass{};
    {
        MockRaiiHandle mockRaiiHandle1{&mockClass};

        EXPECT_TRUE(mockRaiiHandle1.isValid());
        EXPECT_EQ(&mockClass, mockRaiiHandle1.handle());
        EXPECT_FALSE(mockClass.freed());

        mockRaiiHandle1 = std::move(mockRaiiHandle1);

        EXPECT_TRUE(mockRaiiHandle1.isValid());
        EXPECT_EQ(&mockClass, mockRaiiHandle1.handle());
        EXPECT_FALSE(mockClass.freed());
    }
    EXPECT_TRUE(mockClass.freed());
}

TEST_F(RaiiHandleTestFixture, MoveConstruction)
{
    MockClass mockClass{};
    {
        MockRaiiHandle mockRaiiHandle1{&mockClass};

        EXPECT_TRUE(mockRaiiHandle1.isValid());
        EXPECT_EQ(&mockClass, mockRaiiHandle1.handle());
        EXPECT_FALSE(mockClass.freed());

        MockRaiiHandle mockRaiiHandle2{ std::move(mockRaiiHandle1) };

        EXPECT_FALSE(mockRaiiHandle1.isValid());
        EXPECT_EQ(nullptr, mockRaiiHandle1.handle());
        EXPECT_TRUE(mockRaiiHandle2.isValid());
        EXPECT_EQ(&mockClass, mockRaiiHandle2.handle());
        EXPECT_FALSE(mockClass.freed());
    }
    EXPECT_TRUE(mockClass.freed());
}

TEST_F(RaiiHandleTestFixture, Free)
{
    MockClass mockClass{};
    EXPECT_FALSE(mockClass.freed());

    MockRaiiHandle mockRaiiHandle{&mockClass};
    EXPECT_TRUE(mockRaiiHandle.isValid());
    EXPECT_EQ(&mockClass, mockRaiiHandle.handle());
    EXPECT_FALSE(mockClass.freed());

    mockRaiiHandle.free();
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());
    EXPECT_TRUE(mockClass.freed());   
}

TEST_F(RaiiHandleTestFixture, FreeOnInvalid)
{
    MockRaiiHandle mockRaiiHandle{};
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());

    mockRaiiHandle.free();
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());
}

TEST_F(RaiiHandleTestFixture, Detach)
{
    MockClass mockClass{};
    EXPECT_FALSE(mockClass.freed());

    MockRaiiHandle mockRaiiHandle{&mockClass};
    EXPECT_TRUE(mockRaiiHandle.isValid());
    EXPECT_EQ(&mockClass, mockRaiiHandle.handle());
    EXPECT_FALSE(mockClass.freed());

    const auto rawHandle{ mockRaiiHandle.detach() };
    EXPECT_EQ(&mockClass, rawHandle);
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());
    EXPECT_FALSE(mockClass.freed());

    mockFree(rawHandle);
    EXPECT_TRUE(mockClass.freed());
}

TEST_F(RaiiHandleTestFixture, DetachOnInvalid)
{
    MockRaiiHandle mockRaiiHandle{};
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());

    EXPECT_EQ(nullptr, mockRaiiHandle.detach());
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());
}

TEST_F(RaiiHandleTestFixture, GetRef)
{
    MockRaiiHandle mockRaiiHandle{};
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());

    mockAllocate(&mockRaiiHandle.getHandleReference());
    EXPECT_TRUE(mockRaiiHandle.isValid());
    EXPECT_EQ(&mockClass_, mockRaiiHandle.handle());
}

// TODO(MarkBond): Test with different template arguments, e.g. invalidValue something other than nullptr.
// TODO(MarkBond): Consider implementing comparison operators (e.g. < to enable use in std::set).
