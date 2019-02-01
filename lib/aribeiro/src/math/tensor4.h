#ifndef tensor4_h
#define tensor4_h

#include <aribeiro/common.h>

namespace aRibeiro {

class mat4;

/// \brief Tensor with 4x4 components
///
/// It can be used to operate over matrixes as single values with component-wise aritmethic operations.
/// \author Alessandro Ribeiro
///
class ARIBEIRO_API tensor4{
    public:
    union {
        struct{
            float _11,_21,_31,_41,
                  _12,_22,_32,_42,
                  _13,_23,_33,_43,
                  _14,_24,_34,_44;
        };
        float array[16];
        // column-major
        //  x  y  z  w
        //  0  4  8 12
        //  1  5  9 13
        //  2  6 10 14
        //  3  7 11 15
    };
    /// \brief Constructs a ZERO tensor 4x4
    ///
    /// The ZERO tensor4 class have all values to 0
    /// \author Alessandro Ribeiro
    ///
    tensor4();
    /// \brief Constructs a 4x4 tensor
    ///
    /// Initialize all components of the tensor with the same value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to initialize the components
    ///
    tensor4( const float v );
    /// \brief Constructs a 4x4 tensor
    ///
    /// Initialize the tensor4 components from the parameters
    ///
    /// \author Alessandro Ribeiro
    ///
    tensor4(const float __11,const float __12,const float __13,const float __14,
            const float __21,const float __22,const float __23,const float __24,
            const float __31,const float __32,const float __33,const float __34,
            const float __41,const float __42,const float __43,const float __44);
    //---------------------------------------------------------------------------
    /// \brief Constructs a 4x4 tensor
    ///
    /// Initialize the tensor4 components from other tensor4 instance
    ///
    /// \author Alessandro Ribeiro
    /// \param m Tensor to assign to the instance
    ///
    tensor4(const tensor4 &m);
    //---------------------------------------------------------------------------
    /// \brief Component-wise increment operator overload
    ///
    /// Increment the tensor by the components of another tensor
    ///
    /// \author Alessandro Ribeiro
    /// \param v Tensor to increment the current tensor instance
    /// \return A reference to the current instance after the increment
    ///
    tensor4& operator+=(const tensor4& v);

    /// \brief Component-wise decrement operator overload
    ///
    /// Decrement the tensor by the components of another tensor
    ///
    /// \author Alessandro Ribeiro
    /// \param v Tensor to decrement the current tensor instance
    /// \return A reference to the current instance after the decrement
    ///
    tensor4& operator-=(const tensor4& v);

    /// \brief Component-wise minus operator overload
    ///
    /// Negates the tensor components with the operator minus
    ///
    /// \author Alessandro Ribeiro
    /// \return A copy of the current instance after the negation operation
    ///
    tensor4 operator-();

    /// \brief Component-wise multiply operator overload
    ///
    /// Multiply the tensor by the components of another tensor
    ///
    /// \author Alessandro Ribeiro
    /// \param v Tensor to multiply the current tensor instance
    /// \return A reference to the current instance after the multiplication
    ///
    tensor4& operator*=(const tensor4& v);

    /// \brief Component-wise division operator overload
    ///
    /// Divides the tensor by the components of another tensor
    ///
    /// \author Alessandro Ribeiro
    /// \param v Tensor to divide the current tensor instance
    /// \return A reference to the current instance after the division
    ///
    tensor4& operator/=(const tensor4& v);

    /// \brief Single value increment operator overload
    ///
    /// Increment the tensor components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to increment all components of the current tensor instance
    /// \return A reference to the current instance after the increment
    ///
    tensor4& operator+=(const float v);

    /// \brief Single value decrement operator overload
    ///
    /// Decrement the tensor components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current tensor instance
    /// \return A reference to the current instance after the decrement
    ///
    tensor4& operator-=(const float v);

    /// \brief Single value multiply operator overload
    ///
    /// Decrement the tensor components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to decrement all components of the current tensor instance
    /// \return A reference to the current instance after the decrement
    ///
    tensor4& operator*=(const float v);

    /// \brief Single value division operator overload
    ///
    /// Divides the tensor components by a single value
    ///
    /// \author Alessandro Ribeiro
    /// \param v Value to divide all components of the current tensor instance
    /// \return A reference to the current instance after the division
    ///
    tensor4& operator/=(const float v);
    //---------------------------------------------------------------------------
    /// \brief Use the current byte raw data as mat4 type
    ///
    /// \author Alessandro Ribeiro
    /// \return A reference to a mat4
    ///
    mat4& asMat4()const;

	bool operator==(const tensor4&v) const;
	bool operator!=(const tensor4&v) const;
};
//------------------------------------------------------------------------------

}

#endif
