/*****************************************************************************\
*                                                                           *
* File(s): TVector.hpp                                                      *
*                                                                           *
* Content: Template class for a mathematical vector of fixed dimension.     *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
* Author(s): Tom Uhlmann                                                    *
*                                                                           *
*                                                                           *
* The file(s) mentioned above are provided as is under the terms of the     *
* FreeBSD License without any warranty or guaranty to work properly.        *
* For additional license, copyright and contact/support issues see the      *
* supplied documentation.                                                   *
*                                                                           *
\****************************************************************************/

#ifndef ARDUFORGE_TVECTOR_H
#define ARDUFORGE_TVECTOR_H 

#include <inttypes.h>

namespace ArduForge{

    /**
     * \brief Template class for a mathematical vector of fixed dimension.
     * 
     */
    template<typename T, int16_t Dim>
    class TVector{
    public:

        /**
         * \brief Compute the dot product of two vectors.
         * \param[in] Left First operand.
         * \param[in] Right Second operand.
         * \return Dot product value.
         */
        static T dot(const TVector<T, Dim> Left, const TVector<T, Dim> Right){
            T Rval = (T)0;
            for(uint16_t i=0; i < Dim; ++i) 
                Rval += Left[i]*Right[i];
            return Rval;
        }//dot 

        /**
         * \brief Constructor
         * Initializes data with 0.
         */
        TVector(void){
            memset(m_Data, 0, sizeof(T)*Dim);
        }//Constructor

        /**
         * \brief Destructor 
         */
        ~TVector(void){
            clear();
        }//Destructor

        /**
         * \brief Resets the data to 0.
         */
        void clear(void){
            memset(m_Data, 0, sizeof(T)*Dim);
        }//clear

        /**
         * \brief Assign operator.
         * \param[in] Ref Vector to assign.
         */
        void operator=(const TVector<T, Dim> &Ref){
            if(this == &Ref) return;
            for(uint16_t i=0; i < Dim; ++i) m_Data[i] = Ref[i];
        }//operator=

        /**
         * \brief Assign and cast operator.
         * \param[in] Ref Vector to assign.
         */
        template<typename U>
        void operator=(const TVector<U, Dim> &Ref){
            for(uint16_t i=0; i < Dim; ++i) m_Data[i] = (T)Ref[i];
        }//operator= 

        /**
         * \brief Initialization method.
         * \param[in] DefaultVal Default value for all elements.
         */
        void init(const T DefaultVal = (T)0){
            for(uint16_t i=0; i < Dim; ++i) m_Data[i] = DefaultVal;
        }//initialize 

        /**
         * \brief Initialization method.
         * \param[in] Ref Values to assign to this vector.
         */
        void init(const TVector<T, Dim> Ref){
            for(uint16_t i=0; i< Dim; ++i) m_Data[i] = Ref[i];
        }//initialize

        /**
         * \brief Initialization method.
         * \param[in] Ref Values to assign from vector of different type. Individual values will be casted.
         */
        template<typename U>
        void init(const TVector<U, Dim> Ref){
            for(uint16_t i=0; i < Dim; ++i) m_Data[i] = (T)Ref[i];
        }//initialize 

        /**
         * \brief Addition operator.
         * \param[in] Right Second operand.
         * \return Resulting vector of the addition.
         */
        TVector<T, Dim> operator+(const TVector<T, Dim> &Right)const{
            TVector<T, Dim> Rval;
            for(uint16_t i=0; i < Dim; ++i) Rval[i] = m_Data[i] + Right[i];
            return Rval;
        }//operator+

        /**
         * \brief Subtraction operator.
         * \param[in] Right Second operand.
         * \return Resulting vector of the subtraction.
         */
        TVector<T, Dim> operator-(const TVector<T, Dim> &Right)const{
            TVector<T, Dim> Rval;
            for(uint16_t i=0; i< Dim; ++i) Rval[i] = m_Data[i] - Right[i];
        }//operator+

        /**
         * \brief Addition-assign operator.
         * \param[in] Second operand.
         */
        void operator+=(const TVector<T, Dim> &Right){
            for(uint16_t i=0; i < Dim; ++i) m_Data[i] += Right[i];
        }//operator+=

        /**
         * \brief Subtraction-assign operator.
         * \param[in] Right Second operand.
         */
        void operator-=(const TVector<T, Dim> &Right){
            for(uint16_t i=0; i < Dim; ++i) m_Data[i] -= Right[i];
        }//operator-=

        /**
         * \brief Returns dimension of the vector.
         * \return Dimension of the vector.
         */
        int16_t dimension(void)const{
            return Dim;
        }//dimension

        /**
         * \brief Computes length of the vector. \f$ l = \sqrt(\sum_{i=1}^{Dim} v_i) \f$
         * \return Length of the vector.
         **/
        template<typename U>
        U length(void)const{
            return (U)sqrt(dot(*this, *this));
        }//length

        /**
         * \brief Access operator.
         * \return Reference to requested element.
         * \param[in] Index Index of the element. Must be \f$ \in [0, Dim) \f$
         * \note Index is not checked for validity.
         */
        T& operator[](uint16_t Index){
            return m_Data[Index];
        }//operator[]

        /**
         * \brief Access operator.
         * \param[in] Index Index of the element. Must be \f$ \in [0, Dim) \f$
         * \return Element at requested index.
         * \note Index is not checked for validity.
         */
        T operator[](uint16_t Index)const{
            return m_Data[Index];
        }//operator[]

    private:
        T m_Data[Dim];  ///< Data buffer.
    };//TVector

}//name space

#endif 