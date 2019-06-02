#ifndef geometricOperations_h
#define geometricOperations_h

#include <aribeiro/common.h>

namespace aRibeiro {

class vec2;
class vec3;
class vec4;
class mat4;
class tensor4;
class quat;


/// \brief Defines the general aritmetic operations over the vec2, vec3 and vec4 <br />
///        Using the C++ operator overload /, *, + and -
/// \author Alessandro Ribeiro
///
#define OPERATION_DEFINITION(TTYPE)\
ARIBEIRO_API TTYPE operator/( const TTYPE& vecA, const TTYPE& vecB ); \
ARIBEIRO_API TTYPE operator/( const TTYPE& vec , const float value ); \
ARIBEIRO_API TTYPE operator/( const float value, const TTYPE& vec  ); \
ARIBEIRO_API TTYPE operator*( const TTYPE& vecA, const TTYPE& vecB ); \
ARIBEIRO_API TTYPE operator*( const TTYPE& vec , const float value ); \
ARIBEIRO_API TTYPE operator*( const float value, const TTYPE& vec  ); \
ARIBEIRO_API TTYPE operator+( const TTYPE& vecA, const TTYPE& vecB ); \
ARIBEIRO_API TTYPE operator+( const TTYPE& vec , const float value ); \
ARIBEIRO_API TTYPE operator+( const float value, const TTYPE& vec  ); \
ARIBEIRO_API TTYPE operator-( const TTYPE& vecA, const TTYPE& vecB ); \
ARIBEIRO_API TTYPE operator-( const TTYPE& vec , const float value ); \
ARIBEIRO_API TTYPE operator-( const float value, const TTYPE& vec  );

OPERATION_DEFINITION(vec2)
OPERATION_DEFINITION(vec3)
OPERATION_DEFINITION(vec4)
OPERATION_DEFINITION(tensor4)

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
ARIBEIRO_API vec4 operator*(const mat4 &mat, const vec4 &vec);

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
ARIBEIRO_API vec4 operator*(const vec4 &vec, const mat4 &mat);

/// \brief Rotate quaternion a according quaternion b.
///
/// The result is not normalized
///
/// \author Alessandro Ribeiro
/// \param a source quaternion
/// \param b source quaternion
/// \return a rotated according b
///
ARIBEIRO_API quat operator^(const quat &a, const quat &b);

/// \brief Rotate quaternion a according quaternion b.
///
/// The result will be normalized
///
/// \author Alessandro Ribeiro
/// \param a source quaternion
/// \param b source quaternion
/// \return a rotated according b
///
ARIBEIRO_API quat operator*(const quat &a, const quat &b);

/// \brief Rotate vector according a quaternion
///
/// \author Alessandro Ribeiro
/// \param r source quaternion
/// \param vec source vector
/// \return vec rotated according r
///
ARIBEIRO_API vec3 operator*(const quat &r, const vec3 &vec);

/// \brief Rotate vector according a quaternion
///
/// \author Alessandro Ribeiro
/// \param r source quaternion
/// \param vec source vector
/// \return vec rotated according r
///
ARIBEIRO_API vec4 operator*(const quat &r, const vec4 &vec);


/// \brief Compute the spherical linear interpolation between 2 vec3
///
/// \author Alessandro Ribeiro
/// \param a The source vector
/// \param a The target vector
/// \return The quaternion
///
ARIBEIRO_API vec3 slerp(const vec3& a, const vec3& b, const float lerp);

/// \brief Compute the angle between two vectors
///
/// \author Alessandro Ribeiro
/// \param a The source vector
/// \param a The target vector
/// \return The quaternion
///
ARIBEIRO_API float angleBetween(const vec3& a, const vec3& b);


/// \brief Converts a vec4 to a vec3 by discarding the w component of v
///
/// Considering that the parameter is a point (i.e. with w=0) or a vector (i.e. with w=1), it can be converted to a vec3 without the w component.
///
/// \author Alessandro Ribeiro
/// \param v A vector with 4 components
/// \return A vec3 containing xyz from the parameter
///
ARIBEIRO_API vec3 toVec3(const vec4 &v);
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
ARIBEIRO_API vec3 toVec3_PerspDiv(const vec4 &v);
/// \brief Converts a vec3 to a vec4 with w = 0
///
/// Considering that the parameter is not a point, it can be converted to a vector with homogeneous componente as zero.
///
/// \author Alessandro Ribeiro
/// \param v A vector with 3 components
/// \return A vec4 containing vec4(x,y,z,0)
///
ARIBEIRO_API vec4 toVec4(const vec3 &v);
/// \brief Converts a vec3 to a vec4 with w = 1
///
/// Considering that the parameter is a point, it can be converted to a vector with homogeneous componente as one.
///
/// \author Alessandro Ribeiro
/// \param v A vector with 3 components
/// \return A vec4 containing vec4(x,y,z,1)
///
ARIBEIRO_API vec4 toPtn4(const vec3 &v);

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
ARIBEIRO_API vec2 polarToVec2(float pAngle, float pRadius);



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
ARIBEIRO_API vec3 cross( const vec3& a, const vec3& b );
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
ARIBEIRO_API float dot( const vec2& a, const vec2& b );
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
ARIBEIRO_API float dot( const vec3& a, const vec3& b );
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
ARIBEIRO_API float dot( const vec4& a, const vec4& b );
//------------------------------------------------------------------------------
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
ARIBEIRO_API vec2 normalize(const vec2& vec );
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
ARIBEIRO_API vec3 normalize(const vec3& vec );
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
ARIBEIRO_API vec4 normalize(const vec4& vec );
//------------------------------------------------------------------------------
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
ARIBEIRO_API float clamp(const float value,const float min,const float max );
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
ARIBEIRO_API vec2 clamp(const vec2& value,const vec2& min,const vec2& max );
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
ARIBEIRO_API vec3 clamp( const vec3& value, const vec3& min,const vec3& max );
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
ARIBEIRO_API vec4 clamp( const vec4& value, const vec4& min, const vec4& max );

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
ARIBEIRO_API vec2 reflect( const vec2& a, const vec2& N );
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
ARIBEIRO_API vec3 reflect( const vec3& a, const vec3& N );
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
ARIBEIRO_API vec4 reflect( const vec4& a, const vec4& N );

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
ARIBEIRO_API bool refract(const vec3 &rayDir,const vec3 &normal, float ni, float nr, vec3 *vOut );

//------------------------------------------------------------------------------

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
ARIBEIRO_API float sqrLength(const vec2 &a);
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
ARIBEIRO_API float sqrLength(const vec3 &a);
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
ARIBEIRO_API float sqrLength(const vec4 &a);
//------------------------------------------------------------------------------
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
ARIBEIRO_API float length(const vec2 &a);
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
ARIBEIRO_API float length(const vec3 &a);
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
ARIBEIRO_API float length(const vec4 &a);
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
ARIBEIRO_API float sqrDistance(const float a,const float b);
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
ARIBEIRO_API float sqrDistance(const vec2 &a, const vec2 &b);
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
ARIBEIRO_API float sqrDistance(const vec4 &a, const vec4 &b);
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
ARIBEIRO_API float sqrDistance(const vec3 &a, const vec3 &b);
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
ARIBEIRO_API float distance(const float a,const float b);
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
ARIBEIRO_API float distance(const vec2 &a, const vec2 &b);
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
ARIBEIRO_API float distance(const vec4 &a, const vec4 &b);
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
ARIBEIRO_API float distance(const vec3 &a, const vec3 &b);

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
ARIBEIRO_API vec2 parallelComponent(const vec2 &a, const vec2 &unitV);
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
ARIBEIRO_API vec3 parallelComponent(const vec3 &a, const vec3 &unitV);
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
ARIBEIRO_API vec4 parallelComponent(const vec4 &a, const vec4 &unitV);
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
ARIBEIRO_API vec2 perpendicularComponent(const vec2 &a, const vec2 &unitV);
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
ARIBEIRO_API vec3 perpendicularComponent(const vec3 &a, const vec3 &unitV);
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
ARIBEIRO_API vec4 perpendicularComponent(const vec4 &a, const vec4 &unitV);
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
ARIBEIRO_API void vecDecomp(const vec2 &a, const vec2 &unitV,
                             vec2 *perpendicular, vec2 *parallel);

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
ARIBEIRO_API void vecDecomp(const vec3 &a, const vec3 &unitV,
                             vec3 *perpendicular, vec3 *parallel);

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
ARIBEIRO_API void vecDecomp(const vec4 &a, const vec4 &unitV,
                             vec4 *perpendicular, vec4 *parallel);
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
ARIBEIRO_API vec2 quadraticClamp(const vec2 &point, const vec2 &center,const float maxRadius);
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
ARIBEIRO_API vec3 quadraticClamp(const vec3 &point, const vec3 &center,const float maxRadius);
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
ARIBEIRO_API vec4 quadraticClamp(const vec4 &point, const vec4 &center,const float maxRadius);
//------------------------------------------------------------------------------
/// \brief Return the greater value from the parameter
///
/// \author Alessandro Ribeiro
/// \param a Set of values to test
/// \return The greater value from the parameter
///
ARIBEIRO_API float maximum(const vec2 &a);
/// \brief Return the greater value from the parameter
///
/// \author Alessandro Ribeiro
/// \param a Set of values to test
/// \return The greater value from the parameter
///
ARIBEIRO_API float maximum(const vec3 &a);
/// \brief Return the greater value from the parameter
///
/// \author Alessandro Ribeiro
/// \param a Set of values to test
/// \return The greater value from the parameter
///
ARIBEIRO_API float maximum(const vec4 &a);
/// \brief Return the maximum between the two parameters
///
/// \author Alessandro Ribeiro
/// \param a A value to test
/// \param b A value to test
/// \return The maximum value of the parameters
///
ARIBEIRO_API float maximum(const float a,const float b);
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
ARIBEIRO_API vec2 maximum(const vec2 &a, const vec2 &b);
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
ARIBEIRO_API vec3 maximum(const vec3 &a, const vec3 &b);
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
ARIBEIRO_API vec4 maximum(const vec4 &a, const vec4 &b);
/// \brief Return the smaller value from the parameter
///
/// \author Alessandro Ribeiro
/// \param a Set of values to test
/// \return The smaller value from the parameter
///
ARIBEIRO_API float minimum(const vec2 &a);
/// \brief Return the smaller value from the parameter
///
/// \author Alessandro Ribeiro
/// \param a Set of values to test
/// \return The smaller value from the parameter
///
ARIBEIRO_API float minimum(const vec3 &a);
/// \brief Return the smaller value from the parameter
///
/// \author Alessandro Ribeiro
/// \param a Set of values to test
/// \return The smaller value from the parameter
///
ARIBEIRO_API float minimum(const vec4 &a);
/// \brief Return the minimum between the two parameters
///
/// \author Alessandro Ribeiro
/// \param a A value to test
/// \param b A value to test
/// \return The minimum value of the parameters
///
ARIBEIRO_API float minimum(const float a, const float b);
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
ARIBEIRO_API vec2 minimum(const vec2 &a, const vec2 &b);
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
ARIBEIRO_API vec3 minimum(const vec3 &a, const vec3 &b);
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
ARIBEIRO_API vec4 minimum(const vec4 &a, const vec4 &b);
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
ARIBEIRO_API float sign(const float a);
//------------------------------------------------------------------------------
/// \brief Compute the absolute value of a number
///
/// \author Alessandro Ribeiro
/// \param a A vector
/// \return |a|
///
ARIBEIRO_API float absv(float a);
//------------------------------------------------------------------------------
/// \brief Compute the absolute value of a vector
///
/// \author Alessandro Ribeiro
/// \param a A vector
/// \return |a|
///
ARIBEIRO_API vec2 absv(const vec2 &a);
/// \brief Compute the absolute value of a vector
///
/// \author Alessandro Ribeiro
/// \param a A vector
/// \return |a|
///
ARIBEIRO_API vec3 absv(const vec3 &a);
/// \brief Compute the absolute value of a vector
///
/// \author Alessandro Ribeiro
/// \param a A vector
/// \return |a|
///
ARIBEIRO_API vec4 absv(const vec4 &a);
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
ARIBEIRO_API float lerp(const float a,const  float b,const float factor);
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
ARIBEIRO_API vec2 lerp(const vec2 &a,const  vec2 &b,const float factor);
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
ARIBEIRO_API vec3 lerp(const vec3 &a,const  vec3 &b,const float factor);
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
ARIBEIRO_API vec4 lerp(const vec4 &a,const  vec4 &b,const float factor);
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
ARIBEIRO_API tensor4 lerp(const tensor4 &a,const  tensor4 &b,const float factor);

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
ARIBEIRO_API vec3 barylerp(float u, float v, const vec3 &v0, const vec3 &v1, const vec3 &v2);


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
ARIBEIRO_API vec3 blerp(vec3 A, vec3 B, vec3 C, vec3 D,
           float dx, float dy);

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
ARIBEIRO_API vec3 splineCatmullRom(vec3 P0,vec3 P1, vec3 P2, vec3 P3,float t);
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
ARIBEIRO_API vec2 splineCatmullRom(vec2 P0,vec2 P1, vec2 P2, vec2 P3,float t);

/// \brief Extracts the rotation component inside a mat4
///
/// The rotation component is a 3x3 matrix.
///
/// \author Alessandro Ribeiro
/// \param m Origin mat4
/// \return Anothet mat4 with just the rotation
///
ARIBEIRO_API mat4 extractRotation(const mat4& m);

/// \brief Extracts the X axis from a matrix
///
/// \author Alessandro Ribeiro
/// \param m Origin mat4
/// \return A vec3 with the X axis
///
ARIBEIRO_API vec3 extractXaxis(const mat4& m);
/// \brief Extracts the Y axis from a matrix
///
/// \author Alessandro Ribeiro
/// \param m Origin mat4
/// \return A vec3 with the Y axis
///
ARIBEIRO_API vec3 extractYaxis(const mat4& m);
/// \brief Extracts the Z axis from a matrix
///
/// \author Alessandro Ribeiro
/// \param m Origin mat4
/// \return A vec3 with the Z axis
///
ARIBEIRO_API vec3 extractZaxis(const mat4& m);
/// \brief Extracts the Translation part of the matrix
///
/// \author Alessandro Ribeiro
/// \param m Origin mat4
/// \return A vec3 with the translation vector
///
ARIBEIRO_API vec3 extractTranslation(const mat4& m);
/// \brief Computes the transpose matrix
///
/// \author Alessandro Ribeiro
/// \param m Origin mat4
/// \return The transposed matrix
///
ARIBEIRO_API mat4 transpose(const mat4& m);
/// \brief Computes the inverse of a matrix
///
/// \author Alessandro Ribeiro
/// \param m Origin mat4
/// \return The inverse matrix
///
ARIBEIRO_API mat4 inv(const mat4& m);

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
ARIBEIRO_API mat4 translate(const float _x_,const float _y_,const float _z_);
//------------------------------------------------------------------------------
/// \brief Creates a translation 4x4 matrix
///
/// \author Alessandro Ribeiro
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 translate(const vec3 &_v_);
//------------------------------------------------------------------------------
/// \brief Creates a translation 4x4 matrix
///
/// \author Alessandro Ribeiro
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 translate(const vec4 &_v_);
//------------------------------------------------------------------------------
/// \brief Creates a scale 4x4 matrix
///
/// \author Alessandro Ribeiro
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 scale(const float _x_,const float _y_,const float _z_);
//------------------------------------------------------------------------------
/// \brief Creates a scale 4x4 matrix
///
/// \author Alessandro Ribeiro
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 scale(const vec3 &_v_);
//------------------------------------------------------------------------------
/// \brief Creates a scale 4x4 matrix
///
/// \author Alessandro Ribeiro
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 scale(const vec4 &_v_);
//------------------------------------------------------------------------------
/// \brief Creates a rotation 4x4 matrix over the X axis
///
/// \author Alessandro Ribeiro
/// \param _phi_ Angle in radians
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 xRotate(const float _phi_);
//------------------------------------------------------------------------------
/// \brief Creates a rotation 4x4 matrix over the Y axis
///
/// \author Alessandro Ribeiro
/// \param _theta_ Angle in radians
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 yRotate(const float _theta_);
//------------------------------------------------------------------------------
/// \brief Creates a rotation 4x4 matrix over the Z axis
///
/// \author Alessandro Ribeiro
/// \param _psi_ Angle in radians
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 zRotate(const float _psi_);
//------------------------------------------------------------------------------
/// \brief Creates a rotation 4x4 matrix over the Euler angles
///
/// \author Alessandro Ribeiro
/// \param Angle in radians
/// \param Angle in radians
/// \param Angle in radians
/// \return A 4x4 matrix
///
ARIBEIRO_API mat4 eulerRotate(float roll, float pitch, float yaw);

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
ARIBEIRO_API void extractEuler(mat4 m, float *roll, float *pitch, float *yaw);

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
ARIBEIRO_API mat4 rotate(const float _ang_,float x,float y,float z);
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
ARIBEIRO_API mat4 rotate(const float _ang_,const vec3 &axis);
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
ARIBEIRO_API mat4 projection_perspective(const float FovY,const float aspectX, const float near_, const float far_);
ARIBEIRO_API mat4 projection_perspectiveLH(const float FovY,const float aspectX, const float near_, const float far_);

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
ARIBEIRO_API mat4 projection_perspective(const float focalLength, const float w, const float h, const float near_, const float far_);
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
ARIBEIRO_API mat4 projection_frustum(const float Left,const float Right,const float Bottom,const float Top,const float Near,const float Far);
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
ARIBEIRO_API mat4 projection_ortho(const float Left,const float Right,const float Bottom,const float Top,const float Near,const float Far);
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
ARIBEIRO_API mat4 lookAt(const vec3 &front, const vec3 &up, const vec3 &position);
ARIBEIRO_API mat4 modelLookAt(const vec3 &front, const vec3 &up, const vec3 &position);
ARIBEIRO_API quat quatLookAtRotation(const vec3 &front, const vec3 &up);
ARIBEIRO_API quat quatLookAtRotationLH(const vec3 &front, const vec3 &up);
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
ARIBEIRO_API bool unproject(vec3 pointInWindow,
               const mat4 &modelViewMatrix,
               const mat4 &projectionMatrix,
               int viewportX, int viewportY,int viewportW, int viewportH,
               vec3 *worldPtn);
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
ARIBEIRO_API bool project(vec3 worldPtn,
             const mat4 &modelViewMatrix,
             const mat4 &projectionMatrix,
             int viewportX, int viewportY,int viewportW, int viewportH,
             vec3 *pointInWindow);
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
    ARIBEIRO_API quat toQuat(const vec3& v);
    //------------------------------------------------------------------------------
    /// \brief Make que quaterniom from parameter to become a unity quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param q The quaternion to normalize
    /// \return The quaternion
    ///
    ARIBEIRO_API quat normalize(const quat& q);
    //------------------------------------------------------------------------------
    /// \brief Compute the squared length of the quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param q The quaternion
    /// \return The squared length
    ///
    ARIBEIRO_API float sqrLength(const quat& q);
    //------------------------------------------------------------------------------
    /// \brief Compute the length of the quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param q The quaternion
    /// \return The length
    ///
    ARIBEIRO_API float length(const quat& q);
    //------------------------------------------------------------------------------
    /// \brief Compute the spherical linear interpolation between 2 quaternions.
    ///
    /// \author Alessandro Ribeiro
    /// \param q The quaternion
    /// \return The quaternion
    ///
    ARIBEIRO_API quat slerp(const quat& a,const quat& b,const float lerp);
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
    ARIBEIRO_API quat quatFromAxisAngle(const vec3& axis,const float angle_rad);
    //------------------------------------------------------------------------------
    /// \brief Constructs a quaternion from euler angles in radians.
    ///
    /// \author Alessandro Ribeiro
    /// \param pitch radians
    /// \param yaw radians
    /// \param roll radians
    /// \return The quat
    ///
    ARIBEIRO_API quat quatFromEuler(float roll, float pitch, float yaw);
    //------------------------------------------------------------------------------
    /// \brief Constructs a conjugate quaternion
    ///
    /// The conjugate is (-x,-y,-z,w).
    ///
    /// \author Alessandro Ribeiro
    /// \param a Input quaternion
    /// \return The quat
    ///
    ARIBEIRO_API quat conjugate(const quat& a);
    //------------------------------------------------------------------------------
    /// \brief Extracts a quaternion from any matrix that have rotation information
    ///
    /// \author Alessandro Ribeiro
    /// \param m The transformation 4x4 matrix
    /// \return The quat
    ///
    ARIBEIRO_API quat extractQuat(const mat4& m);
    //------------------------------------------------------------------------------
    /// \brief Construct a 4x4 transformation matrix from a quaternion
    ///
    /// \author Alessandro Ribeiro
    /// \param q The rotation quaternion
    /// \return The mat4
    ///
    ARIBEIRO_API mat4 toMat4(const quat& q);
    //------------------------------------------------------------------------------
    /// \brief Convert the quaternion to an axis angle representation. Notice: Not tested
    ///
    /// \author Alessandro Ribeiro
    /// \param q The rotation quaternion
    /// \param axis Output - The axis
    /// \param angle Output - The angle arount the axis in radians
    ///
    ARIBEIRO_API void extractAxisAngle(const quat& q, vec3 *axis, float *angle);
	//------------------------------------------------------------------------------

