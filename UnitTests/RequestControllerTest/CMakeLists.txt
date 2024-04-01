project(RequestControllerTest)

get_source_properties(RequestController)

add_executable(${PROJECT_NAME}
    ${LIB_SOURCE}
    RequestControllerTest.cpp
    CookieFactoryTest.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${GTEST_LIBS}
    ${LIB_DEPENDS}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${LIB_API}
    ${LIB_SRC}
)

register_test(${PROJECT_NAME})
