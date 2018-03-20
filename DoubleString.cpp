

#include "DoubleString.hpp"



#define CHAR_BIT		8
#define DBL_MANT_DIG	53I32
#define DBL_MAX_EXP		1024I32
#define DBL_MIN_EXP		(-1021I32) 
#define LN10			2.3025850929940456840179914547
#define LN1_2217		0.2002433314278771112016301167
#define __MAX(a,b)		(((a) > (b)) ? (a) : (b))
#define __MIN(a,b)		(((a) < (b)) ? (a) : (b))


#ifndef _countof
#ifdef __cplusplus
extern "C++"
{
	template <typename _CountofType, size_t _SizeOfArray>
	char(*__countof_helper(_UNALIGNED _CountofType(&_Array)[_SizeOfArray]))[_SizeOfArray];
	#define _countof(_Array) (sizeof(*__countof_helper(_Array)) + 0U)
}
#else
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif
#endif


enum : INT32
{
	mantissa_bits = DBL_MANT_DIG,
	exponent_bits = (INT32)sizeof(double) * (INT32)CHAR_BIT - DBL_MANT_DIG,
	maximum_binary_exponent = DBL_MAX_EXP - 1I32,
	minimum_binary_exponent = DBL_MIN_EXP - 1I32,
	exponent_bias = 1023I32
};

enum : UINT64
{
	exponent_mask = (1UI64 << (exponent_bits)) - 1UI64,
	normal_mantissa_mask = (1UI64 << (mantissa_bits)) - 1UI64,
	denormal_mantissa_mask = (1UI64 << (mantissa_bits - 1UI64)) - 1UI64,
	special_nan_mantissa_mask = (1UI64 << (mantissa_bits - 2UI64))
};

enum : INT32
{
	maximum_temporary_decimal_exponent = 5200I32,
	minimum_temporary_decimal_exponent = -5200I32
};

struct double_type
{
	UINT64 _mantissa : mantissa_bits - 1I32;
	UINT64 _exponent : exponent_bits;
	UINT64 _sign : 1I32;
};


#pragma optimize( "", off )
static inline VOID Memcpy(LPVOID lpDst, LPCVOID lpSrc, SIZE_T dwLen)
{
	SIZE_T n = dwLen & ~(sizeof(UINT64) - 1U);
	PUINT64 pDst64 = (PUINT64)lpDst;
	PUINT64 pSrc64 = (PUINT64)lpSrc;
	while (n)
	{
		*pDst64++ = *pSrc64++;
		n -= sizeof(UINT64);
	}
	n = dwLen & (sizeof(UINT64) - 1U);
	PUINT8 pDst8 = (PUINT8)pDst64;
	PUINT8 pSrc8 = (PUINT8)pSrc64;
	while (n--)
		*pDst8++ = *pSrc8++;
}
#pragma optimize( "", on )

#ifdef _DEBUG
#pragma optimize( "", off )
static inline VOID Memset(LPVOID lpDst, int Val, SIZE_T dwLen)
{
	PUINT8 pDst8 = (PUINT8)lpDst;
	while (dwLen--)
		*pDst8++ = (UINT8)Val;
}
#pragma optimize( "", on )
#endif

static inline VOID Memsetzero(LPVOID lpDst, SIZE_T dwLen)
{
	SIZE_T n = dwLen & ~(sizeof(UINT64) - 1U);
	PUINT64 pDst64 = (PUINT64)lpDst;
	while (n)
	{
		*pDst64++ = (UINT64)0U;
		n -= sizeof(UINT64);
	}
	n = dwLen & (sizeof(UINT64) - 1U);
	PUINT8 pDst8 = (PUINT8)pDst64;
	while (n--)
		*pDst8++ = (UINT8)0U;
}

static inline double Log(double x)
{
	int k = 0, l = 0;
	for (; x > 1; ++k)
		x /= 10.0;
	for (; x <= 0.1; --k)
		x *= 10.0;
	for (; x < 0.9047; --l)
		x *= 1.2217;
	double y = (x - 1.0) / (x + 1.0);
	double y2 = y * y;
	double t = y2;
	double z = t / 3.0;
	double v = 1.0;
	for (UINT32 i = 3UI32; z; z = (t *= y2) / (double)(i += 2UI32))
		v += z;
	return k * LN10 + l * LN1_2217 + v * y * 2;
}

static inline INT32 Ceil(double x)
{
	double_type& components = (double_type&)x;
	if (!components._exponent && !components._mantissa)
		return 0I32;
	if (components._sign)
		return (INT32)x;
	return (INT32)x + 1I32;
}


struct big_integer
{
	big_integer() : used(0UI32)
	{
	#ifdef _DEBUG
		Memset(data, 0xcc, sizeof(data));
	#endif
	}
	big_integer(const big_integer& other) : used(other.used)
	{
		Memcpy(data, other.data, other.used * sizeof(UINT32));
	}
	big_integer& operator=(const big_integer& other)
	{
		used = other.used;
		Memcpy(data, other.data, (size_t)other.used * sizeof(UINT32));
		return *this;
	}
	enum : UINT32
	{
		maximum_bits = 1074UI32 + 2552UI32 + 32UI32, // 1074 bits required to represent 2^1074 | ceil(log2(10^768)) | shift space
		element_bits = (UINT32)sizeof(UINT32) * (UINT32)CHAR_BIT,
		element_count = (maximum_bits + element_bits - 1UI32) / element_bits
	};
	UINT32 used;
	UINT32 data[element_count];
};

static inline BOOL operator==(const big_integer& lhs, const big_integer& rhs)
{
	if (lhs.used != rhs.used)
		return FALSE;
	for (UINT32 i = 0UI32; i != lhs.used; ++i)
		if (lhs.data[i] != rhs.data[i])
			return FALSE;
	return TRUE;
}

static inline BOOL operator!=(const big_integer& lhs, const big_integer& rhs)
{
	return !(rhs == lhs);
}

static inline BOOL operator<(const big_integer& lhs, const big_integer& rhs)
{
	if (lhs.used > rhs.used)
		return FALSE;
	if (lhs.used < rhs.used)
		return TRUE;
	UINT32 i = lhs.used - 1UI32;
	for (; i != (UINT32)(-1) && lhs.data[i] == rhs.data[i]; --i)
	{
	}
	if (i == (UINT32)(-1))
		return FALSE;
	if (lhs.data[i] <= rhs.data[i])
		return TRUE;
	return FALSE;
}

static inline BOOL operator>=(const big_integer& lhs, const big_integer& rhs)
{
	return !(lhs < rhs);
}

static inline big_integer make_big_integer(UINT64 value)
{
	big_integer x;
	x.data[0] = value & 0xFFFFFFFFUI64;
	x.data[1] = (UINT32)(value >> 32UI64);
	x.used = x.data[1] ? 2UI32 : 1UI32;
	return x;
}

static inline big_integer make_big_integer_power_of_two(UINT32 power)
{
	UINT32 one = 1UI32;
	big_integer x;
	UINT32 element_index = power / big_integer::element_bits;
	UINT32 bit_index = power % big_integer::element_bits;
	Memsetzero(x.data, (size_t)element_index * sizeof(UINT32));
	x.data[element_index] = (one << bit_index);
	x.used = element_index + 1UI32;
	return x;
}

static inline BOOL is_zero(const big_integer& value)
{
	return value.used == 0UI32;
}

static inline UINT32 bit_scan_reverse(UINT32 value)
{
	UINT32 index;
	if (_BitScanReverse((LPDWORD)&index, value))
		return index + 1UI32;
	return 0UI32;
}

static inline UINT32 bit_scan_reverse(UINT64 value)
{
	if (value > 0xFFFFFFFFUI64)
		return bit_scan_reverse(((PUINT32)(&value))[1]) + 32UI32;
	else
		return bit_scan_reverse(((PUINT32)(&value))[0]);
}

static inline UINT32 bit_scan_reverse(const big_integer& x)
{
	if (x.used == 0UI32)
		return 0UI32;
	return (x.used - 1UI32) * big_integer::element_bits + bit_scan_reverse(x.data[x.used - 1UI32]);
}

