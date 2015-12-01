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
void World::Initialize() 
{
	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) 
	{
		m_worldBlockCells.push_back(CubeCell(false));
	}
	BuildZone3D();
}

//----------------------------------------------------
void World::BuildZone2D() 
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	//m_solidBlocks.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);
	//m_temporaryCellularVector.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);

	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++) 
	{
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE_2D ) 
		{
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_worldBlockCells[index].m_isSolid = true;
			int randomNumber = (rand() % 100);

			if ( randomNumber > 98 ) 
			{
				m_worldBlockCells[index].m_cellType = 3;
			} 
			else if ( randomNumber > 65 )
			{
				m_worldBlockCells[index].m_cellType = 2;
			}
			else if ( randomNumber > 32 )
			{
				m_worldBlockCells[index].m_cellType = 1;
			}
			else
			{
				m_worldBlockCells[index].m_cellType = 0;
			}
			m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z), m_worldBlockCells[index].m_cellType);
		}
		else 
		{
			m_worldBlockCells[index].m_isSolid = false;
		}

		m_temporaryCellularVector.push_back((CubeCell(false)));
	}

	m_renderer.PushCubeVerticesToVBO();
}

//----------------------------------------------------
void World::BuildZone3D() 
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	//m_solidBlocks.clear();
	//m_temporaryCellularVector.clear();

	//m_solidBlocks.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);
	m_temporaryCellularVector.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) 
	{
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE_3D ) 
		{
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_worldBlockCells[index].m_isSolid = true;
			int randomNumber = (rand() % 100);

			if ( randomNumber > 98 ) 
			{
				m_worldBlockCells[index].m_cellType = 3;
			} 
			else if ( randomNumber > 65 )
			{
				m_worldBlockCells[index].m_cellType = 2;
			}
			else if ( randomNumber > 32 )
			{
				m_worldBlockCells[index].m_cellType = 1;
			}
			else
			{
				m_worldBlockCells[index].m_cellType = 0;
			}
			m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z), m_worldBlockCells[index].m_cellType);
		}
		else 
		{
			m_worldBlockCells[index].m_isSolid = false;
		}

		m_temporaryCellularVector.push_back((CubeCell(false)));
	}

	m_renderer.PushCubeVerticesToVBO();
}

//----------------------------------------------------
bool World::ProcessKeyDownEvent( unsigned char virtualKeyCode) 
{
	m_isKeyDown[ virtualKeyCode ] = true;
	return true;
}

//----------------------------------------------------
bool World::ProcessKeyUpEvent( unsigned char virtualKeyCode) 
{
	m_isKeyDown[ virtualKeyCode ] = false;
	return true;
}

//----------------------------------------------------
void World::UpdateCameraFromInput( float deltaSeconds ) 
{
	m_camera.m_cameraPitch -= deltaSeconds * m_mousePositionZDifference * ConstantParameters::MOUSE_SPEED;
	m_camera.m_cameraYaw += deltaSeconds * m_mousePositionXDifference * ConstantParameters::MOUSE_SPEED;
}

//----------------------------------------------------
void World::FindCameraFacingVector() 
{
	m_camera.m_cameraFacingVector.x = cos(m_camera.m_cameraYaw) * cos(m_camera.m_cameraPitch);
	m_camera.m_cameraFacingVector.y = sin(m_camera.m_cameraYaw) * cos(m_camera.m_cameraPitch);
	m_camera.m_cameraFacingVector.z = -sin(m_camera.m_cameraPitch);
}

