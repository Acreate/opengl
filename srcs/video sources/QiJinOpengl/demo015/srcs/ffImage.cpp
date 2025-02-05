#include "ffImage.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION

#include"stb_image.h"
#include <tools/io.h>
#include <tools/path.h>
#include "cmake_property_to_c_cpp_header_env.h"
DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );

FfImage * FfImage::readFromFile( const std::string &fileName ) {
	FfImage *result = nullptr;
	if( !std::filesystem::exists( fileName ) ) {
		// 不存在，则返回
		Printer_Error_Info( "路径[" + fileName + "]不存在" );
		return result;
	}
	int _picType = 0;
	int _width = 0;
	int _height = 0;

	//stbimage读入的图片是反过来的
	stbi_set_flip_vertically_on_load( true );

	unsigned char *bits = stbi_load( fileName.c_str( ), &_width, &_height, &_picType, STBI_rgb_alpha );
	if( _width == 0 || _height == 0 ) {
		Printer_Error_Info( "路径[" + fileName + "]文件发生异常" );
	} else
		result = new FfImage( _width, _height, _picType, ( FfRGBA * ) bits );

	stbi_image_free( bits );
	return result;
}
