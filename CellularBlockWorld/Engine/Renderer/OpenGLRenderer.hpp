#ifndef included_OpenGLRenderer
#define included_OpenGLRenderer
#pragma once

//-------------------------------------------------------------
#include <Windows.h>
#include <vector>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"
#include "../Math/MyMathFiles.hpp"
#include "../Math/Vertex.hpp"
#include "../Constants.hpp"
#include "../Camera/Camera.hpp"
#include "../Time/Time.hpp"
#pragma comment( lib, "glu32") // Link in the GLu.lib static library


//-------------------------------------------------------------
class OpenGLRenderer
{
public:
	OpenGLRenderer();
	void Initialize();
	void SendViewMatrix( Camera myCamera );
	void AddCubeToBuffer(const Vector3& minPosition, const RGBA& color);
	void PushCubeVerticesToVBO();
	void PushGridOutlineVerticesToVBO();
	void PopMatrix();
	void SendCubeVBO();

	void DrawTargetCellOutline(Vector3 startPosition);

	void DeleteBuffers();

	int CreateVertexShader(const char* Filename);
	int CreateFragmentShader(const char* Filename);
	int CreateShaderProgram(GLint vertexShaderID, GLint fragmentShaderID);

	bool isInitializing;

	unsigned int m_blockVBOid;
	std::vector<Vertex> m_blockVertices;

	unsigned int m_wireframeVBOid;
	std::vector<Vertex> m_wireframeVertices;

	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;
	GLuint m_shaderProgramID;

	GLuint m_cameraPosition;
	GLuint m_normalAttributeLocation;
	GLuint m_biTangentAttributeLocation;
	GLuint m_tangentAttributeLocation;
};
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;

extern PFNGLUNIFORM3FVPROC glUniform3fv;
#endif //included_OpenGLRenderer