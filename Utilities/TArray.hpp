/*****************************************************************************\
*                                                                           *
* File(s): TArray.hpp                                                       *
*                                                                           *
* Content: A simple dynamic array. This template can be used for            *
*          easy data storage of variable number of elements.                *
*                                                                           *
*                                                                           *
*                                                                           *
* Author(s): Tom Uhlmann                                                    *
*                                                                           *
*                                                                           *
* The file(s) mentioned above are provided as is under the terms of the     *
* MIT License without any warranty or guaranty to work properly.            *
* For additional license, copyright and contact/support issues see the      *
* supplied documentation.                                                   *
*                                                                           *
\****************************************************************************/

#ifndef ARDUFORGE_TARRAY_H
#define ARDUFORGE_TARRAY_H 

#include <Arduino.h>
#include <inttypes.h>

namespace ArduForge{

    /**
     * \brief A simple dynamic array. 
     * 
     * This template class can hold a variable number of elements.
     */
    template<typename T>
    class TArray{
    public:

        /**
         * \brief Constructor 
         * \param[in] InitialSize Reserve this number of elements.
         */
        TArray(uint16_t InitialSize = 0){
            m_pData = nullptr; 
            m_ElementCount = 0;
            if(InitialSize != 0) resize(InitialSize);
        }//Constructor

        /**
         * \brief Destructor
         */
        ~TArray(void){
            clear();
        }//Destructor

        /**
         * \brief Assign operator.
         * \param[in] Ref Array that will be copied.
         */
        void operator=(const TVector<T> &Ref){
            if(this == &Ref) return;
            clear();
            m_pData = new T[Ref.size()];
            for(uint16_t i=0; i< Ref.size(); ++i){
                m_pData[i] = Ref[i];
            }//for[all elements]
        }//operator=

        /**
         * \brief Initialization method.
         * \param[in] ElementCount Number of elements to reserve.
         */
        void init(uint16_t ElementCount){
            clear();
            resize(ElementCount);
        }//Initialize 

        /**
         * \brief Frees all reserved memory.
         */
        void clear(void){
            delete[] m_pData;
            m_pData = nullptr;
            m_ElementCount = 0;
        }//clear
        
        /**
         * \brief Returns number of elements the array can save.
         * \return Number of elements the array can save.
         */
        uint16_t size(void)const{
            return m_ElementCount;
        }//size 

        /**
         * \brief Resizes the array and keeps the elements if possible. 
         * \param[in] ElementCount New number of elements.
         * \note The old elements are kept. If the new size is smaller than the old one, all excessive elements will be discarded.
         */
        void resize(uint16_t ElementCount){
            T* pNewData = new T[ElementCount];
            for(uint16_t i=0; i < min(ElementCount, m_ElementCount); ++i) 
                pNewData[i] = m_pData[i];

            delete[] m_pData;
            m_pData = pNewData;
            m_ElementCount = ElementCount;
        }//resize

        /**
         * \brief Access operator.
         * \param[in] ElementIndex Array position. Must be valid.
         * \return Reference to the requested element.
         */
        T& at(uint16_t ElementIndex){
            if(ElementIndex >= m_ElementCount) resize(ElementIndex);
            return m_pData[ElementIndex];
        }//at 

        /**
         * \brief Access operator.
         * \param[in] ElementIndex Array position. Must be valid.
         * \return Element at the requested position.
         */
        T at(uint16_t ElementIndex)const{
            return m_pData[ElementIndex];
        }//at 

        /**
         * \brief Access operator.
         * \param[in] ElementIndex Array position. Must be valid.
         * \return Reference to the requested element.
         */
        T& operator[](uint16_t ElementIndex){
            return at(ElementIndex);
        }//operator 

        /**
         * \brief Access operator.
         * \param[in] ElementIndex Array position. Must be valid.
         * \return Element at the requested position.
         */
        T operator[](uint16_t ElementIndex)const{
            return m_pData[ElementIndex];
        }//operator[] 

    protected:
        T* m_pData;                 ///< Data buffer.
        uint16_t m_ElementCount;    ///< Number of elements the data buffer can store.
    };//TArray


}//name space

#endif