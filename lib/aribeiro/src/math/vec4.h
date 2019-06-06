#ifndef vec4_h
#define vec4_h

#include <aribeiro/common.h>
#include <aribeiro/vec3.h>
#include <aribeiro/SSE2.h>
#include <aribeiro/floatOPs.h>


namespace aRibeiro{

class vec3;
    
#if defined(ARIBEIRO_SSE2)
    
    extern const __m128 _vec4_zero_sse;
    extern const __m128 _vec4_sign_mask; // -0.f = 1 << 31
    
#pragma pack(push, 16)
#endif
    

/// \brief Homogeneous 3D (vec4)
///
/// Stores three components(x,y,z,w) to represent a tridimensional vector with the homogeneous component w. <br/>
/// It can be used to represent points or vectors in 3D.
/// \author Alessandro Ribeiro
///
_SSE2_ALIGN_PRE class ARIBEIRO_API vec4{
    public:
	_SSE2_ALIGN_PRE union {
		_SSE2_ALIGN_PRE float array[4] _SSE2_ALIGN_POS;///<The 4D low level array representation to pass the vector as pointer parameter
		_SSE2_ALIGN_PRE struct{ float x,y,z,w; } _SSE2_ALIGN_POS;///<Components X, Y, Z and W to be used by the application
		_SSE2_ALIGN_PRE struct{ float r,g,b,a; } _SSE2_ALIGN_POS;///<Components R, G, B and a to be used by the application
#if defined(ARIBEIRO_SSE2)
		_SSE2_ALIGN_PRE __m128 array_sse _SSE2_ALIGN_POS;
#endif
    }_SSE2_ALIGN_POS;
    
#if defined(ARIBEIRO_SSE2)
    //special SSE2 constructor
    ARIBEIRO_INLINE vec4( const __m128 &v ){
        array_sse = v;
    }
#endif
    
    /// \brief Construct a ZERO vec4 class
    ///
    /// The ZERO vec4 class have the point information in the origin as a vector (x=0,y=0,z=0,w=0)
    /// \author Alessandro Ribeiro
    ///
    ARIBEIRO_INLINE vec4(){
#if defined(ARIBEIRO_SSE2)
        array_sse = _vec4_zero_sse;
#else
        x = y = z = w = 0.0f;
#endif
    }
    /// \brief Constructs a tridimensional Vector with homogeneous component
    ///
    /// Initialize the vec3 components with the same float value
    ///
    /// X = v, Y = v, Z = v and W = v
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to initialize the components
    ///
    ARIBEIRO_INLINE vec4( const float &v ){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_set1_ps(v);
#else
        x = y = z = w = v;
#endif
    }
    /// \brief Constructs a tridimensional Vector with homogeneous component
    ///
    /// Initialize the vec4 components from the parameters
    ///
    /// \author Alessandro Ribeiro
    /// \param x Value to assign to the X component of the vector
    /// \param y Value to assign to the Y component of the vector
    /// \param z Value to assign to the Z component of the vector
    /// \param w Value to assign to the W component of the vector
    ///
    ARIBEIRO_INLINE vec4( const float &x, const float &y, const float &z, const float &w ){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_load_( x, y, z, w );
#else
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
#endif
    }
    
    /// \brief Constructs a tridimensional Vector with homogeneous component
    ///
    /// Initialize the vec4 components from a vec3 xyz and an isolated w value
    ///
    /// this->xyz = xyz <br />
    /// this->w = w
    ///
    /// If the w is 0 the class represent a vector. <br />
    /// If the w is 1 the class represent a point. <br />
    /// Otherwise it might have a result of a projection
    ///
    /// \author Alessandro Ribeiro
    /// \param xyz Vector 3D to assign to the components x, y and Z of the instance respectively
    /// \param w Value to assign to the component w of the instance
    ///
    ARIBEIRO_INLINE vec4( const vec3 &xyz , const float &w){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_load_( xyz.x, xyz.y, xyz.z, w );
#else
        x = xyz.x;
        y = xyz.y;
        z = xyz.z;
        this->w = w;
#endif
    }
    /// \brief Constructs a tridimensional Vector with homogeneous component
    ///
    /// Initialize the vec4 components from an isolated x value and a vec3 yzw
    ///
    /// this->x = x <br />
    /// this->yzw = yzw
    ///
    /// \author Alessandro Ribeiro
    /// \param x Value to assign to the component x of the instance
    /// \param yzw Vector 3D to assign to the components y, z and w of the instance respectively
    ///
    ARIBEIRO_INLINE vec4( const float &x, const vec3 &yzw){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_load_(x, yzw.x, yzw.y, yzw.z );
#else
        this->x = x;
        y = yzw.x;
        z = yzw.y;
        w = yzw.z;
#endif
    }
    /// \brief Constructs a tridimensional Vector with homogeneous component
    ///
    /// Initialize the vec4 components from other vec4 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to assign to the instance
    ///
    ARIBEIRO_INLINE vec4( const vec4 &v ){
#if defined(ARIBEIRO_SSE2)
        array_sse = v.array_sse;
#else
        *this = v;
#endif
    }
    /// \brief Constructs a tridimensional Vector with homogeneous component from the subtraction b-a
    ///
    /// Initialize the vec4 components from two other vectors using the equation: <br />
    /// this = b - a
    ///
    /// If a and b were points then the result will be a vector pointing from a to b with the w component equals to zero.
    ///
    /// \author Alessandro Ribeiro
    /// \param a Orign point
    /// \param b Destiny point
    ///
    ARIBEIRO_INLINE vec4( const vec4 &a, const vec4 &b ){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_sub_ps(a.array_sse, b.array_sse);
#else
        x = b.x - a.x;
        y = b.y - a.y;
        z = b.z - a.z;
        w = b.w - a.w;
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
    ARIBEIRO_INLINE bool operator==(const vec4&v) const {
#if defined(ARIBEIRO_SSE2)
        
        __m128 diff_abs = _mm_sub_ps(array_sse, v.array_sse);
        //abs
        diff_abs = _mm_andnot_ps(_vec4_sign_mask, diff_abs);
        
        //static const __m128 epsilon = _mm_set1_ps(1e-4f); // -0.f = 1 << 31
        //_mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));
        
        for(int i=0;i<4;i++){
            if ( _mm_f32_(diff_abs,i) > 1e-4f)
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
    
    ARIBEIRO_INLINE bool operator!=(const vec4&v) const{
        return !((*this) == v);
        //return memcmp(array, v.array, sizeof(float) * 4) != 0;
    }
    
    /// \brief Component-wise increment operator overload
    ///
    /// Increment the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to increment the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    ARIBEIRO_INLINE vec4& operator+=(const vec4& v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_add_ps(array_sse, v.array_sse);
#else
        x+=v.x;
        y+=v.y;
        z+=v.z;
        w+=v.w;
#endif
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
    ARIBEIRO_INLINE vec4& operator-=(const vec4& v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_sub_ps(array_sse, v.array_sse);
#else
        x-=v.x;
        y-=v.y;
        z-=v.z;
        w-=v.w;
#endif
        return (*this);
    }
    
    /// \brief Component-wise minus operator overload
    ///
    /// Negates the vector components with the operator minus
    ///
    /// \author Alessandro Ribeiro
    /// \return A copy of the current instance after the negation operation
    ///
    ARIBEIRO_INLINE vec4 operator-() const{
#if defined(ARIBEIRO_SSE2)
        return _mm_xor_ps(_vec4_sign_mask, array_sse);
#else
        return vec4(-x,-y,-z,-w);
#endif
    }
    /// \brief Component-wise multiply operator overload
    ///
    /// Multiply the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to multiply the current vector instance
    /// \return A reference to the current instance after the multiplication
    ///
    ARIBEIRO_INLINE vec4& operator*=(const vec4& v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_mul_ps(array_sse, v.array_sse);
#else
        x*=v.x;
        y*=v.y;
        z*=v.z;
        w*=v.w;
#endif
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
    ARIBEIRO_INLINE vec4& operator/=(const vec4& v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_div_ps(array_sse, v.array_sse);
#else
        x/=v.x;
        y/=v.y;
        z/=v.z;
        w/=v.w;
#endif
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
    ARIBEIRO_INLINE vec4& operator+=(const float &v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_add_ps(array_sse, _mm_set1_ps(v));
#else
        x+=v;
        y+=v;
        z+=v;
        w+=v;
#endif
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
    ARIBEIRO_INLINE vec4& operator-=(const float &v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_sub_ps(array_sse, _mm_set1_ps(v));
#else
        x-=v;
        y-=v;
        z-=v;
        w-=v;
#endif
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
    ARIBEIRO_INLINE vec4& operator*=(const float &v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_mul_ps(array_sse, _mm_set1_ps(v));
#else
        x*=v;
        y*=v;
        z*=v;
        w*=v;
#endif
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
    ARIBEIRO_INLINE vec4& operator/=(const float &v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_div_ps(array_sse, _mm_set1_ps(v));
#else
        x/=v;
        y/=v;
        z/=v;
        w/=v;
#endif
        return (*this);
    }
    /// \brief Index the components of the vec4 as a C array
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

	SSE2_CLASS_NEW_OPERATOR

}_SSE2_ALIGN_POS;
    
INLINE_OPERATION_IMPLEMENTATION(vec4)

#if defined(ARIBEIRO_SSE2)
#pragma pack(pop)
#endif
    
}

#endif
