#pragma once

#include <intrin.h>

using float4 = __m128;
using dqword = __m128i;

struct Simd_sp;
Simd_sp Rcp( const Simd_sp &A );

// Hopefully, helpful constants
namespace simd
{
	const dqword miMaskOutX = _mm_set_epi32( 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,		0x0 );
	const dqword miMaskOutY = _mm_set_epi32( 0xFFFFFFFF, 0xFFFFFFFF,		0x0, 0xFFFFFFFF );
	const dqword miMaskOutZ = _mm_set_epi32( 0xFFFFFFFF,		0x0, 0xFFFFFFFF, 0xFFFFFFFF );
	const dqword miMaskOutW = _mm_set_epi32(		0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF );
	const dqword miTrueMask = _mm_set1_epi32( 0xFFFFFFFF );

	const float4 m0000 = _mm_setzero_ps();
	const float4 m1111 = _mm_set_ps1( 1.f );
	const float4 mNeg1111 = _mm_set_ps1( -1.f );
	const float4 mMaskOutX = _mm_castsi128_ps( miMaskOutX );
	const float4 mMaskOutY = _mm_castsi128_ps( miMaskOutY );
	const float4 mMaskOutZ = _mm_castsi128_ps( miMaskOutZ );
	const float4 mMaskOutW = _mm_castsi128_ps( miMaskOutW );
	const float4 mTrueMask = _mm_castsi128_ps( miTrueMask );

	struct alignas( 16 )SimdStore_sp
	{
		float x = 0.f, y = 0.f, z = 0.f, w = 0.f;

		SimdStore_sp() = default;
		SimdStore_sp( float X, float Y, float Z, float W )
			:
			x( X ), y( Y ), z( Z ), w( W )
		{}
	};

	Simd_sp MaskCombine( const Simd_sp &A, const Simd_sp &B, const Simd_sp &Mask );
	template<int RollA, int RollB>
	Simd_sp RollLeftAndCombine( const Simd_sp &A, const Simd_sp &B, const float4 &Mask );
}

// Wrapper for SSE intrinsics
struct Simd_sp
{
	float4 vec = simd::m0000;

	// Default constructor, nothing initialized
	Simd_sp() = default;
	// Broadcast S to all four SIMD lanes
	Simd_sp( float S )
		:
		vec( _mm_set_ps1( S ) )
	{}
	// Unaligned load from array, can't guarantee memory is aligned
	Simd_sp( float* pVec )
		:
		vec( _mm_loadu_ps( pVec ) )
	{}
	// Aligned load from SimdStore
	Simd_sp( const simd::SimdStore_sp &Stor )
		:
		vec( _mm_load_ps( reinterpret_cast<const float*>( &Stor ) ) )
	{}
	// Instantiate SIMD class from __m128 object (aliases as float4)
	Simd_sp( const float4 &Vec )
		:
		vec( Vec )
	{}
	// Copy constructor
	Simd_sp( const Simd_sp & ) = default;

	// Don't need move constructor
	Simd_sp( Simd_sp && ) = default;
	// Don't need move assignment operator
	Simd_sp &operator=( Simd_sp && ) = default;
	// Copy assignment operator
	Simd_sp &operator=( const Simd_sp & ) = default;

	// Anonymous shuffle function template, assigns shuffle to self
	// The four immediate numbers passed as template parameters, correspond to which
	// element gets picked. Ex, <0,0,0,0> would mean copy the X element to all four 
	// lanes
	template<int X, int Y, int Z, int W>Simd_sp &Shuffle()
	{
		vec = _mm_shuffle_ps( vec, vec, _MM_SHUFFLE( W, Z, Y, X ) );
		return *this;
	}
	// Anonymous shuffle function template, shuffles self with A.  Modifies self
	template<int X, int Y, int Z, int W>Simd_sp &Shuffle( const Simd_sp &A )
	{
		vec = _mm_shuffle_ps( vec, A.vec, _MM_SHUFFLE( W, Z, Y, X ) );
		return *this;
	}

	// Splat functions all modify self. They use the anonymous Shuffle functions above.

	// SplatX copies the X lane to all four lanes
	Simd_sp &SplatX()
	{
		*this = Shuffle<0, 0, 0, 0>();
		return *this;
	}
	// SplatY copies the Y lane to all four lanes
	Simd_sp &SplatY()
	{
		*this = Shuffle<1, 1, 1, 1>();
		return *this;
	}
	// SplatZ copies the Z lane to all four lanes
	Simd_sp &SplatZ()
	{
		*this = Shuffle<2, 2, 2, 2>();
		return *this;
	}
	// SplatW copies the W lane to all four lanes
	Simd_sp &SplatW()
	{
		*this = Shuffle<3, 3, 3, 3>();
		return *this;
	}

