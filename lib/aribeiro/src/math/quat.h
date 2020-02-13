#ifndef quat4_h
#define quat4_h

#include <aribeiro/common.h>
#include <aribeiro/SSE2.h>
#include <aribeiro/floatOPs.h>
#include <aribeiro/vec4.h>

namespace aRibeiro {

#if defined(ARIBEIRO_SSE2)
    #pragma pack(push, 16)
#endif

/// \brief Quaternion (quat)
///
/// Stores four components(x,y,z,w) to represent a quaternion. <br/>
/// The quaternion can be seen as a unit axis with an angle in radians.
/// \author Alessandro Ribeiro
///
_SSE2_ALIGN_PRE class ARIBEIRO_API quat{
    public:
	_SSE2_ALIGN_PRE union {
		_SSE2_ALIGN_PRE float array[4]_SSE2_ALIGN_POS;///<The 4D low level array representation to pass the vector as pointer parameter
		_SSE2_ALIGN_PRE struct{ float x,y,z,w; }_SSE2_ALIGN_POS;///<Components X, Y, Z and W to be used by the application
#if defined(ARIBEIRO_SSE2)
		_SSE2_ALIGN_PRE __m128 array_sse _SSE2_ALIGN_POS;
#endif

#if defined(ARIBEIRO_NEON)
		_SSE2_ALIGN_PRE float32x4_t array_neon _SSE2_ALIGN_POS;
#endif

    }_SSE2_ALIGN_POS;

#if defined(ARIBEIRO_SSE2)
    //special SSE2 constructor
    ARIBEIRO_INLINE quat( const __m128 &v ){
        array_sse = v;
    }
#endif

#if defined(ARIBEIRO_NEON)
    ARIBEIRO_INLINE quat( const float32x4_t &v ){
        array_neon = v;
    }
#endif

    /// \brief Construct an identity quaternion quat class
    ///
    /// The identity quat class have the folow configuration (x=0,y=0,z=0,w=1)
    /// \author Alessandro Ribeiro
    ///
    ARIBEIRO_INLINE quat(){
#if defined(ARIBEIRO_SSE2)
        static const __m128 _load_0001_ = _mm_load_(0.0f,0.0f,0.0f,1.0f);
        array_sse = _load_0001_;
#elif defined(ARIBEIRO_NEON)
        array_neon = (float32x4_t){0.0f,0.0f,0.0f,1.0f};
#else
        x = y = z = 0.0f;
        w = 1;
#endif
    }
    /// \brief Constructs a quaterion
    ///
    /// Initialize the quat components from the parameters
    ///
    /// \author Alessandro Ribeiro
    /// \param x Value to assign to the X component
    /// \param y Value to assign to the Y component
    /// \param z Value to assign to the Z component
    /// \param w Value to assign to the W component
    ///
    ARIBEIRO_INLINE quat( const float x, const float y, const float z, const float w ){
#if defined(ARIBEIRO_SSE2)
		array_sse = _mm_load_(x, y, z, w);
#elif defined(ARIBEIRO_NEON)
        array_neon = (float32x4_t){x, y, z, w};
#else
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
#endif
    }
    /// \brief Constructs a quaternion
    ///
    /// Initialize the quat components from other quat instance
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to assign to the instance
    ///
    ARIBEIRO_INLINE quat( const quat &v ){
#if defined(ARIBEIRO_SSE2)
        array_sse = v.array_sse;
#elif defined(ARIBEIRO_NEON)
        array_neon = v.array_neon;
#else
        *this = v;
#endif
    }
    /// \brief Binary comparison of vectors
    ///
    /// Compare two vectors using memcmp to see if they are the same.
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to compare against
    /// \return true if the bits of this is the same of v, otherwise false.
    ///
    ARIBEIRO_INLINE bool operator==(const quat&v) const {
#if defined(ARIBEIRO_SSE2)

        __m128 diff_abs = _mm_sub_ps(array_sse, v.array_sse);
        //abs
        static const __m128 _vec4_sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
        diff_abs = _mm_andnot_ps(_vec4_sign_mask, diff_abs);

        //static const __m128 epsilon = _mm_set1_ps(1e-4f); // -0.f = 1 << 31
        //_mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));

        for(int i=0;i<4;i++){
            if (_mm_f32_(diff_abs,i) > 1e-4f)
                return false;
        }

        return true;
#elif defined(ARIBEIRO_NEON)

        float32x4_t diff_abs = vsubq_f32(array_neon, v.array_neon);
        //abs
        diff_abs = vabsq_f32(diff_abs);

        //static const __m128 epsilon = _mm_set1_ps(1e-4f); // -0.f = 1 << 31
        //_mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));

        for(int i=0;i<4;i++){
            if (diff_abs[i] > 1e-4f)
                return false;
        }

        return true;

#else
        for(int i=0;i<4;i++){
            if (absv(array[i]-v.array[i]) > 1e-4f)
                return false;
        }
        return true;
        //return memcmp(array, v.array, sizeof(float) * 4) == 0;
#endif
    }

    ARIBEIRO_INLINE quat operator-() const{
#if defined(ARIBEIRO_SSE2)
        static const __m128 _vec4_sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
        return _mm_xor_ps(_vec4_sign_mask, array_sse);
#elif defined(ARIBEIRO_NEON)
        static const float32x4_t minus_one = (float32x4_t){-1.0f,-1.0f,-1.0f,-1.0f};
        return vmulq_f32(minus_one, array_neon);
#else
        return quat(-x,-y,-z,-w);
#endif
    }

    ARIBEIRO_INLINE bool operator!=(const quat&v) const{
        return !((*this) == v);
        //return memcmp(array, v.array, sizeof(float) * 4) != 0;
    }
    /// \brief Index the components of the quat as a C array
    ///
    /// \author Alessandro Ribeiro
    /// \param v The index of the components starting by 0
    /// \return A reference to the element at the index v
    ///
    ARIBEIRO_INLINE float& operator[](const int v){
        return array[v];
    }

	SSE2_CLASS_NEW_OPERATOR

} _SSE2_ALIGN_POS;

#if defined(ARIBEIRO_SSE2)
    #pragma pack(pop)
#endif

}

#endif
