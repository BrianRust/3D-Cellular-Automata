
#include "World.hpp"

//----------------------------------------------------
World::World(  ) 
	: m_mousePositionXDifference(0.f)
	, m_mousePositionZDifference(0.f)
{
	srand ((unsigned int)(time(NULL)));

	m_camera = Camera();
	m_renderer = OpenGLRenderer();
	m_keyIsHeld = false;
}

void World::Initialize()
{
	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++)
	{
		m_solidBlocks.push_back(CubeCell(false));
	}

	BuildZone();
}

void World::BuildZone()
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	//m_solidBlocks.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);
	//m_temporaryCellularVector.reserve(ConstantParameters::TOTAL_BLOCKS_IN_ZONE);

	int counter = 0;

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++)
	{
		if ( (rand() % 100) < ConstantParameters::SOLID_BLOCK_PERCENTAGE )
		{
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;

			m_solidBlocks[index] = true;
			//m_solidBlocks.push_back(CubeCell(true));
			m_renderer.AddCubeToBuffer(Vector3(x, y, z), RGBA(0.8f, 0.f, 0.f, 1.f));

			counter++;
		}
		else
		{
			m_solidBlocks[index] = false;
			//m_solidBlocks.push_back(CubeCell(false));
		}

		m_temporaryCellularVector.push_back((CubeCell(false)));
	}

	m_renderer.PushCubeVerticesToVBO();
	//m_renderer.PushGridOutlineVerticesToVBO();
}

void World::TestSolidCellularAutomataPass()
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;

	unsigned int THRESHOLD = 5;
	unsigned int SOLID_COUNTER = 0;

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++)
	{
		x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
		y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
		z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;
		
		//ABOVE
		if ( z < ( ConstantParameters::BLOCKS_Z_AXIS - 1 ) )
		{
			if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid )	//above
				SOLID_COUNTER++;

			if ( x > 0 )	//above left
			{
				if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid )	//above left
					SOLID_COUNTER++;

				if ( y > 0 )	//above left bottom
				{
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
						SOLID_COUNTER++;
				}
				
				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) )	//above left top
				{
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
						SOLID_COUNTER++;
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) )	//above right
			{
				if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid )	//above right
					SOLID_COUNTER++;

				if ( y > 0 )	//above right bottom
				{
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
						SOLID_COUNTER++;
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) )	//above right top
				{
					if ( m_solidBlocks[ index + ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
						SOLID_COUNTER++;
				}
			}
		}

		//BOTTOM
		if ( z > 0 )
		{
			if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT ].m_isSolid )	//above
				SOLID_COUNTER++;

			if ( x > 0 )	//above left
			{
				if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 ].m_isSolid )	//above left
					SOLID_COUNTER++;

				if ( y > 0 )	//above left bottom
				{
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
						SOLID_COUNTER++;
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) )	//above left top
				{
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
						SOLID_COUNTER++;
				}
			}

			if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) )	//above right
			{
				if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 ].m_isSolid )	//above right
					SOLID_COUNTER++;

				if ( y > 0 )	//above left bottom
				{
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
						SOLID_COUNTER++;
				}

				if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) )	//above left top
				{
					if ( m_solidBlocks[ index - ConstantParameters::WIDTH_TIMES_HEIGHT + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
						SOLID_COUNTER++;
				}
			}
		}

		//MIDDLE
		if ( x > 0 )	//above left
		{
			if ( m_solidBlocks[ index - 1 ].m_isSolid )	//above left
				SOLID_COUNTER++;

			if ( y > 0 )	//above left bottom
			{
				if ( m_solidBlocks[ index - 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
					SOLID_COUNTER++;
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) )	//above left top
			{
				if ( m_solidBlocks[ index - 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
					SOLID_COUNTER++;
			}
		}

		if ( x < ( ConstantParameters::BLOCKS_X_AXIS - 1 ) )	//above right
		{
			if ( m_solidBlocks[ index + 1 ].m_isSolid )	//above right
				SOLID_COUNTER++;

			if ( y > 0 )	//above left bottom
			{
				if ( m_solidBlocks[ index + 1 - ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
					SOLID_COUNTER++;
			}

			if ( y < ( ConstantParameters::BLOCKS_Y_AXIS - 1 ) )	//above left top
			{
				if ( m_solidBlocks[ index + 1 + ConstantParameters::BLOCKS_Y_AXIS ].m_isSolid )
					SOLID_COUNTER++;
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

	//m_renderer.DeleteBuffers();

	for (unsigned int index = 0; index < ConstantParameters::TOTAL_BLOCKS_IN_ZONE; index++)
	{
		m_solidBlocks[index] = m_temporaryCellularVector[index];

		if ( m_solidBlocks[index].m_isSolid )
		{
			x = index & ConstantParameters::BLOCKS_X_AXIS - 1;
			y = (index >> 6) & ConstantParameters::BLOCKS_Y_AXIS - 1;
			z = (index >> 12) & ConstantParameters::BLOCKS_Z_AXIS - 1;	
			m_renderer.AddCubeToBuffer(Vector3(x, y, z), RGBA(0.8f, 0.f, 0.f, 1.f));
		}
	}

	m_renderer.PushCubeVerticesToVBO();
//	m_renderer.PushGridOutlineVerticesToVBO();

	//m_temporaryCellularVector.clear();
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
void World::UpdatePlayerFromInput( float deltaSeconds )
{	
	if ( m_isKeyDown[ 'T' ] && !m_keyIsHeld )
	{
		m_keyIsHeld = true;
		m_renderer.DeleteBuffers();
		TestSolidCellularAutomataPass();
	}

	if ( m_isKeyDown[ 'R' ] && !m_keyIsHeld )
	{
		m_keyIsHeld = true;
		m_renderer.DeleteBuffers();
		BuildZone();
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

	if ( !( m_isKeyDown[ 'T' ] || m_isKeyDown[ 'R' ] ) )
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
	float deltaSeconds = ConstantParameters::DELTA_SECONDS; // Hack: assume 60 FPS
	//	GetCursorPos()
	UpdatePlayerFromInput( deltaSeconds );
	UpdateCameraFromInput( deltaSeconds );
	CheckForGimbleLock();
}

//----------------------------------------------------
void World::Render()
{	
	m_renderer.SendViewMatrix(m_camera);
	m_renderer.SendCubeVBO();
	m_renderer.PopMatrix();
}