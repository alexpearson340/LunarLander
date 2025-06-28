if(CMAKE_HOST_WIN32 )
  option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" ON)
endif(CMAKE_HOST_WIN32 )

add_executable(
  lunar_lander_tests
  test_main.cpp
  test_vector_2d.cpp
)

target_link_libraries(
  lunar_lander_tests
  GTest::gtest_main
  lunar_lander_lib
)

target_include_directories(lunar_lander_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(WIN32)
  add_custom_command(
    TARGET lunar_lander_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:GTest::gtest> $<TARGET_FILE_DIR:lunar_lander_tests>
    COMMENT "Copying gtest DLL to output directory"
  )
endif()

include(GoogleTest)
gtest_discover_tests(lunar_lander_tests)