static inline BOOL shift_left(big_integer& x, UINT32 n)
{
	UINT32 unit_shift = n / big_integer::element_bits;
	UINT32 bit_shift = n % big_integer::element_bits;
	UINT64 one = 1UI64;
	UINT32 msb_bits = bit_shift;
	UINT32 lsb_bits = big_integer::element_bits - msb_bits;
	UINT32 lsb_mask = (UINT32)((one << lsb_bits) - one);
	UINT32 msb_mask = ~lsb_mask;
	BOOL bit_shifts_into_next_unit = bit_shift > (big_integer::element_bits - bit_scan_reverse(x.data[x.used - 1UI32]));
	BOOL unit_shift_will_overflow = x.used + unit_shift > big_integer::element_count;
	BOOL bit_shift_will_overflow = x.used + unit_shift == big_integer::element_count && bit_shifts_into_next_unit;
	if (unit_shift_will_overflow || bit_shift_will_overflow)
	{
		x = big_integer();
		return FALSE;
	}
	UINT32 max_destination_index = __MIN(x.used + unit_shift, big_integer::element_count - 1UI32);
	for (UINT32 destination_index = max_destination_index; destination_index != (UINT32)(-1) && destination_index >= unit_shift; --destination_index)
	{
		UINT32 upper_source_index = destination_index - unit_shift;
		UINT32 lower_source_index = destination_index - unit_shift - 1UI32;
		UINT32 upper_source = upper_source_index < x.used ? x.data[upper_source_index] : 0UI32;
		UINT32 lower_source = lower_source_index < x.used ? x.data[lower_source_index] : 0UI32;
		UINT32 shifted_upper_source = (upper_source & lsb_mask) << msb_bits;
		UINT32 shifted_lower_source = (lower_source & msb_mask) >> lsb_bits;
		UINT32 combined_shifted_source = shifted_upper_source | shifted_lower_source;
		x.data[destination_index] = combined_shifted_source;
	}
	for (UINT32 destination_index = 0UI32; destination_index != unit_shift; ++destination_index)
		x.data[destination_index] = 0UI32;
	x.used = bit_shifts_into_next_unit ? max_destination_index + 1UI32 : max_destination_index;
	return TRUE;
}

static inline BOOL add(big_integer& x, UINT32 value)
{
	if (value == 0UI32)
		return TRUE;
	UINT32 carry = value;
	for (UINT32 i = 0UI32; i != x.used; ++i)
	{
		UINT64 result = (UINT64)(x.data[i]) + (UINT64)carry;
		x.data[i] = (UINT32)result;
		carry = (UINT32)(result >> 32UI64);
	}
	if (carry)
	{
		if (x.used < big_integer::element_count)
		{
			x.data[x.used] = carry;
			++x.used;
		}
		else
		{
			x = big_integer();
			return FALSE;
		}
	}
	return TRUE;
}

static inline UINT32 add_carry(UINT32& u1, UINT32 u2, UINT32 u_carry)
{
	UINT64 uu = (UINT64)u1 + (UINT64)u2 +(UINT64)u_carry;
	u1 = (UINT32)uu;
	return (UINT32)(uu >> 32UI64);
}

static inline UINT32 add_multiply_carry(UINT32& u_add, UINT32 u_mul_1, UINT32 u_mul_2, UINT32 u_carry)
{
	UINT64 uu_res = (UINT64)u_mul_1 * (UINT64)u_mul_2 + (UINT64)u_add + (UINT64)u_carry;
	u_add = (UINT32)uu_res;
	return  ((PUINT32)(&uu_res))[1];
}

static inline UINT32 multiply_core(PUINT32 multiplicand, UINT32 multiplicand_count, UINT32 multiplier)
{
	UINT32 carry = 0UI32;
	for (UINT32 i = 0UI32; i != multiplicand_count; ++i)
	{
		UINT64 result = (UINT64)multiplicand[i] * (UINT64)multiplier + (UINT64)carry;
		multiplicand[i] = (UINT32)result;
		carry = (UINT32)(result >> 32UI64);
	}
	return carry;
}

static inline BOOL multiply(big_integer& multiplicand, UINT32 multiplier)
{
	if (multiplier == 0UI32)
	{
		multiplicand = big_integer();
		return TRUE;
	}
	if (multiplier == 1UI32)
		return TRUE;
	if (!multiplicand.used)
		return TRUE;
	UINT32 carry = multiply_core(multiplicand.data, multiplicand.used, multiplier);
	if (carry)
	{
		if (multiplicand.used < big_integer::element_count)
		{
			multiplicand.data[multiplicand.used] = carry;
			++multiplicand.used;
		}
		else
		{
			multiplicand = big_integer();
			return FALSE;
		}
	}
	return TRUE;
}

static inline BOOL multiply(big_integer& multiplicand, const big_integer& multiplier)
{
	if (multiplier.used <= 1UI32)
		return multiply(multiplicand, multiplier.data[0]);
	if (multiplicand.used <= 1UI32)
	{
		UINT32 small_multiplier = multiplicand.data[0];
		multiplicand = multiplier;
		return multiply(multiplicand, small_multiplier);
	}
	BOOL multiplier_is_shorter = multiplier.used < multiplicand.used;
	const UINT32* rgu1 = multiplier_is_shorter ? multiplier.data : multiplicand.data;
	const UINT32* rgu2 = multiplier_is_shorter ? multiplicand.data : multiplier.data;
	UINT32 cu1 = multiplier_is_shorter ? multiplier.used : multiplicand.used;
	UINT32 cu2 = multiplier_is_shorter ? multiplicand.used : multiplier.used;
	big_integer result;
	for (UINT32 iu1 = 0UI32; iu1 != cu1; ++iu1)
	{
		UINT32 u_cur = rgu1[iu1];
		if (u_cur == 0UI32)
		{
			if (iu1 == result.used)
			{
				result.data[iu1] = 0UI32;
				result.used = iu1 + 1UI32;
			}
			continue;
		}
		UINT32 u_carry = 0UI32;
		UINT32 iu_res = iu1;
		for (UINT32 iu2 = 0UI32; iu2 != cu2 && iu_res != big_integer::element_count; ++iu2, ++iu_res)
		{
			if (iu_res == result.used)
			{
				result.data[iu_res] = 0UI32;
				result.used = iu_res + 1UI32;
			}
			u_carry = add_multiply_carry(result.data[iu_res], u_cur, rgu2[iu2], u_carry);
		}
		while (u_carry != 0UI32 && iu_res != big_integer::element_count)
		{
			if (iu_res == result.used)
			{
				result.data[iu_res] = 0UI32;
				result.used = iu_res + 1UI32;
			}
			u_carry = add_carry(result.data[iu_res++], 0UI32, u_carry);
		}
		if (iu_res == big_integer::element_count)
		{
			multiplicand = big_integer();
			return FALSE;
		}
	}
	multiplicand = result;
	return TRUE;
}

