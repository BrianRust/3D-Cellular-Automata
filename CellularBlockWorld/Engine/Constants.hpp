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
	const float NINETY_DEGREES_AS_RADIANS = 90.f * CONVERTING_TO_RADIANS;
	const float DELTA_SECONDS = 1/60.f;
	const float CAMERA_SPEED = 6.f;
	const float MOUSE_SPEED = 0.2f;
	const float CAMERA_Z_MAX = PI/2;

	const float BLOCK_HIGHLIGHT_RANGE = 91.f;
	const float NUMBER_OF_STEPS_IN_STEP_AND_SAMPLE = 1000.f;
	const float NUMBER_OF_STEPS_IN_STEP_AND_SAMPLE_INVERSE = 1.f/NUMBER_OF_STEPS_IN_STEP_AND_SAMPLE;

	const float ASPECT_RATIO = 16.f/9.f;
	const float INVERSE_ASPECT_RATIO = 9.f/16.f;

	const unsigned int BLOCKS_X_POWER = 6;
	const unsigned int BLOCKS_Y_POWER = 6;
	const unsigned int BLOCKS_Z_POWER = 6;
	const unsigned int BLOCKS_XY_POWER = BLOCKS_X_POWER + BLOCKS_Y_POWER;
	

	const unsigned int BLOCKS_X_AXIS = ( 1 << BLOCKS_X_POWER );
	const unsigned int BLOCKS_Y_AXIS = ( 1 << BLOCKS_Y_POWER );
	const unsigned int BLOCKS_Z_AXIS = ( 1 << BLOCKS_Z_POWER );

	const unsigned int WIDTH_TIMES_HEIGHT = BLOCKS_X_AXIS * BLOCKS_Y_AXIS;
	const unsigned int WIDTH_TIMES_DEPTH = BLOCKS_X_AXIS * BLOCKS_Z_AXIS;
	const unsigned int TOTAL_BLOCKS_IN_ZONE = BLOCKS_X_AXIS * BLOCKS_Y_AXIS * BLOCKS_Z_AXIS;
	const unsigned int SOLID_BLOCK_PERCENTAGE_2D = 50;
	const unsigned int SOLID_BLOCK_PERCENTAGE_3D = 15;

//2D checks 8 Cubes
	const unsigned int PRODUCE_LIFE_THRESHOLD_2D = 3;
	const unsigned int REMOVE_LIFE_THRESHOLD_UPPERBOUND_2D = 4;
	const unsigned int REMOVE_LIFE_THRESHOLD_LOWERBOUND_2D = 1;

//3D checks 26 Cubes
	/*
	Life Lower - 9
	Life Upper - 10
	Death Lower - 3
	Death Upper - 12
	No Change - 4-8, 11
	*/
	const unsigned int PRODUCE_LIFE_THRESHOLD_LOWERBOUND_3D = 9;
	const unsigned int PRODUCE_LIFE_THRESHOLD_UPPERBOUND_3D = 10;
	const unsigned int REMOVE_LIFE_THRESHOLD_UPPERBOUND_3D = 12;
	const unsigned int REMOVE_LIFE_THRESHOLD_LOWERBOUND_3D = 3;

	const double TIME_BETWEEN_CA_STEPS = 0.25;

	
// 	const unsigned int PRODUCE_LIFE_THRESHOLD_LOWERBOUND_3D = 5;
// 	const unsigned int PRODUCE_LIFE_THRESHOLD_UPPERBOUND_3D = 5;
// 	const unsigned int REMOVE_LIFE_THRESHOLD_UPPERBOUND_3D = 8;
// 	const unsigned int REMOVE_LIFE_THRESHOLD_LOWERBOUND_3D = 1;

/*
---Immigration Variant---
Same as original but with two Living states. 
When a new cell is born, it takes on the ON state that is the majority in the three cells that gave it birth.
*/
	
}

#endif //included_Constants