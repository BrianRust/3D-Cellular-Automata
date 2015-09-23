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

PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = nullptr;

PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;

//----------------------------------
OpenGLRenderer::OpenGLRenderer() 
	: m_modelviewProjectionStack(MatrixStack())
	, m_blockVBOid(0)
{
	
}

//----------------------------------
int OpenGLRenderer::CreateVertexShader(const char* Filename) {
	FILE * shaderFile;
	long sizeOfFile;
	GLchar* shaderText;
	GLint wasSuccessful;
	GLint shaderID;
	shaderFile = fopen ( Filename,"rb");
	if (shaderFile==NULL) {
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
	else {
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
	if ( wasSuccessful != GL_TRUE ) {
		int errorCounter = 0;
		GLint infoLogLength;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
		char* infoLogText = new char[ infoLogLength + 1 ];
		glGetShaderInfoLog( shaderID, infoLogLength, NULL, infoLogText );
		int filePathLength = strlen(Filename);

		for (int index = 0; index < infoLogLength+1; index++) {
			if (infoLogText[index] == '\n') {
				errorCounter++;
			}
		}

		char* replaceText = new char[strlen(Filename)+2];
		strcpy(replaceText, Filename);
		strcat(replaceText, "(");

		std::string infoLogString( infoLogText );

		for (int index = 0; index < errorCounter; index++) {
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
int OpenGLRenderer::CreateFragmentShader(const char* Filename) {
	FILE * shaderFile;
	long sizeOfFile;
	GLchar* shaderText;
	GLint wasSuccessful;
	GLint shaderID;
	shaderFile = fopen ( Filename,"rb");
	if (shaderFile==NULL) {
		int test = MessageBoxA(NULL, Filename, "File Not Found", MB_OK | MB_TOPMOST);
		std::exit(0);
	}
	else {
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
	if ( wasSuccessful != GL_TRUE ) {
		int errorCounter = 0;
		GLint infoLogLength;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
		char* infoLogText = new char[ infoLogLength + 1 ];
		glGetShaderInfoLog( shaderID, infoLogLength, NULL, infoLogText );
		int filePathLength = strlen(Filename);

		for (int index = 0; index < infoLogLength+1; index++) {
			if (infoLogText[index] == '\n') {
				errorCounter++;
			}
		}

		char* replaceText = new char[strlen(Filename)+2];
		strcpy(replaceText, Filename);
		strcat(replaceText, "(");

		std::string infoLogString( infoLogText );

		for (int index = 0; index < errorCounter; index++) {
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
int OpenGLRenderer::CreateShaderProgram(GLint vertexShaderID, GLint fragmentShaderID) {
	GLint wasSuccessful;
	GLint shaderProgramID = glCreateProgram();
	glAttachShader( shaderProgramID, vertexShaderID );
	glAttachShader( shaderProgramID, fragmentShaderID );
	glLinkProgram( shaderProgramID );
	glGetProgramiv( shaderProgramID ,GL_LINK_STATUS, &wasSuccessful);
	if (wasSuccessful != GL_TRUE) {
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
void OpenGLRenderer::Initialize() {
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

	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress( "glEnableVertexAttribArray" );
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) wglGetProcAddress( "glVertexAttribPointer" );
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress( "glDisableVertexAttribArray" );
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress( "glGetAttribLocation" );
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress( "glUniformMatrix4fv" );
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC) wglGetProcAddress( "glVertexAttribIPointer" );

	glUniform3fv = (PFNGLUNIFORM3FVPROC) wglGetProcAddress( "glUniform3fv" );

	//m_orientationTexture = Texture::CreateOrGetTexture("../Data/Images/OrientationTest.png");

	m_vertexShaderID = CreateVertexShader("Data/Rust_330.vertex.glsl");
	m_fragmentShaderID = CreateFragmentShader("Data/Rust_330.fragment.glsl");
	m_shaderProgramID = CreateShaderProgram(m_vertexShaderID, m_fragmentShaderID);

	m_wireFrameBoolLocation = glGetUniformLocation(m_shaderProgramID, "u_wireframeBool");
	m_cameraPosition = glGetUniformLocation(m_shaderProgramID, "u_cameraPosition");
	m_modelViewProjectionUniformLocation = glGetUniformLocation(m_shaderProgramID, "u_modelViewProjectionMatrix");
	//m_normalAttributeLocation = glGetUniformLocation(m_shaderProgramID, "u_normal");
	m_normalAttributeLocation = glGetAttribLocation(m_shaderProgramID, "v_Normal");
	m_vertexAttributeLocation = glGetAttribLocation(m_shaderProgramID, "v_Vertex");

	glGenBuffers(1, &m_blockVBOid);
	isInitializing = true;

	const int NUM_VERTS_PER_BLOCK = 24;

	m_blockVertices.reserve( ConstantParameters::TOTAL_BLOCKS_IN_ZONE * NUM_VERTS_PER_BLOCK * ( ConstantParameters::SOLID_BLOCK_PERCENTAGE_3D / 1000 ) );

	glUseProgram(m_shaderProgramID);
}

//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::AddCubeToBuffer( const Vector3& minPosition ) {
	Vector3 newPosition = Vector3(0.f, 0.f, 0.f);
	unsigned char sideValue;
	
	//FRONT
	//normal = Vector3(0.f, -1.f, 0.f);
	sideValue = 0;
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f);
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	
	//RIGHT
	//normal = Vector3(1.f, 0.f, 0.f);
	sideValue = 1;
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f);
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z + 1.f);
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );

	//BACK
	//normal = Vector3(0.f, 1.f, 0.f);
	sideValue = 2;
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );

	//TOP
	//normal = Vector3(0.f, 0.f, 1.f);
	sideValue = 3;
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );

	//LEFT
	//normal = Vector3(-1.f, 0.f, 0.f);
	sideValue = 4;
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z + 1.f );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );

	//BOTTOM
	//normal = Vector3(0.f, 0.f, -1.f);
	sideValue = 5;
	newPosition = Vector3( minPosition.x, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y + 1.f, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
	newPosition = Vector3( minPosition.x + 1.f, minPosition.y, minPosition.z );
	m_blockVertices.push_back( Vertex( newPosition, sideValue ) );
}

//---------------------
void OpenGLRenderer::SendViewMatrix(const Camera& myCamera ) {
	glUseProgram(0);
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

//---------------------
void OpenGLRenderer::PopMatrix() {
	glPopMatrix();
	m_modelviewProjectionStack.PopMatrix();
}

//---------------------
void OpenGLRenderer::PushCubeVerticesToVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, m_blockVBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_blockVertices.size(),&m_blockVertices.front(),GL_STATIC_DRAW);
}

