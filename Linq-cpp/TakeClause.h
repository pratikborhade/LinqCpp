#pragma once
#pragma once
#include "LinqIterator.h"

template<typename T>
class IEnumerable;


template<typename T>
class TakeClause : public Iterator<T>
{
	mutable IEnumerable<T> base;
	std::function<bool(const T&)> func;

	enum Type
	{
		functor,
		value
	};

	Type type;
	size_t number;
public:
	TakeClause(const IEnumerable<T> &iter, std::function<bool(const T&)> func) : base(iter),
		func(func), type(Type::functor)
	{
	}

	TakeClause(const IEnumerable<T> &iter, size_t val) : base(iter),
		number(val), type(Type::value)
	{
	}



	virtual LinqIterator Next() const override
	{
		LinqIterator ite = nullptr;
		auto next = ++base;
		if (!next.IsValid())
			return nullptr;
		switch (type)
		{
		case Type::functor:
			if (func(*base))
			{
				ite.reset( new TakeClause(next, func) );
			}
			break;
		case Type::value:
			if (number > 0)
			{
				ite.reset( new TakeClause(next, number-1) );
			}
			break;
		}
		return ite;
	}

	virtual const T& operator*() const override
	{
		return *base;
	}

	virtual T& operator*() override
	{
		return *base;
	}

	virtual bool operator == (const Iterator & other)const override
	{
		return  false;
	}

};

template < typename T>
IEnumerable<T> IEnumerable<T>::Take(size_t take) const
{
	/*IEnumerable<T> temp = *this;
	IEnumerable<T> next = ++temp;
	if (next == end())
		return next;*/
	return IEnumerable<T>(LinqIterator(new TakeClause<T>(*this, take)));
}


template < typename T>
IEnumerable<T> IEnumerable<T>::TakeWhile(std::function<bool(const T&)> func) const
{
	/*IEnumerable<T> temp = *this;
	IEnumerable<T> next = ++temp;
	if (next == end())
		return next;*/
	return IEnumerable<T>(LinqIterator(new TakeClause<T>(*this, func)));
}