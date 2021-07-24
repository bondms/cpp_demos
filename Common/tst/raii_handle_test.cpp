// Copyright 2021 Mark Bond

#include "lib/raii_handle.h"

#include <gmock/gmock.h>

#include <cassert>
#include <set>
#include <utility>
#include <vector>

using testing::ElementsAre;

namespace {

class MockClass {
    bool freed_{ false };

 public:
    void free() noexcept {
        freed_ = true;
    };

    bool freed() const noexcept {
        return freed_;
    }

    void reset() noexcept {
        freed_ = false;
    }
};

using MockHandle = MockClass*;

void mockFree(MockHandle mockHandle) noexcept {
    assert(nullptr != mockHandle);
    EXPECT_FALSE(mockHandle->freed()) << "Double free";
    mockHandle->free();
}

using MockRaiiHandle = utilities::RaiiHandle<MockHandle, mockFree>;

class RaiiHandleTestFixture :
    public testing::Test {
 public:
    MockClass mockClass_{};
    void SetUp() override {
        mockClass_.reset();
    }
    void mockAllocate(MockHandle * pMockHandle) {
        *pMockHandle = &mockClass_;
    }
};

std::vector<int> int_mock_free_calls{};
void intMockFree(int mockHandle) noexcept {
    int_mock_free_calls.push_back(mockHandle);
}
using IntMockRaiiHandle = utilities::RaiiHandle<int, intMockFree, -1>;

}  // namespace

TEST_F(RaiiHandleTestFixture, DefaultConstructable) {
    MockRaiiHandle mockRaiiHandle{};
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());
}

TEST_F(RaiiHandleTestFixture, Simple) {
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

TEST_F(RaiiHandleTestFixture, MoveAssignment) {
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

TEST_F(RaiiHandleTestFixture, MoveAssignmentFromSelf) {
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

TEST_F(RaiiHandleTestFixture, MoveConstruction) {
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

TEST_F(RaiiHandleTestFixture, Free) {
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

TEST_F(RaiiHandleTestFixture, FreeOnInvalid) {
    MockRaiiHandle mockRaiiHandle{};
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());

    mockRaiiHandle.free();
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());
}

TEST_F(RaiiHandleTestFixture, Detach) {
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

TEST_F(RaiiHandleTestFixture, DetachOnInvalid) {
    MockRaiiHandle mockRaiiHandle{};
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());

    EXPECT_EQ(nullptr, mockRaiiHandle.detach());
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());
}

TEST_F(RaiiHandleTestFixture, GetRef) {
    MockRaiiHandle mockRaiiHandle{};
    EXPECT_FALSE(mockRaiiHandle.isValid());
    EXPECT_EQ(nullptr, mockRaiiHandle.handle());

    mockAllocate(&mockRaiiHandle.getHandleReference());
    EXPECT_TRUE(mockRaiiHandle.isValid());
    EXPECT_EQ(&mockClass_, mockRaiiHandle.handle());
}

TEST_F(RaiiHandleTestFixture, NonDefaultInvalidValue) {
    int_mock_free_calls.clear();

    {
        IntMockRaiiHandle intMockRaiiHandle{-1};
        EXPECT_FALSE(intMockRaiiHandle.isValid());
    }
    EXPECT_TRUE(int_mock_free_calls.empty());

    for ( auto i = 0 ; i < 3 ; ++i ) {
        IntMockRaiiHandle intMockRaiiHandle{i};
        EXPECT_TRUE(intMockRaiiHandle.isValid());
    }
    EXPECT_THAT(int_mock_free_calls, ElementsAre(0, 1, 2));
}

TEST_F(RaiiHandleTestFixture, UseInNonAssociativeContainers) {
    MockRaiiHandle mockRaiiHandle{};
    std::vector<MockRaiiHandle> v{};
    v.push_back(std::move(mockRaiiHandle));
}

TEST_F(RaiiHandleTestFixture, UseInAssociativeContainers) {
    MockRaiiHandle mockRaiiHandle{};
    std::set<MockRaiiHandle> s{};
    s.insert(std::move(mockRaiiHandle));
}
