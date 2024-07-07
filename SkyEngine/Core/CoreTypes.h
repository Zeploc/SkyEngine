// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

template<typename T>
using THardPointer = std::shared_ptr<T>;
template<typename T>
using TWeakPointer = std::weak_ptr<T>;


// TODO: Fix errors not showing until compile when invalid args are given
template<typename T, typename ... Args>
constexpr THardPointer<T> CreatePointer(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
THardPointer<T> Cast(THardPointer<U> Base)
{
	return std::dynamic_pointer_cast<T>(Base);
}

template<typename T, typename U>
T* GetInterface(THardPointer<U> Base)
{
	// TODO: Is dynamic correct?
	return dynamic_cast<T*>(Base.get());
}

template<typename T>
using TArray = std::vector<T>;

template<typename T,typename U>
class TMap : public std::map<T, U>
{
public:
	// void Add(T Key, U Value)
	// {
	// 	insert(std::pair<T, U>(Key, Value));
	// }
	// using Add insert
	// inline std::pair<std::iterator, bool> Add(U& Val) { return insert(Val); }
};

template<typename T>
using TScope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr TScope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}