//----------------------------------------------------
void World::UpdateFromMouseInput() 
{
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
void World::UpdatePlayerFromInput( float deltaSeconds ) 
{	
	if ( m_isKeyDown[ 'P' ] && !m_keyIsHeld ) 
	{
		m_keyIsHeld = true;
		m_isPaused = !m_isPaused;
	}
	
	if ( m_isKeyDown[ 'T' ] && !m_keyIsHeld ) 
	{
		m_keyIsHeld = true;
		m_renderer.DeleteBuffers();
		if (m_is2DWorld) 
		{
			GameOfLifeCellularAutomataPass2D();
		} 
		else 
		{
			GameOfLifeCellularAutomataPass3D();
		}
	}

	if ( m_isKeyDown[ 'R' ] && !m_keyIsHeld ) 
	{
		m_keyIsHeld = true;
		m_renderer.DeleteBuffers();
		BuildZone3D();
		m_is2DWorld = false;
	}

	if ( m_isKeyDown[ 'E' ] ) 
	{
		m_renderer.DeleteBuffers();
		BuildZone2D();
		m_is2DWorld = true;
	}
	
	if (m_isKeyDown[ VK_LEFT ]) 
	{
		m_camera.m_cameraYaw += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_RIGHT ]) 
	{
		m_camera.m_cameraYaw -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_UP ]) 
	{
		m_camera.m_cameraPitch -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_DOWN ]) 
	{
		m_camera.m_cameraPitch += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'W' ]) 
	{
		m_camera.m_cameraPosition.x += cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.y += sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'D' ]) 
	{
		m_camera.m_cameraPosition.y -= cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.x += sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'S' ]) 
	{
		m_camera.m_cameraPosition.x -= cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.y -= sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'A' ]) 
	{
		m_camera.m_cameraPosition.y += cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.x -= sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_SPACE ]) 
	{
		m_camera.m_cameraPosition.z += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[VK_CONTROL]) 
	{
		m_camera.m_cameraPosition.z -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if ( !( m_isKeyDown[ 'T' ] || m_isKeyDown[ 'R' ] || m_isKeyDown[ 'P' ]) ) 
	{
		m_keyIsHeld = false;
	}
}

//----------------------------------------------------
void World::CheckForGimbleLock() 
{
	if (m_camera.m_cameraPitch >= ConstantParameters::CAMERA_Z_MAX) 
	{
		m_camera.m_cameraPitch = ConstantParameters::CAMERA_Z_MAX - 0.005f;
	}

	if (m_camera.m_cameraPitch <= -ConstantParameters::CAMERA_Z_MAX) 
	{
		m_camera.m_cameraPitch = -ConstantParameters::CAMERA_Z_MAX + 0.005f;
	}
}

//----------------------------------------------------
void World::Update() 
{
	float currentTime = (float) Time::GetCurrentTimeSeconds();
	float deltaSeconds = ConstantParameters::DELTA_SECONDS; // Hack: assume 60 FPS

	UpdatePlayerFromInput( deltaSeconds );
	UpdateCameraFromInput( deltaSeconds );
	UpdateFromMouseInput();
	FindCameraFacingVector();
	CheckForGimbleLock();

	if (!m_isPaused) 
	{
		if ( ( currentTime - m_lastCellularAutomataTime ) > ConstantParameters::TIME_BETWEEN_CA_STEPS ) 
		{
			m_lastCellularAutomataTime = currentTime;
			if (m_is2DWorld) 
			{
				m_renderer.DeleteBuffers();
				GameOfLifeCellularAutomataPass2D();
			} 
			else 
			{
				m_renderer.DeleteBuffers();
				GameOfLifeCellularAutomataPass3D();
			}
		}
	}
}

//----------------------------------------------------
void World::Render() 
{	
	m_renderer.SetModelViewProjectionMatrix(m_camera);
	m_renderer.SendCubeVBO();
	m_renderer.PopMatrix();
}

