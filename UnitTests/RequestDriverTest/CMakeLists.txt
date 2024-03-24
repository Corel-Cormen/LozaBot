project(RequestDriverTest)

add_executable(${PROJECT_NAME}
    RequestDriverTest.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${GTEST_LIBS}
    RequestDriver
)

register_test(${PROJECT_NAME})
