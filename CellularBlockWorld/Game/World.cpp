#include "World.hpp"

static bool leftMouseButtonDown = false;
static bool rightMouseButtonDown = false;

//----------------------------------------------------
World::World(  ) 
	: m_mousePositionXDifference(0.f)
	, m_mousePositionZDifference(0.f)
	, m_is2DWorld(false)
	, m_isPaused(true)
	, m_lastCellularAutomataTime(0.0)
	, m_cellFocusRange(0)
{
	srand ((unsigned int)(time(NULL)));

	m_camera = Camera();
	m_renderer = OpenGLRenderer();
	m_keyIsHeld = false;
}

//----------------------------------------------------
void World::Initialize() {
	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		m_worldBlockCells.push_back(CubeCell(false));
	}
	BuildZone3D();
}

//----------------------------------------------------
void World::BuildZone2D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	//m_solidBlocks.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);
	//m_temporaryCellularVector.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);

	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++) {
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE_2D ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_worldBlockCells[index].m_isSolid = true;

			if ( (rand() % 100) < ConstantParameters::BLACK_BLOCK_PERCENTAGE ) {
				m_worldBlockCells[index].m_isBlack = true;
				m_renderer.AddBlackCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			} else {
				m_worldBlockCells[index].m_isBlack = false;
				m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			}
		}
		else {
			m_worldBlockCells[index].m_isSolid = false;
		}

		m_temporaryCellularVector.push_back((CubeCell(false)));
	}

	m_renderer.PushCubeVerticesToVBO();
}

//----------------------------------------------------
void World::BuildZone3D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	//m_solidBlocks.clear();
	//m_temporaryCellularVector.clear();

	//m_solidBlocks.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);
	m_temporaryCellularVector.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE_3D ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_worldBlockCells[index].m_isSolid = true;
			if ( (rand() % 100) < ConstantParameters::BLACK_BLOCK_PERCENTAGE ) {
				m_worldBlockCells[index].m_isBlack = true;
				m_renderer.AddBlackCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			} else {
				m_worldBlockCells[index].m_isBlack = false;
				m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			}
		}
		else {
			m_worldBlockCells[index].m_isSolid = false;
		}

		m_temporaryCellularVector.push_back((CubeCell(false)));
	}

	m_renderer.PushCubeVerticesToVBO();
}

//----------------------------------------------------
bool World::ProcessKeyDownEvent( unsigned char virtualKeyCode) {
	m_isKeyDown[ virtualKeyCode ] = true;
	return true;
}

//----------------------------------------------------
bool World::ProcessKeyUpEvent( unsigned char virtualKeyCode) {
	m_isKeyDown[ virtualKeyCode ] = false;
	return true;
}

//----------------------------------------------------
void World::UpdateCameraFromInput( float deltaSeconds ) {
	m_camera.m_cameraPitch -= deltaSeconds * m_mousePositionZDifference * ConstantParameters::MOUSE_SPEED;
	m_camera.m_cameraYaw += deltaSeconds * m_mousePositionXDifference * ConstantParameters::MOUSE_SPEED;
}

//----------------------------------------------------
void World::FindCameraFacingVector() {
	m_camera.m_cameraFacingVector.x = cos(m_camera.m_cameraYaw) * cos(m_camera.m_cameraPitch);
	m_camera.m_cameraFacingVector.y = sin(m_camera.m_cameraYaw) * cos(m_camera.m_cameraPitch);
	m_camera.m_cameraFacingVector.z = -sin(m_camera.m_cameraPitch);
}

