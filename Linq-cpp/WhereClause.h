#pragma once
#include "LinqIterator.h"

template<typename T>
class IEnumerable;


template<typename T>
class WhereClause : public Iterator<T>
{
	mutable IEnumerable<T> base;
	std::function<bool(const T&)> functor;

	IEnumerable<T> GetNextValid(IEnumerable<T> &from) const
	{
		auto ite = from;

		while (ite.IsValid() && !functor(*ite))
			ite = ite.Next();

		return ite;
	}

	WhereClause(std::function<bool(const T&)> func) : base(nullptr),
		functor(func)
	{
	}

public:
	WhereClause(const IEnumerable<T> &iter, std::function<bool(const T&)> func) : base(nullptr),
		functor(func)
	{
		auto temp(iter);
		base = GetNextValid(temp);
	}


	virtual LinqIterator Next() const override
	{

		auto ite = GetNextValid(base.Next());
		if (ite.IsValid())
		{
			auto clause = new WhereClause<T>(functor);
			clause->base = ite;
			return LinqIterator(clause);
		}
		return nullptr;
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
		return CheckEquality<WhereClause<T>>(other, [this](const WhereClause<T> & b) { return this->base == b.base; });
	}

};

template < typename T>
IEnumerable<T> IEnumerable<T>::Where(std::function<bool(const T&)> func) const
{
	return IEnumerable<T>(LinqIterator(new WhereClause<T>(*this, func)));
}
