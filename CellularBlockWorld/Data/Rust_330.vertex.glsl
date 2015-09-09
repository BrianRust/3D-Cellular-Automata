#version 330

//	vertex shader, GLSL 3.30
//
//	INPUTS
//		gl_Vertex : vertex position in object space (raw, from VBO)
//		gl_ModelViewProjectionMatrix : concatenation of GL_MODELVIEW and GL_PROJECTION
//			typically the world-to-screen transform
uniform mat4 u_modelViewProjectionMatrix;

in vec4 v_Vertex;
in vec3 v_Normal;


//	OUTPUTS
//		gl_Position : vertex position in screen space (for OpenGL to make triangles out of)
//		v_screenPosition : vertex position in screen space (for me, to pass to my own fragment shader)

out vec4 v_screenPosition;
out vec4 v_worldPosition;
out vec3 v_normal;
//out vec4 v_surfaceColor;
//out vec2 v_textureCoordinates;

//varying vec3 v_normal;


void main()
{
	gl_Position = u_modelViewProjectionMatrix * v_Vertex;
	v_screenPosition = gl_Position;
	v_worldPosition = v_Vertex;

	if( v_Normal == 0 )
	{
		v_normal = vec3(0.0, -1.0, 0.0);
	}
	else if( v_Normal == 1 )
	{
		v_normal = vec3(1.0, 0.0, 0.0);
	}
	else if( v_Normal == 2 )
	{
		v_normal = vec3(0.0, 1.0, 0.0);
	}
	else if( v_Normal == 3 )
	{
		v_normal = vec3(0.0, 0.0, 1.0);
	}
	else if( v_Normal == 4 )
	{
		v_normal = vec3(-1.0, 0.0, 0.0);
	}
	else if( v_Normal == 5 )
	{
		v_normal = vec3(0.0, 0.0, -1.0);
	}

//	v_surfaceColor = gl_Color;
//	v_normal = gl_Normal;
//	v_textureCoordinates = gl_MultiTexCoord0.xy;
}