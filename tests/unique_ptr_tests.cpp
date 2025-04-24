#include <smart_pointers/my_unique_ptr.hpp>
#include <gtest/gtest.h>

struct Foo {
    int value;
};

struct CountingDeleter {
    int* count;
    explicit CountingDeleter(int* c) noexcept : count(c) {}
    void operator()(Foo* p) noexcept {
        ++(*count);
        delete p;
    }
};

TEST(MyUniquePtrTest, DefaultConstructor) {
    my_unique_ptr<Foo> p;
    EXPECT_FALSE(p);
    EXPECT_EQ(p.get(), nullptr);
}

TEST(MyUniquePtrTest, PointerConstructor) {
    my_unique_ptr<Foo> p(new Foo{42});
    EXPECT_TRUE(p);
    EXPECT_NE(p.get(), nullptr);
    EXPECT_EQ(p->value, 42);
    EXPECT_EQ((*p).value, 42);
}

TEST(MyUniquePtrTest, PointerAndDeleterConstructor) {
    int delete_count = 0;
    {
        my_unique_ptr<Foo, CountingDeleter> p(new Foo{7}, CountingDeleter(&delete_count));
        EXPECT_TRUE(p);
        EXPECT_EQ(p->value, 7);
        EXPECT_EQ(delete_count, 0);
    }
    EXPECT_EQ(delete_count, 1);
}

TEST(MyUniquePtrTest, MoveConstructor) {
    my_unique_ptr<Foo> p1(new Foo{1});
    my_unique_ptr<Foo> p2(std::move(p1));
    EXPECT_FALSE(p1);
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_TRUE(p2);
    EXPECT_EQ(p2->value, 1);
}

TEST(MyUniquePtrTest, MoveAssignment) {
    my_unique_ptr<Foo> p1(new Foo{2});
    my_unique_ptr<Foo> p2;
    p2 = std::move(p1);
    EXPECT_FALSE(p1);
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_TRUE(p2);
    EXPECT_EQ(p2->value, 2);
}

TEST(MyUniquePtrTest, Release) {
    my_unique_ptr<Foo> p(new Foo{3});
    Foo* raw = p.release();
    EXPECT_FALSE(p);
    EXPECT_EQ(p.get(), nullptr);
    ASSERT_NE(raw, nullptr);
    EXPECT_EQ(raw->value, 3);
    delete raw;
}

TEST(MyUniquePtrTest, Reset) {
    int delete_count = 0;
    {
        my_unique_ptr<Foo, CountingDeleter> p(new Foo{4}, CountingDeleter(&delete_count));
        p.reset(new Foo{5});
        EXPECT_EQ(delete_count, 1);
        EXPECT_TRUE(p);
        EXPECT_EQ(p->value, 5);
        p.reset();
        EXPECT_EQ(delete_count, 2);
        EXPECT_FALSE(p);
    }
    delete_count = 0;
    Foo* foo = new Foo{6};
    {
        my_unique_ptr<Foo, CountingDeleter> p(foo, CountingDeleter(&delete_count));
        p.reset(foo);
        EXPECT_EQ(delete_count, 0);
    }
    EXPECT_EQ(delete_count, 1);
}

TEST(MyUniquePtrTest, Get) {
    Foo* raw = new Foo{8};
    my_unique_ptr<Foo> p(raw);
    EXPECT_EQ(p.get(), raw);
}

TEST(MyUniquePtrTest, BoolConversion) {
    my_unique_ptr<Foo> p;
    EXPECT_FALSE(p);
    p.reset(new Foo{9});
    EXPECT_TRUE(static_cast<bool>(p));
}

TEST(MyUniquePtrTest, SelfMoveAssignment) {
    int delete_count = 0;
    my_unique_ptr<Foo, CountingDeleter> p(new Foo{10}, CountingDeleter(&delete_count));
    Foo* raw = p.get();
    p = std::move(p);
    EXPECT_EQ(p.get(), raw);
    p.reset();
    EXPECT_EQ(delete_count, 1);
}