#version 110

//	vertex shader, GLSL 1.10
//
//	INPUTS
//		gl_Vertex : vertex position in object space (raw, from VBO)
//		gl_ModelViewProjectionMatrix : concatenation of GL_MODELVIEW and GL_PROJECTION
//			typically the world-to-screen transform

//	OUTPUTS
//		gl_Position : vertex position in screen space (for OpenGL to make triangles out of)
//		v_screenPosition : vertex position in screen space (for me, to pass to my own fragment shader)
varying vec4 v_screenPosition;
varying vec4 v_worldPosition;
varying vec4 v_surfaceColor;
varying vec2 v_textureCoordinates;

varying vec3 v_normal;


void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	v_screenPosition = gl_Position;
	v_worldPosition = gl_Vertex;
	v_surfaceColor = gl_Color;
	v_normal = gl_Normal;
	v_textureCoordinates = gl_MultiTexCoord0.xy;
}