#version 330

//	vertex shader, GLSL 3.30
//
//	INPUTS
//		gl_Vertex : vertex position in object space (raw, from VBO)
//		gl_ModelViewProjectionMatrix : concatenation of GL_MODELVIEW and GL_PROJECTION
//			typically the world-to-screen transform
uniform mat4 u_modelViewProjectionMatrix;

in vec4 v_Vertex;
//in uint v_Vertex[3];

//in uint v_Normal;

in float v_Normal;
in float v_Type;

//	OUTPUTS
//		gl_Position : vertex position in screen space (for OpenGL to make triangles out of)
//		v_screenPosition : vertex position in screen space (for me, to pass to my own fragment shader)

out vec4 v_screenPosition;
out vec4 v_worldPosition;
out vec3 v_normal;
out vec4 v_surfaceColor;

void main()
{
	//vec4 floatVertex = vec4( v_Vertex[0], v_Vertex[1], v_Vertex[2], 1.0 );
	vec4 floatVertex = v_Vertex;
	vec3 assignedColor = vec3(0.0, 0.0, 0.0);

	gl_Position = u_modelViewProjectionMatrix * floatVertex;
	v_screenPosition = gl_Position;
	v_worldPosition = floatVertex;

	v_normal = vec3(0.0, 0.0, 0.0);
	v_surfaceColor = vec4(0.0, 0.0, 0.0, 1.0);

	//0
	v_normal += vec3(0.0, -1.0, 0.0);
	v_surfaceColor.rgb += vec3(0.0, 1.0, 0.0);
	assignedColor.rgb += vec3(0.0, 1.0, 0.0);
	
	//1
	v_normal += vec3(1.0, 1.0, 0.0) * mix(1.0, 0.0, clamp(sign(1.0 - v_Normal), 0.0, 1.0));
	v_surfaceColor.rgb += vec3(1.0, 0.0, 0.0) * mix(1.0, 0.0, clamp(sign(1.0 - v_Normal), 0.0, 1.0));
	assignedColor.rgb += vec3(1.0, 0.0, 0.0) * mix(1.0, 0.0, clamp(sign(1.0 - v_Normal), 0.0, 1.0));

	//2
	v_normal += vec3(-1.0, 1.0, 0.0) * mix(1.0, 0.0, clamp(sign(2.0 - v_Normal), 0.0, 1.0));
	v_surfaceColor.rgb += vec3(-1.0, 0.0, 1.0) * mix(1.0, 0.0, clamp(sign(2.0 - v_Normal), 0.0, 1.0));
	assignedColor.rgb += vec3(-1.0, 0.0, 1.0) * mix(1.0, 0.0, clamp(sign(2.0 - v_Normal), 0.0, 1.0));

	//3
	v_normal += vec3(0.0, -1.0, 1.0) * mix(1.0, 0.0, clamp(sign(3.0 - v_Normal), 0.0, 1.0));
	v_surfaceColor.rgb += vec3(1.0, -1.0, 0.0) * mix(1.0, 0.0, clamp(sign(3.0 - v_Normal), 0.0, 1.0));
	assignedColor.rgb += vec3(1.0, -1.0, 0.0) * mix(1.0, 0.0, clamp(sign(3.0 - v_Normal), 0.0, 1.0));

	//4
	v_normal += vec3(-1.0, 0.0, -1.0) * mix(1.0, 0.0, clamp(sign(4.0 - v_Normal), 0.0, 1.0));
	v_surfaceColor.rgb += vec3(0.0, 0.0, -1.0) * mix(1.0, 0.0, clamp(sign(4.0 - v_Normal), 0.0, 1.0));
	assignedColor.rgb += vec3(0.0, 0.0, -1.0) * mix(1.0, 0.0, clamp(sign(4.0 - v_Normal), 0.0, 1.0));

	//5
	v_normal += vec3(1.0, 0.0, -1.0) * mix(1.0, 0.0, clamp(sign(5.0 - v_Normal), 0.0, 1.0));
	v_surfaceColor.rgb += vec3(-1.0, 0.0, 1.0) * mix(1.0, 0.0, clamp(sign(5.0 - v_Normal), 0.0, 1.0));
	assignedColor.rgb += vec3(-1.0, 0.0, 1.0) * mix(1.0, 0.0, clamp(sign(5.0 - v_Normal), 0.0, 1.0));

	

	v_surfaceColor.rgb *= mix(0.0, 1.0, clamp(sign(1.0 - v_Type), 0.0, 1.0));
	v_surfaceColor.rgb *= mix(0.0, 1.0, clamp(sign(2.0 - v_Type), 0.0, 1.0));
	v_surfaceColor.rgb *= mix(0.0, 1.0, clamp(sign(3.0 - v_Type), 0.0, 1.0));

	v_surfaceColor.rgb += (vec3(2.0, 2.0, 2.0) * mix(1.0, 0.0, clamp(sign(2.0 - v_Type), 0.0, 1.0)));
	v_surfaceColor.rgb += (vec3(0.7, 0.0, 0.5) * mix(1.0, 0.0, clamp(sign(3.0 - v_Type), 0.0, 1.0)));
}