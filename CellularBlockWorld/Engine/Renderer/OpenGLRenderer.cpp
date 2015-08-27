#include "OpenGLRenderer.hpp"

//----------------------------------
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;

PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;

PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;

//----------------------------------
OpenGLRenderer::OpenGLRenderer()
{
	m_blockVBOid = 0;
	m_wireframeVBOid = 0;
}

//----------------------------------
int OpenGLRenderer::CreateVertexShader(const char* Filename)
{
	int length = 0;
	FILE * shaderFile;
	long sizeOfFile;
	GLchar* shaderText;
	GLint wasSuccessful;
	GLint shaderID;
	shaderFile = fopen ( Filename,"rb");
	if (shaderFile==NULL)
	{
		int filePathLength = strlen(Filename);
		char* errorLength = "File Not Found";
		char* errorMessage = new char[filePathLength + strlen(errorLength) + 2];
		std::strcpy(errorMessage, "File ");
		std::strcat(errorMessage, Filename);
		std::strcat(errorMessage, " not found.");
		int test = MessageBoxA(NULL, errorMessage, "File Not Found", MB_OK | MB_TOPMOST);
		//delete[] errorMessage;
		std::exit(0);
	}
	else
	{
		fseek (shaderFile, 0, SEEK_END);
		sizeOfFile = ftell(shaderFile);
		shaderText = new GLchar[sizeOfFile+1];
		rewind(shaderFile);
		fread(shaderText, sizeof(char), sizeOfFile, shaderFile);
		shaderText[sizeOfFile] = 0;
		fclose (shaderFile);
	}

	shaderID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( shaderID, 1, &shaderText, NULL );
	glCompileShader( shaderID );
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &wasSuccessful );
	delete[] shaderText;
	if ( wasSuccessful != GL_TRUE )
	{
		int errorCounter = 0;
		GLint infoLogLength;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
		char* infoLogText = new char[ infoLogLength + 1 ];
		glGetShaderInfoLog( shaderID, infoLogLength, NULL, infoLogText );
		int filePathLength = strlen(Filename);

		for (int index = 0; index < infoLogLength+1; index++)
		{
			if (infoLogText[index] == '\n')
			{
				errorCounter++;
			}
		}

		char* replaceText = new char[strlen(Filename)+2];
		strcpy(replaceText, Filename);
		strcat(replaceText, "(");

		std::string infoLogString( infoLogText );

		for (int index = 0; index < errorCounter; index++)
		{
			infoLogString.replace(infoLogString.find("0("), 2, replaceText);
		}

		char* appendedLogText = new char[filePathLength * 2 + (infoLogLength)];
		strcpy(appendedLogText, infoLogString.c_str());

		OutputDebugStringA(appendedLogText);
		int test = MessageBoxA(NULL, appendedLogText, "ERROR", MB_OK | MB_TOPMOST);
		delete[] replaceText;
		delete[] appendedLogText;
		delete[] infoLogText;

		std::exit(0);
	}
	return shaderID;
}

//----------------------------------
int OpenGLRenderer::CreateFragmentShader(const char* Filename)
{
	int length = 0;
	FILE * shaderFile;
	long sizeOfFile;
	GLchar* shaderText;
	GLint wasSuccessful;
	GLint shaderID;
	shaderFile = fopen ( Filename,"rb");
	if (shaderFile==NULL)
	{
		int test = MessageBoxA(NULL, Filename, "File Not Found", MB_OK | MB_TOPMOST);
		std::exit(0);
	}
	else
	{
		fseek (shaderFile, 0, SEEK_END);
		sizeOfFile = ftell(shaderFile);
		shaderText = new GLchar[sizeOfFile+1];
		rewind(shaderFile);
		fread(shaderText, sizeof(char), sizeOfFile, shaderFile);
		shaderText[sizeOfFile] = 0;
		fclose (shaderFile);
	}

	shaderID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( shaderID, 1, &shaderText, NULL );
	glCompileShader( shaderID );

	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &wasSuccessful );
	delete[] shaderText;
	if ( wasSuccessful != GL_TRUE )
	{
		int errorCounter = 0;
		GLint infoLogLength;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
		char* infoLogText = new char[ infoLogLength + 1 ];
		glGetShaderInfoLog( shaderID, infoLogLength, NULL, infoLogText );
		int filePathLength = strlen(Filename);

		for (int index = 0; index < infoLogLength+1; index++)
		{
			if (infoLogText[index] == '\n')
			{
				errorCounter++;
			}
		}

		char* replaceText = new char[strlen(Filename)+2];
		strcpy(replaceText, Filename);
		strcat(replaceText, "(");

		std::string infoLogString( infoLogText );

		for (int index = 0; index < errorCounter; index++)
		{
			infoLogString.replace(infoLogString.find("0("), 2, replaceText);
		}

		char* appendedLogText = new char[filePathLength * 2 + (infoLogLength)];
		strcpy(appendedLogText, infoLogString.c_str());

		OutputDebugStringA(appendedLogText);
		int test = MessageBoxA(NULL, appendedLogText, "ERROR", MB_OK | MB_TOPMOST);
		delete[] replaceText;
		delete[] appendedLogText;
		delete[] infoLogText;

		std::exit(0);
	}
	return shaderID;
}

