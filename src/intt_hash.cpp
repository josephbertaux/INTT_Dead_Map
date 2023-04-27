#include "intt_hash.hpp"

bool operator<(intt_hash const& _lhs, intt_hash const& _rhs)
{
	return _lhs.hash < _rhs.hash;
}

bool operator>(intt_hash const& _lhs, intt_hash const& _rhs)
{
	return _lhs.hash > _rhs.hash;
}

bool operator<=(intt_hash const& _lhs, intt_hash const& _rhs)
{
	return _lhs.hash <= _rhs.hash;
}

bool operator>=(intt_hash const& _lhs, intt_hash const& _rhs)
{
	return _lhs.hash >= _rhs.hash;
}

bool operator==(intt_hash const& _lhs, intt_hash const& _rhs)
{
	return _lhs.hash == _rhs.hash;
}

//////////////////////////////////////////
//	PUBLIC:				//
//	>	Constructors		//
//	>	Destructor		//
//	>	Accessible members	//
//	>	Inherited members	//
//////////////////////////////////////////

intt_hash::intt_hash()
{
	hash = 0x0;
}

intt_hash::intt_hash(intt_hash const& _o)
{
	hash = _o.hash;
}

intt_hash::intt_hash(HASH const& _h)
{
	hash = _h;
}

intt_hash::intt_hash(std::array<HASH, NUM_FIELDS> const& _a)
{
	hash = find_hash(_a);
}

intt_hash::intt_hash(HASH const& _lyr, HASH const& _ldr, HASH const& _nos, HASH const& _chp, HASH const& _chn, HASH const& _adc)
{
	hash = 0x0;
	std::array<HASH, NUM_FIELDS> a = {0};

	a[i_lyr] = _lyr;
	a[i_ldr] = _ldr;
	a[i_nos] = _nos;
	a[i_chp] = _chp;
	a[i_chn] = _chn;
	a[i_adc] = _adc;

	hash = find_hash(a);
}

intt_hash::HASH& intt_hash::operator*()
{
	return hash;
}

intt_hash& intt_hash::operator++()
{
	//switch(hash)
	//{
	//	case end_hash:
	//		hash = begin_hash;
	//		return *this;

	//	case last_hash:
	//		hash = end_hash;
	//		return *this;

	//	default:
	//		break;
	//}

	if(hash == end_hash)
	{
		hash = begin_hash;
		return *this;
	}

	if(hash == last_hash)
	{
		hash = end_hash;
		return *this;
	}

	SHIFT s = NUM_FIELDS;
	while(true)
	{
		--s;
		if(s < 0)break;
		if(get_field(s) < last_val(s, hash))break;
	}

	if(s < 0)
	{
		hash = end_hash;
		return *this;
	}

	hash += 1 << bit_shfts[s];

	while(true)
	{
		++s;
		if(NUM_FIELDS <= s)break;
		set_field(s, 0);
	}

	return *this;
}

intt_hash& intt_hash::operator--()
{
	//switch(hash)
	//{
	//	case begin_hash:
	//		hash = end_hash;
	//		return *this;

	//	case end_hash:
	//		hash = last_hash;
	//		return *this;

	//	default:
	//		break;
	//}

	if(hash == begin_hash)
	{
		hash = end_hash;
		return *this;
	}

	if(hash == end_hash)
	{
		hash = last_hash;
		return *this;
	}

	SHIFT s = NUM_FIELDS;
	while(true)
	{
		--s;
		if(s < 0)break;
		if(0 < get_field(s))break;
	}

	if(s < 0)
	{
		hash = end_hash;
		return *this;
	}

	hash -= 1 << bit_shfts[s];

	while(true)
	{
		++s;
		if(NUM_FIELDS <= s)break;
		set_field(s, last_val(s, hash));
	}

	return *this;
}

intt_hash::HASH intt_hash::find_hash(std::array<HASH, NUM_FIELDS> const& _a)
{
	HASH h = 0x0;

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		h += (_a[s] & ((1 << bit_wdths[s]) - 1)) << bit_shfts[s];
	}

	return h;
}

intt_hash::HASH intt_hash::find_field(SHIFT const& _s, HASH const& _h)
{
	return (_h >> bit_shfts[_s]) & ((1 << bit_wdths[_s]) - 1);
}

std::array<intt_hash::HASH, intt_hash::NUM_FIELDS> intt_hash::find_indexes(HASH const& _h)
{
	std::array<HASH, NUM_FIELDS> a = {0};

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		a[s] = find_field(s, _h);
	}

	return a;
}

intt_hash intt_hash::begin()
{
	return intt_hash(begin_hash);
}

intt_hash intt_hash::last()
{
	return intt_hash(last_hash);
}

intt_hash intt_hash::end()
{
	return intt_hash(end_hash);
}

intt_hash::SHIFT intt_hash::size() const
{
	return bit_size;
}

intt_hash::SHIFT intt_hash::get_hash() const
{
	return hash;
}

intt_hash::HASH intt_hash::get_field(intt_hash::SHIFT const& _s) const
{
	return (hash >> bit_shfts[_s]) & ((1 << bit_wdths[_s]) - 1);
}

