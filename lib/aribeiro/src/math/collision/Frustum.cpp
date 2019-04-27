#include "Frustum.h"

#include <aribeiro/all_math.h>

namespace aRibeiro {

namespace collision {
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
    
    
	void Frustum::computePlanes(const mat4& matrix) {
		mat4 clipMatrix = transpose(matrix);
		/*
					0 _11,1 _21,2 _31,3 _41,
					4 _12,5 _22,6 _32,7 _42,
					8 _13,9 _23,10_33,11_43,
					12_14,13_24,14_34,15_44;
		*/
        
        if (matrix.c4<0) {
            //
            //right handed
            //
            vec4 aux = clipMatrix[3] - clipMatrix[0];
            positivex.normal = toVec3(aux);
            positivex.distance = -aux.w;
            positivex.normalize();

            aux = clipMatrix[3] + clipMatrix[0];
            negativex.normal = toVec3(aux);
            negativex.distance = -aux.w;
            negativex.normalize();

            aux = clipMatrix[3] + clipMatrix[1];
            bottom.normal = toVec3(aux);
            bottom.distance = -aux.w;
            bottom.normalize();

            aux = clipMatrix[3] - clipMatrix[1];
            top.normal = toVec3(aux);
            top.distance = -aux.w;
            top.normalize();

            aux = clipMatrix[3] + clipMatrix[2];
            near.normal = toVec3(aux);
            near.distance = -aux.w;
            near.normalize();

            aux = clipMatrix[3] - clipMatrix[2];
            far.normal = toVec3(aux);
            far.distance = -aux.w;
            far.normalize();
            
        } else {
            //
            // Left Handed
            //
            vec4 aux = clipMatrix[3] - clipMatrix[0];
            positivex.normal = toVec3(aux);
            positivex.distance = -aux.w;
            positivex.normalize();
            
            aux = clipMatrix[3] + clipMatrix[0];
            negativex.normal = toVec3(aux);
            negativex.distance = -aux.w;
            negativex.normalize();
            
            aux = clipMatrix[3] + clipMatrix[1];
            bottom.normal = toVec3(aux);
            bottom.distance = -aux.w;
            bottom.normalize();
            
            aux = clipMatrix[3] - clipMatrix[1];
            top.normal = toVec3(aux);
            top.distance = -aux.w;
            top.normalize();
            
            aux = clipMatrix[3] + clipMatrix[2];
            near.normal = toVec3(aux);
            near.distance = -aux.w;
            near.normalize();
            
            aux = clipMatrix[3] - clipMatrix[2];
            far.normal = toVec3(aux);
            far.distance = -aux.w;
            far.normalize();
        }
        
        //
        // Compute vertices
        //
        
        Plane::intersectPlanes(positivex, top, near, &vertices[0]);
        Plane::intersectPlanes(positivex, bottom, near, &vertices[1]);
        Plane::intersectPlanes(negativex, bottom, near, &vertices[2]);
        Plane::intersectPlanes(negativex, top, near, &vertices[3]);
        
        Plane::intersectPlanes(positivex, top, far, &vertices[4]);
        Plane::intersectPlanes(positivex, bottom, far, &vertices[5]);
        Plane::intersectPlanes(negativex, bottom, far, &vertices[6]);
        Plane::intersectPlanes(negativex, top, far, &vertices[7]);
        
        
        
        //pre-calculate the vertex projections over the frustum plane
        for (int i = 0; i < 6; i++)
            projectOnAxis(vertices, 8, (*this)[i].normal , &minProjections[i], &maxProjections[i]);
        
	}

    Plane& Frustum::operator[](int idx){
        return ((&positivex)[idx]);
    }
    
    const Plane& Frustum::operator[](int idx)const {
        return ((&positivex)[idx]);
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
            //projectOnAxis(frustum.vertices, 8, frustum[i].normal , &frustum.minProjections[i], &frustum.maxProjections[i]);
            if (boxMax < frustum.minProjections[i] - EPSILON || boxMin > frustum.maxProjections[i] + EPSILON)
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

}
