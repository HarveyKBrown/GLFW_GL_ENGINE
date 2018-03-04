#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 rM;
uniform mat4 pM;
uniform mat4 perspective;

out vec4 color;

void main()
{
	vec4 nVec = normalize (vec4(aPos.x, aPos.y, aPos.z, 1.0));
	gl_Position = perspective * pM * rM * nVec;
	vec4 objColor = vec4(0.5 + nVec.x, 0.5 + nVec.y, 0.5 + nVec.z, 1.0);
	//lightIntensity = 1;
	color = objColor;// * lightIntensity;
}
