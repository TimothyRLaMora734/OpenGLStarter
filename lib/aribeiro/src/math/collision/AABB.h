#ifndef AABB_hpp
#define AABB_hpp

#include <aribeiro/common.h>

//I need a complete definition to instantiate a vec3 as class field
#include <aribeiro/vec3.h>
#include <aribeiro/vec2.h>

namespace aRibeiro {

//class vec2;

namespace collision {

class Sphere;
class LineSegment;
class Plane;
class Ray;
class Triangle;
class Frustum;

/// \brief Axis Aligned Bounding Box (AABB)
///
/// Stores 3D points to represent an Axis Aligned Bounding Box. <br/>
/// It can be used to make collision tests
/// \author Alessandro Ribeiro
///
_SSE2_ALIGN_PRE class ARIBEIRO_API AABB{
    public:
    vec3 min_box;///<Store the minimum values of the AABB Box
    vec3 max_box;///<Store the maximum values of the AABB Box
    //--------------------------------------------------------------------------
    /// \brief Construct a ZERO AABB class
    ///
    /// The ZERO AABB class have both points in the origin (0,0,0)
    /// \author Alessandro Ribeiro
    /// \sa AABB()
    /// \sa AABB(const vec2 &a,const vec2 &b)
    /// \sa AABB(const vec3 &a,const vec3 &b)
    ///
    AABB();
    //--------------------------------------------------------------------------
    /// \brief Constructs a new bidimensional AABB class
    ///
    /// Requires to pass 2 points to construct of the AABB class
    ///
    /// Ex:
    /// <pre>
    ///  CornerA *--* CornerB
    ///          |  |
    ///  CornerC *--* CornerD
    /// </pre>
    ///
    /// You can use any combination: {CornerA,CornerD}, {CornerC,CornerB}, etc...
    ///
    /// \author Alessandro Ribeiro
    /// \param a An AABB corner
    /// \param b Another AABB corner
    /// \warning The z value of both min_box and max_box are -1 and 1 respectively
    /// \sa AABB()
    /// \sa AABB(const vec3 &a,const vec3 &b)
    ///
    AABB(const vec2 &a,const vec2 &b);
    //--------------------------------------------------------------------------
    /// \brief Constructs a new bidimensional AABB class
    ///
    /// Requires to pass 2 points to construct of the AABB class
    ///
    /// Ex:
    /// <pre>
    ///  CornerA *--* CornerB
    ///          |  |
    ///  CornerC *--* CornerD
    /// </pre>
    ///
    /// You can use any combination: {CornerA,CornerD}, {CornerC,CornerB}, etc...
    ///
    /// \author Alessandro Ribeiro
    /// \param a An AABB corner
    /// \param b Another AABB corner
    /// \warning The z value of both min_box and max_box are -1 and 1 respectively
    /// \sa AABB()
    /// \sa AABB(const vec2 &a,const vec2 &b)
    ///
    AABB(const vec3 &a,const vec3 &b);
    //--------------------------------------------------------------------------
    // Static methods
    //--------------------------------------------------------------------------


	/// \brief OpenGL calls to draw the GL_LINES of the AABB in 3D <br/>
    /// If the application doesn't use the gl headers, this method won draw anything
    /// \author Alessandro Ribeiro
    ///
    //static void glDrawLines(const AABB &aabb);
    /// \brief OpenGL calls to draw the GL_QUADS of the AABB in 3D <br/>
    /// If the application doesn't use the gl headers, this method won draw anything
    /// \author Alessandro Ribeiro
    ///
    //static void glDrawQuads(const AABB &aabb);


    /// \brief Test if a point is inside an AABB
    /// \author Alessandro Ribeiro
    /// \param ptn 2D point to test against the AABB
    /// \param b The AABB
    /// \return true if the point is inside the AABB, otherwise false
    /// \warning The z is not used in the test
    ///
    static bool pointInsideAABB(const vec2& ptn,const AABB& b);
    /// \brief Test if a point is inside an AABB
    /// \author Alessandro Ribeiro
    /// \param ptn 3D point to test against the AABB
    /// \param b The AABB
    /// \return true if the point is inside the AABB, otherwise false
    ///
    static bool pointInsideAABB(const vec3& ptn,const AABB& b);

	/// \brief Test if there is some overlaped area between two AABBs
    /// \author Alessandro Ribeiro
    /// \param a The first AABB to test
    /// \param b The second AABB to test against
    /// \return true if there are some overlaped area between the two AABBs, otherwise false
    ///
    static bool aabbOverlapsAABB(const AABB& a,const AABB& b);

    /// \brief Create an AABB that is the union result between the two AABBs
    /// \author Alessandro Ribeiro
    /// \param a The first AABB to consider in the union
    /// \param b The second AABB to consider in the union
    /// \return AABB of the union of the parameters
    ///
    static AABB joinAABB(const AABB& a,const AABB& b);



	static AABB fromTriangle(const vec3& a, const vec3& b, const vec3& c);
	static AABB fromSphere(const vec3& pos, float radius);
	static AABB fromLineSegment(const vec3& a, const vec3& b);
	static AABB fromLineSegment(const LineSegment& ls);

	// Intersect ray R(t) = p + t*d against AABB a. When intersecting,
	// return intersection distance tmin and point q of intersection
	static bool raycastAABB(const Ray &r, const AABB& a, float *outTmin, vec3 *outNormal);

	// Test if segment specified by points p0 and p1 intersects AABB b
	static bool segmentIntersectsAABB(const vec3& p0, const vec3& p1, const AABB &b);
	static bool segmentIntersectsAABB(const LineSegment& ls, const AABB &b);

	// Given point p, return the point q on or in AABB b that is closest to p
	static vec3 closestPointToAABB(const vec3 &p, const AABB &b);

	static bool sphereOverlapsAABB(const Sphere &sphere, const AABB& aabb, vec3 *penetration);
	static bool planeIntersectsAABB(const Plane &plane, const AABB &b);
	static bool triangleIntersectsAABB(const vec3 &v0, const vec3 &v1, const vec3 &v2, const AABB &box);
	static bool triangleIntersectsAABB(const Triangle &t, const AABB &box);

	//
	// Cloned methods from other collision classes
	//
	static bool frustumOverlapsAABB(const Frustum &f, const AABB &aabb);

	SSE2_CLASS_NEW_OPERATOR
} _SSE2_ALIGN_POS;

}

}

#endif