static inline BOOL multiply_by_power_of_ten(big_integer& x, UINT32 power)
{
	static UINT32 large_power_data[] =
	{
		0x540be400, 0x00000002, 0x63100000, 0x6bc75e2d, 0x00000005, 0x40000000, 0x4674edea, 0x9f2c9cd0,
		0x0000000c, 0xb9f56100, 0x5ca4bfab, 0x6329f1c3, 0x0000001d, 0xb5640000, 0xc40534fd, 0x926687d2,
		0x6c3b15f9, 0x00000044, 0x10000000, 0x946590d9, 0xd762422c, 0x9a224501, 0x4f272617, 0x0000009f,
		0x07950240, 0x245689c1, 0xc5faa71c, 0x73c86d67, 0xebad6ddc, 0x00000172, 0xcec10000, 0x63a22764,
		0xefa418ca, 0xcdd17b25, 0x6bdfef70, 0x9dea3e1f, 0x0000035f, 0xe4000000, 0xcdc3fe6e, 0x66bc0c6a,
		0x2e391f32, 0x5a450203, 0x71d2f825, 0xc3c24a56, 0x000007da, 0xa82e8f10, 0xaab24308, 0x8e211a7c,
		0xf38ace40, 0x84c4ce0b, 0x7ceb0b27, 0xad2594c3, 0x00001249, 0xdd1a4000, 0xcc9f54da, 0xdc5961bf,
		0xc75cabab, 0xf505440c, 0xd1bc1667, 0xfbb7af52, 0x608f8d29, 0x00002a94, 0x21000000, 0x17bb8a0c,
		0x56af8ea4, 0x06479fa9, 0x5d4bb236, 0x80dc5fe0, 0xf0feaa0a, 0xa88ed940, 0x6b1a80d0, 0x00006323,
		0x324c3864, 0x8357c796, 0xe44a42d5, 0xd9a92261, 0xbd3c103d, 0x91e5f372, 0xc0591574, 0xec1da60d,
		0x102ad96c, 0x0000e6d3, 0x1e851000, 0x6e4f615b, 0x187b2a69, 0x0450e21c, 0x2fdd342b, 0x635027ee,
		0xa6c97199, 0x8e4ae916, 0x17082e28, 0x1a496e6f, 0x0002196e, 0x32400000, 0x04ad4026, 0xf91e7250,
		0x2994d1d5, 0x665bcdbb, 0xa23b2e96, 0x65fa7ddb, 0x77de53ac, 0xb020a29b, 0xc6bff953, 0x4b9425ab,
		0x0004e34d, 0xfbc32d81, 0x5222d0f4, 0xb70f2850, 0x5713f2f3, 0xdc421413, 0xd6395d7d, 0xf8591999,
		0x0092381c, 0x86b314d6, 0x7aa577b9, 0x12b7fe61, 0x000b616a, 0x1d11e400, 0x56c3678d, 0x3a941f20,
		0x9b09368b, 0xbd706908, 0x207665be, 0x9b26c4eb, 0x1567e89d, 0x9d15096e, 0x7132f22b, 0xbe485113,
		0x45e5a2ce, 0x001a7f52, 0xbb100000, 0x02f79478, 0x8c1b74c0, 0xb0f05d00, 0xa9dbc675, 0xe2d9b914,
		0x650f72df, 0x77284b4c, 0x6df6e016, 0x514391c2, 0x2795c9cf, 0xd6e2ab55, 0x9ca8e627, 0x003db1a6,
		0x40000000, 0xf4ecd04a, 0x7f2388f0, 0x580a6dc5, 0x43bf046f, 0xf82d5dc3, 0xee110848, 0xfaa0591c,
		0xcdf4f028, 0x192ea53f, 0xbcd671a0, 0x7d694487, 0x10f96e01, 0x791a569d, 0x008fa475, 0xb9b2e100,
		0x8288753c, 0xcd3f1693, 0x89b43a6b, 0x089e87de, 0x684d4546, 0xfddba60c, 0xdf249391, 0x3068ec13,
		0x99b44427, 0xb68141ee, 0x5802cac3, 0xd96851f1, 0x7d7625a2, 0x014e718d, 0xfb640000, 0xf25a83e6,
		0x9457ad0f, 0x0080b511, 0x2029b566, 0xd7c5d2cf, 0xa53f6d7d, 0xcdb74d1c, 0xda9d70de, 0xb716413d,
		0x71d0ca4e, 0xd7e41398, 0x4f403a90, 0xf9ab3fe2, 0x264d776f, 0x030aafe6, 0x10000000, 0x09ab5531,
		0xa60c58d2, 0x566126cb, 0x6a1c8387, 0x7587f4c1, 0x2c44e876, 0x41a047cf, 0xc908059e, 0xa0ba063e,
		0xe7cfc8e8, 0xe1fac055, 0xef0144b2, 0x24207eb0, 0xd1722573, 0xe4b8f981, 0x071505ae, 0x7a3b6240,
		0xcea45d4f, 0x4fe24133, 0x210f6d6d, 0xe55633f2, 0x25c11356, 0x28ebd797, 0xd396eb84, 0x1e493b77,
		0x471f2dae, 0x96ad3820, 0x8afaced1, 0x4edecddb, 0x5568c086, 0xb2695da1, 0x24123c89, 0x107d4571,
		0x1c410000, 0x6e174a27, 0xec62ae57, 0xef2289aa, 0xb6a2fbdd, 0x17e1efe4, 0x3366bdf2, 0x37b48880,
		0xbfb82c3e, 0x19acde91, 0xd4f46408, 0x35ff6a4e, 0x67566a0e, 0x40dbb914, 0x782a3bca, 0x6b329b68,
		0xf5afc5d9, 0x266469bc, 0xe4000000, 0xfb805ff4, 0xed55d1af, 0x9b4a20a8, 0xab9757f8, 0x01aefe0a,
		0x4a2ca67b, 0x1ebf9569, 0xc7c41c29, 0xd8d5d2aa, 0xd136c776, 0x93da550c, 0x9ac79d90, 0x254bcba8,
		0x0df07618, 0xf7a88809, 0x3a1f1074, 0xe54811fc, 0x59638ead, 0x97cbe710, 0x26d769e8, 0xb4e4723e,
		0x5b90aa86, 0x9c333922, 0x4b7a0775, 0x2d47e991, 0x9a6ef977, 0x160b40e7, 0x0c92f8c4, 0xf25ff010,
		0x25c36c11, 0xc9f98b42, 0x730b919d, 0x05ff7caf, 0xb0432d85, 0x2d2b7569, 0xa657842c, 0xd01fef10,
		0xc77a4000, 0xe8b862e5, 0x10d8886a, 0xc8cd98e5, 0x108955c5, 0xd059b655, 0x58fbbed4, 0x03b88231,
		0x034c4519, 0x194dc939, 0x1fc500ac, 0x794cc0e2, 0x3bc980a1, 0xe9b12dd1, 0x5e6d22f8, 0x7b38899a,
		0xce7919d8, 0x78c67672, 0x79e5b99f, 0xe494034e, 0x00000001, 0xa1000000, 0x6c5cd4e9, 0x9be47d6f,
		0xf93bd9e7, 0x77626fa1, 0xc68b3451, 0xde2b59e8, 0xcf3cde58, 0x2246ff58, 0xa8577c15, 0x26e77559,
		0x17776753, 0xebe6b763, 0xe3fd0a5f, 0x33e83969, 0xa805a035, 0xf631b987, 0x211f0f43, 0xd85a43db,
		0xab1bf596, 0x683f19a2, 0x00000004, 0xbe7dfe64, 0x4bc9042f, 0xe1f5edb0, 0x8fa14eda, 0xe409db73,
		0x674fee9c, 0xa9159f0d, 0xf6b5b5d6, 0x7338960e, 0xeb49c291, 0x5f2b97cc, 0x0f383f95, 0x2091b3f6,
		0xd1783714, 0xc1d142df, 0x153e22de, 0x8aafdf57, 0x77f5e55f, 0xa3e7ca8b, 0x032f525b, 0x42e74f3d,
		0x0000000a, 0xf4dd1000, 0x5d450952, 0xaeb442e1, 0xa3b3342e, 0x3fcda36f, 0xb4287a6e, 0x4bc177f7,
		0x67d2c8d0, 0xaea8f8e0, 0xadc93b67, 0x6cc856b3, 0x959d9d0b, 0x5b48c100, 0x4abe8a3d, 0x52d936f4,
		0x71dbe84d, 0xf91c21c5, 0x4a458109, 0xd7aad86a, 0x08e14c7c, 0x759ba59c, 0xe43c8800, 0x00000017,
		0x92400000, 0x04f110d4, 0x186472be, 0x8736c10c, 0x1478abfb, 0xfc51af29, 0x25eb9739, 0x4c2b3015,
		0xa1030e0b, 0x28fe3c3b, 0x7788fcba, 0xb89e4358, 0x733de4a4, 0x7c46f2c2, 0x8f746298, 0xdb19210f,
		0x2ea3b6ae, 0xaa5014b2, 0xea39ab8d, 0x97963442, 0x01dfdfa9, 0xd2f3d3fe, 0xa0790280, 0x00000037,
		0x509c9b01, 0xc7dcadf1, 0x383dad2c, 0x73c64d37, 0xea6d67d0, 0x519ba806, 0xc403f2f8, 0xa052e1a2,
		0xd710233a, 0x448573a9, 0xcf12d9ba, 0x70871803, 0x52dc3a9b, 0xe5b252e8, 0x0717fb4e, 0xbe4da62f,
		0x0aabd7e1, 0x8c62ed4f, 0xceb9ec7b, 0xd4664021, 0xa1158300, 0xcce375e6, 0x842f29f2, 0x00000081,
		0x7717e400, 0xd3f5fb64, 0xa0763d71, 0x7d142fe9, 0x33f44c66, 0xf3b8f12e, 0x130f0d8e, 0x734c9469,
		0x60260fa8, 0x3c011340, 0xcc71880a, 0x37a52d21, 0x8adac9ef, 0x42bb31b4, 0xd6f94c41, 0xc88b056c,
		0xe20501b8, 0x5297ed7c, 0x62c361c4, 0x87dad8aa, 0xb833eade, 0x94f06861, 0x13cc9abd, 0x8dc1d56a,
		0x0000012d, 0x13100000, 0xc67a36e8, 0xf416299e, 0xf3493f0a, 0x77a5a6cf, 0xa4be23a3, 0xcca25b82,
		0x3510722f, 0xbe9d447f, 0xa8c213b8, 0xc94c324e, 0xbc9e33ad, 0x76acfeba, 0x2e4c2132, 0x3e13cd32,
		0x70fe91b4, 0xbb5cd936, 0x42149785, 0x46cc1afd, 0xe638ddf8, 0x690787d2, 0x1a02d117, 0x3eb5f1fe,
		0xc3b9abae, 0x1c08ee6f, 0x000002be, 0x40000000, 0x8140c2aa, 0x2cf877d9, 0x71e1d73d, 0xd5e72f98,
		0x72516309, 0xafa819dd, 0xd62a5a46, 0x2a02dcce, 0xce46ddfe, 0x2713248d, 0xb723d2ad, 0xc404bb19,
		0xb706cc2b, 0x47b1ebca, 0x9d094bdc, 0xc5dc02ca, 0x31e6518e, 0x8ec35680, 0x342f58a8, 0x8b041e42,
		0xfebfe514, 0x05fffc13, 0x6763790f, 0x66d536fd, 0xb9e15076, 0x00000662, 0x67b06100, 0xd2010a1a,
		0xd005e1c0, 0xdb12733b, 0xa39f2e3f, 0x61b29de2, 0x2a63dce2, 0x942604bc, 0x6170d59b, 0xc2e32596,
		0x140b75b9, 0x1f1d2c21, 0xb8136a60, 0x89d23ba2, 0x60f17d73, 0xc6cad7df, 0x0669df2b, 0x24b88737,
		0x669306ed, 0x19496eeb, 0x938ddb6f, 0x5e748275, 0xc56e9a36, 0x3690b731, 0xc82842c5, 0x24ae798e,
		0x00000ede, 0x41640000, 0xd5889ac1, 0xd9432c99, 0xa280e71a, 0x6bf63d2e, 0x8249793d, 0x79e7a943,
		0x22fde64a, 0xe0d6709a, 0x05cacfef, 0xbd8da4d7, 0xe364006c, 0xa54edcb3, 0xa1a8086e, 0x748f459e,
		0xfc8e54c8, 0xcc74c657, 0x42b8c3d4, 0x57d9636e, 0x35b55bcc, 0x6c13fee9, 0x1ac45161, 0xb595badb,
		0xa1f14e9d, 0xdcf9e750, 0x07637f71, 0xde2f9f2b, 0x0000229d, 0x10000000, 0x3c5ebd89, 0xe3773756,
		0x3dcba338, 0x81d29e4f, 0xa4f79e2c, 0xc3f9c774, 0x6a1ce797, 0xac5fe438, 0x07f38b9c, 0xd588ecfa,
		0x3e5ac1ac, 0x85afccce, 0x9d1f3f70, 0xe82d6dd3, 0x177d180c, 0x5e69946f, 0x648e2ce1, 0x95a13948,
		0x340fe011, 0xb4173c58, 0x2748f694, 0x7c2657bd, 0x758bda2e, 0x3b8090a0, 0x2ddbb613, 0x6dcf4890,
		0x24e4047e, 0x00005099,
	};
	struct unpack_index
	{
		UINT16 _offset; // The offset of this power's initial byte in the array
		UINT8 _zeroes; // The number of omitted leading zero elements
		UINT8 _size; // The number of elements present for this power
	};
	static const unpack_index large_power_indices[] =
	{
		{ 0UI16, 0UI8, 2UI8 },{ 2UI16, 0UI8, 3UI8 },{ 5UI16, 0UI8, 4UI8 },{ 9UI16, 1UI8, 4UI8 },
		{ 130UI16, 1UI8, 5UI8 },{ 18UI16, 1UI8, 6UI8 },{ 24UI16, 2UI8, 6UI8 },{ 30UI16, 2UI8, 7UI8 },
		{ 370UI16, 2UI8, 8UI8 },{ 45UI16, 3UI8, 8UI8 },{ 53UI16, 3UI8, 9UI8 },{ 62UI16, 3UI8, 10UI8 },
		{ 720UI16, 4UI8, 10UI8 },{ 82UI16, 4UI8, 11UI8 },{ 93UI16, 4UI8, 12UI8 },{ 105UI16, 5UI8, 12UI8 },
		{ 1170UI16, 5UI8, 13UI8 },{ 130UI16, 5UI8, 14UI8 },{ 144UI16, 5UI8, 15UI8 },{ 159UI16, 6UI8, 15UI8 },
		{ 1740UI16, 6UI8, 16UI8 },{ 190UI16, 6UI8, 17UI8 },{ 207UI16, 7UI8, 17UI8 },{ 224UI16, 7UI8, 18UI8 },
		{ 2420UI16, 7UI8, 19UI8 },{ 261UI16, 8UI8, 19UI8 },{ 280UI16, 8UI8, 21UI8 },{ 301UI16, 8UI8, 22UI8 },
		{ 3230UI16, 9UI8, 22UI8 },{ 345UI16, 9UI8, 23UI8 },{ 368UI16, 9UI8, 24UI8 },{ 392UI16, 10UI8, 24UI8 },
		{ 4160UI16, 10UI8, 25UI8 },{ 441UI16, 10UI8, 26UI8 },{ 467UI16, 10UI8, 27UI8 },{ 494UI16, 11UI8, 27UI8 },
		{ 5210UI16, 11UI8, 28UI8 },{ 549UI16, 11UI8, 29UI8 },
	};
	UINT32 large_power = power / 10UI32;
	while (large_power)
	{
		UINT32 current_power = large_power > _countof(large_power_indices) ? _countof(large_power_indices) : large_power;
		const unpack_index& index = large_power_indices[current_power - 1UI32];
		big_integer multiplier;
		multiplier.used = index._size + index._zeroes;
		Memsetzero(multiplier.data, (size_t)index._zeroes * sizeof(UINT32));
		Memcpy(multiplier.data + index._zeroes, large_power_data + index._offset, (size_t)index._size * sizeof(UINT32));
		if (!multiply(x, multiplier))
		{
			x = big_integer();
			return FALSE;
		}
		large_power -= current_power;
	}
	static UINT32 small_powers_of_ten[9] =
	{
		10UI32,
		100UI32,
		1000UI32,
		1000UI32 * 10UI32,
		1000UI32 * 100UI32,
		1000UI32 * 1000UI32,
		1000UI32 * 1000UI32 * 10UI32,
		1000UI32 * 1000UI32 * 100UI32,
		1000UI32 * 1000UI32 * 1000UI32
	};
	UINT32 small_power = power % 10UI32;
	if (small_power)
	{
		if (!multiply(x, small_powers_of_ten[small_power - 1UI32]))
			return FALSE;
	}
	return TRUE;
}

