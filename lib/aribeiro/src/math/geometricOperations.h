#ifndef geometricOperations_h
#define geometricOperations_h

#include <limits>
#include <stdio.h>

#include <aribeiro/common.h>
#include <aribeiro/floatOPs.h>
#include <aribeiro/mat4.h>
#include <aribeiro/quat.h>
#include <aribeiro/SSE2.h>

namespace aRibeiro {

#if defined(ARIBEIRO_SSE2)

    ARIBEIRO_INLINE __m128 dot_sse_3(const __m128 &a, const __m128 &b){
        __m128 mul0 = _mm_mul_ps(a, b);
        //swp0 = [1,0,0,3]
        __m128 swp0 = _mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(3, 0, 0, 1));
        //add0 = [0+1,1+0,2+0,3+3]
        __m128 add0 = _mm_add_ps(mul0, swp0);
        //swp1 = [2,2,1,3]
        __m128 swp1 = _mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(3, 1, 2, 2));
        //add1 = [0+1+2,1+0+2,2+0+1,3+3+3]
        __m128 add1 = _mm_add_ps(add0, swp1 );
        return add1;
    }

    ARIBEIRO_INLINE __m128 dot_sse_4(const __m128 &a, const __m128 &b){
        __m128 mul0 = _mm_mul_ps(a, b);
        //swp0 = [1,0,3,2]
        __m128 swp0 = _mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));
        //add0 = [0+1,1+0,2+3,3+2]
        __m128 add0 = _mm_add_ps(mul0, swp0);
        //swp1 = [3+2,2+3,1+0,0+1]
        __m128 swp1 = _mm_shuffle_ps(add0, add0, _MM_SHUFFLE(0, 1, 2, 3));
        //add1 = [0+1+3+2,1+0+2+3,2+3+1+0,3+2+0+1]
        __m128 add1 = _mm_add_ps(add0, swp1);
        return add1;
    }

    ARIBEIRO_INLINE __m128 clamp_sse_4(const __m128& value,const __m128& min,const __m128& max ){
        __m128 maxStep = _mm_max_ps(value, min);
        __m128 minStep = _mm_min_ps(maxStep, max);
        return minStep;
    }
#elif defined(ARIBEIRO_NEON)





/*

    ARIBEIRO_INLINE float32x4_t vshuffle_2301(const float32x4_t &a, const float32x4_t &b)
    {
        float32x2_t a01 = vrev64_f32(vget_low_f32(a));
        float32x2_t b23 = vrev64_f32(vget_high_f32(b));
        return vcombine_f32(a01, b23);
    }

    ARIBEIRO_INLINE float32x4_t vshuffle_1001(const float32x4_t &a, const float32x4_t &b)
    {
        float32x2_t a01 = vrev64_f32(vget_low_f32(a));
        float32x2_t b10 = vget_low_f32(b);
        return vcombine_f32(a01, b10);
    }

    ARIBEIRO_INLINE float32x4_t vshuffle_3001(const float32x4_t &a, const float32x4_t &b)
    {
        float32x2_t a01 = vrev64_f32(vget_low_f32(a));
        float32x2_t b30 = vrev64_f32(vget_low_f32(vextq_f32(b, b, 3)));
        return vcombine_f32(a01, b30);
    }

    ARIBEIRO_INLINE float32x4_t vshuffle_0122(const float32x4_t &a)
    {
        float32x2_t a22 = vdup_lane_f32(vget_high_f32(a), 0);
        float32x2_t a01 = vrev64_f32(vget_low_f32(a));
        return vcombine_f32(a22, a01);
    }

    ARIBEIRO_INLINE float32x4_t vshuffle_3122(const float32x4_t &a)
    {
        float32x2_t a22 = vdup_lane_f32(vget_high_f32(a), 0);
        float32x2_t a31 = vrev64_f32(vget_low_f32(a));
        a31[1] = a[3];
        return vcombine_f32(a22, a31);
    }
    */

    ARIBEIRO_INLINE float32x4_t dot_neon_4(const float32x4_t &a, const float32x4_t &b){
        float32x4_t prod = vmulq_f32(a, b);
        // sum1 = [ 0+2, 1+3, 2+0, 3+1 ]
        float32x4_t sum1 = vaddq_f32(prod, vshuffle_2301(prod));
        // sum2 = [ 0+2 3+1, 1+3  2+0, 2+0 1+3, 3+1 0+2 ]
        float32x4_t sum2 = vaddq_f32(sum1, vshuffle_0123(sum1));

        return sum2;
    }

    ARIBEIRO_INLINE float32x4_t dot_neon_3(const float32x4_t &a, const float32x4_t &b){
        static const float32x4_t _const_n = (float32x4_t){ 1,1,1,0 };
        return dot_neon_4(vmulq_f32(a,_const_n),b);
    }


    ARIBEIRO_INLINE float32x4_t clamp_neon_4(const float32x4_t& value,const float32x4_t& min,const float32x4_t& max ){
        float32x4_t maxStep = vmaxq_f32(value, min);
        float32x4_t minStep = vminq_f32(maxStep, max);
        return minStep;
    }

#endif


    /// \brief Converts a vec4 to a vec3 by discarding the w component of v
    ///
    /// Considering that the parameter is a point (i.e. with w=0) or a vector (i.e. with w=1), it can be converted to a vec3 without the w component.
    ///
    /// \author Alessandro Ribeiro
    /// \param v A vector with 4 components
    /// \return A vec3 containing xyz from the parameter
    ///
    ARIBEIRO_INLINE vec3 toVec3(const vec4 &v){
#if defined(ARIBEIRO_SSE2)
        return v.array_sse;
#elif defined(ARIBEIRO_NEON)
        return v.array_neon;
#else
        return vec3(v.x, v.y, v.z);
#endif
    }

    /// \brief Converts a vec4 to a vec3 dividing the vec4 by the w component
    ///
    /// Considering that the parameter is a projected point (i.e. w indicating the lenght in the projective space), it can be converted to a point (vec3) as the result of the division:
    ///
    /// vec3.xyz = vec4.xyz / vec4.w
    ///
    /// \author Alessandro Ribeiro
    /// \param v A vector with 4 components
    /// \return A vec3 containing (xyz)/w from the parameter
    ///
    ARIBEIRO_INLINE vec3 toVec3_PerspDiv(const vec4 &v){
#if defined(ARIBEIRO_SSE2)
        return _mm_mul_ps(v.array_sse, _mm_set1_ps( 1.0f / _mm_f32_(v.array_sse,3) ));
#elif defined(ARIBEIRO_NEON)
        return vmulq_f32( v.array_neon, vset1( 1.0f / v.array_neon[3] ) );
#else
        return vec3(v.x, v.y, v.z)*(1.0f / v.w);
#endif
    }


    /// \brief Converts a vec3 to a vec4 with w = 0
    ///
    /// Considering that the parameter is not a point, it can be converted to a vector with homogeneous componente as zero.
    ///
    /// \author Alessandro Ribeiro
    /// \param v A vector with 3 components
    /// \return A vec4 containing vec4(x,y,z,0)
    ///
    ARIBEIRO_INLINE vec4 toVec4(const vec3 &v){
#if defined(ARIBEIRO_SSE2)
        //static const __m128 _const_n = _mm_load_( 1,1,1,0 );
        //return _mm_mul_ps(v.array_sse, _const_n);
        
        //much faster...
        __m128 result = v.array_sse;
		_mm_f32_(result,3) = 0;
        return result;
#elif defined(ARIBEIRO_NEON)
        static const float32x4_t _const_n = (float32x4_t){ 1,1,1,0 };
        return vmulq_f32(v.array_neon, _const_n);
#else
        return vec4(v.x, v.y, v.z, 0);
#endif
    }

    /// \brief Converts a vec3 to a vec4 with w = 1
    ///
    /// Considering that the parameter is a point, it can be converted to a vector with homogeneous componente as one.
    ///
    /// \author Alessandro Ribeiro
    /// \param v A vector with 3 components
    /// \return A vec4 containing vec4(x,y,z,1)
    ///
    ARIBEIRO_INLINE vec4 toPtn4(const vec3 &v){
#if defined(ARIBEIRO_SSE2)
        //static const __m128 _const_n = _mm_load_( 1,1,1,0 );
        //static const __m128 _const2_n = _mm_load_( 0,0,0,1 );
        //return _mm_add_ps( _mm_mul_ps(v.array_sse, _const_n), _const2_n );
        
        //much faster
        __m128 result = v.array_sse;
		_mm_f32_(result,3) = 1.0f;
        return result;
#elif defined(ARIBEIRO_NEON)

        static const float32x4_t _const_n = (float32x4_t){ 1,1,1,0 };
        static const float32x4_t _const2_n = (float32x4_t){ 0,0,0,1 };
        return vaddq_f32( vmulq_f32(v.array_neon, _const_n), _const2_n );

        //float32x4_t result = v.array_neon;
        //result[3] = 1.0f;
        //return result;
#else
        return vec4(v.x, v.y, v.z, 1);
#endif
    }

    /// \brief Converts the 1D polar coordinate to a 2D vector
    ///
    /// Uses the pAngle to define the vector in counter-clockwise orientation.
    ///
    /// The final vector will have the lenght indicated by the radius.
    ///
    /// \author Alessandro Ribeiro
    /// \param pAngle An angle in radians
    /// \param pRadius An radius indicating the final length of the vector
    /// \return A vec2 represented as the polar coordinate and a length
    ///
    ARIBEIRO_INLINE vec2 polarToVec2(float pAngle, float pRadius){
        vec2 vec(0);
        vec.x += pRadius * cosf(DEG2RAD(pAngle));
        vec.y += pRadius * sinf(DEG2RAD(pAngle));

        return vec;
    }



    /// \brief Clamp values in a component wise fashion
    ///
    /// For each component of the vector, evaluate:
    /// <pre>
    ///     if min < value then return min
    ///     if max > value then return max
    ///     else return value
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param value The value to evaluate
    /// \param min The min threshold
    /// \param max The max threshold
    /// \return The evaluated value
    ///
    ARIBEIRO_INLINE vec2 clamp(const vec2& value,const vec2& min,const vec2& max ){
#if defined(ARIBEIRO_SSE2)
        return clamp_sse_4(value.array_sse, min.array_sse, max.array_sse);
#elif defined(ARIBEIRO_NEON)
        return clamp_neon_4(value.array_neon, min.array_neon, max.array_neon);
#else
        return vec2(
                    (value.x < min.x) ? min.x : ((value.x > max.x) ? max.x : value.x),
                    (value.y < min.y) ? min.y : ((value.y > max.y) ? max.y : value.y)
                    );
#endif
    }


    /// \brief Clamp values in a component wise fashion
    ///
    /// For each component of the vector, evaluate:
    /// <pre>
    ///     if min < value then return min
    ///     if max > value then return max
    ///     else return value
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param value The value to evaluate
    /// \param min The min threshold
    /// \param max The max threshold
    /// \return The evaluated value
    ///
    ARIBEIRO_INLINE vec3 clamp( const vec3& value, const vec3& min,const vec3& max ){
#if defined(ARIBEIRO_SSE2)
        return clamp_sse_4(value.array_sse, min.array_sse, max.array_sse);
#elif defined(ARIBEIRO_NEON)
        return clamp_neon_4(value.array_neon, min.array_neon, max.array_neon);
#else
        return vec3(
                    (value.x < min.x) ? min.x : ((value.x > max.x) ? max.x : value.x),
                    (value.y < min.y) ? min.y : ((value.y > max.y) ? max.y : value.y),
                    (value.z < min.z) ? min.z : ((value.z > max.z) ? max.z : value.z)
                    );
#endif
    }


    /// \brief Clamp values in a component wise fashion
    ///
    /// For each component of the vector, evaluate:
    /// <pre>
    ///     if min < value then return min
    ///     if max > value then return max
    ///     else return value
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param value The value to evaluate
    /// \param min The min threshold
    /// \param max The max threshold
    /// \return The evaluated value
    ///
    ARIBEIRO_INLINE vec4 clamp( const vec4& value, const vec4& min, const vec4& max ){
#if defined(ARIBEIRO_SSE2)
        return clamp_sse_4(value.array_sse, min.array_sse, max.array_sse);
#elif defined(ARIBEIRO_NEON)
        return clamp_neon_4(value.array_neon, min.array_neon, max.array_neon);
#else
        return vec4(
                    (value.x < min.x) ? min.x : ((value.x > max.x) ? max.x : value.x),
                    (value.y < min.y) ? min.y : ((value.y > max.y) ? max.y : value.y),
                    (value.z < min.z) ? min.z : ((value.z > max.z) ? max.z : value.z),
                    (value.w < min.w) ? min.w : ((value.w > max.w) ? max.w : value.w)
                    );
#endif
    }


    /// \brief Constructs a conjugate quaternion
    ///
    /// The conjugate is (-x,-y,-z,w).
    ///
    /// \author Alessandro Ribeiro
    /// \param a Input quaternion
    /// \return The quat
    ///
    ARIBEIRO_INLINE quat conjugate(const quat& a){
#if defined(ARIBEIRO_SSE2)
        static const __m128 _quat_conjugate_mask = _mm_load_(-0.f,-0.f,-0.f,0.f);
        return _mm_xor_ps(_quat_conjugate_mask,a.array_sse);
#elif defined(ARIBEIRO_NEON)
        static const float32x4_t _const_n = (float32x4_t){-1,-1,-1,1};
        return vmulq_f32(a.array_neon, _const_n );
#else
        return quat(-a.x, -a.y, -a.z, a.w);
#endif
    }

    /// \brief Computes the dot product between two vectors
    ///
    /// The dot product is a single value computed from the two vectors.
    /// <pre>
    /// It can be interpreted as:
    ///
    /// -The squared length of the vector modulos
    ///  when the two vectors are the same:
    ///    dot(a,a) = |a|^2 = x^2+y^2+z^2
    ///
    /// -The length of a projection
    ///  when some vector is an unit vector:
    ///    dot(a,UnitV) = dot(UnitV,a) =
    ///    signed length of 'a' after the projection in the UnitV
    /// =================================================================
    ///      / a
    ///     /
    ///     ----->UnitV
    ///
    ///      /
    ///     /
    ///     -- projected 'a' over UnitV
    ///    |  |
    ///    length of the projection with positive sign
    /// =================================================================
    ///   \ a
    ///    \                                                            .
    ///     ----->UnitV
    ///
    ///   \                                                             .
    ///    \                                                            .
    ///   -- projected 'a' over UnitV
    ///  |  |
    ///  length of the projection with negative sign
    /// =================================================================
    /// -The hemisfere side of two vectors:
    ///    dot(a,b) = dot(b,a) = value
    ///        -- value < 0 => they are in the opose direction.
    ///        -- value > 0 => they are in the same direction.
    ///        -- value = 0 => they are orthogonal(90 degrees) vectors.
    ///
    /// -The dot product computes the equation:
    ///    dot(a,b) = |cos(angle between a and b)|*|a|*|b|
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The first vector
    /// \param b The second vector
    /// \return The dot product between the two vectors
    ///
    ARIBEIRO_INLINE float dot( const vec2& a, const vec2& b ){
#if defined(ARIBEIRO_SSE2)
        __m128 mul0 = _mm_mul_ps(a.array_sse, b.array_sse);
        //swp0 = [1,0,0,3]
        __m128 swp0 = _mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(0, 0, 0, 1));
        //add0 = [0+1,1+0,2+0,3+3]
        __m128 add0 = _mm_add_ps(mul0, swp0);
        return _mm_f32_(add0,0);
#elif defined(ARIBEIRO_NEON)
        float32x4_t mul0 = vmulq_f32(a.array_neon, b.array_neon);
        return mul0[0] + mul0[1];
#else
        return (a.x * b.x + a.y * b.y);
