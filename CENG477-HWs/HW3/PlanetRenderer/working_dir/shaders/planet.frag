#version 430

// inputs from vert shader
layout(location = 0) in vec2 fUV;
layout(location = 1) in vec3 fNormal;
layout(location = 2) in vec3 fWorldPos;
layout(location = 3) in vec4 fLightPos;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D texAlbedo;
layout(binding = 1) uniform sampler2D texSpec;      // Used when isEarth
layout(binding = 2) uniform sampler2D texNight;   // Used when isEarth
layout(binding = 3) uniform sampler2D texShadow;

layout(location = 0) uniform vec3 uLightDir;
layout(location = 1) uniform vec3 uLightColor;
layout(location = 2) uniform vec3 uCamPos;
layout(location = 3) uniform float uAmbient;
layout(location = 4) uniform float uBias;
layout(location = 5) uniform float uIsEarth;      // 1.0 for Earth, 0.0 for others
layout(location = 6) uniform float uIsBackground; // 1.0 for background, 0.0 otherwise
layout(location = 7) uniform float uIsSun;        // 1.0 for sun, 0.0 otherwise
layout(location = 8) uniform float uIsCloud;      // 1.0 for clouds, 0.0 otherwise

float calcShadow(vec4 lightSpacePos, vec3 n, vec3 l)
{
	vec3 proj = lightSpacePos.xyz / lightSpacePos.w;
	proj = proj * 0.5 + 0.5;
	
	// out of frustum
	if(proj.x < 0.0 || proj.x > 1.0 || proj.y < 0.0 || proj.y > 1.0 || proj.z > 1.0)
		return 1.0;
	
	float depth = proj.z;
	float bias = max(uBias * (1.0 - dot(n, l)), uBias * 0.1);
	
	float d = texture(texShadow, proj.xy).r;
	return (depth - bias <= d) ? 1.0 : 0.0;
}

void main()
{
	// Background mode: just sample texture
	if(uIsBackground > 0.5)
	{
		vec3 stars = texture(texAlbedo, fUV).rgb;
		fragColor = vec4(stars, 1.0);
		return;
	}
	
	// Sun mode: just output bright color
	if(uIsSun > 0.5)
	{
		fragColor = vec4(uLightColor * 2.0, 1.0);
		return;
	}
	
	// Cloud mode: lighting with alpha
	if(uIsCloud > 0.5)
	{
		vec4 cloud = texture(texAlbedo, fUV);
		vec3 N = normalize(fNormal);
		vec3 L = normalize(-uLightDir);
		
		float diff = max(dot(N, L), 0.0);
		
		vec3 col = cloud.rgb * (0.02 + 0.98 * diff) * uLightColor;
		float alpha = cloud.a * (0.3 + 0.7 * diff);
		
		fragColor = vec4(col, alpha);
		return;
	}
	
	// Planet/Earth mode: full lighting
	vec3 albedo = texture(texAlbedo, fUV).rgb;
	vec3 N = normalize(fNormal);
	vec3 L = normalize(-uLightDir);
	
	float NdotL = dot(N, L);
	float facing = max(NdotL, 0.0);
	
	// diffuse
	vec3 diff = facing * uLightColor * albedo;
	
	// specular
	vec3 V = normalize(uCamPos - fWorldPos);
	vec3 R = reflect(-L, N);
	vec3 specular;
	
	if(uIsEarth > 0.5)
	{
		// Earth: use specular map
		float specMap = texture(texSpec, fUV).r;
		float specPow = mix(4.0, 128.0, specMap);
		float s = pow(max(dot(V, R), 0.0), specPow);
		specular = s * uLightColor * 0.4 * specMap;
	}
	else
	{
		// Regular planet: fixed specular
		float spec = pow(max(dot(V, R), 0.0), 32.0);
		specular = spec * uLightColor * 0.3;
	}
	
	// night lights (Earth only)
	vec3 night = vec3(0.0);
	if(uIsEarth > 0.5)
	{
		float nightF = 1.0 - facing;
		nightF = smoothstep(0.0, 0.3, nightF);
		vec3 nightCol = texture(texNight, fUV).rgb;
		night = nightCol * nightF;
	}
	
	float shadow = calcShadow(fLightPos, N, L);
	
	vec3 ambient = uAmbient * albedo;
	
	vec3 col = ambient + shadow * (diff + specular) + night;
	fragColor = vec4(col, 1.0);
}