intt_hash::HASH intt_hash::set_field(intt_hash::SHIFT const& _s, intt_hash::HASH const& _h)
{
	hash -= hash & (((1 << bit_wdths[_s]) - 1) << bit_shfts[_s]);
	hash += (_h & ((1 << bit_wdths[_s]) - 1)) << bit_shfts[_s];

	return hash;
}

std::array<intt_hash::HASH, intt_hash::NUM_FIELDS> intt_hash::get_indexes() const
{
	std::array<HASH, NUM_FIELDS> a = {0};
	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		a[s] = get_field(s);
	}

	return a;
}

intt_hash::HASH intt_hash::pseudo_hash() const
{
	HASH h = 0x0;
	std::array<HASH, NUM_FIELDS> a = get_indexes();

	a[i_ldr] = 0;
	a[i_nos] = 0;
	a[i_chp] = a[i_chp] % 13;
	a[i_chn] = 0;

	h = find_hash(a);

	return h;
}

intt_hash::HASH intt_hash::phi_hash() const
{
	HASH h = 0x0;
	std::array<HASH, NUM_FIELDS> a = get_indexes();

	a[i_nos] = ~(a[i_nos] ^ (a[i_chp] / 13));
	a[i_chp] = 0;

	h = find_hash(a);

	return h;
}

intt_hash::HASH intt_hash::r_hash() const
{
	HASH h = 0x0;
	std::array<HASH, NUM_FIELDS> a = get_indexes();

	a[i_ldr] = 0;
	a[i_nos] = ~(a[i_nos] ^ (a[i_chp] / 13));
	a[i_chp] = 0;

	h = find_hash(a);

	return h;
}

intt_hash::HASH intt_hash::z_hash() const
{
	HASH h = 0x0;
	std::array<HASH, NUM_FIELDS> a = get_indexes();

	a[i_lyr] = 0;
	a[i_ldr] = 0;
	a[i_chp] = a[i_chp] % 13;
	a[i_chn] = 0;

	h = find_hash(a);

	return h;
}

//////////////////////////////////////////
//	PRIVATE:			//
//	>	Non-inherited members	//
//	>	Helper functions	//
//////////////////////////////////////////

intt_hash::HASH intt_hash::last_val(SHIFT const& _s, HASH const& _h)
{
	//each case can only depend on the preceding cases
	switch(_s)
	{
		case i_lyr:	return 3;
		case i_ldr:	return find_field(i_lyr, _h) < 2 ? 11 : 15;
		case i_nos:	return 1;
		case i_chp:	return 25;
		case i_chn:	return 127;
		case i_adc:	return 7;
		default:	break;
	}

	return end_hash;
}

intt_hash::HASH intt_hash::max_val(SHIFT const& _s)
{
	switch(_s)
	{
		case i_lyr:	return 4;
		case i_ldr:	return 16;
		case i_nos:	return 2;
		case i_chp:	return 26;
		case i_chn:	return 128;
		case i_adc:	return 8;
		default:	break;
	}

	return end_hash;
}

const std::array<intt_hash::SHIFT, intt_hash::NUM_FIELDS> intt_hash::max_vals = intt_hash::_max_vals();

constexpr std::array<intt_hash::SHIFT, intt_hash::NUM_FIELDS> intt_hash::_max_vals()
{
	SHIFT pos = 0x0;
	std::array<SHIFT, NUM_FIELDS> a = {0};

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		a[s] = max_val(s);
	}

	return a;
};

const std::array<intt_hash::SHIFT, intt_hash::NUM_FIELDS> intt_hash::bit_wdths = intt_hash::_bit_wdths();

constexpr std::array<intt_hash::SHIFT, intt_hash::NUM_FIELDS> intt_hash::_bit_wdths()
{
	SHIFT pos = 0x0;
	std::array<SHIFT, NUM_FIELDS> a = {0};

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		pos = 0x0;
		while(1 << pos < max_vals[s])++pos;
		a[s] = pos;
	}

	return a;
}

const std::array<intt_hash::SHIFT, intt_hash::NUM_FIELDS> intt_hash::bit_shfts = intt_hash::_bit_shfts();

constexpr std::array<intt_hash::SHIFT, intt_hash::NUM_FIELDS> intt_hash::_bit_shfts()
{
	SHIFT pos = 0x0;
	std::array<SHIFT, NUM_FIELDS> a = {0};

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		a[s] = 0x0;
		pos += bit_wdths[s];
	}

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		pos -= bit_wdths[s];
		a[s] = pos;
	}

	return a;
}

const intt_hash::SHIFT intt_hash::bit_size = intt_hash::_bit_size();

constexpr intt_hash::SHIFT intt_hash::_bit_size()
{
	SHIFT pos = 0x0;

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		pos += bit_wdths[s];
	}

	return ++pos;
}

const intt_hash::HASH intt_hash::begin_hash = intt_hash::_begin_hash();

constexpr intt_hash::HASH intt_hash::_begin_hash()
{
	return 0x0;
}

const intt_hash::HASH intt_hash::last_hash = intt_hash::_last_hash();

constexpr intt_hash::HASH intt_hash::_last_hash()
{
	HASH h = 0x0;

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		h += last_val(s, h) << bit_shfts[s];
	}

	return h;
}

const intt_hash::HASH intt_hash::end_hash = intt_hash::_end_hash();

constexpr intt_hash::HASH intt_hash::_end_hash()
{
	return 1 << bit_size;
}

