add_library(flib SHARED
    fmesh.cpp fmesh.hpp fmodel.cpp fmodel.hpp ftexture.cpp ftexture.hpp
    futils.cpp futils.hpp fimage.cpp fimage.hpp fprogram.cpp fprogram.hpp
    fid.cpp fid.hpp fcamera.cpp fcamera.hpp flight.cpp flight.hpp flib.hpp
    ftransform.cpp ftransform.hpp fscene.cpp fscene.hpp fnode.cpp fnode.hpp
    fkinetic.cpp fkinetic.hpp fapplication.cpp fapplication.hpp fsceneapp.cpp 
    fsceneapp.hpp fskybox.cpp fskybox.hpp
)

target_link_libraries(flib PUBLIC GLEW glfw glm::glm imgui assimp GL)
target_include_directories(flib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.in.h ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)
target_include_directories(flib PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)
