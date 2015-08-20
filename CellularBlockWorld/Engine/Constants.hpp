#ifndef included_Constants
#define included_Constants
#pragma once

//-------------------------------------------------------------


//-------------------------------------------------------------
namespace ConstantParameters
{
	const float PI = 3.1415926535897932384626433832795f;
	const float CONVERTING_TO_RADIANS = PI/180.f;
	const float CONVERTING_TO_DEGREES = 180.f/PI;
	const float DELTA_SECONDS = 1/60.f;
	const float CAMERA_SPEED = 5.f;
	const float MOUSE_SPEED = 0.2f;
	const float CAMERA_Z_MAX = PI/2;
	const float TEXTURE_MAX = 0.03125f;
// 	const unsigned int BLOCKS_X_AXIS = 64;		//256
// 	const unsigned int BLOCKS_Y_AXIS = 64;		//256
// 	const unsigned int BLOCKS_Z_AXIS = 16;		//64
	const unsigned int BLOCKS_X_AXIS = 128;		//256
	const unsigned int BLOCKS_Y_AXIS = 128;		//256
	const unsigned int BLOCKS_Z_AXIS = 64;		//64
	const unsigned int WIDTH_TIMES_HEIGHT = BLOCKS_X_AXIS * BLOCKS_Y_AXIS;
	const unsigned int TOTAL_BLOCKS_IN_ZONE = BLOCKS_X_AXIS * BLOCKS_Y_AXIS * BLOCKS_Z_AXIS;
	const unsigned int SOLID_BLOCK_PERCENTAGE = 10;
}

#endif //included_Constants