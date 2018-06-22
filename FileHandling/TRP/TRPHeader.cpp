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
 * @file TRPHeader.cpp
 * EEncapsulate Header of TRP file data, including I.O
 */

#include "StringUtils.hpp"
#include "TRPStream.hpp"
#include "TRPHeader.hpp"
#include "CivilTime.hpp"
#include "MJD.hpp"
#include "GPSWeekSecond.hpp"

#define debug false

namespace gpstom
{
	using namespace StringUtils;
	using namespace std;


	void TRPHeader::reallyGetRecord(FFStream& ffs)
		throw(exception, FFStreamError, StringException)
	{
		TRPStream& strm = dynamic_cast<TRPStream&>(ffs);

		string line;
		strm.formattedGetLine(line);

		if(debug) std::cout << "TRP Header Line 1" << line << std::endl;

		// line 1
		title = line.substr(0,65);
		int day   = asInt(line.substr(65,2));
		int mon   = getDigitalMonth(line.substr(68, 3));
		if (mon == 0) {
			FFStreamError e("Unknow month characters:" + line.substr(68, 3));
			GPSTOM_THROW(e);
		}
		int year  = asInt(line.substr(72,2));
		int hour  = asInt(line.substr(75,2));
		int minute= asInt(line.substr(78,2));

		try {
			time = CivilTime(year, mon, day, hour, minute, 0.0);
		}
		catch (gpstom::Exception& e){
			FFStreamError fe("Invalid time:" + string(1, line[0]));
			GPSTOM_THROW(fe);
		}

		strm.formattedGetLine(line);
		// line 2 useless line, seperated line
		if (debug) std::cout << "TRP Header line 2" << line << std::endl;

		strm.formattedGetLine(line);
		// line 3 modelings
		if (debug) std::cout << "TRP Header line 3" << line << std::endl;
		if (line.substr(1,14) == "A PRIORI MODEL")
		{
			//get the priori model
			switch (asInt(line.substr(18, 3))) {
				case  0: pModel = unknownp; break;
				case -1: pModel = Saastamoinen; break;
				case -2: pModel = Hopfieldp; break;
				case -3: pModel = EssenFroome; break;
				case -4: pModel = Marini_Murray; break;
				case -5: pModel = SaastamoinenwNiellDry; break;
				case -6: pModel = GPTwGMFDryWet; break;
				case -7: pModel = ECMWFwVMF1DryWet; break;
				case -8: pModel = MendesPavlis; break;
				case -11: pModel = SaastamoinenDry; break;
				case -12: pModel = HopfieldDry; break;
				case -13: pModel = SimplifiedHopfieldDry; break;
				case -15: pModel = SaastamoinenDrywNiellDry; break;
				case -16: pModel = GPTDrywGMFDry; break;
				case -17: pModel = ECMWFwVMF1DryWet; break;
				default:
					FFStreamError e("Unknow priori model value in line 3: "	+ line.substr(18, 3));
					GPSTOM_THROW(e);
			}
		}
		else
		{
			FFStreamError e("Unknown priori model label in line 3: " + line.substr(1,14));
			GPSTOM_THROW(e);
		}

		if (line.substr(24,16) == "MAPPING FUNCTION")
		{
			//get the mapping function
			switch(asInt(string(1, line[45]))) {
				case 0: mFunc = unknownm; break;
				case 1: mFunc = cos_1; break;
				case 2: mFunc = Hopfieldm; break;
				case 3: mFunc = DryNiell; break;
				case 4: mFunc = WetNiell; break;
				case 5: mFunc = DryGMF; break;
				case 6: mFunc = WetGMF; break;
				case 7: mFunc = DryVMF1; break;
				case 8: mFunc = WetVMF1; break;
				default:
					FFStreamError e("Unknow mapping function value in line 3: " + line.substr(24, 16));
					GPSTOM_THROW(e);
			}
		}
		else
		{
			FFStreamError e("Unknown mapping function label in line 3:  " + line.substr(24,16));
			GPSTOM_THROW(e);
		}

		if (line.substr(49,14) == "GRADIENT MODEL")
		{
			//get the gradient model
			switch (asInt(string(1, line[68]))) {
				case 0: gModel = gradientModel::Noestimation; break;
				case 1: gModel = gradientModel::Tilting; break;
				case 2: gModel = gradientModel::Linear; break;
				case 3: gModel = gradientModel::TANZ; break;
				case 4: gModel = gradientModel::CHENHER; break;
				default:
					FFStreamError e("Unknow mapping function value in line 3: " + line.substr(49, 14));
					GPSTOM_THROW(e);
			}
		}
		else
		{
			FFStreamError e("Unknown gradient function lable in line 3: " + line.substr(49, 14));
			GPSTOM_THROW(e);
		}

		if (line.substr(72,14) == "MIN. ELEVATION")
		{
			//get the cut-off angle [degrees]
			cutoff = asDouble(line.substr(90,2));
		}
		else
		{
			FFStreamError e("Unknown cut-off angle in line 3: " + line.substr(72,14));
			GPSTOM_THROW(e);
		}

		if (line.substr(95,16) == "TABULAR INTERVAL")
		{		
			// get the interval
			interval = asInt(line.substr(112,6));
			tabular  = asInt(line.substr(120,6));
		}
		else
		{
			FFStreamError e("Unknown tabelar interval in line 3: " + line.substr(95,16));
			GPSTOM_THROW(e);
		}
		
		// read in 3 unused line 
		for(int i = 4; i<= 6; i++)   // line 4-6
		{
			strm.formattedGetLine(line);
			if (debug) std:cout << "TRP Header Line " << i << " " << line << std::endl;
			if (i == 5)
			{
				tableheader = line;
			}
		}

		// save the header, for use later when reading TRPData records
		strm.header = *this;
	} // end TRPHeader::reallyGetRecord()

