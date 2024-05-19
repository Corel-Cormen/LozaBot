project(RequestDataCacheTest)

get_source_properties(RequestDataCache)

add_executable(${PROJECT_NAME}
    ${LIB_SOURCE}
    JsonUtils.hpp
    RequestDataCacheTest.cpp
    CookieFactoryTest.cpp
    CookieStorageTest.cpp
    JsonParserTest.cpp
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
