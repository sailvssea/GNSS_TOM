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

/// @file TRPData.cpp
/// Encapsulate TRP file data, including I/O

#include "TRPData.hpp"
#include "TRPStream.hpp"
#include "TRPHeader.hpp"
#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"

using namespace gpstom::StringUtils;
using namespace std;

namespace gpstom
{
	void TRPData::reallyGetRecord(FFStream& ffs)
		throw(exception, FFStreamError, StringException)
	{
		// cast the stream to be an TRPStream
		TRPStream& strm = dynamic_cast<TRPStream&>(ffs);

		// status says which records have been written already this call
		// status = 1 if a record was read 
		int status = 0;
		
		// Timesystem for this stream
		TimeSystem timeSystem;
		timeSystem.fromString(strm.header.timeSystemString());

		// loop until an error occurs, or until the entire record ( which may consist
		// of two lines) is read.

		bool unexpectedEOF = false;
		
		while(1) {
			// set the time to be the last epoch read by the stream
			time = strm.currentEpoch;

			// process the lastLine string stored in strm - contains the last line read
			// empty record
			if(strm.lastLine.size() < 3) { // 3 b/c it may contain "EOF"
				// nothing in lastLine - do nothing here, get another line
				;
			}

			// EOF has been read
			else if(strm.lastLine.substr(0,3) == string("EOF")) {
				// if a data record has already been processed during this call,
				// then return here, and let the next call process this EOF.
				// This gives the caller a chance to process the data before hitting EOF.

				break;
			}
			// Record line read
			else
			{
				status = 1;

				StaName = strm.lastLine.substr(0,19);	// Station name

				Flag = strm.lastLine[19]; 				// Flag

				MOD_U = asDouble(strm.lastLine.substr(68,6));

				CORR[0] = asDouble(strm.lastLine.substr(75,8));		// UNE corrected zenth delay
				CORR[1] = asDouble(strm.lastLine.substr(100,8));
				CORR[2] = asDouble(strm.lastLine.substr(117,8));

				SIGMA[0] = asDouble(strm.lastLine.substr(84,7));	// UNE Sigma
				SIGMA[1] = asDouble(strm.lastLine.substr(109,7));
				SIGMA[2] = asDouble(strm.lastLine.substr(126,7));

				TOTAL_U = asDouble(strm.lastLine.substr(92,7));		// Total zenth delay

				// parse the time
				int year = asInt(strm.lastLine.substr(24,4));
				int month = asInt(strm.lastLine.substr(29,2));
				int dom = asInt(strm.lastLine.substr(32,2));
				int hour = asInt(strm.lastLine.substr(35,2));
				int minute = asInt(strm.lastLine.substr(38,2));
				int second = asInt(strm.lastLine.substr(41,2));

				CivilTime t;
				try {
					t = CivilTime(year,month,dom,hour,minute,second,timeSystem);
				}
				catch (gpstom::Exception& e) {
					FFStreamError fe("Invalid time in:" + strm.lastLine);
					GPSTOM_THROW(fe);
				}
				time = strm.currentEpoch = static_cast<CommonTime>(t);
				
			}

			// be tolerant of files without EOF 
			// if previous iteration of the loop found unexpected EOF, then quit here
			if (unexpectedEOF) {
				// add a warning 
				strm.lastLine = string("EOF");

				// clear the eof bit and return, so the user will process this TRPData
				strm.clear(ios::eofbit);
				break;
			}
			else {             // normal flow
				// read next line into the lastLine
				try {
					strm.formattedGetLine(strm.lastLine);
				}
				catch (FFStreamError& err) {
					string what = err.what().substr(0, 21);
					//cout << "Found unexpected EOF" << endl;
					//cout << "what is" << what << endl;
					//cout << "Here is the buffer:\n" << strm.lastLine << endl;
					if (what == string("text 0:Unexpected EOF")) {
						unexpectedEOF = true;

						// there could still be unprocessed data in the buffer:
						if (strm.lastLine.size() < 3) status = 1;
						else                          status = 0;
					}
					else
						GPSTOM_RETHROW(err);
				}
			}

			if (status == 1) break;

		}	//end while loop processing records
	}	//end reallyGetRecord()


	void TRPData::reallyPutRecord(FFStream& ffs) const
		throw(exception, FFStreamError, StringException)
	{
		string line;

		//cast the stream to be an TRPStream
		TRPStream& strm = dynamic_cast<TRPStream&>(ffs);

		// OUTPUT
		
		line += leftJustify(StaName,19);
		line += Flag;

		CivilTime civTime(time);
		line += "    " + civTime.printf("%4Y %02m %02d %02H %02M %02S");

		line += string(25, ' ');

		line += leftJustify(asString(MOD_U,4),7);
		line += rightJustify(asString(CORR[0], 5), 8);
		line += rightJustify(asString(SIGMA[0], 5), 8);
		line += rightJustify(asString(TOTAL_U, 5), 8);
		line += rightJustify(asString(CORR[1], 5), 9);
		line += rightJustify(asString(SIGMA[1], 5), 8);
		line += rightJustify(asString(CORR[2], 5), 9);
		line += rightJustify(asString(SIGMA[2], 5), 8);

		// write the line just built
		strm << line << endl;
		strm.lineNumber++;

	}	// end reallyPutRecord()

	void TRPData::dump(ostream& s, bool includeC) const throw()
	{
		// dump record Station name, Flag, and current epoch
		s << StaName << " " << Flag << " "
			<< (static_cast<CivilTime>(time)).printf("%Y/%02m/%02d %2H:%02M:%06.3f")
			<< " = "
			<< (static_cast<GPSWeekSecond>(time)).printf("%F/%10.3g");

		// dump MOD_U   CORR_U  SIGMA_U TOTAL_U  CORR_N  SIGMA_N  CORR_E  SIGMA_E
		s << fixed << setprecision(4)
			<< " MOD_U=" << setw(2) << MOD_U;
		
		s << fixed << setprecision(5)
			<< " CORR_U" << setw(2) << CORR[0]
			<< " SIGMA_U" << setw(2) << SIGMA[0]
			<< " TOTAL_U" << setw(2) << TOTAL_U
			<< " CORR_N" << setw(2) << CORR[1]
			<< " SIGMA_N" << setw(2) << SIGMA[1]
			<< " CORR_E" << setw(2) << CORR[2]
			<< " SIGMA_E" << setw(2) << SIGMA[2];

		s << endl;
	}
}