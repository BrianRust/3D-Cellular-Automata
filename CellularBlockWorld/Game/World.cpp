
#include "World.hpp"

//----------------------------------------------------
World::World(  ) 
	: m_mousePositionXDifference(0.f)
	, m_mousePositionZDifference(0.f)
	, m_is2DWorld(false)
	, m_isPaused(false)
	, m_lastCellularAutomataTime(0.0)
{
	srand ((unsigned int)(time(NULL)));

	m_camera = Camera();
	m_renderer = OpenGLRenderer();
	m_keyIsHeld = false;
}

void World::Initialize() {
	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		m_solidBlocks.push_back(CubeCell(false));
	}
	BuildZone3D();
}

void World::BuildZone2D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	//m_solidBlocks.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);
	//m_temporaryCellularVector.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);

	int counter = 0;

	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_DEPTH; index++) {
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE_2D ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_solidBlocks[index] = true;
			m_renderer.AddCubeToBuffer(Vector3(x, y, z), RGBA(0.8f, 0.f, 0.f, 1.f));

			counter++;
		}
		else {
			m_solidBlocks[index] = false;
		}

		m_temporaryCellularVector.push_back((CubeCell(false)));
	}

	m_renderer.PushCubeVerticesToVBO();
	m_renderer.PushGridOutlineVerticesToVBO();
}

void World::BuildZone3D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	//m_solidBlocks.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);
	//m_temporaryCellularVector.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);

	int counter = 0;

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE_3D ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_solidBlocks[index] = true;
			m_renderer.AddCubeToBuffer(Vector3(x, y, z), RGBA(0.8f, 0.f, 0.f, 1.f));
			counter++;
		}
		else {
			m_solidBlocks[index] = false;
		}

		m_temporaryCellularVector.push_back((CubeCell(false)));
	}

	m_renderer.PushCubeVerticesToVBO();
	m_renderer.PushGridOutlineVerticesToVBO();
}

