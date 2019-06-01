#ifndef vec2_h
#define vec2_h

#include <aribeiro/common.h>

namespace aRibeiro{

/// \brief Vector 2D (vec2)
///
/// Stores two components(x,y) to represent a bidimensional vector. <br/>
/// It can be used as points or vectors in 2D.
/// \author Alessandro Ribeiro
///
class ARIBEIRO_API vec2{
    public:
    union {
      float array[2];///<The 2D low level array representation to pass the vector as pointer parameter
      struct{ float x,y; };///<Components X and Y to be used by the application
    };
    /// \brief Construct a ZERO vec2 class
    ///
    /// The ZERO vec2 class have the point information in the origin (x=0,y=0)
    /// \author Alessandro Ribeiro
    ///
    vec2();
    /// \brief Constructs a bidimensional Vector
    ///
    /// Initialize the vec2 components with the same float value
    ///
    /// X = v and Y = v
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to initialize the components
    ///
    vec2( const float v );
    /// \brief Constructs a bidimensional Vector
    ///
    /// Initialize the vec2 components from the parameters
    ///
    /// \author Alessandro Ribeiro
    /// \param x Value to assign to the X component of the vector
    /// \param y Value to assign to the Y component of the vector
    ///
    vec2( const float x, const float y );
    /// \brief Constructs a bidimensional Vector
    ///
    /// Initialize the vec2 components from other vec2 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to assign to the instance
    ///
    vec2( const vec2 &v );
    /// \brief Constructs a bidimensional Vector from the subtraction b-a
    ///
    /// Initialize the vec2 components from two other vectors using the equation: <br />
    /// this = b - a
    ///
    /// \author Alessandro Ribeiro
    /// \param a Orign vector
    /// \param b Destiny vector
    ///
    vec2( const vec2 &a, const vec2 &b );

    /// \brief Binary comparison of vectors
    ///
    /// Compare two vectors using memcmp to see if they are the same.
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to compare against
    /// \return true if the bits of this is the same of v, otherwise false.
    ///
    bool operator==(const vec2&v) const ;
	bool operator!=(const vec2&v) const;

    /// \brief Component-wise increment operator overload
    ///
    /// Increment the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to increment the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    vec2& operator+=(const vec2& v);

    /// \brief Component-wise decrement operator overload
    ///
    /// Decrement the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to decrement the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec2& operator-=(const vec2& v);

    /// \brief Component-wise minus operator overload
    ///
    /// Negates the vector components with the operator minus
    ///
    /// \author Alessandro Ribeiro
    /// \return A copy of the current instance after the negation operation
    ///
    vec2 operator-()const;

    /// \brief Component-wise multiply operator overload
    ///
    /// Multiply the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to multiply the current vector instance
    /// \return A reference to the current instance after the multiplication
    ///
    vec2& operator*=(const vec2& v);

    /// \brief Component-wise division operator overload
    ///
    /// Divides the vector by the components of another vector
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to divide the current vector instance
    /// \return A reference to the current instance after the division
    ///
    vec2& operator/=(const vec2& v);

    /// \brief Single value increment operator overload
    ///
    /// Increment the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to increment all components of the current vector instance
    /// \return A reference to the current instance after the increment
    ///
    vec2& operator+=(const float v);

    /// \brief Single value decrement operator overload
    ///
    /// Decrement the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec2& operator-=(const float v);

    /// \brief Single value multiply operator overload
    ///
    /// Decrement the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current vector instance
    /// \return A reference to the current instance after the decrement
    ///
    vec2& operator*=(const float v);

    /// \brief Single value division operator overload
    ///
    /// Divides the vector components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to divide all components of the current vector instance
    /// \return A reference to the current instance after the division
    ///
    vec2& operator/=(const float v);

    /// \brief Index the components of the vec2 as a C array
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
