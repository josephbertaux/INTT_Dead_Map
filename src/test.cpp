#include "test.hpp"

my_itr_class::SHIFT my_itr_class::GetNumFields()
{
	return MAX;
}

my_itr_class::HASH my_itr_class::GetMaxVal(SHIFT const& _s)
{
	switch(_s)
	{
		case i_max:	return 1;
		case i_mid:	return 3;
		case i_min:	return 2;
	}

	return 0;
}

my_itr_class::HASH my_itr_class::GetLastVal(SHIFT const& _s, HASH const& _h, SHIFT* const& _bit_wdths, SHIFT* const& _bit_shfts)
{
	switch(_s)
	{
		case i_max:	return 1;
		case i_mid:	return get_field(i_max, _h, _bit_wdths, _bit_shfts) > 0 ? 3 : 2;
		case i_min:	return 2;
	}

	return 0;
}

my_itr_class::my_itr_class() :
	hash_iterator(&GetNumFields, &GetMaxVal, &GetLastVal)
{
}
