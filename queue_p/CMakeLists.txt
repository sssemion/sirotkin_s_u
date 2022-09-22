add_library(queue_p queue_p.h queue_p.cpp)

add_executable(queue_p_test queue_p.cpp)
target_link_libraries(queue_p_test queue_p)