	void TRPHeader::reallyPutRecord(FFStream& ffs) const
		throw(exception, FFStreamError, StringException)
	{
		try {

			TRPStream& strm = dynamic_cast<TRPStream&>(ffs);
			string line;

			CivilTime civTime(time);
			//line 1
			line  = title;
			//line += string(12,' ');
			line += civTime.printf("%02d-%C-%2Y %2H:%2M");
			strm << line << endl;
			strm.lineNumber++;

			//line 2  seperated line
			line = string(133,'-');
			strm << line << endl;
			strm.lineNumber++;

			//line 3 modelings
			line  = " A PRIORI MODEL:";
			line += rightJustify(asString(pModel),5);
			line += "   MAPPING FUNCTION:";
			line += rightJustify(asString(mFunc),5);
			line += "   GRADIENT MODEL:";
			line += rightJustify(asString(gModel),5);
			line += "   MIN. ELEVATION:";
			line += rightJustify(asString(cutoff,0),5);
			line += "   TABULAR INTERVAL:";
			line += rightJustify(asString(interval),6);
			line += " /" + rightJustify(asString(tabular),6);
			strm << line << endl;
			strm.lineNumber++;

			// line 4 empty line
			strm << endl;
			strm.lineNumber++;

			// line 5 comment table labels
			strm << tableheader << endl;
			strm.lineNumber++;

			// line 6
			strm << endl;
			strm.lineNumber++;

			// save haeder for use with TRPHeadData::reallyPut
			strm.header = *this;

		}
		catch(Exception& e) { GPSTOM_THROW(e);}
		catch(std::exception& e) { Exception g(e.what()); GPSTOM_THROW(g); }
	}

	void TRPHeader::dump(ostream& s) const throw()
	{
		s << "TRP Title:" << title << endl;
		CivilTime ct(time);
		s << "Caculation Time:" << ct.printf("%4Y/%02m/%02d %2H:%02M:%02S") << endl;
		s << "A PRIORI MODEL:" << pModel << endl;
		s << "MAPPING FUNCTION:" << mFunc << endl;
		s << "GRADIENT MODEL:" << gModel << endl;
		s << "MIN. ELEVATION:" << cutoff << endl;
		s << "TABULAR:" << interval << " / " << tabular << endl;
		s << "TABLE Hedar:" << tableheader << endl;

		s << "END of TRP header" << endl;
	} // end TRPHeader::dump()

} // namespace