//----------------------------------------------------
void World::GameOfLifeCellularAutomataPass2D() 
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int SOLID_COUNTER = 0;
	unsigned int BLACK_COUNTER = 0;
	unsigned int COLORED_COUNTER = 0;
	unsigned int WHITE_COUNTER = 0;

	unsigned int surroundingCellIndices[8];
	
	for ( unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++ ) 
	{
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( x > 0 ) 
		{
			surroundingCellIndices[NEIGHBOR_WEST] = index - 1; //left
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_WEST] = index + ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //left
		}

		if ( y > 0 ) 
		{
			surroundingCellIndices[NEIGHBOR_SOUTH] = index - ConstantParameters::BLOCKS_Y_AXIS; //bottom
			surroundingCellIndices[NEIGHBOR_SOUTHWEST] = surroundingCellIndices[NEIGHBOR_WEST] - ConstantParameters::BLOCKS_Y_AXIS; //bottom left
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_SOUTH] = index + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //bottom
			surroundingCellIndices[NEIGHBOR_SOUTHWEST] = surroundingCellIndices[NEIGHBOR_WEST] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //bottom left
		}

		if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) 
		{
			surroundingCellIndices[NEIGHBOR_NORTH] = index + ConstantParameters::BLOCKS_Y_AXIS; //top
			surroundingCellIndices[NEIGHBOR_NORTHWEST] = surroundingCellIndices[NEIGHBOR_WEST] + ConstantParameters::BLOCKS_Y_AXIS; //top left
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_NORTH] = index - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //top
			surroundingCellIndices[NEIGHBOR_NORTHWEST] = surroundingCellIndices[NEIGHBOR_WEST] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //top left
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) 
		{
			surroundingCellIndices[NEIGHBOR_EAST] = index + 1; //right
			surroundingCellIndices[NEIGHBOR_SOUTHEAST] = surroundingCellIndices[NEIGHBOR_SOUTH] + 1; //bottom right
			surroundingCellIndices[NEIGHBOR_NORTHEAST] = surroundingCellIndices[NEIGHBOR_NORTH] + 1; //top right
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_EAST] = index - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //right
			surroundingCellIndices[NEIGHBOR_SOUTHEAST] = surroundingCellIndices[NEIGHBOR_SOUTH] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //bottom right
			surroundingCellIndices[NEIGHBOR_NORTHEAST] = surroundingCellIndices[NEIGHBOR_NORTH] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //top right
		}

		
		for ( unsigned int surroundingIndex = 0; surroundingIndex < NUM_NEIGHBORS; surroundingIndex++ ) 
		{
			if ( m_worldBlockCells[ surroundingCellIndices[surroundingIndex] ].m_isSolid ) 
			{
				SOLID_COUNTER++;

				switch(m_worldBlockCells[ surroundingCellIndices[surroundingIndex] ].m_cellType)
				{
					case 0:
						COLORED_COUNTER++;
						break;
					case 1:
						BLACK_COUNTER++;
						break;
					case 2:
						WHITE_COUNTER++;
						break;
				}
			}
		}

		if ( SOLID_COUNTER == ConstantParameters::PRODUCE_LIFE_THRESHOLD_2D ) 
		{
			m_temporaryCellularVector[index].m_isSolid = true;

			if ( ( COLORED_COUNTER > 0 ) && ( WHITE_COUNTER > 0 ) && ( BLACK_COUNTER > 0 ) )
			{
				m_temporaryCellularVector[index].m_cellType = 3;
			}
			else if ( ( BLACK_COUNTER > COLORED_COUNTER ) && ( BLACK_COUNTER > WHITE_COUNTER ) )
			{
				m_temporaryCellularVector[index].m_cellType = 1;
			}
			else if ( ( WHITE_COUNTER > COLORED_COUNTER ) && ( WHITE_COUNTER > BLACK_COUNTER ) )
			{
				m_temporaryCellularVector[index].m_cellType = 2;
			}
			else
			{
				m_temporaryCellularVector[index].m_cellType = 0;
			}
		} 
		else if ( ( SOLID_COUNTER <= ConstantParameters::REMOVE_LIFE_THRESHOLD_LOWERBOUND_2D ) || ( SOLID_COUNTER >= ConstantParameters::REMOVE_LIFE_THRESHOLD_UPPERBOUND_2D ) ) 
		{
			m_temporaryCellularVector[index].m_isSolid = false;
		}
		else 
		{
			m_temporaryCellularVector[index].m_isSolid = m_worldBlockCells[index].m_isSolid;
			m_temporaryCellularVector[index].m_cellType = m_worldBlockCells[index].m_cellType;
		}

		SOLID_COUNTER = 0;
		BLACK_COUNTER = 0;
		COLORED_COUNTER = 0;
		WHITE_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::WIDTH_TIMES_HEIGHT; index++) 
	{
		m_worldBlockCells[index] = m_temporaryCellularVector[index];

		if ( m_worldBlockCells[index].m_isSolid ) 
		{
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;
			
			m_renderer.AddCubeToBuffer( Vector3((float) x, (float) y, (float) z), m_worldBlockCells[index].m_cellType );
		}
	}

	m_renderer.PushCubeVerticesToVBO();
}

