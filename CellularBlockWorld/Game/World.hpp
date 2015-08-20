#ifndef included_World
#define included_World
#pragma once

//-------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"
#include "../Engine/Camera/Camera.hpp"
#include "../Engine/Time/Time.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"
#include "CubeCell.hpp"

//-------------------------------------------------------------
class World
{
public:
	World();
	bool ProcessKeyDownEvent( unsigned char virtualKeyCode);
	bool ProcessKeyUpEvent( unsigned char virtualKeyCode);
	void UpdatePlayerFromInput( float deltaseconds );
	void UpdateCameraFromInput( float deltaseconds );
	void CheckForGimbleLock();
	void BuildZone();

	void TestSolidCellularAutomataPass();

	void Update();
	void Render();

	bool m_isKeyDown[ 256 ];
	std::vector<CubeCell> m_solidBlocks;

	bool m_keyIsHeld;

	std::vector<CubeCell> m_temporaryCellularVector;

	Camera m_camera;
	OpenGLRenderer m_renderer;

	float m_mousePositionXDifference;
	float m_mousePositionZDifference;

	float m_textureX;
	float m_textureY;
};

#endif //included_World