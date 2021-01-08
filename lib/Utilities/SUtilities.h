/*****************************************************************************\
*                                                                           *
* File(s): SUtilities.hpp                                                   *
*                                                                           *
* Content: SIngleton class which contains valuable utility methods.         *
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

#ifndef ARDUFORGE_SUTILITIES_H
#define ARDUFORGE_SUTILITIES_H

#include <inttypes.h>

namespace ArduForge{

    /**
     * \brief This class defines a number of handy utility functions.
     */
    class SUtilities{
    public:
        /**
         * \brief Computes the available random access memory on an Arduino board.
         * \return Size of the available ram in bytes.
         */
        static int16_t freeMemory(void);

    protected:
        /**
         * \brief Constructor
         */
        SUtilities(void){

        }//constructor

        /**
         * \brief Destructor 
         */
        ~SUtilities(void){

        }//Destructor
    private:

    };//SUtilities

    typedef SUtilities Utilities;
}//name space

#endif