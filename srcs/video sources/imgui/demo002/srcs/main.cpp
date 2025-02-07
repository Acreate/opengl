#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <tools/io.h>
#include <tools/path.h>
#include <cmake_include_to_c_cpp_header_env.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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
int main( int argc, char **argv ) {
	if( glfwInit( ) == GLFW_FALSE ) {
		Printer_Error_Info( "无法初始化 glfw 库" );
		exit( EXIT_FAILURE ); // 异常退出
	}

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	int width = 800;
	int height = 600;
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

	auto projectCunrrentFont = project_name + "/resources/font/GlowSansSC-v0.93/GlowSansSC-Normal-Regular.otf";
	cyl::tools::path path( projectCunrrentFont );
	float sizePixels = 42.0f;
	auto chineseSimplifiedCommon = io.Fonts->GetGlyphRangesChineseSimplifiedCommon( );
	if( path.isExists( ) )
		io.Fonts->AddFontFromFileTTF( projectCunrrentFont.c_str( ), sizePixels, NULL, chineseSimplifiedCommon ); // 加载中文
	else
		io.Fonts->AddFontFromFileTTF( "c:\\Windows\\Fonts\\simsun.ttc", sizePixels, NULL, chineseSimplifiedCommon ); // 加载中文
	ImGui::StyleColorsDark( );
	ImGui_ImplGlfw_InitForOpenGL( glfWwindow, true );
	ImGui_ImplOpenGL3_Init( glsl_version );

	while( !glfwWindowShouldClose( glfWwindow ) ) {
		glfwPollEvents( ); // 事件循环
		if( glfwGetWindowAttrib( glfWwindow, GLFW_ICONIFIED ) != 0 ) {
			ImGui_ImplGlfw_Sleep( 10 );
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplGlfw_NewFrame( );
		ImGui::NewFrame( );
		// 下一个窗口大小
		// ImGui::SetNextWindowSize( ImVec2( 200, 200 ) );
		// 仅仅设置一次
		ImGui::SetWindowSize( ImVec2( 200, 200 ) );
		// 不允许调整大小，不允许折叠
		if( ImGui::Begin( ( const char * ) u8"测试", nullptr, ImGuiWindowFlags_NoResize ) ) {
			//ImGui::End( ); // 使用折叠功能时会发生异常
		}

		ImGui::End( ); // ImGui::Begin 必须 ImGui::End
		// Rendering
		ImGui::Render( );
		int display_w, display_h;
		glfwGetFramebufferSize( glfWwindow, &display_w, &display_h );
		glViewport( 0, 0, display_w, display_h );
		glClearColor( 0, 0, 0, 0 );
		glClear( GL_COLOR_BUFFER_BIT );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

		glfwSwapBuffers( glfWwindow ); // 交换
	}
	glfwDestroyWindow( glfWwindow );
	glfwTerminate( ); // 关闭 glfw 资源
	exit( EXIT_SUCCESS ); // 安全退出
}