//---------------------
void OpenGLRenderer::SendCubeVBO() {
	//render the sides
	glUniformMatrix4fv(m_modelViewProjectionUniformLocation, 1, false, m_modelviewProjectionStack.m_MatrixStack[m_modelviewProjectionStack.m_MatrixStack.size()-1].m_Matrix);
	glUniform1i( m_wireFrameBoolLocation, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_blockVBOid);
	glEnableVertexAttribArray(m_vertexAttributeLocation);
	//glVertexAttribPointer(m_vertexAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (float*) offsetof(Vertex, vertexPosition));
	glVertexAttribPointer(m_vertexAttributeLocation, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (float*) offsetof(Vertex, positionBytes));
	//glVertexAttribIPointer(m_vertexAttributeLocation, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (unsigned char*) offsetof(Vertex, positionBytes));
	glEnableVertexAttribArray(m_normalAttributeLocation);
	glVertexAttribPointer(m_normalAttributeLocation, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (unsigned char*) offsetof(Vertex, side));
	//glVertexAttribIPointer(m_normalAttributeLocation, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (unsigned char*) offsetof(Vertex, side));

	glDrawArrays(GL_QUADS, 0, m_blockVertices.size());

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
	glUniform1i( m_wireFrameBoolLocation, 0);
	glDrawArrays(GL_QUADS, 0, m_blockVertices.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );


	glDisableVertexAttribArray(m_vertexAttributeLocation);
	glDisableVertexAttribArray(m_normalAttributeLocation);
}

//----------------------
void OpenGLRenderer::DeleteBuffers() {
	//glDeleteBuffers(1, &m_blockVBOid);
	m_blockVertices.clear();
}