//----------------------------------------------------
void World::UpdateFromMouseInput() {
	// Only allow one press per frame
	bool mouseButtonPressed = false;
	
	//Check the mouse left button 
	if ( (GetKeyState(VK_LBUTTON) & 0x8000) != 0 ) 
	{
		if ( !leftMouseButtonDown ) 
		{
			TurnTargetedCellToSolid();
		}

		leftMouseButtonDown = true;
		mouseButtonPressed = true;
	} 
	else 
	{
		leftMouseButtonDown = false;
	}

	if ( mouseButtonPressed )
	{
		return;
	}

	//Check the mouse right button 
	if ( (GetKeyState(VK_RBUTTON) & 0x8000) != 0 ) 
	{
		if ( !rightMouseButtonDown ) 
		{
			TurnTargetedCellToNonSolid();
		} 
		rightMouseButtonDown = true;
	} 
	else 
	{
		rightMouseButtonDown = false;
	}
}

//----------------------------------------------------
void World::UpdatePlayerFromInput( float deltaSeconds ) {	
	if ( m_isKeyDown[ 'P' ] && !m_keyIsHeld ) {
		m_keyIsHeld = true;
		m_isPaused = !m_isPaused;
	}
	
	if ( m_isKeyDown[ 'T' ] && !m_keyIsHeld ) {
		m_keyIsHeld = true;
		m_renderer.DeleteBuffers();
		if (m_is2DWorld) {
			GameOfLifeCellularAutomataPass2D();
		} else {
			GameOfLifeCellularAutomataPass3D();
		}
	}

	if ( m_isKeyDown[ 'R' ] && !m_keyIsHeld ) {
		m_keyIsHeld = true;
		m_renderer.DeleteBuffers();
		BuildZone3D();
		m_is2DWorld = false;
	}

	if ( m_isKeyDown[ 'E' ] ) {
		m_renderer.DeleteBuffers();
		BuildZone2D();
		m_is2DWorld = true;
	}
	
	if (m_isKeyDown[ VK_LEFT ]) {
		m_camera.m_cameraYaw += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_RIGHT ]) {
		m_camera.m_cameraYaw -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_UP ]) {
		m_camera.m_cameraPitch -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_DOWN ]) {
		m_camera.m_cameraPitch += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'W' ]) {
		m_camera.m_cameraPosition.x += cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.y += sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'D' ]) {
		m_camera.m_cameraPosition.y -= cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.x += sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'S' ]) {
		m_camera.m_cameraPosition.x -= cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.y -= sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'A' ]) {
		m_camera.m_cameraPosition.y += cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.x -= sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_SPACE ]) {
		m_camera.m_cameraPosition.z += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[VK_CONTROL]) {
		m_camera.m_cameraPosition.z -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if ( !( m_isKeyDown[ 'T' ] || m_isKeyDown[ 'R' ] || m_isKeyDown[ 'P' ]) ) {
		m_keyIsHeld = false;
	}
}

//----------------------------------------------------
void World::CheckForGimbleLock() {
	if (m_camera.m_cameraPitch >= ConstantParameters::CAMERA_Z_MAX) {
		m_camera.m_cameraPitch = ConstantParameters::CAMERA_Z_MAX - 0.005f;
	}

	if (m_camera.m_cameraPitch <= -ConstantParameters::CAMERA_Z_MAX) {
		m_camera.m_cameraPitch = -ConstantParameters::CAMERA_Z_MAX + 0.005f;
	}
}

//----------------------------------------------------
void World::Update() {
	float currentTime = (float) Time::GetCurrentTimeSeconds();
	float deltaSeconds = ConstantParameters::DELTA_SECONDS; // Hack: assume 60 FPS
	//	GetCursorPos()
	UpdatePlayerFromInput( deltaSeconds );
	UpdateCameraFromInput( deltaSeconds );
	UpdateFromMouseInput();
	FindCameraFacingVector();
	CheckForGimbleLock();

	if (!m_isPaused) {
		if (m_is2DWorld) {
			m_renderer.DeleteBuffers();
			GameOfLifeCellularAutomataPass2D();
		} else {
			if ( ( currentTime - m_lastCellularAutomataTime ) > ConstantParameters::TIME_BETWEEN_CA_STEPS ) {
				m_lastCellularAutomataTime = currentTime;
				m_renderer.DeleteBuffers();
				GameOfLifeCellularAutomataPass3D();
			}
		}
	}

	//GetAllCellsInRayTrace();
	//CapFocus();
}

