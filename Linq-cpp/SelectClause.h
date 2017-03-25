#pragma once
#include "LinqIterator.h"

template<typename T>
class IEnumerable;

template< typename T, typename E >
class SelectClause: public Iterator<T>
{
	IEnumerable<E> base;
	std::function<T(const E&)> functor;
	T value;
public:
	SelectClause(IEnumerable<E> val, std::function<T(const E&)> func ) : base(val), functor(func)
	{
		value = functor(*base);
	}

	virtual LinqIterator Next() const
	{
		auto next = base.Next();
		if (!next.IsValid())
			return LinqIterator(nullptr);
		return LinqIterator( new SelectClause( base.Next(), functor ));
	}


	virtual const T& operator*() const
	{
		return value;
	}

	virtual T& operator*()
	{
		return value;
	}

	virtual bool operator == (const Iterator & other)const
	{
		throw "Not Implemented Exception";
	}

};

template < typename T >
template<typename Q, typename E = std::enable_if_t< std::is_class(T), T>::type>
IEnumerable<Q> IEnumerable<T>::Select(Q E::* field) const
{
	return IEnumerable<T>(
		LinqIterator(
			new SelectClause< T, Q >(*this, [&field](const T& a) -> Q
				{
					return a::*field;
				})
		));
}

template < typename T >
template<typename Q>
IEnumerable<Q> IEnumerable<T>::Select(std::function<Q(const T&)> func)const
{
	return IEnumerable<T>(
		LinqIterator(
			new SelectClause<T, Q>(*this, func)
		));
}