static inline UINT32 count_sequential_high_zeroes(UINT32 u)
{
	UINT32 result;
	return _BitScanReverse((LPDWORD)&result, u) ? 31UI32 - result : 32UI32;
}

#ifdef _M_IX86
static inline UINT64 multiply_64_32(UINT64 multiplicand, UINT32 multiplier)
{
	__asm
	{
		mov eax, dword ptr[multiplicand + 4UI64]
		mul multiplier
		mov ecx, eax
		mov eax, dword ptr[multiplicand]
		mul multiplier
		add edx, ecx
	}
}
#else
static inline UINT64 multiply_64_32(UINT64 multiplicand, UINT32 multiplier)
{
	return multiplicand * multiplier;
}
#endif

static UINT64 divide(big_integer& numerator, const big_integer& denominator)
{
	if (!numerator.used)
		return 0UI64;
	if (!denominator.used)
	{
		//_ASSERTE(("Division by zero", FALSE));
		return 0UI64;
	}
	UINT32 max_numerator_element_index = numerator.used - 1UI32;
	UINT32 max_denominator_element_index = denominator.used - 1UI32;
	if (!max_denominator_element_index)
	{
		UINT32 small_denominator = denominator.data[0];
		if (small_denominator == 1UI32)
		{
			UINT32 quotient = numerator.data[0];
			numerator = big_integer();
			return quotient;
		}
		if (!max_numerator_element_index)
		{
			UINT32 small_numerator = numerator.data[0];
			numerator = big_integer();
			numerator.data[0] = small_numerator % small_denominator;
			numerator.used = numerator.data[0] ? 1UI32 : 0UI32;
			return small_numerator / small_denominator;
		}
		UINT64 quotient = 0UI64;
		UINT64 uu = 0UI64;
		for (UINT32 iv = max_numerator_element_index; iv != (UINT32)(-1); --iv)
		{
			uu = (uu << 32UI64) | numerator.data[iv];
			quotient = (quotient << 32UI64) + (UINT32)(uu / small_denominator);
			uu %= small_denominator;
		}
		numerator = big_integer();
		numerator.data[1] = (UINT32)(uu >> 32UI64);
		numerator.data[0] = (UINT32)uu;
		numerator.used = numerator.data[1]? 2UI32 : 1UI32;
		return quotient;
	}

	if (max_denominator_element_index > max_numerator_element_index)
		return 0UI64;
	UINT32 cu_den = max_denominator_element_index + 1UI32;
	INT32 cu_diff = max_numerator_element_index - max_denominator_element_index;
	INT32 cu_quo = cu_diff;
	for (INT32 iu = max_numerator_element_index; ; --iu)
	{
		if (iu < cu_diff)
		{
			++cu_quo;
			break;
		}
		if (denominator.data[iu - cu_diff] != numerator.data[iu])
		{
			if (denominator.data[iu - cu_diff] < numerator.data[iu])
				++cu_quo;
			break;
		}
	}
	if (!cu_quo)
		return 0UI64;
	UINT32 u_den = denominator.data[cu_den - 1UI32];
	UINT32 u_den_next = denominator.data[cu_den - 2UI32];
	UINT32 cbit_shift_left = count_sequential_high_zeroes(u_den);
	UINT32 cbit_shift_right = 32UI32 - cbit_shift_left;
	if (cbit_shift_left)
	{
		u_den = (u_den << cbit_shift_left) | (u_den_next >> cbit_shift_right);
		u_den_next <<= cbit_shift_left;
		if (cu_den > 2UI32)
			u_den_next |= denominator.data[cu_den - 3UI32] >> cbit_shift_right;
	}
	UINT64 quotient = 0UI64;
	for (INT32 iu = cu_quo; --iu >= 0I32; )
	{
		UINT32 u_num_hi = ((UINT32)iu + cu_den <= max_numerator_element_index) ? numerator.data[(UINT32)iu + cu_den] : 0UI32;
		UINT64 uu_num = numerator.data[(UINT32)iu + cu_den - 1UI32];
		((PUINT32)(&uu_num))[1] = u_num_hi;
		UINT32 u_num_next = numerator.data[(UINT32)iu + cu_den - 2UI32];
		if (cbit_shift_left)
		{
			uu_num = (uu_num << cbit_shift_left) | (u_num_next >> cbit_shift_right);
			u_num_next <<= cbit_shift_left;
			if ((UINT32)iu + cu_den >= 3UI32)
				u_num_next |= numerator.data[(UINT32)iu + cu_den - 3UI32] >> cbit_shift_right;
		}
		UINT64 uu_quo = uu_num / (UINT64)u_den;
		UINT64 uu_rem = (UINT64)(UINT32)(uu_num % (UINT64)u_den);
		if (uu_quo > 0xFFFFFFFFUI64)
		{
			uu_rem += (UINT64)u_den * (uu_quo - 0xFFFFFFFFUI64);
			uu_quo = 0xFFFFFFFFUI64;
		}
		while (uu_rem <= 0xFFFFFFFFUI64 && uu_quo * (UINT64)u_den_next > ((uu_rem << 32UI64) | (UINT64)u_num_next))
		{
			--uu_quo;
			uu_rem += (UINT64)u_den;
		}
		if (uu_quo)
		{
			UINT64 uu_borrow = 0UI64;
			for (UINT32 iu2 = 0UI32; iu2 < cu_den; ++iu2)
			{
				uu_borrow += multiply_64_32(uu_quo, denominator.data[iu2]);
				UINT32 u_sub = (UINT32)uu_borrow;
				uu_borrow >>= 32UI64;
				if (numerator.data[(UINT32)iu + iu2] < u_sub)
					++uu_borrow;
				numerator.data[(UINT32)iu + iu2] -= u_sub;
			}
			if (u_num_hi < uu_borrow)
			{
				UINT32 u_carry = 0UI32;
				for (UINT32 iu2 = 0UI32; iu2 < cu_den; ++iu2)
				{
					UINT64 sum = (UINT64)(numerator.data[(UINT32)iu + iu2]) + (UINT64)(denominator.data[iu2]) + (UINT64)u_carry;
					numerator.data[(UINT32)iu + iu2] = (UINT32)sum;
					u_carry = sum >> 32UI64;
				}
				--uu_quo;
			}
			max_numerator_element_index = (UINT32)iu + cu_den - 1UI32;
		}
		quotient = (quotient << 32UI64) + (UINT64)(UINT32)uu_quo;
	}
	numerator.used = max_numerator_element_index + 1UI32;
	while (numerator.used && !numerator.data[numerator.used - 1UI32])
		--numerator.used;
	return quotient;
}