//----------------------------------------------------
void World::Render() {	
	m_renderer.SetModelViewProjectionMatrix(m_camera);
	m_renderer.SendCubeVBO();
	m_renderer.PopMatrix();
}

//----------------------------------------------------
void World::GameOfLifeCellularAutomataPass2D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int SOLID_COUNTER = 0;
	unsigned int BLACK_COUNTER = 0;
	unsigned int COLORED_COUNTER = 0;

	unsigned int surroundingCellIndices[8];
	const unsigned int NUM_SURROUNDING_CELLS = 8;
	
	for ( unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++ ) {
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( x > 0 ) {
			surroundingCellIndices[0] = index - 1; //left
		} else {
			surroundingCellIndices[0] = index + ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //left
		}

		if ( y > 0 ) {
			surroundingCellIndices[1] = index - ConstantParameters::BLOCKS_Y_AXIS; //bottom
			surroundingCellIndices[2] = surroundingCellIndices[0] - ConstantParameters::BLOCKS_Y_AXIS; //bottom left
		} else {
			surroundingCellIndices[1] = index + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //bottom
			surroundingCellIndices[2] = surroundingCellIndices[0] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //bottom left
		}

		if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
			surroundingCellIndices[3] = index + ConstantParameters::BLOCKS_Y_AXIS; //top
			surroundingCellIndices[4] = surroundingCellIndices[0] + ConstantParameters::BLOCKS_Y_AXIS; //top left
		} else {
			surroundingCellIndices[3] = index - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //top
			surroundingCellIndices[4] = surroundingCellIndices[0] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //top left
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {
			surroundingCellIndices[5] = index + 1; //right
			surroundingCellIndices[6] = surroundingCellIndices[1] + 1; //bottom right
			surroundingCellIndices[7] = surroundingCellIndices[3] + 1; //top right
		} else {
			surroundingCellIndices[5] = index - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //right
			surroundingCellIndices[6] = surroundingCellIndices[1] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //bottom right
			surroundingCellIndices[7] = surroundingCellIndices[3] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //top right
		}

		
		for ( unsigned int surroundingIndex = 0; surroundingIndex < NUM_SURROUNDING_CELLS; surroundingIndex++ ) {
			if ( m_worldBlockCells[ surroundingCellIndices[surroundingIndex] ].m_isSolid ) {
				SOLID_COUNTER++;
				if ( m_worldBlockCells[ surroundingCellIndices[surroundingIndex] ].m_isBlack ) {
					BLACK_COUNTER++;
				} else {
					COLORED_COUNTER++;
				}
			}
		}

		if ( SOLID_COUNTER == ConstantParameters::PRODUCE_LIFE_THRESHOLD_2D ) {
			m_temporaryCellularVector[index].m_isSolid = true;

			if ( BLACK_COUNTER > COLORED_COUNTER ) {
				m_temporaryCellularVector[index].m_isBlack = true;
			} else {
				m_temporaryCellularVector[index].m_isBlack = false;
			}
		} 
		else if ( ( SOLID_COUNTER <= ConstantParameters::REMOVE_LIFE_THRESHOLD_LOWERBOUND_2D ) || ( SOLID_COUNTER >= ConstantParameters::REMOVE_LIFE_THRESHOLD_UPPERBOUND_2D ) ) {
			m_temporaryCellularVector[index].m_isSolid = false;
		}
		else {
			m_temporaryCellularVector[index].m_isSolid = m_worldBlockCells[index].m_isSolid;
			m_temporaryCellularVector[index].m_isBlack = m_worldBlockCells[index].m_isBlack;
		}

		SOLID_COUNTER = 0;
		BLACK_COUNTER = 0;
		COLORED_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++) {
		m_worldBlockCells[index] = m_temporaryCellularVector[index];

		if ( m_worldBlockCells[index].m_isSolid ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			if ( m_worldBlockCells[index].m_isBlack ) {
				m_renderer.AddBlackCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			} else {
				m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			}
		}
	}

	m_renderer.PushCubeVerticesToVBO();
}

