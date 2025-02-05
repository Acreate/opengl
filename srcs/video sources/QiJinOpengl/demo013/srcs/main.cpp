#include "base.h"
#include "shader.h"
#include "ffImage.h"
#include <tools/io.h>
#include <tools/path.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmake_include_to_c_cpp_header_env.h>
DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );

/// @brief 纹理对象指针
FfImage *g_pImage = nullptr;

/// @brief 读取纹路
void initTexture( ) {
	auto imageFilePath = project_name + "/resources/wall.jpg";
	g_pImage = FfImage::readFromFile( imageFilePath );
}
GLuint ebo = 0;
GLuint g_vbo = 0;
GLuint g_vao = 0;
GLuint g_texture = 0;

/// @brief 构建模型
void initModel( ) {
	float vertices[ ] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
		};

	glGenVertexArrays( 1, &g_vao );
	glBindVertexArray( g_vao );

	glGenBuffers( 1, &g_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, g_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void * ) 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void * ) ( sizeof( float ) * 3 ) );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	// 请求生成纹理
	glGenTextures( 1, &g_texture );
	// 绑定到 2d 纹理
	glBindTexture( GL_TEXTURE_2D, g_texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	// 内存设置为 g_pImage
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, g_pImage->getWidth( ), g_pImage->getHeight( ), 0, GL_RGBA, GL_UNSIGNED_BYTE, g_pImage->getData( ) );
	// 重置
	glBindTexture( GL_TEXTURE_2D, 0 );
	// 重置 g_vbo
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	// 重置 g_vao
	glBindVertexArray( 0 );
	
	glEnable( GL_DEPTH_TEST ); // 深度检测
	glEnable( GL_BLEND ); // 启用混合
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); // 混合透明
}

/// @brief 着色器程序
Shader g_shaderProgram;
/// @brief 使用着色器
/// @param vertex_shader_file_path_name  顶点着色器源码路径
/// @param fragment_shader_file_path_name 片元着色器源码路径
void initShader( const std::string &vertex_shader_file_path_name, const std::string &fragment_shader_file_path_name ) {
	g_shaderProgram.initShader( vertex_shader_file_path_name, fragment_shader_file_path_name );
}

int g_width = 800;
int g_height = 600;
glm::mat4 _viewMatrix( 1.0f );
glm::mat4 _projMatrix( 1.0f );

/// @brief 渲染到渲染区
void rend( ) {
	glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	_viewMatrix = glm::lookAt( glm::vec3( 3.0f, 3.0f, 3.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	_projMatrix = glm::perspective( glm::radians( 45.0f ), ( float ) g_width / ( float ) g_height, 0.1f, 100.0f );

	glBindTexture( GL_TEXTURE_2D, g_texture );

	g_shaderProgram.start( );
	g_shaderProgram.setMatrix( "_viewMatrix", _viewMatrix );
	g_shaderProgram.setMatrix( "_projMatrix", _projMatrix );
	glBindVertexArray( g_vao );
	glDrawArrays( GL_TRIANGLES, 0, 36 );
	g_shaderProgram.end( );

}

/// @brief 窗口大小(帧缓存)发生变换时，发生调用
/// @brief 在 opengl 概念当中，窗口的可绘制区域被称之“缓存”，每次绘制当做“一帧”
/// @param window 改变的窗口
/// @param width 新宽度
/// @param height 新高度
void frameBuffSizeCallback( GLFWwindow *window, int width, int height ) {
	g_width = width;
	g_height = height;
	glViewport( 0, 0, width, height ); // 指定 opengl 渲染矩形
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
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE ); // opengl 通用模式

	GLFWwindow *glfWwindow = glfwCreateWindow( g_width, g_height, project_name.c_str( ), nullptr, nullptr );
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
	frameBuffSizeCallback( glfWwindow, g_width, g_height );

	initTexture( );
	initModel( );
	auto vertexShaderFilePath = project_name + "/resources/vertexShader.glsl";
	auto fragmentShaderFilePath = project_name + "/resources/fragmentShader.glsl";
	initShader( vertexShaderFilePath, fragmentShaderFilePath );

	while( !glfwWindowShouldClose( glfWwindow ) ) {
		processInput( glfWwindow );
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		rend( );
		glfwSwapBuffers( glfWwindow ); // 交换
		glfwPollEvents( ); // 事件循环
	}
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glDeleteTextures( 1, &g_texture );
	glDeleteBuffers( 1, &g_vbo );
	glDeleteBuffers( 1, &ebo );
	glDeleteVertexArrays( 1, &g_vao );
	glfwDestroyWindow( glfWwindow );
	glfwTerminate( ); // 关闭 glfw 资源
	exit( EXIT_SUCCESS ); // 安全退出
}