//----------------------------------
int OpenGLRenderer::CreateShaderProgram(GLint vertexShaderID, GLint fragmentShaderID)
{
	GLint wasSuccessful;
	GLint shaderProgramID = glCreateProgram();
	glAttachShader( shaderProgramID, vertexShaderID );
	glAttachShader( shaderProgramID, fragmentShaderID );
	glLinkProgram( shaderProgramID );
	glGetProgramiv( shaderProgramID ,GL_LINK_STATUS, &wasSuccessful);
	if (wasSuccessful != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv( shaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLength );
		char* infoLogText = new char[ infoLogLength + 1 ];
		glGetProgramInfoLog( shaderProgramID, infoLogLength, NULL, infoLogText );
		std::string infoLogString( infoLogText );
		OutputDebugStringA(infoLogText);
		int test = MessageBoxA(NULL, infoLogText, "ERROR", MB_OK | MB_TOPMOST);
		delete[] infoLogText;
		std::exit(0);
	}
	return shaderProgramID;
}

//----------------------------------
void OpenGLRenderer::Initialize()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress( "glGenBuffers" );
	glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress( "glBindBuffer" );
	glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress( "glBufferData" );
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress( "glDeleteBuffers" );
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress( "glGenerateMipmap" );
	glGetShaderiv = (PFNGLGETSHADERIVPROC) wglGetProcAddress( "glGetShaderiv" );
	glCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress( "glCreateProgram" );
	glAttachShader = (PFNGLATTACHSHADERPROC) wglGetProcAddress( "glAttachShader" );
	glLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress( "glLinkProgram" );
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress( "glGetShaderInfoLog" );
	glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress( "glCreateShader" );
	glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress( "glCompileShader" );
	glShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress( "glShaderSource" );
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) wglGetProcAddress( "glGetProgramInfoLog" );
	glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress( "glUseProgram" );
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress( "glGetUniformLocation" );
	glUniform1f = (PFNGLUNIFORM1FPROC) wglGetProcAddress( "glUniform1f" );
	glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress( "glUniform1i" );
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC) wglGetProcAddress( "glGetProgramiv" );
	glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress( "glActiveTexture");

	glUniform3fv = (PFNGLUNIFORM3FVPROC) wglGetProcAddress( "glUniform3fv" );

	//m_orientationTexture = Texture::CreateOrGetTexture("../Data/Images/OrientationTest.png");

	m_vertexShaderID = CreateVertexShader("Data/Rust_100.vertex.glsl");
	m_fragmentShaderID = CreateFragmentShader("Data/Rust_100.fragment.glsl");
	m_shaderProgramID = CreateShaderProgram(m_vertexShaderID, m_fragmentShaderID);

	m_cameraPosition = glGetUniformLocation(m_shaderProgramID, "u_cameraPosition");

	glGenBuffers(1, &m_blockVBOid);
	glGenBuffers(1, &m_wireframeVBOid);

	isInitializing = true;

	//m_blockVertices.reserve( ConstantParameters::TOTAL_BLOCKS_IN_ZONE * NUM_VERTS_PER_BLOCK );
	//m_wireframeVertices.reserve( ConstantParameters::TOTAL_BLOCKS_IN_ZONE * NUM_VERTS_PER_BLOCK );
}

