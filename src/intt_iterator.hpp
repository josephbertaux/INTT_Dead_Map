#ifndef INTT_ITERATOR_HPP
#define INTT_ITERATOR_HPP

#include "hash_iterator.hpp"

class intt_iterator : public hash_iterator
{
public:
	using hash_iterator::SHIFT;
	using hash_iterator::HASH;
	using hash_iterator::hash_iterator;

	enum INTT_index
	{
		i_lyr,	//most significant leading bit field
		i_ldr,
		i_nos,
		i_chp,
		i_chn,
		i_adc,	//least significant trailing bit field

		MAX	
	};

	static SHIFT intt_num_fields();
	static HASH intt_max_val(SHIFT const& _s);
	static HASH intt_last_val(SHIFT const& _s, HASH const&, SHIFT* const&, SHIFT* const&);

	intt_iterator();

	HASH pseudo_hash() const override;
	HASH phi_hash() const override;
};

#endif