#endif
    }
    /// \brief Computes the dot product between two vectors
    ///
    /// For a more detailed description, see dot( const vec2& a, const vec2& b )
    ///
    /// \author Alessandro Ribeiro
    /// \sa dot( const vec2& a, const vec2& b )
    /// \author Alessandro Ribeiro
    /// \param a The first vector
    /// \param b The second vector
    /// \return The dot product between the two vectors
    ///
    ARIBEIRO_INLINE float dot( const vec3& a, const vec3& b ){
#if defined(ARIBEIRO_SSE2)
        return _mm_f32_(dot_sse_3(a.array_sse, b.array_sse),0);
#elif defined(ARIBEIRO_NEON)
        return dot_neon_3(a.array_neon, b.array_neon)[0];
#else
        return (a.x * b.x + a.y * b.y + a.z * b.z);
#endif
    }
    /// \brief Computes the dot product between two vectors
    ///
    /// For a more detailed description, see dot( const vec2& a, const vec2& b )
    ///
    /// \author Alessandro Ribeiro
    /// \sa dot( const vec2& a, const vec2& b )
    /// \param a The first vector
    /// \param b The second vector
    /// \return The dot product between the two vectors
    ///
    ARIBEIRO_INLINE float dot( const vec4& a, const vec4& b ){
#if defined(ARIBEIRO_SSE2)
        return _mm_f32_(dot_sse_4(a.array_sse, b.array_sse),0);
#elif defined(ARIBEIRO_NEON)
        return dot_neon_4(a.array_neon, b.array_neon)[0];
#else
        return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
#endif
    }

    /// \brief Computes the dot product between two quaternions
    ///
    /// For a more detailed description, see dot( const vec2& a, const vec2& b )
    ///
    /// \author Alessandro Ribeiro
    /// \sa dot(const quat& a, const quat& b)
    /// \param a The first quat
    /// \param b The second quat
    /// \return The dot product between the two quaternions
    ///
    ARIBEIRO_INLINE float dot(const quat& a, const quat& b){
#if defined(ARIBEIRO_SSE2)
        return _mm_f32_(dot_sse_4(a.array_sse, b.array_sse),0);
#elif defined(ARIBEIRO_NEON)
        return dot_neon_4(a.array_neon, b.array_neon)[0];
#else
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
#endif
    }


    /// \brief Normalize a vector
    ///
    /// Returns a unit vector in the same direction of the parameter.
    ///
    /// result = vec/|vec|
    ///
    /// \author Alessandro Ribeiro
    /// \param vec The vector to normalize
    /// \return The unit vector
    ///
    ARIBEIRO_INLINE vec2 normalize(const vec2& vec ){
        vec2 result = vec;
        if (vec == vec2(0)) return vec;
        const float TOLERANCE = 1e-6f;
        // Don't normalize if we don't have to
        float mag2 = dot(vec, vec);
        if (fabs(mag2) > TOLERANCE && fabs(mag2 - 1.0f) > TOLERANCE)
            result = (vec * (1.0f / sqrtf(mag2)));
        return result;
    }

    /// \brief Normalize a vector
    ///
    /// Returns a unit vector in the same direction of the parameter.
    ///
    /// result = vec/|vec|
    ///
    /// \author Alessandro Ribeiro
    /// \param vec The vector to normalize
    /// \return The unit vector
    ///
    ARIBEIRO_INLINE vec3 normalize(const vec3& vec ){
#if defined(ARIBEIRO_SSE2)
        __m128 mag2 = dot_sse_3(vec.array_sse, vec.array_sse);
        //const float TOLERANCE = 1e-6f;
        if (fabs(_mm_f32_(mag2,0)) > EPSILON2 && fabs(_mm_f32_(mag2,0) - 1.0f) > EPSILON2)
        {
            //_mm_rsqrt_ps low precision issues...
            __m128 magInv = _mm_set1_ps(1.0f/sqrtf(_mm_f32_(mag2,0)));//_mm_rsqrt_ps( mag2 );
            return _mm_mul_ps( vec.array_sse, magInv );
        }
        return vec;
#elif defined(ARIBEIRO_NEON)
        float32x4_t mag2 = dot_neon_3(vec.array_neon, vec.array_neon);
        //const float TOLERANCE = 1e-6f;
        if (fabs(mag2[0]) > EPSILON2 && fabs(mag2[0] - 1.0f) > EPSILON2)
        {
            //_mm_rsqrt_ps low precision issues...
            float32x4_t magInv = vset1(1.0f/sqrtf(mag2[0]));//_mm_rsqrt_ps( mag2 );
            return vmulq_f32( vec.array_neon, magInv );
        }
        return vec;
#else
        vec3 result = vec;
        if (vec == vec3(0)) return vec;
        //const float TOLERANCE = 1e-6f;
        // Don't normalize if we don't have to
        float mag2 = dot(vec, vec);
        if (fabs(mag2) > EPSILON2 && fabs(mag2 - 1.0f) > EPSILON2)
            result = (vec * (1.0f / sqrtf(mag2)));
        return result;
#endif
    }

    /// \brief Normalize a vector
    ///
    /// Returns a unit vector in the same direction of the parameter.
    ///
    /// result = vec/|vec|
    ///
    /// \author Alessandro Ribeiro
    /// \param vec The vector to normalize
    /// \return The unit vector
    ///
    ARIBEIRO_INLINE vec4 normalize(const vec4& vec ){
#if defined(ARIBEIRO_SSE2)
        __m128 mag2 = dot_sse_4(vec.array_sse, vec.array_sse);
        //const float TOLERANCE = 1e-6f;
        if (fabs(_mm_f32_(mag2,0)) > EPSILON2 && fabs(_mm_f32_(mag2,0) - 1.0f) > EPSILON2){
            //_mm_rsqrt_ps low precision issues...
            __m128 magInv = _mm_set1_ps(1.0f/sqrtf(_mm_f32_(mag2,0)));//_mm_rsqrt_ps( mag2 );
            return _mm_mul_ps( vec.array_sse, magInv );
        }
        return vec;
#elif defined(ARIBEIRO_NEON)
        float32x4_t mag2 = dot_neon_4(vec.array_neon, vec.array_neon);
        //const float TOLERANCE = 1e-6f;
        if (fabs(mag2[0]) > EPSILON2 && fabs(mag2[0] - 1.0f) > EPSILON2)
        {
            //_mm_rsqrt_ps low precision issues...
            float32x4_t magInv = vset1(1.0f/sqrtf(mag2[0]));//_mm_rsqrt_ps( mag2 );
            return vmulq_f32( vec.array_neon, magInv );
        }
        return vec;
#else
        vec4 result = vec;
        if (vec == vec4(0)) return vec;
        //const float TOLERANCE = 1e-6f;
        // Don't normalize if we don't have to
        float mag2 = dot(vec, vec);
        if (fabs(mag2) > EPSILON2 && fabs(mag2 - 1.0f) > EPSILON2)
            result = (vec * (1.0f / sqrtf(mag2)));
        return result;
#endif
    }
    /// \brief Make que quaterniom from parameter to become a unity quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param q The quaternion to normalize
    /// \return The quaternion
    ///
    ARIBEIRO_INLINE quat normalize(const quat& q){
        quat result = q;
        //const float TOLERANCE = 1e-6f;
        // Don't normalize if we don't have to

        float mag2 = dot(q,q);//q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
        if (fabs(mag2) > EPSILON2 && fabs(mag2 - 1.0f) > EPSILON2) {
#if defined(ARIBEIRO_SSE2)
            //__m128 magInv = _mm_rsqrt_ps( _mm_set1_ps(mag2) );
            //_mm_rsqrt_ps low precision issues...
            __m128 magInv = _mm_set1_ps(1.0f/sqrtf(mag2));//_mm_rsqrt_ps( mag2 );
            result.array_sse = _mm_mul_ps( q.array_sse, magInv );
#elif defined(ARIBEIRO_NEON)
            float32x4_t magInv = vset1(1.0f/sqrtf(mag2));//_mm_rsqrt_ps( mag2 );
            result.array_neon = vmulq_f32( q.array_neon, magInv );

#else
            float magInv = 1.0f / sqrt(mag2);
            result.w = q.w * magInv;
            result.x = q.x * magInv;
            result.y = q.y * magInv;
            result.z = q.z * magInv;
#endif
        }

        /*
         float qmagsq = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
         if (std::abs(1.0f - qmagsq) < TOLERANCE) {
         float invMag = 2.0f / (1.0f + qmagsq);
         result.w = q.w * invMag;
         result.x = q.x * invMag;
         result.y = q.y * invMag;
         result.z = q.z * invMag;
         } else {
         float invMag = 1.0f / sqrt(qmagsq);
         result.w = q.w * invMag;
         result.x = q.x * invMag;
         result.y = q.y * invMag;
         result.z = q.z * invMag;
         }
         */

        return result;
    }

    /// \brief Extracts a quaternion from any matrix that have rotation information
    ///
    /// \author Alessandro Ribeiro
    /// \param m The transformation 4x4 matrix
    /// \return The quat
    ///
    ARIBEIRO_INLINE quat extractQuat(const mat4& mp){
        mat4 m = mp;

        //normalize rotation part
        m[0] = toVec4(normalize(toVec3(m[0])));
        m[1] = toVec4(normalize(toVec3(m[1])));
        m[2] = toVec4(normalize(toVec3(m[2])));


        float t = 1.0f + m._11 + m._22 + m._33;
        // large enough
        if (t > 0.001f) {
            float s = sqrt(t) * 2.0f;
            return quat((m._32 - m._23) / s,
                        (m._13 - m._31) / s,
                        (m._21 - m._12) / s,
                        0.25f * s);
        } // else we have to check several cases
        else if (m._11 > m._22 && m._11 > m._33) {
            // Column 0:
            float s = sqrt(1.0f + m._11 - m._22 - m._33) * 2.0f;
            return quat(0.25f * s,
                        (m._21 + m._12) / s,
                        (m._13 + m._31) / s,
                        (m._32 - m._23) / s);
        }
        else if (m._22 > m._33) {
            // Column 1:
            float s = sqrt(1.0f + m._22 - m._11 - m._33) * 2.0f;
            return quat((m._21 + m._12) / s,
                        0.25f * s,
                        (m._32 + m._23) / s,
                        (m._13 - m._31) / s);
        }
        else {
            // Column 2:
            float s = sqrt(1.0f + m._33 - m._11 - m._22) * 2.0f;
            return quat((m._13 + m._31) / s,
                        (m._32 + m._23) / s,
                        0.25f * s,
                        (m._21 - m._12) / s);
        }
    }


    /// \brief Operator overload to multiply a matrix 4x4 by a vector with 4 components
    ///
    /// <pre>
    /// [ 11 12 13 14 ]  [ x ]      [ x11+y12+z13+w14 ]
    /// [ 21 22 23 24 ]  [ y ]   =  [ x21+y22+z23+w24 ]
    /// [ 31 32 33 34 ]  [ z ]      [ x31+y32+z33+w34 ]
    /// [ 41 42 43 44 ]  [ w ]      [ x41+y42+z43+w44 ]
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param mat source matrix
    /// \param vec source vector
    /// \return A vec3 containing xyz from the parameter
    ///
    ARIBEIRO_INLINE vec4 operator*(const mat4 &mat, const vec4 &vec){
    #if defined(ARIBEIRO_SSE2)

        __m128 v0 = _mm_shuffle_ps(vec.array_sse, vec.array_sse, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 v1 = _mm_shuffle_ps(vec.array_sse, vec.array_sse, _MM_SHUFFLE(1, 1, 1, 1));
        __m128 v2 = _mm_shuffle_ps(vec.array_sse, vec.array_sse, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 v3 = _mm_shuffle_ps(vec.array_sse, vec.array_sse, _MM_SHUFFLE(3, 3, 3, 3));

        __m128 m0 = _mm_mul_ps(mat.array_sse[0], v0);
        __m128 m1 = _mm_mul_ps(mat.array_sse[1], v1);
        __m128 m2 = _mm_mul_ps(mat.array_sse[2], v2);
        __m128 m3 = _mm_mul_ps(mat.array_sse[3], v3);

        __m128 a0 = _mm_add_ps(m0, m1);
        __m128 a1 = _mm_add_ps(m2, m3);
        __m128 a2 = _mm_add_ps(a0, a1);

        return a2;
    #elif defined(ARIBEIRO_NEON)

        float32x4_t v0 = vshuffle_0000(vec.array_neon);
        float32x4_t v1 = vshuffle_1111(vec.array_neon);
        float32x4_t v2 = vshuffle_2222(vec.array_neon);
        float32x4_t v3 = vshuffle_3333(vec.array_neon);

        float32x4_t m0 = vmulq_f32(mat.array_neon[0], v0);
        float32x4_t m1 = vmulq_f32(mat.array_neon[1], v1);
        float32x4_t m2 = vmulq_f32(mat.array_neon[2], v2);
        float32x4_t m3 = vmulq_f32(mat.array_neon[3], v3);

        float32x4_t a0 = vaddq_f32(m0, m1);
        float32x4_t a1 = vaddq_f32(m2, m3);
        float32x4_t a2 = vaddq_f32(a0, a1);

        return a2;

    #else

        vec4 result;
        result.x = mat._11*vec.x + mat._12*vec.y + mat._13*vec.z + mat._14*vec.w;
        result.y = mat._21*vec.x + mat._22*vec.y + mat._23*vec.z + mat._24*vec.w;
        result.z = mat._31*vec.x + mat._32*vec.y + mat._33*vec.z + mat._34*vec.w;
        result.w = mat._41*vec.x + mat._42*vec.y + mat._43*vec.z + mat._44*vec.w;
        return result;

    #endif
    }

    /// \brief Operator overload to multiply a vector with 4 components by a matrix 4x4
    ///
    /// <pre>
    ///             [ 11 12 13 14 ]
    /// (x y z w)   [ 21 22 23 24 ]   =  (11x+21y+31z+41w  12x+22y+32z+42w  13x+23y+33z+43w  14x+24y+34z+44w)
    ///             [ 31 32 33 34 ]
    ///             [ 41 42 43 44 ]
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param vec source vector
    /// \param mat source matrix
    /// \return A vec3 containing xyz from the parameter
    ///
    ARIBEIRO_INLINE vec4 operator*(const vec4 &vec, const mat4 &mat){

    #if defined(ARIBEIRO_SSE2)

        __m128 m0 = _mm_mul_ps(vec.array_sse, mat.array_sse[0]);
        __m128 m1 = _mm_mul_ps(vec.array_sse, mat.array_sse[1]);
        __m128 m2 = _mm_mul_ps(vec.array_sse, mat.array_sse[2]);
        __m128 m3 = _mm_mul_ps(vec.array_sse, mat.array_sse[3]);

        __m128 u0 = _mm_unpacklo_ps(m0, m1);
        __m128 u1 = _mm_unpackhi_ps(m0, m1);
        __m128 a0 = _mm_add_ps(u0, u1);

        __m128 u2 = _mm_unpacklo_ps(m2, m3);
        __m128 u3 = _mm_unpackhi_ps(m2, m3);
        __m128 a1 = _mm_add_ps(u2, u3);

        __m128 f0 = _mm_movelh_ps(a0, a1);
        __m128 f1 = _mm_movehl_ps(a1, a0);
        __m128 f2 = _mm_add_ps(f0, f1);

        return f2;
    #elif defined(ARIBEIRO_NEON)
        float32x4_t a = vmulq_f32(vec.array_neon, mat.array_neon[0]);
        float32x4_t b = vmulq_f32(vec.array_neon, mat.array_neon[1]);
        float32x4_t c = vmulq_f32(vec.array_neon, mat.array_neon[2]);
        float32x4_t d = vmulq_f32(vec.array_neon, mat.array_neon[3]);

        //
        //transpose ops
        //
        float32x4x2_t ab = vtrnq_f32(a,b);
        float32x4x2_t cd = vtrnq_f32(c,d);
        float32x4_t a_=vcombine_f32(vget_low_f32(ab.val[0]),vget_low_f32(cd.val[0]));
        float32x4_t b_=vcombine_f32(vget_low_f32(ab.val[1]),vget_low_f32(cd.val[1]));
        float32x4_t c_=vcombine_f32(vget_high_f32(ab.val[0]),vget_high_f32(cd.val[0]));
        float32x4_t d_=vcombine_f32(vget_high_f32(ab.val[1]),vget_high_f32(cd.val[1]));

        float32x4_t add0 = vaddq_f32(a_,b_);
        float32x4_t add1 = vaddq_f32(c_,d_);

        float32x4_t add2 = vaddq_f32(add0,add1);

        return add2;
    #else

        vec4 result;
        result.x = mat._11*vec.x + mat._21*vec.y + mat._31*vec.z + mat._41*vec.w;
        result.y = mat._12*vec.x + mat._22*vec.y + mat._32*vec.z + mat._42*vec.w;
        result.z = mat._13*vec.x + mat._23*vec.y + mat._33*vec.z + mat._43*vec.w;
        result.w = mat._14*vec.x + mat._24*vec.y + mat._34*vec.z + mat._44*vec.w;
        return result;
    #endif
    }

    /// \brief Rotate quaternion a according quaternion b.
    ///
    /// The result is not normalized
    ///
    /// \author Alessandro Ribeiro
    /// \param a source quaternion
    /// \param b source quaternion
    /// \return a rotated according b
    ///
    ARIBEIRO_INLINE quat operator^(const quat &a, const quat &b){
#if defined(ARIBEIRO_SSE2)
        __m128 a0 = _mm_shuffle_ps(a.array_sse, a.array_sse, _MM_SHUFFLE(3,3,3,3));
        //__m128 b0 = _mm_shuffle_ps(b.array_sse, b.array_sse, _MM_SHUFFLE(3,2,1,0));

        __m128 a1 = _mm_shuffle_ps(a.array_sse, a.array_sse, _MM_SHUFFLE(0,2,1,0));
        __m128 b1 = _mm_shuffle_ps(b.array_sse, b.array_sse, _MM_SHUFFLE(0,3,3,3));

        __m128 a2 = _mm_shuffle_ps(a.array_sse, a.array_sse, _MM_SHUFFLE(1,0,2,1));
        __m128 b2 = _mm_shuffle_ps(b.array_sse, b.array_sse, _MM_SHUFFLE(1,1,0,2));

        __m128 a3 = _mm_shuffle_ps(a.array_sse, a.array_sse, _MM_SHUFFLE(2,1,0,2));
        __m128 b3 = _mm_shuffle_ps(b.array_sse, b.array_sse, _MM_SHUFFLE(2,0,2,1));

        static const __m128 signMask0 = _mm_load_(0.0f,0.0f,0.0f,-0.0f);
        //static const __m128 signMask0 = _mm_load_(1.0f,1.0f,1.0f,-1.0f);

        //__m128 mul0 = _mm_mul_ps(a0, b0);
        __m128 mul0 = _mm_mul_ps(a0, b.array_sse);

        __m128 mul1 = _mm_mul_ps(a1, b1);
        mul1 = _mm_xor_ps(mul1, signMask0 );//much faster
        //mul1 = _mm_mul_ps(mul1, signMask0);

        __m128 mul2 = _mm_mul_ps(a2, b2);
        mul2 = _mm_xor_ps(mul2, signMask0 );//much faster
        //mul2 = _mm_mul_ps(mul2, signMask0);

        __m128 mul3 = _mm_mul_ps(a3, b3);

        __m128 add0 = _mm_add_ps(mul0, mul1);
        add0 = _mm_add_ps(add0, mul2);
        add0 = _mm_sub_ps(add0, mul3);

        return add0;
#elif defined(ARIBEIRO_NEON)

        float32x4_t a0 = vshuffle_3333(a.array_neon);
        //__m128 b0 = _mm_shuffle_ps(b.array_sse, b.array_sse, _MM_SHUFFLE(3,2,1,0));

        float32x4_t a1 = vshuffle_0210(a.array_neon);
        float32x4_t b1 = vshuffle_0333(b.array_neon);

        float32x4_t a2 = vshuffle_1021(a.array_neon);
        float32x4_t b2 = vshuffle_1102(b.array_neon);

        float32x4_t a3 = vshuffle_2102(a.array_neon);
        float32x4_t b3 = vshuffle_2021(b.array_neon);

        static const float32x4_t signMask0 = (float32x4_t){1.0f,1.0f,1.0f,-1.0f};

        //__m128 mul0 = _mm_mul_ps(a0, b0);
        float32x4_t mul0 = vmulq_f32(a0, b.array_neon);

        float32x4_t mul1 = vmulq_f32(a1, b1);
        mul1 = vmulq_f32(mul1, signMask0);

        float32x4_t mul2 = vmulq_f32(a2, b2);
        mul2 = vmulq_f32(mul2, signMask0);

        float32x4_t mul3 = vmulq_f32(a3, b3);

        float32x4_t add0 = vaddq_f32(mul0, mul1);
        add0 = vaddq_f32(add0, mul2);
        add0 = vsubq_f32(add0, mul3);

        return add0;


#else

        return quat(
                    a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                    a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
                    a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
                    a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
                    ) ;

#endif
    }

    /// \brief Rotate quaternion a according quaternion b.
    ///
    /// The result will be normalized
    ///
    /// \author Alessandro Ribeiro
    /// \param a source quaternion
    /// \param b source quaternion
    /// \return a rotated according b
    ///
    ARIBEIRO_INLINE quat operator*(const quat &a, const quat &b){
        return normalize(a ^ b);
    }

    /// \brief Rotate vector according a quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param r source quaternion
    /// \param vec source vector
    /// \return vec rotated according r
    ///
    ARIBEIRO_INLINE vec3 operator*(const quat &a, const vec3 &v){
#if defined(ARIBEIRO_SSE2)
        //much faster
        quat result = quat(v.array_sse);
        _mm_f32_(result.array_sse, 3) = 0;
        result = a ^ result ^ conjugate(a);
        //static const __m128 _const_n = _mm_load_( 1,1,1,0 );
        //quat result = a ^ quat( _mm_mul_ps( v.array_sse, _const_n) ) ^ conjugate(a);
        return result.array_sse;
#elif defined(ARIBEIRO_NEON)

        static const float32x4_t _const_n = (float32x4_t){ 1,1,1,0 };

        quat result = a ^ quat( vmulq_f32(v.array_neon, _const_n ) ) ^ conjugate(a);
        return result.array_neon;
#else
        //quat result = mul(a, mul(quat(v.x, v.y, v.z, 0.0f), conjugate(a)));
        //
        // non normalized multiplication of the quaternion
        //
        quat result = a ^ quat(v.x, v.y, v.z, 0.0f) ^ conjugate(a);
        return vec3(result.x, result.y, result.z);
#endif
    }

    /// \brief Rotate vector according a quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param r source quaternion
    /// \param vec source vector
    /// \return vec rotated according r
    ///
    ARIBEIRO_INLINE vec4 operator*(const quat &a, const vec4 &v){
#if defined(ARIBEIRO_SSE2)
        /*
        static const __m128 _const_n = _mm_load_( 1,1,1,0 );
        static const __m128 _const2_n = _mm_load_( 0,0,0,1 );

        quat result = a ^ quat( _mm_mul_ps( v.array_sse, _const_n) ) ^ conjugate(a);

        return _mm_add_ps(
          _mm_mul_ps(result.array_sse, _const_n),
          _mm_mul_ps(v.array_sse, _const2_n)
        );
        */
        //much faster
        quat result = quat(v.array_sse);
        _mm_f32_(result.array_sse, 3) = 0;
        result = a ^ result ^ conjugate(a);
		_mm_f32_(result.array_sse,3) = _mm_f32_(v.array_sse,3);
        return result.array_sse;
#elif defined(ARIBEIRO_NEON)

        static const float32x4_t _const_n = (float32x4_t){ 1,1,1,0 };
        static const float32x4_t _const2_n = (float32x4_t){ 0,0,0,1 };

        quat result = a ^ quat( vmulq_f32(v.array_neon, _const_n ) ) ^ conjugate(a);

        return vaddq_f32(
                vmulq_f32(result.array_neon, _const_n),
                vmulq_f32(v.array_neon, _const2_n)
        );

		//_mm_f32_(result.array_sse,3) = v.w;

        //return result.array_neon;
#else
        //quat result = mul(a, mul(quat(v.x, v.y, v.z, 0.0f), conjugate(a)));
        //quat result = a * quat(v.x, v.y, v.z, 0.0f) * conjugate(a);
        //
        // non normalized multiplication of the quaternion
        //
        quat result = a ^ quat(v.x, v.y, v.z, 0.0f) ^ conjugate(a);
        return vec4(result.x, result.y, result.z, v.w);
#endif
    }


    /// \brief Compute the spherical linear interpolation between 2 quaternions.
    ///
    /// \author Alessandro Ribeiro
    /// \param q The quaternion
    /// \return The quaternion
    ///
    ARIBEIRO_INLINE quat slerp(const quat& a,const quat& b,const float lerp){
        /*
         if (lerp <= 0.0f) return a;
         if (lerp >= 1.0f) return b;

         // calc cosine theta
         float cosom = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

         // adjust signs (if necessary)
         quat end = b;
         if (cosom < 0.0f) {
         cosom = -cosom;
         end.x = -end.x;   // Reverse all signs
         end.y = -end.y;
         end.z = -end.z;
         end.w = -end.w;
         }

         // Calculate coefficients
         float sclp, sclq;
         if ((1.0f - cosom) > 0.0001f) { // 0.0001 -> some epsillon
         // Standard case (slerp)
         float omega, sinom;
         omega = acos(cosom); // extract theta from dot product's cos theta
         sinom = sin(omega);
         sclp = sin((1.0f - lerp) * omega) / sinom;
         sclq = sin(lerp * omega) / sinom;
         }
         else {
         // Very close, do linear interp (because it's faster)
         sclp = 1.0f - lerp;
         sclq = lerp;
         }
         return quat(sclp * a.x + sclq * end.x,
         sclp * a.y + sclq * end.y,
         sclp * a.z + sclq * end.z,
         sclp * a.w + sclq * end.w);
         */
        //
        // https://github.com/g-truc/glm
        //
        if (lerp <= 0.0f) return a;
        if (lerp >= 1.0f) return b;

        //float fCos = dot(a, b);
        float fCos = dot(a,b);//a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        quat y2(b); //BUG!!! qua<T> y2;
        if (fCos < 0.0f)
        {
            y2 = -b;
            //y2 = quat(-b.x, -b.y, -b.z, -b.w);
            fCos = -fCos;
        }

        //if(fCos > 1.0f) // problem
        float k0, k1;
        //const float EPSILON = 1e-6f;
        if (fCos > (1.0f - EPSILON))
        {
            k0 = 1.0f - lerp;
            k1 = 0.0f + lerp; //BUG!!! 1.0f + lerp;
        }
        else
        {
            float fSin = sqrt( 1.0f - fCos * fCos );
            float fAngle = atan2(fSin, fCos);
            float fOneOverSin = 1.0f / fSin;
            k0 = sin((1.0f - lerp) * fAngle) * fOneOverSin;
            k1 = sin((0.0f + lerp) * fAngle) * fOneOverSin;
        }
#if defined(ARIBEIRO_SSE2)
        __m128 aResult = _mm_mul_ps(a.array_sse, _mm_set1_ps(k0) );
        __m128 y2Result = _mm_mul_ps(y2.array_sse, _mm_set1_ps(k1) );
        return normalize(quat(_mm_add_ps(aResult, y2Result)));
#elif defined(ARIBEIRO_NEON)
        float32x4_t aResult = vmulq_f32(a.array_neon, vset1(k0) );
        float32x4_t y2Result = vmulq_f32(y2.array_neon, vset1(k1) );
        return normalize(quat(vaddq_f32(aResult, y2Result)));
#else
        return normalize(quat(
                              k0 * a.x + k1 * y2.x,
                              k0 * a.y + k1 * y2.y,
                              k0 * a.z + k1 * y2.z,
                              k0 * a.w + k1 * y2.w
                              ));
#endif
    }


    /// \brief Compute the spherical linear interpolation between 2 vec3
    ///
    /// \author Alessandro Ribeiro
    /// \param a The source vector
    /// \param a The target vector
    /// \return The quaternion
    ///
    ARIBEIRO_INLINE vec3 slerp(const vec3& a, const vec3& b, const float &lerp){

        //
        // https://github.com/g-truc/glm
        //
        // get cosine of angle between vectors (-1 -> 1)
        float CosAlpha = clamp(dot( normalize(a), normalize(b) ), -1.0f, 1.0f);
        // get angle (0 -> pi)
        float Alpha = acos(CosAlpha);
        // get sine of angle between vectors (0 -> 1)
        float SinAlpha = sin(Alpha);

        // simply perform linear interpolation if sinAlpha = 0
        if (absv(SinAlpha) < 1e-6f )
            return a + (b - a) * lerp;

        // this breaks down when SinAlpha = 0, i.e. Alpha = 0 or pi
        float t1 = sin( (1.0f - lerp) * Alpha ) / SinAlpha;
        float t2 = sin(lerp * Alpha) / SinAlpha;

        // interpolate src vectors
        return a * t1 + b * t2;
    }




    /// \brief Compute the angle between two vectors
    ///
    /// \author Alessandro Ribeiro
    /// \param a The source vector
    /// \param a The target vector
    /// \return The quaternion
    ///
    ARIBEIRO_INLINE float angleBetween(const vec3& a, const vec3& b) {
#if defined(ARIBEIRO_SSE2)
        static const __m128 _minus_one = _mm_set1_ps(-1.0f);
        static const __m128 _one = _mm_set1_ps(1.0f);

        __m128 dot0 = dot_sse_3( normalize(a).array_sse, normalize(b).array_sse );
        __m128 cosA = clamp_sse_4(dot0, _minus_one, _one);
        return acos(_mm_f32_(cosA,0));
#elif defined(ARIBEIRO_NEON)
        static const float32x4_t _minus_one = vset1(-1.0f);
        static const float32x4_t _one = vset1(1.0f);

        float32x4_t dot0 = dot_neon_3( normalize(a).array_neon, normalize(b).array_neon );
        float32x4_t cosA = clamp_neon_4(dot0, _minus_one, _one);
        return acos(cosA[0]);
#else
        float cosA = clamp(dot(normalize(a), normalize(b)),-1.0f,1.0f);
        return acos(cosA);
#endif
    }

    /// \brief Computes the angle between two quaternions
    ///
    ///
    /// \author Alessandro Ribeiro
    /// \sa angleBetween(const quat& a, const quat& b)
    /// \param a The first quat
    /// \param b The second quat
    /// \return The dot product between the two quaternions
    ///
    ARIBEIRO_INLINE float angleBetween(const quat& a, const quat& b){
#if defined(ARIBEIRO_SSE2)
        static const __m128 _minus_one = _mm_set1_ps(-1.0f);
        static const __m128 _one = _mm_set1_ps(1.0f);

        __m128 dot0 = dot_sse_4( normalize(a).array_sse, normalize(b).array_sse );
        __m128 cosA = clamp_sse_4(dot0, _minus_one, _one);
        return acos(_mm_f32_(cosA,0)) * 2.0f;
#elif defined(ARIBEIRO_NEON)
        static const float32x4_t _minus_one = vset1(-1.0f);
        static const float32x4_t _one = vset1(1.0f);

        float32x4_t dot0 = dot_neon_4( normalize(a).array_neon, normalize(b).array_neon );
        float32x4_t cosA = clamp_neon_4(dot0, _minus_one, _one);
        return acos(cosA[0]) * 2.0f;
#else
        return acos(
                    clamp(dot(normalize(a), normalize(b)),-1.0f,1.0f)
                    ) * 2.0f;
        //return acos( dot(normalize(vec3(a.x, a.y, a.z)), normalize(vec3(b.x, b.y, b.z))) );
#endif
    }








    //------------------------------------------------------------------------------
    /// \brief Computes the cross product between two vectors
    ///
    /// The cross product is an orthogonal vector to the others two vectores, i. e. the vector have 90 degrees to each vector at the same time.
    ///
    /// The side of the vector is defined by the right hand rule.
    ///
    /// <pre>
    ///     the first vector (a)
    ///    |
    ///    |____this is the cross result
    ///   /
    ///  /
    ///  the second vector (b)
    ///
    ///     the second vector (b)
    ///    |
    ///    |____the first vector (a)
    ///   /
    ///  /
    ///  this is the cross result
    ///
    ///     ____the first vector (a)
    ///   /|
    ///  / |
    ///  the second vector (b)
    ///    |
    ///     this is the cross result
    /// </pre>
    ///
    /// The length of the cross product is the same as:
    ///
    /// |sen(a)|*|a|*|b|, a = angle between the two vectors
    ///
    /// \author Alessandro Ribeiro
    /// \param a The first vector
    /// \param b The second vector
    /// \return The cross product between the two vectors
    ///
    ARIBEIRO_INLINE vec3 cross( const vec3& a, const vec3& b ){
#if defined(ARIBEIRO_SSE2)
        __m128 swp0 = _mm_shuffle_ps(a.array_sse, a.array_sse, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 swp1 = _mm_shuffle_ps(a.array_sse, a.array_sse, _MM_SHUFFLE(3, 1, 0, 2));
        __m128 swp2 = _mm_shuffle_ps(b.array_sse, b.array_sse, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 swp3 = _mm_shuffle_ps(b.array_sse, b.array_sse, _MM_SHUFFLE(3, 1, 0, 2));
        __m128 mul0 = _mm_mul_ps(swp0, swp3);
        __m128 mul1 = _mm_mul_ps(swp1, swp2);
        __m128 sub0 = _mm_sub_ps(mul0, mul1);
        return sub0;
#elif defined(ARIBEIRO_NEON)
        float32x4_t swp0 = vshuffle_3021(a.array_neon);
        float32x4_t swp1 = vshuffle_3102(a.array_neon);
        float32x4_t swp2 = vshuffle_3021(b.array_neon);
        float32x4_t swp3 = vshuffle_3102(b.array_neon);
        float32x4_t mul0 = vmulq_f32(swp0, swp3);
        float32x4_t mul1 = vmulq_f32(swp1, swp2);
        float32x4_t sub0 = vsubq_f32(mul0, mul1);
        return sub0;
#else
        return vec3((a.y * b.z - a.z * b.y),
                    (a.z * b.x - a.x * b.z),
                    (a.x * b.y - a.y * b.x));
#endif
    }


    //------------------------------------------------------------------------------
    /// \brief Computes the reflected vector 'a' related to a normal N
    ///
    /// The reflection of a vector is another vector with the same length, but the direction is
    /// modified by the surface normal (after hit the surface).
    ///
    /// <pre>
    ///          \  |
    ///           \ |w
    ///           a\|a
    /// Normal <----|l
    ///            /|l
    ///           / |
    ///          /  |
    /// reflected   |
    ///
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The incident vector
    /// \param N The normal of a surface (unit vector)
    /// \return The reflected vector 'a' considering the normal N
    ///
    ARIBEIRO_INLINE vec2 reflect( const vec2& a, const vec2& N ){
        return (a - N * (2.0f * dot(a, N)));
    }
    /// \brief Computes the reflected vector 'a' related to a normal N
    ///
    /// For a more detailed description, see reflect( const vec2& a, const vec2& N )
    ///
    /// \author Alessandro Ribeiro
    /// \sa reflect( const vec2& a, const vec2& N )
    /// \param a The incident vector
    /// \param N The normal of a surface (unit vector)
    /// \return The reflected vector 'a' considering the normal N
    ///
    ARIBEIRO_INLINE vec3 reflect( const vec3& a, const vec3& N ){
#if defined(ARIBEIRO_SSE2)
        static const __m128 _two = _mm_set1_ps(2.0f);

        __m128 dt = dot_sse_3(a.array_sse, N.array_sse);
        __m128 mul0 = _mm_mul_ps(dt, N.array_sse);
        __m128 mul1 = _mm_mul_ps(mul0, _two);
        return _mm_sub_ps(a.array_sse, mul1);
#elif defined(ARIBEIRO_NEON)
        static const float32x4_t _two = vset1(2.0f);
        float32x4_t dt = dot_neon_3(a.array_neon, N.array_neon);
        float32x4_t mul0 = vmulq_f32(dt, N.array_neon);
        float32x4_t mul1 = vmulq_f32(mul0, _two);
        return vsubq_f32(a.array_neon, mul1);
#else
        return (a - N * (2.0f * dot(a, N)));
#endif
    }
    /// \brief Computes the reflected vector 'a' related to a normal N
    ///
    /// For a more detailed description, see reflect( const vec2& a, const vec2& N )
    ///
    /// \author Alessandro Ribeiro
    /// \sa reflect( const vec2& a, const vec2& N )
    /// \param a The incident vector
    /// \param N The normal of a surface (unit vector)
    /// \return The reflected vector 'a' considering the normal N
    ///
    ARIBEIRO_INLINE vec4 reflect( const vec4& a, const vec4& N ){
#if defined(ARIBEIRO_SSE2)
        static const __m128 _two = _mm_set1_ps(2.0f);

        __m128 dt = dot_sse_4(a.array_sse, N.array_sse);
        __m128 mul0 = _mm_mul_ps(dt, N.array_sse);
        __m128 mul1 = _mm_mul_ps(mul0, _two);
        return _mm_sub_ps(a.array_sse, mul1);
#elif defined(ARIBEIRO_NEON)
        static const float32x4_t _two = vset1(2.0f);
        float32x4_t dt = dot_neon_4(a.array_neon, N.array_neon);
        float32x4_t mul0 = vmulq_f32(dt, N.array_neon);
        float32x4_t mul1 = vmulq_f32(mul0, _two);
        return vsubq_f32(a.array_neon, mul1);
#else
        return (a - N * (2.0f * dot(a, N)));
#endif
    }

    /// \brief snell law refraction, vector implementation
    ///
    /// from input ray, normal, ni and nr calculate the refracted vector
    /// ni = source index of refraction
    /// nr = target index of refraction
    ///
    /// \author Alessandro Ribeiro
    /// \sa refract( const vec3 &ray,const vec3 &normal, float ni, float nr, vec3 *vOut )
    /// \param rayDir Incident ray direction
    /// \param normal The normal of a surface (unit vector)
    /// \param ni source index of refraction
    /// \param nr target index of refraction
    /// \param vOut output vector
    /// \return true if vector is calculated, false if this is a total internal reflection case
    ARIBEIRO_INLINE bool refract(const vec3 &rayDir,const vec3 &normal, const float &ni, const float &nr, vec3 *vOut ){

        vec3 L = normalize(-rayDir);

        float ni_nr = ni/nr;
        float cos_i = dot( normal, L );

        float cos_r = 1.0f - ni_nr*ni_nr*( 1.0f - cos_i*cos_i );

        if (cos_r <=0)
            return false;

        cos_r = sqrt(cos_r);

        vec3 T = (ni_nr*cos_i - cos_r) * normal - ni_nr * L;

        *vOut = normalize( T );
        return true;
    }

    //------------------------------------------------------------------------------

    /// \brief Compute the squared length of the quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param q The quaternion
    /// \return The squared length
    ///
    ARIBEIRO_INLINE float sqrLength(const quat& q){
        //return q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
        return dot(q,q);
    }

    /// \brief Computes the squared length of a vector
    ///
    /// The squared length of a vector 'a' is:
    ///
    /// |a|^2
    ///
    /// It is cheaper to compute this value than the length of 'a'.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The Vector
    /// \return The squared length
    ///
    ARIBEIRO_INLINE float sqrLength(const vec2 &a){
        return dot(a, a);
    }
    /// \brief Computes the squared length of a vector
    ///
    /// The squared length of a vector 'a' is:
    ///
    /// |a|^2
    ///
    /// It is cheaper to compute this value than the length of 'a'.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The Vector
    /// \return The squared length
    ///
    ARIBEIRO_INLINE float sqrLength(const vec3 &a){
        return dot(a, a);
    }
    /// \brief Computes the squared length of a vector
    ///
    /// The squared length of a vector 'a' is:
    ///
    /// |a|^2
    ///
    /// It is cheaper to compute this value than the length of 'a'.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The Vector
    /// \return The squared length
    ///
    ARIBEIRO_INLINE float sqrLength(const vec4 &a){
        return dot(a, a);
    }
    //------------------------------------------------------------------------------



    /// \brief Compute the length of the quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param q The quaternion
    /// \return The length
    ///
    ARIBEIRO_INLINE float length(const quat& q){
        return sqrtf(dot(q, q));
    }


    /// \brief Computes the length of a vector
    ///
    /// The length of a vector 'a' is:
    ///
    /// |a|
    ///
    /// This computation uses the sqrtf, and it consumes a lot of cicles to compute.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The Vector
    /// \return The length
    ///
    ARIBEIRO_INLINE float length(const vec2 &a){
        return sqrtf(dot(a, a));
    }
    /// \brief Computes the length of a vector
    ///
    /// The length of a vector 'a' is:
    ///
    /// |a|
    ///
    /// This computation uses the sqrtf, and it consumes a lot of cicles to compute.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The Vector
    /// \return The length
    ///
    ARIBEIRO_INLINE float length(const vec3 &a){
        return sqrtf(dot(a, a));
    }
    /// \brief Computes the length of a vector
    ///
    /// The length of a vector 'a' is:
    ///
    /// |a|
    ///
    /// This computation uses the sqrtf, and it consumes a lot of cicles to compute.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The Vector
    /// \return The length
    ///
    ARIBEIRO_INLINE float length(const vec4 &a){
        return sqrtf(dot(a, a));
    }
    //------------------------------------------------------------------------------
    /// \brief Computes the squared distance between two vectors
    ///
    /// The squared distance is the euclidian distance, without the square root:
    ///
    /// |b-a|^2
    ///
    /// It is cheaper to compute this value than the distance from 'a' to 'b'.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The First vector
    /// \param b The Second vector
    /// \return The squared distance between a and b
    ///
    //ARIBEIRO_API float sqrDistance(const float a,const float b);
    /// \brief Computes the squared distance between two vectors
    ///
    /// The squared distance is the euclidian distance, without the square root:
    ///
    /// |b-a|^2
    ///
    /// It is cheaper to compute this value than the distance from 'a' to 'b'.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The First vector
    /// \param b The Second vector
    /// \return The squared distance between a and b
    ///
    ARIBEIRO_INLINE float sqrDistance(const vec2 &a, const vec2 &b){
        vec2 ab = b - a;
        return dot(ab, ab);
    }
    /// \brief Computes the squared distance between two vectors
    ///
    /// The squared distance is the euclidian distance, without the square root:
    ///
    /// |b-a|^2
    ///
    /// It is cheaper to compute this value than the distance from 'a' to 'b'.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The First vector
    /// \param b The Second vector
    /// \return The squared distance between a and b
    ///
    ARIBEIRO_INLINE float sqrDistance(const vec4 &a, const vec4 &b){
        vec4 ab = b - a;
        return dot(ab, ab);
    }
    /// \brief Computes the squared distance between two vectors
    ///
    /// The squared distance is the euclidian distance, without the square root:
    ///
    /// |b-a|^2
    ///
    /// It is cheaper to compute this value than the distance from 'a' to 'b'.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The First vector
    /// \param b The Second vector
    /// \return The squared distance between a and b
    ///
    ARIBEIRO_INLINE float sqrDistance(const vec3 &a, const vec3 &b){
        vec3 ab = b - a;
        return dot(ab, ab);
    }
    //------------------------------------------------------------------------------
    /// \brief Computes the distance between two vectors
    ///
    /// The squared distance is the euclidian distance:
    ///
    /// |b-a|
    ///
    /// This computation uses the sqrtf, and it consumes a lot of cicles to compute.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The First vector
    /// \param b The Second vector
    /// \return The distance between a and b
    ///
    //ARIBEIRO_API float distance(const float a,const float b);
    /// \brief Computes the distance between two vectors
    ///
    /// The squared distance is the euclidian distance:
    ///
    /// |b-a|
    ///
    /// This computation uses the sqrtf, and it consumes a lot of cicles to compute.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The First vector
    /// \param b The Second vector
    /// \return The distance between a and b
    ///
    ARIBEIRO_INLINE float distance(const vec2 &a, const vec2 &b){
        vec2 ab = b - a;
        return sqrtf( dot(ab, ab) );
    }
    /// \brief Computes the distance between two vectors
    ///
    /// The squared distance is the euclidian distance:
    ///
    /// |b-a|
    ///
    /// This computation uses the sqrtf, and it consumes a lot of cicles to compute.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The First vector
    /// \param b The Second vector
    /// \return The distance between a and b
    ///
    ARIBEIRO_INLINE float distance(const vec4 &a, const vec4 &b){
        vec4 ab = b - a;
        return sqrtf( dot(ab, ab) );
    }
    /// \brief Computes the distance between two vectors
    ///
    /// The squared distance is the euclidian distance:
    ///
    /// |b-a|
    ///
    /// This computation uses the sqrtf, and it consumes a lot of cicles to compute.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The First vector
    /// \param b The Second vector
    /// \return The distance between a and b
    ///
    ARIBEIRO_INLINE float distance(const vec3 &a, const vec3 &b){
        vec3 ab = b - a;
        return sqrtf( dot(ab, ab) );
    }

    //------------------------------------------------------------------------------
    /// \brief Computes the projection of a vector over a unit vector
    ///
    /// The projection result is a vector parallel to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /                 |
    ///     a  /      | unitV     | vOut
    ///       o       o           o
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \return The projection of 'a' over unitV
    ///
    ARIBEIRO_INLINE vec2 parallelComponent(const vec2 &a, const vec2 &unitV){
        return unitV * (dot(a, unitV));//dot(a,unitV) É a projeção de a em unitV
    }
    /// \brief Computes the projection of a vector over a unit vector
    ///
    /// The projection result is a vector parallel to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /                 |
    ///     a  /      | unitV     | vOut
    ///       o       o           o
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \return The projection of 'a' over unitV
    ///
    ARIBEIRO_INLINE vec3 parallelComponent(const vec3 &a, const vec3 &unitV){
#if defined(ARIBEIRO_SSE2)
        __m128 dot0 = dot_sse_3(a.array_sse,unitV.array_sse);
        __m128 mul0 = _mm_mul_ps(unitV.array_sse, dot0);
        return mul0;
#elif defined(ARIBEIRO_NEON)
        float32x4_t dot0 = dot_neon_3(a.array_neon,unitV.array_neon);
        float32x4_t mul0 = vmulq_f32(unitV.array_neon, dot0);
        return mul0;
#else
        return unitV * (dot(a, unitV));//dot(a,unitV) É a projeção de a em unitV
#endif
    }
    /// \brief Computes the projection of a vector over a unit vector
    ///
    /// The projection result is a vector parallel to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /                 |
    ///     a  /      | unitV     | vOut
    ///       o       o           o
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \return The projection of 'a' over unitV
    ///
    ARIBEIRO_INLINE vec4 parallelComponent(const vec4 &a, const vec4 &unitV){
#if defined(ARIBEIRO_SSE2)
        __m128 dot0 = dot_sse_4(a.array_sse,unitV.array_sse);
        __m128 mul0 = _mm_mul_ps(unitV.array_sse, dot0);
        return mul0;
#elif defined(ARIBEIRO_NEON)
        float32x4_t dot0 = dot_neon_4(a.array_neon,unitV.array_neon);
        float32x4_t mul0 = vmulq_f32(unitV.array_neon, dot0);
        return mul0;
#else
        return unitV * (dot(a, unitV));//dot(a,unitV) É a projeção de a em unitV
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Computes a vector perpendicular to the projection of a vector over a unit vector
    ///
    /// The vector perpendicular to the projection result is a vector normal to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /
    ///     a  /      | unitV
    ///       o       o           o-- vOut
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \return The normal to the projection of 'a' over unitV
    ///
    ARIBEIRO_INLINE vec2 perpendicularComponent(const vec2 &a, const vec2 &unitV){
        return a - unitV * (dot(a, unitV)); //unitV*(dot(a,unitV)) É o componenete paralelo
    }

    /// \brief Computes a vector perpendicular to the projection of a vector over a unit vector
    ///
    /// The vector perpendicular to the projection result is a vector normal to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /
    ///     a  /      | unitV
    ///       o       o           o-- vOut
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \return The normal to the projection of 'a' over unitV
    ///
    ARIBEIRO_INLINE vec3 perpendicularComponent(const vec3 &a, const vec3 &unitV){
#if defined(ARIBEIRO_SSE2)
        __m128 dot0 = dot_sse_3(a.array_sse,unitV.array_sse);
        __m128 mul0 = _mm_mul_ps(unitV.array_sse, dot0);
        __m128 sub = _mm_sub_ps(a.array_sse, mul0);
        return sub;
#elif defined(ARIBEIRO_NEON)
        float32x4_t dot0 = dot_neon_3(a.array_neon,unitV.array_neon);
        float32x4_t mul0 = vmulq_f32(unitV.array_neon, dot0);
        float32x4_t sub = vsubq_f32(a.array_neon, mul0);
        return sub;
#else
        return a - unitV * (dot(a, unitV)); //unitV*(dot(a,unitV)) É o componenete paralelo
#endif
    }

    /// \brief Computes a vector perpendicular to the projection of a vector over a unit vector
    ///
    /// The vector perpendicular to the projection result is a vector normal to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /
    ///     a  /      | unitV
    ///       o       o           o-- vOut
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \return The normal to the projection of 'a' over unitV
    ///
    ARIBEIRO_INLINE vec4 perpendicularComponent(const vec4 &a, const vec4 &unitV){
#if defined(ARIBEIRO_SSE2)
        __m128 dot0 = dot_sse_4(a.array_sse,unitV.array_sse);
        __m128 mul0 = _mm_mul_ps(unitV.array_sse, dot0);
        __m128 sub = _mm_sub_ps(a.array_sse, mul0);
        return sub;
#elif defined(ARIBEIRO_NEON)
        float32x4_t dot0 = dot_neon_4(a.array_neon,unitV.array_neon);
        float32x4_t mul0 = vmulq_f32(unitV.array_neon, dot0);
        float32x4_t sub = vsubq_f32(a.array_neon, mul0);
        return sub;
#else
        return a - unitV * (dot(a, unitV)); //unitV*(dot(a,unitV)) É o componenete paralelo
#endif
    }

    //------------------------------------------------------------------------------
    /// \brief Computes both: a vector perpendicular and a parallel to the projection of a vector over a unit vector
    ///
    /// The vector perpendicular to the projection result is a vector normal to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /
    ///     a  /      | unitV
    ///       o       o           o-- perpendicular
    /// </pre>
    ///
    /// The projection result is a vector parallel to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /                 |
    ///     a  /      | unitV     | parallel
    ///       o       o           o
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \param perpendicular It is a return parameter, thats will hold the computed perpendicular vector
    /// \param parallel It is a return parameter, thats will hold the computed parallel vector
    ///
    ARIBEIRO_INLINE void vecDecomp(const vec2 &a, const vec2 &unitV,
                                vec2 *perpendicular, vec2 *parallel){
        *parallel = unitV * (dot(a, unitV));
        *perpendicular = a - *parallel;
    }

    /// \brief Computes both: a vector perpendicular and a parallel to the projection of a vector over a unit vector
    ///
    /// The vector perpendicular to the projection result is a vector normal to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /
    ///     a  /      | unitV
    ///       o       o           o-- perpendicular
    /// </pre>
    ///
    /// The projection result is a vector parallel to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /                 |
    ///     a  /      | unitV     | parallel
    ///       o       o           o
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \param perpendicular It is a return parameter, thats will hold the computed perpendicular vector
    /// \param parallel It is a return parameter, thats will hold the computed parallel vector
    ///
    ARIBEIRO_INLINE void vecDecomp(const vec3 &a, const vec3 &unitV,
                                vec3 *perpendicular, vec3 *parallel){
#if defined(ARIBEIRO_SSE2)
        __m128 dot0 = dot_sse_3(a.array_sse,unitV.array_sse);
        __m128 mul0 = _mm_mul_ps(unitV.array_sse, dot0);
        __m128 sub = _mm_sub_ps(a.array_sse, mul0);

        parallel->array_sse = mul0;
        perpendicular->array_sse = sub;
#elif defined(ARIBEIRO_NEON)
        float32x4_t dot0 = dot_neon_3(a.array_neon,unitV.array_neon);
        float32x4_t mul0 = vmulq_f32(unitV.array_neon, dot0);
        float32x4_t sub = vsubq_f32(a.array_neon, mul0);

        parallel->array_neon = mul0;
        perpendicular->array_neon = sub;

#else
        *parallel = unitV * (dot(a, unitV));
        *perpendicular = a - *parallel;
#endif
    }

    /// \brief Computes both: a vector perpendicular and a parallel to the projection of a vector over a unit vector
    ///
    /// The vector perpendicular to the projection result is a vector normal to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /
    ///     a  /      | unitV
    ///       o       o           o-- perpendicular
    /// </pre>
    ///
    /// The projection result is a vector parallel to the unitV
    ///
    /// <pre>
    /// ex.:
    ///         /                 |
    ///     a  /      | unitV     | parallel
    ///       o       o           o
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param a The vector to decompose
    /// \param unitV The base vector (must be unit)
    /// \param perpendicular It is a return parameter, thats will hold the computed perpendicular vector
    /// \param parallel It is a return parameter, thats will hold the computed parallel vector
    ///
    ARIBEIRO_INLINE void vecDecomp(const vec4 &a, const vec4 &unitV,
                                vec4 *perpendicular, vec4 *parallel){
#if defined(ARIBEIRO_SSE2)
        __m128 dot0 = dot_sse_4(a.array_sse,unitV.array_sse);
        __m128 mul0 = _mm_mul_ps(unitV.array_sse, dot0);
        __m128 sub = _mm_sub_ps(a.array_sse, mul0);

        parallel->array_sse = mul0;
        perpendicular->array_sse = sub;
#elif defined(ARIBEIRO_NEON)
        float32x4_t dot0 = dot_neon_4(a.array_neon,unitV.array_neon);
        float32x4_t mul0 = vmulq_f32(unitV.array_neon, dot0);
        float32x4_t sub = vsubq_f32(a.array_neon, mul0);

        parallel->array_neon = mul0;
        perpendicular->array_neon = sub;

#else
        *parallel = unitV * (dot(a, unitV));
        *perpendicular = a - *parallel;
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Test a point and clips its values according the euclidian distance from another point
    ///
    /// The quadradic clamp can be used to make limits like circular limits or spherical limits.
    ///
    /// \author Alessandro Ribeiro
    /// \param point The point to clip
    /// \param center The center to compute the euclidian distance
    /// \param maxRadius The max distance that the point can be from the center
    /// \return The point if it is below the distance or a point in the same line of the point, but with distance from center equals to maxRadius
    ///
    ARIBEIRO_INLINE vec2 quadraticClamp(const vec2 &point, const vec2 &center,const float maxRadius){
        vec2 direction = point - center;
        float length = sqrtf(sqrLength(direction));
        if (length > maxRadius)
            return center + direction * (maxRadius / length);
        return point;
    }
    /// \brief Test a point and clips its values according the euclidian distance from another point
    ///
    /// The quadradic clamp can be used to make limits like circular limits or spherical limits.
    ///
    /// \author Alessandro Ribeiro
    /// \param point The point to clip
    /// \param center The center to compute the euclidian distance
    /// \param maxRadius The max distance that the point can be from the center
    /// \return The point if it is below the distance or a point in the same line of the point, but with distance from center equals to maxRadius
    ///
    ARIBEIRO_INLINE vec3 quadraticClamp(const vec3 &point, const vec3 &center,const float maxRadius){
        vec3 direction = point - center;
        float length = sqrtf(sqrLength(direction));
        if (length > maxRadius)
            return center + direction * (maxRadius / length);
        return point;
    }
    /// \brief Test a point and clips its values according the euclidian distance from another point
    ///
    /// The quadradic clamp can be used to make limits like circular limits or spherical limits.
    ///
    /// \author Alessandro Ribeiro
    /// \param point The point to clip
    /// \param center The center to compute the euclidian distance
    /// \param maxRadius The max distance that the point can be from the center
    /// \return The point if it is below the distance or a point in the same line of the point, but with distance from center equals to maxRadius
    ///
    ARIBEIRO_INLINE vec4 quadraticClamp(const vec4 &point, const vec4 &center,const float maxRadius){
        vec4 direction = point - center;
        float length = sqrtf(sqrLength(direction));
        if (length > maxRadius)
            return center + direction * (maxRadius / length);
        return point;
    }
    //------------------------------------------------------------------------------
    /// \brief Return the greater value from the parameter
    ///
    /// \author Alessandro Ribeiro
    /// \param a Set of values to test
    /// \return The greater value from the parameter
    ///
    ARIBEIRO_INLINE float maximum(const vec2 &a){
        return (a.x > a.y) ? (a.x) : (a.y);
    }

    /// \brief Return the greater value from the parameter
    ///
    /// \author Alessandro Ribeiro
    /// \param a Set of values to test
    /// \return The greater value from the parameter
    ///
    ARIBEIRO_INLINE float maximum(const vec3 &a){
        return (a.x > a.y) ? ((a.x > a.z) ? (a.x) : (a.z)) : (a.y);
    }

    /// \brief Return the greater value from the parameter
    ///
    /// \author Alessandro Ribeiro
    /// \param a Set of values to test
    /// \return The greater value from the parameter
    ///
    ARIBEIRO_INLINE float maximum(const vec4 &a){
        return (a.x > a.y) ? ((a.x > a.z) ? ((a.x > a.w) ? (a.x) : (a.w)) : (a.z)) : (a.y);
    }

    /// \brief Return the maximum between the two parameters
    ///
    /// \author Alessandro Ribeiro
    /// \param a A value to test
    /// \param b A value to test
    /// \return The maximum value of the parameters
    ///
    //ARIBEIRO_API float maximum(const float a,const float b);
    /// \brief Component-wise maximum value from two vectors
    ///
    ///  Return the maximum value considering each component of the vector.
    ///
    /// ex.: vec2( maximum(a.x,b.x), maximum(a.y,b.y) )
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \param b A vector
    /// \return The maximum value for each vector component
    ///
    ARIBEIRO_INLINE vec2 maximum(const vec2 &a, const vec2 &b){
#if defined(ARIBEIRO_SSE2)
        return _mm_max_ps(a.array_sse, b.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vmaxq_f32(a.array_neon, b.array_neon);
#else
        return vec2((a.x > b.x) ? a.x : b.x,
                    (a.y > b.y) ? a.y : b.y);
#endif
    }

    /// \brief Component-wise maximum value from two vectors
    ///
    ///  Return the maximum value considering each component of the vector.
    ///
    /// ex.: vec2( maximum(a.x,b.x), maximum(a.y,b.y) )
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \param b A vector
    /// \return The maximum value for each vector component
    ///
    ARIBEIRO_INLINE vec3 maximum(const vec3 &a, const vec3 &b){
#if defined(ARIBEIRO_SSE2)
        return _mm_max_ps(a.array_sse, b.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vmaxq_f32(a.array_neon, b.array_neon);
#else
        return vec3((a.x > b.x) ? a.x : b.x,
                    (a.y > b.y) ? a.y : b.y,
                    (a.z > b.z) ? a.z : b.z);
#endif
    }

    /// \brief Component-wise maximum value from two vectors
    ///
    ///  Return the maximum value considering each component of the vector.
    ///
    /// ex.: vec2( maximum(a.x,b.x), maximum(a.y,b.y) )
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \param b A vector
    /// \return The maximum value for each vector component
    ///
    ARIBEIRO_INLINE vec4 maximum(const vec4 &a, const vec4 &b){
#if defined(ARIBEIRO_SSE2)
        return _mm_max_ps(a.array_sse, b.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vmaxq_f32(a.array_neon, b.array_neon);
#else
        return vec4((a.x > b.x) ? a.x : b.x,
                    (a.y > b.y) ? a.y : b.y,
                    (a.z > b.z) ? a.z : b.z,
                    (a.w > b.w) ? a.w : b.w);
#endif
    }

    /// \brief Return the smaller value from the parameter
    ///
    /// \author Alessandro Ribeiro
    /// \param a Set of values to test
    /// \return The smaller value from the parameter
    ///
    ARIBEIRO_INLINE float minimum(const vec2 &a){
        return (a.x < a.y) ? (a.x) : (a.y);
    }

    /// \brief Return the smaller value from the parameter
    ///
    /// \author Alessandro Ribeiro
    /// \param a Set of values to test
    /// \return The smaller value from the parameter
    ///
    ARIBEIRO_INLINE float minimum(const vec3 &a){
        return (a.x < a.y) ? ((a.x < a.z) ? (a.x) : (a.z)) : (a.y);
    }

    /// \brief Return the smaller value from the parameter
    ///
    /// \author Alessandro Ribeiro
    /// \param a Set of values to test
    /// \return The smaller value from the parameter
    ///
    ARIBEIRO_INLINE float minimum(const vec4 &a){
        return (a.x < a.y) ? ((a.x < a.z) ? ((a.x < a.w) ? (a.x) : (a.w)) : (a.z)) : (a.y);
    }
    /// \brief Return the minimum between the two parameters
    ///
    /// \author Alessandro Ribeiro
    /// \param a A value to test
    /// \param b A value to test
    /// \return The minimum value of the parameters
    ///
    //ARIBEIRO_API float minimum(const float a, const float b);
    /// \brief Component-wise minimum value from two vectors
    ///
    ///  Return the minimum value considering each component of the vector.
    ///
    /// ex.: vec2( minimum(a.x,b.x), minimum(a.y,b.y) )
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \param b A vector
    /// \return The minimum value for each vector component
    ///
    ARIBEIRO_INLINE vec2 minimum(const vec2 &a, const vec2 &b){
#if defined(ARIBEIRO_SSE2)
        return _mm_min_ps(a.array_sse, b.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vminq_f32(a.array_neon, b.array_neon);
#else
        return vec2((a.x < b.x) ? a.x : b.x,
                    (a.y < b.y) ? a.y : b.y);
#endif
    }
    /// \brief Component-wise minimum value from two vectors
    ///
    ///  Return the minimum value considering each component of the vector.
    ///
    /// ex.: vec2( minimum(a.x,b.x), minimum(a.y,b.y) )
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \param b A vector
    /// \return The minimum value for each vector component
    ///
    ARIBEIRO_INLINE vec3 minimum(const vec3 &a, const vec3 &b){
#if defined(ARIBEIRO_SSE2)
        return _mm_min_ps(a.array_sse, b.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vminq_f32(a.array_neon, b.array_neon);
#else
        return vec3((a.x < b.x) ? a.x : b.x,
                    (a.y < b.y) ? a.y : b.y,
                    (a.z < b.z) ? a.z : b.z);
#endif
    }
    /// \brief Component-wise minimum value from two vectors
    ///
    ///  Return the minimum value considering each component of the vector.
    ///
    /// ex.: vec2( minimum(a.x,b.x), minimum(a.y,b.y) )
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \param b A vector
    /// \return The minimum value for each vector component
    ///
    ARIBEIRO_INLINE vec4 minimum(const vec4 &a, const vec4 &b){
#if defined(ARIBEIRO_SSE2)
        return _mm_min_ps(a.array_sse, b.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vminq_f32(a.array_neon, b.array_neon);
#else
        return vec4((a.x < b.x) ? a.x : b.x,
                    (a.y < b.y) ? a.y : b.y,
                    (a.z < b.z) ? a.z : b.z,
                    (a.w < b.w) ? a.w : b.w);
#endif
    }

    //------------------------------------------------------------------------------
    /// \brief Compute the sign of a float
    ///
    /// if a >=0 then sign = 1
    /// else sign = -1
    ///
    /// \author Alessandro Ribeiro
    /// \param a The value to test
    /// \return The sign of a
    ///
    //ARIBEIRO_API float sign(const float a);
    //------------------------------------------------------------------------------
    /// \brief Compute the absolute value of a number
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \return |a|
    ///
    //ARIBEIRO_API float absv(float a);
    //------------------------------------------------------------------------------
    /// \brief Compute the absolute value of a vector
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \return |a|
    ///
    ARIBEIRO_INLINE vec2 absv(const vec2 &a){
#if defined(ARIBEIRO_SSE2)
        static const __m128 _vec4_sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
        return _mm_andnot_ps(_vec4_sign_mask, a.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vabsq_f32(a.array_neon);
#else
        return vec2((a.x < 0) ? (-a.x) : (a.x),
                    (a.y < 0) ? (-a.y) : (a.y));
#endif
    }

    /// \brief Compute the absolute value of a vector
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \return |a|
    ///
    ARIBEIRO_INLINE vec3 absv(const vec3 &a){
#if defined(ARIBEIRO_SSE2)
        static const __m128 _vec4_sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
        return _mm_andnot_ps(_vec4_sign_mask, a.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vabsq_f32(a.array_neon);
#else
        return vec3((a.x < 0) ? (-a.x) : (a.x),
                    (a.y < 0) ? (-a.y) : (a.y),
                    (a.z < 0) ? (-a.z) : (a.z));
#endif
    }
    /// \brief Compute the absolute value of a vector
    ///
    /// \author Alessandro Ribeiro
    /// \param a A vector
    /// \return |a|
    ///
    ARIBEIRO_INLINE vec4 absv(const vec4 &a){
#if defined(ARIBEIRO_SSE2)
        static const __m128 _vec4_sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
        return _mm_andnot_ps(_vec4_sign_mask, a.array_sse);
#elif defined(ARIBEIRO_NEON)
        return vabsq_f32(a.array_neon);
#else
        return vec4((a.x < 0) ? (-a.x) : (a.x),
                    (a.y < 0) ? (-a.y) : (a.y),
                    (a.z < 0) ? (-a.z) : (a.z),
                    (a.w < 0) ? (-a.w) : (a.w));
#endif
    }
    //------------------------------------------------------------------------------


    /// \brief Computes the linear interpolation
    ///
    /// When the fator is between 0 and 1, it returns the convex relation (linear interpolation) between a and b.
    ///
    /// \author Alessandro Ribeiro
    /// \param a Origin Vector
    /// \param b Target Vector
    /// \param factor The amount (%) to leave the Origin to the Target.
    /// \return The interpolation result
    ///
    //ARIBEIRO_API float lerp(const float a,const  float b,const float factor);


    /// \brief Computes the linear interpolation
    ///
    /// When the fator is between 0 and 1, it returns the convex relation (linear interpolation) between a and b.
    ///
    /// \author Alessandro Ribeiro
    /// \param a Origin Vector
    /// \param b Target Vector
    /// \param factor The amount (%) to leave the Origin to the Target.
    /// \return The interpolation result
    ///
    ARIBEIRO_INLINE vec2 lerp(const vec2 &a,const  vec2 &b,const float &factor){
        //  return a+(b-a)*fator;
        return a * (1.0f - factor) + (b*factor);
    }
    /// \brief Computes the linear interpolation
    ///
    /// When the fator is between 0 and 1, it returns the convex relation (linear interpolation) between a and b.
    ///
    /// \author Alessandro Ribeiro
    /// \param a Origin Vector
    /// \param b Target Vector
    /// \param factor The amount (%) to leave the Origin to the Target.
    /// \return The interpolation result
    ///
    ARIBEIRO_INLINE vec3 lerp(const vec3 &a,const  vec3 &b,const float &factor){
        //  return a+(b-a)*fator;
        return a * (1.0f - factor) + (b*factor);
    }
    /// \brief Computes the linear interpolation
    ///
    /// When the fator is between 0 and 1, it returns the convex relation (linear interpolation) between a and b.
    ///
    /// \author Alessandro Ribeiro
    /// \param a Origin Vector
    /// \param b Target Vector
    /// \param factor The amount (%) to leave the Origin to the Target.
    /// \return The interpolation result
    ///
    ARIBEIRO_INLINE vec4 lerp(const vec4 &a,const  vec4 &b,const float &factor){
        //  return a+(b-a)*fator;
        return a * (1.0f - factor) + (b*factor);
    }
    /// \brief Computes the linear interpolation
    ///
    /// When the fator is between 0 and 1, it returns the convex relation (linear interpolation) between a and b.
    ///
    /// \author Alessandro Ribeiro
    /// \param a Origin Tensor
    /// \param b Target Tensor
    /// \param factor The amount (%) to leave the Origin to the Target.
    /// \return The interpolation result
    ///
    ARIBEIRO_INLINE mat4 lerp(const mat4 &a,const  mat4 &b,const float &factor){
        //  return a+(b-a)*fator;
        return a * (1.0f - factor) + (b*factor);
    }

    /// \brief Computes the result of the interpolation based on the baricentric coordinate (uv) considering 3 points
    ///
    /// It is possible to discover the value of 'u' and 'v' by using the triangle area formula.
    ///
    /// After that it is possible to use this function to interpolate normals, colors, etc... based on the baricentric coorginate uv
    ///
    /// Note: If the uv were calculated in euclidian space of a triangle, then interpolation of colors, normals or coordinates are not affected by the perspective projection.
    ///
    /// \author Alessandro Ribeiro
    /// \param u The u component of a baricentric coord
    /// \param v The v component of a baricentric coord
    /// \param v0 The first vector to interpolate
    /// \param v1 The second vector to interpolate
    /// \param v2 The third vector to interpolate
    /// \return A vector interpolated based on uv considering the 3 vectors of the parameter
    ///
    ARIBEIRO_INLINE vec3 barylerp(const float &u, const float &v, const vec3 &v0, const vec3 &v1, const vec3 &v2){
        // return v0*(1-uv[0]-uv[1])+v1*uv[0]+v2*uv[1];
        return v0 * (1 - u - v) + v1 * u + v2 * v;
    }


    /// \brief Computes the result of the bilinear interpolation over a square patch with 4 points
    ///
    /// The bilinear interpolation is usefull to compute colors between pixels in a image.
    ///
    /// This implementation considers that the square formed by the four points is a square.
    ///
    /// If you try to interpolate values of a non square area, you will have a result, but it might be weird.
    ///
    /// <pre>
    /// dx - [0..1]
    /// dy - [0..1]
    ///
    ///  D-f(0,1) ---*----- C-f(1,1)
    ///     |        |         |
    ///     |        |         |
    /// .   *--------P---------*   P = (dx,dy)
    ///     |        |         |
    ///     |        |         |
    ///  A-f(0,0) ---*----- B-f(1,0)
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param A The lower-left vector
    /// \param B The lower-right vector
    /// \param C The upper-right vector
    /// \param D The upper-left vector
    /// \param dx The x axis interpolation factor
    /// \param dy The y axis interpolation factor
    /// \return A vector interpolated based on dxdy considering the 4 vectors of the parameter
    ///
    ARIBEIRO_INLINE vec3 blerp(const vec3 &A,const vec3 &B,const vec3 &C,const vec3 &D,
                               const float &dx, const float &dy){
        float omdx = 1.0f - dx,
        omdy = 1.0f - dy;
        return (omdx * omdy)*A + (omdx * dy)*D + (dx * omdy)*B + (dx * dy)*C;
    }

    /// \brief Computes the result of the spline interpolation using the CatmullRom aproach
    ///
    /// The spline is a curve based in four points. The CatmullRom aproach makes the curve walk through the control points.
    ///
    /// It can be used to make smooth curves in paths.
    ///
    /// The values interpolated will be bewteen the 2nd and 3rd control points.
    ///
    /// <pre>
    /// t - [0..1]
    ///
    /// q(t) = 0.5 * (1.0f,t,t2,t3)  *
    ///
    /// [  0  2  0  0 ]  [P0]
    /// [ -1  0  1  0 ]* [P1]
    /// [  2 -5  4 -1 ]  [P2]
    /// [ -1  3 -3  1 ]  [P3]
    ///
    /// q(t) = 0.5 *((2 * P1) +
    ///              (-P0 + P2) * t +
    ///              (2*P0 - 5*P1 + 4*P2 - P3) * t2 +
    ///              (-P0 + 3*P1- 3*P2 + P3) * t3)
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param P0 The 1st control point
    /// \param P1 The 2nd control point
    /// \param P2 The 3rd control point
    /// \param P3 The 4th control point
    /// \param t The interpolation value
    /// \return A vector interpolated based on t considering the 4 control points
    ///
    ARIBEIRO_INLINE vec3 splineCatmullRom(const vec3 &P0,const vec3 &P1, const vec3 &P2, const vec3 &P3,const float &t){
        return 0.5f *((2.0f * P1) +
                      (P2 - P0) * t +
                      (2.0f*P0 - 5.0f*P1 + 4.0f*P2 - P3) * (t*t) +
                      (3.0f*P1 - P0 - 3.0f*P2 + P3) * (t*t*t));
    }
    /// \brief Computes the result of the spline interpolation using the CatmullRom aproach
    ///
    /// The spline is a curve based in four points. The CatmullRom aproach makes the curve walk through the control points.
    ///
    /// It can be used to make smooth curves in paths.
    ///
    /// The values interpolated will be bewteen the 2nd and 3rd control points.
    ///
    /// <pre>
    /// t - [0..1]
    ///
    /// q(t) = 0.5 * (1.0f,t,t2,t3)  *
    ///
    /// [  0  2  0  0 ]  [P0]
    /// [ -1  0  1  0 ]* [P1]
    /// [  2 -5  4 -1 ]  [P2]
    /// [ -1  3 -3  1 ]  [P3]
    ///
    /// q(t) = 0.5 *((2 * P1) +
    ///              (-P0 + P2) * t +
    ///              (2*P0 - 5*P1 + 4*P2 - P3) * t2 +
    ///              (-P0 + 3*P1- 3*P2 + P3) * t3)
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param P0 The 1st control point
    /// \param P1 The 2nd control point
    /// \param P2 The 3rd control point
    /// \param P3 The 4th control point
    /// \param t The interpolation value
    /// \return A vector interpolated based on t considering the 4 control points
    ///
    ARIBEIRO_INLINE vec2 splineCatmullRom(const vec2 &P0,const vec2 &P1,const vec2 &P2,const vec2 &P3,const float &t){
        return 0.5f *((2.0f * P1) +
                      (P2 - P0) * t +
                      (2.0f*P0 - 5.0f*P1 + 4.0f*P2 - P3) * (t*t) +
                      (3.0f*P1 - P0 - 3.0f*P2 + P3) * (t*t*t));
    }

    /// \brief Extracts the rotation component inside a mat4
    ///
    /// The rotation component is a 3x3 matrix.
    ///
    /// \author Alessandro Ribeiro
    /// \param m Origin mat4
    /// \return Anothet mat4 with just the rotation
    ///
    ARIBEIRO_INLINE mat4 extractRotation(const mat4& m){
#if defined(ARIBEIRO_SSE2)
        /*
        static const __m128 _valuemask = _mm_load_( 1,1,1,0 );
        static const __m128 _one = _mm_load_( 0,0,0,1 );

        return mat4(
                    _mm_mul_ps(m.array_sse[0], _valuemask),
                    _mm_mul_ps(m.array_sse[1], _valuemask),
                    _mm_mul_ps(m.array_sse[2], _valuemask),
                    _one
                    //_mm_add_ps( _mm_mul_ps(m.array_sse[3], _valuemask), _one)
        );
*/
        //much faster
        //static const __m128 _one = _mm_load_( 0,0,0,1 );
        mat4 r(m.array_sse[0],m.array_sse[1],m.array_sse[2],mat4::IdentityMatrix.array_sse[3]);//_one
		_mm_f32_(r.array_sse[0],3) = 0;
		_mm_f32_(r.array_sse[1],3) = 0;
		_mm_f32_(r.array_sse[2],3) = 0;
        return r;
#elif defined(ARIBEIRO_NEON)

        static const float32x4_t _valuemask = (float32x4_t){ 1,1,1,0 };
        static const float32x4_t _one = (float32x4_t){ 0,0,0,1 };

        return mat4(
            vmulq_f32(m.array_neon[0],_valuemask),
            vmulq_f32(m.array_neon[1],_valuemask),
            vmulq_f32(m.array_neon[2],_valuemask),
            _one
            //vaddq_f32(vmulq_f32(m.array_neon[3],_valuemask), _one)
        );

#else
        return mat4(m._11, m._12, m._13, 0,
                    m._21, m._22, m._23, 0,
                    m._31, m._32, m._33, 0,
                    0, 0, 0, 1);
#endif
    }

    /// \brief Extracts the X axis from a matrix
    ///
    /// \author Alessandro Ribeiro
    /// \param m Origin mat4
    /// \return A vec3 with the X axis
    ///
    ARIBEIRO_INLINE vec3 extractXaxis(const mat4& m){
#if defined(ARIBEIRO_SSE2)
        return m.array_sse[0];
#elif defined(ARIBEIRO_NEON)
        return m.array_neon[0];
#else
        return vec3(m.a1, m.a2, m.a3);
#endif
    }
    /// \brief Extracts the Y axis from a matrix
    ///
    /// \author Alessandro Ribeiro
    /// \param m Origin mat4
    /// \return A vec3 with the Y axis
    ///
    ARIBEIRO_INLINE vec3 extractYaxis(const mat4& m){
#if defined(ARIBEIRO_SSE2)
        return m.array_sse[1];
#elif defined(ARIBEIRO_NEON)
        return m.array_neon[1];
#else
        return vec3(m.b1, m.b2, m.b3);
#endif
    }
    /// \brief Extracts the Z axis from a matrix
    ///
    /// \author Alessandro Ribeiro
    /// \param m Origin mat4
    /// \return A vec3 with the Z axis
    ///
    ARIBEIRO_INLINE vec3 extractZaxis(const mat4& m){
#if defined(ARIBEIRO_SSE2)
        return m.array_sse[2];
#elif defined(ARIBEIRO_NEON)
        return m.array_neon[2];
#else
        return vec3(m.c1, m.c2, m.c3);
#endif
    }
    /// \brief Extracts the Translation part of the matrix
    ///
    /// \author Alessandro Ribeiro
    /// \param m Origin mat4
    /// \return A vec3 with the translation vector
    ///
    ARIBEIRO_INLINE vec3 extractTranslation(const mat4& m){
#if defined(ARIBEIRO_SSE2)
        return m.array_sse[3];
#elif defined(ARIBEIRO_NEON)
        return m.array_neon[3];
#else
        return vec3(m.d1, m.d2, m.d3);
#endif
    }
    /// \brief Computes the transpose matrix
    ///
    /// \author Alessandro Ribeiro
    /// \param m Origin mat4
    /// \return The transposed matrix
    ///
    ARIBEIRO_INLINE mat4 transpose(const mat4& m){
#if defined(ARIBEIRO_SSE2)
        __m128 tmp0 = _mm_shuffle_ps(m.array_sse[0], m.array_sse[1], _MM_SHUFFLE(1, 0, 1, 0));
        __m128 tmp2 = _mm_shuffle_ps(m.array_sse[0], m.array_sse[1], _MM_SHUFFLE(3, 2, 3, 2));
        __m128 tmp1 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[3], _MM_SHUFFLE(1, 0, 1, 0));
        __m128 tmp3 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[3], _MM_SHUFFLE(3, 2, 3, 2));

        return mat4(
            _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(2, 0, 2, 0)),
            _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(3, 1, 3, 1)),
            _mm_shuffle_ps(tmp2, tmp3, _MM_SHUFFLE(2, 0, 2, 0)),
            _mm_shuffle_ps(tmp2, tmp3, _MM_SHUFFLE(3, 1, 3, 1)));
#elif defined(ARIBEIRO_NEON)

        float32x4x2_t ab = vtrnq_f32(m.array_neon[0],m.array_neon[1]);
        float32x4x2_t cd = vtrnq_f32(m.array_neon[2],m.array_neon[3]);
        float32x4_t a_=vcombine_f32(vget_low_f32(ab.val[0]),vget_low_f32(cd.val[0]));
        float32x4_t b_=vcombine_f32(vget_low_f32(ab.val[1]),vget_low_f32(cd.val[1]));
        float32x4_t c_=vcombine_f32(vget_high_f32(ab.val[0]),vget_high_f32(cd.val[0]));
        float32x4_t d_=vcombine_f32(vget_high_f32(ab.val[1]),vget_high_f32(cd.val[1]));

        return mat4(a_,b_,c_,d_);
#else
        return mat4(m._11, m._21, m._31, m._41,
                    m._12, m._22, m._32, m._42,
                    m._13, m._23, m._33, m._43,
                    m._14, m._24, m._34, m._44);
#endif
    }
    /// \brief Computes the inverse of a matrix
    ///
    /// \author Alessandro Ribeiro
    /// \param m Origin mat4
    /// \return The inverse matrix
    ///
    ARIBEIRO_INLINE mat4 inv(const mat4& m){
#if defined(ARIBEIRO_SSE2)

        __m128 Fac0;
        {
            //    valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            //    valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            //    valType SubFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
            //    valType SubFactor13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

            __m128 Swp0a = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(3, 3, 3, 3));
            __m128 Swp0b = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(2, 2, 2, 2));

            __m128 Swp00 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(3, 3, 3, 3));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac0 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 Fac1;
        {
            //    valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            //    valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            //    valType SubFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
            //    valType SubFactor14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

            __m128 Swp0a = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(3, 3, 3, 3));
            __m128 Swp0b = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(1, 1, 1, 1));

            __m128 Swp00 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(3, 3, 3, 3));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac1 = _mm_sub_ps(Mul00, Mul01);
        }


        __m128 Fac2;
        {
            //    valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            //    valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            //    valType SubFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
            //    valType SubFactor15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

            __m128 Swp0a = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 Swp0b = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(1, 1, 1, 1));

            __m128 Swp00 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(2, 2, 2, 2));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac2 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 Fac3;
        {
            //    valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            //    valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            //    valType SubFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
            //    valType SubFactor16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

            __m128 Swp0a = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(3, 3, 3, 3));
            __m128 Swp0b = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(0, 0, 0, 0));

            __m128 Swp00 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(3, 3, 3, 3));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac3 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 Fac4;
        {
            //    valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            //    valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            //    valType SubFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
            //    valType SubFactor17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

            __m128 Swp0a = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 Swp0b = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(0, 0, 0, 0));

            __m128 Swp00 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(2, 2, 2, 2));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac4 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 Fac5;
        {
            //    valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            //    valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            //    valType SubFactor12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
            //    valType SubFactor18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

            __m128 Swp0a = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 Swp0b = _mm_shuffle_ps(m.array_sse[3], m.array_sse[2], _MM_SHUFFLE(0, 0, 0, 0));

            __m128 Swp00 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(m.array_sse[2], m.array_sse[1], _MM_SHUFFLE(1, 1, 1, 1));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac5 = _mm_sub_ps(Mul00, Mul01);
        }

        //static const __m128 SignA = _mm_set_ps( 1.0f,-1.0f, 1.0f,-1.0f);
        //static const __m128 SignB = _mm_set_ps(-1.0f, 1.0f,-1.0f, 1.0f);
        static const __m128 SignAMask = _mm_set_ps( 0.0f,-0.0f, 0.0f,-0.0f);
        static const __m128 SignBMask = _mm_set_ps(-0.0f, 0.0f,-0.0f, 0.0f);

        // m[1][0]
        // m[0][0]
        // m[0][0]
        // m[0][0]
        __m128 Temp0 = _mm_shuffle_ps(m.array_sse[1], m.array_sse[0], _MM_SHUFFLE(0, 0, 0, 0));
        __m128 Vec0 = _mm_shuffle_ps(Temp0, Temp0, _MM_SHUFFLE(2, 2, 2, 0));

        // m[1][1]
        // m[0][1]
        // m[0][1]
        // m[0][1]
        __m128 Temp1 = _mm_shuffle_ps(m.array_sse[1], m.array_sse[0], _MM_SHUFFLE(1, 1, 1, 1));
        __m128 Vec1 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(2, 2, 2, 0));

        // m[1][2]
        // m[0][2]
        // m[0][2]
        // m[0][2]
        __m128 Temp2 = _mm_shuffle_ps(m.array_sse[1], m.array_sse[0], _MM_SHUFFLE(2, 2, 2, 2));
        __m128 Vec2 = _mm_shuffle_ps(Temp2, Temp2, _MM_SHUFFLE(2, 2, 2, 0));

        // m[1][3]
        // m[0][3]
        // m[0][3]
        // m[0][3]
        __m128 Temp3 = _mm_shuffle_ps(m.array_sse[1], m.array_sse[0], _MM_SHUFFLE(3, 3, 3, 3));
        __m128 Vec3 = _mm_shuffle_ps(Temp3, Temp3, _MM_SHUFFLE(2, 2, 2, 0));

        // col0
        // + (Vec1[0] * Fac0[0] - Vec2[0] * Fac1[0] + Vec3[0] * Fac2[0]),
        // - (Vec1[1] * Fac0[1] - Vec2[1] * Fac1[1] + Vec3[1] * Fac2[1]),
        // + (Vec1[2] * Fac0[2] - Vec2[2] * Fac1[2] + Vec3[2] * Fac2[2]),
        // - (Vec1[3] * Fac0[3] - Vec2[3] * Fac1[3] + Vec3[3] * Fac2[3]),
        __m128 Mul00 = _mm_mul_ps(Vec1, Fac0);
        __m128 Mul01 = _mm_mul_ps(Vec2, Fac1);
        __m128 Mul02 = _mm_mul_ps(Vec3, Fac2);
        __m128 Sub00 = _mm_sub_ps(Mul00, Mul01);
        __m128 Add00 = _mm_add_ps(Sub00, Mul02);
        //__m128 Inv0 = _mm_mul_ps(SignB, Add00);
        __m128 Inv0 = _mm_xor_ps(SignBMask, Add00);

        // col1
        // - (Vec0[0] * Fac0[0] - Vec2[0] * Fac3[0] + Vec3[0] * Fac4[0]),
        // + (Vec0[0] * Fac0[1] - Vec2[1] * Fac3[1] + Vec3[1] * Fac4[1]),
        // - (Vec0[0] * Fac0[2] - Vec2[2] * Fac3[2] + Vec3[2] * Fac4[2]),
        // + (Vec0[0] * Fac0[3] - Vec2[3] * Fac3[3] + Vec3[3] * Fac4[3]),
        __m128 Mul03 = _mm_mul_ps(Vec0, Fac0);
        __m128 Mul04 = _mm_mul_ps(Vec2, Fac3);
        __m128 Mul05 = _mm_mul_ps(Vec3, Fac4);
        __m128 Sub01 = _mm_sub_ps(Mul03, Mul04);
        __m128 Add01 = _mm_add_ps(Sub01, Mul05);
        //__m128 Inv1 = _mm_mul_ps(SignA, Add01);
        __m128 Inv1 = _mm_xor_ps(SignAMask, Add01);

        // col2
        // + (Vec0[0] * Fac1[0] - Vec1[0] * Fac3[0] + Vec3[0] * Fac5[0]),
        // - (Vec0[0] * Fac1[1] - Vec1[1] * Fac3[1] + Vec3[1] * Fac5[1]),
        // + (Vec0[0] * Fac1[2] - Vec1[2] * Fac3[2] + Vec3[2] * Fac5[2]),
        // - (Vec0[0] * Fac1[3] - Vec1[3] * Fac3[3] + Vec3[3] * Fac5[3]),
        __m128 Mul06 = _mm_mul_ps(Vec0, Fac1);
        __m128 Mul07 = _mm_mul_ps(Vec1, Fac3);
        __m128 Mul08 = _mm_mul_ps(Vec3, Fac5);
        __m128 Sub02 = _mm_sub_ps(Mul06, Mul07);
        __m128 Add02 = _mm_add_ps(Sub02, Mul08);
        //__m128 Inv2 = _mm_mul_ps(SignB, Add02);
        __m128 Inv2 = _mm_xor_ps(SignBMask, Add02);

        // col3
        // - (Vec1[0] * Fac2[0] - Vec1[0] * Fac4[0] + Vec2[0] * Fac5[0]),
        // + (Vec1[0] * Fac2[1] - Vec1[1] * Fac4[1] + Vec2[1] * Fac5[1]),
        // - (Vec1[0] * Fac2[2] - Vec1[2] * Fac4[2] + Vec2[2] * Fac5[2]),
        // + (Vec1[0] * Fac2[3] - Vec1[3] * Fac4[3] + Vec2[3] * Fac5[3]));
        __m128 Mul09 = _mm_mul_ps(Vec0, Fac2);
        __m128 Mul10 = _mm_mul_ps(Vec1, Fac4);
        __m128 Mul11 = _mm_mul_ps(Vec2, Fac5);
        __m128 Sub03 = _mm_sub_ps(Mul09, Mul10);
        __m128 Add03 = _mm_add_ps(Sub03, Mul11);
        //__m128 Inv3 = _mm_mul_ps(SignA, Add03);
        __m128 Inv3 = _mm_xor_ps(SignAMask, Add03);

        __m128 Row0 = _mm_shuffle_ps(Inv0, Inv1, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 Row1 = _mm_shuffle_ps(Inv2, Inv3, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 Row2 = _mm_shuffle_ps(Row0, Row1, _MM_SHUFFLE(2, 0, 2, 0));

        //    valType Determinant = m[0][0] * Inverse[0][0]
        //                        + m[0][1] * Inverse[1][0]
        //                        + m[0][2] * Inverse[2][0]
        //                        + m[0][3] * Inverse[3][0];
        __m128 Det0 = dot_sse_4(m.array_sse[0], Row2);
        
        if (_mm_f32_( Det0, 0 ) == 0){
            fprintf(stderr,"trying to invert a singular matrix\n");
            exit(-1);
        }

        __m128 Rcp0 = _mm_set1_ps( 1.0f/_mm_f32_( Det0, 0 ) );
        //__m128 Rcp0 = _mm_div_ps(_mm_set1_ps(1.0f), Det0);
        //__m128 Rcp0 = _mm_rcp_ps(Det0);

        //    Inverse /= Determinant;
        return mat4(_mm_mul_ps(Inv0, Rcp0),
                    _mm_mul_ps(Inv1, Rcp0),
                    _mm_mul_ps(Inv2, Rcp0),
                    _mm_mul_ps(Inv3, Rcp0));

#elif defined(ARIBEIRO_NEON)

        float32x4_t Fac0;
        {
            //    valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            //    valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            //    valType SubFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
            //    valType SubFactor13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

            float32x4_t Swp0a = vshuffle_3333( m.array_neon[3], m.array_neon[2] );
            float32x4_t Swp0b = vshuffle_2222( m.array_neon[3], m.array_neon[2] );

            float32x4_t Swp00 = vshuffle_2222( m.array_neon[2], m.array_neon[1] );
            float32x4_t Swp01 = vshuffle_2000( Swp0a );
            float32x4_t Swp02 = vshuffle_2000( Swp0b );
            float32x4_t Swp03 = vshuffle_3333( m.array_neon[2], m.array_neon[1] );

            float32x4_t Mul00 = vmulq_f32(Swp00, Swp01);
            float32x4_t Mul01 = vmulq_f32(Swp02, Swp03);
            Fac0 = vsubq_f32(Mul00, Mul01);
        }

        float32x4_t Fac1;
        {
            //    valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            //    valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            //    valType SubFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
            //    valType SubFactor14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

            float32x4_t Swp0a = vshuffle_3333( m.array_neon[3], m.array_neon[2] );
            float32x4_t Swp0b = vshuffle_1111( m.array_neon[3], m.array_neon[2] );

            float32x4_t Swp00 = vshuffle_1111( m.array_neon[2], m.array_neon[1] );
            float32x4_t Swp01 = vshuffle_2000(Swp0a);
            float32x4_t Swp02 = vshuffle_2000(Swp0b);
            float32x4_t Swp03 = vshuffle_3333(m.array_neon[2], m.array_neon[1] );

            float32x4_t Mul00 = vmulq_f32(Swp00, Swp01);
            float32x4_t Mul01 = vmulq_f32(Swp02, Swp03);
            Fac1 = vsubq_f32(Mul00, Mul01);
        }


        float32x4_t Fac2;
        {
            //    valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            //    valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            //    valType SubFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
            //    valType SubFactor15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

            float32x4_t Swp0a = vshuffle_2222(m.array_neon[3], m.array_neon[2]);
            float32x4_t Swp0b = vshuffle_1111(m.array_neon[3], m.array_neon[2]);

            float32x4_t Swp00 = vshuffle_1111(m.array_neon[2], m.array_neon[1]);
            float32x4_t Swp01 = vshuffle_2000(Swp0a);
            float32x4_t Swp02 = vshuffle_2000(Swp0b);
            float32x4_t Swp03 = vshuffle_2222(m.array_neon[2], m.array_neon[1]);

            float32x4_t Mul00 = vmulq_f32(Swp00, Swp01);
            float32x4_t Mul01 = vmulq_f32(Swp02, Swp03);
            Fac2 = vsubq_f32(Mul00, Mul01);
        }

        float32x4_t Fac3;
        {
            //    valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            //    valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            //    valType SubFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
            //    valType SubFactor16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

            float32x4_t Swp0a = vshuffle_3333(m.array_neon[3], m.array_neon[2]);
            float32x4_t Swp0b = vshuffle_0000(m.array_neon[3], m.array_neon[2]);

            float32x4_t Swp00 = vshuffle_0000(m.array_neon[2], m.array_neon[1]);
            float32x4_t Swp01 = vshuffle_2000(Swp0a);
            float32x4_t Swp02 = vshuffle_2000(Swp0b);
            float32x4_t Swp03 = vshuffle_3333(m.array_neon[2], m.array_neon[1]);

            float32x4_t Mul00 = vmulq_f32(Swp00, Swp01);
            float32x4_t Mul01 = vmulq_f32(Swp02, Swp03);
            Fac3 = vsubq_f32(Mul00, Mul01);
        }

        float32x4_t Fac4;
        {
            //    valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            //    valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            //    valType SubFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
            //    valType SubFactor17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

            float32x4_t Swp0a = vshuffle_2222(m.array_neon[3], m.array_neon[2]);
            float32x4_t Swp0b = vshuffle_0000(m.array_neon[3], m.array_neon[2]);

            float32x4_t Swp00 = vshuffle_0000(m.array_neon[2], m.array_neon[1]);
            float32x4_t Swp01 = vshuffle_2000(Swp0a);
            float32x4_t Swp02 = vshuffle_2000(Swp0b);
            float32x4_t Swp03 = vshuffle_2222(m.array_neon[2], m.array_neon[1]);

            float32x4_t Mul00 = vmulq_f32(Swp00, Swp01);
            float32x4_t Mul01 = vmulq_f32(Swp02, Swp03);
            Fac4 = vsubq_f32(Mul00, Mul01);
        }

        float32x4_t Fac5;
        {
            //    valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            //    valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            //    valType SubFactor12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
            //    valType SubFactor18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

            float32x4_t Swp0a = vshuffle_1111(m.array_neon[3], m.array_neon[2]);
            float32x4_t Swp0b = vshuffle_0000(m.array_neon[3], m.array_neon[2]);

            float32x4_t Swp00 = vshuffle_0000(m.array_neon[2], m.array_neon[1]);
            float32x4_t Swp01 = vshuffle_2000(Swp0a);
            float32x4_t Swp02 = vshuffle_2000(Swp0b);
            float32x4_t Swp03 = vshuffle_1111(m.array_neon[2], m.array_neon[1]);

            float32x4_t Mul00 = vmulq_f32(Swp00, Swp01);
            float32x4_t Mul01 = vmulq_f32(Swp02, Swp03);
            Fac5 = vsubq_f32(Mul00, Mul01);
        }

        static const float32x4_t SignA = (float32x4_t){ 1.0f,-1.0f, 1.0f,-1.0f };
        static const float32x4_t SignB = (float32x4_t){ -1.0f, 1.0f,-1.0f, 1.0f };

        // m[1][0]
        // m[0][0]
        // m[0][0]
        // m[0][0]
        float32x4_t Temp0 = vshuffle_0000(m.array_neon[1], m.array_neon[0]);
        float32x4_t Vec0 = vshuffle_2220(Temp0);

        // m[1][1]
        // m[0][1]
        // m[0][1]
        // m[0][1]
        float32x4_t Temp1 = vshuffle_1111(m.array_neon[1], m.array_neon[0]);
        float32x4_t Vec1 = vshuffle_2220(Temp1);

        // m[1][2]
        // m[0][2]
        // m[0][2]
        // m[0][2]
        float32x4_t Temp2 = vshuffle_2222(m.array_neon[1], m.array_neon[0]);
        float32x4_t Vec2 = vshuffle_2220(Temp2);

        // m[1][3]
        // m[0][3]
        // m[0][3]
        // m[0][3]
        float32x4_t Temp3 = vshuffle_3333(m.array_neon[1], m.array_neon[0]);
        float32x4_t Vec3 = vshuffle_2220(Temp3);

        // col0
        // + (Vec1[0] * Fac0[0] - Vec2[0] * Fac1[0] + Vec3[0] * Fac2[0]),
        // - (Vec1[1] * Fac0[1] - Vec2[1] * Fac1[1] + Vec3[1] * Fac2[1]),
        // + (Vec1[2] * Fac0[2] - Vec2[2] * Fac1[2] + Vec3[2] * Fac2[2]),
        // - (Vec1[3] * Fac0[3] - Vec2[3] * Fac1[3] + Vec3[3] * Fac2[3]),
        float32x4_t Mul00 = vmulq_f32(Vec1, Fac0);
        float32x4_t Mul01 = vmulq_f32(Vec2, Fac1);
        float32x4_t Mul02 = vmulq_f32(Vec3, Fac2);
        float32x4_t Sub00 = vsubq_f32(Mul00, Mul01);
        float32x4_t Add00 = vaddq_f32(Sub00, Mul02);
        float32x4_t Inv0 = vmulq_f32(SignB, Add00);

        // col1
        // - (Vec0[0] * Fac0[0] - Vec2[0] * Fac3[0] + Vec3[0] * Fac4[0]),
        // + (Vec0[0] * Fac0[1] - Vec2[1] * Fac3[1] + Vec3[1] * Fac4[1]),
        // - (Vec0[0] * Fac0[2] - Vec2[2] * Fac3[2] + Vec3[2] * Fac4[2]),
        // + (Vec0[0] * Fac0[3] - Vec2[3] * Fac3[3] + Vec3[3] * Fac4[3]),
        float32x4_t Mul03 = vmulq_f32(Vec0, Fac0);
        float32x4_t Mul04 = vmulq_f32(Vec2, Fac3);
        float32x4_t Mul05 = vmulq_f32(Vec3, Fac4);
        float32x4_t Sub01 = vsubq_f32(Mul03, Mul04);
        float32x4_t Add01 = vaddq_f32(Sub01, Mul05);
        float32x4_t Inv1 = vmulq_f32(SignA, Add01);

        // col2
        // + (Vec0[0] * Fac1[0] - Vec1[0] * Fac3[0] + Vec3[0] * Fac5[0]),
        // - (Vec0[0] * Fac1[1] - Vec1[1] * Fac3[1] + Vec3[1] * Fac5[1]),
        // + (Vec0[0] * Fac1[2] - Vec1[2] * Fac3[2] + Vec3[2] * Fac5[2]),
        // - (Vec0[0] * Fac1[3] - Vec1[3] * Fac3[3] + Vec3[3] * Fac5[3]),
        float32x4_t Mul06 = vmulq_f32(Vec0, Fac1);
        float32x4_t Mul07 = vmulq_f32(Vec1, Fac3);
        float32x4_t Mul08 = vmulq_f32(Vec3, Fac5);
        float32x4_t Sub02 = vsubq_f32(Mul06, Mul07);
        float32x4_t Add02 = vaddq_f32(Sub02, Mul08);
        float32x4_t Inv2 = vmulq_f32(SignB, Add02);

        // col3
        // - (Vec1[0] * Fac2[0] - Vec1[0] * Fac4[0] + Vec2[0] * Fac5[0]),
        // + (Vec1[0] * Fac2[1] - Vec1[1] * Fac4[1] + Vec2[1] * Fac5[1]),
        // - (Vec1[0] * Fac2[2] - Vec1[2] * Fac4[2] + Vec2[2] * Fac5[2]),
        // + (Vec1[0] * Fac2[3] - Vec1[3] * Fac4[3] + Vec2[3] * Fac5[3]));
        float32x4_t Mul09 = vmulq_f32(Vec0, Fac2);
        float32x4_t Mul10 = vmulq_f32(Vec1, Fac4);
        float32x4_t Mul11 = vmulq_f32(Vec2, Fac5);
        float32x4_t Sub03 = vsubq_f32(Mul09, Mul10);
        float32x4_t Add03 = vaddq_f32(Sub03, Mul11);
        float32x4_t Inv3 = vmulq_f32(SignA, Add03);

        float32x4_t Row0 = vshuffle_0000(Inv0, Inv1);
        float32x4_t Row1 = vshuffle_0000(Inv2, Inv3);
        float32x4_t Row2 = vshuffle_2020(Row0, Row1);

        //    valType Determinant = m[0][0] * Inverse[0][0]
        //                        + m[0][1] * Inverse[1][0]
        //                        + m[0][2] * Inverse[2][0]
        //                        + m[0][3] * Inverse[3][0];
        float32x4_t Det0 = dot_neon_4(m.array_neon[0], Row2);
        
        if (Det0[0] == 0){
            fprintf(stderr,"trying to invert a singular matrix\n");
            exit(-1);
        }
        
        float32x4_t Rcp0 = vset1(1.0f/Det0[0]);//_mm_div_ps(_mm_set1_ps(1.0f), Det0);
        //__m128 Rcp0 = _mm_rcp_ps(Det0);

        //    Inverse /= Determinant;
        return mat4(vmulq_f32(Inv0, Rcp0),
                    vmulq_f32(Inv1, Rcp0),
                    vmulq_f32(Inv2, Rcp0),
                    vmulq_f32(Inv3, Rcp0));

#else


        #define mkIndex(y, x) ((y)*4+(x))
        float determinant =
        (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) +
        (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) +
        (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) +
        (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) +
        (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) +
        (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) +
        (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) +
        (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) +
        (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)]) +
        (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) +
        (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) +
        (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)]) -
        (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) -
        (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) -
        (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) -
        (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) -
        (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) -
        (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) -
        (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) -
        (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) -
        (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)]) -
        (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) -
        (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) -
        (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)]);
        if (determinant == 0) {
        
            fprintf(stderr,"trying to invert a singular matrix\n");
            exit(-1);
            
            //1/0 = &infin;
            //log (0) = -&infin;
            //sqrt (-1) = NaN
            //suporte NAN
            //return mat4(std::numeric_limits<float>::quiet_NaN());
            //throw std::runtime_error("singular matrix, cant calculate determinant");
        }
        determinant = 1.0f / determinant;
        mat4 retorno;
        retorno.array[mkIndex(0, 0)] =
        ((m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) -
         (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)])) *
        determinant;
        retorno.array[mkIndex(0, 1)] =
        ((m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) -
         (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)])) *
        determinant;
        retorno.array[mkIndex(0, 2)] =
        ((m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 2)]) -
         (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 1)])) *
        determinant;
        retorno.array[mkIndex(0, 3)] =
        ((m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)]) -
         (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)])) *
        determinant;
        retorno.array[mkIndex(1, 0)] =
        ((m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) -
         (m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)])) *
        determinant;
        retorno.array[mkIndex(1, 1)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)])) *
        determinant;
        retorno.array[mkIndex(1, 2)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 0)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 2)])) *
        determinant;
        retorno.array[mkIndex(1, 3)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)])) *
        determinant;
        retorno.array[mkIndex(2, 0)] =
        ((m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) + (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)]) -
         (m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) - (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)])) *
        determinant;
        retorno.array[mkIndex(2, 1)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)])) *
        determinant;
        retorno.array[mkIndex(2, 2)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 0)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 1)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 1)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 0)])) *
        determinant;
        retorno.array[mkIndex(2, 3)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)])) *
        determinant;
        retorno.array[mkIndex(3, 0)] =
        ((m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)]) -
         (m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)])) *
        determinant;
        retorno.array[mkIndex(3, 1)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)])) *
        determinant;
        retorno.array[mkIndex(3, 2)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 0)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 1)])) *
        determinant;
        retorno.array[mkIndex(3, 3)] =
        ((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)]) -
         (m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)])) *
        determinant;
        return retorno;
