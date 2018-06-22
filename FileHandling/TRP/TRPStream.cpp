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
 * @file TRPStream.cpp
 * gpstom::TRPStream - TRP[abc] format fie stream
 */
 

 #include "TRPStream.hpp"

namespace gpstom
{


	TRPStream :: 
		TRPStream()
		: wroteEOF(false),
			writingMode(false),
			lastLine(std::string())
	{
	}

	TRPStream ::
		TRPStream(const char* filename,
						std::ios::openmode mode): FFTextStream(filename, mode)
	{
		init(mode);
	}

	TRPStream ::
		~TRPStream()
	{
		close();
	}

	void TRPStream :: 
		close(void) throw(Exception)
	{
		if(is_open())
		{
			try{
				//if writing, add the final line
				if(writingMode && !wroteEOF) {
					(*this) << "EOF\n";
					wroteEOF = true;
				}
				FFTextStream::close();
				
			}
			catch(std::exception& e) {
				Exception ge(e.what());
				GPSTOM_THROW(ge);
			}
		}
	}


	void TRPStream :: 
		open(const char* filename, std::ios::openmode mode)
	{
		FFTextStream::open(filename,mode);
		init(mode);
	}

	void TRPStream :: init(std::ios::openmode mode)
	{
		header = TRPHeader();
		warnings.clear();

		// for close() later
		wroteEOF = writingMode = false;
		if( (mode & std::ios::out) && !(mode & std::ios::in) )
			writingMode = true;
	}

} //namespace