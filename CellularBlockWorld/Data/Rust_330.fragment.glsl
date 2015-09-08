#version 110

//	fragment shader, GLSL 1.10
//
//	INPUTS
//		screenPosition : position of this pixel in screen space
uniform float u_time;
uniform int u_debugInt;
uniform sampler2D u_diffuseTexture;
uniform vec4 u_color;
uniform vec3 u_cameraPosition;

varying vec4 v_screenPosition;
varying vec4 v_worldPosition;
varying vec4 v_surfaceColor;
varying vec2 v_textureCoordinates;
varying vec3 v_normal;
varying vec3 v_biTangent;
varying vec3 v_tangent;

//	OUTPUTS
//		gl_FragColor - final color (RGBA) to report to the framebuffer (to be blended)
// inout pass by reference

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
	float distanceBetweenPointAndLight;
	float adjustedBrightness;
	float adjustedBrightnessDueToAperture;
	float dotProductBetweenLightDirectionAndLightToPoint;

	vec3 lightToPoint;
	vec3 totalDiffuseColorForAllLights = vec3(0.0, 0.0, 0.0);
	vec3 totalSpecularForAllLights = vec3(0.0, 0.0, 0.0);
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

	distanceBetweenPointAndLight = distance(v_worldPosition.xyz, u_cameraPosition);
	lightToPoint = normalize(u_cameraPosition - v_worldPosition.xyz);

	totalDiffuseColorForAllLights += clamp(dot( v_normal, lightToPoint ), 0.0, 1.0)* lightColor.xyz * lightColor.w;
	totalSpecularForAllLights += lightColor.xyz * lightColor.w;

	gl_FragColor.xyz = v_surfaceColor.xyz;
	gl_FragColor.w = 1.0;

	gl_FragColor.xyz = v_surfaceColor.xyz * totalDiffuseColorForAllLights;
}