//----------------------------------------------------
void World::GameOfLifeCellularAutomataPass3D() 
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int SOLID_COUNTER = 0;
	unsigned int BLACK_COUNTER = 0;
	unsigned int COLORED_COUNTER = 0;
	unsigned int WHITE_COUNTER = 0;

	unsigned int surroundingCellIndices[26];
	
	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) 
	{
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( z < ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ) 
		{
			surroundingCellIndices[NEIGHBOR_UP_3D] = index + ConstantParameters::WIDTH_TIMES_HEIGHT; //above
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_UP_3D] = index - ( ConstantParameters::WIDTH_TIMES_HEIGHT * ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ); //above
		}

		if ( z > 0 ) 
		{
			surroundingCellIndices[NEIGHBOR_DOWN_3D] = index - ConstantParameters::WIDTH_TIMES_HEIGHT; //below
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_DOWN_3D] = index + ( ConstantParameters::WIDTH_TIMES_HEIGHT * ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) ); //below
		}

		if ( x > 0 ) 
		{
			surroundingCellIndices[NEIGHBOR_WEST_3D] = index - 1; //left
			surroundingCellIndices[NEIGHBOR_UP_WEST_3D] = surroundingCellIndices[NEIGHBOR_UP_3D] - 1; //above left
			surroundingCellIndices[NEIGHBOR_DOWN_WEST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_3D] - 1; //below left
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_WEST_3D] = index + ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //left
			surroundingCellIndices[NEIGHBOR_UP_WEST_3D] = surroundingCellIndices[NEIGHBOR_UP_3D] + ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //above left
			surroundingCellIndices[NEIGHBOR_DOWN_WEST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_3D] + ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //below left
		}

		if ( y > 0 ) 
		{
			surroundingCellIndices[NEIGHBOR_SOUTH_3D] = index - ConstantParameters::BLOCKS_Y_AXIS; //bottom
			surroundingCellIndices[NEIGHBOR_UP_SOUTH_3D] = surroundingCellIndices[NEIGHBOR_UP_3D] - ConstantParameters::BLOCKS_Y_AXIS; //above bottom
			surroundingCellIndices[NEIGHBOR_DOWN_SOUTH_3D] = surroundingCellIndices[NEIGHBOR_DOWN_3D] - ConstantParameters::BLOCKS_Y_AXIS; //below bottom
			surroundingCellIndices[NEIGHBOR_SOUTHWEST_3D] = surroundingCellIndices[NEIGHBOR_WEST_3D] - ConstantParameters::BLOCKS_Y_AXIS; //bottom left
			surroundingCellIndices[NEIGHBOR_UP_SOUTHWEST_3D] = surroundingCellIndices[NEIGHBOR_UP_WEST_3D] - ConstantParameters::BLOCKS_Y_AXIS; //above bottom left
			surroundingCellIndices[NEIGHBOR_DOWN_SOUTHWEST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_WEST_3D] - ConstantParameters::BLOCKS_Y_AXIS; //below bottom left
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_SOUTH_3D] = index + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //bottom
			surroundingCellIndices[NEIGHBOR_UP_SOUTH_3D] = surroundingCellIndices[NEIGHBOR_UP_3D] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //above bottom
			surroundingCellIndices[NEIGHBOR_DOWN_SOUTH_3D] = surroundingCellIndices[NEIGHBOR_DOWN_3D] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //below bottom
			surroundingCellIndices[NEIGHBOR_SOUTHWEST_3D] = surroundingCellIndices[NEIGHBOR_WEST_3D] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //bottom left
			surroundingCellIndices[NEIGHBOR_UP_SOUTHWEST_3D] = surroundingCellIndices[NEIGHBOR_UP_WEST_3D] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //above bottom left
			surroundingCellIndices[NEIGHBOR_DOWN_SOUTHWEST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_WEST_3D] + ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //below bottom left
		}

		if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) ) 
		{
			surroundingCellIndices[NEIGHBOR_NORTH_3D] = index + ConstantParameters::BLOCKS_Y_AXIS; //top
			surroundingCellIndices[NEIGHBOR_UP_NORTH_3D] = surroundingCellIndices[NEIGHBOR_UP_3D] + ConstantParameters::BLOCKS_Y_AXIS; //above top
			surroundingCellIndices[NEIGHBOR_DOWN_NORTH_3D] = surroundingCellIndices[NEIGHBOR_DOWN_3D] + ConstantParameters::BLOCKS_Y_AXIS; //below top
			surroundingCellIndices[NEIGHBOR_NORTHWEST_3D] = surroundingCellIndices[NEIGHBOR_WEST_3D] + ConstantParameters::BLOCKS_Y_AXIS; //top left
			surroundingCellIndices[NEIGHBOR_UP_NORTHWEST_3D] = surroundingCellIndices[NEIGHBOR_UP_WEST_3D] + ConstantParameters::BLOCKS_Y_AXIS; //above top left
			surroundingCellIndices[NEIGHBOR_DOWN_NORTHWEST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_WEST_3D] + ConstantParameters::BLOCKS_Y_AXIS; //below top left
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_NORTH_3D] = index - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //top
			surroundingCellIndices[NEIGHBOR_UP_NORTH_3D] = surroundingCellIndices[NEIGHBOR_UP_3D] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //above top
			surroundingCellIndices[NEIGHBOR_DOWN_NORTH_3D] = surroundingCellIndices[NEIGHBOR_DOWN_3D] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //below top
			surroundingCellIndices[NEIGHBOR_NORTHWEST_3D] = surroundingCellIndices[NEIGHBOR_WEST_3D] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //top left
			surroundingCellIndices[NEIGHBOR_UP_NORTHWEST_3D] = surroundingCellIndices[NEIGHBOR_UP_WEST_3D] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //above top left
			surroundingCellIndices[NEIGHBOR_DOWN_NORTHWEST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_WEST_3D] - ( ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) * ConstantParameters::BLOCKS_X_AXIS ); //below top left
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) ) 
		{
			surroundingCellIndices[NEIGHBOR_EAST_3D] = index + 1; //right
			surroundingCellIndices[NEIGHBOR_UP_EAST_3D] = surroundingCellIndices[NEIGHBOR_UP_3D] + 1; //above right
			surroundingCellIndices[NEIGHBOR_DOWN_EAST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_3D] + 1; //below right
			surroundingCellIndices[NEIGHBOR_SOUTHEAST_3D] = surroundingCellIndices[NEIGHBOR_SOUTH_3D] + 1; //bottom right
			surroundingCellIndices[NEIGHBOR_UP_SOUTHEAST_3D] = surroundingCellIndices[NEIGHBOR_UP_SOUTH_3D] + 1; //above bottom right
			surroundingCellIndices[NEIGHBOR_DOWN_SOUTHEAST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_SOUTH_3D] + 1; //below bottom right
			surroundingCellIndices[NEIGHBOR_NORTHEAST_3D] = surroundingCellIndices[NEIGHBOR_NORTH_3D] + 1; //top right
			surroundingCellIndices[NEIGHBOR_UP_NORTHEAST_3D] = surroundingCellIndices[NEIGHBOR_UP_NORTH_3D] + 1; //above top right
			surroundingCellIndices[NEIGHBOR_DOWN_NORTHEAST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_NORTH_3D] + 1; //below top right
		} 
		else 
		{
			surroundingCellIndices[NEIGHBOR_EAST_3D] = index - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //right
			surroundingCellIndices[NEIGHBOR_UP_EAST_3D] = surroundingCellIndices[NEIGHBOR_UP_3D] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //above right
			surroundingCellIndices[NEIGHBOR_DOWN_EAST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_3D] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //below right
			surroundingCellIndices[NEIGHBOR_SOUTHEAST_3D] = surroundingCellIndices[NEIGHBOR_SOUTH_3D] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //bottom right
			surroundingCellIndices[NEIGHBOR_UP_SOUTHEAST_3D] = surroundingCellIndices[NEIGHBOR_UP_SOUTH_3D] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //above bottom right
			surroundingCellIndices[NEIGHBOR_DOWN_SOUTHEAST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_SOUTH_3D] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //below bottom right
			surroundingCellIndices[NEIGHBOR_NORTHEAST_3D] = surroundingCellIndices[NEIGHBOR_NORTH_3D] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //top right
			surroundingCellIndices[NEIGHBOR_UP_NORTHEAST_3D] = surroundingCellIndices[NEIGHBOR_UP_NORTH_3D] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //above top right
			surroundingCellIndices[NEIGHBOR_DOWN_NORTHEAST_3D] = surroundingCellIndices[NEIGHBOR_DOWN_NORTH_3D] - ( ConstantParameters::BLOCKS_X_AXIS - 1 ); //below top right
		}


		for ( unsigned int surroundingIndex = 0; surroundingIndex < NUM_NEIGHBORS_3D; surroundingIndex++ ) 
		{
			if ( m_worldBlockCells[ surroundingCellIndices[surroundingIndex] ].m_isSolid ) 
			{
				SOLID_COUNTER++;
				switch(m_worldBlockCells[ surroundingCellIndices[surroundingIndex] ].m_cellType)
				{
					case 0:
						COLORED_COUNTER++;
						break;
					case 1:
						BLACK_COUNTER++;
						break;
					case 2:
						WHITE_COUNTER++;
						break;
				}
			}
		}

		if ( ( SOLID_COUNTER > ( ConstantParameters::PRODUCE_LIFE_THRESHOLD_LOWERBOUND_3D - 1 ) ) && ( SOLID_COUNTER < ( ConstantParameters::PRODUCE_LIFE_THRESHOLD_UPPERBOUND_3D + 1 ) ) ) 
		{
			m_temporaryCellularVector[index].m_isSolid = true;

			if ( ( COLORED_COUNTER > 0 ) && ( WHITE_COUNTER > 0 ) && ( BLACK_COUNTER > 0 ) )
			{
				m_temporaryCellularVector[index].m_cellType = 3;
			}
			else if ( ( BLACK_COUNTER > COLORED_COUNTER ) && ( BLACK_COUNTER > WHITE_COUNTER ) )
			{
				m_temporaryCellularVector[index].m_cellType = 1;
			}
			else if ( ( WHITE_COUNTER > COLORED_COUNTER ) && ( WHITE_COUNTER > BLACK_COUNTER ) )
			{
				m_temporaryCellularVector[index].m_cellType = 2;
			}
			else
			{
				m_temporaryCellularVector[index].m_cellType = 0;
			}
		} 
		else if ( ( SOLID_COUNTER <= ConstantParameters::REMOVE_LIFE_THRESHOLD_LOWERBOUND_3D ) || ( SOLID_COUNTER >= ConstantParameters::REMOVE_LIFE_THRESHOLD_UPPERBOUND_3D ) ) 
		{
			m_temporaryCellularVector[index].m_isSolid = false;
		}
		else 
		{
			m_temporaryCellularVector[index].m_isSolid = m_worldBlockCells[index].m_isSolid;
			m_temporaryCellularVector[index].m_cellType = m_worldBlockCells[index].m_cellType;
		}

		SOLID_COUNTER = 0;
		BLACK_COUNTER = 0;
		COLORED_COUNTER = 0;
		WHITE_COUNTER = 0;
	}

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) 
	{
		m_worldBlockCells[index] = m_temporaryCellularVector[index];

		if ( m_worldBlockCells[index].m_isSolid ) 
		{
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_renderer.AddCubeToBuffer( Vector3((float) x, (float) y, (float) z), m_worldBlockCells[index].m_cellType );
		}
	}

	m_renderer.PushCubeVerticesToVBO();
}

