#include <tools/io.h>
#include <tools/path.h>
#include "../auto_generate_files/macro/cmake_include_to_c_cpp_header_env.h"
DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );
#define BUFFER_OFFSET(x)  ((const void*) (x))
#include <iostream>
using namespace std;

#include "stdio.h"
DEF_ALL_CMAKE_PROPERTIES_VALUE_STD_VECTOR( );
DEF_ALL_CMAKE_PROPERTIES_VALUE_GET_VALID_PAIRS_STD_VECTOR( );
int main( int argc, char **argv ) {
	auto pairs = getValidCmakePropertyPairs( );
	for( auto &pair : pairs )
		std::cout << pair.first << ":" << pair.second << std::endl;
}
