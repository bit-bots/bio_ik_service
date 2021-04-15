project(bio_ik_service)

cmake_minimum_required(VERSION 3.5)

add_compile_options(-std=c++11)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  message("${PROJECT_NAME}: You did not request a specific build type: Choosing 'Release' for maximum performance")
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(catkin REQUIRED COMPONENTS
  roscpp
  moveit_core
  tf
  tf_conversions
  bio_ik
  bio_ik_msgs
  eigen_conversions
)

find_package(PythonLibs COMPONENTS Interpreter Development)
if(PYTHONLIBS_VERSION_STRING VERSION_LESS "3.8")
    find_package(Boost REQUIRED COMPONENTS python3)
else()
    find_package(Boost REQUIRED COMPONENTS python)
endif()

catkin_package(CATKIN_DEPENDS
  roscpp
  moveit_core
  tf
  tf_conversions
  bio_ik
  bio_ik_msgs
  eigen_conversions
)

catkin_python_setup()

add_executable(bio_ik_service src/bio_ik_service.cpp)
target_include_directories(bio_ik_service PRIVATE ${catkin_INCLUDE_DIRS})
add_dependencies(bio_ik_service ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(bio_ik_service ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(bio_ik_python SHARED src/bio_ik_python.cpp)
target_include_directories(bio_ik_python PRIVATE ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
add_dependencies(bio_ik_python ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(bio_ik_python ${catkin_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(bio_ik_python PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_PYTHON_DESTINATION}
)