	/// \brief Convert the quaternion to Euler representation. Notice: Not found an algorithm that works...
    ///
    /// \author Alessandro Ribeiro
    /// \param q The rotation quaternion
    /// \param roll radians
	/// \param pitch radians
	/// \param yaw radians
    ///
    ARIBEIRO_API void extractEuler(quat q, float *roll,float *pitch,float *yaw );

	//------------------------------------------------------------------------------
    /// \brief Computes the inverse of a quaternion. Notice
    ///
    /// \author Alessandro Ribeiro
    /// \param q The rotation quaternion
    /// \return The inverse of the quaternion
    ///
    ARIBEIRO_API quat inv( const quat &q );

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
	ARIBEIRO_API float dot(const quat& a, const quat& b);

	/// \brief Computes the angle between two quaternions
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa angleBetween(const quat& a, const quat& b)
	/// \param a The first quat
	/// \param b The second quat
	/// \return The dot product between the two quaternions
	///
	ARIBEIRO_API float angleBetween(const quat& a, const quat& b);





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
	ARIBEIRO_API float move(float current, float target, float maxDistanceVariation);

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa move(float current, float target, float maxDistanceVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
	ARIBEIRO_API vec2 move(const vec2 &current, const vec2 &target, float maxDistanceVariation);

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa move(float current, float target, float maxDistanceVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
	ARIBEIRO_API vec3 move(const vec3 &current, const vec3 &target, float maxDistanceVariation);

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa move(float current, float target, float maxDistanceVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
	ARIBEIRO_API vec4 move(const vec4 &current, const vec4 &target, float maxDistanceVariation);

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa moveSlerp(const vec3 &current, const vec3 &target, float maxAngleVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
	ARIBEIRO_API vec3 moveSlerp(const vec3 &current, const vec3 &target, float maxAngleVariation);

	/// \brief Move from current to target, considering the max variation
	///
	///
	/// \author Alessandro Ribeiro
	/// \sa moveSlerp(const quat &current, const quat &target, float maxAngleVariation)
	/// \param current The current state
	/// \param target The target state
	/// \return the lerp from current to target according max variation
	///
	ARIBEIRO_API quat moveSlerp(const quat &current, const quat &target, float maxAngleVariation);

}


#endif
