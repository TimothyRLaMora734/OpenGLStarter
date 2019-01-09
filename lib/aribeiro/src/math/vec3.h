#ifndef vec3_h
#define vec3_h

#include <aribeiro/common.h>

namespace aRibeiro{

class vec2;

/// \brief Vector 3D (vec3)
///
/// Stores three components(x,y,z) to represent a tridimensional vector. <br/>
/// It can be used as points or vectors in 3D.
/// \warning The class is not designed to represent 2D homogeneous space.
/// \author Alessandro Ribeiro
///
class ARIBEIRO_API vec3{
    public:
    union {
        float array[3]; ///<The 3D low level array representation to pass the vector as pointer parameter
        struct{ float x,y,z; };///<Components X, Y and Z to be used by the application
        struct{ float r,g,b; };///<Components R, G and B to be used by the application
    };
    /// \brief Construct a ZERO vec3 class
    ///
    /// The ZERO vec3 class have the point information in the origin (x=0,y=0,z=0)
    /// \author Alessandro Ribeiro
    ///
    vec3();
    /// \brief Constructs a tridimensional Vector
    ///
    /// Initialize the vec3 components with the same float value
    ///
    /// X = v, Y = v and Z = v
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to initialize the components
    ///
    vec3( const float v );
    /// \brief Constructs a tridimensional Vector
    ///
    /// Initialize the vec3 components from the parameters
    ///
    /// \author Alessandro Ribeiro
    /// \param x Value to assign to the X component of the vector
    /// \param y Value to assign to the Y component of the vector
    /// \param z Value to assign to the Z component of the vector
    ///
    vec3( const float x, const float y, const float z );
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
    vec3( const vec2 &xy , float z);
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
    vec3( float x, const vec2 &yz);
    /// \brief Constructs a tridimensional Vector
    ///
    /// Initialize the vec3 components from other vec3 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to assign to the instance
    ///
    vec3( const vec3 &v );
    /// \brief Constructs a tridimensional Vector from the subtraction b-a
    ///
    /// Initialize the vec3 components from two other vectors using the equation: <br />
    /// this = b - a
    ///
    /// \author Alessandro Ribeiro
    /// \param a Orign vector
    /// \param b Destiny vector
    ///
    vec3( const vec3 &a, const vec3 &b );
    /// \brief Binary comparison of vectors
    ///
    /// Compare two vectors using memcmp to see if they are the same.
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to compare against
    /// \return true if the bits of this is the same of v, otherwise false.
    ///
    bool operator==(const vec3&v) const ;
    /// \brief Component-wise increment operator overload
    ///
    /// Increment the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to increment the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    vec3& operator+=(const vec3& v);
    /// \brief Component-wise decrement operator overload
    ///
    /// Decrement the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to decrement the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec3& operator-=(const vec3& v);
    /// \brief Component-wise minus operator overload
    ///
    /// Negates the vector components with the operator minus
    ///
    /// \author Alessandro Ribeiro
    /// \return A copy of the current instance after the negation operation
    ///
    vec3 operator-();
    /// \brief Component-wise multiply operator overload
    ///
    /// Multiply the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to multiply the current vector instance
    /// \return A reference to the current instance after the multiplication
    ///
    vec3& operator*=(const vec3& v);
    /// \brief Component-wise division operator overload
    ///
    /// Divides the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to divide the current vector instance
    /// \return A reference to the current instance after the division
    ///
    vec3& operator/=(const vec3& v);
    /// \brief Single value increment operator overload
    ///
    /// Increment the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to increment all components of the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    vec3& operator+=(const float v);
    /// \brief Single value decrement operator overload
    ///
    /// Decrement the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec3& operator-=(const float v);
    /// \brief Single value multiply operator overload
    ///
    /// Decrement the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec3& operator*=(const float v);
    /// \brief Single value division operator overload
    ///
    /// Divides the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to divide all components of the current vector instance
    /// \return A reference to the current instance after the division
    ///
    vec3& operator/=(const float v);
    /// \brief Index the components of the vec3 as a C array
    ///
    /// \author Alessandro Ribeiro
    /// \param v The index of the components starting by 0
    /// \return A reference to the element at the index v
    ///
    float& operator[](const int v);
};

}

#endif
