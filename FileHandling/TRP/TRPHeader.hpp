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
 * @file TRPHeader.hpp
 * Encapsulate Header of TRP file data, including I.O
 */

#ifndef GPSTOM_TRPHEADER_HPP
#define GPSTOM_TRPHEADER_HPP

#include <string>
#include "TRPBase.hpp"
#include "CommonTime.hpp"
#include "TimeSystem.hpp"


namespace gpstom
{

	/// @ingroup FileHandling
	//@{

	/**
	* This class models the header for a TRP file.
	*
	*
	* @note A valid header MUST be read before data can be read
	* from an TRP file because the header contains the file version
	* or format. The version in this Header is used by TRPStream to
	* determine the format of output TRPData
	*
	*
	* @sa gpstom::TRPStream and gpstom::TRPData for more information
	*/
	class TRPHeader : public TRPBase
	{
	public:

		const enum MONTH 
		{
			JAN = 1,
			FEB = 2,
			MAR = 3,
			APR = 4,
			MAY = 5,
			JUN = 6,
			JUL = 7,
			AUG = 8,
			SEP = 9,
			OCT = 10,
			NOV = 11,
			DEC = 12
		};

		/// access the digital number of month or file format
		/// @para month char three abbr
		/// @return the digital month
		static const int getDigitalMonth(const std::string month) throw()
		{
			int mon = 0;
			if (month == "JAN") mon = 1;
			if (month == "FEB") mon = 2;
			if (month == "MAR") mon = 3;
			if (month == "APR") mon = 4;
			if (month == "MAY") mon = 5;
			if (month == "JUN") mon = 6;
			if (month == "JUL") mon = 7;
			if (month == "AUG") mon = 8;
			if (month == "SEP") mon = 9;
			if (month == "OCT") mon = 10;
			if (month == "NOV") mon = 11;
			if (month == "DEC") mon = 12;

			return mon;
		};
		
		enum prioriModel
		{
			unknownp = 0,
			Saastamoinen = -1,
			Hopfieldp = -2,
			EssenFroome = -3,
			Marini_Murray = -4,
			SaastamoinenwNiellDry = -5,
			GPTwGMFDryWet = -6,
			ECMWFwVMF1DryWet = -7,
			MendesPavlis = -8,
			SaastamoinenDry = -11,
			HopfieldDry = -12,
			SimplifiedHopfieldDry = -13,
			SaastamoinenDrywNiellDry = -15,
			GPTDrywGMFDry = -16,
			ECMWFDrywVMF1Dry = -17
		};

		enum mapFunc
		{
			unknownm = 0,
			cos_1 = 1,
			Hopfieldm = 2,
			DryNiell = 3,
			WetNiell = 4,
			DryGMF = 5,
			WetGMF = 6,
			DryVMF1 = 7,
			WetVMF1 = 8
		};

		enum gradientModel
		{
			Noestimation = 0,
			Tilting = 1,
			Linear = 2,
			TANZ = 3,
			CHENHER =4
		};

		// constructor
		TRPHeader() : pModel(unknownp), 
				      mFunc(unknownm), 
			          gModel(Noestimation), timeSystem(TimeSystem::Any)
		{}


		/// destructor
		virtual ~TRPHeader() {}


		/// access the priori model or file format
		/// @return the current priorModel
		prioriModel getprioriModel() const throw() {return pModel;}
		/**
		 *  access the priori model or file format as a string
		 *  @return a string  of the current priori model
		 */
		std::string prioriModelString(void) const throw()
		{
			return prioriModelString(pModel);
		}
		
		/**
		 * access the version or file format as a string
		 * @param pmod priori model
		 * @return a string of the current priori model
		 */
		static std::string prioriModelString(prioriModel pmod) throw()
		{
			std::string str;
			switch(pmod) {
				case Saastamoinen:
					str = std::string("Saastamoinen"); break;
				case Hopfieldp:
					str = std::string("Hopfield (Remondi)"); break;
				case EssenFroome:
					str = std::string("Essen and Froome"); break;
				case Marini_Murray:
					str = std::string("Marini-Murray (SLR)"); break;
				case SaastamoinenwNiellDry:
					str = std::string("Saastamoinen with Niell dry mapping"); break;
				case GPTwGMFDryWet:
					str = std::string("GPT with GMF dry+wet mapping"); break;
				case ECMWFwVMF1DryWet:
					str = std::string("ECMWF with VMF1 dry+wet mapping"); break;
				case MendesPavlis:
					str = std::string("Mendes-Pavlis (SLR)"); break;
				case SaastamoinenDry:
					str = std::string("Saastamoinen dry part only"); break;
				case HopfieldDry:
					str = std::string("Hopfield dry part only"); break;
				case SimplifiedHopfieldDry:
					str = std::string("Simplified Hopfield dry part only"); break;
				case SaastamoinenDrywNiellDry:
					str = std::string("Saastamoinen dry with Niell dry mapping"); break;
				case GPTDrywGMFDry:
					str = std::string("GPT dry with GMF dry mapping"); break;
				case ECMWFDrywVMF1Dry:
					str = std::string("ECMWF dry with VMF1 dry mapping"); break;
				case unknownp: default:
					str = std::string("Unknown"); break;
			};
			return str;
		}



