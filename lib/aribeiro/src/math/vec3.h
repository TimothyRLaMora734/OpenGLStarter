#ifndef vec3_h
#define vec3_h

#include <aribeiro/common.h>
#include <aribeiro/vec2.h>
#include <aribeiro/SSE2.h>
#include <aribeiro/floatOPs.h>

namespace aRibeiro{

class vec2;
    
#if defined(ARIBEIRO_SSE2)

    extern const __m128 _vec3_zero_sse;
    extern const __m128 _vec3_sign_mask; // -0.f = 1 << 31

#pragma pack(push, 16)
#endif
    
/// \brief Vector 3D (vec3)
///
/// Stores three components(x,y,z) to represent a tridimensional vector. <br/>
/// It can be used as points or vectors in 3D.
/// \warning The class is not designed to represent 2D homogeneous space.
/// \author Alessandro Ribeiro
///
_SSE2_ALIGN_PRE class ARIBEIRO_API vec3{
    public:
	_SSE2_ALIGN_PRE union {
		_SSE2_ALIGN_PRE float array[3] _SSE2_ALIGN_POS; ///<The 3D low level array representation to pass the vector as pointer parameter
		_SSE2_ALIGN_PRE struct{ float x,y,z; } _SSE2_ALIGN_POS;///<Components X, Y and Z to be used by the application
		_SSE2_ALIGN_PRE struct{ float r,g,b; } _SSE2_ALIGN_POS;///<Components R, G and B to be used by the application
#if defined(ARIBEIRO_SSE2)
		_SSE2_ALIGN_PRE __m128 array_sse _SSE2_ALIGN_POS;
#endif
    }_SSE2_ALIGN_POS;
    
    
#if defined(ARIBEIRO_SSE2)
    //special SSE2 constructor
    ARIBEIRO_INLINE vec3( const __m128 &v ){
        array_sse = v;
    }
#endif
    
    /// \brief Construct a ZERO vec3 class
    ///
    /// The ZERO vec3 class have the point information in the origin (x=0,y=0,z=0)
    /// \author Alessandro Ribeiro
    ///
    ARIBEIRO_INLINE vec3() {
#if defined(ARIBEIRO_SSE2)
        array_sse = _vec3_zero_sse;
#else
        x = y = z = 0.0f;
#endif
    }
    /// \brief Constructs a tridimensional Vector
    ///
    /// Initialize the vec3 components with the same float value
    ///
    /// X = v, Y = v and Z = v
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to initialize the components
    ///
    ARIBEIRO_INLINE vec3( const float &v ){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_load_(v,v,v,0);
#else
        x = y = z = v;
#endif   
    }
    /// \brief Constructs a tridimensional Vector
    ///
    /// Initialize the vec3 components from the parameters
    ///
    /// \author Alessandro Ribeiro
    /// \param x Value to assign to the X component of the vector
    /// \param y Value to assign to the Y component of the vector
    /// \param z Value to assign to the Z component of the vector
    ///
    ARIBEIRO_INLINE vec3( const float &x, const float &y, const float &z ){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_load_(x,y,z,0);
#else
        this->x = x;
        this->y = y;
        this->z = z;
#endif
    }
    /// \brief Constructs a tridimensional Vector
    ///
    /// Initialize the vec3 components from a vec2 xy and an isolated z value
    ///
    /// this->xy = xy <br />
    /// this->z = z
    ///
    /// \author Alessandro Ribeiro
    /// \param xy Vector 2D to assign to the components x and y of the instance respectively
    /// \param z Value to assign to the component z of the instance
    ///
    ARIBEIRO_INLINE vec3( const vec2 &xy , const float &z){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_load_(xy.x,xy.y,z,0);
#else
        x = xy.x;
        y = xy.y;
        this->z = z;
#endif
    }
    /// \brief Constructs a tridimensional Vector
    ///
    /// Initialize the vec3 components from an isolated x value and a vec2 yz
    ///
    /// this->x = x <br />
    /// this->yz = yz
    ///
    /// \author Alessandro Ribeiro
    /// \param x Value to assign to the component x of the instance
    /// \param yz Vector 2D to assign to the components y and z of the instance respectively
    ///
    ARIBEIRO_INLINE vec3( const float &x, const vec2 &yz){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_load_(x,yz.x,yz.y,0);
#else
        this->x = x;
        y = yz.x;
        z = yz.y;
#endif
    }
    /// \brief Constructs a tridimensional Vector
    ///
    /// Initialize the vec3 components from other vec3 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to assign to the instance
    ///
    ARIBEIRO_INLINE vec3( const vec3 &v ){
#if defined(ARIBEIRO_SSE2)
        array_sse = v.array_sse;
#else
        *this = v;
#endif
    }
    /// \brief Constructs a tridimensional Vector from the subtraction b-a
    ///
    /// Initialize the vec3 components from two other vectors using the equation: <br />
    /// this = b - a
    ///
    /// \author Alessandro Ribeiro
    /// \param a Orign vector
    /// \param b Destiny vector
    ///
    ARIBEIRO_INLINE vec3( const vec3 &a, const vec3 &b ){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_sub_ps(a.array_sse, b.array_sse);
#else
        x = b.x - a.x;
        y = b.y - a.y;
        z = b.z - a.z;
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
    ARIBEIRO_INLINE bool operator==(const vec3&v) const {
#if defined(ARIBEIRO_SSE2)
        __m128 diff_abs = _mm_sub_ps(array_sse, v.array_sse);
        //abs
        diff_abs = _mm_andnot_ps(_vec3_sign_mask, diff_abs);
        
        //static const __m128 epsilon = _mm_set1_ps(1e-4f); // -0.f = 1 << 31
        //_mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));
        
        for(int i=0;i<3;i++){
            if (_mm_f32_(diff_abs,i) > 1e-4f)
                return false;
        }
        
        return true;
#else
        for(int i=0;i<3;i++){
            if (absv(array[i]-v.array[i]) > 1e-4f)
                return false;
        }
        return true;
        //return memcmp(array, v.array, sizeof(float) * 3) == 0;
#endif
    }
    
    ARIBEIRO_INLINE bool operator!=(const vec3&v) const{
        return !((*this) == v);
        //return memcmp(array, v.array, sizeof(float) * 3) != 0;
    }
    /// \brief Component-wise increment operator overload
    ///
    /// Increment the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to increment the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    ARIBEIRO_INLINE vec3& operator+=(const vec3& v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_add_ps(array_sse, v.array_sse);
#else
        x+=v.x;
        y+=v.y;
        z+=v.z;
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
    ARIBEIRO_INLINE vec3& operator-=(const vec3& v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_sub_ps(array_sse, v.array_sse);
#else
        x-=v.x;
        y-=v.y;
        z-=v.z;
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
    ARIBEIRO_INLINE vec3 operator-() const{
#if defined(ARIBEIRO_SSE2)
        return _mm_xor_ps(_vec3_sign_mask, array_sse);
#else
        return vec3(-x,-y,-z);
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
    ARIBEIRO_INLINE vec3& operator*=(const vec3& v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_mul_ps(array_sse, v.array_sse);
#else
        x*=v.x;
        y*=v.y;
        z*=v.z;
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
    ARIBEIRO_INLINE vec3& operator/=(const vec3& v){
#if defined(ARIBEIRO_SSE2)
        __m128 param = _mm_load_(v.x,v.y,v.z,1.0f);
        array_sse = _mm_div_ps(array_sse, param);
#else
        x/=v.x;
        y/=v.y;
        z/=v.z;
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
    ARIBEIRO_INLINE vec3& operator+=(const float &v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_add_ps(array_sse, _mm_set1_ps(v));
#else
        x+=v;
        y+=v;
        z+=v;
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
    ARIBEIRO_INLINE vec3& operator-=(const float &v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_sub_ps(array_sse, _mm_set1_ps(v));
#else
        x-=v;
        y-=v;
        z-=v;
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
    ARIBEIRO_INLINE vec3& operator*=(const float &v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_mul_ps(array_sse, _mm_set1_ps(v));
#else
        x*=v;
        y*=v;
        z*=v;
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
    ARIBEIRO_INLINE vec3& operator/=(const float &v){
#if defined(ARIBEIRO_SSE2)
        array_sse = _mm_div_ps(array_sse, _mm_set1_ps(v));
#else
        x/=v;
        y/=v;
        z/=v;
#endif
        return (*this);
    }
    /// \brief Index the components of the vec3 as a C array
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
    
INLINE_OPERATION_IMPLEMENTATION(vec3)

#if defined(ARIBEIRO_SSE2)
#pragma pack(pop)
#endif
    
}

#endif
