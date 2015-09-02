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
	void Initialize();
	void BuildZone2D();
	void BuildZone3D();

	void TestSolidCellularAutomataPass();

	void GameOfLifeCellularAutomataPass2D();
	void GameOfLifeCellularAutomataPass3D();

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

	bool m_is2DWorld;
	bool m_isPaused;

	double m_lastCellularAutomataTime;
};

#endif //included_World