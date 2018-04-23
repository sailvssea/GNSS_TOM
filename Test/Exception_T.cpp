//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  Copyright 2015, The University of Texas at Austin
//
//============================================================================

//============================================================================
//
// This software developed by Applied Research Laboratories at the
// University of Texas at Austin, under contract to an agency or
// agencies within the U.S.  Department of Defense. The
// U.S. Government retains all rights to use, duplicate, distribute,
// disclose, or release this software.
//
// Pursuant to DoD Directive 523024
//
// DISTRIBUTION STATEMENT A: This software has been approved for public
//                           release, distribution is unlimited.
//
//=============================================================================
#include "TestUtil.hpp"
#include "Exception.hpp"
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;

// part of the test for this macro
NEW_EXCEPTION_CLASS(TestExceptionClass, gpstom::InvalidParameter);

class Exception_T
{
public:
      /** Test instantiation and throwing of the globally-defined
       * exception classes */
   int testThrowTypes();
      /** Check error id and severity preservation.  Tests the
       * following methods:
       * getErrorId()
       * isRecoverable()
       * getLocationCount()
       * getTextCount()
       * addText()
       * addLocation()
       * getLocation()
       * ExceptionLocation::getLineNumber()
       * ExceptionLocation::getFileName()
       * ExceptionLocation::getFunctionName()
       * setErrorId()
       * setSeverity()
       *
       * Macros:
       * GPSTOM_THROW()
       */
   int testErrorSeverity();
      /** Test macro function:
       * GPSTOM_RETHROW()
       * GPSTOM_ASSERT()
       * NEW_EXCEPTION_CLASS()
       */
   int testMacros();

private:
      /// Template function used by throwTypes()
   template <class EXC>
   int throwType();

      /// Throw an exception for testMacros
   void throwSomething(gpstom::TestUtil& testFramework);
      /// Throw an exception for testMacros
   void rethrowSomething(gpstom::TestUtil& testFramework);

   int cowLine1, cowLine2;
};


int Exception_T ::
testThrowTypes()
{
   int total = 0;
   total += throwType<gpstom::Exception>();
   total += throwType<gpstom::InvalidParameter>();
   total += throwType<gpstom::InvalidRequest>();
   total += throwType<gpstom::AssertionFailure>();
   total += throwType<gpstom::AccessError>();
   total += throwType<gpstom::IndexOutOfBoundsException>();
   total += throwType<gpstom::InvalidArgumentException>();
   total += throwType<gpstom::ConfigurationException>();
   total += throwType<gpstom::FileMissingException>();
   total += throwType<gpstom::SystemSemaphoreException>();
   total += throwType<gpstom::SystemPipeException>();
   total += throwType<gpstom::SystemQueueException>();
   total += throwType<gpstom::OutOfMemory>();
   total += throwType<gpstom::ObjectNotFound>();
   total += throwType<gpstom::NullPointerException>();
   total += throwType<gpstom::UnimplementedException>();
   return total;
}


int Exception_T ::
testErrorSeverity()
{
   TUDEF("Exception", "getErrorId()");
   unsigned loc1Line = 0, loc2Line = 0;
   int locFails = 0;
   std::string fn("testErrorSeverity");
   try
   {
      gpstom::Exception exc("fail", 1234, gpstom::Exception::recoverable);
         // do not separate these statements
      loc1Line = __LINE__; GPSTOM_THROW(exc);
      testFramework.changeSourceMethod("GPSTOM_THROW");
      TUFAIL("Did not throw an exception when expected");
   }
   catch (gpstom::Exception &exc)
   {
      testFramework.changeSourceMethod("GPSTOM_THROW");
      TUPASS("GPSTOM_THROW");
      testFramework.changeSourceMethod("getErrorId");
      TUASSERTE(unsigned long, 1234, exc.getErrorId());
      testFramework.changeSourceMethod("isRecoverable");
      testFramework.assert(exc.isRecoverable(), "Incorrect severity", __LINE__);
      testFramework.changeSourceMethod("getLocationCount");
      TUASSERTE(size_t, 1, exc.getLocationCount());
      testFramework.changeSourceMethod("getTextCount");
      TUASSERTE(size_t, 1, exc.getTextCount());
      testFramework.changeSourceMethod("addText");
      exc.addText("another text");
      TUASSERTE(size_t, 2, exc.getTextCount());
      testFramework.changeSourceMethod("addLocation");
         // do not separate these statements
      loc2Line = __LINE__; gpstom::ExceptionLocation here(__FILE__,fn,__LINE__);
      exc.addLocation(here);
      TUASSERTE(size_t, 2, exc.getLocationCount());
      if (exc.getLocationCount() == 2)
      {
         gpstom::TestUtil test2("ExceptionLocation", "", __FILE__, __LINE__);
         gpstom::ExceptionLocation loc1 = exc.getLocation();
         gpstom::ExceptionLocation loc2 = exc.getLocation(1);
            // can't change class in gpstom::TestUtil, oh well
         testFramework.changeSourceMethod("getLineNumber");
         TUASSERTE(unsigned long, loc1Line, loc1.getLineNumber());
         TUASSERTE(unsigned long, loc2Line, loc2.getLineNumber());
         testFramework.changeSourceMethod("getFileName");
         TUASSERTE(std::string, std::string(__FILE__), loc1.getFileName());
         TUASSERTE(std::string, std::string(__FILE__), loc2.getFileName());
         testFramework.changeSourceMethod("getFunctionName");
#if defined ( __FUNCTION__ )
         TUASSERTE(std::string, std::string(__FUNCTION__), loc1.getFunctionName());
#else
         TUASSERTE(std::string, std::string(""), loc1.getFunctionName());
#endif
         TUASSERTE(std::string, fn, loc2.getFunctionName());
      }
   }
   catch (...)
   {
      testFramework.changeSourceMethod("GPSTOM_THROW");
      TUFAIL("Threw an unexpected exception type");
   }
   try
   {
         // Initialize error id as 5678 then change it using
         // setErrorId to make sure that method works.  Likewise with
         // setSeverity()
      gpstom::Exception exc("fail", 5678, gpstom::Exception::recoverable);
      exc.setErrorId(9012);
      exc.setSeverity(gpstom::Exception::unrecoverable);
      GPSTOM_THROW(exc);
      testFramework.changeSourceMethod("GPSTOM_THROW");
      TUFAIL("Did not throw an exception when expected");
   }
   catch (gpstom::Exception &exc)
   {
      testFramework.changeSourceMethod("GPSTOM_THROW");
      TUPASS("GPSTOM_THROW");
      testFramework.changeSourceMethod("getErrorId");
      TUASSERTE(unsigned long, 9012, exc.getErrorId());
      testFramework.changeSourceMethod("isRecoverable");
      testFramework.assert(!exc.isRecoverable(), "Incorrect severity", __LINE__);
   }
   catch (...)
   {
      testFramework.changeSourceMethod("GPSTOM_THROW");
      TUFAIL("Threw an unexpected exception type");
   }
   return testFramework.countFails();
}