//-------------------------------------
void OpenGLRenderer::DrawTargetCellOutline(Vector3 startPosition) {
	RGBA lineColor(1.f, 1.f, 1.f, 1.f);
	//glUseProgram(0);
	
	glLineWidth(3.f);
	glBegin(GL_LINES);
	{	
		glColor4f( lineColor.r, lineColor.g, lineColor.b, lineColor.a );
		glVertex3f(startPosition.x, startPosition.y, startPosition.z);
		glVertex3f(startPosition.x, startPosition.y + 1.f, startPosition.z);

		glVertex3f(startPosition.x, startPosition.y, startPosition.z);
		glVertex3f(startPosition.x + 1.f, startPosition.y, startPosition.z);

		glVertex3f(startPosition.x, startPosition.y, startPosition.z);
		glVertex3f(startPosition.x, startPosition.y, startPosition.z + 1.f);

		glVertex3f(startPosition.x + 1.f, startPosition.y + 1.f, startPosition.z);
		glVertex3f(startPosition.x, startPosition.y + 1.f, startPosition.z);

		glVertex3f(startPosition.x + 1.f, startPosition.y + 1.f, startPosition.z);
		glVertex3f(startPosition.x + 1.f, startPosition.y, startPosition.z);

		glVertex3f(startPosition.x + 1.f, startPosition.y + 1.f, startPosition.z);
		glVertex3f(startPosition.x + 1.f, startPosition.y + 1.f , startPosition.z + 1.f);

		glVertex3f(startPosition.x, startPosition.y + 1.f, startPosition.z + 1.f);
		glVertex3f(startPosition.x, startPosition.y + 1.f, startPosition.z);

		glVertex3f(startPosition.x, startPosition.y + 1.f, startPosition.z + 1.f);
		glVertex3f(startPosition.x, startPosition.y, startPosition.z + 1.f);

		glVertex3f(startPosition.x, startPosition.y + 1.f, startPosition.z + 1.f);
		glVertex3f(startPosition.x + 1.f, startPosition.y + 1.f, startPosition.z + 1.f);

		glVertex3f(startPosition.x + 1.f, startPosition.y, startPosition.z + 1.f);
		glVertex3f(startPosition.x + 1.f, startPosition.y, startPosition.z);

		glVertex3f(startPosition.x + 1.f, startPosition.y, startPosition.z + 1.f);
		glVertex3f(startPosition.x + 1.f, startPosition.y + 1.f, startPosition.z + 1.f);

		glVertex3f(startPosition.x + 1.f, startPosition.y, startPosition.z + 1.f);
		glVertex3f(startPosition.x, startPosition.y, startPosition.z + 1.f);
	}
	glEnd();
	glUseProgram(m_shaderProgramID);
}

//-----------------------------------
void OpenGLRenderer::SetModelViewProjectionMatrix(const Camera& camera)
{
	glUniform3fv(m_cameraPosition, 1, &camera.m_cameraPosition.x);
	
	MatrixStack multiplyMatrix;

	m_modelviewProjectionStack.PushMatrix();
	m_modelviewProjectionStack.SetToPerspectiveMatrix(90.f * ConstantParameters::CONVERTING_TO_RADIANS, (ConstantParameters::ASPECT_RATIO), 0.1f, 1000.f);

	Matrix4x4& currentMatrix = m_modelviewProjectionStack.m_MatrixStack[m_modelviewProjectionStack.m_MatrixStack.size()-1];

	multiplyMatrix.SetToRotationMatrixX(-1.f * ConstantParameters::NINETY_DEGREES_AS_RADIANS);
	currentMatrix = currentMatrix * multiplyMatrix.m_MatrixStack[multiplyMatrix.m_MatrixStack.size()-1];

	multiplyMatrix.SetToRotationMatrixZ(ConstantParameters::NINETY_DEGREES_AS_RADIANS);
	currentMatrix = currentMatrix * multiplyMatrix.m_MatrixStack[multiplyMatrix.m_MatrixStack.size()-1];

	multiplyMatrix.SetToRotationMatrixX(-1.f * camera.m_cameraRoll);
	currentMatrix = currentMatrix * multiplyMatrix.m_MatrixStack[multiplyMatrix.m_MatrixStack.size()-1];

	multiplyMatrix.SetToRotationMatrixY(-1.f * camera.m_cameraPitch);
	currentMatrix = currentMatrix * multiplyMatrix.m_MatrixStack[multiplyMatrix.m_MatrixStack.size()-1];

	multiplyMatrix.SetToRotationMatrixZ(-1.f * camera.m_cameraYaw);
	currentMatrix = currentMatrix * multiplyMatrix.m_MatrixStack[multiplyMatrix.m_MatrixStack.size()-1];

	multiplyMatrix.SetToTranslationMatrix(-camera.m_cameraPosition.x, -camera.m_cameraPosition.y, -camera.m_cameraPosition.z);
	currentMatrix = currentMatrix * multiplyMatrix.m_MatrixStack[multiplyMatrix.m_MatrixStack.size()-1];
}