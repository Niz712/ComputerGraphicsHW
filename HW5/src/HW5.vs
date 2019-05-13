#version 330 core
layout (location = 0) in vec3 aPos;		// 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor;	// 颜色变量的属性位置值为 1

out vec3 ourColor;	// 向片段着色器输出一个颜色

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 整型变量gl_VertexID储存了正在绘制顶点的当前ID。
// 当（使用glDrawElements）进行索引渲染的时候，这个变量会存储正在绘制顶点的当前索引。
// 当（使用glDrawArrays）不使用索引进行绘制的时候，这个变量会储存从渲染调用开始的已处理顶点数量。

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}