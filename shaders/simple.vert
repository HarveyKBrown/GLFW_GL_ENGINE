#version 330 core

layout (location = 0) in vec3 aPos;
out vec4 color;

void main()
{
	vec4 nVec = normalize (vec4(aPos.x, aPos.y, aPos.z, 1.0));
	gl_Position = nVec;
	color = vec4(nVec.x, nVec.y, 0.5, 1.0);
}
