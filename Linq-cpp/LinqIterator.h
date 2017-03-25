#pragma once
#include <memory>
#include <type_traits>
#include <functional>
template<typename T>
class Iterator
{
public:
	typedef std::shared_ptr<Iterator<T>> LinqIterator;
	Iterator() = default;

	virtual LinqIterator Next() const = 0;
	virtual const T& operator*() const = 0;
	virtual T& operator*() = 0;
	virtual bool operator == (const Iterator & other)const = 0;

	virtual ~Iterator() = default;
protected:

	template<typename Q>
	bool CheckEquality(const Iterator<T> &member, std::function<bool(const Q& second)> functor) const
	{
		auto isinstance = dynamic_cast<const Q*> (&member);
		if (isinstance == nullptr)
			return false;
		if (functor)
			return functor( *isinstance );
		return true;
	}

};
