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

	bool Frustum::aabbOverlapsFrustum(const AABB &aabb, const Frustum &frustum) {
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
	}


}
