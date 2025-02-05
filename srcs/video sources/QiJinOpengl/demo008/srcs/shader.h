#ifndef SHADER_H_H_HEAD__FILE__
#define SHADER_H_H_HEAD__FILE__
#pragma once
#include "base.h"

/// @brief 着色器对象
class Shader {
	/// @brief 输出着色器错误
	/// @param shader 着色器对象
	std::string getShaderInfo( GLuint shader ) const;

	/// @brief 从文件中读取着色器源码，并且加载到新生成的着色器当中
	/// @param file_path_name 着色器源码
	/// @param shader_type 着色器类型{ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }
	/// @return 失败返回 0，成功返回着色器
	GLuint fileBuilderShader( const std::string &file_path_name, GLenum shader_type ) const;
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
	std::string getProgramInfo( ) const;
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
	bool initShader( const std::string &vertex_shader_file_path_name, const std::string &fragment_shader_file_path_name );

};

#endif // SHADER_H_H_HEAD__FILE__
