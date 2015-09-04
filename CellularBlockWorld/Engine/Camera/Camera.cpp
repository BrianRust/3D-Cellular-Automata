#include "Camera.hpp"

//----------------------------------------------------
Camera::Camera() 
	: m_cameraPosition( -5.f, 0.f, 18.f )
	, m_cameraFacingVector( 0.f, 0.f, 0.f )
	, m_cameraPitch(0.f)
	, m_cameraRoll(0.f)
	, m_cameraYaw(0.f)
{
	
}