		/// access the mapping function or file format
		/// @return the current mappng function
		mapFunc getmapFunc() const throw() {return mFunc;}

		/**
		*  access the mapping function or file format as a string
		*  @return a string  of the current priori model
		*/
		std::string mapFuncString(void) const throw()
		{
			return mapFuncString(mFunc);
		}

		/**
		* access the mapping function or file format as a string
		* @param mf mapping function
		* @return a string  of the current mapping function
		*/
		static std::string mapFuncString(mapFunc mf) throw()
		{
			std::string str;
			switch (mf) {
			case cos_1:
				str = std::string("1/Cos(z)"); break;
			case Hopfieldm:
				str = std::string("Hopfield"); break;
			case DryNiell:
				str = std::string("Dry Niell"); break;
			case WetNiell:
				str = std::string("Wet Niell"); break;
			case DryGMF:
				str = std::string("Dry GMF"); break;
			case WetGMF:
				str = std::string("Wet GMF"); break;
			case DryVMF1:
				str = std::string("Dry VMF1"); break;
			case WetVMF1:
				str = std::string("Wet VMF1"); break;
			case unknownm: default:
				str = std::string("Unknown"); break;
			};
			return str;
		}


		/// access the gradient model or file format
		/// @return the current gradient model
		gradientModel getgradientModel() const throw() {return gModel;}

		/**
		*  access the gradient model or file format as a string
		*  @return a string version of the current gradient model
		*/
		std::string gradientModelString(void) const throw()
		{
			return gradientModelString(gModel);
		}

		/**
		* access the version or file format as a string
		* @param pmod priori model
		* @return a string version of the current mapping function
		*/
		static std::string gradientModelString(gradientModel gmod) throw()
		{
			std::string str;
			switch (gmod) {
			case Noestimation:
				str = std::string("No estimation"); break;
			case Tilting:
				str = std::string("Tilting"); break;
			case Linear:
				str = std::string("Linear"); break;
			case TANZ:
				str = std::string("TANZ from MacMillan (1995)"); break;
			case CHENHER:
				str = std::string("CHENHER from Chen and Herring (1997)"); break;
			default:
				str = std::string("Unknown"); break;
			};
			return str;
		}

		/// return a string with time system name
		std::string timeSystemString() const throw()
		{
			return timeSystem.asString();
		};

		// The next four lines is our common interface
		//  TRPHeader is a "header" so this function always retruns true.
		virtual bool isHeader() const { return true; }

		//
		virtual void dump(std::ostream& s=std::cout) const throw();

		// member variable
		std::string title;     ///Title
		prioriModel pModel;    ///Priori model
		mapFunc mFunc;         ///mapping function
		gradientModel gModel;  ///gradient model
		double cutoff;         ///cut-off angle, in degrees
		int interval;       ///tabular interval 
		int tabular;        ///tabular
		std::string tableheader; // headers.
		CommonTime time;      /// time of the calculation
		TimeSystem timeSystem;  ///< Time system used

		friend class TRPData;
	protected:
		/// Writes the record formatted to the FFStream \a s.
		/// @throws StringException when a StringUtils function fails
		virtual void reallyPutRecord(FFStream& s) const
			throw(std::exception, FFStreamError,
				StringUtils::StringException);

		/// This function retrieves the SP3 header from the given FFStream.
		/// If an error is encoutered in the retrieval of the header, the
		/// stream is reset to its original position and ites fail-bit is set.
		/// @throws StringException when a StringUtils function fails
		/// @throws FFStreamError when exceptions(failbit) is set and 
		/// a read or formatting error occurs. This also resets the stream
		/// to its pre-read position.
		
		virtual void reallyGetRecord(FFStream& s)
			throw(std::exception, FFStreamError,
				StringUtils::StringException);


	}; // end class TRPHeader


	//@}
} // namespace

#endif