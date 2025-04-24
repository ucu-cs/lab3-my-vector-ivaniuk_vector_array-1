find_package(GTest REQUIRED)

add_executable(vector-tests
        vector_tests.cpp
)

target_link_libraries(vector-tests
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        my_vector
)

add_executable(array-tests
        array_tests.cpp
)

target_link_libraries(array-tests
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        my_array
)

add_executable(unique-ptr-tests
        unique_ptr_tests.cpp
)

target_link_libraries(unique-ptr-tests
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        my_smart_pointers
)

include_directories(../include)

enable_testing()
add_test(NAME vector-tests COMMAND vector-tests)
add_test(NAME array-tests COMMAND array-tests)
add_test(NAME unique-ptr-tests COMMAND unique-ptr-tests)
