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
 *  @file TRPData.hpp
 *  Encapsulate TRP file data, including I/O
 */
 
#ifndef GPSTOM_TRPDATA_HPP
#define GPSTOM_TRPDATA_HPP

#include "TRPBase.hpp"
#include "CommonTime.hpp"
#include <iomanip>

namespace gpstom
{
	/// @ ingroup FileHandling
	//@ {

	/**
	 *  This class encapsulates data for TRP files from BERNESE
	 *  
	 *  This class is used in conjuction with class TRPStream, which
	 *  handles the I/O, and TRPHeader, which holds information from
	 *  the TRP file header.
	 * @code
	 * TRPStream ss("F1_122340.TRP");
	 * TRPHeader sh;
	 * TRPData sd;
	 *
	 * ss >> sh;
	 *
	 * while (ss >> sd)
	 * {
	 *		// Interesting stuff...
	 * }
	 * 
	 * TRPStream ssout("myfile.TRP", ios::out);
	 * ssout << sh;
	 * for(...) {
	 *		// perhaps modify sd
	 *		ssout << sd
	 * }
	 * @endcode
	 *
	 * @sa gpstom::TRPHeader and gpstom::TRPStream for more information.
	 * @sa petest.cpp for an example.
	 */

	class TRPData : public TRPBase
	{
	public:
		/// constructor.
		TRPData() :  time(CommonTime::BEGINNING_OF_TIME), Flag(' ')
		{}

		/// Destructor
		virtual ~TRPData() {}

		// The next four lines is our common interface
		/// TRPData is "data" so this fuction always returns true.

		virtual bool isData() const {return true;}

		/// Debug output function
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
		virtual void dump(std::ostream& s=std::cout, bool includeC=true) const throw();
#pragma clang diagnostic pop


		std::string StaName;		///< Station name
		CommonTime time;			///< Time of epoch for this record
		char Flag;					///< Flag
		double MOD_U;				///< zenth delay from apriori model (m)
		double CORR[3];				///< Corrected zenth delay (UNE,m)
		double SIGMA[3];			///< std dev of the zenth delay (UNE,m)
		double TOTAL_U;				///< total zenth delay (m)
									///< total zenth delay = zenth delay from apriori model +
																///corrected zenth delay

	protected:

		/// writes the formatted record to the FFStream \a s.
		/// @warning This function is currently unimplemented
		virtual void reallyPutRecord(FFStream& s) const
			throw(std::exception, FFStreamError,
				gpstom::StringUtils::StringException);

			/**
			 *  This fuction reads a record from the given FFStream.
			 *  If an error is encountered in retrieving the record, the 
			 *  stream is reset to its original position and its fail-bit is set.
			 *  @throws StringException when a StringUtils fuction fails
			 *  @throws FFStreamError when exceptions (failbit) is set and 
			 *  a read or formatting error occurs. This also reset the stream
			 *  to its pre-read position.
			 */
		virtual void reallyGetRecord(FFStream& s)
			throw(std::exception, FFStreamError,
				gpstom::StringUtils::StringException);


	};

		//@}
}	// namespace

#endif