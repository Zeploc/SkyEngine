// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Asset/AssetInterface.h"
#include "Core/Core.h"

class ENGINE_API CObject : public std::enable_shared_from_this<CObject>
{
public:
};

class ENGINE_API CAssetObject : public CObject, public IAssetObjectInterface
{
public:
	
	std::shared_ptr<CAssetObject> shared_from_this();
	// TWeakPointer<CAssetObject> weak_from_this();
};


struct SObjectPointerBase
{
public:
	template<class T>
	bool Equals(const TWeakPointer<T>& lhs, const TWeakPointer<T>& rhs);
}

template<class T>
struct ENGINE_API TObjectPointer
{
public:
	// Empty
	TObjectPointer()
	{
	}
	
	// Handles derived type weak pointer initialisation
	template<class U, std::enable_if_t<std::is_base_of_v<T, U>, bool> = true>
	TObjectPointer(const TWeakPointer<U>& InObjectPointer)
		: ObjectPointer(InObjectPointer)
	{
	}
	
	// Handles direct same type weak pointer initialization
	TObjectPointer(const TWeakPointer<T>& InObjectPointer)
		: ObjectPointer(InObjectPointer)
	{
		
	}
	
	// Handles same object pointer with derived class type T being upcast
	template<class U, std::enable_if_t<std::is_base_of_v<T, U>, bool> = true>
	TObjectPointer(const TObjectPointer<U>& InObjectPointer)
	: ObjectPointer(InObjectPointer.GetWeak())
	{
		
	}
	
	// Handles raw pointer being initialized as a weak ptr, has risk if base pointer is not managed by a smart pointer
	template<class U, std::enable_if_t<std::is_base_of_v<T, U>, bool> = true>
	TObjectPointer(THardPointer<U> InObjectPointer)
	: ObjectPointer(TWeakPointer<T>(InObjectPointer))
	{
		
	}
	
	// Handles raw pointer being initialized as a weak ptr, has risk if base pointer is not managed by a smart pointer
	TObjectPointer(T* InPointer)
	: ObjectPointer(TWeakPointer<T>(THardPointer<T>(InPointer)))
	{
		
	}

	// TODO: Could use these to handle copy constructor using weak pointer instead of default behaviour
	// TObjectPointer(const TObjectPointer<T>& UpCastObjectPointer)
	// 	: TObjectPointer(UpCastObjectPointer.ObjectPointer)
	// {
	// 	
	// }
	// TObjectPointer(TObjectPointer<T> UpCastObjectPointer)
	// 	: TObjectPointer(UpCastObjectPointer.ObjectPointer)
	// {
	// 	
	// }
	
	T* operator->()
	{
		if (!IsValid())
		{
			return nullptr;
		}
		return ObjectPointer.lock().get();
	}
	// TODO: Should this return a const ptr?
	T* operator->() const
	{
		if (!IsValid())
		{
			return nullptr;
		}
		return ObjectPointer.lock().get();
	}
	T& operator*() const noexcept
	{
		return *ObjectPointer.lock().get();
	}

	bool operator==(const TObjectPointer& other) const
	{
		/* Based on: https://stackoverflow.com/questions/12301916/how-can-you-efficiently-check-whether-two-stdweak-ptr-pointers-are-pointing-to*/
		return !ObjectPointer.owner_before(other.ObjectPointer) && !other.ObjectPointer.owner_before(ObjectPointer);
		return ObjectPointer.lock() == other.ObjectPointer.lock();
	}
	// bool operator==(TObjectPointer& other) const
	// {
	// 	return ObjectPointer == other.ObjectPointer;
	// }

	// Compare for maps
	bool operator<(const TObjectPointer& other) const
	{
		return std::owner_less<TWeakPointer<T>>()(ObjectPointer, other.ObjectPointer);
	}
	
	// Compare for maps
	// auto operator<=>(const TObjectPointer&) const = default;
	
	// Compare for maps
	// bool operator() (const TObjectPointer& lhs, const TObjectPointer& rhs) const
	// {
	// 	return lhs.ObjectPointer < rhs.ObjectPointer;
	// }
	// bool operator() (const TObjectPointer lhs, const TObjectPointer rhs) const
	// {
	// 	return lhs.ObjectPointer < rhs.ObjectPointer;
	// }

	TWeakPointer<T> GetWeak() const { return ObjectPointer; }

	explicit operator bool() const noexcept {
		return IsValid();
	}

	// TODO: Override equal operator

	bool IsValid() const
	{
		return !ObjectPointer.expired();
	}

protected:	
	TWeakPointer<T> ObjectPointer;
};

class CAssetObject;
template <typename T>
concept ConvertableToAssetObject = std::constructible_from<CAssetObject, T>;

// TODO: Limit template type to CAssetObject, And not allow type asset
template<class T = CAssetObject>//, std::enable_if_t<std::is_base_of_v<CAssetObject, T>, bool> = true>
struct ENGINE_API TAssetObjectPointer : public TObjectPointer<T>
{
public:
	using TObjectPointer<T>::TObjectPointer;
		
	// template<ConvertableToAssetObject U>
	// TAssetObjectPointer(U InObjectPointer)
	// 	: TObjectPointer(InObjectPointer)
	// {
	// 	
	// }
	// TAssetObjectPointer(TAssetObjectPointer& UpCastObjectPointer)
	// 	: TObjectPointer(std::move(UpCastObjectPointer.ObjectPointer))
	// {
	// 	
	// }
	// TAssetObjectPointer(const TAssetObjectPointer& UpCastObjectPointer)
	// 	: TObjectPointer(std::move(UpCastObjectPointer.ObjectPointer))
	// {
	// 	
	// }

	// TODO: Asset reference to check loaded state and link to load/unload
	// TODO: Compare if asset matches instead? not nessecary and weak ptr comparison works
	// bool operator==(const TObjectPointer& other) const
	// {
	// 	return ObjectPointer == other.GetWeak();
	// }
	
};

template<typename T, typename U>
TAssetObjectPointer<T> Cast(TAssetObjectPointer<U> Base)
{
	return TAssetObjectPointer<T>(std::dynamic_pointer_cast<T>(Base.GetWeak().lock()));
}