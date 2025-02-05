#ifndef SHADER_H_H_HEAD__FILE__
#define SHADER_H_H_HEAD__FILE__
#pragma once
#include "base.h"

/// @brief 着色器对象
class Shader {
	/// @brief 输出着色器错误
	/// @param shader 着色器对象
	std::string getShaderInfo( GLuint shader ) const {
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
	GLuint fileBuilderShader( const std::string &file_path_name, GLenum shader_type ) const {
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
private:
	GLuint m_shaderProgram;
public:
	Shader( ) { }
	virtual ~Shader( ) {
		if( glIsProgram( m_shaderProgram ) == GL_TRUE )
			glDeleteProgram( m_shaderProgram );
	}

	/// @brief 输出着色器错误
	/// @return 错误信息
	std::string getProgramInfo( ) const {
		if( glIsProgram( m_shaderProgram ) == GL_FALSE )
			return "不是有效的着色器";
		GLint param;
		glGetProgramiv( m_shaderProgram, GL_INFO_LOG_LENGTH, &param );
		GLchar *buff = new GLchar[ param ];
		glGetProgramInfoLog( m_shaderProgram, param, nullptr, buff );
		std::string result( buff );
		delete[] buff;
		return result;
	}
	/// @brief 开始使用着色器
	/// @return 成功返回 true
	bool start( ) const {
		if( glIsProgram( m_shaderProgram ) == GL_FALSE )
			return false;
		glUseProgram( m_shaderProgram );
		return false;
	}
	/// @brief 结束使用-重置为 0
	void end( ) const {
		glUseProgram( 0 );
	}
	/// @brief 使用着色器
	/// @param vertex_shader_file_path_name  顶点着色器源码路径
	/// @param fragment_shader_file_path_name 片元着色器源码路径
	/// @result 成功返回 true
	bool initShader( const std::string &vertex_shader_file_path_name, const std::string &fragment_shader_file_path_name ) {
		auto vertexShader = fileBuilderShader( vertex_shader_file_path_name, GL_VERTEX_SHADER );
		auto framentShader = fileBuilderShader( fragment_shader_file_path_name, GL_FRAGMENT_SHADER );
		m_shaderProgram = glCreateProgram( );
		if( m_shaderProgram ) {
			glAttachShader( m_shaderProgram, vertexShader );
			if( glGetError( ) != GL_NO_ERROR )
				Printer_Error_Info( "连接顶点着色器失败 : " + getProgramInfo() );
			glAttachShader( m_shaderProgram, framentShader );
			if( glGetError( ) != GL_NO_ERROR )
				Printer_Error_Info( "链接片元着色器失败 : " + getProgramInfo() );
			glLinkProgram( m_shaderProgram );
			glDeleteShader( vertexShader ); // 删除顶点着色器
			glDeleteShader( framentShader ); // 删除片元着色器
			GLint param;
			glGetProgramiv( m_shaderProgram, GL_LINK_STATUS, &param );
			if( param != GL_TRUE ) {
				Printer_Error_Info( getProgramInfo( ) );
				glDeleteProgram( m_shaderProgram );
				m_shaderProgram = 0;
				return false;
			} else
				glUseProgram( m_shaderProgram );
		}
		return true;
	}

};

#endif // SHADER_H_H_HEAD__FILE__