LPTSTR Double2String(LPTSTR lpszBuffer, double value, UINT32 FractionalDigitCount)
{
	double_type& components = (double_type&)value;
	if (!components._exponent && !components._mantissa)
	{
		lpszBuffer[0] = TEXT('0');
		if (FractionalDigitCount)
		{
			lpszBuffer[1] = TEXT('.');
			LPTSTR buffer_it = lpszBuffer + 2;
			while (FractionalDigitCount--)
				*buffer_it++ = TEXT('0');
			*buffer_it = TEXT('\0');
		}
		else
			lpszBuffer[1] = TEXT('\0');
		return lpszBuffer;
	}
	if (components._exponent == (1I32 << exponent_bits) - 1I32)
	{
		if (!components._mantissa)
		{
			lpszBuffer[0] = TEXT('1');
			lpszBuffer[1] = TEXT('#');
			lpszBuffer[2] = TEXT('I');
			lpszBuffer[3] = TEXT('N');
			lpszBuffer[4] = TEXT('F');
			lpszBuffer[5] = TEXT('\0');
		}
		else if (components._sign == 1UI64 && components._mantissa == special_nan_mantissa_mask)
		{
			lpszBuffer[0] = TEXT('1');
			lpszBuffer[1] = TEXT('#');
			lpszBuffer[2] = TEXT('I');
			lpszBuffer[3] = TEXT('N');
			lpszBuffer[4] = TEXT('D');
			lpszBuffer[5] = TEXT('\0');
		}
		else if (components._mantissa & special_nan_mantissa_mask)
		{
			lpszBuffer[0] = TEXT('1');
			lpszBuffer[1] = TEXT('#');
			lpszBuffer[2] = TEXT('Q');
			lpszBuffer[3] = TEXT('N');
			lpszBuffer[4] = TEXT('A');
			lpszBuffer[5] = TEXT('N');
			lpszBuffer[6] = TEXT('\0');
		}
		else
		{
			lpszBuffer[0] = TEXT('1');
			lpszBuffer[1] = TEXT('#');
			lpszBuffer[2] = TEXT('S');
			lpszBuffer[3] = TEXT('N');
			lpszBuffer[4] = TEXT('A');
			lpszBuffer[5] = TEXT('N');
			lpszBuffer[6] = TEXT('\0');
		}
		return lpszBuffer;
	}
	BOOL bPositive = !components._sign;
	components._sign = 0UI64;
	UINT64 mantissa_adjustment;
	INT32 exponent_adjustment;
	if (components._exponent)
	{
		mantissa_adjustment = 1UI64 << (mantissa_bits - 1UI64);
		exponent_adjustment = 1UI32;
	}
	else
	{
		mantissa_adjustment = 0UI64;
		exponent_adjustment = 2UI32;
	}
	UINT64 f = components._mantissa + mantissa_adjustment;
	INT32 e = (INT32)components._exponent - exponent_bias - mantissa_bits + exponent_adjustment;
	INT32 k = (INT32)Ceil(Log(value) / LN10);
	big_integer r = make_big_integer(f);
	big_integer s;
	if (e >= 0I32)
	{
		if (r != make_big_integer_power_of_two(mantissa_bits - 1I32))
		{
			shift_left(r, (UINT32)(e + 1I32));
			s = make_big_integer(2UI64);
		}
		else
		{
			shift_left(r, (UINT32)(e + 2I32));
			s = make_big_integer(4UI64);
		}
	}
	else
	{
		if (e == minimum_binary_exponent || r != make_big_integer_power_of_two(mantissa_bits - 1))
		{
			shift_left(r, 1UI32);
			s = make_big_integer_power_of_two((UINT32)(-e + 1I32));
		}
		else
		{
			shift_left(r, 2UI32);
			s = make_big_integer_power_of_two((UINT32)(-e + 2I32));
		}
	}
	if (k >= 0I32)
		multiply_by_power_of_ten(s, (UINT32)k);
	else
		multiply_by_power_of_ten(r, (UINT32)-k);
	LPTSTR mantissa_it = lpszBuffer + 3;
	multiply(r, 10UI32);
	UINT32 initial_digit = (UINT32)divide(r, s);
	TCHAR ch;
	if (initial_digit == 10UI32)
	{
		++k;
		ch = TEXT('1');
		multiply(s, 10UI32);
	}
	else if (!initial_digit)
	{
		--k;
		ch = TEXT('\0');
	}
	else
		ch = (TCHAR)(TEXT('0') + initial_digit);
	while (k <= 0I32)
	{
		++k;
		*mantissa_it++ = TEXT('0');
	}
	if (ch != TEXT('\0'))
		*mantissa_it++ = ch;
	LPTSTR mantissa_last = lpszBuffer + 3 + k + FractionalDigitCount + 1;
	if (mantissa_it < mantissa_last)
	{
		for (;;)
		{
			const UINT32 digits_per_iteration = 9UI32;
			const UINT32 digits_per_iteration_multiplier = 1000UI32 * 1000UI32 * 1000UI32;
			multiply(r, digits_per_iteration_multiplier);
			UINT32 quotient = (UINT32)divide(r, s);
			#pragma warning(suppress: 6293)
			for (UINT32 i = digits_per_iteration - 1UI32; i != (UINT32)(-1); --i)
			{
				TCHAR d = (TCHAR)(TEXT('0') + quotient % 10UI32);
				quotient /= 10UI32;
				if ((UINT32)(mantissa_last - mantissa_it) < i)
					continue;
				mantissa_it[i] = d;
			}
			mantissa_it += __MIN(digits_per_iteration, (UINT32)(mantissa_last - mantissa_it));
			if (mantissa_it >= mantissa_last)
				break;
			if (is_zero(r))
			{
				while (mantissa_it < mantissa_last)
					*mantissa_it++ = TEXT('0');
				break;
			}
		}
	}
	else if (is_zero(r))
	{
		while (mantissa_it < mantissa_last)
			*mantissa_it++ = TEXT('0');
	}
	lpszBuffer[2] = TEXT('0');
	LPTSTR buffer_it = lpszBuffer + 3;
	if (*--mantissa_last >= TEXT('5'))
	{
		LPTSTR it = mantissa_last - 1;
		while (*it == TEXT('9'))
		{
			*it-- = TEXT('0');
		}
		*it += 1;
	}
	if (lpszBuffer[2] == TEXT('1'))
	{
		++k;
		--buffer_it;
	}
	if (FractionalDigitCount)
	{
		--buffer_it;
		for (INT32 i = 0I32; i != k; ++i)
			buffer_it[i] = buffer_it[i + 1I32];
		buffer_it[k] = TEXT('.');
	}
	if (!bPositive)
		*--buffer_it = TEXT('-');
	*mantissa_last = TEXT('\0');
	return buffer_it;
}


