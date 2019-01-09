#ifndef quat4_h
#define quat4_h

#include <aribeiro/common.h>

namespace aRibeiro {

class vec3;

/// \brief Quaternion (quat)
///
/// Stores four components(x,y,z,w) to represent a quaternion. <br/>
/// The quaternion can be seen as a unit axis with an angle in radians.
/// \author Alessandro Ribeiro
///
class ARIBEIRO_API quat{
    public:
    union {
        float array[4];///<The 4D low level array representation to pass the vector as pointer parameter
        struct{ float x,y,z,w; };///<Components X, Y, Z and W to be used by the application
    };
    /// \brief Construct an identity quaternion quat class
    ///
    /// The identity quat class have the folow configuration (x=0,y=0,z=0,w=1)
    /// \author Alessandro Ribeiro
    ///
    quat();
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
    quat( const float x, const float y, const float z, const float w );
    /// \brief Constructs a quaternion
    ///
    /// Initialize the quat components from other quat instance
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to assign to the instance
    ///
    quat( const quat &v );
    /// \brief Binary comparison of vectors
    ///
    /// Compare two vectors using memcmp to see if they are the same.
    ///
    /// \author Alessandro Ribeiro
    /// \param v Vector to compare against
    /// \return true if the bits of this is the same of v, otherwise false.
    ///
    bool operator==(const quat&v) const ;
    /// \brief Index the components of the quat as a C array
    ///
    /// \author Alessandro Ribeiro
    /// \param v The index of the components starting by 0
    /// \return A reference to the element at the index v
    ///
    float& operator[](const int v);
};

}

#endif
