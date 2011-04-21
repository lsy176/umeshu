project( test )

add_executable( Point2_test Point2_test.cpp )
add_test( Point2_test Point2_test )
target_link_libraries( Point2_test ${Boost_LIBRARIES} umeshu)
