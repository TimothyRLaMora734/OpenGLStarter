
#ifndef floatOPs__H
#define floatOPs__H

#include "constants.h"
#include <aribeiro/SSE2.h>

namespace aRibeiro {

const float _float_bitsign = -.0f; // -0.f = 1 << 31
const uint32_t _float_bitsign_uint32_t = (*(uint32_t*)(&_float_bitsign));
    
const float _float_one = 1.0f;
const uint32_t _float_one_uint32_t = (*(uint32_t*)(&_float_one));
    
/// \brief Compute the absolute value of a number
///
/// \author Alessandro Ribeiro
/// \param a A vector
/// \return |a|
///
ARIBEIRO_INLINE float absv(float a){
    uint32_t result = ((~_float_bitsign_uint32_t) & (*(uint32_t*)&a));
    return *((float*)&result);
    //return (~_float_bitsign) & a;
    //return (a < 0) ? (-a) : (a);
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
ARIBEIRO_INLINE float clamp(const float &value,const float &min,const float &max ) {
#if defined(ARIBEIRO_SSE2)
    __m128 maxStep = _mm_max_ss(_mm_set_ss(value), _mm_set_ss(min));
    __m128 minStep = _mm_min_ss(maxStep, _mm_set_ss(max));
    return _mm_f32_(minStep,0);
#else
    return (value < min) ? min : ((value > max) ? max : value);
#endif
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
ARIBEIRO_INLINE float sqrDistance(const float &a, const float &b) {
    float ab = b - a;
    return ab * ab;
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
ARIBEIRO_INLINE float distance(const float &a,const float &b) {
    float ab = b - a;
    //return sqrtf(ab*ab);
    return absv(ab);
}


/// \brief Return the maximum between the two parameters
///
/// \author Alessandro Ribeiro
/// \param a A value to test
/// \param b A value to test
/// \return The maximum value of the parameters
///
ARIBEIRO_INLINE float maximum(const float &a,const float &b){
#if defined(ARIBEIRO_SSE2)
    return _mm_f32_(_mm_max_ss(_mm_set_ss(a),_mm_set_ss(b)),0);
#else
    return (a > b) ? a : b;
#endif
}


/// \brief Return the minimum between the two parameters
///
/// \author Alessandro Ribeiro
/// \param a A value to test
/// \param b A value to test
/// \return The minimum value of the parameters
///
ARIBEIRO_INLINE float minimum(const float &a, const float &b){
#if defined(ARIBEIRO_SSE2)
    return _mm_f32_(_mm_min_ss(_mm_set_ss(a),_mm_set_ss(b)),0);
#else
    return (a < b) ? a : b;
#endif
}


/// \brief Compute the sign of a float
///
/// if a >=0 then sign = 1
/// else sign = -1
///
/// \author Alessandro Ribeiro
/// \param a The value to test
/// \return The sign of a
///
ARIBEIRO_INLINE float sign(const float &a) {
    //uint32_t result = _float_bitsign_uint32_t & (*(uint32_t*)&a);
    //result |= _float_one_uint32_t;
    //return *((float*)&result);
    //return a & _float_bitsign;
    return (a >= 0) ? 1.0f : -1.0f;
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
ARIBEIRO_INLINE float lerp(const float &a,const  float &b,const float &factor){
    //  return a+(b-a)*factor;
    return a * (1.0f - factor) + (b*factor);
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
ARIBEIRO_INLINE float move(const float &current, const float &target, const float &maxDistanceVariation){
    const float EPSILON = 1e-6f;
    float deltaDistance = distance(current, target);
    if (deltaDistance < maxDistanceVariation + EPSILON)
        return target;
    return lerp(current, target, maxDistanceVariation / deltaDistance);
}

    
    
#define INLINE_OPERATION_IMPLEMENTATION(TTYPE)\
static ARIBEIRO_INLINE TTYPE operator/( const TTYPE& vecA, const TTYPE& vecB ){ return (TTYPE(vecA)/=vecB); } \
static ARIBEIRO_INLINE TTYPE operator/( const TTYPE& vec , const float value ){ return (TTYPE(vec)/=value); } \
static ARIBEIRO_INLINE TTYPE operator/( const float value, const TTYPE& vec  ){ return (TTYPE(value)/=vec); } \
static ARIBEIRO_INLINE TTYPE operator*( const TTYPE& vecA, const TTYPE& vecB ){ return (TTYPE(vecA)*=vecB); } \
static ARIBEIRO_INLINE TTYPE operator*( const TTYPE& vec , const float value ){ return (TTYPE(vec)*=value); } \
static ARIBEIRO_INLINE TTYPE operator*( const float value, const TTYPE& vec  ){ return (TTYPE(value)*=vec); } \
static ARIBEIRO_INLINE TTYPE operator+( const TTYPE& vecA, const TTYPE& vecB ){ return (TTYPE(vecA)+=vecB); } \
static ARIBEIRO_INLINE TTYPE operator+( const TTYPE& vec , const float value ){ return (TTYPE(vec)+=value); } \
static ARIBEIRO_INLINE TTYPE operator+( const float value, const TTYPE& vec  ){ return (TTYPE(value)+=vec); } \
static ARIBEIRO_INLINE TTYPE operator-( const TTYPE& vecA, const TTYPE& vecB ){ return (TTYPE(vecA)-=vecB); } \
static ARIBEIRO_INLINE TTYPE operator-( const TTYPE& vec , const float value ){ return (TTYPE(vec)-=value); } \
static ARIBEIRO_INLINE TTYPE operator-( const float value, const TTYPE& vec  ){ return (TTYPE(value)-=vec); }
    
    
}
    
#endif

