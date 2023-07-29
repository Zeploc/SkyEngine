// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Cursor.h"

// Library Includes //

// Engine Includes //
#include "Input/Input.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Cursor::Cursor(const char* _CursorImage) : UIImage({0, 0}, EANCHOR::TOP_LEFT, 0, {1.0f, 1.0f, 1.0f, 1.0f}, 25, 35, _CursorImage, 1)
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Cursor::~Cursor()
{
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Cursor::Update()
{
	position = glm::vec3(Input::GetInstance()->MousePos, 0);
	if ((position.x < m_v2InvisibleRange.x && position.y < m_v2InvisibleRange.y) ||
		(position.x > m_v2VisibleRange.x || position.y > m_v2VisibleRange.y) && m_v2VisibleRange.x != 0 && m_v2VisibleRange.y != 0)
	{
		SetActive(false);
	}
	else
	{
		SetActive(true);
	}
}

/************************************************************
#--Description--#: 	Sets an invisible boundry where the mouse will be hidden
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the position of the range corner
#--Return--#: 		NA
************************************************************/
void Cursor::SetInvisibleRange(glm::vec2 _v2NewRange)
{
	m_v2InvisibleRange = _v2NewRange;
}

/************************************************************
#--Description--#: 	Sets an visible boundry where the mouse will only be visible in that boundry
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the position of the range corner
#--Return--#: 		NA
************************************************************/
void Cursor::SetVisibleRange(glm::vec2 _v2NewRange)
{
	m_v2VisibleRange = _v2NewRange;
}
