#include "intt_iterator.hpp"

intt_iterator::SHIFT intt_iterator::intt_num_fields()
{
	return MAX;
}

intt_iterator::HASH intt_iterator::intt_max_val(SHIFT const& _s)
{
	switch(_s)
	{
		case i_lyr:	return 3;
		case i_ldr:	return 15;
		case i_nos:	return 1;
		case i_chp:	return 25;
		case i_chn:	return 127;
		case i_adc:	return 7;
	}

	return 0;
}

intt_iterator::HASH intt_iterator::intt_last_val(SHIFT const& _s, HASH const& _h, SHIFT* const& _bit_wdths, SHIFT* const& _bit_shfts)
{
	switch(_s)
	{
		case i_lyr:	return 3;
		case i_ldr:	return get_field(i_lyr, _h, _bit_wdths, _bit_shfts) < 2 ? 11 : 15;
		case i_nos:	return 1;
		case i_chp:	return 25;
		case i_chn:	return 127;
		case i_adc:	return 7;
	}

	return 0;
}

intt_iterator::intt_iterator() :
	hash_iterator(&intt_num_fields, &intt_max_val, &intt_last_val)
{
}

intt_iterator::HASH intt_iterator::pseudo_hash() const
{
	HASH h = hash;

	set_field(i_chp, get_field(i_chp) % 13,
			h, bit_wdths, bit_shfts);

	set_field(i_ldr, 0, 
			h, bit_wdths, bit_shfts);
	set_field(i_chn, 0,
			h, bit_wdths, bit_shfts);

	return h;
}

intt_iterator::HASH intt_iterator::phi_hash() const
{
	HASH h = hash;

	set_field(i_chp, (get_field(i_chp) / 13 + get_field(i_nos)) % 2,
			h, bit_wdths, bit_shfts);

	set_field(i_nos, 0,
			h, bit_wdths, bit_shfts);

	return h;
}
