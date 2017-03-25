#pragma once
#include "LinqIterator.h"


template<typename T>
class IEnumerable
{

	typedef typename Iterator<T>::LinqIterator LinqIterator;

	LinqIterator iterator;

	IEnumerable() = default;

public:

	virtual bool operator == (const IEnumerable& other) const
	{
		if (!IsValid() && !other.IsValid())
			return true;

		if (!IsValid() || !other.IsValid())
			return false;

		return *iterator == *other.iterator;
	}

	virtual bool operator != (const IEnumerable& other) const
	{
		return !operator==(other);
	}

	IEnumerable(const IEnumerable&) = default;
	virtual IEnumerable<T> &operator=(const IEnumerable<T>&) = default;

	IEnumerable(LinqIterator ite) : iterator(ite)
	{
	}

	virtual IEnumerable<T> Next() const
	{
		return IEnumerable(iterator->Next());
	}

	virtual IEnumerable<T> end() const
	{
		return IEnumerable(nullptr);
	}

	IEnumerable<T> operator ++()
	{
		auto next = Next();
		*this = next;
		return next;
	}


	virtual const T& operator*() const
	{
		return **iterator;
	}

	virtual T& operator*()
	{
		return **iterator;
	}

	bool IsValid() const
	{
		return iterator != nullptr;
	}

	IEnumerable<T> Where(std::function<bool(const T&)> func) const;
	
	template<typename Q, typename E= std::enable_if_t< std::is_class(T), T>::type>
	IEnumerable<Q> Select(Q E::* field)const;
	
	template<typename Q>
	IEnumerable<Q> Select(std::function<Q(const T&)> func)const;
	
};

#include "WhereClause.h"
#include "LinqConvertor.h"
#include "SelectClause.h"