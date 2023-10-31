add_executable(test-glm test_glm.cpp)
target_link_libraries(test-glm PRIVATE flib)
add_executable(test-obj test_obj.cpp)
target_link_libraries(test-obj PRIVATE flib)
