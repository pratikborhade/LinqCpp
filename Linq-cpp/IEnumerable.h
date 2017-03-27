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

	virtual IEnumerable<T> begin() const
	{
		return *this;
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

	std::vector<T> ToList() const
	{
		std::vector<T> returnVec;
		for (auto i = *this; i != end(); ++i)
			returnVec.push_back(*i);
		return returnVec;
	}

	T Sum() const
	{
		T sum = 0;

		for (auto i : *this)
			sum += i;

		return sum;
	}

	IEnumerable<T> Where(std::function<bool(const T&)> func) const;
	
	template<typename Q, typename E= std::enable_if_t< std::is_class(T), T>::type>
	IEnumerable<Q> Select(Q E::* field)const;
	
	template<typename Q = T>
	IEnumerable<Q> Select(std::function<Q(const T&)> func)const;


	IEnumerable<T> Skip(size_t value) const
	{
		if (value == 0)
			return end();
		auto ite = *this;
		auto e = end();
		for (int i = 0; i < value; ++i)
		{
			++ite;
			if (ite == e)
				return e;
		}
		return ite;
	}
	
	IEnumerable<T> Take(unsigned int value) const;


	IEnumerable<T> TakeWhile(std::function<bool(const T&)> functor) const;


	IEnumerable<T> SkipWhile(std::function<bool (const T&)> functor) const
	{
		auto ite = *this;
		++ite;
		auto e = end();
		while (ite != end() && functor(*ite))
			++ite;
		return ite;
	}
};

#include "WhereClause.h"
#include "LinqConvertor.h"
#include "SelectClause.h"
#include "RangeClause.h"
#include "TakeClause.h"