#----------------------------------------
# gpstk/swig/CMakeLists.txt
#----------------------------------------

cmake_minimum_required( VERSION 2.8.4  )
include( make_copy_targets.cmake )

include( PythonSetup.cmake )

find_package( SWIG REQUIRED )
include( ${SWIG_USE_FILE} )

#----------------------------------------
# Compiler warning disables
#----------------------------------------
#     362: operator= ignored
#     383: operator++ ignored
#     384: operator-- ignored
#     389: operator[] ignored
#     503: can't wrap 'identifer' unless renamed to a valid identifier (e.g. operator<<)

set( CMAKE_SWIG_FLAGS "-w362,383,384,389,503 -builtin" )

#----------------------------------------
# Don't set the RPATH for dependencies.
#----------------------------------------
set( CMAKE_SKIP_RPATH TRUE )

#----------------------------------------
# Compiler flags that are used only
# in the compiling of the swig bindings
# @todo: see if -std=c++11 can be removed from all builds
#----------------------------------------
if( ${CMAKE_SYSTEM_NAME} MATCHES "SunOS" )
    set( CMAKE_CXX_FLAGS "-std=c++11" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=c++11" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set( CMAKE_CXX_FLAGS "-std=c++11" )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( CMAKE_CXX_FLAGS "-std=c++11" )
endif()


set_source_files_properties( gpstk.i PROPERTIES CPLUSPLUS ON )

if( DEBUG_SWITCH )
    message( STATUS "SWIG_EXECUTABLE          = ${SWIG_EXECUTABLE}" )
    message( STATUS "SWIG_USE_FILE            = ${SWIG_USE_FILE}" )
    message( STATUS "SWIG_DOC_DIR             = ${SWIG_DOC_DIR}")
    message( STATUS "PYTHON_INSTALL_PREFIX    = ${PYTHON_INSTALL_PREFIX}")
    message( STATUS "PROJECT_BINARY_DIR       = ${PROJECT_BINARY_DIR}/libgpstk.so.${GPSTK_VERSION_MAJOR}.${GPSTK_VERSION_MINOR}.${GPSTK_VERSION_PATCH}")
endif()

#----------------------------------------
# Swig module build target
#----------------------------------------
# Note: the cmake swig module doesn't seem to know
#       how to track dependancy on other .i files...
#       so if the other .i files are changed,
#       you must rerun cmake, not just make.
#
# Note: list of all headers files are included in
#       gpstk_swig.hpp which is included in gpstk.i
set( SWIG_MODULE "gpstk" )

include_directories( ${PYTHON_INCLUDE_DIRS} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )

swig_add_module( ${SWIG_MODULE} python gpstk.i ) # why not also list all the other files.i here?!
swig_link_libraries( ${SWIG_MODULE} gpstk )
swig_link_libraries( ${SWIG_MODULE} ${PYTHON_LIBRARIES} )

# Install the gpstk module into the package file tree
set( MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR}/module )
set( CMAKE_SWIG_OUTDIR ${MODULE_PATH}/${SWIG_MODULE} )


#----------------------------------------
# Add custom commands to the _gpstk.so build target
#----------------------------------------
#    ...to relocate the output from teh swig build target to
#    the correct directory within the python package file tree
#        CMake documentation claims that all SWIG target output
#        will be written to $CMAKE_SWIG_OUTDIR, but
#        testing shows that this ONLY works for the gpstk.py
#        and does NOT work for the _gpstk.so
#----------------------------------------
add_custom_target(swig-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MODULE_PATH}/gpstk)
add_dependencies("_${SWIG_MODULE}" swig-make-directory)
add_custom_command( TARGET "_${SWIG_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "_${SWIG_MODULE}.so" ${CMAKE_SWIG_OUTDIR} )
add_custom_command( TARGET "_${SWIG_MODULE}" POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${SWIG_MODULE}.py" ${CMAKE_SWIG_OUTDIR} )

add_custom_command( TARGET "_${SWIG_MODULE}"
                    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/libgpstk.so.${GPSTK_VERSION}" ${CMAKE_SWIG_OUTDIR}
                    DEPENDS gpstk)

# Find all files in the gpstk, apps, and tests directories and set up dependancies to
# copy them to the python module directory
file(GLOB_RECURSE module_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/gpstk/*.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/*.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.py"
  )
make_copy_targets(module_files ${CMAKE_CURRENT_SOURCE_DIR} ${MODULE_PATH})

# Do the same for the boilerplate files
set(meta_files "" )
list(APPEND meta_files AUTHORS.md COPYING.md COPYING.LESSER.md ChangeLog.md INSTALL.md README.md )
make_copy_targets(meta_files ${CMAKE_SOURCE_DIR} ${MODULE_PATH})


#--------------------------------------------
# Initialize a working space to build the doxygen/sphinx docs
#--------------------------------------------

set( SWIG_DOC_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc )
include_directories( ${SWIG_DOC_DIR} )
file( MAKE_DIRECTORY ${SWIG_DOC_DIR} )
file( WRITE ${SWIG_DOC_DIR}/doc.i "// dummy file. Is replaced when docs are generated for bindings." )

# Provide ability to use CMake variables to define some values within the setup.py script
set( SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in" )
set( SETUP_PY    "${MODULE_PATH}/setup.py" )
configure_file( ${SETUP_PY_IN} ${SETUP_PY} )

set( GPSTK_ROOT_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/gpstk/__init__.py.in" )
set( GPSTK_ROOT_PY    "${MODULE_PATH}/gpstk/__init__.py" )
configure_file( ${GPSTK_ROOT_PY_IN} ${GPSTK_ROOT_PY} )


# Note: For setup.py install options, see https://docs.python.org/2/install/
if( BUILD_FOR_PACKAGE_SWITCH )
    # If building for a package, use the "root" switch to imitate the system package path
    install( DIRECTORY DESTINATION ${PYTHON_INSTALL_PREFIX}/lib/python2.7/site-packages )
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --root=${PYTHON_INSTALL_PREFIX} WORKING_DIRECTORY ${MODULE_PATH})" )
elseif( PYTHON_USER_INSTALL )
    # If performing a user-install, use the `--user` flag, otherwise it may not create the correct directores.
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --user WORKING_DIRECTORY ${MODULE_PATH})" )
else()
    install( DIRECTORY DESTINATION ${PYTHON_INSTALL_PREFIX}/lib/python2.7/site-packages )
    install( CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --prefix=${PYTHON_INSTALL_PREFIX} WORKING_DIRECTORY ${MODULE_PATH})" )
endif()

if( PIP_WHEEL_SWITCH )
    add_custom_target(swig-make-wheel ALL
            COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} bdist_wheel
            WORKING_DIRECTORY ${MODULE_PATH}
            DEPENDS "_${SWIG_MODULE}")
endif()


#------------------------------------------------------------
# These files aren't required to use the bindings but are
# required to make a swig module that depends upon these bindings
#------------------------------------------------------------

file( GLOB swig_includes ${CMAKE_CURRENT_SOURCE_DIR}/src/* )
install( FILES ${swig_includes} ${CMAKE_CURRENT_SOURCE_DIR}/gpstk.i DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" )


include(swig_tests.cmake)