//----------------------------------------------------
void World::GameOfLifeCellularAutomataPass3D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int SOLID_COUNTER = 0;
	unsigned int BLACK_COUNTER = 0;
	unsigned int COLORED_COUNTER = 0;

	unsigned int surroundingCellIndices[26];
	const unsigned int NUM_SURROUNDING_CELLS = 26;
	
	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( z < ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ) {
			surroundingCellIndices[8] = index + ConstantParameters::WIDTH_TIMES_HEIGHT; //above
		} else {
			surroundingCellIndices[8] = index - ( ConstantParameters::WIDTH_TIMES_HEIGHT * ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ); //above
		}

		if ( z > 0 ) {
			surroundingCellIndices[9] = index - ConstantParameters::WIDTH_TIMES_HEIGHT; //below
		} else {
			surroundingCellIndices[9] = index + ( ConstantParameters::WIDTH_TIMES_HEIGHT * ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ); //below
		}

		if ( x > 0 ) {
			surroundingCellIndices[0] = index - 1; //left
			surroundingCellIndices[10] = surroundingCellIndices[8] - 1; //above left
			surroundingCellIndices[11] = surroundingCellIndices[9] - 1; //below left
		} else {
			surroundingCellIndices[0] = index + ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //left
			surroundingCellIndices[10] = surroundingCellIndices[8] + ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //above left
			surroundingCellIndices[11] = surroundingCellIndices[9] + ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //below left
		}

		if ( y > 0 ) {
			surroundingCellIndices[1] = index - ConstantParameters::BLOCKS_Y_AXIS; //bottom
			surroundingCellIndices[12] = surroundingCellIndices[8] - ConstantParameters::BLOCKS_Y_AXIS; //above bottom
			surroundingCellIndices[13] = surroundingCellIndices[9] - ConstantParameters::BLOCKS_Y_AXIS; //below bottom
			surroundingCellIndices[2] = surroundingCellIndices[0] - ConstantParameters::BLOCKS_Y_AXIS; //bottom left
			surroundingCellIndices[14] = surroundingCellIndices[10] - ConstantParameters::BLOCKS_Y_AXIS; //above bottom left
			surroundingCellIndices[15] = surroundingCellIndices[11] - ConstantParameters::BLOCKS_Y_AXIS; //below bottom left
		} else {
			surroundingCellIndices[1] = index + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //bottom
			surroundingCellIndices[12] = surroundingCellIndices[8] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //above bottom
			surroundingCellIndices[13] = surroundingCellIndices[9] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //below bottom
			surroundingCellIndices[2] = surroundingCellIndices[0] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //bottom left
			surroundingCellIndices[14] = surroundingCellIndices[10] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //above bottom left
			surroundingCellIndices[15] = surroundingCellIndices[11] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //below bottom left
		}

		if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
			surroundingCellIndices[3] = index + ConstantParameters::BLOCKS_Y_AXIS; //top
			surroundingCellIndices[16] = surroundingCellIndices[8] + ConstantParameters::BLOCKS_Y_AXIS; //above top
			surroundingCellIndices[17] = surroundingCellIndices[9] + ConstantParameters::BLOCKS_Y_AXIS; //below top
			surroundingCellIndices[4] = surroundingCellIndices[0] + ConstantParameters::BLOCKS_Y_AXIS; //top left
			surroundingCellIndices[18] = surroundingCellIndices[10] + ConstantParameters::BLOCKS_Y_AXIS; //above top left
			surroundingCellIndices[19] = surroundingCellIndices[11] + ConstantParameters::BLOCKS_Y_AXIS; //below top left
		} else {
			surroundingCellIndices[3] = index - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //top
			surroundingCellIndices[16] = surroundingCellIndices[8] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //above top
			surroundingCellIndices[17] = surroundingCellIndices[9] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //below top
			surroundingCellIndices[4] = surroundingCellIndices[0] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //top left
			surroundingCellIndices[18] = surroundingCellIndices[10] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //above top left
			surroundingCellIndices[19] = surroundingCellIndices[11] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //below top left
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {
			surroundingCellIndices[5] = index + 1; //right
			surroundingCellIndices[20] = surroundingCellIndices[8] + 1; //above right
			surroundingCellIndices[21] = surroundingCellIndices[9] + 1; //below right
			surroundingCellIndices[6] = surroundingCellIndices[1] + 1; //bottom right
			surroundingCellIndices[22] = surroundingCellIndices[12] + 1; //above bottom right
			surroundingCellIndices[23] = surroundingCellIndices[13] + 1; //below bottom right
			surroundingCellIndices[7] = surroundingCellIndices[3] + 1; //top right
			surroundingCellIndices[24] = surroundingCellIndices[16] + 1; //above top right
			surroundingCellIndices[25] = surroundingCellIndices[17] + 1; //below top right
		} else {
			surroundingCellIndices[5] = index - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //right
			surroundingCellIndices[20] = surroundingCellIndices[8] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //above right
			surroundingCellIndices[21] = surroundingCellIndices[9] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //below right
			surroundingCellIndices[6] = surroundingCellIndices[1] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //bottom right
			surroundingCellIndices[22] = surroundingCellIndices[12] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //above bottom right
			surroundingCellIndices[23] = surroundingCellIndices[13] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //below bottom right
			surroundingCellIndices[7] = surroundingCellIndices[3] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //top right
			surroundingCellIndices[24] = surroundingCellIndices[16] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //above top right
			surroundingCellIndices[25] = surroundingCellIndices[17] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //below top right
		}

		for ( unsigned int surroundingIndex = 0; surroundingIndex < NUM_SURROUNDING_CELLS; surroundingIndex++ ) {
			if ( m_worldBlockCells[ surroundingCellIndices[surroundingIndex] ].m_isSolid ) {
				SOLID_COUNTER++;
				if ( m_worldBlockCells[ surroundingCellIndices[surroundingIndex] ].m_isBlack ) {
					BLACK_COUNTER++;
				} else {
					COLORED_COUNTER++;
				}
			}
		}

		if ( ( SOLID_COUNTER > ( ConstantParameters::PRODUCE_LIFE_THRESHOLD_LOWERBOUND_3D - 1 ) ) && ( SOLID_COUNTER < ( ConstantParameters::PRODUCE_LIFE_THRESHOLD_UPPERBOUND_3D + 1 ) ) ) {
			m_temporaryCellularVector[index].m_isSolid = true;

			if ( BLACK_COUNTER > COLORED_COUNTER ) {
				m_temporaryCellularVector[index].m_isBlack = true;
			} else {
				m_temporaryCellularVector[index].m_isBlack = false;
			}
		} 
		else if ( ( SOLID_COUNTER <= ConstantParameters::REMOVE_LIFE_THRESHOLD_LOWERBOUND_3D ) || ( SOLID_COUNTER >= ConstantParameters::REMOVE_LIFE_THRESHOLD_UPPERBOUND_3D ) ) {
			m_temporaryCellularVector[index].m_isSolid = false;
		}
		else {
			m_temporaryCellularVector[index].m_isSolid = m_worldBlockCells[index].m_isSolid;
			m_temporaryCellularVector[index].m_isBlack = m_worldBlockCells[index].m_isBlack;
		}

		SOLID_COUNTER = 0;
		BLACK_COUNTER = 0;
		COLORED_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		m_worldBlockCells[index] = m_temporaryCellularVector[index];

		if ( m_worldBlockCells[index].m_isSolid ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			if ( m_worldBlockCells[index].m_isBlack ) {
				m_renderer.AddBlackCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			} else {
				m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			}
		}
	}

	m_renderer.PushCubeVerticesToVBO();
}