	// Aligned store using SimdStore_sp
	simd::SimdStore_sp Store()const
	{
		simd::SimdStore_sp dst;
		_mm_store_ps( reinterpret_cast< float* >( &dst ), vec );
		return dst;
	}
	// Unaligned store using float* as parameter
	void Store( float *pVec )const
	{
		_mm_storeu_ps( pVec, vec );
	}

	// Math operator overloads
	Simd_sp &operator+=( const Simd_sp &A )
	{
		vec = _mm_add_ps( vec, A.vec );
		return *this;
	}
	Simd_sp &operator-=( const Simd_sp &A )
	{
		vec = _mm_sub_ps( simd::m0000, A.vec );
		return *this;
	}
	Simd_sp &operator*=( const Simd_sp &A )
	{
		vec = _mm_mul_ps( vec, A.vec );
		return *this;
	}
	Simd_sp &operator/=( const Simd_sp &A )
	{
		vec = _mm_mul_ps( vec, Rcp( A ).vec );
		return *this;
	}

};

// Math operators

inline Simd_sp operator+( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( A ) += B;
	return result;
}
inline Simd_sp operator-( const Simd_sp &A )
{
	const auto result = Simd_sp( simd::m0000 ) -= A;
	return result;
}
inline Simd_sp operator-( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( A ) -= B;
	return result;
}
inline Simd_sp operator*( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( A ) *= B;
	return result;
}
inline Simd_sp operator/( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( A ) /= B;
	return result;
}
inline Simd_sp Sqrt( const Simd_sp &A )
{
	return Simd_sp( _mm_sqrt_ps( A.vec ) );
}
inline Simd_sp Rcp( const Simd_sp &A )
{
	return Simd_sp( _mm_rcp_ps( A.vec ) );
}
inline Simd_sp Min( const Simd_sp &A, const Simd_sp &B )
{
	return Simd_sp( _mm_min_ps( A.vec, B.vec ) );
}
inline Simd_sp Max( const Simd_sp &A, const Simd_sp &B )
{	
	return Simd_sp( _mm_max_ps( A.vec, B.vec ) );
}

// Logical operators

inline Simd_sp Andnot( const Simd_sp &A, const Simd_sp &Mask )
{
	const auto result = Simd_sp( _mm_andnot_ps( Mask.vec, A.vec ) );
	return result;
}
// The Andnot function is more efficient as it does the two steps at once
inline Simd_sp operator~( const Simd_sp &Mask )
{
	const auto notMask = Simd_sp( Andnot( simd::mTrueMask, Mask ) );
	return notMask;
}
inline Simd_sp operator&( const Simd_sp &A, const Simd_sp &Mask )
{
	const auto result = Simd_sp( _mm_and_ps( Mask.vec, A.vec ) );
	return result;
}
inline Simd_sp operator|( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( _mm_or_ps( A.vec, B.vec ) );
	return result;
}

// Comparison operators

inline Simd_sp operator<=( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( _mm_cmple_ps( A.vec, B.vec ) );
	return result;
}
inline Simd_sp operator>=( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( _mm_cmpge_ps( A.vec, B.vec ) );
	return result;
}
inline Simd_sp operator<( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( _mm_cmplt_ps( A.vec, B.vec ) );
	return result;
}
inline Simd_sp operator>( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( _mm_cmpgt_ps( A.vec, B.vec ) );
	return result;
}
inline Simd_sp operator==( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( _mm_cmpeq_ps( A.vec, B.vec ) );
	return result;
}
inline Simd_sp operator!=( const Simd_sp &A, const Simd_sp &B )
{
	const auto result = Simd_sp( _mm_cmpneq_ps( A.vec, B.vec ) );
	return result;
}

// Simd utilities

// MaskCombine combines A and B by anding the mask with A
// then inverting the mask and anding with B.
// The results from the previous operations are then or'd together for the result.
inline Simd_sp simd::MaskCombine( const Simd_sp &A, const Simd_sp &B, const Simd_sp &Mask )
{
	const auto result = ( A & Mask ) | ( A & ~Mask );	
	return result;
}