#endif
    }

    /// \brief Computes the inverse of a matrix
    ///
    /// ** This inv_faster function in a certain angle returns the wrong matrix...
    ///
    /// \author Alessandro Ribeiro
    /// \param m Origin mat4
    /// \return The inverse matrix
    ///
    //ARIBEIRO_API mat4 inv_faster(const mat4& m);

    /// \brief Reads the modelview matrix from the Opengl and return it as a mat4
    ///
    /// \author Alessandro Ribeiro
    /// \return The OpenGL modelview matrix of opengl state
    ///
    //ARIBEIRO_API mat4 getModelviewFromOpenGL();
    /// \brief Reads the projection matrix from the Opengl and return it as a mat4
    ///
    /// \author Alessandro Ribeiro
    /// \return The OpenGL projection matrix of opengl state
    ///
    //ARIBEIRO_API mat4 getProjectionFromOpenGL();
    /// \brief Reads the texture matrix from the Opengl and return it as a mat4
    ///
    /// \author Alessandro Ribeiro
    /// \return The OpenGL texture matrix of opengl state
    ///
    //ARIBEIRO_API mat4 getTextureFromGL();
    /// \brief Writes a mat4 to the current active matrix of OpenGL state
    ///
    /// \author Alessandro Ribeiro
    /// \param m The source matrix
    ///
    //ARIBEIRO_API void loadMatrixToGL(const mat4& m);
    /// \brief Multiply a mat4 to the current active matrix of OpenGL state
    ///
    /// \author Alessandro Ribeiro
    /// \param m The source matrix
    ///
    //ARIBEIRO_API void mulMatrixToGL(const mat4& m);
    /// \brief Creates a translation 4x4 matrix
    ///
    /// \author Alessandro Ribeiro
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 translate(const float &_x_,const float &_y_,const float &_z_){
#if defined(ARIBEIRO_SSE2)
        return mat4(mat4::IdentityMatrix.array_sse[0],
                    mat4::IdentityMatrix.array_sse[1],
                    mat4::IdentityMatrix.array_sse[2],
                    _mm_load_(_x_,_y_,_z_,1));
#elif defined(ARIBEIRO_NEON)
        return mat4((float32x4_t){1,0,0,0},
					(float32x4_t){0,1,0,0},
					(float32x4_t){0,0,1,0},
					(float32x4_t){_x_,_y_,_z_,1});

#else
        return mat4(1, 0, 0, _x_,
                    0, 1, 0, _y_,
                    0, 0, 1, _z_,
                    0, 0, 0, 1);
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a translation 4x4 matrix
    ///
    /// \author Alessandro Ribeiro
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 translate(const vec3 &_v_){
#if defined(ARIBEIRO_SSE2)
        mat4 result = mat4(mat4::IdentityMatrix.array_sse[0],
                           mat4::IdentityMatrix.array_sse[1],
                           mat4::IdentityMatrix.array_sse[2],
                           _v_.array_sse);
        _mm_f32_(result.array_sse[3],3) = 1.0f;
        return result;
#elif defined(ARIBEIRO_NEON)
        return mat4((float32x4_t){1,0,0,0},
					(float32x4_t){0,1,0,0},
					(float32x4_t){0,0,1,0},

					vaddq_f32( vmulq_f32(_v_.array_neon, (float32x4_t){1,1,1,0}), (float32x4_t){0,0,0,1})

					);
#else
        return mat4(1, 0, 0, _v_.x,
                    0, 1, 0, _v_.y,
                    0, 0, 1, _v_.z,
                    0, 0, 0, 1);
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a translation 4x4 matrix
    ///
    /// \author Alessandro Ribeiro
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 translate(const vec4 &_v_){
#if defined(ARIBEIRO_SSE2)
        
        mat4 result = mat4(mat4::IdentityMatrix.array_sse[0],
                           mat4::IdentityMatrix.array_sse[1],
                           mat4::IdentityMatrix.array_sse[2],
                           _v_.array_sse);
        _mm_f32_(result.array_sse[3],3) = 1.0f; // much faster than make a lot of multiplications

        return result;
        
        /*
        return mat4(
                    _mm_load_(1,0,0,0),
                    _mm_load_(0,1,0,0),
                    _mm_load_(0,0,1,0),
                    _mm_add_ps(
                               _mm_mul_ps( _v_.array_sse, _mm_load_(1,1,1,0) ),
                               _mm_load_(0,0,0,1)
                    )
                );
        */
        /*
        mat4 result = mat4(
			_mm_load_(1,0,0,0),
			_mm_load_(0,1,0,0),
			_mm_load_(0,0,1,0),
			_v_.array_sse);
        _mm_f32_(result.array_sse[3],3) = 1.0f;
        return result;
         */
#elif defined(ARIBEIRO_NEON)
        mat4 result = mat4((float32x4_t){1,0,0,0},
					       (float32x4_t){0,1,0,0},
					       (float32x4_t){0,0,1,0},
                           _v_.array_neon
                           //old code slower...
                           //vaddq_f32( vmulq_f32(_v_.array_neon, (float32x4_t){1,1,1,0}), (float32x4_t){0,0,0,1})
					);
        result.array_neon[3][3] = 1.0f;
        return result;
#else
        return mat4(1, 0, 0, _v_.x,
                    0, 1, 0, _v_.y,
                    0, 0, 1, _v_.z,
                    0, 0, 0, 1);
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a scale 4x4 matrix
    ///
    /// \author Alessandro Ribeiro
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 scale(const float &_x_,const float &_y_,const float &_z_){
#if defined(ARIBEIRO_SSE2)
        return mat4(
			_mm_load_(_x_,0,0,0),
			_mm_load_(0,_y_,0,0),
			_mm_load_(0,0,_z_,0),
			mat4::IdentityMatrix.array_sse[3]);
#elif defined(ARIBEIRO_NEON)
        return mat4((float32x4_t){_x_,0,0,0},
					(float32x4_t){0,_y_,0,0},
					(float32x4_t){0,0,_z_,0},
                    (float32x4_t){0,0,0,1}
					);
#else
        return mat4(_x_, 0, 0, 0,
                    0, _y_, 0, 0,
                    0, 0, _z_, 0,
                    0, 0, 0, 1);
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a scale 4x4 matrix
    ///
    /// \author Alessandro Ribeiro
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 scale(const vec3 &_v_){
#if defined(ARIBEIRO_SSE2)
        return mat4(
			_mm_load_(_v_.x,0,0,0),
			_mm_load_(0,_v_.y,0,0),
			_mm_load_(0,0,_v_.z,0),
			mat4::IdentityMatrix.array_sse[3]
		);
#elif defined(ARIBEIRO_NEON)
        return mat4((float32x4_t){_v_.x,0,0,0},
					(float32x4_t){0,_v_.y,0,0},
					(float32x4_t){0,0,_v_.z,0},
                    (float32x4_t){0,0,0,1}
					);
#else
        return mat4(_v_.x, 0, 0, 0,
                    0, _v_.y, 0, 0,
                    0, 0, _v_.z, 0,
                    0, 0, 0, 1);
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a scale 4x4 matrix
    ///
    /// \author Alessandro Ribeiro
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 scale(const vec4 &_v_){
#if defined(ARIBEIRO_SSE2)
        return mat4(
			_mm_load_(_v_.x,0,0,0),
			_mm_load_(0,_v_.y,0,0),
			_mm_load_(0,0,_v_.z,0),
			mat4::IdentityMatrix.array_sse[3]
		);
#elif defined(ARIBEIRO_NEON)
        return mat4((float32x4_t){_v_.x,0,0,0},
					(float32x4_t){0,_v_.y,0,0},
					(float32x4_t){0,0,_v_.z,0},
                    (float32x4_t){0,0,0,1}
					);
#else
        return mat4(_v_.x, 0, 0, 0,
                    0, _v_.y, 0, 0,
                    0, 0, _v_.z, 0,
                    0, 0, 0, 1);
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a rotation 4x4 matrix over the X axis
    ///
    /// \author Alessandro Ribeiro
    /// \param _phi_ Angle in radians
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 xRotate(const float _phi_){
        float c = cos(_phi_);
        float s = sin(_phi_);
        return mat4(
                    1, 0, 0, 0,
                    0, c, -s, 0,
                    0, s, c, 0,
                    0, 0, 0, 1);
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a rotation 4x4 matrix over the Y axis
    ///
    /// \author Alessandro Ribeiro
    /// \param _theta_ Angle in radians
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 yRotate(const float _theta_){
        float c = cos(_theta_);
        float s = sin(_theta_);
        return mat4(
                    c, 0, s, 0,
                    0, 1, 0, 0,
                    -s, 0, c, 0,
                    0, 0, 0, 1);
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a rotation 4x4 matrix over the Z axis
    ///
    /// \author Alessandro Ribeiro
    /// \param _psi_ Angle in radians
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 zRotate(const float _psi_){
        float c = cos(_psi_);
        float s = sin(_psi_);
        return mat4(
                    c, -s, 0, 0,
                    s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a rotation 4x4 matrix over the Euler angles
    ///
    /// \author Alessandro Ribeiro
    /// \param Angle in radians
    /// \param Angle in radians
    /// \param Angle in radians
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 eulerRotate(const float &roll, const float &pitch, const float &yaw){
        return zRotate(yaw) * yRotate(pitch) * xRotate(roll);
    }

    //------------------------------------------------------------------------------
    /// \brief Extract euler angles from matrix
    ///
    /// \author Alessandro Ribeiro
    /// \param Matrix
    /// \param Angle in radians
    /// \param Angle in radians
    /// \param Angle in radians
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE void extractEuler(mat4 m, float *roll, float *pitch, float *yaw){
        //
        // https://www.learnopencv.com/rotation-matrix-to-euler-angles/
        //
        float sy = sqrt(m._11 * m._11 + m._21 * m._21);

        bool singular = sy < 1e-6f; // If

        float x, y, z;
        if (!singular)
        {
            x = atan2(m._32, m._33);
            y = atan2(-m._31, sy);
            z = atan2(m._21, m._11);
        }
        else
        {
            x = atan2(-m._23, m._22);
            y = atan2(-m._31, sy);
            z = 0;
        }

        *roll = x;
        *pitch = y;
        *yaw = z;
    }

    //------------------------------------------------------------------------------
    /// \brief Creates a rotation 4x4 matrix over an arbitrari axis
    ///
    /// <pre>
    /// c = cossine
    /// s = sine
    /// ||<x,y,z>|| = 1
    ///
    /// |  xx(1-c)+c     xy(1-c)-zs    xz(1-c)+ys     0  |
    /// |  yx(1-c)+zs    yy(1-c)+c     yz(1-c)-xs     0  |
    /// |  xz(1-c)-ys    yz(1-c)+xs    zz(1-c)+c      0  |
    /// |       0            0             0          1  |
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param _ang_ Angle in radians
    /// \param x X component of the axis
    /// \param y Y component of the axis
    /// \param z Z component of the axis
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 rotate(const float &_ang_,float x,float y,float z){
        // depois tem como otimizar esta rotação
        float length_inv = sqrt(x*x + y * y + z * z);
        if (length_inv == 0){
            fprintf(stderr,"division by zero\n");
            exit(-1);
          //throw std::runtime_error("division by zero");
        }
        length_inv = 1.0f / length_inv;
        x *= length_inv;
        y *= length_inv;
        z *= length_inv;
        float c = cos(_ang_);
        float s = sin(_ang_);
        //original -- rota�o em sentido anti-hor�io
        return  mat4(x*x*(1 - c) + c, x*y*(1 - c) - z * s, x*z*(1 - c) + y * s, 0,
                     y*x*(1 - c) + z * s, y*y*(1 - c) + c, y*z*(1 - c) - x * s, 0,
                     x*z*(1 - c) - y * s, y*z*(1 - c) + x * s, z*z*(1 - c) + c, 0,
                     0, 0, 0, 1);

        //transposto -- rota�o em sentido hor�io
        //  return  mat4(x*x*(1-c)+c  ,  y*x*(1-c)+z*s  ,  x*z*(1-c)-y*s,   0  ,
        //               x*y*(1-c)-z*s,  y*y*(1-c)+c    ,  y*z*(1-c)+x*s,   0  ,
        //               x*z*(1-c)+y*s,  y*z*(1-c)-x*s  ,  z*z*(1-c)+c  ,   0  ,
        //                   0        ,        0        ,      0        ,   1  );
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a rotation 4x4 matrix over an arbitrari axis
    ///
    /// <pre>
    /// c = cossine
    /// s = sine
    /// ||<x,y,z>|| = 1
    ///
    /// |  xx(1-c)+c     xy(1-c)-zs    xz(1-c)+ys     0  |
    /// |  yx(1-c)+zs    yy(1-c)+c     yz(1-c)-xs     0  |
    /// |  xz(1-c)-ys    yz(1-c)+xs    zz(1-c)+c      0  |
    /// |       0            0             0          1  |
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param _ang_ Angle in radians
    /// \param axis The axis
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 rotate(const float &_ang_,const vec3 &axis){
        return rotate(_ang_, axis.x, axis.y, axis.z);
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a projection matrix
    ///
    /// <pre>
    /// f=cotangent(CampoDeVisao/2)
    /// matriz:
    ///
    /// f/aspect  0      0                           0
    /// 0         f      0                           0
    /// 0         0    (zfar+znear)/(znear-zfar)    (2*zfar*znear)/(znear-zfar)
    /// 0         0     -1                           0
    /// </pre>
    ///
    /// \author Alessandro Ribeiro
    /// \param FovY Angle in degrees
    /// \param aspectX The aspect of X related to the Height (ex.: Width/Height)
    /// \param near_ Near plane
    /// \param far_ Far plane
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 projection_perspective(const float &FovY,const float &aspectX, const float &near_, const float &far_){

        //   f=cotangent(CampoDeVisao/2)
        //matriz:
        //
        //f/aspect  0      0                           0
        //0         f      0                           0
        //0         0    (zfar+znear)/(znear-zfar)    (2*zfar*znear)/(znear-zfar)
        //0         0     -1                           0

        if ((aspectX == 0.0f) || (near_ - far_) == 0) {
            return mat4::IdentityMatrix;
        }
        float focus = (float)tanf(DEG2RAD(FovY) / 2.0f);
        if (focus == 0.0f) focus = 0.000000000002f;
        focus = 1.0f / focus;
        return mat4(focus / aspectX, 0, 0, 0,
                    0, focus, 0, 0,
                    0, 0, (near_ + far_) / (near_ - far_), (2.0f*near_*far_) / (near_ - far_),
                    0, 0, -1, 0);
    }


    ARIBEIRO_INLINE mat4 projection_perspectiveLH(const float &FovY,const float &aspectX, const float &near_, const float &far_){
        //f=cotangent(CampoDeVisao/2)
        //matriz:
        //
        //f/aspect  0      0                           0
        //0         f      0                           0
        //0         0    -(zfar+znear)/(znear-zfar)    (2*zfar*znear)/(znear-zfar)
        //0         0     1                           0
        if ((aspectX == 0.0f) || (near_ - far_) == 0) {
            return mat4::IdentityMatrix;
        }
        float focus = (float)tanf(DEG2RAD(FovY) / 2.0f);
        if (focus == 0.0f) focus = 0.000000000002f;
        focus = 1.0f / focus;
        return mat4(focus / aspectX, 0, 0, 0,
                    0, focus, 0, 0,
                    0, 0, -(near_ + far_) / (near_ - far_), (2.0f*near_*far_) / (near_ - far_),
                    0, 0, 1, 0);
    }

    //------------------------------------------------------------------------------
    /// \brief Creates a projection matrix
    ///
    /// The unit of the focal length is the same as specified by the width and height.
    ///
    /// ex.: Considering millimeters (mm) in a focal length of 35mm in a CCD area of 50x30 mm.<br />
    /// It is possible to use this function to configure the projection:
    ///
    /// projection_perspective(35,50,30,0.001,100.0)
    ///
    /// \author Alessandro Ribeiro
    /// \param focalLength The focal length
    /// \param w Width
    /// \param h Height
    /// \param near_ Near plane
    /// \param far_ Far plane
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 projection_perspective(const float &focalLength, const float &w, const float &h, const float &near_, const float &far_){
        float fovY = 2.0f*atanf((h*0.5f) / focalLength);
        fovY = RAD2DEG(fovY);
        float aspectX = w / h;
        return projection_perspective(fovY, aspectX, near_, far_);
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a projection matrix from the frustum definition
    ///
    /// \author Alessandro Ribeiro
    /// \param Left The left limit of the projection plane
    /// \param Right The right limit of the projection plane
    /// \param Bottom The bottom limit of the projection plane
    /// \param Top The top limit of the projection plane
    /// \param Near Near plane
    /// \param Far Far plane
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 projection_frustum(const float &Left,const float &Right,const float &Bottom,const float &Top,const float &Near,const float &Far){
        return mat4((2 * Near) / (Right - Left), 0, (Right + Left) / (Right - Left), 0,
                    0, (2 * Near) / (Top - Bottom), (Top + Bottom) / (Top - Bottom), 0,
                    0, 0, (-(Far + Near)) / (Far - Near), (-2 * Far*Near) / (Far - Near),
                    0, 0, -1, 0);
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a projection matrix from the orthographic definition
    ///
    /// \author Alessandro Ribeiro
    /// \param Left The left limit of the projection plane
    /// \param Right The right limit of the projection plane
    /// \param Bottom The bottom limit of the projection plane
    /// \param Top The top limit of the projection plane
    /// \param Near Near plane
    /// \param Far Far plane
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 projection_ortho(const float &Left,const float &Right,const float &Bottom,const float &Top,const float &Near,const float &Far){
        return mat4(2.0f / (Right - Left), 0, 0, -(Right + Left) / (Right - Left),
                    0, 2.0f / (Top - Bottom), 0, -(Top + Bottom) / (Top - Bottom),
                    0, 0, -2.0f / (Far - Near), -(Far + Near) / (Far - Near),
                    0, 0, 0, 1);
    }
    //------------------------------------------------------------------------------
    /// \brief Creates a transformation matrix
    ///
    /// This matrix can be used as base to a camera node
    ///
    /// \author Alessandro Ribeiro
    /// \param front A vector pointing to the direction you want a camera
    /// \param up A vector to indicate the up orientation of a camera
    /// \param position A point to be used as origin to the camera
    /// \return A 4x4 matrix
    ///
    ARIBEIRO_INLINE mat4 lookAt(const vec3 &front, const vec3 &up, const vec3 &position){
        vec3 lookTo = front;
        vec3 x, y, z;
        z = normalize(lookTo)*-1;
        x = normalize(cross(up, z));
        y = normalize(cross(z, x));
        return //scale(0.002f,0.002f,0.002f)*
        mat4(x.x, x.y, x.z, -dot(x, position),
             y.x, y.y, y.z, -dot(y, position),
             z.x, z.y, z.z, -dot(z, position),
             0, 0, 0, 1);
    }
    ARIBEIRO_INLINE mat4 modelLookAt(const vec3 &front, const vec3 &up, const vec3 &position){
        vec3 lookTo = front;
        vec3 x, y, z;
        z = normalize(lookTo)*-1;
        x = normalize(cross(up, z));
        y = normalize(cross(z, x));
        return //scale(0.002f,0.002f,0.002f)*
        mat4(x.x, y.x, z.x, position.x,
             x.y, y.y, z.y, position.y,
             x.z, y.z, z.z, position.z,
             0, 0, 0, 1);
    }
    ARIBEIRO_INLINE quat quatLookAtRotation(const vec3 &front, const vec3 &up){
        vec3 lookTo = front;
        vec3 x, y, z;
        z = normalize(lookTo)*-1;
        x = normalize(cross(up, z));
        y = normalize(cross(z, x));
#if defined(ARIBEIRO_SSE2)
        
        //much faster
        //static const __m128 _w = _mm_load_(0,0,0,1);
        mat4 m(x.array_sse,
               y.array_sse,
               z.array_sse,
               mat4::IdentityMatrix.array_sse[3] ); //_w
        _mm_f32_(m.array_sse[0],3) = 0;
        _mm_f32_(m.array_sse[1],3) = 0;
        _mm_f32_(m.array_sse[2],3) = 0;
        
        /*
        static const __m128 mask = _mm_load_(1, 1, 1, 0);
        static const __m128 _w = _mm_load_(0,0,0,1);

        mat4 m(_mm_mul_ps( x.array_sse, mask),
               _mm_mul_ps( y.array_sse, mask),
               _mm_mul_ps( z.array_sse, mask),
			_w
		);
         */
        return extractQuat(m);
#elif defined(ARIBEIRO_NEON)
        mat4 m(vmulq_f32( x.array_neon, (float32x4_t){1,1,1,0}),
               vmulq_f32( y.array_neon, (float32x4_t){1,1,1,0}),
               vmulq_f32( x.array_neon, (float32x4_t){1,1,1,0}),
			(float32x4_t){0,0,0,1}
		);
        return extractQuat(m);
#else
        return extractQuat(
                           mat4(x.x, y.x, z.x, 0,
                                x.y, y.y, z.y, 0,
                                x.z, y.z, z.z, 0,
                                0, 0, 0, 1));
#endif
    }
    ARIBEIRO_INLINE quat quatLookAtRotationLH(const vec3 &front, const vec3 &up){
        vec3 lookTo = front;
        vec3 x, y, z;
        z = normalize(lookTo);
        x = normalize(cross(up, z));
        y = normalize(cross(z, x));
#if defined(ARIBEIRO_SSE2)
        //much faster
        //static const __m128 _w = _mm_load_(0,0,0,1);
        mat4 m(x.array_sse,
               y.array_sse,
               z.array_sse,
               mat4::IdentityMatrix.array_sse[3] ); // _w
        _mm_f32_(m.array_sse[0],3) = 0;
        _mm_f32_(m.array_sse[1],3) = 0;
        _mm_f32_(m.array_sse[2],3) = 0;
        
        /*
         static const __m128 mask = _mm_load_(1, 1, 1, 0);
         static const __m128 _w = _mm_load_(0,0,0,1);
         
         mat4 m(_mm_mul_ps( x.array_sse, mask),
         _mm_mul_ps( y.array_sse, mask),
         _mm_mul_ps( z.array_sse, mask),
         _w
         );
         */
        
        return extractQuat(m);
#elif defined(ARIBEIRO_NEON)
        mat4 m(vmulq_f32( x.array_neon, (float32x4_t){1,1,1,0}),
               vmulq_f32( y.array_neon, (float32x4_t){1,1,1,0}),
               vmulq_f32( x.array_neon, (float32x4_t){1,1,1,0}),
			(float32x4_t){0,0,0,1}
		);
        return extractQuat(m);

#else
        return extractQuat(
                           mat4(x.x, y.x, z.x, 0,
                                x.y, y.y, z.y, 0,
                                x.z, y.z, z.z, 0,
                                0, 0, 0, 1));
#endif
    }
    //------------------------------------------------------------------------------

    /// \brief Computes the inverse of a 4x4 matrix
    ///
    /// This method is the same as presented by Shan Hao Bo, April 2006.
    ///
    /// NOTICE: Does not compute inverse for all cases...removed from library
    ///
    /// \author Alessandro Ribeiro
    /// \param src The source matrix
    /// \param inverse The return parameter
    /// \return True if it is possible to compute the inverse, false if src is a singular matrix
    ///
    //ARIBEIRO_API bool inverse_alternative(const mat4 &src, mat4 *inverse);

    //------------------------------------------------------------------------------
    /// \brief Computes the result of the reverse projection of a point in the projection plane
    ///
    /// \author Alessandro Ribeiro
    /// \param pointInWindow The 2D point in the screen with the z component indicating the depth z (0-near,1-far)
    /// \param modelViewMatrix The main transform applied to the pointInWindow
    /// \param projectionMatrix The projection matrix
    /// \param viewportX The minimum X limit of the screen
    /// \param viewportY The minimum Y limit of the screen
    /// \param viewportW The width of the screen
    /// \param viewportH The height of the screen
    /// \param worldPtn The return parameter with the unprojected point
    /// \return True if it is possible to compute the point, false if the projection of the modelView is singular
    ///
    ARIBEIRO_INLINE bool unproject(vec3 pointInWindow,
                   const mat4 &modelViewMatrix,
                   const mat4 &projectionMatrix,
                   int viewportX, int viewportY,int viewportW, int viewportH,
                                   vec3 *worldPtn){
        mat4 modelViewProjection_inverse = projectionMatrix * modelViewMatrix;//pre_multiplyed ogl Like

        modelViewProjection_inverse = inv(modelViewProjection_inverse);

        if ( modelViewProjection_inverse.array[0] == std::numeric_limits<float>::quiet_NaN()) {
            return false;
        }

        //if (!inverse_alternative(modelViewProjection_inverse, &modelViewProjection_inverse))
        //return false;
        /* Map x and y from window coordinates */
        pointInWindow.x = (pointInWindow.x - float(viewportX)) / float(viewportW);
        pointInWindow.y = (pointInWindow.y - float(viewportY)) / float(viewportH);
        /* Map to range -1 to 1 */
        pointInWindow = pointInWindow * 2.0f - 1.0f;
        vec4 position_homogeneos = modelViewProjection_inverse * vec4(pointInWindow, 1);
        if (position_homogeneos.w == 0.0)
            return false;
        *worldPtn = toVec3_PerspDiv(position_homogeneos);
        return true;
    }
    //------------------------------------------------------------------------------
    /// \brief Computes the result of the reverse projection of a point in the projection plane Using the matrixes from the OpenGL state
    ///
    /// \author Alessandro Ribeiro
    /// \param pointInWindow The 2D point in the screen with the z component indicating the depth z (0-near,1-far)
    /// \param worldPtn The return parameter with the unprojected point
    /// \return True if it is possible to compute the point, false if the projection of the modelView is singular
    ///
    //ARIBEIRO_API bool GL_unproject(const vec3 &pointInWindow,
    //                                vec3 *worldPtn);
    //------------------------------------------------------------------------------
    /// \brief Computes the result of the projection of a point in the projection plane
    ///
    /// \author Alessandro Ribeiro
    /// \param worldPtn A point in 3D space to be projected
    /// \param modelViewMatrix The main transform applied to the pointInWindow
    /// \param projectionMatrix The projection matrix
    /// \param viewportX The minimum X limit of the screen
    /// \param viewportY The minimum Y limit of the screen
    /// \param viewportW The width of the screen
    /// \param viewportH The height of the screen
    /// \param pointInWindow The return parameter with the 2D point projected screen
    /// \return True if it is possible to compute the point, false if the projection of the modelView is singular
    ///
    ARIBEIRO_INLINE bool project(vec3 worldPtn,
                 const mat4 &modelViewMatrix,
                 const mat4 &projectionMatrix,
                 int viewportX, int viewportY,int viewportW, int viewportH,
                                 vec3 *pointInWindow){
        mat4 modelViewProjection = projectionMatrix * modelViewMatrix;//pre_multiplyed ogl Like
        vec4 position_homogeneos = modelViewProjection * vec4(worldPtn, 1);
        if (position_homogeneos.w == 0.0)
            return false;
        vec3 result = toVec3_PerspDiv(position_homogeneos);
        // Map x, y and z to range 0-1
        result = result * 0.5f + 0.5f;
        // Map x,y to viewport
        result.x = result.x*float(viewportW) + float(viewportX);
        result.y = result.y*float(viewportH) + float(viewportY);
        *pointInWindow = result;
        return true;
    }
    //------------------------------------------------------------------------------
    /// \brief Computes the result of the projection of a point in the projection plane Using the matrixes from the OpenGL state
    ///
    /// \author Alessandro Ribeiro
    /// \param worldPtn A point in 3D space to be projected
    /// \param pointInWindow The return parameter with the 2D point projected screen
    /// \return True if it is possible to compute the point, false if the projection of the modelView is singular
    ///
    //ARIBEIRO_API bool GL_project(const vec3 &worldPtn,
    //                vec3 *pointInWindow);
    //
    // quaternion operations based on http://assimp.sourceforge.net/,
    //  http://www.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
    //  and http://www.gamedev.net/page/resources/_/reference/programming/math-and-physics/quaternions/quaternion-powers-r1095
    //
    //------------------------------------------------------------------------------
    /// \brief Convert a vec3 to a unity quaternion pointing to the vec3 axis.
    ///
    /// \author Alessandro Ribeiro
    /// \param v A 3 component vector
    /// \return The quaternion
    ///
    ARIBEIRO_INLINE quat toQuat(const vec3& vp){
#if defined(ARIBEIRO_SSE2)
        vec3 v = normalize(vp);
        float t = 1.0f - dot(v,v);
        //const float EPSILON = 1e-6f;
        quat result(v.array_sse);
        if (t < EPSILON)
            _mm_f32_(result.array_sse,3) = 0.0f;
        else
			_mm_f32_(result.array_sse,3) = sqrt(t);
        return result;
#elif defined(ARIBEIRO_NEON)
        vec3 v = normalize(vp);
        float t = 1.0f - dot(v,v);
        //const float EPSILON = 1e-6f;
        quat result(v.array_neon);
        if (t < EPSILON)
            result.array_neon[3] = 0.0f;
        else
			result.array_neon[3] = sqrt(t);
        return result;
#else
        vec3 v = normalize(vp);
        float t = 1.0f - (v.x*v.x) - (v.y*v.y) - (v.z*v.z);
        //const float EPSILON = 1e-6f;
        if (t < EPSILON)
            return quat(v.x, v.y, v.z, 0.0f);
        else
            return quat(v.x, v.y, v.z, sqrt(t));
#endif
    }
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------

	/// \brief Multiply two quaternions in a right handed fashion (like mat4 multiplication)
    ///
    /// \author Alessandro Ribeiro
    /// \param a The quaternion
    /// \param b The quaternion
    /// \return The quaternion
    ///
    //ARIBEIRO_API quat mul(const quat& a,const quat& b);

    //------------------------------------------------------------------------------

	/// \brief Apply the quaternion multiplication directly over a point
    ///
    /// \author Alessandro Ribeiro
    /// \param a The quaternion
    /// \param v The point
    /// \return The point transformed
    ///
    //ARIBEIRO_API vec3 rotateVec(const quat& a,const vec3& v);

    //------------------------------------------------------------------------------

	/// \brief Apply the quaternion multiplication directly over a vec4
    ///
    /// The vec4 will be rotated according the x,y,z coords. The w component will be copied from the parameter to the result.
    ///
    /// \author Alessandro Ribeiro
    /// \param a The quaternion
    /// \param v The vec4
    /// \return The vec4 transformed with the same w as the parameter
    ///
    //ARIBEIRO_API vec4 rotateVec(const quat& a,const vec4& v);

    //------------------------------------------------------------------------------
    /// \brief Constructs a quaternion from an axis and an angle in radians.
    ///
    /// \author Alessandro Ribeiro
    /// \param axis The reference axis
    /// \param angle_rad The angle to rotate around the axis
    /// \return The quat
    ///
    ARIBEIRO_INLINE quat quatFromAxisAngle(const vec3& axis,const float &angle_rad){
#if defined(ARIBEIRO_SSE2)
        float sinAngle;
        float angle = angle_rad * 0.5f;
        vec3 vn = normalize(axis);
        sinAngle = sin(angle);
        vn *= sinAngle;
        quat result(vn.array_sse);
		_mm_f32_(result.array_sse,3) = cos(angle);
        return result;
#elif defined(ARIBEIRO_NEON)
        float sinAngle;
        float angle = angle_rad * 0.5f;
        vec3 vn = normalize(axis);
        sinAngle = sin(angle);
        vn *= sinAngle;
        quat result(vn.array_neon);
		result.array_neon[3] = cos(angle);
        return result;
#else
        float sinAngle;
        float angle = angle_rad * 0.5f;
        vec3 vn = normalize(axis);
        sinAngle = sin(angle);
        return quat((vn.x * sinAngle),
                    (vn.y * sinAngle),
                    (vn.z * sinAngle),
                    cos(angle));
#endif
    }
    //------------------------------------------------------------------------------
    /// \brief Constructs a quaternion from euler angles in radians.
    ///
    /// \author Alessandro Ribeiro
    /// \param pitch radians
    /// \param yaw radians
    /// \param roll radians
    /// \return The quat
    ///
    ARIBEIRO_INLINE quat quatFromEuler(float roll, float pitch, float yaw){

        pitch *= 0.5f;
        yaw *= 0.5f;
        roll *= 0.5f;

#if defined(ARIBEIRO_SSE2)

        __m128 rollSinCos = _mm_load_(sin(roll),cos(roll),0,0);
        __m128 pitchSinCos = _mm_load_(sin(pitch),cos(pitch),0,0);
        __m128 yawSinCos = _mm_load_(sin(yaw),cos(yaw),0,0);

        __m128 row0 = _mm_shuffle_ps(rollSinCos, rollSinCos, _MM_SHUFFLE(1,1,1,0));
        __m128 row1 = _mm_shuffle_ps(rollSinCos, rollSinCos, _MM_SHUFFLE(0,0,0,1));

        __m128 pitch0 = _mm_shuffle_ps(pitchSinCos, pitchSinCos, _MM_SHUFFLE(1,1,0,1));
        __m128 pitch1 = _mm_shuffle_ps(pitchSinCos, pitchSinCos, _MM_SHUFFLE(0,0,1,0));

        __m128 yaw0 = _mm_shuffle_ps(yawSinCos, yawSinCos, _MM_SHUFFLE(1,0,1,1));
        __m128 yaw1 = _mm_shuffle_ps(yawSinCos, yawSinCos, _MM_SHUFFLE(0,1,0,0));

        __m128 mul0 = _mm_mul_ps(row0, pitch0);
        mul0 = _mm_mul_ps(mul0, yaw0);

        __m128 mul1 = _mm_mul_ps(row1, pitch1);
        mul1 = _mm_mul_ps(mul1, yaw1);

        //static const __m128 _mask = _mm_load_(-1.0f,1.0f,-1.0f,1.0f);
        static const __m128 _mask_xor = _mm_load_(-0.0f,0.0f,-0.0f,0.0f);

        //mul1 = _mm_mul_ps(mul1, _mask );
        mul1 = _mm_xor_ps(mul1, _mask_xor );//much faster

        return _mm_add_ps(mul0, mul1);
#elif defined(ARIBEIRO_NEON)


        float32x4_t rollSinCos = (float32x4_t){(float)sin(roll),(float)cos(roll),0,0};
        float32x4_t pitchSinCos = (float32x4_t){(float)sin(pitch),(float)cos(pitch),0,0};
        float32x4_t yawSinCos = (float32x4_t){(float)sin(yaw),(float)cos(yaw),0,0};

        float32x4_t row0 = vshuffle_1110(rollSinCos);
        float32x4_t row1 = vshuffle_0001(rollSinCos);

        float32x4_t pitch0 = vshuffle_1101(pitchSinCos);
        float32x4_t pitch1 = vshuffle_0010(pitchSinCos);

        float32x4_t yaw0 = vshuffle_1011(yawSinCos);
        float32x4_t yaw1 = vshuffle_0100(yawSinCos);

        float32x4_t mul0 = vmulq_f32(row0, pitch0);
        mul0 = vmulq_f32(mul0, yaw0);

        float32x4_t mul1 = vmulq_f32(row1, pitch1);
        mul1 = vmulq_f32(mul1, yaw1);
        mul1 = vmulq_f32(mul1, (float32x4_t){-1.0f,1.0f,-1.0f,1.0f} );

        return vaddq_f32(mul0, mul1);


#else

        float sinPitch = sin(pitch);
        float cosPitch = cos(pitch);
        float sinYaw = sin(yaw);
        float cosYaw = cos(yaw);
        float sinRoll = sin(roll);
        float cosRoll = cos(roll);

        float cosPitchCosYaw = cosPitch * cosYaw;
        float sinPitchSinYaw = sinPitch * sinYaw;

        return quat(
                    sinRoll * cosPitchCosYaw - cosRoll * sinPitchSinYaw,
                    cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
                    cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
                    cosRoll * cosPitchCosYaw + sinRoll * sinPitchSinYaw
                    );
#endif

        /*
         return
         quatFromAxisAngle(vec3(0.0, 0.0, 1.0), yaw) *
         quatFromAxisAngle(vec3(0.0, 1.0, 0.0), pitch) *
         quatFromAxisAngle(vec3(1.0, 0.0, 0.0), roll);
         */

    }
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    /// \brief Construct a 4x4 transformation matrix from a quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param q The rotation quaternion
    /// \return The mat4
    ///
    ARIBEIRO_INLINE mat4 toMat4(const quat& q){

#if defined(ARIBEIRO_SSE2)


        __m128 x2y2z2 = _mm_mul_ps(q.array_sse, q.array_sse);

        __m128 op0 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 1, 0, 0));
        __m128 op1 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 2, 2, 1));

        __m128 xy_xz_yz = _mm_mul_ps(op0, op1);

        __m128 wx_wy_wz = _mm_mul_ps(

                                     _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(3,3,3,3))

                                     //_mm_set1_ps(_mm_f32_(q.array_sse,3))
                                     ,
                                     q.array_sse);

        static const __m128 _2 = _mm_set1_ps(2.0f);


        __m128 m0 =
        _mm_mul_ps(_2, _mm_load_(
           ( _mm_f32_(x2y2z2,1) + _mm_f32_(x2y2z2,2)),
           (_mm_f32_(xy_xz_yz,0) + _mm_f32_(wx_wy_wz,2)),
           (_mm_f32_(xy_xz_yz,1) - _mm_f32_(wx_wy_wz,1)),
           0
        ));

		_mm_f32_(m0, 0) = 1.0f - _mm_f32_(m0, 0);


        __m128 m1 =
        _mm_mul_ps(_2, _mm_load_(
            (_mm_f32_(xy_xz_yz,0) - _mm_f32_(wx_wy_wz,2)),
            (_mm_f32_(x2y2z2,0) + _mm_f32_(x2y2z2,2)),
            (_mm_f32_(xy_xz_yz,2) + _mm_f32_(wx_wy_wz,0)),
            0
        ));
		_mm_f32_(m1, 1) = 1.0f - _mm_f32_(m1, 1);

        __m128 m2 =
        _mm_mul_ps(_2, _mm_load_(
            (_mm_f32_(xy_xz_yz,1) + _mm_f32_(wx_wy_wz,1)),
            (_mm_f32_(xy_xz_yz,2) - _mm_f32_(wx_wy_wz,0)),
            (_mm_f32_(x2y2z2,0) + _mm_f32_(x2y2z2,1)),
            0
        ));

		_mm_f32_(m2, 2) = 1.0f - _mm_f32_(m2, 2);

        //static const __m128 m3 = _mm_load_(0,0,0,1.0f);

        return mat4(m0,m1,m2,mat4::IdentityMatrix.array_sse[3]);//m3


        /*

        __m128 elm0 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 0, 0, 1));
        __m128 elm1 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 2, 1, 1));
        __m128 op0 = _mm_mul_ps(elm0, elm1);
        elm0 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 3, 3, 2));
        elm1 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 1, 2, 2));
        __m128 op1 = _mm_mul_ps(elm0, elm1);
        op1 = _mm_mul_ps((__m128){1.0f,1.0f,-1.0f,0.0f}, op1);

        __m128 m0_ = _mm_add_ps(op0, op1);
        m0_ = _mm_mul_ps((__m128){-2.0f,2.0f,2.0f,0.0f}, m0_);
        m0_[0] += 1.0f;


        elm0 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 1, 0, 0));
        elm1 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 2, 0, 1));
        op0 = _mm_mul_ps(elm0, elm1);
        elm0 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 3, 2, 3));
        __m128 elm1_s = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 0, 2, 2));
        op1 = _mm_mul_ps(elm0, elm1_s);
        op1 = _mm_mul_ps((__m128){-1.0f,1.0f,1.0f,0.0f}, op1);

        __m128 m1_ = _mm_add_ps(op0, op1);
        m1_ = _mm_mul_ps((__m128){2.0f,-2.0f,2.0f,0.0f}, m1_);
        m1_[1] += 1.0f;


        elm0 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 0, 1, 0));
        //elm1 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 0, 2, 2));
        op0 = _mm_mul_ps(elm0, elm1_s);
        elm0 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 1, 3, 3));
        elm1 = _mm_shuffle_ps(q.array_sse, q.array_sse, _MM_SHUFFLE(0, 1, 0, 1));
        op1 = _mm_mul_ps(elm0, elm1);
        op1 = _mm_mul_ps((__m128){1.0f,-1.0f,1.0f,0.0f}, op1);

        __m128 m2_ = _mm_add_ps(op0, op1);
        m2_ = _mm_mul_ps((__m128){2.0f,2.0f,-2.0f,0.0f}, m2_);
        m2_[2] += 1.0f;

        __m128 m3_ = (__m128){0,0,0,1.0f};

        return mat4(m0_,m1_,m2_,m3_);

        */

        /*
        float x2 = q.x * q.x;
        float y2 = q.y * q.y;
        float z2 = q.z * q.z;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;
        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;

        __m128 m0 = (__m128){
            1.0f - 2.0f * (y2 + z2),
            2.0f * (xy + wz),
            2.0f * (xz - wy),
            0
        };

        __m128 m1 = (__m128){
            2.0f * (xy - wz),
            1.0f - 2.0f * (x2 + z2),
            2.0f * (yz + wx),
            0
        };

        __m128 m2 = (__m128){
            2.0f * (xz + wy),
            2.0f * (yz - wx),
            1.0f - 2.0f * (x2 + y2),
            0
        };

        __m128 m3 = (__m128){0,0,0,1.0f};

        return mat4(m0,m1,m2,m3);

         */
#elif defined(ARIBEIRO_NEON)

        float32x4_t x2y2z2 = vmulq_f32(q.array_neon, q.array_neon);

        float32x4_t op0 = vshuffle_0100(q.array_neon);
        float32x4_t op1 = vshuffle_0221(q.array_neon);

        float32x4_t xy_xz_yz = vmulq_f32(op0, op1);
        float32x4_t wx_wy_wz = vmulq_f32( vshuffle_3333(q.array_neon) , q.array_neon);

        static const float32x4_t _2 = vset1(2.0f);


        float32x4_t m0 =
        vmulq_f32(_2, (float32x4_t){
           ( x2y2z2[1] + x2y2z2[2]  ),
           ( xy_xz_yz[0] + wx_wy_wz[2]),
           ( xy_xz_yz[1] - wx_wy_wz[1]),
           0
        });

		m0[0] = 1.0f - m0[0];


        float32x4_t m1 =
        vmulq_f32(_2, (float32x4_t){
            (xy_xz_yz[0] - wx_wy_wz[2]),
            (x2y2z2[0] + x2y2z2[2]),
            (xy_xz_yz[2] + wx_wy_wz[0]),
            0
        });
		m1[1] = 1.0f - m1[1];

        float32x4_t m2 =
        vmulq_f32(_2, (float32x4_t){
            (xy_xz_yz[1] + wx_wy_wz[1]),
            (xy_xz_yz[2] - wx_wy_wz[0]),
            (x2y2z2[0] + x2y2z2[1]),
            0
        });

		m2[2] = 1.0f - m2[2];

        static const float32x4_t m3 = (float32x4_t){0,0,0,1.0f};

        return mat4(m0,m1,m2,m3);

#else

		float x2 = q.x * q.x;
		float y2 = q.y * q.y;
		float z2 = q.z * q.z;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;
		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;


        // This calculation would be a lot more complicated for non-unit length quaternions
        // Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
        //   OpenGL
        return mat4(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
                    2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
                    2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
#endif

    }
    //------------------------------------------------------------------------------
    /// \brief Convert the quaternion to an axis angle representation. Notice: Not tested
    ///
    /// \author Alessandro Ribeiro
    /// \param q The rotation quaternion
    /// \param axis Output - The axis
    /// \param angle Output - The angle arount the axis in radians
    ///
    ARIBEIRO_INLINE void extractAxisAngle(const quat& q, vec3 *axis, float *angle){
#if defined(ARIBEIRO_SSE2)
        __m128 dot0 = dot_sse_3(q.array_sse,q.array_sse);
        __m128 inv_length = _mm_set1_ps(1.0f/sqrtf(_mm_f32_(dot0,0)));
        __m128 result = _mm_mul_ps(q.array_sse, inv_length);
        axis->array_sse = result;
        *angle = acos(clamp(q.w,-1.0f,1.0f)) * 2.0f;
#elif defined(ARIBEIRO_NEON)
        float32x4_t dot0 = dot_neon_3(q.array_neon,q.array_neon);
        float32x4_t inv_length = vset1(1.0f/sqrtf(dot0[0]));
        float32x4_t result = vmulq_f32(q.array_neon, inv_length);
        axis->array_neon = result;
        *angle = acos(clamp(q.w,-1.0f,1.0f)) * 2.0f;
#else
        float scale = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
        axis->x = q.x / scale;
        axis->y = q.y / scale;
        axis->z = q.z / scale;
        *angle = acos(clamp(q.w,-1.0f,1.0f)) * 2.0f;
#endif
    }
	//------------------------------------------------------------------------------

	/// \brief Convert the quaternion to Euler representation. Notice: Not found an algorithm that works...
    ///
    /// \author Alessandro Ribeiro
    /// \param q The rotation quaternion
    /// \param roll radians
	/// \param pitch radians
	/// \param yaw radians
    ///
    ARIBEIRO_INLINE void extractEuler(const quat &q, float *roll,float *pitch,float *yaw ){
        extractEuler(toMat4(q), roll, pitch, yaw);
    }

	//------------------------------------------------------------------------------
    /// \brief Computes the inverse of a quaternion. Notice
    ///
    /// \author Alessandro Ribeiro
    /// \param q The rotation quaternion
    /// \return The inverse of the quaternion
    ///
    ARIBEIRO_INLINE quat inv( const quat &q ){
        return conjugate(q);
    }


	//
	// Move functions
	//
	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa move(float current, float target, float maxDistanceVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
	//ARIBEIRO_API float move(float current, float target, float maxDistanceVariation);

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa move(float current, float target, float maxDistanceVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
    ARIBEIRO_INLINE vec2 move(const vec2 &current, const vec2 &target, const float &maxDistanceVariation){
        //const float EPSILON = 1e-6f;
        float deltaDistance = distance(current, target);
        if (deltaDistance < maxDistanceVariation + EPSILON)
            return target;
        return lerp(current, target, maxDistanceVariation / deltaDistance);
    }

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa move(float current, float target, float maxDistanceVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
    ARIBEIRO_INLINE vec3 move(const vec3 &current, const vec3 &target, float maxDistanceVariation){
        //const float EPSILON = 1e-6f;
        float deltaDistance = distance(current, target);
        if (deltaDistance < maxDistanceVariation + EPSILON)
            return target;
        return lerp(current, target, maxDistanceVariation / deltaDistance);
    }

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa move(float current, float target, float maxDistanceVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
    ARIBEIRO_INLINE vec4 move(const vec4 &current, const vec4 &target, float maxDistanceVariation){
        //const float EPSILON = 1e-6f;
        float deltaDistance = distance(current, target);
        if (deltaDistance < maxDistanceVariation + EPSILON)
            return target;
        return lerp(current, target, maxDistanceVariation / deltaDistance);
    }

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa moveSlerp(const vec3 &current, const vec3 &target, float maxAngleVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
    ARIBEIRO_INLINE vec3 moveSlerp(const vec3 &currentParam, const vec3 &targetParam, const float &maxAngleVariation){
        vec3 current = currentParam;
        vec3 target = targetParam;
        //const float EPSILON = 1e-6f;

        float lengthCurrent = length(current);
        float lengthTarget = length(target);

        //trying to interpolate from zero vector
        if (lengthCurrent < EPSILON2)
            return targetParam;
        //trying to interpolate to zero vector
        if (lengthTarget < EPSILON2)
            return currentParam;

        current *= (1.0f/lengthCurrent);
        target *= (1.0f/lengthTarget);

        //float deltaAngle = angleBetween(current, target);
        float deltaAngle = acos(clamp(dot(current,target),-1.0f,1.0f));
        if (deltaAngle < maxAngleVariation + EPSILON)
            return target;

        // 180º case interpolation -- force one orientation based on eulerAngle
        if (deltaAngle >= PI - EPSILON){
            static const quat fixedRotation = quatFromEuler(DEG2RAD(0.5f),DEG2RAD(0.5f),DEG2RAD(0.5f));
            current = fixedRotation * current;
            deltaAngle = angleBetween(current, target);
        }

        float lrpFactor = maxAngleVariation / deltaAngle;
        vec3 result = slerp(current, target, lrpFactor);

        result = normalize(result) * lerp(lengthCurrent, lengthTarget, lrpFactor);

        return result;
    }

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa moveSlerp(const quat &current, const quat &target, float maxAngleVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
    ARIBEIRO_INLINE quat moveSlerp(const quat &currentParam, const quat &target, const float &maxAngleVariation){
        quat current = currentParam;

        //const float EPSILON = 1e-6f;
        float deltaAngle = angleBetween(current, target);
        if (deltaAngle < maxAngleVariation + EPSILON)
            return target;

        // 180º case interpolation -- force one orientation based on eulerAngle
        if (deltaAngle >= PI - EPSILON){
            static const quat fixedRotation = quatFromEuler(DEG2RAD(0.5f),DEG2RAD(0.5f),DEG2RAD(0.5f));
            current = fixedRotation * current;
            deltaAngle = angleBetween(current, target);
        }

        return slerp(current, target, maxAngleVariation / deltaAngle);
    }

}


#endif
