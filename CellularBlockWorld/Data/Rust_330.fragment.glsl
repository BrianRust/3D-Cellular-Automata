#version 330

//	fragment shader, GLSL 3.30
//
//	INPUTS
uniform vec3 u_cameraPosition;
uniform int u_wireframeBool;
//uniform int u_normal;

in vec4 v_screenPosition;
in vec4 v_worldPosition;
in vec3 v_normal;

in vec4 v_surfaceColor;

//	OUTPUTS
out vec4 FragmentColor;

//------------------------------------------------------------------------------------------------------
float Calcluminance( vec4 color )
{
	vec3 luminanceCoefficients = vec3( 0.22, 0.71, 0.07);
	float luminance = dot( luminanceCoefficients, color.rgb );
	return luminance;
}

//------------------------------------------------------------------------------------------------------
vec4 GetAsGreyScale( vec4 color )
{
	float luminance = Calcluminance( color );
	return vec4( luminance, luminance, luminance, color.a);
}

//------------------------------------------------------------------------------------------------------
void main()
{
	vec3 lightToPoint;
	vec3 totalDiffuseColorForAllLights = vec3(0.0, 0.0, 0.0);
	vec3 totalSpecularForAllLights = vec3(0.0, 0.0, 0.0);
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

	lightToPoint = normalize(u_cameraPosition - v_worldPosition.xyz);

	totalDiffuseColorForAllLights += clamp(dot( v_normal, lightToPoint ), 0.0, 1.0) * lightColor.xyz * lightColor.w;

	FragmentColor.xyz = v_surfaceColor.xyz * totalDiffuseColorForAllLights * 0.7 * u_wireframeBool;
	FragmentColor.w = 1.0;
}