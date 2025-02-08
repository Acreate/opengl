#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <tools/io.h>
#include <tools/path.h>
#include <cmake_include_to_c_cpp_header_env.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cctype>

#include "tools/stringTools.h"

DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );

/// @brief 窗口大小(帧缓存)发生变换时，发生调用
/// @brief 在 opengl 概念当中，窗口的可绘制区域被称之“缓存”，每次绘制当做“一帧”
/// @param window 改变的窗口
/// @param width 新宽度
/// @param height 新高度
void frameBuffSizeCallback( GLFWwindow *window, int width, int height ) {
	int left, top, right, bottom;
	glfwGetWindowFrameSize( window, &left, &top, &right, &bottom );
	glViewport( left, top, right, bottom ); // 指定 opengl 渲染矩形
}

/// @brief 定义opengl版本
/// @return 版本头缀
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
/// @brief 使用新的字符替换编码文件，改进中文的显示-解决乱码
/// @param io 替换的输入输入对象
void loadNewFontChangeChineseFull( ImGuiIO &io ) {
	cyl::tools::path cylFont( project_name + "/resources/font/GlowSansSC-Normal-Regular.otf" );
	float sizePixels = 42.0f;
	auto chineseSimplifiedCommon = io.Fonts->GetGlyphRangesChineseFull( );
	if( cylFont.isExists( ) )
		io.Fonts->AddFontFromFileTTF( cylFont.getAbsPath( ).string( ).c_str( ), sizePixels, NULL, chineseSimplifiedCommon ); // 加载中文
	else
		io.Fonts->AddFontFromFileTTF( "c:\\Windows\\Fonts\\SimSun.ttc", sizePixels, NULL, chineseSimplifiedCommon ); // 加载中文
}

const char * getCString( const std::u8string &u8_std_string ) {
	return reinterpret_cast< const char * >( u8_std_string.c_str( ) );
}
int main( int argc, char **argv ) {
	if( glfwInit( ) == GLFW_FALSE ) {
		Printer_Error_Info( "无法初始化 glfw 库" );
		exit( EXIT_FAILURE ); // 异常退出
	}
	auto defOpenGlVersion = defOpenGLVersion( );
	int width = 1000;
	int height = 1000;
	GLFWwindow *glfWwindow = glfwCreateWindow( width, height, project_name.c_str( ), nullptr, nullptr );
	if( glfWwindow == nullptr ) { // 不存在窗口
		Printer_Error_Info( "无法创建匹配的窗口" );
		glfwTerminate( ); // 终止 glfw
		exit( EXIT_FAILURE ); // 异常退出
	}
	glfwMakeContextCurrent( glfWwindow ); // opengl 行为树绑定到窗口
	if( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) ) { // 无法初始化
		Printer_Error_Info( "无法初始化 glad 库" );
		glfwTerminate( ); // 终止 glfw
		exit( EXIT_FAILURE ); // 异常退出
	}

	glfwSetFramebufferSizeCallback( glfWwindow, frameBuffSizeCallback );
	frameBuffSizeCallback( glfWwindow, width, height );

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );
	ImGuiIO &io = ImGui::GetIO( );
	( void ) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

	loadNewFontChangeChineseFull( io );

	ImGui::StyleColorsDark( );
	ImGui_ImplGlfw_InitForOpenGL( glfWwindow, true );
	ImGui_ImplOpenGL3_Init( defOpenGlVersion.c_str( ) );
	glClearColor( 0, 0, 0, 0 );
	bool testBool = false;
	int intTest = 10;
	float floatTest = 5.5f;
	while( !glfwWindowShouldClose( glfWwindow ) ) {
		glClear( GL_COLOR_BUFFER_BIT );
		glfwPollEvents( ); // 事件循环
		if( glfwGetWindowAttrib( glfWwindow, GLFW_ICONIFIED ) != 0 ) {
			ImGui_ImplGlfw_Sleep( 10 );
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplGlfw_NewFrame( );
		ImGui::NewFrame( );

		ImGui::SetNextWindowSize( ImVec2( 400, 400 ) );
		ImGui::SetNextWindowPos( ImVec2( 10, 10 ) );
		// 不允许调整大小，不允许折叠
		if( ImGui::Begin( getCString( u8"测试" ), nullptr, ImGuiWindowFlags_NoResize ) ) {
			ImGui::Checkbox( getCString( u8"窗口控制" ), &testBool );
			if( ImGui::Button( getCString( u8"显示窗口" ) ) ) {
				// 点击发生事件
				testBool = true;
			}
			// 标题作为 id，不能出现相同，如果存在相同，使用 ImGui::PushID 区分
			ImGui::PushID( 1 ); 
			if( ImGui::SliderInt( getCString( u8"改变数值" ), &intTest, 1, 25 ) ) {
				if( intTest == 25 )
					testBool = true;
				else
					testBool = false;
			}
			// 弹出上一次压入的id
			ImGui::PopID( );
			 // 使用新的 id
			ImGui::PushID( 2 );
			if( ImGui::SliderFloat( getCString( u8"改变数值" ), &floatTest, 0.1f, 5.5f ) ) {
				if( floatTest == 5.5f )
					testBool = true;
				else
					testBool = false;
			}
			// 弹出上一次压入的id
			ImGui::PopID( );

		}

		if( testBool ) {
			ImGui::SetNextWindowSize( ImVec2( 200, 200 ) );
			ImGui::SetNextWindowPos( ImVec2( 400, 200 ) );
			if( ImGui::Begin( getCString( u8"窗口 2" ) ) ) {
				if( ImGui::Button( getCString( u8"关闭窗口" ) ) ) {
					testBool = false;
				}
			}
			ImGui::End( );
		}

		ImGui::End( ); // ImGui::Begin 必须 ImGui::End
		// Rendering
		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

		glfwSwapBuffers( glfWwindow ); // 交换
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown( );
	ImGui_ImplGlfw_Shutdown( );
	ImGui::DestroyContext( );
	
	glfwDestroyWindow( glfWwindow );
	glfwTerminate( ); // 关闭 glfw 资源
	exit( EXIT_SUCCESS ); // 安全退出
}
