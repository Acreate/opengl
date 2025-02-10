#include "function.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <tools/io.h>
#include <tools/path.h>
#include <stacktrace>
#include <cmake_include_to_c_cpp_header_env.h>
#include <source_location>

DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );

int main( int argc, char **argv ) {
	if( glfwInit( ) == GLFW_FALSE ) {
		Printer_Error_Info( "无法初始化 glfw 库" );
		exit( EXIT_FAILURE ); // 异常退出
	}
	auto defOpenGlVersion = defOpenGLVersion( );
	int width = 1440;
	int height = 800;
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

		ImGui::SetNextWindowSize( ImVec2( 500, 600 ) );
		ImGui::SetNextWindowPos( ImVec2( 10, 10 ) );
		// 不允许调整大小，不允许折叠
		if( ImGui::Begin( STDCString( u8"测试" ), nullptr, ImGuiWindowFlags_NoResize ) ) {
			ImGui::Checkbox( STDCString( u8"窗口控制" ), &testBool );
			if( ImGui::Button( STDCString( u8"显示窗口" ) ) ) {
				// 点击发生事件
				testBool = true;
			}
			// ## 符号指定 id，并且不会显示在页面内
			ImGui::Text( STDCString( u8"整数" ) );
			ImGui::SliderInt( STDCString( u8"##1" ), &intTest, 1, 25 );
			ImGui::Text( STDCString( u8"浮点" ) );
			ImGui::SliderFloat( STDCString( u8"##2" ), &floatTest, 0.1f, 5.5f );
			// 配置绘制坐标
			ImGui::SetCursorPos( ImVec2( 50, 350 ) );
			// 按键使用明显的大小声明
			ImGui::Button( STDCString( u8"测试按键" ), ImVec2( 500, 50 ) );
		}

		if( testBool || floatTest == 5.5f || intTest == 25 ) {
			ImGui::SetNextWindowSize( ImVec2( 200, 200 ) );
			ImGui::SetNextWindowPos( ImVec2( 600, 200 ) );
			if( ImGui::Begin( STDCString( u8"窗口 2" ) ) ) {
				if( ImGui::Button( STDCString( u8"关闭窗口" ) ) ) {
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
	glfwDestroyWindow( glfWwindow );
	glfwTerminate( ); // 关闭 glfw 资源
	exit( EXIT_SUCCESS ); // 安全退出
}
