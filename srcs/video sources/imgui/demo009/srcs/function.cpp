#include "function.h"
#include <tools/io.h>
#include <tools/path.h>
#include <cmake_include_to_c_cpp_header_env.h>
#include <backends/imgui_impl_glfw.h>

DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );
void frameBuffSizeCallback( GLFWwindow *window, int width, int height ) {
	int left, top, right, bottom;
	glfwGetWindowFrameSize( window, &left, &top, &right, &bottom );
	glViewport( left, top, right, bottom ); // 指定 opengl 渲染矩形
}
std::string defOpenGLVersion( ) {
	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    return "#version 100";
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    return "#version 300 es";
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    return "#version 150";
#else
	// GL 3.0 + GLSL 130
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	return "#version 130";
#endif

}
void loadNewFontChangeChineseFull( ImGuiIO &io ) {
	cyl::tools::path cylFont( project_name + "/resources/font/GlowSansSC-Normal-Regular.otf" );
	float sizePixels = 42.0f;
	auto chineseSimplifiedCommon = io.Fonts->GetGlyphRangesChineseFull( );
	if( cylFont.isExists( ) )
		io.Fonts->AddFontFromFileTTF( cylFont.getAbsPath( ).string( ).c_str( ), sizePixels, NULL, chineseSimplifiedCommon ); // 加载中文
	else
		io.Fonts->AddFontFromFileTTF( "c:\\Windows\\Fonts\\SimSun.ttc", sizePixels, NULL, chineseSimplifiedCommon ); // 加载中文
}
const char * STDCString( const std::u8string &u8_std_string ) {
	return reinterpret_cast< const char * >( u8_std_string.c_str( ) );
}