void World::TestSolidCellularAutomataPass() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int THRESHOLD = 5;
	unsigned int SOLID_COUNTER = 0;

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;
		
		//ABOVE
		if ( z < ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ) {
			if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid ) {	//above
				SOLID_COUNTER++;
			}

			if ( x > 0 ) {	//above left
				if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid ) {	//above left
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above left bottom
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
				
				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//above right
				if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid ) {	//above right
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above right bottom
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above right top
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}
		}

		//BOTTOM
		if ( z > 0 ) {
			if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid ) {	//above
				SOLID_COUNTER++;
			}

			if ( x > 0 ) {	//above left
				if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid ) {	//above left
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above left bottom
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//above right
				if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid ) {	//above right
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above left bottom
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}
		}

		//MIDDLE
		if ( x > 0 ) {	//above left
			if ( m_solidBlocks[ index - 1 ].m_isSolid ) {	//above left
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {	//above left bottom
				if ( m_solidBlocks[ index - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
				if ( m_solidBlocks[ index - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//above right
			if ( m_solidBlocks[ index + 1 ].m_isSolid ) {	//above right
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {	//above left bottom
				if ( m_solidBlocks[ index + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above left top
				if ( m_solidBlocks[ index + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
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
		m_solidBlocks[index] = m_temporaryCellularVector[index];

		if ( m_solidBlocks[index].m_isSolid ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
			m_renderer.AddCubeToBuffer(Vector3(x, y, z), RGBA(0.8f, 0.f, 0.f, 1.f));
		}
	}

	m_renderer.PushCubeVerticesToVBO();
	m_renderer.PushGridOutlineVerticesToVBO();
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
	float currentTime = Time::GetCurrentTimeSeconds();
	float deltaSeconds = ConstantParameters::DELTA_SECONDS; // Hack: assume 60 FPS
	//	GetCursorPos()
	UpdatePlayerFromInput( deltaSeconds );
	UpdateCameraFromInput( deltaSeconds );
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
}

//----------------------------------------------------
void World::Render() {	
	m_renderer.SendViewMatrix(m_camera);
	m_renderer.SendCubeVBO();
	m_renderer.PopMatrix();
}

//----------------------------------------------------
void World::GameOfLifeCellularAutomataPass2D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int SOLID_COUNTER = 0;
	
	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_DEPTH; index++) {
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( x > 0 ) {	//left
			if ( m_solidBlocks[ index - 1 ].m_isSolid ) {	//left
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {
				if ( m_solidBlocks[ index - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//bottom
					SOLID_COUNTER++;
				}

				if ( m_solidBlocks[ index - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//left bottom
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
				if ( m_solidBlocks[ index + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //top
					SOLID_COUNTER++;
				}
				
				if ( m_solidBlocks[ index - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//left top
					SOLID_COUNTER++;
				}
			}

		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//right
			if ( m_solidBlocks[ index + 1 ].m_isSolid ) {	//right
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {	//bottom right
				if ( m_solidBlocks[ index + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above right
				if ( m_solidBlocks[ index + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
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
			m_temporaryCellularVector[index].m_isSolid = m_solidBlocks[index].m_isSolid;
		}

		SOLID_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_DEPTH; index++) {
		m_solidBlocks[index] = m_temporaryCellularVector[index];

		if ( m_solidBlocks[index].m_isSolid ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
			m_renderer.AddCubeToBuffer(Vector3(x, y, z), RGBA(0.8f, 0.f, 0.f, 1.f));
		}
	}

	m_renderer.PushCubeVerticesToVBO();
	m_renderer.PushGridOutlineVerticesToVBO();
}

//----------------------------------------------------
void World::GameOfLifeCellularAutomataPass3D() {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int SOLID_COUNTER = 0;
	
	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		//Above
		if ( z < ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ) {
			if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid ) {	//above
				SOLID_COUNTER++;
			}

			if ( x > 0 ) {	//above left
				if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid ) {	//above left
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//above bottom
						SOLID_COUNTER++;
					}
					
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //above left bottom
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//above top
						SOLID_COUNTER++;
					}

					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //above left top
						SOLID_COUNTER++;
					}
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//above right
				if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid ) {	//above right
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//above right bottom
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above right top
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}
		}

		//Below
		if ( z > 0 ) {
			if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid ) {	//below
				SOLID_COUNTER++;
			}

			if ( x > 0 ) {	//below left
				if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid ) {	//below left
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//below bottom
						SOLID_COUNTER++;
					}

					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //below left bottom
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//below top
						SOLID_COUNTER++;
					}

					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //below left top
						SOLID_COUNTER++;
					}
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//below right
				if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid ) {	//below right
					SOLID_COUNTER++;
				}

				if ( y > 0 ) {	//below right bottom
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//below right top
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {
						SOLID_COUNTER++;
					}
				}
			}
		}

		//Middle
		if ( x > 0 ) {	//left
			if ( m_solidBlocks[ index - 1 ].m_isSolid ) {	//left
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {
				if ( m_solidBlocks[ index - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//bottom
					SOLID_COUNTER++;
				}

				if ( m_solidBlocks[ index - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//left bottom
					SOLID_COUNTER++;
				}
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {
				if ( m_solidBlocks[ index + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) { //top
					SOLID_COUNTER++;
				}

				if ( m_solidBlocks[ index - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid ) {	//left top
					SOLID_COUNTER++;
				}
			}
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) {	//right
			if ( m_solidBlocks[ index + 1 ].m_isSolid ) {	//right
				SOLID_COUNTER++;
			}

			if ( y > 0 ) {	//bottom right
				if ( m_solidBlocks[ index + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
					SOLID_COUNTER++;
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) {	//above right
				if ( m_solidBlocks[ index + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
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
			m_temporaryCellularVector[index].m_isSolid = m_solidBlocks[index].m_isSolid;
		}

		SOLID_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) {
		m_solidBlocks[index] = m_temporaryCellularVector[index];

		if ( m_solidBlocks[index].m_isSolid ) {
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
			m_renderer.AddCubeToBuffer(Vector3(x, y, z), RGBA(0.8f, 0.f, 0.f, 1.f));
		}
	}

	m_renderer.PushCubeVerticesToVBO();
	m_renderer.PushGridOutlineVerticesToVBO();
}
