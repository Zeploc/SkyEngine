// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <list>
#include <functional>

#include "Core.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class CDelegateBase
{
public:
	CDelegateBase()
	{
		
	}	

	virtual ~CDelegateBase()
	{
	}
	//virtual CDelegateBase* copy() {};
	virtual void operator()() = 0;

	// virtual bool EqualTo(void* Object, void(*Function)())
	// {
	// 	return false;
	// }

	CDelegateBase(const CDelegateBase& other)
	{
	}

};

// using VoidFunc = std::function<void()>;

template <class T>
class TVoidDelegate : public CDelegateBase
{
public:
	using VoidFunc = void(T::*)();//std::function<void(T*)>;
	
	TVoidDelegate()
	{
		
	}

	TVoidDelegate(const TVoidDelegate& other)
	{
		// TODO:
	}
	
	TVoidDelegate(T* trg, VoidFunc op) : m_rTarget(trg)
							   , m_Operation(op)
	{
	}

	virtual ~TVoidDelegate()
	{
	}
	//virtual CDelegateBase* copy() {};
	void operator()()
	{
		(m_rTarget->*m_Operation)();
	}
	
	bool EqualTo(T* Object, void(T::*Function)())
	{
		return m_rTarget == Object && m_Operation == Function;
	}

	//private:

	T* m_rTarget;
	VoidFunc m_Operation;
};

class FDelegate
{
public:
	
	FDelegate()
	{
		
	}

	FDelegate(const FDelegate& other)
	{
		// TODO:
	}
	
	template <class T>
	void Bind(T* Object, void(T::*Function)())
	{
		Clear();
		Delegate = new TVoidDelegate<T>(Object, Function);
	}

	void Clear()
	{
		delete Delegate;
		Delegate = nullptr;
	}

	void Broadcast() const
	{
		if (Delegate)
		{
			(*Delegate)();
		}
	}
	
	virtual ~FDelegate()
	{
		Clear();
	}

private:
	CDelegateBase* Delegate = nullptr;

};


class ENGINE_API FMulticastDelegate
{
public:	
	FMulticastDelegate()
	{
		
	}
	
	FMulticastDelegate(const FMulticastDelegate& other)
	{
		// TODO:
	}
	
	template <class T>
	// void Bind(std::function<void(T*)> Function)
	void Bind(T* Object, void(T::*Function)())
	{
		// using VoidFunc = std::function<void(T*)>;
		// Delegates.push_back(Function);
		Delegates.push_back(new TVoidDelegate<T>(Object, Function));
	}
	
	template <class T>
	bool Unbind(T* Object, void(T::*Function)())
	{
		using void_delegate = TVoidDelegate<T>;
		for (CDelegateBase* Delegate : Delegates)
		{
			// TODO: Improve from static cast
			void_delegate* VoidDelegate = static_cast<void_delegate*>(Delegate);
			if (Delegate && VoidDelegate->EqualTo(Object, Function))
			{
				Delegates.remove(Delegate);
				delete Delegate;
				return true;
			}
		}
		return false;
	}

	void Clear()
	{
		for (const CDelegateBase* Delegate : Delegates)
		{
			delete Delegate;
		}
		Delegates.clear();
	}

	void Broadcast() const
	{
		const std::list<CDelegateBase*> DelegatesCopy = Delegates;
		for (CDelegateBase* Delegate : DelegatesCopy)
		{
			// TODO: Need to account for being unbound from another delegate
			if (Delegate)
			{
				(*Delegate)();
			}
		}
	}

	FMulticastDelegate(const FDelegate& other)
	{
	}
	
	virtual ~FMulticastDelegate()
	{
		Clear();
	}

private:
	std::list<CDelegateBase*> Delegates;
};


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