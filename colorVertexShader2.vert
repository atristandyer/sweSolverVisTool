#version 400

layout(location=0) in vec4 in_Position;
out vec4 ex_Color;

uniform mat4 MVPMatrix;
uniform vec2 HeightRange;

void main(void)
{
	gl_Position = MVPMatrix*in_Position;
        if (HeightRange[1] == HeightRange[0])
            ex_Color = vec4(0.0, 0.4, 0.8, 1.0);
        else
            ex_Color = vec4(0.0, 0.0, 0.4 + 0.6*(in_Position.z - HeightRange[0])/(HeightRange[1] - HeightRange[0]), 1.0);
}