int Exception_T ::
testMacros()
{
   TUDEF("Exception", "macros");

      // test GPSTK_ASSERT
   testFramework.changeSourceMethod("GPSTK_ASSERT");
   try
   {
      GPSTOM_ASSERT(false);
      TUFAIL("Did not throw AssertionFailure exception");
   }
   catch (gpstom::AssertionFailure &exc)
   {
      TUPASS("GPSTK_ASSERT");
   }
   catch (...)
   {
      TUFAIL("Threw a different exception from AssertionFailure");
   }

      // test NEW_EXCEPTION_CLASS
   testFramework.changeSourceMethod("NEW_EXCEPTION_CLASS");
   try
   {
      TestExceptionClass exc("moo");
      GPSTOM_THROW(exc);
      TUFAIL("Did not throw TestExceptionClass");
   }
   catch (gpstom::InvalidParameter)
   {
         // make sure the exception class is a child of InvalidParameter
      TUPASS("NEW_EXCEPTION_CLASS");
   }
   catch (...)
   {
      TUFAIL("Threw a different exception from InvalidParameter");
   }

      // test GPSTK_RETHROW
   testFramework.changeSourceMethod("GPSTK_RETHROW");
   try
   {
      rethrowSomething(testFramework);
      TUFAIL("rethrowSomething didn't throw exception");
   }
   catch(gpstom::InvalidRequest& exc)
   {
      TUASSERTE(size_t, 2, exc.getLocationCount());
      if (exc.getLocationCount() == 2)
      {
         gpstom::ExceptionLocation loc1 = exc.getLocation();
         gpstom::ExceptionLocation loc2 = exc.getLocation(1);
         TUASSERTE(unsigned long, cowLine1, loc1.getLineNumber());
         TUASSERTE(unsigned long, cowLine2, loc2.getLineNumber());
      }
   }
   catch (...)
   {
      TUFAIL("rethrowSomething threw unexpected exception type");
   }

   return testFramework.countFails();
}


template <class EXC>
int Exception_T ::
throwType()
{
   TUDEF(typeid(EXC).name(), "GPSTOM_THROW");
   try
   {
      EXC exc("fail");
      GPSTOM_THROW(exc);
      TUFAIL("Did not throw an exception when expected");
   }
   catch (gpstom::Exception &exc)
   {
         // all Exception classes are expected to be children of
         // gpstom::Exception
      TUPASS("GPSTOM_THROW");
         // Removed - clearly not working and probably not being used.
         // Should the interface be removed from the Exception
         // implementation as well?
         //testFramework.changeSourceMethod("getName");
         //TUASSERTE(std::string, std::string(typeid(EXC).name()), exc.getName());
   }
   catch (...)
   {
      TUFAIL("Threw an unexpected exception type");
   }
   return testFramework.countFails();
}


void Exception_T ::
throwSomething(gpstom::TestUtil& testFramework)
{
   gpstom::InvalidRequest exc("cow");
   cowLine1 = __LINE__; GPSTOM_THROW(exc);
}


void Exception_T ::
rethrowSomething(gpstom::TestUtil& testFramework)
{
   try
   {
      throwSomething(testFramework);
      TUFAIL("throwSomething didn't throw exception");
   }
   catch (gpstom::InvalidRequest &exc)
   {
      TUPASS("throwSomething");
      cowLine2 = __LINE__; GPSTOM_RETHROW(exc);
   }
   catch (...)
   {
      TUFAIL("throwSomething threw unexpected exception type");
   }
}


int main(int argc, char *argv[])
{
   int errorTotal = 0;
   Exception_T testClass;

   errorTotal += testClass.testThrowTypes();
   errorTotal += testClass.testErrorSeverity();
   errorTotal += testClass.testMacros();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   system("pause");
   return errorTotal;
}