static inline UINT32 parse_digit(TCHAR ch)
{
	if (ch >= TEXT('0') && ch <= TEXT('9'))
		return (UINT32)(ch - TEXT('0'));
#ifndef SLD_NO_HEX
	if (ch >= TEXT('a') && ch <= TEXT('z'))
		return (UINT32)(ch - TEXT('a')) + 10UI32;
	if (ch >= TEXT('A') && ch <= TEXT('Z'))
		return (UINT32)(ch - TEXT('A')) + 10UI32;
#endif
	return (UINT32)(-1);
}

static inline VOID accumulate_decimal_digits_into_big_integer(PUINT8 first_digit, PUINT8 last_digit, big_integer& result)
{
	UINT32 accumulator = 0UI32;
	UINT32 accumulator_count = 0UI32;
	for (PUINT8 it = first_digit; it != last_digit; ++it)
	{
		if (accumulator_count == 9UI32)
		{
			multiply(result, 1000UI32 * 1000UI32 * 1000UI32);
			add(result, accumulator);
			accumulator = 0UI32;
			accumulator_count = 0UI32;
		}
		accumulator *= 10UI32;
		accumulator += (UINT32)*it;
		++accumulator_count;
	}
	if (accumulator_count)
	{
		multiply_by_power_of_ten(result, accumulator_count);
		add(result, accumulator);
	}
}

static inline VOID right_shift_with_rounding(UINT64& value, UINT64 shift, BOOL bZeroTail)
{
	if (shift >= sizeof(UINT64) * (UINT64)CHAR_BIT)
	{
		value = 0UI64;
		return;
	}
	const BOOL lsb_bit = (value & (1UI64 << shift)) != 0UI64;
	const BOOL round_bit = (value & ((1UI64 << (shift - 1UI64)))) != 0UI64;
	const BOOL tail_bits = !bZeroTail || (value & ((1UI64 << (shift - 1UI64)) - 1UI64)) != 0UI64;
	value >>= shift;
	if (!round_bit && !tail_bits)
		return;
	if (round_bit && (tail_bits || lsb_bit))
		++value;
}

