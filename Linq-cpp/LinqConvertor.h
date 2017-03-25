#pragma once

#include "LinqIterator.h"
template <typename T, typename Container>
class LinqConvert : public Iterator<T>
{
	LinqConvert() = default;
public:
	typename Container::iterator iteThis;
	std::shared_ptr<Container> container;

	LinqConvert(const LinqConvert&) = default;
	LinqConvert& operator =(const LinqConvert&) = default;

	LinqConvert(Container &cont)
	{
		this->container = std::shared_ptr<Container>(new Container(cont));
		iteThis = container->begin();
	}

	virtual LinqIterator Next() const override
	{
		typename Container::iterator next = iteThis + 1;
		if (next == container->end())
			return LinqIterator(nullptr);
		auto ret = new LinqConvert<T, Container>();
		ret->container = container;
		ret->iteThis = next;
		return LinqIterator(ret);
	}

	virtual const T& operator*() const override
	{
		return *iteThis;
	}
	virtual T& operator*()  override
	{
		return *iteThis;
	}
	virtual bool operator == (const Iterator & other)const  override
	{
		auto casted = dynamic_cast<const LinqConvert *> (&other);
		if (casted != nullptr)
		{
			return iteThis == casted->iteThis;
		}
		return false;
	}

};

template <typename Input>
auto LINQ(Input &input)
{
	using OUT = typename std::iterator_traits<typename Input::iterator>::value_type;
	Iterator<OUT>::LinqIterator ite(new LinqConvert<OUT, Input>(input));
	return IEnumerable<OUT>(ite);
}