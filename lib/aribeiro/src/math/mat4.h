#ifndef mat4_h
#define mat4_h

#include <aribeiro/common.h>

namespace aRibeiro {

class tensor4;
class vec4;

/// \brief Matrix with 4x4 components
///
/// Matrix definition to work with rigid transformations
/// \author Alessandro Ribeiro
///
class ARIBEIRO_API mat4{
    public:

    union {
        struct{
            float _11,_21,_31,_41,
                  _12,_22,_32,_42,
                  _13,_23,_33,_43,
                  _14,_24,_34,_44;
        };
		struct {
			float a1, a2, a3, a4,
				  b1, b2, b3, b4,
				  c1, c2, c3, c4,
				  d1, d2, d3, d4;
		};
        float array[16];
        // column-major (OpenGL like matrix byte order)
        //  x  y  z  w
        //  0  4  8 12
        //  1  5  9 13
        //  2  6 10 14
        //  3  7 11 15
    };
    //---------------------------------------------------------------------------
    /// \brief Constructs a ZERO matrix 4x4
    ///
    /// The ZERO mat4 class have all values to 0
    /// \author Alessandro Ribeiro
    ///
    mat4();
    //---------------------------------------------------------------------------
    /// \brief Constructs a 4x4 matrix
    ///
    /// Initialize all components of the matrix with the same value
    ///
    /// \author Alessandro Ribeiro
    /// \param value Value to initialize the components
    ///
    mat4(const float value);
    //---------------------------------------------------------------------------
    /// \brief Constructs a 4x4 matrix
    ///
    /// Initialize the mat4 components from the parameters
    ///
    /// \author Alessandro Ribeiro
    ///
    mat4(const float a1,const float b1,const float c1,const float d1,
         const float a2,const float b2,const float c2,const float d2,
         const float a3,const float b3,const float c3,const float d3,
         const float a4,const float b4,const float c4,const float d4);
    //---------------------------------------------------------------------------
    /// \brief Constructs a 4x4 matrix
    ///
    /// Initialize the mat4 components from other mat4 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param m Matrix to assign to the instance
    ///
    mat4(const mat4 &m);
    //---------------------------------------------------------------------------
    /// \brief Matrix multiplication
    ///
    /// Makes the full 4x4 matrix multiplication
    ///
    /// \author Alessandro Ribeiro
    /// \param M the matrix to be multiplied by the current instance
    /// \return A reference to the multiplied matrix current instance
    ///
    mat4& operator*=(const mat4 &M);
    //---------------------------------------------------------------------------
    /// \brief Matrix multiplication
    ///
    /// Makes the full 4x4 matrix multiplication, by coping the multiplication result.
    ///
    /// \author Alessandro Ribeiro
    /// \param M the matrix to be multiplied by the current instance
    /// \return A copy to the multiplied matrix
    ///
    mat4 operator*(const mat4 &M) const ;
    //---------------------------------------------------------------------------
    /// \brief Matrix access based on X (col) and Y (row)
    ///
    /// \author Alessandro Ribeiro
    /// \param x The column to get the element at index
    /// \param y The row to get the element at index
    /// \return A reference to the matrix element
    ///
    float& operator()(const int x,const int y);
    //---------------------------------------------------------------------------
    /// \brief Matrix row access based
    ///
    /// Acess one of the 4 rows of the matrix as a vec4 type
    ///
    /// \author Alessandro Ribeiro
    /// \param x The row to get
    /// \return A reference to the matrix row as vec4
    ///
    vec4& operator[](const int x);
	vec4 operator[](const int v)const;
    //---------------------------------------------------------------------------
    /// \brief Use the current byte raw data as tensor4 type
    ///
    /// \author Alessandro Ribeiro
    /// \return A reference to a tensor4
    ///
    tensor4& asTensor4()const;
    static const mat4 IdentityMatrix;///<An static Identity matrix
};

}


#endif
