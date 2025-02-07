#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <tools/io.h>
#include <tools/path.h>
#include <cmake_include_to_c_cpp_header_env.h>
DEF_CURRENT_RELATIVELY_PATH_STATIC_VALUE( __FILE__ );
DEF_CURRENT_PROJECT_NAME( );

GLuint g_vbo = 0;
GLuint g_vao = 0;
/// @brief 构建模型
void initModel( ) {
	// 三角形的三个顶点
	float verices[ ] = {
			-0.5f, -0.5f, 0,
			0.5f, -0.5f, 0,
			0, 0.5f, 0
		};

	// 请求生成 g_vao
	glGenVertexArrays( 1, &g_vao );
	// 绑定 g_vao
	glBindVertexArray( g_vao );

	// 请求生成 g_vbo
	glGenBuffers( 1, &g_vbo );
	// 绑定内存
	glBindBuffer( GL_ARRAY_BUFFER, g_vbo );
	// 配置内存
	glBufferData( GL_ARRAY_BUFFER, sizeof( verices ), verices, GL_STATIC_DRAW );
	// 定位 layout
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), 0 );
	// 激活 layout
	glEnableVertexAttribArray( 0 );

	// 重置 g_vbo
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	// 重置 g_vao
	glBindVertexArray( 0 );

}

/// @brief 输出着色器错误
/// @param shader 着色器对象
std::string getShaderInfo( GLuint shader ) {
	GLint param;
	glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &param );
	GLchar *buff = new GLchar[ param ];
	glGetShaderInfoLog( shader, param, nullptr, buff );
	std::string result( buff );
	delete[] buff;
	return result;
}

/// @brief 从文件中读取着色器源码，并且加载到新生成的着色器当中
/// @param file_path_name 着色器源码
/// @param shader_type 着色器类型{ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }
/// @return 失败返回 0，成功返回着色器
GLuint fileBuilderShader( const std::string &file_path_name, GLenum shader_type ) {
	if( shader_type != GL_VERTEX_SHADER && shader_type != GL_FRAGMENT_SHADER )
		return 0;
	cyl::tools::path vertexPathObj( file_path_name );
	std::string vertexSharderSourceCode;
	if( vertexPathObj.readPathFile( &vertexSharderSourceCode ) ) {
		GLuint shader = glCreateShader( shader_type );
		if( shader ) {
			auto cStr = vertexSharderSourceCode.c_str( );
			glShaderSource( shader, 1, &cStr, nullptr );
			GLenum pname = GL_SHADER_SOURCE_LENGTH;
			GLint params;
			glGetShaderiv( shader, pname, &params );
			if( params ) {
				glCompileShader( shader );
				pname = GL_COMPILE_STATUS;
				glGetShaderiv( shader, pname, &params );
				if( params == GL_TRUE )
					return shader; // 成功
				Printer_Error_Info( "编译失败("+file_path_name+") : " + getShaderInfo(shader) );
				glDeleteShader( shader );
			} else
				Printer_Error_Info( "源码加载失败("+file_path_name+") : " + getShaderInfo(shader) );
		} else
			Printer_Error_Info( "着色器生成失败("+file_path_name+") : " + getShaderInfo(shader) );
	} else
		Printer_Error_Info( "着色器源码路径异常("+file_path_name+") : 不存在的源码或者源码长度为 0" );
	return 0; // 失败
}

/// @brief 输出着色器错误
/// @param shader_program 着色器程序
std::string getProgramInfo( GLuint shader_program ) {
	GLint param;
	glGetProgramiv( shader_program, GL_INFO_LOG_LENGTH, &param );
	GLchar *buff = new GLchar[ param ];
	glGetProgramInfoLog( shader_program, param, nullptr, buff );
	std::string result( buff );
	delete[] buff;
	return result;
}

/// @brief 着色器程序
GLuint shaderProgram;
/// @brief 使用着色器
/// @param vertex_shader_file_path_name  顶点着色器源码路径
/// @param fragment_shader_file_path_name 片元着色器源码路径
void initShader( const std::string &vertex_shader_file_path_name, const std::string &fragment_shader_file_path_name ) {
	auto vertexShader = fileBuilderShader( vertex_shader_file_path_name, GL_VERTEX_SHADER );
	auto framentShader = fileBuilderShader( fragment_shader_file_path_name, GL_FRAGMENT_SHADER );
	shaderProgram = glCreateProgram( );
	if( shaderProgram ) {
		glAttachShader( shaderProgram, vertexShader );
		if( glGetError( ) != GL_NO_ERROR )
			Printer_Error_Info( "连接顶点着色器失败 : " + getProgramInfo( shaderProgram ) );
		glAttachShader( shaderProgram, framentShader );
		if( glGetError( ) != GL_NO_ERROR )
			Printer_Error_Info( "链接片元着色器失败 : " + getProgramInfo( shaderProgram ) );
		glLinkProgram( shaderProgram );
		glDeleteShader( vertexShader ); // 删除顶点着色器
		glDeleteShader( framentShader ); // 删除片元着色器
		GLint param;
		glGetProgramiv( shaderProgram, GL_LINK_STATUS, &param );
		if( param != GL_TRUE ) {
			Printer_Error_Info( getProgramInfo( shaderProgram ) );
			glDeleteProgram( shaderProgram );
			shaderProgram = 0;

		} else
			glUseProgram( shaderProgram );
	}
}

