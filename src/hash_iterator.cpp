#include "hash_iterator.hpp"

//////////////////////////////////////////
//	PUBLIC:				//
//	>	Constructors		//
//	>	Destructor		//
//	>	Accessible members	//
//	>	Inherited members	//
//////////////////////////////////////////

hash_iterator::hash_iterator()
{
	//Do nothing
}

hash_iterator::hash_iterator(_NUM_FIELDS const& _num_fields, _MAX_VAL const& _max_val, _LAST_VAL const& _last_val)
{
	std::cout << "Begining the parent constructor we want..." << std::endl;

	if(!_num_fields || !_max_val || !_last_val)return;

	NUM_FIELDS = (*_num_fields)();

	std::cout << "NUM_FIELDS should be correct: " << NUM_FIELDS << std::endl;

	if(!NUM_FIELDS)return;

	SHIFT s = 0;
	SHIFT pos = 0;

	bit_wdths = new SHIFT[NUM_FIELDS];
	bit_shfts = new SHIFT[NUM_FIELDS];
	for(s = 0; s < NUM_FIELDS; ++s)
	{
		pos = 0;
		while(((1 << pos) - 1) < (*_max_val)(s))++pos;
		bit_wdths[s] = pos;
	}
	pos = 0;
	for(s = 0; s < NUM_FIELDS; ++s)
	{
		pos += bit_wdths[s];
	}
	for(s = 0; s < NUM_FIELDS; ++s)
	{
		pos -= bit_wdths[s];
		bit_shfts[s] = pos;
	}
	LAST_VAL = _last_val;

	std::cout << "Check the bit arrays:" << std::endl;
	for(s = 0; s < NUM_FIELDS; ++s)
	{
		std::cout << "\twidth: " << bit_wdths[s] << "\tshift: " << "\t" << bit_shfts[s] << std::endl;
	}

	pos = 0;	
	for(s = 0; s < NUM_FIELDS; ++s)
	{
		pos += bit_wdths[s];
	}
	SIZE = ++pos;

	std::cout << "SIZE should be correct: " << SIZE << std::endl;

	BEGIN_HASH = 0;
	hash = 0;
	for(s = 0; s < NUM_FIELDS; ++s)
	{
		hash += (*LAST_VAL)(s, hash, bit_wdths, bit_shfts) << bit_shfts[s];
	}
	LAST_HASH = hash;
	END_HASH = 1 << SIZE;

	hash = 0;

	std::cout << "Exiting the constructor..." << std::endl;
}

hash_iterator::hash_iterator(hash_iterator const& _o)
{
	NUM_FIELDS = _o.NUM_FIELDS;

	if(!NUM_FIELDS)return;

	bit_wdths = new SHIFT[NUM_FIELDS];
	bit_shfts = new SHIFT[NUM_FIELDS];
	LAST_VAL = _o.LAST_VAL;

	for(SHIFT s = 0; s < NUM_FIELDS; ++s)
	{
		bit_wdths[s] = _o.bit_wdths[s];
		bit_shfts[s] = _o.bit_shfts[s];
	}

	BEGIN_HASH = _o.BEGIN_HASH;
	LAST_HASH = _o.LAST_HASH;
	END_HASH = _o.END_HASH;

	SIZE = _o.SIZE;

	hash = _o.hash;
}

hash_iterator::hash_iterator(hash_iterator const& _o, HASH const& _h):
	hash_iterator(_o)
{
	hash = _h;
}

hash_iterator::~hash_iterator()
{
	if(NUM_FIELDS)
	{
		delete[] bit_wdths;
		delete[] bit_shfts;
	}
}

hash_iterator::HASH& hash_iterator::operator*()
{
	return hash;
}

hash_iterator& hash_iterator::operator++()
{
	if(hash == END_HASH)
	{
		hash = BEGIN_HASH;
		return *this;
	}

	if(hash == LAST_HASH)
	{
		hash = END_HASH;
		return *this;
	}

	SHIFT s = NUM_FIELDS;
	while(true)
	{
		--s;
		if(s < 0)break;
		if(get_field(s) < (*LAST_VAL)(s, hash, bit_wdths, bit_shfts))break;
	}

	if(s < 0)
	{
		hash = END_HASH;
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

hash_iterator& hash_iterator::operator--()
{
	if(hash == BEGIN_HASH)
	{
		hash = END_HASH;
		return *this;
	}

	if(hash == END_HASH)
	{
		hash = LAST_HASH;
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
		hash = END_HASH;
		return *this;
	}

	hash -= 1 << bit_shfts[s];

	while(true)
	{
		++s;
		if(NUM_FIELDS <= s)break;
		set_field(s, (*LAST_VAL)(s, hash, bit_wdths, bit_shfts));
	}

	return *this;
}

bool operator<(hash_iterator const& _lhs, hash_iterator const& _rhs)
{
	return _lhs.hash < _rhs.hash;
}

bool operator>(hash_iterator const& _lhs, hash_iterator const& _rhs)
{
	return _lhs.hash > _rhs.hash;
}

bool operator<=(hash_iterator const& _lhs, hash_iterator const& _rhs)
{
	return _lhs.hash <= _rhs.hash;
}

bool operator>=(hash_iterator const& _lhs, hash_iterator const& _rhs)
{
	return _lhs.hash >= _rhs.hash;
}

bool operator==(hash_iterator const& _lhs, hash_iterator const& _rhs)
{
	return _lhs.hash == _rhs.hash;
}

bool operator!=(hash_iterator const& _lhs, hash_iterator const& _rhs)
{
	return _lhs.hash != _rhs.hash;
}

hash_iterator hash_iterator::begin()
{
	return hash_iterator(*this, BEGIN_HASH);
}

hash_iterator hash_iterator::last()
{
	return hash_iterator(*this, LAST_HASH);
}

hash_iterator hash_iterator::end()
{
	return hash_iterator(*this, END_HASH);
}

hash_iterator::SHIFT hash_iterator::size() const
{
	return SIZE;
}

hash_iterator::SHIFT hash_iterator::num_fields() const
{
	return NUM_FIELDS;
}

hash_iterator::HASH hash_iterator::get_hash() const
{
	return hash;
}

hash_iterator::HASH hash_iterator::get_field(SHIFT const& _s, HASH const& _h, SHIFT* const& _bit_wdths, SHIFT* const& _bit_shfts)
{
	return (_h >> _bit_shfts[_s]) & ((1 << _bit_wdths[_s]) - 1);
}

hash_iterator::HASH hash_iterator::get_field(SHIFT const& _s) const
{
	return (hash >> bit_shfts[_s]) & ((1 << bit_wdths[_s]) - 1);
}

hash_iterator::HASH hash_iterator::set_field(SHIFT const& _s, HASH const& _h)
{
	hash -= hash & (((1 << bit_wdths[_s]) - 1) << bit_shfts[_s]);
	hash += (_h & ((1 << bit_wdths[_s]) - 1)) << bit_shfts[_s];

	return hash;
}

hash_iterator::HASH hash_iterator::pseudo_hash() const
{
	HASH h = 0;

	return h;
}

hash_iterator::HASH hash_iterator::phi_hash() const
{
	HASH h = 0;

	return h;
}

hash_iterator::HASH hash_iterator::r_hash() const
{
	HASH h = 0;

	return h;
}

hash_iterator::HASH hash_iterator::z_hash() const
{
	HASH h = 0;

	return h;
}
