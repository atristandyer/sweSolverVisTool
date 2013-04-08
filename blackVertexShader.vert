#version 400

layout(location=0) in vec4 in_Position;
out vec4 ex_Color;

uniform mat4 MVPMatrix;

void main(void)
{
	gl_Position = MVPMatrix*in_Position;
        ex_Color = vec4(0.0, 0.0, 0.0, 1.0);
}
