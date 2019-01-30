#include "geometricOperations.h"
#include "all_math.h"

#include <limits>
#include <exception>

#include <stdexcept>// runtime_error

#include <string>

#define ASILVA_INCLUDE_OPENGL_HEADERS

#ifdef ASILVA_INCLUDE_OPENGL_HEADERS

#include <glew/glew.h>

#if defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#elif defined(WIN32) || \
	      defined(_WIN32) || defined(_WIN32_) || \
		  defined(__MSVC)
#include <windows.h>
#include <GL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#endif
#endif


namespace aRibeiro {

#define OPERATION_IMPLEMENTATION(TTYPE)\
TTYPE operator/( const TTYPE& vecA, const TTYPE& vecB ){ return (TTYPE(vecA)/=vecB); } \
TTYPE operator/( const TTYPE& vec , const float value ){ return (TTYPE(vec)/=value); } \
TTYPE operator/( const float value, const TTYPE& vec  ){ return (TTYPE(value)/=vec); } \
TTYPE operator*( const TTYPE& vecA, const TTYPE& vecB ){ return (TTYPE(vecA)*=vecB); } \
TTYPE operator*( const TTYPE& vec , const float value ){ return (TTYPE(vec)*=value); } \
TTYPE operator*( const float value, const TTYPE& vec  ){ return (TTYPE(value)*=vec); } \
TTYPE operator+( const TTYPE& vecA, const TTYPE& vecB ){ return (TTYPE(vecA)+=vecB); } \
TTYPE operator+( const TTYPE& vec , const float value ){ return (TTYPE(vec)+=value); } \
TTYPE operator+( const float value, const TTYPE& vec  ){ return (TTYPE(value)+=vec); } \
TTYPE operator-( const TTYPE& vecA, const TTYPE& vecB ){ return (TTYPE(vecA)-=vecB); } \
TTYPE operator-( const TTYPE& vec , const float value ){ return (TTYPE(vec)-=value); } \
TTYPE operator-( const float value, const TTYPE& vec  ){ return (TTYPE(value)-=vec); }

	OPERATION_IMPLEMENTATION(vec2)
		OPERATION_IMPLEMENTATION(vec3)
		OPERATION_IMPLEMENTATION(vec4)
		OPERATION_IMPLEMENTATION(tensor4)


	//[ 11 12 13 14 ]  [ x ]      [ x11+y12+z13+w14 ]
	//[ 21 22 23 24 ]  [ y ]   =  [ x21+y22+z23+w24 ]
	//[ 31 32 33 34 ]  [ z ]      [ x31+y32+z33+w34 ]
	//[ 41 42 43 44 ]  [ w ]      [ x41+y42+z43+w44 ]
	vec4 operator*(const mat4 &mat, const vec4 &vec) {
		vec4 result;
		result.x = mat._11*vec.x + mat._12*vec.y + mat._13*vec.z + mat._14*vec.w;
		result.y = mat._21*vec.x + mat._22*vec.y + mat._23*vec.z + mat._24*vec.w;
		result.z = mat._31*vec.x + mat._32*vec.y + mat._33*vec.z + mat._34*vec.w;
		result.w = mat._41*vec.x + mat._42*vec.y + mat._43*vec.z + mat._44*vec.w;
		return result;
	}

	//            [ 11 12 13 14 ]
	//(x y z w)   [ 21 22 23 24 ]   =  (11x+21y+31z+41w  12x+22y+32z+42w  13x+23y+33z+43w  14x+24y+34z+44w)
	//            [ 31 32 33 34 ]
	//            [ 41 42 43 44 ]
	vec4 operator*(const vec4 &vec, const mat4 &mat) {
		vec4 result;
		result.x = mat._11*vec.x + mat._21*vec.y + mat._31*vec.z + mat._41*vec.w;
		result.y = mat._12*vec.x + mat._22*vec.y + mat._32*vec.z + mat._42*vec.w;
		result.z = mat._13*vec.x + mat._23*vec.y + mat._33*vec.z + mat._43*vec.w;
		result.w = mat._14*vec.x + mat._24*vec.y + mat._34*vec.z + mat._44*vec.w;
		return result;
	}





	ARIBEIRO_API quat operator*(const quat &a, const quat &b) {

		return quat(
			a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
			a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
			a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
			a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);

		//return mul(a,b);
	}

	ARIBEIRO_API vec3 operator*(const quat &a, const vec3 &v) {
		//quat result = mul(a, mul(quat(v.x, v.y, v.z, 0.0f), conjugate(a)));
		quat result = a * quat(v.x, v.y, v.z, 0.0f) * conjugate(a);
		return vec3(result.x, result.y, result.z);
	}

	ARIBEIRO_API vec4 operator*(const quat &a, const vec4 &v) {
		//quat result = mul(a, mul(quat(v.x, v.y, v.z, 0.0f), conjugate(a)));
		quat result = a * quat(v.x, v.y, v.z, 0.0f) * conjugate(a);
		return vec4(result.x, result.y, result.z, v.w);
	}


	vec3 slerp(const vec3& a, const vec3& b, const float lerp) {

		//
		// https://github.com/g-truc/glm
		//
		// get cosine of angle between vectors (-1 -> 1)
		float CosAlpha = dot(a, b);
		// get angle (0 -> pi)
		float Alpha = acos(CosAlpha);
		// get sine of angle between vectors (0 -> 1)
		float SinAlpha = sin(Alpha);
		// this breaks down when SinAlpha = 0, i.e. Alpha = 0 or pi
		float t1 = sin( (1.0f - lerp) * Alpha ) / SinAlpha;
		float t2 = sin(lerp * Alpha) / SinAlpha;

		// interpolate src vectors
		return a * t1 + b * t2;
	}

	float angleBetween(const vec3& a, const vec3& b) {
		float cosA = dot(normalize(a), normalize(b));
		return acos(cosA);
	}


	//typedef vec2 float2;
	//typedef vec3 float3;
	//typedef vec4 float4;

	//namespace vectorConversion{

	//------------------------------------------------------------------------------
	/*
	  Funções para conversão de tipos
			 vec4 -> vec3 // com e sem divisão perspectiva
			 vec3 -> vec4
			 vec3 -> ptn4
	*/
	vec3 toVec3(const vec4 &v) {
		return vec3(v.x, v.y, v.z);
	}
	vec3 toVec3_PerspDiv(const vec4 &v) {
		return vec3(v.x, v.y, v.z)*(1.0f / v.w);
	}
	vec4 toVec4(const vec3 &v) {
		return vec4(v.x, v.y, v.z, 0);
	}
	vec4 toPtn4(const vec3 &v) {
		return vec4(v.x, v.y, v.z, 1);
	}
	// retorna um vetor dado um ângulo e raio
	vec2 polarToVec2(float pAngle, float pRadius)
	{
		vec2 vec(0);
		vec.x += pRadius * cosf(DEG2RAD(pAngle));
		vec.y += pRadius * sinf(DEG2RAD(pAngle));

		return vec;
	}



	//namespace vectorOperations{

