# require a sane minimum version of cmake
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

# build debug by default with single-config generators
# must be done *before* first call to project()
set(CMAKE_BUILD_TYPE_INIT Debug)

# disable in-source builds https://stackoverflow.com/questions/1208681/with-cmake-how-would-you-disable-in-source-builds
# must be done *before* first call to project()
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# setup base project
project(SimulationFramework)

# provide a list of config choices for single-config generators when using cmake-gui/ccmake
if(DEFINED CMAKE_BUILD_TYPE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo)
endif()

# set up sane default C++ standard settings for all projects
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# tell cmake where to find included .cmake files
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

# setup glad to generate code for OpenGL 3.3
# remember: CACHE INTERNAL implies FORCE!
set(GLAD_API gl=3.3 CACHE INTERNAL "")
set(GLAD_EXPORT ON CACHE INTERNAL "")
set(GLAD_EXTENSIONS GL_EXT_texture_compression_s3tc,GL_EXT_texture_sRGB,GL_EXT_texture_filter_anisotropic CACHE INTERNAL "") # https://www.khronos.org/opengl/wiki/Ubiquitous_Extension
set(GLAD_GENERATOR c CACHE INTERNAL "")
set(GLAD_INSTALL OFF CACHE INTERNAL "")
set(GLAD_NO_LOADER ON CACHE INTERNAL "")
set(GLAD_PROFILE core CACHE INTERNAL "")
set(GLAD_SPEC gl CACHE INTERNAL "")
set(GLAD_REPRODUCIBLE ON CACHE INTERNAL "")
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE INTERNAL "")
set(GLAD_ALL_EXTENSIONS OFF CACHE INTERNAL "")
set(GLAD_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/glad" CACHE INTERNAL "")
add_subdirectory(glad)

# add OpenGLObjects library
add_subdirectory(OpenGLObjects)

# set up folders for better manageability in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(
	glad glad-generate-files OpenGLObjects
	PROPERTIES
	FOLDER Libraries
)

# setup CMAKE_PREFIX_PATH to use included copy of Eigen by default
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/eigen/share/eigen3/cmake")
find_package(Eigen3 REQUIRED)

# find required Qt components
set(_QT_COMPONENTS Core Gui Widgets OpenGLWidgets)
include(SetupQtPrefixPath)
find_package(Qt6 6.2 REQUIRED COMPONENTS ${_QT_COMPONENTS} CONFIG)
if(Qt6_VERSION VERSION_GREATER_EQUAL 6.3)
	qt_standard_project_setup()
else()
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	find_package(Qt6 6.2 REQUIRED COMPONENTS Tools CONFIG)
endif()

# add an executable target and make it the default debug/startup project on VS
qt_add_executable(${PROJECT_NAME}
    Prog1_2Renderer.hpp
    Prog1_2Renderer.cpp)
set_directory_properties(
	PROPERTIES
	VS_STARTUP_PROJECT ${PROJECT_NAME}
)

if(WIN32)
	# not a console application
	set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE ON)

	# add a pretty executable icon
	target_sources(
		${PROJECT_NAME}
		PRIVATE
		icon.rc
	)
endif()

# set target sources
target_sources(
	${PROJECT_NAME}
	PRIVATE
	main.cpp
	OpenGLWidget.cpp OpenGLWidget.hpp
	OpenGLRenderer.hpp
	GLMainWindow.cpp GLMainWindow.hpp GLMainWindow.ui
	ExampleRenderer.cpp ExampleRenderer.hpp
	Prog1_1Simulation.cpp Prog1_1Simulation.hpp
	Prog1_2Simulation.cpp Prog1_2Simulation.hpp
	Prog1_3Simulation.cpp Prog1_3Simulation.hpp
	helpers.hpp helpers.cpp
	constants.hpp
	shaders/icosphere.vert shaders/icosphere.frag
	shaders/skybox.vert shaders/skybox.frag
	icon.qrc
	textures.qrc
)

# set target requirements
target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	Eigen3::Eigen
	glad
	OpenGLObjects
)

# set install folder
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# link required Qt components
foreach(_COMP ${_QT_COMPONENTS})
	target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		Qt6::${_COMP}
	)
endforeach()

# # copy/install required dlls
if(WIN32)
	target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6Gui_PRIVATE_INCLUDE_DIRS})
	add_custom_command(
		TARGET ${PROJECT_NAME}
		COMMAND
			Qt6::windeployqt
			$<TARGET_FILE_NAME:${PROJECT_NAME}>
			--no-translations
			--no-opengl-sw
			--no-system-d3d-compiler
			--no-compiler-runtime
		WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
endif()

# setup automatic processing of Qt files
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC ON)
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}
)

source_group("Form Files" REGULAR_EXPRESSION "\\.ui$")
source_group("Resource Files" REGULAR_EXPRESSION "\\.q?rc$")

# setup shaders as qrc
include(ShadersToQRC)
shaders_to_qrc()

add_subdirectory(tests)
