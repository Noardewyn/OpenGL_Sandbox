macro(conan_get_dependencies _conanfile_path _install_dir)

message(STATUS "\n\n-----> Conan - Installing dependencies...")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CONAN_COMPILER "clang")
    set(CONAN_ADDITIONAL_ARGS ${CONAN_ADDITIONAL_ARGS} -s compiler.libcxx=libstdc++)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CONAN_COMPILER "gcc")
    set(CONAN_ADDITIONAL_ARGS ${CONAN_ADDITIONAL_ARGS} -s compiler.libcxx=libstdc++)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CONAN_COMPILER "intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CONAN_COMPILER "Visual Studio")
endif()

# This is needed to correct search Modules for find_package
set(CMAKE_MODULE_PATH ${_install_dir})

execute_process(COMMAND conan install ${_conanfile_path} 
                              --build missing 
                              -s build_type=${CMAKE_BUILD_TYPE}
                              -s compiler=${CONAN_COMPILER}
                              -s compiler.version=${CONAN_COMPILER_VERSION}
                              ${CONAN_ADDITIONAL_ARGS}
                RESULT_VARIABLE return_code				 
                WORKING_DIRECTORY ${_install_dir}
                )

if(return_code)
    message(FATAL_ERROR "\n\n-----> Conan return error status: " ${return_code})
endif()

include(${_install_dir}/conanbuildinfo.cmake)
conan_basic_setup()
conan_set_vs_runtime()
conan_set_libcxx()
conan_output_dirs_setup()

message(STATUS "-----> Conan - Installing dependencies - done.\n\n")

endmacro()
