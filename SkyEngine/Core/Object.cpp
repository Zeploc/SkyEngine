#include "SEPCH.h"
#include "Object.h"

template <typename T>
TPointer<T> TObjectPtr<T>::Get() const
{
	
}

template <typename T>
void TObjectPtr<T>::Serialize(std::ostream& os)
{
	os << FilePath;
}

template <typename T>
void TObjectPtr<T>::Deserialize(std::istream& is)
{
	is >> FilePath;
}