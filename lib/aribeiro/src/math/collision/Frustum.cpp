#include "Frustum.h"

#include <aribeiro/all_math.h>

namespace aRibeiro {

	void Frustum::computePlanes(const mat4& matrix) {
		mat4 clipMatrix = transpose(matrix);
		/*
					0 _11,1 _21,2 _31,3 _41,
					4 _12,5 _22,6 _32,7 _42,
					8 _13,9 _23,10_33,11_43,
					12_14,13_24,14_34,15_44;
		*/
		vec4 aux = clipMatrix[3] - clipMatrix[0];
		right.normal = toVec3(aux);
		right.distance = -aux.w;
		right.normalize();

		aux = clipMatrix[3] + clipMatrix[0];
		left.normal = toVec3(aux);
		left.distance = -aux.w;
		left.normalize();

		aux = clipMatrix[3] + clipMatrix[1];
		bottom.normal = toVec3(aux);
		bottom.distance = -aux.w;
		bottom.normalize();

		aux = clipMatrix[3] - clipMatrix[1];
		top.normal = toVec3(aux);
		top.distance = -aux.w;
		top.normalize();

		aux = clipMatrix[3] - clipMatrix[2];
		front.normal = toVec3(aux);
		front.distance = -aux.w;
		front.normalize();

		aux = clipMatrix[3] + clipMatrix[2];
		back.normal = toVec3(aux);
		back.distance = -aux.w;
		back.normalize();
        
        //
        // Compute vertices
        //
        
        Plane::intersectPlanes(right, top, front, &vertices[0]);
        Plane::intersectPlanes(right, bottom, front, &vertices[1]);
        Plane::intersectPlanes(left, bottom, front, &vertices[2]);
        Plane::intersectPlanes(left, top, front, &vertices[3]);
        
        Plane::intersectPlanes(right, top, back, &vertices[4]);
        Plane::intersectPlanes(right, bottom, back, &vertices[5]);
        Plane::intersectPlanes(left, bottom, back, &vertices[6]);
        Plane::intersectPlanes(left, top, back, &vertices[7]);
        
	}

    Plane& Frustum::operator[](int idx){
        return ((&right)[idx]);
    }
    
    const Plane& Frustum::operator[](int idx)const {
        return ((&right)[idx]);
    }
    
	Frustum::Frustum(const mat4& projection) {
		computePlanes(projection);
	}

	Frustum::Frustum(const mat4& projection, const mat4& camera) {
		computePlanes(projection * camera);
	}


	bool Frustum::pointInsideFrustum(const vec3 &p, const Frustum &frustum) {
		for (int i = 0; i < 6; i++) {
			if (Plane::pointDistanceToPlane(p, frustum[i]) < 0)
				return false;
		}
		return true;
	}

	bool Frustum::sphereOverlapsFrustum(const Sphere &s, const Frustum &frustum) {
		for (int i = 0; i < 6; i++) {
			if (Plane::pointDistanceToPlane(s.center, frustum[i]) < -s.radius)
				return false;
		}
		return true;
	}
    
    static inline void projectOnAxis(const vec3 *points, int count, const vec3 &axis, float *outMin, float *outMax)
    {
        float min = FLT_MAX;// double.PositiveInfinity;
        float max = -FLT_MAX;// double.NegativeInfinity;
        for (int i = 0; i < count; i++)
        {
            const vec3 &p = points[i];
            float val = dot(axis, p);
            if (val < min)
                min = val;
            if (val > max)
                max = val;
        }
        *outMin = min;
        *outMax = max;
    }

	bool Frustum::aabbOverlapsFrustum(const AABB &aabb, const Frustum &frustum) {
        
        
        // Test the box normals (x-, y- and z-axes)
        vec3 boxNormals[] = {
            vec3(1,0,0),
            vec3(0,1,0),
            vec3(0,0,1)
        };
        
        float frustumMin,frustumMax;
        for (int i = 0; i < 3; i++)
        {
            projectOnAxis(frustum.vertices, 8, boxNormals[i], &frustumMin, &frustumMax);
            if (frustumMax < aabb.min_box[i] - EPSILON || frustumMin > aabb.max_box[i] + EPSILON)
                return false; // No intersection possible.
        }
        
        
        vec3 box_Vertices[] = {
            vec3(aabb.min_box.x,aabb.min_box.y,aabb.min_box.z),// 000
            vec3(aabb.min_box.x,aabb.min_box.y,aabb.max_box.z),// 001
            vec3(aabb.min_box.x,aabb.max_box.y,aabb.min_box.z),// 010
            vec3(aabb.min_box.x,aabb.max_box.y,aabb.max_box.z),// 011
            vec3(aabb.max_box.x,aabb.min_box.y,aabb.min_box.z),// 100
            vec3(aabb.max_box.x,aabb.min_box.y,aabb.max_box.z),// 101
            vec3(aabb.max_box.x,aabb.max_box.y,aabb.min_box.z),// 110
            vec3(aabb.max_box.x,aabb.max_box.y,aabb.max_box.z),// 111
        };
        
        float boxMin, boxMax;
        
        for (int i = 0; i < 6; i++) {
            projectOnAxis(box_Vertices, 8, frustum[i].normal , &boxMin, &boxMax);
            //if (boxMax < frustum[i].distance - EPSILON || boxMin > frustum[i].distance + EPSILON)
            if (boxMax < frustum[i].distance - EPSILON ) //|| boxMin > frustum[i].distance + EPSILON)
                return false; // No intersection possible.
        }
        
        return true;
        
        /*
		vec3 center = (aabb.min_box + aabb.max_box) * 0.5;
		vec3 halfbounds = (aabb.max_box - aabb.min_box) * 0.5;

		for (int i = 0; i < 6; i++) {
			
			if (Plane::pointDistanceToPlane(center + halfbounds * vec3(1, 1, 1), frustum[i]) >= 0.0f) continue; // 000
			if (Plane::pointDistanceToPlane(center + halfbounds * vec3(1, 1, -1), frustum[i]) >= 0.0f) continue; // 001
			if (Plane::pointDistanceToPlane(center + halfbounds * vec3(1, -1, 1), frustum[i]) >= 0.0f) continue; // 010
			if (Plane::pointDistanceToPlane(center + halfbounds * vec3(1, -1, -1), frustum[i]) >= 0.0f) continue; // 011
			if (Plane::pointDistanceToPlane(center + halfbounds * vec3(-1, 1, 1), frustum[i]) >= 0.0f) continue; // 100
			if (Plane::pointDistanceToPlane(center + halfbounds * vec3(-1, 1, -1), frustum[i]) >= 0.0f) continue; // 101
			if (Plane::pointDistanceToPlane(center + halfbounds * vec3(-1, -1, 1), frustum[i]) >= 0.0f) continue; // 110
			if (Plane::pointDistanceToPlane(center + halfbounds * vec3(-1, -1, -1), frustum[i]) >= 0.0f) continue; // 111

			return false;
		}
		return true;
        */
	}


}
