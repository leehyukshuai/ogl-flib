add_executable(09-reflect reflect.cpp)

target_link_libraries(09-reflect PRIVATE flib)

add_executable(09-refract refract.cpp)

target_link_libraries(09-refract PRIVATE flib)
