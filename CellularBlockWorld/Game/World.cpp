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

	int counter = 0;

	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++) {
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE_2D ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_worldBlockCells[index] = true;
			m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));

			counter++;
		}
		else {
			m_worldBlockCells[index] = false;
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

	int counter = 0;

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE_3D ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_worldBlockCells[index] = true;
			m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
			counter++;
		}
		else {
			m_worldBlockCells[index] = false;
		}

		m_temporaryCellularVector.push_back((CubeCell(false)));
	}

	m_renderer.PushCubeVerticesToVBO();
}

//----------------------------------------------------
void World::TestSolidCellularAutomataPass() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int THRESHOLD = 5;
	unsigned int SOLID_COUNTER = 0;

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;
		
		//ABOVE
		if ( z < ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ) {
			if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid ) {	//above
				SOLID_COUNTER++;
			}

			if ( x > 0 ) {	//above left
				if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid ) {	//above left
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above left bottom
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
				
				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//above right
				if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid ) {	//above right
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above right bottom
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above right top
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}
		}

		//BOTTOM
		if ( z > 0 ) {
			if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid ) {	//above
				SOLID_COUNTER++;
			}

			if ( x > 0 ) {	//above left
				if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid ) {	//above left
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above left bottom
					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//above right
				if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid ) {	//above right
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above left bottom
					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}
		}

		//MIDDLE
		if ( x > 0 ) {	//above left
			if ( m_worldBlockCells[ index - 1 ].m_isSolid ) {	//above left
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {	//above left bottom
				if ( m_worldBlockCells[ index - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
				if ( m_worldBlockCells[ index - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//above right
			if ( m_worldBlockCells[ index + 1 ].m_isSolid ) {	//above right
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {	//above left bottom
				if ( m_worldBlockCells[ index + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
				if ( m_worldBlockCells[ index + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}
		}

		if ( SOLID_COUNTER >= THRESHOLD ) {
			m_temporaryCellularVector[index].m_isSolid = true;
		}
		else {
			m_temporaryCellularVector[index].m_isSolid = false;
		}

		SOLID_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		m_worldBlockCells[index] = m_temporaryCellularVector[index];

		if ( m_worldBlockCells[index].m_isSolid ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
			m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
		}
	}

	m_renderer.PushCubeVerticesToVBO();
	//m_temporaryCellularVector.clear();
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
		GameOfLifeCellularAutomataPass3D();
		//TestSolidCellularAutomataPass();
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
// 	unsigned int x = 0;
// 	unsigned int y = 0;
// 	unsigned int z = 0;

	m_renderer.SetModelViewProjectionMatrix(m_camera);
	m_renderer.SendCubeVBO();

// 	if ( !m_raytraceCells.empty() ) {
// 		x = m_raytraceCells[m_cellFocusRange] & ConstantParameters::BLOCKS_X_AXIS - 1;
// 		y = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
// 		z = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;
// 
// 		m_renderer.SendViewMatrix(m_camera);
// 		m_renderer.DrawTargetCellOutline(Vector3((float) x, (float) y, (float) z));
// 	}

	m_renderer.PopMatrix();
}

//----------------------------------------------------
void World::GameOfLifeCellularAutomataPass2D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int SOLID_COUNTER = 0;
	
	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++) {
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( x > 0 ) {	//left
			if ( m_worldBlockCells[ index - 1 ].m_isSolid ) {	//left
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {
				if ( m_worldBlockCells[ index - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//bottom
					SOLID_COUNTER++;
				}

				if ( m_worldBlockCells[ index - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//left bottom
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
				if ( m_worldBlockCells[ index + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //top
					SOLID_COUNTER++;
				}
				
				if ( m_worldBlockCells[ index - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//left top
					SOLID_COUNTER++;
				}
			}

		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//right
			if ( m_worldBlockCells[ index + 1 ].m_isSolid ) {	//right
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {	//bottom right
				if ( m_worldBlockCells[ index + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above right
				if ( m_worldBlockCells[ index + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}
		}

		if ( SOLID_COUNTER == ConstantParameters::PRODUCE_LIFE_THRESHOLD_2D ) {
			m_temporaryCellularVector[index].m_isSolid = true;
		} 
		else if ( ( SOLID_COUNTER <= ConstantParameters::REMOVE_LIFE_THRESHOLD_LOWERBOUND_2D ) || ( SOLID_COUNTER >= ConstantParameters::REMOVE_LIFE_THRESHOLD_UPPERBOUND_2D ) ) {
			m_temporaryCellularVector[index].m_isSolid = false;
		}
		else {
			m_temporaryCellularVector[index].m_isSolid = m_worldBlockCells[index].m_isSolid;
		}

		SOLID_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++) {
		m_worldBlockCells[index] = m_temporaryCellularVector[index];

		if ( m_worldBlockCells[index].m_isSolid ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
			m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
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
	
	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		//Above
		if ( z < ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ) {
			if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid ) {	//above
				SOLID_COUNTER++;
			}

			if ( x > 0 ) {	//above left
				if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid ) {	//above left
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//above bottom
						SOLID_COUNTER++;
					}
					
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //above left bottom
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//above top
						SOLID_COUNTER++;
					}

					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //above left top
						SOLID_COUNTER++;
					}
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//above right
				if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid ) {	//above right
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above right bottom
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above right top
					if ( m_worldBlockCells[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}
		}

		//Below
		if ( z > 0 ) {
			if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid ) {	//below
				SOLID_COUNTER++;
			}

			if ( x > 0 ) {	//below left
				if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid ) {	//below left
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {
					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//below bottom
						SOLID_COUNTER++;
					}

					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //below left bottom
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//below top
						SOLID_COUNTER++;
					}

					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //below left top
						SOLID_COUNTER++;
					}
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//below right
				if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid ) {	//below right
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//below right bottom
					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//below right top
					if ( m_worldBlockCells[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}
		}

		//Middle
		if ( x > 0 ) {	//left
			if ( m_worldBlockCells[ index - 1 ].m_isSolid ) {	//left
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {
				if ( m_worldBlockCells[ index - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//bottom
					SOLID_COUNTER++;
				}

				if ( m_worldBlockCells[ index - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//left bottom
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
				if ( m_worldBlockCells[ index + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //top
					SOLID_COUNTER++;
				}

				if ( m_worldBlockCells[ index - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//left top
					SOLID_COUNTER++;
				}
			}
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//right
			if ( m_worldBlockCells[ index + 1 ].m_isSolid ) {	//right
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {	//bottom right
				if ( m_worldBlockCells[ index + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
					SOLID_COUNTER++;
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above right
				if ( m_worldBlockCells[ index + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
					SOLID_COUNTER++;
			}
		}

		if ( ( SOLID_COUNTER > ( ConstantParameters::PRODUCE_LIFE_THRESHOLD_LOWERBOUND_3D - 1 ) ) && ( SOLID_COUNTER < ( ConstantParameters::PRODUCE_LIFE_THRESHOLD_UPPERBOUND_3D + 1 ) ) ) {
			m_temporaryCellularVector[index].m_isSolid = true;
		} 
		else if ( ( SOLID_COUNTER <= ConstantParameters::REMOVE_LIFE_THRESHOLD_LOWERBOUND_3D ) || ( SOLID_COUNTER >= ConstantParameters::REMOVE_LIFE_THRESHOLD_UPPERBOUND_3D ) ) {
			m_temporaryCellularVector[index].m_isSolid = false;
		}
		else {
			m_temporaryCellularVector[index].m_isSolid = m_worldBlockCells[index].m_isSolid;
		}

		SOLID_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		m_worldBlockCells[index] = m_temporaryCellularVector[index];

		if ( m_worldBlockCells[index].m_isSolid ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
			m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z));
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


