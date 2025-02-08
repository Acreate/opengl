#include <iostream>
#include <tools/io.h>
#include <tools/path.h>
#include <cmake_include_to_c_cpp_header_env.h>
#include <source_location>
#include <stacktrace>
DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );

void out( ) {
	auto currentsacktrace = std::stacktrace::current( );
	std::stringstream ss;
	ss << "\n";
	for( auto &entry : currentsacktrace ) {
		auto description = entry.description( );
		auto sourceFile = entry.source_file( );

		auto sourceLine = entry.source_line( );
		auto nativeHandle = entry.native_handle( );

		ss << "=============\n";
		ss << "description = " << description << "\n";
		ss << "sourceFile = " << sourceFile << "\n";
		ss << "sourceLine = " << sourceLine << "\n";
		ss << "----------->\n";
		ss << "entry = " << entry << "\n";
		ss << "=============\n";
	}
	Printer_Normal_Info( ss.str( ) );
	ss = std::stringstream( );
	ss << "\n";
	ss << "=============\n";
	auto sourceLocation = std::source_location::current( );
	ss << "file: "
		<< sourceLocation.file_name( ) << "("
		<< sourceLocation.line( ) << ":"
		<< sourceLocation.column( ) << ") `"
		<< sourceLocation.function_name( ) << "\n";
	ss << "=============\n";
	Printer_Normal_Info( ss.str( ) );
}

int main( int argc, char **argv ) {

	out( );
	
	out( );
	exit( EXIT_SUCCESS ); // 安全退出
}
