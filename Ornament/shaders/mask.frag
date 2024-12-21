#version 440
// 指定GLSL版本

// 输入纹理坐标
layout(location = 0) in vec2 qt_TexCoord0;
// 输出片段颜色
layout(location = 0) out vec4 fragColor;
// 缓冲对象
layout(std140, binding = 0) uniform buf {
    // 变换矩阵
    mat4 qt_Matrix;
    // root item的透明度
    float qt_Opacity;
};
// 二维采样器
layout(binding = 1) uniform sampler2D source; // 源纹理
layout(binding = 2) uniform sampler2D mask; // 遮罩纹理

void main() {
    fragColor = texture(source, qt_TexCoord0)
            * texture(mask, qt_TexCoord0).a
            * qt_Opacity;
}
