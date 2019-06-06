#ifndef vec2_h
#define vec2_h

#include <aribeiro/common.h>
#include <aribeiro/SSE2.h>
#include <aribeiro/floatOPs.h>

namespace aRibeiro{
    
#if defined(ARIBEIRO_SSE2)
    
    extern const __m128 _vec2_zero_sse;
    extern const __m128 _vec2_sign_mask; // -0.f = 1 << 31
    
#pragma pack(push, 16)
#endif

/// \brief Vector 2D (vec2)
///
/// Stores two components(x,y) to represent a bidimensional vector. <br/>
/// It can be used as points or vectors in 2D.
/// \author Alessandro Ribeiro
///
_SSE2_ALIGN_PRE class ARIBEIRO_API vec2{
    public:
	_SSE2_ALIGN_PRE union {
	  _SSE2_ALIGN_PRE float array[2] _SSE2_ALIGN_POS;///<The 2D low level array representation to pass the vector as pointer parameter
	  _SSE2_ALIGN_PRE struct{ float x,y; } _SSE2_ALIGN_POS;///<Components X and Y to be used by the application
#if defined(ARIBEIRO_SSE2)
	  _SSE2_ALIGN_PRE __m128 array_sse _SSE2_ALIGN_POS;
#endif
    }_SSE2_ALIGN_POS;
    
#if defined(ARIBEIRO_SSE2)
    //special SSE2 constructor
    ARIBEIRO_INLINE vec2( const __m128 &v ){
        array_sse = v;
    }
#endif
    
    /// \brief Construct a ZERO vec2 class
    ///
    /// The ZERO vec2 class have the point information in the origin (x=0,y=0)
    /// \author Alessandro Ribeiro
    ///
    ARIBEIRO_INLINE vec2(){
        x = y = 0.0f;
    }
    /// \brief Constructs a bidimensional Vector
    ///
    /// Initialize the vec2 components with the same float value
    ///
    /// X = v and Y = v
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to initialize the components
    ///
    ARIBEIRO_INLINE vec2( const float &v ){
        x = y = v;
    }
    /// \brief Constructs a bidimensional Vector
    ///
    /// Initialize the vec2 components from the parameters
    ///
    /// \author Alessandro Ribeiro
    /// \param x Value to assign to the X component of the vector
    /// \param y Value to assign to the Y component of the vector
    ///
    ARIBEIRO_INLINE vec2( const float &x, const float &y ){
        this->x = x;
        this->y = y;
    }
    /// \brief Constructs a bidimensional Vector
    ///
    /// Initialize the vec2 components from other vec2 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to assign to the instance
    ///
    ARIBEIRO_INLINE vec2( const vec2 &v ){
        *this = v;
    }
    /// \brief Constructs a bidimensional Vector from the subtraction b-a
    ///
    /// Initialize the vec2 components from two other vectors using the equation: <br />
    /// this = b - a
    ///
    /// \author Alessandro Ribeiro
    /// \param a Orign vector
    /// \param b Destiny vector
    ///
    ARIBEIRO_INLINE vec2( const vec2 &a, const vec2 &b ){
        x = b.x - a.x;
        y = b.y - a.y;
    }

    /// \brief Binary comparison of vectors
    ///
    /// Compare two vectors using memcmp to see if they are the same.
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to compare against
    /// \return true if the bits of this is the same of v, otherwise false.
    ///
    ARIBEIRO_INLINE bool operator==(const vec2&v) const {
        for(int i=0;i<2;i++){
            if (absv(array[i]-v.array[i]) > 1e-4f)
                return false;
        }
        return true;
        //return memcmp(array, v.array, sizeof(float) * 2) == 0;
    }
    ARIBEIRO_INLINE bool operator!=(const vec2&v) const{
        return !((*this) == v);
        //return memcmp(array, v.array, sizeof(float) * 2) != 0;
    }

    /// \brief Component-wise increment operator overload
    ///
    /// Increment the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to increment the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    ARIBEIRO_INLINE vec2& operator+=(const vec2& v){
        x+=v.x;
        y+=v.y;
        return (*this);
    }

    /// \brief Component-wise decrement operator overload
    ///
    /// Decrement the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to decrement the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    ARIBEIRO_INLINE vec2& operator-=(const vec2& v){
        x-=v.x;
        y-=v.y;
        return (*this);
    }

    /// \brief Component-wise minus operator overload
    ///
    /// Negates the vector components with the operator minus
    ///
    /// \author Alessandro Ribeiro
    /// \return A copy of the current instance after the negation operation
    ///
    ARIBEIRO_INLINE vec2 operator-()const{
        return vec2(-x,-y);
    }

    /// \brief Component-wise multiply operator overload
    ///
    /// Multiply the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to multiply the current vector instance
    /// \return A reference to the current instance after the multiplication
    ///
    ARIBEIRO_INLINE vec2& operator*=(const vec2& v){
        x*=v.x;
        y*=v.y;
        return (*this);
    }

    /// \brief Component-wise division operator overload
    ///
    /// Divides the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to divide the current vector instance
    /// \return A reference to the current instance after the division
    ///
    ARIBEIRO_INLINE vec2& operator/=(const vec2& v){
        x/=v.x;
        y/=v.y;
        return (*this);
    }

    /// \brief Single value increment operator overload
    ///
    /// Increment the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to increment all components of the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    ARIBEIRO_INLINE vec2& operator+=(const float &v){
        x+=v;
        y+=v;
        return (*this);
    }

    /// \brief Single value decrement operator overload
    ///
    /// Decrement the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    ARIBEIRO_INLINE vec2& operator-=(const float &v){
        x-=v;
        y-=v;
        return (*this);
    }

    /// \brief Single value multiply operator overload
    ///
    /// Decrement the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    ARIBEIRO_INLINE vec2& operator*=(const float &v){
        x*=v;
        y*=v;
        return (*this);
    }

    /// \brief Single value division operator overload
    ///
    /// Divides the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to divide all components of the current vector instance
    /// \return A reference to the current instance after the division
    ///
    ARIBEIRO_INLINE vec2& operator/=(const float &v){
        x/=v;
        y/=v;
        return (*this);
    }

    /// \brief Index the components of the vec2 as a C array
    ///
    /// \author Alessandro Ribeiro
    /// \param v The index of the components starting by 0
    /// \return A reference to the element at the index v
    ///
    ARIBEIRO_INLINE float& operator[](const int v){
        return array[v];
    }
    ARIBEIRO_INLINE float operator[](const int v)const{
        return array[v];
    }
    
}_SSE2_ALIGN_POS;
    
INLINE_OPERATION_IMPLEMENTATION(vec2)

#if defined(ARIBEIRO_SSE2)
#pragma pack(pop)
#endif
    
}

#endif
