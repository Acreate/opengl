#include <iostream>
#include <chrono>
#include "../auto_generate_files/macro/cmake_to_c_cpp_header_env.h"

#include <tools/toolsNamespace.h>
#include <tools/time.h>
#include <tools/stringTools.h>
#include <tools/io.h>
#include <tools/path.h>
#include <cwctype>
#include <Windows.h>
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
void readCmakePropertiesInfoFile( const string &cmake_path ) {
	cyl::tools::path targetProperties( cmake_path + "/cmake/Properties on Targets.txt" );
	if( targetProperties.isExists( ) ) { // 读取目标
		std::wstring textContext;
		if( targetProperties.readPathFile( &textContext ) ) {
			std::wstring spliteStr( L"\n" );
			auto splitString = cyl::tools::stringTools::splitString( textContext, spliteStr );
			std::wstringstream cHeardFile, cmkaeFcuntionGetProperty, defineVector;
			defineVector << L"\n#include <utility>\n#include <string>\n#include <vector>\n/// @brief cmake 的全变量宏定义容器 \n#define DEF_ALL_CMAKE_VALUE_STD_VECTOR( ) \\\n\tconst static std::vector<std::pair<std::string, std::string>> cmake_all_name_value = { \\\n";

			for( auto &removeBothSpace : splitString ) {
				auto removeBothSpaceChar = cyl::tools::stringTools::removeBothSpaceChar( removeBothSpace );

				// 长度为0，或者字符串当中存在空格，则不处理
				if( removeBothSpaceChar.length( ) == 0 || cyl::tools::stringTools::hasSpace( removeBothSpaceChar ) || removeBothSpaceChar.find( L"<" ) != std::wstring::npos || removeBothSpaceChar.find( L"LOCATION" ) != std::wstring::npos )
					continue;
				auto cmakeKeyName = L"cmake_property_" + cyl::tools::stringTools::toLower( removeBothSpaceChar );
				auto cHeardKeyName = L"cmake_property_" + cyl::tools::stringTools::toUpper( removeBothSpaceChar );
				cHeardFile << L"/// @brief \n#define " << cHeardKeyName << L" \"${" << cmakeKeyName << L"}\"\n";
				// get_target_property( var ${target_obj} <CONFIG>_OUTPUT_NAME )
				cmkaeFcuntionGetProperty << L"get_target_property( " << cmakeKeyName << L" ${target_obj} " << removeBothSpaceChar << L" )\n";
				// std::pair<int,std::pair<std::string, std::string>>
				defineVector << L"\tstd::pair<std::string, std::string>(\"" << cHeardKeyName << "\", " << cHeardKeyName << "),\\\n";
			}
			defineVector << L"}";
			cyl::tools::path targetPropertiesWriteHeader( cmake_path + "/cmake/write/Properties on Targets.txt" );
			cHeardFile << L"\n" << defineVector.str( ) << L"\n\n// cmake -> \n" << cmkaeFcuntionGetProperty.str( ) << L"\n\n// cmake end\n";
			targetPropertiesWriteHeader.writePathFile( cHeardFile.str( ) );
		}
	}
}
/// @brief 测试字符串切分
void testSplitString( ) {
	std::wstring source = L"3333243573";
	std::wstring checkString = L"3";
	std::wcout << L"原始字符串:(" << source.length( ) << L")=>\t" << source << std::endl;
	std::wcout << L"匹配字符串:(" << checkString.length( ) << L")=>\t" << checkString << std::endl;
	std::wcout << L"==================" << std::endl;
	auto splitString = cyl::tools::stringTools::splitString( source, checkString );
	for( auto &outPrt : splitString )
		std::wcout << L"\t长度:(" << outPrt.length( ) << L")=>\t" << outPrt << std::endl;
}
/// @brief 空格移除实验
void testRmoveSpace( ) {
	std::wstring obj( L" d sadsa " );
	auto removeLeftSpaceChar = cyl::tools::stringTools::removeLeftSpaceChar( obj );
	std::wcout << L"$" << removeLeftSpaceChar << L"$" << std::endl;
	auto removeRightSpaceChar = cyl::tools::stringTools::removeRightSpaceChar( obj );
	std::wcout << L"$" << removeRightSpaceChar << L"$" << std::endl;
	auto removeBothSpaceChar = cyl::tools::stringTools::removeBothSpaceChar( obj );
	std::wcout << L"$" << removeBothSpaceChar << L"$" << std::endl;
}

DEF_ALL_CMAKE_VALUE_STD_VECTOR( );
int main( int argc, char *argv[ ] ) {
	try {
		auto text = "en_US.UTF-8";
		std::locale locale( text );
		std::setlocale( LC_ALL, text );
		std::locale::global( locale );
		std::wcout.imbue( locale );
		std::cout.imbue( locale );
	} catch( const std::exception &msg ) {
		std::cerr << msg.what( ) << std::endl;
		return -1;
	}
	for( auto &pair : cmake_all_name_value ) {
		std::cout << pair.first << " : " << pair.second << std::endl;
	}
	// readOpenglEnumInfoFile( project_name + "/resources/opengl" );
	//testSplitString( );
	//testRmoveSpace( );
	readCmakePropertiesInfoFile( project_name + "/resources" );

	exit( EXIT_SUCCESS ); // 程序退出
}
