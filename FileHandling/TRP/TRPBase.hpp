//========================================================================
//
//
//
//
//
//                   Copy Right
//
//
//
//
//
//
//=========================================================================

//=========================================================================
//     Descreption
//
//=========================================================================


/**
* @file TRPBase.hpp
* Base clas for TRP file data
*/

#ifndef GPSTOM_TRPBASE_HPP
#define GPSTOM_TRPBASE_HPP

#include "FFData.hpp"

namespace gpstom
{
	// @ingroup FileHandling
	//@{

	/// This class is here to make readable inhertance diagrams.
	class TRPBase : public FFData
	{
	public:
		/// Destructor per the coding standards
		virtual ~TRPBase() {}
	};


	//@}
} // namespace

#endif // GPSTOM_TRPBASE_HPP