//---------------------------------------------
void World::GetAllCellsInRayTrace() {
	m_raytraceCells.clear();

	bool foundBlock = false;

	float xPos = 0.f;
	float yPos = 0.f;
	float zPos = 0.f;
	float range = 0.f;
	for (float counter = 0.f; counter <= ConstantParameters::NUMBER_OF_STEPS_IN_STEP_AND_SAMPLE; counter += 1.f){
		range = ConstantParameters::BLOCK_HIGHLIGHT_RANGE * counter*ConstantParameters::NUMBER_OF_STEPS_IN_STEP_AND_SAMPLE_INVERSE;
		xPos = m_camera.m_cameraPosition.x + (m_camera.m_cameraFacingVector.x * range);
		yPos = m_camera.m_cameraPosition.y + (m_camera.m_cameraFacingVector.y * range);
		zPos = m_camera.m_cameraPosition.z + (m_camera.m_cameraFacingVector.z * range);

		if ( ( (xPos < ConstantParameters::BLOCKS_X_AXIS) && ( xPos >= 0.f ) ) && 
			( (yPos < ConstantParameters::BLOCKS_Y_AXIS) && ( yPos >= 0.f ) ) && 
			( (zPos < ConstantParameters::BLOCKS_Z_AXIS) && ( zPos >= 0.f ) ) )
		{
			int x = (int) xPos;
			int y = (int) yPos;
			int z = (int) zPos;

			int index = ( x + ( y * ConstantParameters::BLOCKS_Y_AXIS ) + ( z * ConstantParameters::BLOCKS_X_AXIS * ConstantParameters::BLOCKS_Y_AXIS ) );

			if ( !m_raytraceCells.empty() ) {
				for (unsigned int counter = 0; counter < m_raytraceCells.size(); counter++) {
					if ( m_raytraceCells[counter] == index ) {
						foundBlock = true;
						break;
					}
				}

				if (!foundBlock) {
					m_raytraceCells.push_back( index );
				}
			} else {
				m_raytraceCells.push_back( index );
			}
		}
		foundBlock = false;
	}
}

