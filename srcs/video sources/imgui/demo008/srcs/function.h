#ifndef FUNCTION_H_H_HEAD__FILE__
#define FUNCTION_H_H_HEAD__FILE__
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <imgui.h>

/// @brief 窗口大小(帧缓存)发生变换时，发生调用
/// @brief 在 opengl 概念当中，窗口的可绘制区域被称之“缓存”，每次绘制当做“一帧”
/// @param window 改变的窗口
/// @param width 新宽度
/// @param height 新高度
void frameBuffSizeCallback( GLFWwindow *window, int width, int height );

/// @brief 定义opengl版本
/// @return 版本头缀
std::string defOpenGLVersion( );
/// @brief 使用新的字符替换编码文件，改进中文的显示-解决乱码
/// @param io 替换的输入输入对象
void loadNewFontChangeChineseFull( ImGuiIO &io );

const char * STDCString( const std::u8string &u8_std_string );
#endif // FUNCTION_H_H_HEAD__FILE__
