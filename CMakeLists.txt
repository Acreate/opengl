cmake_minimum_required( VERSION 3.19 )

# # 加载模块
find_package( CmakeFuncs REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/srcs/cmakeFuncs/CmakeFuncs" )

# # 获取当前文件夹名称
get_current_dir_name( prject_name ${CMAKE_CURRENT_SOURCE_DIR} )
message( "============ ${prject_name}" )
message( "name = " ${prject_name} ) # # 当前文件名配置为项目名称
project( ${prject_name} )
message( "============ ${CURRENT_FOLDER}" )

set( CMAKE_C_STANDARD 17 )
set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_VISIBILITY_PRESET hidden )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_VISIBILITY_PRESET hidden )

supper_cmake_builder_language( _cmake_supper_language_list ) # # 语言支持列表

include( GenerateExportHeader )
include( GNUInstallDirs )

append_CXX_C_source_file_extensions( h hpp hc H HPP HC ) # # 追加后缀名

get_cmake_separator( cmake_sep_char )

set( Project_Run_Bin_Path "${CMAKE_HOME_DIRECTORY}/builder/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_CXX_COMPILER_ID}_bin/" )
set( Project_Run_Pbd_Path "${CMAKE_HOME_DIRECTORY}/builder/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_CXX_COMPILER_ID}_pbd/" )
set( Project_Run_Static_Lib_Path "${CMAKE_HOME_DIRECTORY}/builder/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_CXX_COMPILER_ID}_lib/" )
set( Project_Install_Path "${CMAKE_HOME_DIRECTORY}/builder/install/" )

add_subdirectory_tools_lib()

# # add_subdirectory_test_code_project() ## 测试工具
get_path_cmake_dir_path( lib_list "${CMAKE_CURRENT_SOURCE_DIR}/srcs/libs/users" "CMakeLists.txt" )
get_path_cmake_dir_path( bin_list "${CMAKE_CURRENT_SOURCE_DIR}/srcs/testCode/" "CMakeLists.txt" )
get_path_cmake_dir_path( bin_list "${CMAKE_CURRENT_SOURCE_DIR}/srcs/video sources/imgui/" "CMakeLists.txt" )
# #get_path_cmake_dir_path( bin_list "${CMAKE_CURRENT_SOURCE_DIR}/srcs/web/" "CMakeLists.txt" )

list( APPEND out_path_list ${lib_list} ${bin_list} )

set( libs_path "${CMAKE_CURRENT_SOURCE_DIR}/srcs/libs/" )

filter_path_repetition( list_result out_path_list )
append_sub_directory_cmake_project_path_list( list_result )

configure_all_target()