SLD_STATUS String2Double(LPCTSTR lpszString, double& result)
{
	double_type& components = (double_type&)result;
	while (*lpszString == TEXT(' ') || *lpszString == TEXT('\t') || *lpszString == TEXT('\r') || *lpszString == TEXT('\n') || *lpszString == TEXT('\v') || *lpszString == TEXT('\f'))
		++lpszString;
	if (*lpszString == TEXT('-'))
	{
		components._sign = 1UI64;
		++lpszString;
	}
	else
	{
		components._sign = 0UI64;
		if (*lpszString == TEXT('+'))
			++lpszString;
	}
	if (*lpszString == TEXT('I') || *lpszString == TEXT('i'))
	{
		++lpszString;
		if (*lpszString == TEXT('N') || *lpszString == TEXT('n'))
		{
			++lpszString;
			if (*lpszString == TEXT('F') || *lpszString == TEXT('f'))
			{
				components._exponent = exponent_mask;
				components._mantissa = 0UI64;
				return SLD_OK;
			}
		}
		goto NoDigits;
	}
	if (*lpszString == TEXT('N') || *lpszString == TEXT('n'))
	{
		++lpszString;
		if (*lpszString == TEXT('A') || *lpszString == TEXT('a'))
		{
			++lpszString;
			if (*lpszString == TEXT('N') || *lpszString == TEXT('n'))
			{
				components._exponent = exponent_mask;
				++lpszString;
				if (*lpszString == TEXT('('))
				{
					++lpszString;
					if (*lpszString == TEXT('S') || *lpszString == TEXT('s'))
					{
						++lpszString;
						if (*lpszString == TEXT('N') || *lpszString == TEXT('n'))
						{
							++lpszString;
							if (*lpszString == TEXT('A') || *lpszString == TEXT('a'))
							{
								++lpszString;
								if (*lpszString == TEXT('N') || *lpszString == TEXT('n'))
								{
									++lpszString;
									if (*lpszString == TEXT(')'))
									{
										components._mantissa = 1UI64;
										return SLD_OK;
									}
								}
							}
						}
					}
					else if (*lpszString == TEXT('I') || *lpszString == TEXT('i'))
					{
						++lpszString;
						if (*lpszString == TEXT('N') || *lpszString == TEXT('n'))
						{
							++lpszString;
							if (*lpszString == TEXT('D') || *lpszString == TEXT('d'))
							{
								++lpszString;
								if (*lpszString == TEXT(')'))
								{
									components._sign = 1UI64;
									components._mantissa = special_nan_mantissa_mask;
									return SLD_OK;
								}
							}
						}
					}
				}
				components._mantissa = denormal_mantissa_mask;
				return SLD_OK;
			}
		}
		goto NoDigits;
	}
#ifndef SLD_NO_HEX
	UINT32 max_digit_value;
	if (*lpszString == TEXT('0') && (*(lpszString + 1) == TEXT('x') || *(lpszString + 1) == TEXT('X')))
	{
		max_digit_value = 0xFUI64;
		lpszString += 2;
	}
	else
		max_digit_value = 9UI64;
#else
	UINT32 max_digit_value = 9UI64;
#endif
	INT32 exponent_adjustment = 0I32;
	BOOL bDigits = FALSE;
	while (*lpszString == TEXT('0'))
	{
		bDigits = TRUE;
		++lpszString;
	}
	UINT8 mantissa_buffer[768];
	PUINT8 mantissa_it = mantissa_buffer;
	PUINT8 mantissa_last = mantissa_buffer + _countof(mantissa_buffer);
	for (; ; ++lpszString)
	{
		UINT32 digit_value = parse_digit(*lpszString);
		if (digit_value > max_digit_value)
			break;
		bDigits = TRUE;
		if (mantissa_it != mantissa_last)
			*mantissa_it++ = (UINT8)digit_value;
		++exponent_adjustment;
	}
	if (*lpszString == TEXT('.'))
	{
		++lpszString;
		if (mantissa_it == mantissa_buffer)
		{
			while (*lpszString == TEXT('0'))
			{
				bDigits = TRUE;
				--exponent_adjustment;
				++lpszString;
			}
		}
		for (; ; ++lpszString)
		{
			UINT32 digit_value = parse_digit(*lpszString);
			if (digit_value > max_digit_value)
				break;
			bDigits = TRUE;
			if (mantissa_it != mantissa_last)
				*mantissa_it++ = (UINT8)digit_value;
		}
	}
	if (!bDigits)
	{
#ifndef SLD_NO_HEX
		if (max_digit_value == 0xFUI64)
		{
			components._exponent = 0UI64;
			components._mantissa = 0UI64;
			return SLD_OK;
		}
#endif
		goto NoDigits;
	}
	BOOL bExponent;
	switch (*lpszString)
	{
	case 'e':
	case 'E':
#ifndef SLD_NO_HEX
		bExponent = max_digit_value != 0xFUI64;
#else
		bExponent = TRUE;
#endif
		break;
#ifndef SLD_NO_HEX
	case 'p':
	case 'P':
		bExponent = max_digit_value == 0xFUI64;
		break;
#endif
	default:
		bExponent = FALSE;
		break;
	}
	INT32 exponent = 0I32;
	if (bExponent)
	{
		++lpszString;
		BOOL bExponentNegative;
		if (*lpszString == TEXT('-'))
		{
			bExponentNegative = TRUE;
			++lpszString;
		}
		else
		{
			bExponentNegative = FALSE;
			if (*lpszString == TEXT('+'))
				++lpszString;
		}
		while (*lpszString == TEXT('0'))
			++lpszString;
		for (; ; ++lpszString)
		{
			UINT32 digit_value = parse_digit(*lpszString);
			if (digit_value >= 10UI32)
				break;
			exponent = exponent * 10I32 + (INT32)digit_value;
			if (exponent > maximum_temporary_decimal_exponent)
			{
				exponent = maximum_temporary_decimal_exponent + 1I32;
				break;
			}
		}
		if (bExponentNegative)
			exponent = -exponent;
	}
	while (mantissa_it != mantissa_buffer && *(mantissa_it - 1) == 0UI8)
		--mantissa_it;
	if (mantissa_it == mantissa_buffer)
	{
		components._exponent = 0UI64;
		components._mantissa = 0UI64;
		return SLD_OK;
	}
	if (exponent > maximum_temporary_decimal_exponent)
	{
		components._exponent = exponent_mask;
		components._mantissa = 0UI64;
		return SLD_OVERFLOW;
	}
	if (exponent < minimum_temporary_decimal_exponent)
	{
		components._exponent = 0UI64;
		components._mantissa = 0UI64;
		return SLD_UNDERFLOW;
	}
#ifndef SLD_NO_HEX
	exponent += exponent_adjustment * (max_digit_value == 0xFUI64 ? 4I32 : 1I32);
#else
	exponent += exponent_adjustment;
#endif
	if (exponent > maximum_temporary_decimal_exponent)
	{
		components._exponent = exponent_mask;
		components._mantissa = 0UI64;
		return SLD_OVERFLOW;
	}
	if (exponent < minimum_temporary_decimal_exponent)
	{
		components._exponent = 0UI64;
		components._mantissa = 0UI64;
		return SLD_UNDERFLOW;
	}
	UINT64 mantissa_value;
	INT32 exponent_value;
	BOOL bZeroTail;
#ifndef SLD_NO_HEX
	if (max_digit_value == 0xFUI64)
	{
		mantissa_value = 0UI64;
		exponent_value = exponent + mantissa_bits - 1I32;
		mantissa_last = mantissa_it;
		mantissa_it = mantissa_buffer;
		while (mantissa_it != mantissa_last && mantissa_value <= normal_mantissa_mask)
		{
			mantissa_value = mantissa_value * 16UI64 + *mantissa_it++;
			exponent_value -= 4I32;
		}
		bZeroTail = TRUE;
		while (mantissa_it != mantissa_last && bZeroTail)
			bZeroTail = bZeroTail && *mantissa_it++ == 0UI8;
		goto AssembleDouble;;
	}
#endif
	{
		const UINT32 required_bits_of_precision = (UINT32)mantissa_bits + 1UI32;
		const UINT32 positive_exponent = (UINT32)__MAX(0I32, exponent);
		const UINT32 mantissa_count = (UINT32)(mantissa_it - mantissa_buffer);
		const UINT32 integer_digits_present = __MIN(positive_exponent, mantissa_count);
		const UINT32 integer_digits_missing = positive_exponent - integer_digits_present;
		const PUINT8 integer_last = mantissa_buffer + integer_digits_present;
		const PUINT8 fractional_last = mantissa_buffer + mantissa_count;
		const UINT32 fractional_digits_present = (UINT32)(fractional_last - integer_last);
		{
			big_integer integer_value;
			accumulate_decimal_digits_into_big_integer(mantissa_buffer, integer_last, integer_value);
			if (integer_digits_missing && !multiply_by_power_of_ten(integer_value, integer_digits_missing))
			{
				components._exponent = exponent_mask;
				components._mantissa = 0UI64;
				return SLD_OVERFLOW;
			}
			const UINT32 integer_bits_of_precision = bit_scan_reverse(integer_value);
			if (integer_bits_of_precision >= required_bits_of_precision || !fractional_digits_present)
				goto BigIntegerToDouble;
			{
				big_integer fractional_numerator;
				accumulate_decimal_digits_into_big_integer(integer_last, fractional_last, fractional_numerator);
				big_integer fractional_denominator = make_big_integer(1UI64);
				if (!multiply_by_power_of_ten(fractional_denominator, exponent < 0I32 ? fractional_digits_present + (UINT32)(-exponent) : fractional_digits_present))
				{
					components._exponent = 0UI64;
					components._mantissa = 0UI64;
					return SLD_UNDERFLOW;
				}
				const UINT32 fractional_numerator_bits = bit_scan_reverse(fractional_numerator);
				const UINT32 fractional_denominator_bits = bit_scan_reverse(fractional_denominator);
				const UINT32 fractional_shift = fractional_denominator_bits > fractional_numerator_bits ? fractional_denominator_bits - fractional_numerator_bits : 0UI32;
				if (fractional_shift)
					shift_left(fractional_numerator, fractional_shift);
				const UINT32 required_fractional_bits_of_precision = required_bits_of_precision - integer_bits_of_precision;
				UINT32 remaining_bits_of_precision_required = required_fractional_bits_of_precision;
				if (integer_bits_of_precision)
				{
					if (fractional_shift > remaining_bits_of_precision_required)
						goto BigIntegerToDouble;
					remaining_bits_of_precision_required -= fractional_shift;
				}
				const UINT32 fractional_exponent = fractional_numerator < fractional_denominator ? fractional_shift + 1U : fractional_shift;
				shift_left(fractional_numerator, remaining_bits_of_precision_required);
				UINT64 fractional_mantissa = divide(fractional_numerator, fractional_denominator);
				bZeroTail = !fractional_numerator.used;
				const UINT32 fractional_mantissa_bits = bit_scan_reverse(fractional_mantissa);
				if (fractional_mantissa_bits > required_fractional_bits_of_precision)
				{
					const UINT32 shift = fractional_mantissa_bits - required_fractional_bits_of_precision;
					bZeroTail = bZeroTail && (fractional_mantissa & ((1UI64 << (UINT64)shift) - 1UI64)) == 0UI64;
					fractional_mantissa >>= (UINT64)shift;
				}
				const UINT32 integer_mantissa_low = integer_value.used ? integer_value.data[0] : 0UI32;
				const UINT32 integer_mantissa_high = integer_value.used > 1UI32 ? integer_value.data[1] : 0UI32;
				const UINT64 integer_mantissa = (UINT64)integer_mantissa_low + ((UINT64)integer_mantissa_high << 32UI64);
				mantissa_value = (integer_mantissa << (UINT64)required_fractional_bits_of_precision) + fractional_mantissa;
				exponent_value = integer_bits_of_precision ? (INT32)integer_bits_of_precision - 2I32 : -(INT32)fractional_exponent - 1I32;
				goto AssembleDouble;
			}

		BigIntegerToDouble:
			const INT32 base_exponent = mantissa_bits - 1I32;
			if (integer_bits_of_precision <= 64UI32)
			{
				const UINT32 mantissa_low = integer_value.used ? integer_value.data[0] : 0UI32;
				const UINT32 mantissa_high = integer_value.used > 1UI32 ? integer_value.data[1] : 0UI32;
				mantissa_value = mantissa_low + ((UINT64)mantissa_high << 32UI64);
				exponent_value = base_exponent;
				bZeroTail = fractional_digits_present == 0UI32;
				goto AssembleDouble;
			}
			const UINT32 top_element_bits = integer_bits_of_precision % 32UI32;
			const UINT32 top_element_index = integer_bits_of_precision / 32UI32;
			const UINT32 middle_element_index = top_element_index - 1UI32;
			const UINT32 bottom_element_index = top_element_index - 2UI32;
			if (!top_element_bits)
			{
				mantissa_value = integer_value.data[bottom_element_index] + ((UINT64)(integer_value.data[middle_element_index]) << 32UI64);
				exponent_value = base_exponent + (INT32)(bottom_element_index * 32UI32);
				bZeroTail = fractional_digits_present == 0UI32;
				for (UINT32 i = 0UI32; i != bottom_element_index; ++i)
					bZeroTail &= integer_value.data[i] == 0UI32;
				goto AssembleDouble;
			}
			const UINT32 top_element_mask = (1UI32 << top_element_bits) - 1UI32;
			const UINT32 top_element_shift = 64UI32 - top_element_bits;
			const UINT32 middle_element_shift = top_element_shift - 32UI32;
			const UINT32 bottom_element_bits = 32UI32 - top_element_bits;
			const UINT32 bottom_element_mask = ~top_element_mask;
			const UINT32 bottom_element_shift = 32UI32 - bottom_element_bits;
			mantissa_value = ((UINT64)(integer_value.data[top_element_index] & top_element_mask) << top_element_shift) + ((UINT64)integer_value.data[middle_element_index] << middle_element_shift) + ((UINT64)(integer_value.data[bottom_element_index] & bottom_element_mask) >> bottom_element_shift);
			exponent_value = base_exponent + (INT32)(bottom_element_index * 32UI32 + top_element_bits);
			bZeroTail = fractional_digits_present == 0UI32 && (integer_value.data[bottom_element_index] & top_element_mask) == 0UI32;
			for (UINT32 i = 0UI32; i != bottom_element_index; ++i)
				bZeroTail &= integer_value.data[i] == 0UI32;
		}

	AssembleDouble:
		const INT32 normal_mantissa_shift = mantissa_bits - (INT32)bit_scan_reverse(mantissa_value);
		INT32 normal_exponent = exponent_value - normal_mantissa_shift;
		if (normal_exponent > maximum_binary_exponent)
		{
			components._exponent = exponent_mask;
			components._mantissa = 0UI64;
			return SLD_OVERFLOW;
		}
		if (normal_exponent < minimum_binary_exponent)
		{
			const INT32 denormal_mantissa_shift = normal_mantissa_shift + normal_exponent + exponent_bias - 1I32;
			normal_exponent = -exponent_bias;
			if (denormal_mantissa_shift < 0I32)
			{
				right_shift_with_rounding(mantissa_value, -denormal_mantissa_shift, bZeroTail);
				if (!mantissa_value)
				{
					components._exponent = exponent_mask;
					components._mantissa = 0UI64;
					return SLD_UNDERFLOW;
				}
				if (mantissa_value > denormal_mantissa_mask)
					normal_exponent = exponent_value - (denormal_mantissa_shift + 1I32) - normal_mantissa_shift;
			}
			else
			{
				mantissa_value <<= (UINT64)denormal_mantissa_shift;
			}
		}
		else
		{
			if (normal_mantissa_shift < 0I32)
			{
				right_shift_with_rounding(mantissa_value, -normal_mantissa_shift, bZeroTail);
				if (mantissa_value > normal_mantissa_mask)
				{
					mantissa_value >>= 1UI64;
					++normal_exponent;
					if (normal_exponent > maximum_binary_exponent)
					{
						components._exponent = exponent_mask;
						components._mantissa = 0UI64;
						return SLD_OVERFLOW;
					}
				}
			}
			else if (normal_mantissa_shift > 0I32)
			{
				mantissa_value <<= (UINT64)normal_mantissa_shift;
			}
		}
		mantissa_value &= denormal_mantissa_mask;
		components._exponent = (UINT64)(normal_exponent + exponent_bias);
		components._mantissa = mantissa_value;
		return SLD_OK;
	}

NoDigits:
	components._exponent = 0UI64;
	components._mantissa = 0UI64;
	return SLD_NODIGITS;
}