// RollA/RollB must be integer literals from 0 to 3.  
// RollA will roll A by the amount specified
// RollB will roll B by the amount specified, 
// the results are then or masked together using the provided mask
template<int RollA, int RollB>
Simd_sp simd::RollLeftAndCombine( const Simd_sp &A, const Simd_sp &B, const float4 &Mask )
{
	const Simd_sp tA = _mm_rolll_ps<RollA>( A );
	const Simd_sp tB = _mm_rolll_ps<RollB>( B );
	const Simd_sp result = simd::MaskCombine( tA, tB, Mask );

	return result;
}

// Function is used to roll A by as specified by Count.  
// Valid range for Count is 0-3 inclusive.  
// 0: returns A, no rolling
// 1: rolls all elements to the left, the first element is moved to the back
// 2: rolls all elements to the left twice, the first two elements are swapped with the last two
// 3: rolls all elements to the left thrice, the last element is moved to the front.
template<int Count>
Simd_sp _mm_rolll_ps( const Simd_sp &A )
{
	auto result = A;
	constexpr int count = Count % 4;
	switch ( count )
	{
		case 0:
			return result;
			break;
		case 1:
			return result.Shuffle<1, 2, 3, 0>();
			break;
		case 2:
			return result.Shuffle<2, 3, 0, 1>();
			break;
		case 3:
			return result.Shuffle<3, 0, 1, 2>();
	}
}

template<int Count>
Simd_sp _mm_rollr_ps( const Simd_sp &A )
{
	auto result = A;
	constexpr int count = Count % 4;
	switch ( count )
	{
		case 0:
			return result;
			break;
		case 1:
			return result.Shuffle<3, 0, 1, 2>();
			break;
		case 2:
			return result.Shuffle<2, 3, 0, 1>();
			break;
		case 3:
			return result.Shuffle<1, 2, 3, 0>();
	}
}

// Vec2p is a two element math vector of simd elements.  
// Instead of two floats, it handles two sets of four floats
struct Vec2p
{
	Simd_sp x = simd::m0000, y = simd::m0000;

	Vec2p() = default;
	Vec2p( float X, float Y )
		:
		x( _mm_set_ps1( X ) ),
		y( _mm_set_ps1( Y ) )
	{}
	Vec2p( const Simd_sp &X, const Simd_sp &Y )
		:
		x( X ),
		y( Y )
	{}
	Vec2p( const Vec2p &Src )
		:
		x( Src.x ),
		y( Src.y )
	{}

	Vec2p &operator+=( const Vec2p &V )
	{
		x = x + V.x;
		y = y + V.y;
		return *this;
	}
	Vec2p &operator-=( const Vec2p &V )
	{
		x = x - V.x;
		y = y - V.y;
		return *this;
	}
	Vec2p &operator*=( const Simd_sp &S )
	{
		x = x * S;
		y = y * S;
		return *this;
	}
	Vec2p &operator/=( const Simd_sp &S )
	{		
		*this *= Rcp( S );
		return *this;
	}
};

inline Vec2p operator+( const Vec2p &A, const Vec2p &B )
{
	Vec2p result( A );
	result += B;
	return result;
}
inline Vec2p operator-( const Vec2p &A )
{
	Vec2p result( simd::m0000, simd::m0000 );
	result -= A;
	return result;
}
inline Vec2p operator-( const Vec2p &A, const Vec2p &B )
{
	Vec2p result( A );
	result -= B;
	return result;
}
inline Vec2p operator*( const Vec2p &A, const Simd_sp &B )
{
	Vec2p result( A );
	result *= B;
	return result;
}
inline Vec2p operator*( const Simd_sp &A, const Vec2p &B)
{
	Vec2p result( B );
	result *= A;
	return result;
}
inline Vec2p operator/( const Vec2p &A, const Simd_sp &B )
{
	Vec2p result( A );
	result /= B;
	return result;
}
inline Vec2p operator/( const Simd_sp &A, const Vec2p &B)
{
	Vec2p result( B );
	result /= A;
	return result;
}
Simd_sp DotProduct( const Vec2p &A, const Vec2p &B )
{
	const Simd_sp result = ( ( A.x * B.x ) + ( A.y * B.y ) );
	return result;
}
Vec2p Normalize( const Vec2p &A )
{
	const Vec2p result = A / Sqrt( DotProduct( A, A ) );
	return result;
}

// Vec3p: Coming soon
// Vec4p: Coming sooner or later