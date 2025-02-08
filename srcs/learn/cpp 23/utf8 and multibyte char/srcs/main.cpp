#include <iostream>
#include <tools/io.h>
#include <tools/path.h>
#include <cmake_include_to_c_cpp_header_env.h>
#include <source_location>
#include <cuchar>
DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );

int main( int argc, char **argv ) {
	auto srcCh = "中文";
	auto srcLen = std::strlen( srcCh ) + 1;
	wchar_t *result = new wchar_t[ srcLen ] {0};
	std::mbstate_t ps;
	size_t size = mbrlen( srcCh, srcLen, &ps );
	std::cout << "srcCh = " << srcCh << std::endl;
	std::cout << "mbrlen = " << size << std::endl;
	
	ps = std::mbstate_t( );
	size_t mbrtowc = std::mbrtowc( result, srcCh, size, &ps );
	std::wcout << "result[0] = " << result[ 0 ] << std::endl;
	std::cout << "mbrtowc = " << mbrtowc << std::endl;
	delete[] result;
	return 0;
}
