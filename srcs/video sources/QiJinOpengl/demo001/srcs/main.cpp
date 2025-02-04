#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <tools/io.h>
#include <tools/path.h>
#include <cmake_include_to_c_cpp_header_env.h>
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
void processInput( GLFWwindow *glfw_window ) {
	if( glfwGetKey( glfw_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		glfwSetWindowShouldClose( glfw_window, true );
}
int main( int argc, char **argv ) {
	if( glfwInit( ) == GLFW_FALSE ) {
		Printer_Error_Info( "无法初始化 glfw 库" );
		exit( EXIT_FAILURE ); // 异常退出
	}
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); // 最小主版本号
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 ); // 最小次版本号
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // opengl 核心模式
	GLFWwindow *glfWwindow = glfwCreateWindow( 800, 600, project_name.c_str( ), nullptr, nullptr );
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
	frameBuffSizeCallback( glfWwindow, 0, 0 );

	while( !glfwWindowShouldClose( glfWwindow ) ) {
		processInput( glfWwindow );
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		glfwSwapBuffers( glfWwindow ); // 交换
		glfwPollEvents( ); // 事件循环
	}
	glfwDestroyWindow( glfWwindow );
	glfwTerminate( ); // 关闭 glfw 资源
	exit( EXIT_SUCCESS ); // 安全退出
}
