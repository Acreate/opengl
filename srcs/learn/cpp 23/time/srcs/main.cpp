#include <iostream>
#include <tools/io.h>
#include <tools/path.h>
#include <cmake_include_to_c_cpp_header_env.h>
#include <chrono>
DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );

int main( int argc, char **argv ) {
	using dest_clock_type = std::chrono::utc_clock;
	using source_clock_type = std::chrono::system_clock;
	auto timePoint = source_clock_type::now( );
	std::cout << "timePoint = " << timePoint << std::endl;
	auto conver = std::chrono::clock_time_conversion< dest_clock_type, source_clock_type >( );
	std::cout << "clock_time_conversion = " << conver( timePoint ) << std::endl;

	std::cout << "=================" << std::endl;
	auto currentZone = std::chrono::current_zone( );
	auto zoneName = currentZone->name(  );
	
	auto localTime = currentZone->to_local( timePoint );
	std::cout << "std::chrono::current_zone() = " << currentZone << std::endl;
	std::cout << "std::chrono::current_zone()::zoneName = " << zoneName << std::endl;
	std::cout << " currentZone->to_local( timePoint ) = " << localTime << std::endl;
	std::cout << "=================" << std::endl;
	return 0;
}
