//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Time.cpp
// Description    	:    main implementation for Time
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //

// OpenGL Library Includes //
#include <glew.h>
//#include <freeglut.h>
#include <glfw3.h>

// Engine Includes //

// Local Includes //

// This Includes //
#include "Time.h"

// Static Variables //
double Time::dPrevTime = 0.0;
double Time::dTimeDelta = 0.0;
double Time::dCurrentTime = 0.0;
double Time::dFPS = 0.0;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Time::Time()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Time::~Time()
{
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Time::Update()
{
	dCurrentTime = glfwGetTime();// glutGet(GLUT_ELAPSED_TIME);
	dTimeDelta = (dCurrentTime - dPrevTime);// / 1000;
	dPrevTime = glfwGetTime();// glutGet(GLUT_ELAPSED_TIME);
	dFPS = 1.0 / dTimeDelta;
}