//---------------------------------------------
void World::GetAllCellsInRayTrace() 
{
	m_raytraceCells.clear();

	bool foundBlock = false;

	float xPos = 0.f;
	float yPos = 0.f;
	float zPos = 0.f;
	float range = 0.f;
	for (float counter = 0.f; counter <= ConstantParameters::NUMBER_OF_STEPS_IN_STEP_AND_SAMPLE; counter += 1.f)
	{
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

			if ( !m_raytraceCells.empty() ) 
			{
				for (unsigned int counter = 0; counter < m_raytraceCells.size(); counter++) 
				{
					if ( m_raytraceCells[counter] == index ) {
						foundBlock = true;
						break;
					}
				}

				if (!foundBlock) 
				{
					m_raytraceCells.push_back( index );
				}
			} 
			else 
			{
				m_raytraceCells.push_back( index );
			}
		}
		foundBlock = false;
	}
}

//--------------------------------------------
void World::IncreaseFocus() 
{
	m_cellFocusRange++;
	if (m_cellFocusRange >= m_raytraceCells.size() ) 
	{
		m_cellFocusRange = m_raytraceCells.size() - 1;
	}
}

//--------------------------------------------
void World::ReduceFocus() 
{
	m_cellFocusRange--;
	if (m_cellFocusRange < 0) 
	{
		m_cellFocusRange = 0;
	}
}

