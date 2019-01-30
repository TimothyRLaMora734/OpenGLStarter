#ifndef vec4_h
#define vec4_h

#include <aribeiro/common.h>

namespace aRibeiro{

class vec3;

/// \brief Homogeneous 3D (vec4)
///
/// Stores three components(x,y,z,w) to represent a tridimensional vector with the homogeneous component w. <br/>
/// It can be used to represent points or vectors in 3D.
/// \author Alessandro Ribeiro
///
class ARIBEIRO_API vec4{
    public:
    union {
        float array[4];///<The 4D low level array representation to pass the vector as pointer parameter
        struct{ float x,y,z,w; };///<Components X, Y, Z and W to be used by the application
        struct{ float r,g,b,a; };///<Components R, G, B and a to be used by the application
    };
    /// \brief Construct a ZERO vec4 class
    ///
    /// The ZERO vec4 class have the point information in the origin as a vector (x=0,y=0,z=0,w=0)
    /// \author Alessandro Ribeiro
    ///
    vec4();
    /// \brief Constructs a tridimensional Vector with homogeneous component
    ///
    /// Initialize the vec3 components with the same float value
    ///
    /// X = v, Y = v, Z = v and W = v
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to initialize the components
    ///
    vec4( const float v );
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
    vec4( const float x, const float y, const float z, const float w );
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
    vec4( const vec3 &xyz , float w);
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
    vec4( float x, const vec3 &yzw);
    /// \brief Constructs a tridimensional Vector with homogeneous component
    ///
    /// Initialize the vec4 components from other vec4 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to assign to the instance
    ///
    vec4( const vec4 &v );
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
    vec4( const vec4 &a, const vec4 &b );
    /// \brief Binary comparison of vectors
    ///
    /// Compare two vectors using memcmp to see if they are the same.
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to compare against
    /// \return true if the bits of this is the same of v, otherwise false.
    ///
    bool operator==(const vec4&v) const ;
    /// \brief Component-wise increment operator overload
    ///
    /// Increment the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to increment the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    vec4& operator+=(const vec4& v);
    /// \brief Component-wise decrement operator overload
    ///
    /// Decrement the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to decrement the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec4& operator-=(const vec4& v);
    /// \brief Component-wise minus operator overload
    ///
    /// Negates the vector components with the operator minus
    ///
    /// \author Alessandro Ribeiro
    /// \return A copy of the current instance after the negation operation
    ///
    vec4 operator-();
    /// \brief Component-wise multiply operator overload
    ///
    /// Multiply the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to multiply the current vector instance
    /// \return A reference to the current instance after the multiplication
    ///
    vec4& operator*=(const vec4& v);
    /// \brief Component-wise division operator overload
    ///
    /// Divides the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to divide the current vector instance
    /// \return A reference to the current instance after the division
    ///
    vec4& operator/=(const vec4& v);
    /// \brief Single value increment operator overload
    ///
    /// Increment the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to increment all components of the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    vec4& operator+=(const float v);
    /// \brief Single value decrement operator overload
    ///
    /// Decrement the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec4& operator-=(const float v);
    /// \brief Single value multiply operator overload
    ///
    /// Decrement the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec4& operator*=(const float v);
    /// \brief Single value division operator overload
    ///
    /// Divides the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to divide all components of the current vector instance
    /// \return A reference to the current instance after the division
    ///
    vec4& operator/=(const float v);
    /// \brief Index the components of the vec4 as a C array
    ///
    /// \author Alessandro Ribeiro
    /// \param v The index of the components starting by 0
    /// \return A reference to the element at the index v
    ///
    float& operator[](const int v);
	float operator[](const int v)const;
};

}

#endif
