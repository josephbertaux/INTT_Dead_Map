#ifndef INTT_HASH_H
#define INTT_HASH_H

#include <iostream>
#include <array>

class intt_hash;

bool operator<(intt_hash const&, intt_hash const&);
bool operator>(intt_hash const&, intt_hash const&);
bool operator<=(intt_hash const&, intt_hash const&);
bool operator>=(intt_hash const&, intt_hash const&);
bool operator==(intt_hash const&, intt_hash const&);

class intt_hash
{
public:
	typedef int HASH;
	typedef int SHIFT;

	enum BIT_SHIFTS : SHIFT
	{
		i_lyr,
		i_ldr,
		i_nos,
		i_chp,
		i_chn,
		i_adc,

		NUM_FIELDS
	};

	friend bool operator<(intt_hash const&, intt_hash const&);
	friend bool operator>(intt_hash const&, intt_hash const&);
	friend bool operator<=(intt_hash const&, intt_hash const&);
	friend bool operator>=(intt_hash const&, intt_hash const&);
	friend bool operator==(intt_hash const&, intt_hash const&);

	intt_hash();
	intt_hash(intt_hash const&);
	intt_hash(HASH const&);
	intt_hash(std::array<HASH, NUM_FIELDS> const&);
	intt_hash(HASH const&, HASH const&, HASH const&, HASH const&, HASH const&, HASH const&);

	HASH& operator*();
	intt_hash& operator++();
	intt_hash& operator--();

	static HASH find_hash(std::array<HASH, NUM_FIELDS> const&);
	static HASH find_field(SHIFT const&, HASH const&);
	static std::array<HASH, NUM_FIELDS> find_indexes(HASH const&);

	static intt_hash begin();
	static intt_hash last();
	static intt_hash end();

	virtual SHIFT size() const;
	virtual HASH get_hash() const;
	virtual HASH get_field(SHIFT const&) const;
	virtual HASH set_field(SHIFT const&, HASH const&);
	virtual std::array<HASH, NUM_FIELDS> get_indexes() const;

	virtual HASH pseudo_hash() const;
	virtual HASH phi_hash() const;
	virtual HASH r_hash() const;
	virtual HASH z_hash() const;

protected:
	HASH hash;

private:
	//const(expr) static functions can only be used in the same file that they are implemented
	//these are declared here and calls exist only in intt_hash.cpp
	//thus they are relegated to only be called as helper functions in intt_hash.cpp
	//when implementing other static functions
	static HASH last_val(SHIFT const&, HASH const&);
	static HASH max_val(SHIFT const&);

	const static std::array<HASH, NUM_FIELDS> max_vals;
	constexpr static std::array<HASH, NUM_FIELDS> _max_vals();

	const static std::array<SHIFT, NUM_FIELDS> bit_wdths;
	constexpr static std::array<SHIFT, NUM_FIELDS> _bit_wdths();

	const static std::array<SHIFT, NUM_FIELDS> bit_shfts;
	constexpr static std::array<SHIFT, NUM_FIELDS> _bit_shfts();


	const static SHIFT bit_size;
	constexpr static SHIFT _bit_size();

	const static HASH begin_hash;
	constexpr static HASH _begin_hash();

	const static HASH last_hash;
	constexpr static HASH _last_hash();

	const static HASH end_hash;
	constexpr static HASH _end_hash(); 
};

#endif
