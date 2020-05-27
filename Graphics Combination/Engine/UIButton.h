//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    UIButton.h
// Description		:    Header file outlining the Class
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

#pragma once

// Library Includes //
#include <string>
//#include <boost/function.hpp>

// OpenGL Library Includes //
#include <glm\common.hpp>

// Engine Includes //
#include "UIElement.h"
#include "UIText.h"
#include "UIImage.h"

//class FDelegateWrapper
//{
	/*class DelegateBase
	{
	public:
		virtual ~DelegateBase() {}
		virtual DelegateBase* copy() = 0;
	}*/

	template <class T>
	class FDelegate// : public DelegateBase
	{
	public:
		typedef void (T::*fn)();

		FDelegate(T* trg, fn op)
			: m_rTarget(trg)
			, m_Operation(op)
		{
		}

		void operator()()
		{
			(m_rTarget->*m_Operation)();
		}
		/*void Execute()
		{
			(m_rTarget.*m_Operation)();
		}*/

	//private:
		FDelegate(const FDelegate<T>& other);
		/*{
			m_rTarget = other.m_rTarget;
			m_Operation = other.m_Operation;
		}*/

		T* m_rTarget;
		fn m_Operation;

	};

	/*template <class T>
	FDelegate<T> Bind(FDelegate<T>)
	{

	}*/
//};


class UIButton : public UIElement
{
public:
	UIButton(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, glm::vec4 _Colour, glm::vec4 _HightlightColour, int iWidth, int iHeight, void(*func)());
	UIButton(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, const char* _ImagePath, glm::vec4 _HightlightColour, int iWidth, int iHeight, void(*func)());
	~UIButton();

	void AddText(std::string sText, std::string sFont, int iPSize, glm::vec4 TextColour, Utils::EANCHOR _Anchor);
	void AddText(std::string sText, std::string sFont, int iPSize, glm::vec4 TextColour, Utils::EANCHOR _Anchor, glm::vec2 _v2Offset);
	template <class T>
	void BindPress(FDelegate<T> callback)//FDelegate<T>* callback);//  std::function<void()> f);
	{
		//callback();
	}
	void BindHold(void(*func)());
	void BindRelease(void(*func)());
	void SetActive(bool _bIsActive);
	void SetPressSound(const char* _SoundPath);

	UIText TextComponent;
	UIImage ImageComponent;

	void DrawUIElement();
	void Update();
	void SetPosition(glm::vec2 _NewPosition);

	void Pressed();
	void Hovered();

	bool HoverOverride = false;

	bool bPressed = false;

	static bool bButtonPressedThisFrame;

private:
	using GMVoidFunc = void(*)();

	//FDelegate<T>* PressFunc;
	//std::function<void()> PressFunc;
	GMVoidFunc PressFuncCall = nullptr;
	GMVoidFunc HoldFuncCall = nullptr;
	GMVoidFunc ReleaseFuncCall = nullptr;

	bool bHasText = false;


	glm::vec4 btnColour;
	glm::vec4 btnHighlightColour;

	const char* m_PressSoundPath = "";

};
