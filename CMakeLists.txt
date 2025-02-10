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
set( CMAKE_C_EXTENSIONS ON )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_VISIBILITY_PRESET hidden )
set( CMAKE_CXX_EXTENSIONS ON )

supper_cmake_builder_language( _cmake_supper_language_list ) # # 语言支持列表
append_source_file_extensions( LANGUAGES C CXX SUFFIXS h hpp hc H HPP HC ui UI ) # # 追加后缀名

include( GenerateExportHeader )
include( GNUInstallDirs )
get_cmake_separator( cmake_sep_char )

set( Project_Run_Bin_Path "${CMAKE_HOME_DIRECTORY}/builder/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_CXX_COMPILER_ID}_bin/" )
set( Project_Run_Pbd_Path "${CMAKE_HOME_DIRECTORY}/builder/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_CXX_COMPILER_ID}_pbd/" )
set( Project_Run_Static_Lib_Path "${CMAKE_HOME_DIRECTORY}/builder/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_CXX_COMPILER_ID}_lib/" )
set( Project_Install_Path "${CMAKE_HOME_DIRECTORY}/builder/install/" )

add_subdirectory_tools_lib()

add_subdirectory_code_imgui_project() # # 包含 imgui 学习裤

get_path_cmake_dir_path( bin_list "${CMAKE_CURRENT_SOURCE_DIR}/srcs/video sources/imgui/" "CMakeLists.txt" )

list( APPEND out_path_list ${lib_list} ${bin_list} )

set( libs_path "${CMAKE_CURRENT_SOURCE_DIR}/srcs/libs/" )

filter_path_repetition( list_result out_path_list )
append_sub_directory_cmake_project_path_list( list_result )

configure_all_target()
