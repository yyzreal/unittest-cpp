#include "Checks.h"
#include <cstring>

namespace UnitTest {

namespace {

bool CheckStringsEqual(TestResults& results, char const* expected, char const* actual,
                       TestDetails const& details)
{
	using namespace std;

	// Before calling strcmp, just compare the pointers - it may be an optimization, but it also means CHECK() for NULL strings can pass without crashing.
    if ((expected != actual) && strcmp(expected, actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << expected << " but was " << actual;

        results.OnTestFailure(details, stream.GetText());
        return false;
    }
    return true;
}

}


bool CheckEqual(TestResults& results, char const* expected, char const* actual,
                TestDetails const& details)
{
    return CheckStringsEqual(results, expected, actual, details);
}

bool CheckEqual(TestResults& results, char* expected, char* actual,
                TestDetails const& details)
{
    return CheckStringsEqual(results, expected, actual, details);
}

bool CheckEqual(TestResults& results, char* expected, char const* actual,
                TestDetails const& details)
{
    return CheckStringsEqual(results, expected, actual, details);
}

bool CheckEqual(TestResults& results, char const* expected, char* actual,
                TestDetails const& details)
{
    return CheckStringsEqual(results, expected, actual, details);
}


}
