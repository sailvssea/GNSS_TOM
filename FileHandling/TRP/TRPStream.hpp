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
 * @file TRPStream.hpp
 * gpstom::TRPStream - TRP[abc] format file stream
 */

#ifndef TRPSTREAM_INCLUDE
#define TRPSTREAM_INCLUDE

#include "FFTextStream.hpp"
#include "TRPHeader.hpp"

namespace gpstom
{
	///@INGROUP FileHandling
	///@{
	
	/**
	 * This Class performs file I/O on an SP3 file for the TRPHeader
	 * and TRPData classes。
	 * @note the file format.
	 */
	
	class TRPStream : public FFTextStream
	{
	public:
		/// Default constructor
		TRPStream();

		/**
		 * @brief [brief description]
		 * @details [long description]
		 * 
		 * @param filename [the name of the ASCII TRP format file to be open] 
		 * @param mode [mode the ios::openmode to be used]
		 */
		TRPStream(const char* filename,
					std::ios::openmode mode = std::ios::in);

		/// destructor; override to force "close"
		virtual ~TRPStream();

		/// override close() to write EOF line
		virtual void close(void) throw(Exception);

		/**
		 * @brief [brief description]
		 * @details [long description]
		 * 
		 * @param filname the name of the ASCII TRP format file
		 *   to be opened
		 * @param mode the ios::openmode to be used
		 */	
		virtual void open(const char* filname, std::ios::openmode mode);

		TRPHeader header;    ///< TRPHeader of this file
		bool wroteEOF;       ///< True if the final 'EOF' has been read.
		bool writingMode;    ///< True if the stream is open in 'out', not 'in', mode
		CommonTime currentEpoch; ///< Time from last epoch record read
		std::string lastLine;    ///< Last line read, perhaps not yet processed
		std::vector<std::string> warnings; ///< warnings produced by reallyGetRecord()s

	private:

		/// Initialize internal data structures according to file mode
		void init(std::ios::openmode);

	}; // class TRPStream

	//@}
} // namespace gpstom

#endif // TRPSTREAM_INCLUDE