const GLchar * getTypeString( GLenum type ) {
	// There are many more types than are covered here, but
	// these are the most common in these examples.
	switch( type ) {
		case GL_FLOAT :
			return "float";
		case GL_FLOAT_VEC2 :
			return "vec2";
		case GL_FLOAT_VEC3 :
			return "vec3";
		case GL_FLOAT_VEC4 :
			return "vec4";
		case GL_DOUBLE :
			return "double";
		case GL_INT :
			return "int";
		case GL_UNSIGNED_INT :
			return "unsigned int";
		case GL_BOOL :
			return "bool";
		case GL_FLOAT_MAT2 :
			return "mat2";
		case GL_FLOAT_MAT3 :
			return "mat3";
		case GL_FLOAT_MAT4 :
			return "mat4";
		case GL_INT_VEC2 :
			return "ivec2";
		case GL_INT_VEC3 :
			return "ivec3";
		case GL_INT_VEC4 :
			return "ivec4";
		case GL_BOOL_VEC2 :
			return "bvec2";
		case GL_BOOL_VEC3 :
			return "bvec3";
		case GL_BOOL_VEC4 :
			return "bvec4";
		case GL_UNSIGNED_INT_VEC2 :
			return "uvec2";
		case GL_UNSIGNED_INT_VEC3 :
			return "uvec3";
		case GL_UNSIGNED_INT_VEC4 :
			return "uvec4";
		default :
			return "?";
	}
}
/// @brief 打印着色器信息
void printfProgramInfo( ) {
	GLint numAttribs;
	int programInterface = GL_PROGRAM_INPUT;
	glGetProgramInterfaceiv( shaderProgram, programInterface, GL_ACTIVE_RESOURCES, &numAttribs );

	GLenum properties[ ] {GL_NAME_LENGTH,GL_TYPE,GL_LOCATION};
	std::stringstream ss;
	ss << "\n" << "当前有效 layout : " << numAttribs << "\n";
	for( int i = 0; i < numAttribs; ++i ) {
		GLint results[ 3 ];
		glGetProgramResourceiv( shaderProgram, programInterface, i, 3, properties, 3, nullptr, results );

		GLint nameBufSize = results[ 0 ] + 1;
		GLchar *name = new char[ nameBufSize ];
		glGetProgramResourceName( shaderProgram, programInterface, i, nameBufSize, nullptr, name );
		ss << "location:" << results[ 2 ] << "    " << name << "(" << getTypeString( results[ 1 ] ) << ")\n";
		delete[] name;
	}

	ss << "\n";
	GLint params = 0;
	glGetProgramiv( shaderProgram, GL_ATTACHED_SHADERS, &params );
	ss << "GL_ATTACHED_SHADERS: " << std::to_string( params ) << "\n";

	glGetProgramiv( shaderProgram, GL_ACTIVE_ATTRIBUTES, &params );
	ss << "GL_ACTIVE_ATTRIBUTES: " << std::to_string( params ) << "\n";

	glGetProgramiv( shaderProgram, GL_ACTIVE_UNIFORMS, &params );
	ss << "\n" << "当前有效 unforms : " << numAttribs << "\n";
	for( int i = 0; i < params; ++i ) {
		GLint results[ 3 ];
		glGetProgramResourceiv( shaderProgram, GL_UNIFORM, i, 3, properties, 3, nullptr, results );

		GLint nameBufSize = results[ 0 ] + 1;
		GLchar *name = new char[ nameBufSize ];
		glGetProgramResourceName( shaderProgram, GL_UNIFORM, i, nameBufSize, nullptr, name );
		ss << "unforms:" << results[ 2 ] << "    " << name << "(" << getTypeString( results[ 1 ] ) << ")\n";
		delete[] name;
	}

	Printer_Normal_Info( ss.str( ) );
}
/// @brief 渲染到渲染区
void rend( ) {
	glBindVertexArray( g_vao );
	glBindBuffer( GL_ARRAY_BUFFER, g_vbo );
	glUseProgram( shaderProgram );
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	glUseProgram( 0 );
}

/// @brief 窗口大小(帧缓存)发生变换时，发生调用
/// @brief 在 opengl 概念当中，窗口的可绘制区域被称之“缓存”，每次绘制当做“一帧”
/// @param window 改变的窗口
/// @param width 新宽度
/// @param height 新高度
void frameBuffSizeCallback( GLFWwindow *window, int width, int height ) {
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
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // 最小主版本号
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 ); // 最小次版本号
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE ); // opengl 核心模式
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

	initModel( );
	auto vertexShaderFilePath = project_name + "/resources/vertexShader.glsl";
	auto fragmentShaderFilePath = project_name + "/resources/fragmentShader.glsl";
	initShader( vertexShaderFilePath, fragmentShaderFilePath );
	printfProgramInfo( );
	while( !glfwWindowShouldClose( glfWwindow ) && false ) {
		processInput( glfWwindow );
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		rend( );
		glfwSwapBuffers( glfWwindow ); // 交换
		glfwPollEvents( ); // 事件循环
	}
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glDeleteBuffers( 1, &g_vbo );
	glDeleteProgram( shaderProgram );
	glfwDestroyWindow( glfWwindow );
	glfwTerminate( ); // 关闭 glfw 资源
	exit( EXIT_SUCCESS ); // 安全退出
}