//--------------------------------------------
void World::CapFocus() 
{
	if (m_cellFocusRange >= m_raytraceCells.size() ) 
	{
		m_cellFocusRange = m_raytraceCells.size() - 1;
	}

	if (m_cellFocusRange < 0) 
	{
		m_cellFocusRange = 0;
	}
}

//--------------------------------------------
void World::TurnTargetedCellToSolid() 
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;
	
	if ( !m_raytraceCells.empty() ) 
	{
		x = m_raytraceCells[m_cellFocusRange] & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( !m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_isSolid ) 
		{
			m_renderer.DeleteBuffers();
			m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_isSolid = true;
			m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_cellType = 0;

			for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) 
			{
				if ( m_worldBlockCells[index].m_isSolid ) 
				{
					x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
					y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
					z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
					m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z), 0);
				}
			}
			m_renderer.PushCubeVerticesToVBO();
		}
	}
}

//--------------------------------------------
void World::TurnTargetedCellToNonSolid() 
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;
	
	if ( !m_raytraceCells.empty() ) 
	{
		x = m_raytraceCells[m_cellFocusRange] & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (m_raytraceCells[m_cellFocusRange] >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;

		if ( m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_isSolid ) 
		{
			m_renderer.DeleteBuffers();
			m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_isSolid = false;
			m_worldBlockCells[m_raytraceCells[m_cellFocusRange]].m_cellType = 0;

			for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++) 
			{
				if ( m_worldBlockCells[index].m_isSolid ) 
				{
					x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
					y = (index >> ConstantParameters::BLOCKS_Y_POWER) & ConstantParameters::BLOCKS_Y_AXIS - 1;
					z = (index >> ConstantParameters::BLOCKS_XY_POWER) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
					m_renderer.AddCubeToBuffer(Vector3((float) x, (float) y, (float) z), 0);
				}
			}
			m_renderer.PushCubeVerticesToVBO();
		}
	}
}
