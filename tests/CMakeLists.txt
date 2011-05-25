project(test)

add_executable(HDS_test HDS_test.cpp)
add_test(HDS_test HDS_test)
target_link_libraries(HDS_test ${Boost_LIBRARIES} umeshu)

add_executable(Triangulation_test Triangulation_test.cpp)
add_test(Triangulation_test Triangulation_test)
target_link_libraries(Triangulation_test ${Boost_LIBRARIES} umeshu)

add_executable(Point2_test Point2_test.cpp)
add_test(Point2_test Point2_test)
target_link_libraries(Point2_test ${Boost_LIBRARIES} umeshu)
