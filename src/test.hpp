#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>

#include "hash_iterator.hpp"

class my_itr_class : public hash_iterator
{
public:
	using hash_iterator::SHIFT;
	using hash_iterator::HASH;
	using hash_iterator::hash_iterator;

	enum MyIndexes
	{
		i_max,	//most significant bit field
		i_mid,	//...
		i_min,	//least significant bit field

		MAX	
	};

	static SHIFT GetNumFields();
	static HASH GetMaxVal(SHIFT const& _s);
	static HASH GetLastVal(SHIFT const& _s, HASH const&, SHIFT* const&, SHIFT* const&);

	my_itr_class();
};

#endif
