#version 430

#define IN_POS			layout(location = 0)
#define IN_NORMAL		layout(location = 1)
#define IN_UV			layout(location = 2)

#define OUT_UV			layout(location = 0)
#define OUT_NORMAL		layout(location = 1)
#define OUT_WORLD_POS	layout(location = 2)
#define OUT_LIGHT_POS	layout(location = 3)

#define U_TRANSFORM_MODEL	layout(location = 0)
#define U_TRANSFORM_VIEW	layout(location = 1)
#define U_TRANSFORM_PROJ	layout(location = 2)
#define U_TRANSFORM_NORMAL	layout(location = 3)
#define U_LIGHT_VP			layout(location = 4)

in IN_POS	 vec3 vPos;
in IN_NORMAL vec3 vNormal;
in IN_UV	 vec2 vUV;

out gl_PerVertex {vec4 gl_Position;};

out OUT_UV		vec2 fUV;
out OUT_NORMAL	vec3 fNormal;
out OUT_WORLD_POS vec3 fWorldPos;
out OUT_LIGHT_POS vec4 fLightPos;

U_TRANSFORM_MODEL	uniform mat4 uModel;
U_TRANSFORM_VIEW	uniform mat4 uView;
U_TRANSFORM_PROJ	uniform mat4 uProjection;
U_TRANSFORM_NORMAL  uniform mat3 uNormalMatrix;
U_LIGHT_VP			uniform mat4 uLightVP;

void main(void)
{
	fUV = vUV;
	vec4 worldPos = uModel * vec4(vPos, 1.0);
	fWorldPos = worldPos.xyz;
	fLightPos = uLightVP * worldPos;
	fNormal = normalize(uNormalMatrix * vNormal);
	gl_Position = uProjection * uView * worldPos;
}


