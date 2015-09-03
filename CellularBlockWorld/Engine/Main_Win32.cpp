#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include "../Game/World.hpp"
#include "Time/Time.hpp"
#include "Constants.hpp"
#include "Camera/Camera.hpp"
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment( lib, "glu32") // Link in the GLu.lib static library

//-----------------------------------
const int SCREEN_WIDTH = 1600; //1024;
const int SCREEN_HEIGHT = 900; //576;
const float TILEWORLD_WIDTH = 45.f;
const float TILEWORLD_HEIGHT = 45.f;
const double FRAMETIME = 1/60.f;
const int HALF_SCREEN_WIDTH = SCREEN_WIDTH / 2;
const int HALF_SCREEN_HEIGHT = SCREEN_HEIGHT / 2;

//-------------------------------------------------------------------------------------------------
World g_theWorld;
Camera g_myCamera;
bool g_windowHasFocus;


//-----------------------------------------------------------------------------------------------
#define UNUSED(x) (void)(x);


//-----------------------------------------------------------------------------------------------
bool g_isQuitting = false;
HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "Win32 OpenGL Minimum Test App";

//-----------------------------------------------------------------------------------------------
void moveInCameraFacingDirection() {
	g_myCamera.m_cameraPosition.x += 0.1f * sin(g_myCamera.m_cameraPitch);
	g_myCamera.m_cameraPosition.y += 0.1f * -sin(g_myCamera.m_cameraRoll);
	g_myCamera.m_cameraPosition.z += 0.1f * -cos(g_myCamera.m_cameraPitch);
}

//-----------------------------------------------------------------------------------------------
void moveOppositeOfCameraFacingDirection() {
	g_myCamera.m_cameraPosition.x += 0.1f * sin(g_myCamera.m_cameraPitch);
	g_myCamera.m_cameraPosition.y -= 0.1f * -sin(g_myCamera.m_cameraRoll);
	g_myCamera.m_cameraPosition.z += 0.1f * cos(g_myCamera.m_cameraPitch);
}

//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam ) {
	switch( wmMessageCode ) {
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			g_isQuitting = true;
			return 0;

		case WM_KEYDOWN:
			{
				unsigned char asKey = (unsigned char) wParam;
				if ( asKey == VK_ESCAPE ) {
					g_isQuitting = true;
					return 0;
				}

				bool wasProcessed = g_theWorld.ProcessKeyDownEvent( asKey );
				if ( wasProcessed ) {
					return 0;
				}

				break;

			}

		case WM_KEYUP:
			{

				unsigned char asKey = (unsigned char) wParam;
				bool wasProcessed = g_theWorld.ProcessKeyUpEvent( asKey );
				if ( wasProcessed ) {
					return 0;
				}

				break;
			}
		
		case WM_ACTIVATE:
			{
				if ( wParam == WA_ACTIVE ) {
					g_windowHasFocus = true;
					break;
				}
				else if ( wParam == WA_INACTIVE) {
					g_windowHasFocus = false;
					break;
				}
				else if ( wParam == WA_CLICKACTIVE ) {
					g_windowHasFocus = true;
					break;
				}
			}

		case WM_KILLFOCUS:
			{
				g_windowHasFocus = false;
				break;
			}
	}

// 	if ( wParam == WA_ACTIVE )
// 		g_windowHasFocus = true;
// 
// 	if ( wParam == WA_INACTIVE )
// 		g_windowHasFocus = false;

	if ( g_windowHasFocus ) {
		POINT cursorPosition;
		BOOL cursorPositionResult = GetCursorPos(&cursorPosition);

		if (cursorPositionResult) {
			g_theWorld.m_mousePositionXDifference = (float)(HALF_SCREEN_WIDTH - cursorPosition.x);
			g_theWorld.m_mousePositionZDifference = (float)(HALF_SCREEN_HEIGHT- cursorPosition.y);
		}
	}
	else {
		g_theWorld.m_mousePositionXDifference = 0.f;
		g_theWorld.m_mousePositionZDifference = 0.f;
	}

	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}


//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow( HINSTANCE applicationInstanceHandle ) {
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( WindowsMessageHandlingProcedure ); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" );
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );

	RECT windowRect = { 50, 50, 50 + SCREEN_WIDTH, 50 + SCREEN_HEIGHT };
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle)/sizeof(windowTitle[0]) );
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL );

	ShowWindow( g_hWnd, SW_SHOW );
	SetForegroundWindow( g_hWnd );
	SetFocus( g_hWnd );

	g_displayDeviceContext = GetDC( g_hWnd );

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize			= sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion		= 1;
	pixelFormatDescriptor.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType	= PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits	= 24;
	pixelFormatDescriptor.cDepthBits	= 24;
	pixelFormatDescriptor.cAccumBits	= 0;
	pixelFormatDescriptor.cStencilBits	= 8;

	int pixelFormatCode = ChoosePixelFormat( g_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	g_openGLRenderingContext = wglCreateContext( g_displayDeviceContext );
	wglMakeCurrent( g_displayDeviceContext, g_openGLRenderingContext );

	//Set up OpenGL states the way we want em
	glEnable( GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_CULL_FACE );
	glFrontFace( GL_CCW );
	glCullFace( GL_BACK );
	glEnable(GL_DEPTH_TEST);


	//glOrtho( 0.f, TILEWORLD_WIDTH, 0.f, TILEWORLD_HEIGHT, 0.f, 1.f );
	//glOrtho( 0.f, (float) SCREEN_WIDTH, 0.f, (float) SCREEN_HEIGHT, 0.f, 1.f );
}


//-----------------------------------------------------------------------------------------------
void RunMessagePump() {
	MSG queuedMessage;
	for ( ;; ) {
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if ( !wasMessagePresent ) {
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}

//-----------------------------------------------------------------------------------------------
void Update() {
	g_theWorld.Update();
	if (g_windowHasFocus) {
		SetCursorPos(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT);
	}
}

//-----------------------------------------------------------------------------------------------
void Render() {
	glClearColor( 0.5f, 0.5f, 0.5f, 1.f );
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_theWorld.Render();
}

//-----------------------------------------------------------------------------------------------
void WaitUntilNextFrameTime() {
	static double timeAtLastUpdate = Time::GetCurrentTimeSeconds();

	double timeNow = Time::GetCurrentTimeSeconds();
	static double targetTime = timeNow;
	while ( timeNow < targetTime ) {
		timeNow = Time::GetCurrentTimeSeconds();
	}
	targetTime = timeNow + FRAMETIME;
}

//-----------------------------------------------------------------------------------------------
void RunFrame() {
	RunMessagePump();
	Update();
	Render();
	WaitUntilNextFrameTime();
	SwapBuffers( g_displayDeviceContext );
}


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int ) {
	UNUSED( commandLineString );
	g_windowHasFocus = true;
	CreateOpenGLWindow( applicationInstanceHandle );
	g_theWorld.m_renderer.Initialize();
	g_theWorld.Initialize();
	SetCursorPos(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT);
	ShowCursor(false);
	while ( !g_isQuitting ) {
		RunFrame();
	}
	ShowCursor(true);

#if defined( _WIN32 ) && defined( _DEBUG )
	assert( _CrtCheckMemory() );
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}