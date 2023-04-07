// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <string>

// OpenGL Library Includes //

// Engine Includes //
#include "UIElement.h"
#include "UIImage.h"
#include "UIText.h"

//class FDelegateWrapper
//{
//private:
//	class ParameterBase {
//	public:
//		virtual ~ParameterBase() {}
//		virtual ParameterBase* copy() = 0;
//		virtual void foo() = 0;
//	};
//
//	template <typename T>
//	class ParameterModel : public ParameterBase {
//	public:
//		// take by value so we simply move twice, if movable
//		ParameterModel(const T& t) : t(t) {}
//		ParameterModel(T&& t) : t(t) {}
//		void foo() { t.foo(); }
//		ParameterModel* copy() { return new ParameterModel(*this); }
//	private:
//		T t;
//	};
//
//public:
//	template <typename T>
//	FDelegateWrapper(T&& t)
//		: pp(new ParameterModel< typename std::remove_reference<T>::type >(std::forward<T>(t))) {}
//
//	// Movable and Copyable only
//	FDelegateWrapper(FDelegateWrapper&&) = default;
//	FDelegateWrapper& operator=(FDelegateWrapper&&) = default;
//
//	FDelegateWrapper(const FDelegateWrapper& other) : pp(other.pp->copy()) {};
//	FDelegateWrapper operator=(const FDelegateWrapper& other) {
//		pp.reset(other.pp->copy());
//		return *this;
//	};
//
//	// members
//
//	void foo() { pp->foo(); }
//private:
//	std::unique_ptr<ParameterBase> pp;
//};

class DelegateBase
{
public:
	DelegateBase()
	{
	};

	virtual ~DelegateBase()
	{
	};
	//virtual DelegateBase* copy() {};
	virtual void operator()()
	{
	}

	DelegateBase(const DelegateBase& other)
	{
	};
};

// TODO: Move out of UI
template <class T>
class FDelegate : public DelegateBase
{
public:
	using fn = void(T::*)();

	FDelegate(T* trg, fn op) : m_rTarget(trg)
	                           , m_Operation(op)
	{
	}

	void operator()() override
	{
		(m_rTarget->*m_Operation)();
	}

	//private:
	FDelegate(const FDelegate<T>& other)
	{
	};

	T* m_rTarget;
	fn m_Operation;
};

class UIButton : public UIElement
{
public:
	UIButton(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, glm::vec4 _Colour, glm::vec4 _HightlightColour, int iWidth, int iHeight, void (*func)() = nullptr);

	UIButton(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, const char* _ImagePath, glm::vec4 _HightlightColour, int iWidth, int iHeight, void (*func)() = nullptr);

	~UIButton();

	void AddText(std::string sText, std::string sFont, int iPSize, glm::vec4 TextColour, Utils::EANCHOR _Anchor);

	void AddText(std::string sText, std::string sFont, int iPSize, glm::vec4 TextColour, Utils::EANCHOR _Anchor, glm::vec2 _v2Offset);

	template <class T>
	void BindPress(FDelegate<T>* callback)
	{
		PressFunc = callback;
	}

	void BindHold(void (*func)());

	template <class T>
	void BindRelease(FDelegate<T>* callback)
	{
		ReleaseFunc = callback;
	}

	void SetActive(bool _bIsActive) override;

	void SetPressSound(const char* _SoundPath);

	UIText TextComponent;
	UIImage ImageComponent;

	void DrawUIElement() override;

	void Update() override;

	void SetPosition(glm::vec2 _NewPosition) override;

	void Pressed();

	void Hovered();

	bool HoverOverride = false;

	bool bPressed = false;

	static bool bButtonPressedThisFrame;

private:
	using GMVoidFunc = void(*)();

	DelegateBase* PressFunc;
	DelegateBase* ReleaseFunc;
	GMVoidFunc PressFuncCall = nullptr;
	GMVoidFunc HoldFuncCall = nullptr;
	GMVoidFunc ReleaseFuncCall = nullptr;

	bool bHasText = false;

	glm::vec4 btnColour;
	glm::vec4 btnHighlightColour;

	const char* m_PressSoundPath = "";
};
