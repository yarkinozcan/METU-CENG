#version 430

#define IN_POS			layout(location = 0)
#define IN_NORMAL		layout(location = 1)
#define IN_UV			layout(location = 2)

#define U_TRANSFORM_MODEL	layout(location = 0)
#define U_TRANSFORM_VIEW	layout(location = 1)
#define U_TRANSFORM_PROJ	layout(location = 2)

in IN_POS vec3 vPos;

out gl_PerVertex {vec4 gl_Position;};

U_TRANSFORM_MODEL	uniform mat4 uModel;
U_TRANSFORM_VIEW	uniform mat4 uView;
U_TRANSFORM_PROJ	uniform mat4 uProjection;

void main(void)
{
	gl_Position = uProjection * uView * uModel * vec4(vPos.xyz, 1.0f);
}


