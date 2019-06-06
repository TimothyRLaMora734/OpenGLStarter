#ifndef mat4_h
#define mat4_h

#include <aribeiro/common.h>
#include <aribeiro/vec4.h>
#include <aribeiro/SSE2.h>
#include <aribeiro/floatOPs.h>

namespace aRibeiro {


//class tensor4;
//class vec4;
    
#if defined(ARIBEIRO_SSE2)
    
    //const __m128 _vec3_zero_sse = _mm_set1_ps(0.0f);
    //const __m128 _vec3_sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31
    
#pragma pack(push, 16)
#endif

/// \brief Matrix with 4x4 components
///
/// Matrix definition to work with rigid transformations
/// \author Alessandro Ribeiro
///
_SSE2_ALIGN_PRE class ARIBEIRO_API mat4{
    public:

	_SSE2_ALIGN_PRE union {
		_SSE2_ALIGN_PRE struct{
            float _11,_21,_31,_41,
                  _12,_22,_32,_42,
                  _13,_23,_33,_43,
                  _14,_24,_34,_44;
        }_SSE2_ALIGN_POS;
		_SSE2_ALIGN_PRE struct {
			float a1, a2, a3, a4,
				  b1, b2, b3, b4,
				  c1, c2, c3, c4,
				  d1, d2, d3, d4;
		}_SSE2_ALIGN_POS;
		_SSE2_ALIGN_PRE float array[16]_SSE2_ALIGN_POS;
        // column-major (OpenGL like matrix byte order)
        //  x  y  z  w
        //  0  4  8 12
        //  1  5  9 13
        //  2  6 10 14
        //  3  7 11 15
#if defined(ARIBEIRO_SSE2)
		_SSE2_ALIGN_PRE __m128 array_sse[4] _SSE2_ALIGN_POS;
#endif
    }_SSE2_ALIGN_POS;
    
#if defined(ARIBEIRO_SSE2)
    //special SSE2 constructor
    ARIBEIRO_INLINE mat4( const __m128 &a, const __m128 &b, const __m128 &c, const __m128 &d  ){
        array_sse[0] = a;
        array_sse[1] = b;
        array_sse[2] = c;
        array_sse[3] = d;
    }
#endif
    
    //---------------------------------------------------------------------------
    /// \brief Constructs a ZERO matrix 4x4
    ///
    /// The ZERO mat4 class have all values to 0
    /// \author Alessandro Ribeiro
    ///
    ARIBEIRO_INLINE mat4(){
#if defined(ARIBEIRO_SSE2)
        array_sse[0] = _vec4_zero_sse;
        array_sse[1] = _vec4_zero_sse;
        array_sse[2] = _vec4_zero_sse;
        array_sse[3] = _vec4_zero_sse;
#else
        _11=_12=_13=_14=_21=_22=_23=_24=
        _31=_32=_33=_34=_41=_42=_43=_44= 0;
#endif
        
    }
    //---------------------------------------------------------------------------
    /// \brief Constructs a 4x4 matrix
    ///
    /// Initialize all components of the matrix with the same value
    ///
    /// \author Alessandro Ribeiro
    /// \param value Value to initialize the components
    ///
    ARIBEIRO_INLINE mat4(const float &value){
#if defined(ARIBEIRO_SSE2)
        array_sse[0] = _mm_set1_ps(value);
        array_sse[1] = array_sse[0];
        array_sse[2] = array_sse[0];
        array_sse[3] = array_sse[0];
#else
        _11=_12=_13=_14=_21=_22=_23=_24=
        _31=_32=_33=_34=_41=_42=_43=_44= value;
#endif
    }
    //---------------------------------------------------------------------------
    /// \brief Constructs a 4x4 matrix
    ///
    /// Initialize the mat4 components from the parameters
    ///
    /// \author Alessandro Ribeiro
    ///
    ARIBEIRO_INLINE mat4(
         const float &a1,const float &b1,const float &c1,const float &d1,
         const float &a2,const float &b2,const float &c2,const float &d2,
         const float &a3,const float &b3,const float &c3,const float &d3,
         const float &a4,const float &b4,const float &c4,const float &d4){
#if defined(ARIBEIRO_SSE2)
        array_sse[0] = _mm_load_(a1, a2, a3, a4);//_mm_set_ps(a4, a1, a2, a3);
        array_sse[1] = _mm_load_(b1, b2, b3, b4);
        array_sse[2] = _mm_load_(c1, c2, c3, c4);
        array_sse[3] = _mm_load_(d1, d2, d3, d4);
#else
        _11=a1;_12=b1;_13=c1;_14=d1;
        _21=a2;_22=b2;_23=c2;_24=d2;
        _31=a3;_32=b3;_33=c3;_34=d3;
        _41=a4;_42=b4;_43=c4;_44=d4;
#endif
    }
    //---------------------------------------------------------------------------
    /// \brief Constructs a 4x4 matrix
    ///
    /// Initialize the mat4 components from other mat4 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param m Matrix to assign to the instance
    ///
    ARIBEIRO_INLINE mat4(const mat4 &m){
#if defined(ARIBEIRO_SSE2)
        array_sse[0] = m.array_sse[0];
        array_sse[1] = m.array_sse[1];
        array_sse[2] = m.array_sse[2];
        array_sse[3] = m.array_sse[3];
#else
        *this = m;
#endif
        //memcpy(array,m.array,sizeof(float)*4*4);
    }
    //---------------------------------------------------------------------------
    /// \brief Matrix multiplication
    ///
    /// Makes the full 4x4 matrix multiplication
    ///
    /// \author Alessandro Ribeiro
    /// \param M the matrix to be multiplied by the current instance
    /// \return A reference to the multiplied matrix current instance
    ///
    ARIBEIRO_INLINE mat4& operator*=(const mat4 &M){
        
#if defined(ARIBEIRO_SSE2)
        __m128 array_sse_result[4];
        {
            __m128 e0 = _mm_shuffle_ps(M.array_sse[0], M.array_sse[0], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 e1 = _mm_shuffle_ps(M.array_sse[0], M.array_sse[0], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 e2 = _mm_shuffle_ps(M.array_sse[0], M.array_sse[0], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 e3 = _mm_shuffle_ps(M.array_sse[0], M.array_sse[0], _MM_SHUFFLE(3, 3, 3, 3));
            
            __m128 m0 = _mm_mul_ps(array_sse[0], e0);
            __m128 m1 = _mm_mul_ps(array_sse[1], e1);
            __m128 m2 = _mm_mul_ps(array_sse[2], e2);
            __m128 m3 = _mm_mul_ps(array_sse[3], e3);
            
            __m128 a0 = _mm_add_ps(m0, m1);
            __m128 a1 = _mm_add_ps(m2, m3);
            __m128 a2 = _mm_add_ps(a0, a1);
            
            array_sse_result[0] = a2;
        }
        
        {
            __m128 e0 = _mm_shuffle_ps(M.array_sse[1], M.array_sse[1], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 e1 = _mm_shuffle_ps(M.array_sse[1], M.array_sse[1], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 e2 = _mm_shuffle_ps(M.array_sse[1], M.array_sse[1], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 e3 = _mm_shuffle_ps(M.array_sse[1], M.array_sse[1], _MM_SHUFFLE(3, 3, 3, 3));
            
            __m128 m0 = _mm_mul_ps(array_sse[0], e0);
            __m128 m1 = _mm_mul_ps(array_sse[1], e1);
            __m128 m2 = _mm_mul_ps(array_sse[2], e2);
            __m128 m3 = _mm_mul_ps(array_sse[3], e3);
            
            __m128 a0 = _mm_add_ps(m0, m1);
            __m128 a1 = _mm_add_ps(m2, m3);
            __m128 a2 = _mm_add_ps(a0, a1);
            
            array_sse_result[1] = a2;
        }
        
        {
            __m128 e0 = _mm_shuffle_ps(M.array_sse[2], M.array_sse[2], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 e1 = _mm_shuffle_ps(M.array_sse[2], M.array_sse[2], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 e2 = _mm_shuffle_ps(M.array_sse[2], M.array_sse[2], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 e3 = _mm_shuffle_ps(M.array_sse[2], M.array_sse[2], _MM_SHUFFLE(3, 3, 3, 3));
            
            __m128 m0 = _mm_mul_ps(array_sse[0], e0);
            __m128 m1 = _mm_mul_ps(array_sse[1], e1);
            __m128 m2 = _mm_mul_ps(array_sse[2], e2);
            __m128 m3 = _mm_mul_ps(array_sse[3], e3);
            
            __m128 a0 = _mm_add_ps(m0, m1);
            __m128 a1 = _mm_add_ps(m2, m3);
            __m128 a2 = _mm_add_ps(a0, a1);
            
            array_sse_result[2] = a2;
        }
        
        {
            //(__m128&)_mm_shuffle_epi32(__m128i&)in2[0], _MM_SHUFFLE(3, 3, 3, 3))
            __m128 e0 = _mm_shuffle_ps(M.array_sse[3], M.array_sse[3], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 e1 = _mm_shuffle_ps(M.array_sse[3], M.array_sse[3], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 e2 = _mm_shuffle_ps(M.array_sse[3], M.array_sse[3], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 e3 = _mm_shuffle_ps(M.array_sse[3], M.array_sse[3], _MM_SHUFFLE(3, 3, 3, 3));
            
            __m128 m0 = _mm_mul_ps(array_sse[0], e0);
            __m128 m1 = _mm_mul_ps(array_sse[1], e1);
            __m128 m2 = _mm_mul_ps(array_sse[2], e2);
            __m128 m3 = _mm_mul_ps(array_sse[3], e3);
            
            __m128 a0 = _mm_add_ps(m0, m1);
            __m128 a1 = _mm_add_ps(m2, m3);
            __m128 a2 = _mm_add_ps(a0, a1);
            
            array_sse_result[3] = a2;
        }
        
        array_sse[0] = array_sse_result[0];
        array_sse[1] = array_sse_result[1];
        array_sse[2] = array_sse_result[2];
        array_sse[3] = array_sse_result[3];

#else
        
        float a,b,c,d;
        a = _11;b = _12;c = _13;d = _14;
        _11 = (a*M._11+b*M._21+c*M._31+d*M._41);
        _12 = (a*M._12+b*M._22+c*M._32+d*M._42);
        _13 = (a*M._13+b*M._23+c*M._33+d*M._43);
        _14 = (a*M._14+b*M._24+c*M._34+d*M._44);
        
        a = _21;b = _22;c = _23;d = _24;
        _21 = (a*M._11+b*M._21+c*M._31+d*M._41);
        _22 = (a*M._12+b*M._22+c*M._32+d*M._42);
        _23 = (a*M._13+b*M._23+c*M._33+d*M._43);
        _24 = (a*M._14+b*M._24+c*M._34+d*M._44);
        
        a = _31;b = _32;c = _33;d = _34;
        _31 = (a*M._11+b*M._21+c*M._31+d*M._41);
        _32 = (a*M._12+b*M._22+c*M._32+d*M._42);
        _33 = (a*M._13+b*M._23+c*M._33+d*M._43);
        _34 = (a*M._14+b*M._24+c*M._34+d*M._44);
        
        a = _41;b = _42;c = _43;d = _44;
        _41 = (a*M._11+b*M._21+c*M._31+d*M._41);
        _42 = (a*M._12+b*M._22+c*M._32+d*M._42);
        _43 = (a*M._13+b*M._23+c*M._33+d*M._43);
        _44 = (a*M._14+b*M._24+c*M._34+d*M._44);
#endif
        return *this;
    }
    //---------------------------------------------------------------------------
    /// \brief Matrix multiplication
    ///
    /// Makes the full 4x4 matrix multiplication, by coping the multiplication result.
    ///
    /// \author Alessandro Ribeiro
    /// \param M the matrix to be multiplied by the current instance
    /// \return A copy to the multiplied matrix
    ///
    //ARIBEIRO_INLINE mat4 operator*(const mat4 &M) const {
        //return (mat4(*this)*=M);
    //}
    //---------------------------------------------------------------------------
    /// \brief Matrix access based on X (col) and Y (row)
    ///
    /// \author Alessandro Ribeiro
    /// \param x The column to get the element at index
    /// \param y The row to get the element at index
    /// \return A reference to the matrix element
    ///
    ARIBEIRO_INLINE float& operator()(const int x,const int y){
        return array[y*4+x];
    }
    //---------------------------------------------------------------------------
    /// \brief Matrix row access based
    ///
    /// Acess one of the 4 rows of the matrix as a vec4 type
    ///
    /// \author Alessandro Ribeiro
    /// \param x The row to get
    /// \return A reference to the matrix row as vec4
    ///
    ARIBEIRO_INLINE vec4& operator[](const int x){
        return *((vec4*)&array[x*4]);
    }
    ARIBEIRO_INLINE vec4 operator[](const int v)const{
        return *((vec4*)&array[v * 4]);
    }
    //---------------------------------------------------------------------------
    /// \brief Use the current byte raw data as tensor4 type
    ///
    /// \author Alessandro Ribeiro
    /// \return A reference to a tensor4
    ///
    //tensor4& asTensor4()const;

    ARIBEIRO_INLINE bool operator==(const mat4&v) const{
        
#if defined(ARIBEIRO_SSE2)
        
        __m128 diff_abs[4];
        diff_abs[0] = _mm_sub_ps(array_sse[0], v.array_sse[0]);
        diff_abs[0] = _mm_andnot_ps(_vec4_sign_mask, diff_abs[0]); //abs
        
        diff_abs[1] = _mm_sub_ps(array_sse[1], v.array_sse[1]);
        diff_abs[1] = _mm_andnot_ps(_vec4_sign_mask, diff_abs[1]); //abs
        
        diff_abs[2] = _mm_sub_ps(array_sse[2], v.array_sse[2]);
        diff_abs[2] = _mm_andnot_ps(_vec4_sign_mask, diff_abs[2]); //abs
        
        diff_abs[3] = _mm_sub_ps(array_sse[3], v.array_sse[3]);
        diff_abs[3] = _mm_andnot_ps(_vec4_sign_mask, diff_abs[3]); //abs
        
        //static const __m128 epsilon = _mm_set1_ps(1e-4f); // -0.f = 1 << 31
        //_mm_shuffle_ps(mul0, mul0, _MM_SHUFFLE(2, 3, 0, 1));
        for(int j=0;j<4;j++){
            for(int i=0;i<4;i++){
                if (_mm_f32_(diff_abs[j],i) > 1e-4f)
                    return false;
            }
        }
        
        return true;
        
#else
  
        for(int i=0;i<16;i++){
            if (absv(array[i]-v.array[i]) > 1e-4f)
                return false;
        }
        return true;
        //return memcmp(array, v.array, sizeof(float) * 16) == 0;
#endif
    }
    ARIBEIRO_INLINE bool operator!=(const mat4&v) const{
        return !((*this) == v);
        //return memcmp(array, v.array, sizeof(float) * 16) != 0;
    }
    
    ARIBEIRO_INLINE mat4& operator+=(const mat4& v){
#if defined(ARIBEIRO_SSE2)
        array_sse[0] = _mm_add_ps(array_sse[0], v.array_sse[0]);
        array_sse[1] = _mm_add_ps(array_sse[1], v.array_sse[1]);
        array_sse[2] = _mm_add_ps(array_sse[2], v.array_sse[2]);
        array_sse[3] = _mm_add_ps(array_sse[3], v.array_sse[3]);
#else
        _11+=v._11;_12+=v._12;_13+=v._13;_14+=v._14;
        _21+=v._21;_22+=v._22;_23+=v._23;_24+=v._24;
        _31+=v._31;_32+=v._32;_33+=v._33;_34+=v._34;
        _41+=v._41;_42+=v._42;_43+=v._43;_44+=v._44;
#endif
        return *this;
    }
    ARIBEIRO_INLINE mat4& operator-=(const mat4& v){
#if defined(ARIBEIRO_SSE2)
        array_sse[0] = _mm_sub_ps(array_sse[0], v.array_sse[0]);
        array_sse[1] = _mm_sub_ps(array_sse[1], v.array_sse[1]);
        array_sse[2] = _mm_sub_ps(array_sse[2], v.array_sse[2]);
        array_sse[3] = _mm_sub_ps(array_sse[3], v.array_sse[3]);
#else
        _11-=v._11;_12-=v._12;_13-=v._13;_14-=v._14;
        _21-=v._21;_22-=v._22;_23-=v._23;_24-=v._24;
        _31-=v._31;_32-=v._32;_33-=v._33;_34-=v._34;
        _41-=v._41;_42-=v._42;_43-=v._43;_44-=v._44;
#endif
        return *this;
    }
    ARIBEIRO_INLINE mat4 operator-() const{
#if defined(ARIBEIRO_SSE2)
        mat4 result;
        
        result.array_sse[0] = _mm_xor_ps(_vec4_sign_mask, array_sse[0]);
        result.array_sse[1] = _mm_xor_ps(_vec4_sign_mask, array_sse[1]);
        result.array_sse[2] = _mm_xor_ps(_vec4_sign_mask, array_sse[2]);
        result.array_sse[3] = _mm_xor_ps(_vec4_sign_mask, array_sse[3]);
        
        return result;
#else
        return mat4(-_11,-_21,-_31,-_41,
                    -_12,-_22,-_32,-_42,
                    -_13,-_23,-_33,-_43,
                    -_14,-_24,-_34,-_44);
#endif
    }
    /*
    ARIBEIRO_INLINE mat4& operator*=(const mat4& v){
        _11*=v._11;_12*=v._12;_13*=v._13;_14*=v._14;
        _21*=v._21;_22*=v._22;_23*=v._23;_24*=v._24;
        _31*=v._31;_32*=v._32;_33*=v._33;_34*=v._34;
        _41*=v._41;_42*=v._42;_43*=v._43;_44*=v._44;
        return *this;
    }
    */
    
    ARIBEIRO_INLINE mat4& operator/=(const mat4& v){
        mat4 operant(
          1.0f/v.a1,1.0f/v.b1,1.0f/v.c1,1.0f/v.d1,
          1.0f/v.a2,1.0f/v.b2,1.0f/v.c2,1.0f/v.d2,
          1.0f/v.a3,1.0f/v.b3,1.0f/v.c3,1.0f/v.d3,
          1.0f/v.a4,1.0f/v.b4,1.0f/v.c4,1.0f/v.d4
        );
        (*this) *= operant;
        return *this;
    }
    
    ARIBEIRO_INLINE mat4& operator+=(const float &v){
#if defined(ARIBEIRO_SSE2)
        __m128 tmp = _mm_set1_ps(v);
        
        array_sse[0] = _mm_add_ps(array_sse[0], tmp);
        array_sse[1] = _mm_add_ps(array_sse[1], tmp);
        array_sse[2] = _mm_add_ps(array_sse[2], tmp);
        array_sse[3] = _mm_add_ps(array_sse[3], tmp);
        
#else
        _11+=v;_12+=v;_13+=v;_14+=v;
        _21+=v;_22+=v;_23+=v;_24+=v;
        _31+=v;_32+=v;_33+=v;_34+=v;
        _41+=v;_42+=v;_43+=v;_44+=v;
#endif
        return *this;
    }
    ARIBEIRO_INLINE mat4& operator-=(const float &v){
#if defined(ARIBEIRO_SSE2)
        __m128 tmp = _mm_set1_ps(v);
        
        array_sse[0] = _mm_sub_ps(array_sse[0], tmp);
        array_sse[1] = _mm_sub_ps(array_sse[1], tmp);
        array_sse[2] = _mm_sub_ps(array_sse[2], tmp);
        array_sse[3] = _mm_sub_ps(array_sse[3], tmp);
        
#else
        _11-=v;_12-=v;_13-=v;_14-=v;
        _21-=v;_22-=v;_23-=v;_24-=v;
        _31-=v;_32-=v;_33-=v;_34-=v;
        _41-=v;_42-=v;_43-=v;_44-=v;
#endif
        return *this;
    }
    ARIBEIRO_INLINE mat4& operator*=(const float &v){
#if defined(ARIBEIRO_SSE2)
        __m128 tmp = _mm_set1_ps(v);
        
        array_sse[0] = _mm_mul_ps(array_sse[0], tmp);
        array_sse[1] = _mm_mul_ps(array_sse[1], tmp);
        array_sse[2] = _mm_mul_ps(array_sse[2], tmp);
        array_sse[3] = _mm_mul_ps(array_sse[3], tmp);
        
#else
        _11*=v;_12*=v;_13*=v;_14*=v;
        _21*=v;_22*=v;_23*=v;_24*=v;
        _31*=v;_32*=v;_33*=v;_34*=v;
        _41*=v;_42*=v;_43*=v;_44*=v;
#endif
        return *this;
    }
    ARIBEIRO_INLINE mat4& operator/=(const float &v){
#if defined(ARIBEIRO_SSE2)
        __m128 tmp = _mm_set1_ps(v);
        
        array_sse[0] = _mm_div_ps(array_sse[0], tmp);
        array_sse[1] = _mm_div_ps(array_sse[1], tmp);
        array_sse[2] = _mm_div_ps(array_sse[2], tmp);
        array_sse[3] = _mm_div_ps(array_sse[3], tmp);
        
#else
        _11/=v;_12/=v;_13/=v;_14/=v;
        _21/=v;_22/=v;_23/=v;_24/=v;
        _31/=v;_32/=v;_33/=v;_34/=v;
        _41/=v;_42/=v;_43/=v;_44/=v;
#endif
        return *this;
    }

	SSE2_CLASS_NEW_OPERATOR

    static const mat4 IdentityMatrix;///<An static Identity matrix
};

INLINE_OPERATION_IMPLEMENTATION(mat4)
    
#if defined(ARIBEIRO_SSE2)
#pragma pack(pop)
#endif

}


#endif