//--------------------------------------------
void World::IncreaseFocus() {
	m_cellFocusRange++;
	if (m_cellFocusRange >= m_raytraceCells.size() ) {
		m_cellFocusRange = m_raytraceCells.size() - 1;
	}
}

//--------------------------------------------
void World::ReduceFocus() {
	m_cellFocusRange--;
	if (m_cellFocusRange < 0) {
		m_cellFocusRange = 0;
	}
}

//--------------------------------------------
void World::CapFocus() {
	if (m_cellFocusRange >= m_raytraceCells.size() ) {
		m_cellFocusRange = m_raytraceCells.size() - 1;
	}

	if (m_cellFocusRange < 0) {
		m_cellFocusRange = 0;
	}
}

//--------------------------------------------
void World::TurnTargetedCellToSolid() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;
	
	if ( !m_raytraceCells.empty() ) {
		x = m_raytraceCells[m_cellFocusRange] & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( !m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_isSolid ) {
			m_renderer.DeleteBuffers();
			m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_isSolid = true;

			for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
				if ( m_worldBlockCells[index].m_isSolid ) {
					x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
					y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
					z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
					m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
				}
			}
			m_renderer.PushCubeVerticesToVBO();
		}
	}
}

//--------------------------------------------
void World::TurnTargetedCellToNonSolid() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;
	
	if ( !m_raytraceCells.empty() ) {
		x = m_raytraceCells[m_cellFocusRange] & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_isSolid ) {
			m_renderer.DeleteBuffers();
			m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_isSolid = false;

			for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
				if ( m_worldBlockCells[index].m_isSolid ) {
					x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
					y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
					z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
					m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
				}
			}
			m_renderer.PushCubeVerticesToVBO();
		}
	}
}


