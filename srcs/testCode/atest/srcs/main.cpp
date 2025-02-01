#include <iostream>
#include <chrono>
#include "../auto_generate_files/macro/cmake_to_c_cpp_header_env.h"

#include <tools/toolsNamespace.h>
#include <tools/time.h>
#include <tools/stringTools.h>
#include <tools/io.h>
#include <cwctype>
using namespace std;
DEF_CURRENT_PROJECT_NAME( );
DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );

std::vector< wstring > getFileGLEnumKey( const std::string &path ) {
	std::vector< wstring > result;
	std::wstring glGet;
	cyl::tools::path cylPath( path );
	if( cylPath.readPathFile( &glGet ) ) {
		size_t length = glGet.length( ) - 2; // 满足求取最后两个下标
		auto cStr = glGet.c_str( );
		std::vector< std::wstring::value_type > buff;
		for( size_t index = 0; index < length; ++index ) {
			if( cStr[ index ] == L'G' && cStr[ index + 1 ] == L'L' && cStr[ index + 2 ] == L'_' ) {
				if( index != 0 && cStr[ index - 1 ] != L'\n' )
					continue; // 不满足起始的 GL
				index += 3;
				for( ; index < length; ++index )
					if( !std::iswspace( cStr[ index ] ) )
						buff.emplace_back( cStr[ index ] );
					else
						break;
				result.emplace_back( std::wstring( buff.data( ), buff.size( ) ) );
				buff.clear( );
			}
		}
	}
	return result;
}

size_t writeGLEnumKey( const std::string &path, const std::vector< std::wstring > &write_context, const std::wstring &enum_class_name ) {
	if( write_context.size( ) ) {
		// 开始写入文件
		std::wstringstream ss;
		auto begin = write_context.begin( );
		auto end = write_context.end( );
		ss << L"enum class ";
		ss << cyl::tools::stringTools::toUpper( enum_class_name );
		ss << L" {\n";

		if( begin != end )
			do {
				auto key = *begin;
				ss << "\t" << key << L" = GL_" << key;
				++begin;
				if( begin == end )
					break;
				ss << L", // \n";
			} while( true );
		ss << L", // \n}; ";
		cyl::tools::path cylPath( path );
		return cylPath.writePathFile( ss.str( ) );
	}
	return 0;
}
void readOpenglEnumInfoFile( const std::string &path ) {
	cyl::tools::path cylPath( path );
	if( cylPath.isDir( ) ) {
		std::vector< std::string > fileVector, dirVector;
		cylPath.getPathDirAndFile( &fileVector, &dirVector );
		if( fileVector.size( ) ) { // 只处理文件
			for( auto &filePath : fileVector ) {
				auto enumKey = getFileGLEnumKey( filePath );
				if( enumKey.size( ) ) { // 内容大于 1 时
					std::filesystem::path currentFilePath( filePath );
					auto dirPath = currentFilePath.parent_path( ).parent_path( ); // 获取文件的目录
					auto filename = currentFilePath.filename( );
					string newFileName = dirPath.string( ) + "/conver/enum_" + filename.string( ) + ".h";
					std::wstring converWStr;
					cyl::tools::stringTools::converString( project_name, &converWStr );
					size_t writeGlEnumKey = writeGLEnumKey( newFileName, enumKey, cyl::tools::stringTools::toUpper( converWStr ) );
					if( writeGlEnumKey )
						Printer_Normal_Info( "路径 : [ " + newFileName + " ] 写入 : ( " + std::to_string( writeGlEnumKey ) + " )" );
					else
						Printer_Error_Info( "路径 : [ " + newFileName + " ] 写入错误" );
				} else
					Printer_Error_Info( "路径文件无法匹配宏说明 -> " +filePath );
			}
		}
	} else
		Printer_Error_Info( "路径是一个文件 -> " +path );

}
int main( int argc, char *argv[ ] ) {
	// readOpenglEnumInfoFile( project_name + "/resources" );
	std::cout << "TZDB List:\n";
	std::chrono::tzdb_list &listDB = std::chrono::get_tzdb_list( );
	for( auto &rDB : listDB )
		std::cout << " - " << rDB.version << "\n";

	std::cout << "\nDefault TZDB\n";
	const std::chrono::tzdb &db = std::chrono::get_tzdb( );
	std::cout << " Version: " << db.version << ", with "
		<< db.zones.size( ) << " zones, "
		<< db.links.size( ) << " zone links\n";
	std::stringstream ss;
	std::cout << " Links:\n";
	for( auto &link : db.links ) {
		std::string msg;
		msg.append( "  - " ).append( link.name( ) ).append( ":" ).append( link.target( ) ).append( "\n" );
		std::cout << msg;
		ss << msg;

	}
	std::cout << " Zones:\n";
	for( auto &zone : db.zones ) {
		std::string msg;
		msg.append( "  - " ).append( zone.name( ) ).append( "\n" );
		std::cout << msg;
		ss << msg;
	}
	cyl::tools::path writeFile( project_name + "/resources/write/local.txt" );
	writeFile.writePathFile( ss.str( ) );

	// std::chrono::system_clock::time_point
	auto tpSys = std::chrono::system_clock::now( );
	std::cout << "System time: " << tpSys << "\n";

	// std::chrono::local_time<std::chrono::system_clock::duration>
	auto tpLoc = std::chrono::current_zone( )->to_local( tpSys );
	std::cout << "Local time: " << tpLoc << "\n";
	auto commonType = tpLoc.time_since_epoch( ) - tpSys.time_since_epoch( );
	std::chrono::hh_mm_ss hms( commonType );
	std::cout << "sub : " << hms << "\n";
	
	std::chrono::zoned_time tpSysZone( tpSys );
	std::cout << "System zone time: " << tpSysZone << "\n";

	std::chrono::zoned_time tpZone( std::chrono::current_zone( ), tpSys );
	std::cout << "Zone time: " << tpZone << "\n";
	exit( EXIT_SUCCESS ); // 程序退出
}
