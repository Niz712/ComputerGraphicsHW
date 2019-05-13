#version 330 core
layout (location = 0) in vec3 aPos;		// λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aColor;	// ��ɫ����������λ��ֵΪ 1

out vec3 ourColor;	// ��Ƭ����ɫ�����һ����ɫ

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ���ͱ���gl_VertexID���������ڻ��ƶ���ĵ�ǰID��
// ����ʹ��glDrawElements������������Ⱦ��ʱ�����������洢���ڻ��ƶ���ĵ�ǰ������
// ����ʹ��glDrawArrays����ʹ���������л��Ƶ�ʱ����������ᴢ�����Ⱦ���ÿ�ʼ���Ѵ�����������

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
}