#ifndef __Engine_Typedef_h__

namespace Engine
{
	typedef		bool					_bool;

	typedef		signed char				_char;
	typedef		signed char				_byte;
	typedef		unsigned char			_ubyte;

	typedef		wchar_t					_tchar;

	typedef		signed short			_short;
	typedef		unsigned short			_ushort;

	typedef		signed int				_int;
	typedef		unsigned int			_uint;

	typedef		signed long				_long;
	typedef		unsigned long			_ulong;
	typedef		unsigned long long		_ullong;

	typedef		float					_float;
	typedef		double					_double;

	typedef		XMFLOAT2				_float2;		// Member
	typedef		XMFLOAT3				_float3;		// Member
	typedef		XMFLOAT4				_float4;		// Member

	typedef		XMVECTOR				_vector;		// Local
	typedef		FXMVECTOR				_fvector;		// Parameter 1st ~ 3rd
	typedef		GXMVECTOR				_gvector;		// Parameter 4th
	typedef		HXMVECTOR				_hvector;		// Parameter 5th ~ 6th

	typedef		XMFLOAT4X4				_float4x4;		// Member

	typedef		XMMATRIX				_matrix;		// Local
	typedef		FXMMATRIX				_fmatrix;		// Parameter 1st
	typedef		CXMMATRIX				_cmatrix;		// Parameter 2nd ~ 

	typedef		XMINT2					_int2;
	typedef		XMINT3					_int3;
	typedef		XMINT4					_int4;
}

#define __Engine_Typedef_h__
#endif // __Engine_Typedef_h__