	//------------------------------------------------------------------------------
	/*
	  função que calcula o produto vetorial entre dois vetores de dimensão 3
	*/
	vec3 cross(const vec3& a, const vec3& b) {
		return vec3((a.y * b.z - b.y * a.z),
			(b.x * a.z - a.x * b.z),
			(a.x * b.y - b.x * a.y));
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam o produto escalar entre dois vetores
	  para as dimensões:  2 D, 3 D, 4 D
	*/
	float dot(const vec2& a, const vec2& b) {
		return (a.x * b.x + a.y * b.y);
	}
	float dot(const vec3& a, const vec3& b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	float dot(const vec4& a, const vec4& b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que normalizam um determinado vetor, tornando-o
	  unitáio para as dimensões:  2 D, 3 D, 4 D
	*/
	vec2 normalize(const vec2& vec) {
		vec2 result = vec;
		if (vec == vec2(0)) return vec;
		const float TOLERANCE = 0.001f;
		// Don't normalize if we don't have to
		float mag2 = dot(vec, vec);
		if (fabs(mag2) > TOLERANCE && fabs(mag2 - 1.0f) > TOLERANCE)
			result = (vec * (1.0f / sqrtf(mag2)));
		return result;
	}
	vec3 normalize(const vec3& vec) {
		vec3 result = vec;
		if (vec == vec3(0)) return vec;
		const float TOLERANCE = 0.001f;
		// Don't normalize if we don't have to
		float mag2 = dot(vec, vec);
		if (fabs(mag2) > TOLERANCE && fabs(mag2 - 1.0f) > TOLERANCE)
			result = (vec * (1.0f / sqrtf(mag2)));
		return result;
	}
	vec4 normalize(const vec4& vec) {
		vec4 result = vec;
		if (vec == vec4(0)) return vec;
		const float TOLERANCE = 0.001f;
		// Don't normalize if we don't have to
		float mag2 = dot(vec, vec);
		if (fabs(mag2) > TOLERANCE && fabs(mag2 - 1.0f) > TOLERANCE)
			result = (vec * (1.0f / sqrtf(mag2)));
		return result;
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que limitam os valores internos do vetor
	  limitando seus valores máximos e mínimos de cada componente
	  para as dimensões:  1 D, 2 D, 3 D, 4 D
	*/
	float clamp(const float value, const float min, const float max) {
		return (value < min) ? min : ((value > max) ? max : value);
	}
	vec2 clamp(const vec2& value, const vec2& min, const vec2& max) {
		return vec2(
			(value.x < min.x) ? min.x : ((value.x > max.x) ? max.x : value.x),
			(value.y < min.y) ? min.y : ((value.y > max.y) ? max.y : value.y)
		);
	}
	vec3 clamp(const vec3& value, const vec3& min, const vec3& max) {
		return vec3(
			(value.x < min.x) ? min.x : ((value.x > max.x) ? max.x : value.x),
			(value.y < min.y) ? min.y : ((value.y > max.y) ? max.y : value.y),
			(value.z < min.z) ? min.z : ((value.z > max.z) ? max.z : value.z)
		);
	}
	vec4 clamp(const vec4& value, const vec4& min, const vec4& max) {
		return vec4(
			(value.x < min.x) ? min.x : ((value.x > max.x) ? max.x : value.x),
			(value.y < min.y) ? min.y : ((value.y > max.y) ? max.y : value.y),
			(value.z < min.z) ? min.z : ((value.z > max.z) ? max.z : value.z),
			(value.w < min.w) ? min.w : ((value.w > max.w) ? max.w : value.w)
		);
	}

	//------------------------------------------------------------------------------
	/*
	  Funções que retornam o vetor refletido de acordo com uma determinada Normal
	  para as dimensões:  2 D, 3 D, 4 D
	*/
	vec2 reflect(const vec2& a, const vec2& N) {
		return (a - N * (2.0f * dot(a, N)));
	}
	vec3 reflect(const vec3& a, const vec3& N) {
		return (a - N * (2.0f * dot(a, N)));
	}
	vec4 reflect(const vec4& a, const vec4& N) {
		return (a - N * (2.0f * dot(a, N)));
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam o tamanho quadrado de um vetor
	  para as dimensões:  2 D, 3 D, 4 D
	*/
	float sqrLength(const vec2 &a) {
		return dot(a, a);
	}
	float sqrLength(const vec3 &a) {
		return dot(a, a);
	}
	float sqrLength(const vec4 &a) {
		return dot(a, a);
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam o tamanho de um vetor
	  para as dimensões:  2 D, 3 D, 4 D
	*/
	float length(const vec2 &a) {
		return sqrtf(dot(a, a));
	}
	float length(const vec3 &a) {
		return sqrtf(dot(a, a));
	}
	float length(const vec4 &a) {
		return sqrtf(dot(a, a));
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam a distâcia
	  quadrada de um dado ponto até outro
	  para as dimensões:  1 D, 2 D, 3 D, 4 D
	*/
	float sqrDistance(const float a, const float b) {
		float ab = b - a;
		return ab * ab;
	}
	float sqrDistance(const vec2 &a, const vec2 &b) {
		vec2 ab = b - a;
		return dot(ab, ab);
	}
	float sqrDistance(const vec4 &a, const vec4 &b) {
		vec4 ab = b - a;
		return dot(ab, ab);
	}
	float sqrDistance(const vec3 &a, const vec3 &b) {
		vec3 ab = b - a;
		return dot(ab, ab);
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam a distâcia
	  de um dado ponto até outro
	  para as dimensões:  1 D, 2 D, 3 D, 4 D
	*/
	float distance(const float a, const float b) {
		float ab = b - a;
		return sqrtf(ab*ab);
	}
	float distance(const vec2 &a, const vec2 &b) {
		vec2 ab = b - a;
		return sqrtf(dot(ab, ab));
	}
	float distance(const vec4 &a, const vec4 &b) {
		vec4 ab = b - a;
		return sqrtf(dot(ab, ab));
	}
	float distance(const vec3 &a, const vec3 &b) {
		vec3 ab = b - a;
		return sqrtf(dot(ab, ab));
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam o componente paralelo a
	  um outro vetor unitáio.
	  para as dimensões:   2 D, 3 D, 4 D

	  ex.:
			 /                 |
		v2  /      | unitV     | vOut
		   *       *           *

		  o componente paralelo de v2 sobre unitV
		  é dado por vOut
	*/
	vec2 parallelComponent(const vec2 &a, const vec2 &unitV) {
		return unitV * (dot(a, unitV));//dot(a,unitV) É a projeção de a em unitV
	}
	vec3 parallelComponent(const vec3 &a, const vec3 &unitV) {
		return unitV * (dot(a, unitV));//dot(a,unitV) É a projeção de a em unitV
	}
	vec4 parallelComponent(const vec4 &a, const vec4 &unitV) {
		return unitV * (dot(a, unitV));//dot(a,unitV) É a projeção de a em unitV
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam o componente perpendicular a
	  um outro vetor unitáio.
	  para as dimensões:   2 D, 3 D, 4 D

	  ex.:
			 /
		v2  /      | unitV
		   *       *           *-- vOut

		  o componente perpendicular de v2 sobre unitV
		  é dado por vOut
	*/
	vec2 perpendicularComponent(const vec2 &a, const vec2 &unitV) {
		return a - unitV * (dot(a, unitV)); //unitV*(dot(a,unitV)) É o componenete paralelo
	}
	vec3 perpendicularComponent(const vec3 &a, const vec3 &unitV) {
		return a - unitV * (dot(a, unitV)); //unitV*(dot(a,unitV)) É o componenete paralelo
	}
	vec4 perpendicularComponent(const vec4 &a, const vec4 &unitV) {
		return a - unitV * (dot(a, unitV)); //unitV*(dot(a,unitV)) É o componenete paralelo
	}
	//------------------------------------------------------------------------------
	/*
	  realiza a decomposicao de vetores em seus componentes:
		  paralelo e perpendicular
	*/
	void vecDecomp(const vec2 &a, const vec2 &unitV,
		vec2 *perpendicular, vec2 *paralelo) {
		*paralelo = unitV * (dot(a, unitV));
		*perpendicular = a - *paralelo;
	}
	void vecDecomp(const vec3 &a, const vec3 &unitV,
		vec3 *perpendicular, vec3 *paralelo) {
		*paralelo = unitV * (dot(a, unitV));
		*perpendicular = a - *paralelo;
	}
	void vecDecomp(const vec4 &a, const vec4 &unitV,
		vec4 *perpendicular, vec4 *paralelo) {
		*paralelo = unitV * (dot(a, unitV));
		*perpendicular = a - *paralelo;
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que limitam um determinado ponto a um limite quadrático
	  maximo : em 2d um cículo; em 3d uma esfera; em 4d sei lá o que.
	  Para as dimensões:   2 D, 3 D, 4 D
	*/
	vec2 quadraticClamp(const vec2 &point, const vec2 &center, const float maxRadius) {
		vec2 direction = point - center;
		float length = sqrtf(sqrLength(direction));
		if (length > maxRadius)
			return center + direction * (maxRadius / length);
		return point;
	}
	vec3 quadraticClamp(const vec3 &point, const vec3 &center, const float maxRadius) {
		vec3 direction = point - center;
		float length = sqrtf(sqrLength(direction));
		if (length > maxRadius)
			return center + direction * (maxRadius / length);
		return point;
	}
	vec4 quadraticClamp(const vec4 &point, const vec4 &center, const float maxRadius) {
		vec4 direction = point - center;
		float length = sqrtf(sqrLength(direction));
		if (length > maxRadius)
			return center + direction * (maxRadius / length);
		return point;
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam o maximo ou minimo
	  Para as dimensões:   1 D, 2 D, 3D, 4D
	*/
	float maximum(const vec2 &a) {
		return (a.x > a.y) ? (a.x) : (a.y);
	}
	float maximum(const vec3 &a) {
		return (a.x > a.y) ? ((a.x > a.z) ? (a.x) : (a.z)) : (a.y);
	}
	float maximum(const vec4 &a) {
		return (a.x > a.y) ? ((a.x > a.z) ? ((a.x > a.w) ? (a.x) : (a.w)) : (a.z)) : (a.y);
	}
	float maximum(const float a, const float b) {
		return (a > b) ? a : b;
	}
	vec2 maximum(const vec2 &a, const vec2 &b) {
		return vec2((a.x > b.x) ? a.x : b.x,
			(a.y > b.y) ? a.y : b.y);
	}
	vec3 maximum(const vec3 &a, const vec3 &b) {
		return vec3((a.x > b.x) ? a.x : b.x,
			(a.y > b.y) ? a.y : b.y,
			(a.z > b.z) ? a.z : b.z);
	}
	vec4 maximum(const vec4 &a, const vec4 &b) {
		return vec4((a.x > b.x) ? a.x : b.x,
			(a.y > b.y) ? a.y : b.y,
			(a.z > b.z) ? a.z : b.z,
			(a.w > b.w) ? a.w : b.w);
	}
	float minimum(const vec2 &a) {
		return (a.x < a.y) ? (a.x) : (a.y);
	}
	float minimum(const vec3 &a) {
		return (a.x < a.y) ? ((a.x < a.z) ? (a.x) : (a.z)) : (a.y);
	}
	float minimum(const vec4 &a) {
		return (a.x < a.y) ? ((a.x < a.z) ? ((a.x < a.w) ? (a.x) : (a.w)) : (a.z)) : (a.y);
	}
	float minimum(const float a, const float b) {
		return (a < b) ? a : b;
	}
	vec2 minimum(const vec2 &a, const vec2 &b) {
		return vec2((a.x < b.x) ? a.x : b.x,
			(a.y < b.y) ? a.y : b.y);
	}
	vec3 minimum(const vec3 &a, const vec3 &b) {
		return vec3((a.x < b.x) ? a.x : b.x,
			(a.y < b.y) ? a.y : b.y,
			(a.z < b.z) ? a.z : b.z);
	}
	vec4 minimum(const vec4 &a, const vec4 &b) {
		return vec4((a.x < b.x) ? a.x : b.x,
			(a.y < b.y) ? a.y : b.y,
			(a.z < b.z) ? a.z : b.z,
			(a.w < b.w) ? a.w : b.w);
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam 1 se o nmero for maior ou igual a zero
						   0 se o numero for zero de acordo com um epsilon
						  -1 se o numero for menor que zero
	  Para as dimensões:   1 D
	*/
	float sign(const float a) {
		return (a >= 0) ? 1.0f : -1.0f;
	}
	//------------------------------------------------------------------------------
	/*
	  Funções que retornam o valor absoluto para cada componente
	  Para as dimensões:   1 D, 2 D, 3 D, 4 D
	*/

#if _MSC_VER

#include <cstdlib>

	float absv(float a) {
		return abs(a);
	}
	vec2 absv(const vec2 &a) {
		return vec2(abs(a.x),
			abs(a.y));
	}
	vec3 absv(const vec3 &a) {
		return vec3(abs(a.x),
			abs(a.y),
			abs(a.z));
	}
	vec4 absv(const vec4 &a) {
		return vec4(abs(a.x),
			abs(a.y),
			abs(a.z),
			abs(a.w));
	}

#else

	float absv(float a) {
		return (a < 0) ? (-a) : (a);
	}
	vec2 absv(const vec2 &a) {
		return vec2((a.x < 0) ? (-a.x) : (a.x),
			(a.y < 0) ? (-a.y) : (a.y));
	}
	vec3 absv(const vec3 &a) {
		return vec3((a.x < 0) ? (-a.x) : (a.x),
			(a.y < 0) ? (-a.y) : (a.y),
			(a.z < 0) ? (-a.z) : (a.z));
	}
	vec4 absv(const vec4 &a) {
		return vec4((a.x < 0) ? (-a.x) : (a.x),
			(a.y < 0) ? (-a.y) : (a.y),
			(a.z < 0) ? (-a.z) : (a.z),
			(a.w < 0) ? (-a.w) : (a.w));
	}

#endif

	//------------------------------------------------------------------------------



	//namespace interpolation{

	//------------------------------------------------------------------------------
	/*
	  Funções que retornam a interpolação linear entre 2 pontos
	  para as dimensões:  1 D, 2 D, 3 D, 4 D
	*/
	float lerp(const float a, const  float b, const float fator) {
		//  return a+(b-a)*fator;
		return a * (1.0f - fator) + (b*fator);
	}
	vec2 lerp(const vec2 &a, const  vec2 &b, const float fator) {
		//  return a+(b-a)*fator;
		return a * (1.0f - fator) + (b*fator);
	}
	vec3 lerp(const vec3 &a, const  vec3 &b, const float fator) {
		//  return a+(b-a)*fator;
		return a * (1.0f - fator) + (b*fator);
	}
	vec4 lerp(const vec4 &a, const  vec4 &b, const float fator) {
		//  return a+(b-a)*fator;
		return a * (1.0f - fator) + (b*fator);
	}
	tensor4 lerp(const tensor4 &a, const  tensor4 &b, const float fator) {
		//  return a+(b-a)*fator;
		return a * (1.0f - fator) + (b*fator);
	}


	/*
	  o calculo desta interpolação baricentrica baseada em duas coordenadas
	não é afetado pela projeção perspectiva se os coeficientes U e V forem calculados
	no triangulo no espaço euclidiano.
	*/
	vec3 barylerp(float u, float v, const vec3 &v0, const vec3 &v1, const vec3 &v2) {
		// return v0*(1-uv[0]-uv[1])+v1*uv[0]+v2*uv[1];
		return v0 * (1 - u - v) + v1 * u + v2 * v;
	}

	//dx - [0..1]
	//dy - [0..1]
	//
	//  D-f(0,1) ---*----- C-f(1,1)
	//     |        |         |
	//     |        |         |
	//     *--------P---------*   P = (dx,dy)
	//     |        |         |
	//     |        |         |
	//  A-f(0,0) ---*----- B-f(1,0)
	//
	vec3 blerp(vec3 A, vec3 B, vec3 C, vec3 D,
		float dx, float dy) {
		float omdx = 1.0f - dx,
			omdy = 1.0f - dy;
		return omdx * omdy*A + omdx * dy*D +
			dx * omdy*B + dx * dy*C;
	}

	/*
	q(t) = 0.5 * (1.0f,t,t2,t3)  *

	[  0  2  0  0 ]  [P0]
	[ -1  0  1  0 ]* [P1]
	[  2 -5  4 -1 ]  [P2]
	[ -1  3 -3  1 ]  [P3]

	q(t) = 0.5 *((2 * P1) +
				 (-P0 + P2) * t +
				 (2*P0 - 5*P1 + 4*P2 - P3) * t2 +
				 (-P0 + 3*P1- 3*P2 + P3) * t3)

	o valor de t deve estar entre 0 e 1
	*/

	vec3 splineCatmullRom(vec3 P0, vec3 P1, vec3 P2, vec3 P3, float t) {
		return 0.5f *((2.0f * P1) +
			(P2 - P0) * t +
			(2.0f*P0 - 5.0f*P1 + 4.0f*P2 - P3) * (t*t) +
			(3.0f*P1 - P0 - 3.0f*P2 + P3) * (t*t*t));
	}

	vec2 splineCatmullRom(vec2 P0, vec2 P1, vec2 P2, vec2 P3, float t) {
		return 0.5f *((2.0f * P1) +
			(P2 - P0) * t +
			(2.0f*P0 - 5.0f*P1 + 4.0f*P2 - P3) * (t*t) +
			(3.0f*P1 - P0 - 3.0f*P2 + P3) * (t*t*t));
	}




	//namespace matrixOperations
	//{
	//using namespace vectorOperations;
	//using namespace vectorConversion;

	mat4 extractRotation(const mat4& m) {
		return mat4(m._11, m._12, m._13, 0,
			m._21, m._22, m._23, 0,
			m._31, m._32, m._33, 0,
			0, 0, 0, 1);
	}

	vec3 extractXaxis(const mat4& m) {
		return vec3(m._11, m._21, m._31);
	}
	vec3 extractYaxis(const mat4& m) {
		return vec3(m._12, m._22, m._32);
	}
	vec3 extractZaxis(const mat4& m) {
		return vec3(m._13, m._23, m._33);
	}

	vec3 extractTranslation(const mat4& m) {
		return vec3(m._14, m._24, m._34);
	}

	mat4 transpose(const mat4& m) {
		return mat4(m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44);
	}

#define mkIndex(y, x) ((y)*4+(x))

	mat4 inv(const mat4& m) {
		float determinant =
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) +
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) +
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) +
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) +
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) +
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) +
			(m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) +
			(m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) +
			(m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)]) +
			(m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) +
			(m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) +
			(m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) -
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) -
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) -
			(m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) -
			(m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) -
			(m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) -
			(m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)]);
		if (determinant == 0) {
			//1/0 = &infin;
			 //log (0) = -&infin;
			 //sqrt (-1) = NaN
			//suporte NAN
			return mat4(std::numeric_limits<float>::quiet_NaN());
			//throw std::runtime_error("singular matrix, cant calculate determinant");
		}
		determinant = 1.0f / determinant;
		mat4 retorno;
		retorno.array[mkIndex(0, 0)] =
			((m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) -
			(m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)])) *
			determinant;
		retorno.array[mkIndex(0, 1)] =
			((m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) -
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)])) *
			determinant;
		retorno.array[mkIndex(0, 2)] =
			((m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 2)]) -
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 1)])) *
			determinant;
		retorno.array[mkIndex(0, 3)] =
			((m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)]) -
			(m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)])) *
			determinant;
		retorno.array[mkIndex(1, 0)] =
			((m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)])) *
			determinant;
		retorno.array[mkIndex(1, 1)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)])) *
			determinant;
		retorno.array[mkIndex(1, 2)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 2)])) *
			determinant;
		retorno.array[mkIndex(1, 3)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 3)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)])) *
			determinant;
		retorno.array[mkIndex(2, 0)] =
			((m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) + (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)]) -
			(m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) - (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)])) *
			determinant;
		retorno.array[mkIndex(2, 1)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 3)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)])) *
			determinant;
		retorno.array[mkIndex(2, 2)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 3)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 0)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 1)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(3, 1)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 3)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 0)])) *
			determinant;
		retorno.array[mkIndex(2, 3)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 1)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 3)]) + (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 3)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 3)] * m.array[mkIndex(2, 0)]) - (m.array[mkIndex(0, 3)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)])) *
			determinant;
		retorno.array[mkIndex(3, 0)] =
			((m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)])) *
			determinant;
		retorno.array[mkIndex(3, 1)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 0)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 1)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(2, 2)] * m.array[mkIndex(3, 1)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(2, 0)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(2, 1)] * m.array[mkIndex(3, 0)])) *
			determinant;
		retorno.array[mkIndex(3, 2)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 1)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 2)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 0)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(3, 2)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(3, 0)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(3, 1)])) *
			determinant;
		retorno.array[mkIndex(3, 3)] =
			((m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 2)]) + (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 0)]) + (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 1)]) -
			(m.array[mkIndex(0, 0)] * m.array[mkIndex(1, 2)] * m.array[mkIndex(2, 1)]) - (m.array[mkIndex(0, 1)] * m.array[mkIndex(1, 0)] * m.array[mkIndex(2, 2)]) - (m.array[mkIndex(0, 2)] * m.array[mkIndex(1, 1)] * m.array[mkIndex(2, 0)])) *
			determinant;
		return retorno;
	}

	/*
	mat4 inv_faster(const mat4& m) {
		mat4 result;
		inverse_alternative(m, &result);
		return result;
	}
	*/


	mat4 getModelviewFromOpenGL() {
		mat4 retorno;
#ifdef ASILVA_INCLUDE_OPENGL_HEADERS
		glGetFloatv(GL_MODELVIEW_MATRIX, retorno.array);
#else
		throw std::runtime_error("You must include GL headers to use the math->OpenGL integration functions.");
#endif
		return retorno;
	}

	mat4 getProjectionFromOpenGL() {
		mat4 retorno;
#ifdef ASILVA_INCLUDE_OPENGL_HEADERS
		glGetFloatv(GL_PROJECTION_MATRIX, retorno.array);
#else
		throw std::runtime_error("You must include GL headers to use the math->OpenGL integration functions.");
#endif
		return retorno;
	}

	mat4 getTextureFromGL() {
		mat4 retorno;
#ifdef ASILVA_INCLUDE_OPENGL_HEADERS
		glGetFloatv(GL_TEXTURE_MATRIX, retorno.array);
#else
		throw std::runtime_error("You must include GL headers to use the math->OpenGL integration functions.");
#endif
		return retorno;
	}

	void loadMatrixToGL(const mat4& m) {
#ifdef ASILVA_INCLUDE_OPENGL_HEADERS
		glLoadMatrixf((m).array);
#else
		throw std::runtime_error("You must include GL headers to use the math->OpenGL integration functions.");
#endif
	}

	void mulMatrixToGL(const mat4& m) {
#ifdef ASILVA_INCLUDE_OPENGL_HEADERS
		glMultMatrixf((m).array);
#else
		throw std::runtime_error("You must include GL headers to use the math->OpenGL integration functions.");
#endif
	}

	mat4 translate(const float _x_, const float _y_, const float _z_) {
		return mat4(1, 0, 0, _x_,
			0, 1, 0, _y_,
			0, 0, 1, _z_,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 translate(const vec3 &_v_) {
		return mat4(1, 0, 0, _v_.x,
			0, 1, 0, _v_.y,
			0, 0, 1, _v_.z,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 translate(const vec4 &_v_) {
		return mat4(1, 0, 0, _v_.x,
			0, 1, 0, _v_.y,
			0, 0, 1, _v_.z,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 scale(const float _x_, const float _y_, const float _z_) {
		return mat4(_x_, 0, 0, 0,
			0, _y_, 0, 0,
			0, 0, _z_, 0,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 scale(const vec3 &_v_) {
		return mat4(_v_.x, 0, 0, 0,
			0, _v_.y, 0, 0,
			0, 0, _v_.z, 0,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 scale(const vec4 &_v_) {
		return mat4(_v_.x, 0, 0, 0,
			0, _v_.y, 0, 0,
			0, 0, _v_.z, 0,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	/*
	  rotation in right_handed system
	*/
	mat4 xRotate(const float _phi_) {
		float c = cos(_phi_);
		float s = sin(_phi_);
		return mat4(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 yRotate(const float _theta_) {
		float c = cos(_theta_);
		float s = sin(_theta_);
		return mat4(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 zRotate(const float _psi_) {
		float c = cos(_psi_);
		float s = sin(_psi_);
		return mat4(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 eulerRotate(float roll, float pitch, float yaw) {
		return zRotate(yaw) * yRotate(pitch) * xRotate(roll);
	}
	//------------------------------------------------------------------------------
	void extractEuler(mat4 m, float *roll, float *pitch, float *yaw) {
		//
		// https://www.learnopencv.com/rotation-matrix-to-euler-angles/
		//
		float sy = sqrt(m._11 * m._11 + m._21 * m._21);

		bool singular = sy < 1e-6f; // If

		float x, y, z;
		if (!singular)
		{
			x = atan2(m._32, m._33);
			y = atan2(-m._31, sy);
			z = atan2(m._21, m._11);
		}
		else
		{
			x = atan2(-m._23, m._22);
			y = atan2(-m._31, sy);
			z = 0;
		}

		*roll = x;
		*pitch = y;
		*yaw = z;
	}
	//------------------------------------------------------------------------------
	/*
	openglRotation
	c = cosseno
	s = seno
	||<x,y,z>|| = 1

	|  xx(1-c)+c     xy(1-c)-zs    xz(1-c)+ys     0  |
	|  yx(1-c)+zs    yy(1-c)+c     yz(1-c)-xs     0  |
	|  xz(1-c)-ys    yz(1-c)+xs    zz(1-c)+c      0  |
	|       0            0             0          1  |

	outra rotação achada na net
	1 + (1-cos(angle))*(x*x-1)        -z*sin(angle)+(1-cos(angle))*x*y     y*sin(angle)+(1-cos(angle))*x*z
	z*sin(angle)+(1-cos(angle))*x*y    1 + (1-cos(angle))*(y*y-1)          -x*sin(angle)+(1-cos(angle))*y*z
	-y*sin(angle)+(1-cos(angle))*x*z   x*sin(angle)+(1-cos(angle))*y*z     1 + (1-cos(angle))*(z*z-1)

	*/
	//------------------------------------------------------------------------------
	mat4 rotate(const float _ang_, float x, float y, float z) {
		/*
		  depois tem como otimizar esta rotação
		*/
		float length_inv = sqrt(x*x + y * y + z * z);
		if (length_inv == 0) throw std::runtime_error("division by zero");
		length_inv = 1.0f / length_inv;
		x *= length_inv;
		y *= length_inv;
		z *= length_inv;
		float c = cos(_ang_);
		float s = sin(_ang_);
		//original -- rota�o em sentido anti-hor�io
		return  mat4(x*x*(1 - c) + c, x*y*(1 - c) - z * s, x*z*(1 - c) + y * s, 0,
			y*x*(1 - c) + z * s, y*y*(1 - c) + c, y*z*(1 - c) - x * s, 0,
			x*z*(1 - c) - y * s, y*z*(1 - c) + x * s, z*z*(1 - c) + c, 0,
			0, 0, 0, 1);
		//*/
		/*
		  //transposto -- rota�o em sentido hor�io
		  return  mat4(x*x*(1-c)+c  ,  y*x*(1-c)+z*s  ,  x*z*(1-c)-y*s,   0  ,
					   x*y*(1-c)-z*s,  y*y*(1-c)+c    ,  y*z*(1-c)+x*s,   0  ,
					   x*z*(1-c)+y*s,  y*z*(1-c)-x*s  ,  z*z*(1-c)+c  ,   0  ,
						   0        ,        0        ,      0        ,   1  );
		*/
	}
	//------------------------------------------------------------------------------
	mat4 rotate(const float _ang_, const vec3 &axis) {
		return rotate(_ang_, axis.x, axis.y, axis.z);
	}
	//------------------------------------------------------------------------------
	mat4 projection_perspective(const float FovY, const float aspectX, const float near_, const float far_) {

		/*
		   f=cotangent(CampoDeVisao/2)
		matriz:

		f/aspect  0      0                           0
		0         f      0                           0
		0         0    (zfar+znear)/(znear-zfar)    (2*zfar*znear)/(znear-zfar)
		0         0     -1                           0
		   */
		if ((aspectX == 0.0f) || (near_ - far_) == 0) {
			return mat4::IdentityMatrix;
		}
		float focus = (float)tanf(DEG2RAD(FovY) / 2.0f);
		if (focus == 0.0f) focus = 0.000000000002f;
		focus = 1.0f / focus;
		return mat4(focus / aspectX, 0, 0, 0,
			0, focus, 0, 0,
			0, 0, (near_ + far_) / (near_ - far_), (2.0f*near_*far_) / (near_ - far_),
			0, 0, -1, 0);
	}
	//------------------------------------------------------------------------------
	// unidade da distancia focal em relação a mesma unidade do width e height
	//  ex.: se considerar milimetros (mm) então distancia focal de 35mm para tela de 50x30 mm
	//             projection_perspective(35,50,30,0.001,100.0)
	mat4 projection_perspective(const float focalLength, const float w, const float h, const float near_, const float far_) {
		float fovY = 2.0f*atanf((h*0.5f) / focalLength);
		fovY = RAD2DEG(fovY);
		float aspectX = w / h;
		return projection_perspective(fovY, aspectX, near_, far_);
	}
	//------------------------------------------------------------------------------
	mat4 projection_frustum(const float Left, const float Right, const float Bottom, const float Top, const float Near, const float Far) {
		return mat4((2 * Near) / (Right - Left), 0, (Right + Left) / (Right - Left), 0,
			0, (2 * Near) / (Top - Bottom), (Top + Bottom) / (Top - Bottom), 0,
			0, 0, (-(Far + Near)) / (Far - Near), (-2 * Far*Near) / (Far - Near),
			0, 0, -1, 0);
	}
	//------------------------------------------------------------------------------
	mat4 projection_ortho(const float Left, const float Right, const float Bottom, const float Top, const float Near, const float Far) {
		return mat4(2.0f / (Right - Left), 0, 0, -(Right + Left) / (Right - Left),
			0, 2.0f / (Top - Bottom), 0, -(Top + Bottom) / (Top - Bottom),
			0, 0, -2.0f / (Far - Near), -(Far + Near) / (Far - Near),
			0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	mat4 lookAt(const vec3 &front, const vec3 &up, const vec3 &position) {
		vec3 lookTo = front;
		vec3 x, y, z;
		z = normalize(lookTo)*-1;
		x = normalize(cross(up, z));
		y = normalize(cross(z, x));
		return //scale(0.002f,0.002f,0.002f)*
			mat4(x.x, x.y, x.z, -dot(x, position),
				y.x, y.y, y.z, -dot(y, position),
				z.x, z.y, z.z, -dot(z, position),
				0, 0, 0, 1);
	}
	//------------------------------------------------------------------------------
	/*
	** inverse = invert(src)
	** New, faster implementation by Shan Hao Bo, April 2006.
	*/
	/*
	bool inverse_alternative(const mat4 &src, mat4 *inverse) {
		int i, j, k;
		float t;
		mat4 temp = src;
		*inverse = mat4::IdentityMatrix;
		for (i = 0; i < 4; i++) {
			if (temp(i, i) == 0.0f) {
				// Look for non-zero element in column
				for (j = i + 1; j < 4; j++)
					if (temp(i, j) != 0.0f) break;
				if (j != 4) {
					// Swap rows.
					for (k = 0; k < 4; k++) {
						t = temp(k, i);
						temp(k, i) = temp(k, j);
						temp(k, j) = t;

						t = inverse->array[i * 4 + k];
						inverse->array[i * 4 + k] = inverse->array[j * 4 + k];
						inverse->array[j * 4 + k] = t;
					}
				}
				else {
					// No non-zero pivot.  The matrix is singular, which shouldn't
					// happen.  This means the user gave us a bad matrix.
					return false;
				}
			}
			t = 1.0f / temp(i, i);
			for (k = 0; k < 4; k++) {
				temp(k, i) *= t;
				inverse->array[i * 4 + k] *= t;
			}
			for (j = 0; j < 4; j++) {
				if (j != i) {
					t = temp(i, j);
					for (k = 0; k < 4; k++) {
						temp(k, j) -= temp(k, i)*t;
						inverse->array[j * 4 + k] -= inverse->array[i * 4 + k] * t;
					}
				}
			}
		}
		return true;
	}
	*/
	//------------------------------------------------------------------------------
	// retorna false se a matriz final é singular e n pode ser invertida
	bool unproject(vec3 pointInWindow,
		const mat4 &modelViewMatrix,
		const mat4 &projectionMatrix,
		int viewportX, int viewportY, int viewportW, int viewportH,
		vec3 *worldPtn) {
		mat4 modelViewProjection_inverse = projectionMatrix * modelViewMatrix;//pre_multiplyed ogl Like

		modelViewProjection_inverse = inv(modelViewProjection_inverse);

		if ( modelViewProjection_inverse.array[0] == std::numeric_limits<float>::quiet_NaN()) {
			return false;
		}

		/*
		if (!inverse_alternative(modelViewProjection_inverse, &modelViewProjection_inverse))
			return false;
		*/
		/* Map x and y from window coordinates */
		pointInWindow.x = (pointInWindow.x - float(viewportX)) / float(viewportW);
		pointInWindow.y = (pointInWindow.y - float(viewportY)) / float(viewportH);
		/* Map to range -1 to 1 */
		pointInWindow = pointInWindow * 2.0f - 1.0f;
		vec4 position_homogeneos = modelViewProjection_inverse * vec4(pointInWindow, 1);
		if (position_homogeneos.w == 0.0)
			return false;
		*worldPtn = toVec3_PerspDiv(position_homogeneos);
		return true;
	}
	//------------------------------------------------------------------------------
	bool GL_unproject(const vec3 &pointInWindow,
		vec3 *worldPtn) {
#ifdef ASILVA_INCLUDE_OPENGL_HEADERS
		mat4 modelViewMatrix = getModelviewFromOpenGL();
		mat4 projectionMatrix = getProjectionFromOpenGL();
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, &viewport[0]);
		return   unproject(pointInWindow,
			modelViewMatrix,
			projectionMatrix,
			viewport[0], viewport[1], viewport[2], viewport[3],
			worldPtn);
#else
		throw std::runtime_error("You must include GL headers to use the math->OpenGL integration functions.");
#endif
	}

	//------------------------------------------------------------------------------
	// retorna false se a matriz final é singular e n pode ser invertida
	bool project(vec3 worldPtn,
		const mat4 &modelViewMatrix,
		const mat4 &projectionMatrix,
		int viewportX, int viewportY, int viewportW, int viewportH,
		vec3 *pointInWindow) {
		mat4 modelViewProjection = projectionMatrix * modelViewMatrix;//pre_multiplyed ogl Like
		vec4 position_homogeneos = modelViewProjection * vec4(worldPtn, 1);
		if (position_homogeneos.w == 0.0)
			return false;
		vec3 result = toVec3_PerspDiv(position_homogeneos);
		/* Map x, y and z to range 0-1 */
		result = result * 0.5f + 0.5f;
		/* Map x,y to viewport */
		result.x = result.x*float(viewportW) + float(viewportX);
		result.y = result.y*float(viewportH) + float(viewportY);
		*pointInWindow = result;
		return true;
	}
	//------------------------------------------------------------------------------
	bool GL_project(const vec3 &worldPtn,
		vec3 *pointInWindow) {
#ifdef ASILVA_INCLUDE_OPENGL_HEADERS
		mat4 modelViewMatrix = getModelviewFromOpenGL();
		mat4 projectionMatrix = getProjectionFromOpenGL();
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, &viewport[0]);
		return project(worldPtn,
			modelViewMatrix,
			projectionMatrix,
			viewport[0], viewport[1], viewport[2], viewport[3],
			pointInWindow);
#else
		throw std::runtime_error("You must include GL headers to use the math->OpenGL integration functions.");
#endif
	}



	//}



	//namespace quaternionOperations{

		//------------------------------------------------------------------------------
	quat toQuat(const vec3& v) {
		const float t = 1.0f - (v.x*v.x) - (v.y*v.y) - (v.z*v.z);
		if (t < 0.0f)
			return quat(v.x, v.y, v.z, 0.0f);
		else
			return quat(v.x, v.y, v.z, sqrt(t));
	}
	//------------------------------------------------------------------------------
	quat normalize(const quat& q) {
		quat result = q;
		const float TOLERANCE = 0.001f;
		// Don't normalize if we don't have to
		float mag2 = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
		if (fabs(mag2) > TOLERANCE && fabs(mag2 - 1.0f) > TOLERANCE) {
			float mag = sqrt(mag2);
			result.w = q.w / mag;
			result.x = q.x / mag;
			result.y = q.y / mag;
			result.z = q.z / mag;
		}
		return result;
	}
	//------------------------------------------------------------------------------
	float sqrLength(const quat& q) {
		return q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
	}
	//------------------------------------------------------------------------------
	float length(const quat& q) {
		return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	}
	//------------------------------------------------------------------------------
	// Performs a spherical interpolation between two quaternions
	// Implementation adopted from the gmtl project.
	quat slerp(const quat& a, const quat& b, const float lerp) {
		/*
		if (lerp <= 0.0f) return a;
		if (lerp >= 1.0f) return b;

		// calc cosine theta
		float cosom = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

		// adjust signs (if necessary)
		quat end = b;
		if (cosom < 0.0f) {
			cosom = -cosom;
			end.x = -end.x;   // Reverse all signs
			end.y = -end.y;
			end.z = -end.z;
			end.w = -end.w;
		}

		// Calculate coefficients
		float sclp, sclq;
		if ((1.0f - cosom) > 0.0001f) { // 0.0001 -> some epsillon
			// Standard case (slerp)
			float omega, sinom;
			omega = acos(cosom); // extract theta from dot product's cos theta
			sinom = sin(omega);
			sclp = sin((1.0f - lerp) * omega) / sinom;
			sclq = sin(lerp * omega) / sinom;
		}
		else {
			// Very close, do linear interp (because it's faster)
			sclp = 1.0f - lerp;
			sclq = lerp;
		}
		return quat(sclp * a.x + sclq * end.x,
			sclp * a.y + sclq * end.y,
			sclp * a.z + sclq * end.z,
			sclp * a.w + sclq * end.w);
		*/
		//
		// https://github.com/g-truc/glm
		//
		if (lerp <= 0.0f) return a;
		if (lerp >= 1.0f) return b;

		//float fCos = dot(a, b);
		float fCos = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		quat y2(b); //BUG!!! qua<T> y2;
		if (fCos < 0.0f)
		{
			//y2 = -y;
			y2 = quat(-b.x, -b.y, -b.z, -b.w);
			fCos = -fCos;
		}

		//if(fCos > 1.0f) // problem
		float k0, k1;
		const float EPSILON = 1e-6f;
		if (fCos > (1.0f - EPSILON))
		{
			k0 = 1.0f - lerp;
			k1 = 0.0f + lerp; //BUG!!! 1.0f + lerp;
		}
		else
		{
			float fSin = sqrt( 1.0f - fCos * fCos );
			float fAngle = atan2(fSin, fCos);
			float fOneOverSin = 1.0f / fSin;
			k0 = sin((1.0f - lerp) * fAngle) * fOneOverSin;
			k1 = sin((0.0f + lerp) * fAngle) * fOneOverSin;
		}

		return quat(
			k0 * a.x + k1 * y2.x,
			k0 * a.y + k1 * y2.y,
			k0 * a.z + k1 * y2.z,
			k0 * a.w + k1 * y2.w);
	}
	//------------------------------------------------------------------------------
	/*
	// Multiplying q1 with q2 applies the rotation q2 to q1
	quat mul(const quat& a, const quat& b) {
		// the constructor takes its arguments as (x, y, z, w)
		return quat(
			a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
			a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
			a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
			a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
	}
	*/
	//------------------------------------------------------------------------------
	/*
	vec3 rotateVec(const quat& a, const vec3& v) {
		quat result = mul(a, mul(quat(v.x, v.y, v.z, 0.0f), conjugate(a)));
		return vec3(result.x, result.y, result.z);
	}
	//------------------------------------------------------------------------------
	vec4 rotateVec(const quat& a, const vec4& v) {
		quat result = mul(a, mul(quat(v.x, v.y, v.z, 0.0f), conjugate(a)));
		return vec4(result.x, result.y, result.z, v.w);
	}
	*/
	//------------------------------------------------------------------------------
	quat quatFromAxisAngle(const vec3& axis, const float angle_rad) {
		float sinAngle;
		float angle = angle_rad * 0.5f;
		vec3 vn = normalize(axis);
		sinAngle = sin(angle);
		return quat((vn.x * sinAngle),
			(vn.y * sinAngle),
			(vn.z * sinAngle),
			cos(angle));
	}
	//------------------------------------------------------------------------------
	quat quatFromEuler(float roll, float pitch, float yaw) {

		pitch *= 0.5f;
		yaw *= 0.5f;
		roll *= 0.5f;

		float sinPitch = sin(pitch);
		float cosPitch = cos(pitch);
		float sinYaw = sin(yaw);
		float cosYaw = cos(yaw);
		float sinRoll = sin(roll);
		float cosRoll = cos(roll);

		float cosPitchCosYaw = cosPitch * cosYaw;
		float sinPitchSinYaw = sinPitch * sinYaw;

		return quat(
			sinRoll * cosPitchCosYaw - cosRoll * sinPitchSinYaw,
			cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
			cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
			cosRoll * cosPitchCosYaw + sinRoll * sinPitchSinYaw
		);

		/*
		return 
			quatFromAxisAngle(vec3(0.0, 0.0, 1.0), yaw) * 
			quatFromAxisAngle(vec3(0.0, 1.0, 0.0), pitch) * 
			quatFromAxisAngle(vec3(1.0, 0.0, 0.0), roll);
		*/
		
	}
	//------------------------------------------------------------------------------
	quat conjugate(const quat& a) {
		return quat(-a.x, -a.y, -a.z, a.w);
	}
	//------------------------------------------------------------------------------
	quat extractQuat(const mat4& m) {
		float t = 1.0f + m._11 + m._22 + m._33;
		// large enough
		if (t > 0.001f) {
			float s = sqrt(t) * 2.0f;
			return quat((m._32 - m._23) / s,
				(m._13 - m._31) / s,
				(m._21 - m._12) / s,
				0.25f * s);
		} // else we have to check several cases
		else if (m._11 > m._22 && m._11 > m._33) {
			// Column 0:
			float s = sqrt(1.0f + m._11 - m._22 - m._33) * 2.0f;
			return quat(0.25f * s,
				(m._21 + m._12) / s,
				(m._13 + m._31) / s,
				(m._32 - m._23) / s);
		}
		else if (m._22 > m._33) {
			// Column 1:
			float s = sqrt(1.0f + m._22 - m._11 - m._33) * 2.0f;
			return quat((m._21 + m._12) / s,
				0.25f * s,
				(m._32 + m._23) / s,
				(m._13 - m._31) / s);
		}
		else {
			// Column 2:
			float s = sqrt(1.0f + m._33 - m._11 - m._22) * 2.0f;
			return quat((m._13 + m._31) / s,
				(m._32 + m._23) / s,
				0.25f * s,
				(m._21 - m._12) / s);
		}
	}
	//------------------------------------------------------------------------------
	mat4 toMat4(const quat& q) {
		float x2 = q.x * q.x;
		float y2 = q.y * q.y;
		float z2 = q.z * q.z;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;
		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;

		// This calculation would be a lot more complicated for non-unit length quaternions
		// Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
		//   OpenGL
		return mat4(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

	}
	//------------------------------------------------------------------------------
	void extractAxisAngle(const quat& q, vec3 *axis, float *angle) {
		float scale = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
		axis->x = q.x / scale;
		axis->y = q.y / scale;
		axis->z = q.z / scale;
		*angle = acos(q.w) * 2.0f;
	}
	//------------------------------------------------------------------------------

	/*
	//
	// http://bediyap.com/programming/convert-quaternion-to-euler-rotations/ - Doesn't work
	//
	enum RotSeq { zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy, xzx };

	static inline void twoaxisrot(float r11, float r12, float r21, float r31, float r32, float res[]) {
		res[0] = atan2(r11, r12);
		res[1] = acos(r21);
		res[2] = atan2(r31, r32);
	}

	static inline void threeaxisrot(float r11, float r12, float r21, float r31, float r32, float res[]) {
		res[0] = atan2(r31, r32);
		res[1] = asin(r21);
		res[2] = atan2(r11, r12);
	}

	// note: 
	// return values of res[] depends on rotSeq.
	// i.e.
	// for rotSeq zyx, 
	// x = res[0], y = res[1], z = res[2]
	// for rotSeq xyz
	// z = res[0], y = res[1], x = res[2]
	// ...
	static inline void quaternion2Euler(const quat& q, float res[], RotSeq rotSeq)
	{
		switch (rotSeq) {
		case zyx:
			threeaxisrot(2 * (q.x*q.y + q.w*q.z),
				q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
				-2 * (q.x*q.z - q.w*q.y),
				2 * (q.y*q.z + q.w*q.x),
				q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
				res);
			break;

		case zyz:
			twoaxisrot(2 * (q.y*q.z - q.w*q.x),
				2 * (q.x*q.z + q.w*q.y),
				q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
				2 * (q.y*q.z + q.w*q.x),
				-2 * (q.x*q.z - q.w*q.y),
				res);
			break;

		case zxy:
			threeaxisrot(-2 * (q.x*q.y - q.w*q.z),
				q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
				2 * (q.y*q.z + q.w*q.x),
				-2 * (q.x*q.z - q.w*q.y),
				q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
				res);
			break;

		case zxz:
			twoaxisrot(2 * (q.x*q.z + q.w*q.y),
				-2 * (q.y*q.z - q.w*q.x),
				q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
				2 * (q.x*q.z - q.w*q.y),
				2 * (q.y*q.z + q.w*q.x),
				res);
			break;

		case yxz:
			threeaxisrot(2 * (q.x*q.z + q.w*q.y),
				q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
				-2 * (q.y*q.z - q.w*q.x),
				2 * (q.x*q.y + q.w*q.z),
				q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
				res);
			break;

		case yxy:
			twoaxisrot(2 * (q.x*q.y - q.w*q.z),
				2 * (q.y*q.z + q.w*q.x),
				q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
				2 * (q.x*q.y + q.w*q.z),
				-2 * (q.y*q.z - q.w*q.x),
				res);
			break;

		case yzx:
			threeaxisrot(-2 * (q.x*q.z - q.w*q.y),
				q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
				2 * (q.x*q.y + q.w*q.z),
				-2 * (q.y*q.z - q.w*q.x),
				q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
				res);
			break;

		case yzy:
			twoaxisrot(2 * (q.y*q.z + q.w*q.x),
				-2 * (q.x*q.y - q.w*q.z),
				q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
				2 * (q.y*q.z - q.w*q.x),
				2 * (q.x*q.y + q.w*q.z),
				res);
			break;

		case xyz:
			threeaxisrot(-2 * (q.y*q.z - q.w*q.x),
				q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
				2 * (q.x*q.z + q.w*q.y),
				-2 * (q.x*q.y - q.w*q.z),
				q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
				res);
			break;

		case xyx:
			twoaxisrot(2 * (q.x*q.y + q.w*q.z),
				-2 * (q.x*q.z - q.w*q.y),
				q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
				2 * (q.x*q.y - q.w*q.z),
				2 * (q.x*q.z + q.w*q.y),
				res);
			break;

		case xzy:
			threeaxisrot(2 * (q.y*q.z + q.w*q.x),
				q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
				-2 * (q.x*q.y - q.w*q.z),
				2 * (q.x*q.z + q.w*q.y),
				q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
				res);
			break;

		case xzx:
			twoaxisrot(2 * (q.x*q.z - q.w*q.y),
				2 * (q.x*q.y + q.w*q.z),
				q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
				2 * (q.x*q.z + q.w*q.y),
				-2 * (q.x*q.y - q.w*q.z),
				res);
			break;
		default:

			//std::cout << "Unknown rotation sequence" << std::endl;
			break;
		}
	}

	//*/

	//all algorithms failed to extract the euler angles...

	void extractEuler(quat q, float *roll, float *pitch, float *yaw) {

		/*
		float x2 = q.x * q.x;
		float y2 = q.y * q.y;
		float z2 = q.z * q.z;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;
		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;

		float m_11 = 1.0f - 2.0f * (y2 + z2);
		//float m_12 = 2.0f * (xy - wz);
		//float m_13 = 2.0f * (xz + wy);

		float m_21 = 2.0f * (xy + wz);
		float m_22 = 1.0f - 2.0f * (x2 + z2);
		float m_23 = 2.0f * (yz - wx);

		float m_31 = 2.0f * (xz - wy);
		float m_32 = 2.0f * (yz + wx);
		float m_33 = 1.0f - 2.0f * (x2 + y2);

		//
		// https://www.learnopencv.com/rotation-matrix-to-euler-angles/
		//
		float sy = sqrt(m_11 * m_11 + m_21 * m_21);

		bool singular = sy < 1e-6; // If

		float x, y, z;
		if (!singular)
		{
			x = atan2(m_32, m_33);
			y = atan2(-m_31, sy);
			z = atan2(m_21, m_11);
		}
		else
		{
			x = atan2(-m_23, m_22);
			y = atan2(-m_31, sy);
			z = 0;
		}

		*roll = x;
		*pitch = y;
		*yaw = z;
		*/


		// can't find an algorithm that works... so use the mat4 to get the euler angles...
		extractEuler(toMat4(q), roll, pitch, yaw);

		/*
		// http://bediyap.com/programming/convert-quaternion-to-euler-rotations/ - Doesn't work
		float res[3];
		quaternion2Euler(q, res, zxy);
		
		*roll = res[1];
		*pitch = res[0];
		*yaw = res[2];

		*/
		
		/*
		//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles -- Doesn't work

		// roll (x-axis rotation)
		float sinr_cosp = +2.0f * (q.w * q.x + q.y * q.z);
		float cosr_cosp = +1.0f - 2.0f * (q.x * q.x + q.y * q.y);
		*roll = atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		float sinp = +2.0 * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
			*pitch = copysign( PI / 2.0f, sinp); // use 90 degrees if out of range
		else
			*pitch = asin(sinp);

		// yaw (z-axis rotation)
		float siny_cosp = +2.0f * (q.w * q.z + q.x * q.y);
		float cosy_cosp = +1.0f - 2.0f * (q.y * q.y + q.z * q.z);
		*yaw = atan2(siny_cosp, cosy_cosp);

		//*/

		
		/*
		//https://www.gamedev.net/forums/topic/166424-quaternion-to-euler/ -- Doesn't work
		float sqw;
		float sqx;
		float sqy;
		float sqz;

		sqw = q.w * q.w;
		sqx = q.x * q.x;
		sqy = q.y * q.y;
		sqz = q.z * q.z;

		*roll = (float)atan2l(2.0f * (q.y * q.z + q.x * q.w), (-sqx - sqy + sqz + sqw));
		*pitch = (float)asinl(-2.0f * (q.x * q.z - q.y * q.w));
		*yaw = (float)atan2l(2.0f * (q.x * q.y + q.z * q.w), (sqx - sqy - sqz + sqw));

		//*/

		/*

		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm -- Doesn't work

		double sqw = q.w*q.w;
		double sqx = q.x*q.x;
		double sqy = q.y*q.y;
		double sqz = q.z*q.z;
		double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
		double test = q.x*q.y + q.z*q.w;

		
		if (test > 0.499*unit) { // singularity at north pole
			*pitch = 2.0 * atan2(q.x, q.w);
			*yaw = PI / 2.0;
			*roll = 0;
			return;
		}
		if (test < -0.499*unit) { // singularity at south pole
			*pitch = -2.0 * atan2(q.x, q.w);
			*yaw = -PI / 2.0;
			*roll = 0;
			return;
		}

		*pitch = atan2(2.0 * q.y*q.w - 2.0 * q.x*q.z, sqx - sqy - sqz + sqw);
		*yaw = asin(2.0 * test / unit);
		*roll = atan2(2.0 * q.x*q.w - 2.0 * q.y*q.z, -sqx + sqy - sqz + sqw);

		//*/

	}
	//------------------------------------------------------------------------------
	quat inv(const quat &q) {
		return conjugate(q);
		/*
		quat result;
		const float TOLERANCE = 0.001f;
		// Don't normalize if we don't have to
		float mag2 = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
		if (fabs(mag2) > TOLERANCE && fabs(mag2 - 1.0f) > TOLERANCE) {
			float invmag2 = 1.0f / mag2;
			result = quat(-q.x*invmag2, -q.y*invmag2, -q.z*invmag2, -q.w*invmag2);
		}
		else {
			result = quat(-q.x, -q.y, -q.z, -q.w);
		}
		return result;
		*/
	}
	//------------------------------------------------------------------------------
	float dot(const quat& a, const quat& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	float angleBetween(const quat& a, const quat& b) {
		return acos(dot(normalize(a), normalize(b))) * 2.0f;
		//return acos( dot(normalize(vec3(a.x, a.y, a.z)), normalize(vec3(b.x, b.y, b.z))) );
	}
}
