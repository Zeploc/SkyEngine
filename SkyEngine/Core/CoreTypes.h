// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

template<typename T>
using TPointer = std::shared_ptr<T>;
template<typename T>
using TWeakPointer = std::weak_ptr<T>;

// TODO: Fix errors not showing until compile when invalid args are given
template<typename T, typename ... Args>
constexpr TPointer<T> CreatePointer(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
TPointer<T> Cast(TPointer<U> Base)
{
	return std::dynamic_pointer_cast<T>(Base);
}

template<typename T, typename U>
T* GetInterface(TPointer<U> Base)
{
	// TODO: Is dynamic correct?
	return dynamic_cast<T*>(Base.get());
}

template<typename T>
using TArray = std::vector<T>;

template<typename T>
using TScope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr TScope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
struct TObjectPtr
{
	std::string FilePath;

	TPointer<T> Get() const;
	
	void Serialize(std::ostream& os);
	void Deserialize(std::istream& is);

private:
	TPointer<T> Object = nullptr;
};