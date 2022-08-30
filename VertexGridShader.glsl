#version 330 core
// vertex shader

layout (location = 0) in vec3 aPos;

out vec3 fragPos;

uniform mat4 model;
uniform mat4 normalModel;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// ---- Transform Local space into Clip space ---- 
	// Vclip = Mprojection * Mview * Mmodel * Vlocal

    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}