//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::AddCubeToBuffer( const Vector3& minPosition, const RGBA& color )
{
	//const int NUM_VERTS_PER_BLOCK = 24;
	//m_blockVertices.clear();
	//m_blockVertices.reserve( ConstantParameters::TOTAL_BLOCKS_IN_ZONE * NUM_VERTS_PER_BLOCK );
	Vector2 texCoordinate = Vector2(0.f, 0.f);
	Vector3 newPosition = Vector3(0.f, 0.f, 0.f);
	Vector3 normal = Vector3(0.f, 0.f, 1.f);
	RGBA GridColor = RGBA(0.f, 0.f, 0.f, 1.f);
	
	//FRONT
	normal = Vector3(0.f, -1.f, 0.f);
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f);
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	
	//RIGHT
	normal = Vector3(1.f, 0.f, 0.f);
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f);
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z + 1.f);
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );

	//BACK
	normal = Vector3(0.f, 1.f, 0.f);
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );

	//TOP
	normal = Vector3(0.f, 0.f, 1.f);
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );

	//LEFT
	normal = Vector3(-1.f, 0.f, 0.f);
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );

	//BOTTOM
	normal = Vector3(0.f, 0.f, -1.f);
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, color, texCoordinate, normal ) );


// 	newPosition = Vector3(minPosition.x, minPosition.y, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x, minPosition.y + 1.f, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x, minPosition.y, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x, minPosition.y, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x, minPosition.y, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x, minPosition.y + 1.f, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y + 1.f , minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x, minPosition.y + 1.f, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x, minPosition.y, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y, minPosition.z);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 
// 	newPosition = Vector3(minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
// 	newPosition = Vector3(minPosition.x, minPosition.y, minPosition.z + 1.f);
// 	m_wireframeVertices.push_back( Vertex( newPosition, GridColor, texCoordinate ) );
}

//---------------------
void OpenGLRenderer::SendViewMatrix( Camera myCamera )
{
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	gluPerspective(50.0, (16.f/9.f), 0.1, 1000.0);

	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glRotatef(90.f, 0.f, 0.f, 1.f);

	glRotatef(-(ConstantParameters::CONVERTING_TO_DEGREES * myCamera.m_cameraRoll), 1.f, 0.f, 0.f);
	glRotatef(-(ConstantParameters::CONVERTING_TO_DEGREES * myCamera.m_cameraPitch), 0.f, 1.f, 0.f);
	glRotatef(-(ConstantParameters::CONVERTING_TO_DEGREES * myCamera.m_cameraYaw), 0.f, 0.f, 1.f);

	glTranslatef(-myCamera.m_cameraPosition.x, -myCamera.m_cameraPosition.y, -myCamera.m_cameraPosition.z);

	glUniform3fv(m_cameraPosition, 1, &myCamera.m_cameraPosition.x);
}

void OpenGLRenderer::PopMatrix()
{
	glPopMatrix();
}

void OpenGLRenderer::PushCubeVerticesToVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_blockVBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_blockVertices.size(),&m_blockVertices.front(),GL_STATIC_DRAW);
}

void OpenGLRenderer::PushGridOutlineVerticesToVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_wireframeVBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_wireframeVertices.size(),&m_wireframeVertices.front(),GL_STATIC_DRAW);
}

void OpenGLRenderer::SendCubeVBO()
{
	float timeNow = (float)Time::GetCurrentTimeSeconds();
	GLint uniformloc;
	
	glUseProgram(m_shaderProgramID);
	uniformloc = glGetUniformLocation(m_shaderProgramID, "u_time");
	timeNow = fmod(timeNow, 1.f);
	glUniform1f(uniformloc, timeNow);
	uniformloc = glGetUniformLocation(m_shaderProgramID, "u_diffuseTexture");

	glBindBuffer(GL_ARRAY_BUFFER, m_blockVBOid);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*) offsetof(Vertex, vertexPosition));
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void*) offsetof(Vertex, color)); 
//	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*) offsetof(Vertex, color)); 
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*) offsetof(Vertex, normal));
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glDrawArrays(GL_QUADS, 0, m_blockVertices.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	//wireframe
// 	glBindBuffer(GL_ARRAY_BUFFER, m_wireframeVBOid);
// 	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (float*) offsetof(Vertex, vertexPosition));
// 	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (float*) offsetof(Vertex, color)); 
// 	glEnableClientState(GL_VERTEX_ARRAY);
// 	glEnableClientState(GL_COLOR_ARRAY);
// 
// 	glDrawArrays(GL_LINES, 0, m_wireframeVertices.size());
// 	glDisableClientState(GL_VERTEX_ARRAY);
// 	glDisableClientState(GL_COLOR_ARRAY);
}

void OpenGLRenderer::DeleteBuffers()
{
	//glDeleteBuffers(1, &m_blockVBOid);
	m_blockVertices.clear();

	//glDeleteBuffers(1, &m_wireframeVBOid);
	//m_wireframeVertices.clear();
}