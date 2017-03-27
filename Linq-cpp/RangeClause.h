#pragma once
#include "LinqIterator.h"

template<typename T>
class IEnumerable;

template <typename T>
class RangeIterator : public Iterator<T>
{
	T start;
	T end;
	T step;
	T current;
public:

	RangeIterator(T start, T end, T step, T current)
		: start(start), end(end), step(step), current(current)
	{
	}

	virtual LinqIterator Next() const override
	{
		auto val = current + step;
		if (val >= end)
			return nullptr;
		return LinqIterator( new RangeIterator(start, end, step, val) );
	}

	virtual const T& operator*() const override
	{
		return current;
	}

	virtual T& operator*() override
	{
		return current;
	}

	virtual bool operator == (const Iterator & other)const override
	{
		return CheckEquality<RangeIterator<T>>(other, [this](const RangeIterator<T> & b) 
		{ 
			return this->current == b.current && this->start == b.start && this->end == b.end && this->step == b.step ;
		});
	}

};

template <typename T>
IEnumerable<T> RANGE(T start, T end, T step = 1)
{
	return IEnumerable<T>(
		Iterator<T>::LinqIterator( new RangeIterator<T>( start, end, step, start ))
		);
}

template <typename T>
IEnumerable<T> RANGE(T end)
{
	return IEnumerable<T>(
		Iterator<T>::LinqIterator(new RangeIterator<T>(start, end, step, start))
		);
}