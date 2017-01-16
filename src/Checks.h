#ifndef UNITTEST_CHECKS_H
#define UNITTEST_CHECKS_H

#include "Config.h"
#include "TestResults.h"
#include "MemoryOutStream.h"
#include "TypeHelpers.h"

namespace UnitTest {


template< typename Value >
inline bool Check(Value const value)
{
    return !!value; // doing double negative to avoid silly VS warnings
}


template< typename Expected, typename Actual >
inline bool CheckEqual(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
{
    if (!(expected == actual))
    {
        UnitTest::MemoryOutStream stream;
		stream << "Expected values to be the same, but they were not";
		if (CanStringify(expected) && CanStringify(actual))
		{
			stream << std::endl;
			stream << "\tExpected: " << Stringify(expected) << std::endl;
			stream << "\t  Actual: " << Stringify(actual) << std::endl;
		}
			
        results.OnTestFailure(details, stream.GetText());
        return false;
    }
    return true;
}

bool CheckEqual(TestResults& results, char const* expected, char const* actual, TestDetails const& details);

bool CheckEqual(TestResults& results, char* expected, char* actual, TestDetails const& details);

bool CheckEqual(TestResults& results, char* expected, char const* actual, TestDetails const& details);

bool CheckEqual(TestResults& results, char const* expected, char* actual, TestDetails const& details);

inline bool CheckEqual(TestResults& results, const std::string& expected, const std::string& actual, TestDetails const& details)
{
	return CheckEqual(results, expected.c_str(), actual.c_str(), details);
}

template< typename Comperand, typename Actual >
inline bool CheckNotEqual(TestResults& results, Comperand const& comperant, Actual const& actual, TestDetails const& details)
{
    if (!(comperant != actual))
    {
        UnitTest::MemoryOutStream stream;
		if (CanStringify(comperant))
			stream << "Expected values to be not equal, but they were both " << Stringify(comperant);
		else
			stream << "Expected values to be not equal, but they were equal";

        results.OnTestFailure(details, stream.GetText());
        return false;
    }
    return true;
}

bool CheckNotEqual(TestResults& results, char const* comperant, char const* actual, TestDetails const& details);

bool CheckNotEqual(TestResults& results, char* comperant, char* actual, TestDetails const& details);

bool CheckNotEqual(TestResults& results, char* comperant, char const* actual, TestDetails const& details);

bool CheckNotEqual(TestResults& results, char const* comperant, char* actual, TestDetails const& details);

inline bool CheckNotEqual(TestResults& results, const std::string& comperant, const std::string& actual, TestDetails const& details)
{
    return CheckNotEqual(results, comperant.c_str(), actual.c_str(), details);
}

template< typename Expected, typename Actual, typename Tolerance >
bool AreClose(Expected const& expected, Actual const& actual, Tolerance const& tolerance)
{
    return (actual >= (expected - tolerance)) && (actual <= (expected + tolerance));
}

template< typename Expected, typename Actual, typename Tolerance >
bool CheckClose(TestResults& results, Expected const& expected, Actual const& actual, Tolerance const& tolerance,
                TestDetails const& details)
{
    if (!AreClose(expected, actual, tolerance))
    { 
        UnitTest::MemoryOutStream stream;
		if (CanStringify(expected) && CanStringify(tolerance) && CanStringify(actual))
			stream << "Expected " << Stringify(expected) << " +/- " << Stringify(tolerance) << " but was " << Stringify(actual);
		else
			stream << "Expected values to be close to within a given tolerance, but they weren't";

        results.OnTestFailure(details, stream.GetText());
        return false;
    }
    return true;
}

template< typename Expected, typename Actual, typename Tolerance >
bool AreCloseRelative(Expected const& expected, Actual const& actual, Tolerance const& relativeTolerance)
{
	// Relative error
	Expected relativeError = (expected - actual) / expected;
    return ( relativeError <= relativeTolerance );
}

template< typename Expected, typename Actual, typename Tolerance >
bool CheckCloseRelativeNoZero(TestResults& results, Expected const& expected, Actual const& actual,
                Tolerance const& relativeTolerance, TestDetails const& details)
{
    if (!AreCloseRelative(expected, actual, relativeTolerance))
    { 
        UnitTest::MemoryOutStream stream;
		if (CanStringify(expected) && CanStringify(relativeTolerance) && CanStringify(actual))
			stream << "Expected " << Stringify(expected) << " R(+/-) " << Stringify(relativeTolerance) << " but was " << Stringify(actual);
		else
			stream << "Expected values to be close to within a given relative tolerance, but they weren't";

        results.OnTestFailure(details, stream.GetText());
        return false;
    }
    return true;
}


template< typename Expected, typename Actual >
bool CheckArrayEqual(TestResults& results, Expected const& expected, Actual const& actual,
                int const count, TestDetails const& details)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= (expected[i] == actual[i]);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;

		stream << "Expected array elements to be equal up to " << count << "elements, but they were not.";

		if (CanStringify(expected[0]) && CanStringify(actual[0]))
		{
			stream << std::endl << "\tExpected: [ ";

			for (int expectedIndex = 0; expectedIndex < count; ++expectedIndex)
				stream << Stringify(expected[expectedIndex]) << " ";

			stream << "]";
			stream << std::endl << "\t  Actual: [ ";

			for (int actualIndex = 0; actualIndex < count; ++actualIndex)
				stream << Stringify(actual[actualIndex]) << " ";

			stream << "]" << std::endl;
		}
			
        results.OnTestFailure(details, stream.GetText());
        return false;
    }

