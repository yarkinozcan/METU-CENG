#version 430

#define OUT_FBO		layout(location = 0)

out OUT_FBO float fragDepth;

void main(void)
{
	fragDepth = gl_FragCoord.z;
}


