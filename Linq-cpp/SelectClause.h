#pragma once
#include "LinqIterator.h"

template<typename T>
class IEnumerable;

template< typename Q, typename E >
class SelectClause: public Iterator<Q>
{
	IEnumerable<E> base;
	std::function<Q(const E&)> functor;
	Q value;
public:
	SelectClause(IEnumerable<E> val, std::function<Q(const E&)> func ) : base(val), functor(func)
	{
		value = functor(*base);
	}

	virtual LinqIterator Next() const
	{
		auto next = base.Next();
		if (!next.IsValid())
			return LinqIterator(nullptr);
		return LinqIterator( new SelectClause( next, functor ));
	}


	virtual const Q& operator*() const
	{
		return value;
	}

	virtual Q& operator*()
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
	std::function<Q(const T&)> func = [&field](const T& a) -> Q
	{
		return a.*field;
	};
	return IEnumerable<Q>(
		Iterator<Q>::LinqIterator(
		new SelectClause<Q, T>(*this, func)
	));
}

template < typename T >
template<typename Q = T>
IEnumerable<Q> IEnumerable<T>::Select(std::function<Q(const T&)> func)const
{
	return IEnumerable<Q>(
		Iterator<Q>::LinqIterator(
			new SelectClause<Q, T>(*this, func)
		));
}