    return true;
}

template< typename Expected, typename Actual, typename Tolerance >
bool ArrayAreClose(Expected const& expected, Actual const& actual, int const count, Tolerance const& tolerance)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= AreClose(expected[i], actual[i], tolerance);
    return equal;
}

template< typename Expected, typename Actual, typename Tolerance >
bool CheckArrayClose(TestResults& results, Expected const& expected, Actual const& actual,
                   int const count, Tolerance const& tolerance, TestDetails const& details)
{
    bool equal = ArrayAreClose(expected, actual, count, tolerance);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;
		
		if (CanStringify(tolerance))
			stream << "Expected array elements to be close to within a tolerance of " << Stringify(tolerance) << " up to " << count << "elements, but they were not.";
		else
			stream << "Expected array elements to be close up to " << count << "elements, but they were not.";

		if (CanStringify(expected[0]) && CanStringify(actual[0]))
		{
			stream << std::endl << "\tExpected: [ ";
			
			for (int expectedIndex = 0; expectedIndex < count; ++expectedIndex)
				stream << Stringify(expected[expectedIndex]) << " ";
			
			stream << "]";
			stream << std::endl << "\t  Actual: [ ";
			
			for (int actualIndex = 0; actualIndex < count; ++actualIndex)
				stream << Stringify(actual[actualIndex]) << " ";
			
			stream << "]" << std::endl;
		}
		
        results.OnTestFailure(details, stream.GetText());
        return false;
    }
    return true;
}

template< typename Expected, typename Actual, typename Tolerance >
bool CheckArray2DClose(TestResults& results, Expected const& expected, Actual const& actual,
                   int const rows, int const columns, Tolerance const& tolerance, TestDetails const& details)
{
    bool equal = true;
    for (int i = 0; i < rows; ++i)
        equal &= ArrayAreClose(expected[i], actual[i], columns, tolerance);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;
		
		if (CanStringify(tolerance))
			stream << "Expected array elements to be close to within a tolerance of " << Stringify(tolerance) << " across " << rows << "rows of " << columns << " columns, but they were not.";
		else
			stream << "Expected array elements to be close across " << rows << "rows of " << columns << " columns, but they were not.";

		if (CanStringify(expected[0]) && CanStringify(actual[0]))
		{
			stream << std::endl;
			stream << "\tExpected: [ ";

			for (int expectedRow = 0; expectedRow < rows; ++expectedRow)
			{
				stream << "[ ";
				for (int expectedColumn = 0; expectedColumn < columns; ++expectedColumn)
					stream << Stringify(expected[expectedRow][expectedColumn]) << " ";
				stream << "] ";
			}

			stream << "]" << std::endl;
			
			stream << "\t  Actual: [ ";

			for (int actualRow = 0; actualRow < rows; ++actualRow)
			{
				stream << "[ ";
				for (int actualColumn = 0; actualColumn < columns; ++actualColumn)
					stream << Stringify(actual[actualRow][actualColumn]) << " ";
				stream << "] ";
			}

			stream << "]" << std::endl;
		}

        results.OnTestFailure(details, stream.GetText());
        return false;
    }
    return true;
}

template<typename HaystackIterator, typename Needle>
bool CheckContains(TestResults& results, HaystackIterator begin, HaystackIterator end, Needle needle, TestDetails const& details)
{
    if (std::find(begin, end, needle) == end)
    {
        UnitTest::MemoryOutStream message;

        // Make sure the message starts with the text "Expected" so that the ConsoleTestReporter will print it.
        message << "Expected to find '" << needle << "', but it was not there.\n";
        message << "Collection count: " << std::distance(begin, end) << std::endl;
        message << "The first few:\n";
        int i = 0;
        HaystackIterator it = begin;
        while (it != end && i < 3)
        {
            message << "\t'" << *it << "'\n";
            ++it;
            ++i;
        }

        results.OnTestFailure(details, message.GetText());
        return false;
    }
    return true;
}

template<typename HaystackIterator, typename Needle>
bool CheckContains(TestResults& results, HaystackIterator begin, HaystackIterator end, Needle needle, TestDetails const& details)
{
    if (std::find(begin, end, needle) == end)
    {
        UnitTest::MemoryOutStream message;

        // Make sure the message starts with the text "Expected" so that the ConsoleTestReporter will print it.
        message << "Expected to find " << Stringify(needle, "given value") << " in the range of " << std::distance(begin, end) << " elements, but it was not there.";
		if (begin != end && CanStringify(*begin))
		{
			message << std::endl << "\tThe first few:" << std::endl;
			HaystackIterator it = begin;
			for (int i = 0; it != end && i < 3; ++it, ++i)
			{
				message << "\t\t" << Stringify(*it) << "\n";
			}
		}

        results.OnTestFailure(details, message.GetText());
        return false;
    }
    return true;
}

}

#endif
