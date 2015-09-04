#ifndef included_Camera
#define included_Camera
#pragma once

//-------------------------------------------------------------
#include "../Math/Vector3.hpp"

//-------------------------------------------------------------
class Camera
{
public:
	Camera();

	Vector3 m_cameraPosition;
	Vector3 m_cameraFacingVector;
	//Vector3 m_cameraOrientation; //Roll, Pitch, Yaw

	float m_cameraRoll;
	float m_cameraPitch;
	float m_cameraYaw;
};

#endif //included_Camera