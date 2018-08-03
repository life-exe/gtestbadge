#include "BadgeEventListener.h"
#include "SvgTemplate.h"

int strReplace(std::string & iString, const char * iOldValue, const char * iNewValue)
{
  std::string tmpOldValue = iOldValue;
  std::string tmpNewValue = iNewValue;

  int numOccurance = 0;

  if (tmpOldValue.size() > 0)
  {
    size_t startPos = 0;    
    size_t findPos = std::string::npos;
    do
    {
      findPos = iString.find(tmpOldValue, startPos);
      if (findPos != std::string::npos)
      {
        iString.replace(findPos, tmpOldValue.length(), tmpNewValue);
        startPos = findPos + tmpNewValue.length();
        numOccurance++;
      }
    }
    while (findPos != -1);
  }
  return numOccurance;
}

int strReplace(std::string & iString, const std::string & iOldValue, const std::string & iNewValue)
{
  return strReplace(iString, iOldValue.c_str(), iNewValue.c_str());
}

std::string toString(const int & value)
{
  #pragma warning( push )
  #pragma warning( disable: 4996 ) //warning C4996: 'itoa': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _itoa. See online help for details.
  char tmp[1024];
  std::string str = itoa(value, tmp, 10);
  #pragma warning( pop )
  return str;
}

BadgeEventListener::BadgeEventListener()
{
  mOutputFilename = "badge.svg";
}

BadgeEventListener::~BadgeEventListener()
{
}

void BadgeEventListener::OnTestProgramStart(const UnitTest& unit_test)
{
}

void BadgeEventListener::OnTestIterationStart(const UnitTest& unit_test, int iteration)
{
}

void BadgeEventListener::OnEnvironmentsSetUpStart(const UnitTest& unit_test)
{
}

void BadgeEventListener::OnEnvironmentsSetUpEnd(const UnitTest& unit_test)
{
}

void BadgeEventListener::OnTestCaseStart(const TestCase& test_case)
{
}

void BadgeEventListener::OnTestStart(const TestInfo& test_info)
{
}

void BadgeEventListener::OnTestPartResult(const TestPartResult& test_part_result)
{
}

void BadgeEventListener::OnTestEnd(const TestInfo& test_info)
{
}

void BadgeEventListener::OnTestCaseEnd(const TestCase& test_case)
{
}

void BadgeEventListener::OnEnvironmentsTearDownStart(const UnitTest& unit_test)
{
}

void BadgeEventListener::OnEnvironmentsTearDownEnd(const UnitTest& unit_test)
{
}

void BadgeEventListener::OnTestIterationEnd(const UnitTest& unit_test, int iteration)
{
}

void BadgeEventListener::OnTestProgramEnd(const UnitTest& unit_test)
{
  int numDisabled = unit_test.disabled_test_count();
  int numFailed = unit_test.failed_test_count();
  int numSuccess = unit_test.successful_test_count();
  int numRun = unit_test.test_to_run_count();
  int numTotalTests = unit_test.total_test_count();

  bool success = generateBadge(mOutputFilename, numSuccess, numFailed, numDisabled, ICON_NONE);
}

void BadgeEventListener::setOutputFilename(const std::string & iFilename)
{
  mOutputFilename = iFilename;
}

bool BadgeEventListener::generateBadge(const std::string & iFilename, int success, int failures, int disabled, const SYSTEM_ICON & iIcon)
{
  int total = success + failures;

  //define badge color & right_text
  char * color = NULL;
  bool showSuccess = true;
  std::string right_text;
  size_t numDigits = 0;
  const double failureRatio = double(failures)/double(total);
  if (failures == 0)
  {
    color = "#97CA00"; //green
    right_text = toString(success);
    numDigits = right_text.size();
    right_text += " passed";
    showSuccess = true;
  }
  else if (failureRatio >= 0.10)
  {
    color = "#e05d44"; //red
    right_text = toString(failures);
    numDigits = right_text.size();
    right_text += " failures";
    showSuccess = false;
  }
  else
  {
    color = "#fe7d37"; //orange
    right_text = toString(failures);
    numDigits = right_text.size();
    right_text += " failures";
    showSuccess = false;
  }

  //define width
  int width = 0;
  int right_text_length = 0;
  int right_text_x = 0;
  if (showSuccess)
  {
    //right_text_x      = 36 * numDigits + 620;
    //right_text_length = 72 * numDigits + 420;
    //width             = (int)(7.2*double(numDigits) + 89.0);

    right_text_x      = (int)(36.667*double(numDigits) + 618.33 + 0.01);
    right_text_length = (int)(73.333*double(numDigits) + 416.67 + 0.01);
    width             = (int)(7.3333*double(numDigits) + 88.667 + 0.01);
  }
  else
  {
    //right_text_x      = 36 * numDigits + 630;
    //right_text_length = 72 * numDigits + 440;
    //width             = (int)(7.2*double(numDigits) + 91.0);

    right_text_x      = (int)(36.667*double(numDigits) + 628.33 + 0.01);
    right_text_length = (int)(73.333*double(numDigits) + 436.67 + 0.01);
    width             = (int)(7.3333*double(numDigits) + 90.667 + 0.01);
  }

  //define icon
  static const std::string icon_none = "";
  static const std::string icon_appveyor = "    <image x=\"5\" y=\"3\" width=\"14\" height=\"14\" xlink:href=\"data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI0MCIgaGVpZ2h0PSI0MCIgdmlld0JveD0iMCAwIDQwIDQwIj4NCjxwYXRoIGZpbGw9IiNkZGQiIGQ9Ik0yMCAwYzExIDAgMjAgOSAyMCAyMHMtOSAyMC0yMCAyMFMwIDMxIDAgMjAgOSAwIDIwIDB6bTQuOSAyMy45YzIuMi0yLjggMS45LTYuOC0uOS04LjktMi43LTIuMS02LjctMS42LTkgMS4yLTIuMiAyLjgtMS45IDYuOC45IDguOSAyLjggMi4xIDYuOCAxLjYgOS0xLjJ6bS0xMC43IDEzYzEuMi41IDMuOCAxIDUuMSAxTDI4IDI1LjNjMi44LTQuMiAyLjEtOS45LTEuOC0xMy0zLjUtMi44LTguNC0yLjctMTEuOSAwTDIuMiAyMS42Yy4zIDMuMiAxLjIgNC44IDEuMiA0LjlsNi45LTcuNWMtLjUgMy4zLjcgNi43IDMuNSA4LjggMi40IDEuOSA1LjMgMi40IDguMSAxLjhsLTcuNyA3LjN6Ii8+DQo8L3N2Zz4=\"/>";
  static const std::string icon_travis   = "    <image x=\"5\" y=\"3\" width=\"14\" height=\"14\" xlink:href=\"data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCA0MCA0MCI+PGcgZmlsbD0iI2ZmZiI+PHBhdGggZD0iTTI2LjIyMyAyMy44NDVjLjA0OCAxLjMzMy43NjEgMS4zMjMgMS42MDkgMS4yOTIgMS4xNDItLjAyMSAxLjc2OS0uMzcxIDIuMzQyLS4wOTUtLjAwOS0uMDMyLS4xODgtLjU2Ny0uOTc5LS40NzQuMDkyLS4xOTguMTMxLS40NjYuMTAzLS44MzQtLjEwNC0xLjMzMi0uODc5LTIuMzktMS43MjktMi4zNi0uODQ4LjAyOS0xLjM5MyAxLjEzNS0xLjM0NiAyLjQ3MXptMS4yNjYtMS42MjVjLjIzOS0uMDA1LjQzOC4xODYuNDQxLjQyNi4wMDUuMjM5LS4xODcuNDM4LS40MjUuNDQyLS4yNC4wMDQtLjQzOC0uMTg4LS40NDItLjQyOC0uMDA1LS4yMzkuMTg3LS40MzYuNDI2LS40NHptLTEyLjkwOCAzLjE1N2MuMDg4LjAwMi4xOCAwIC4yNzQtLjAwMi4wODIgMCAuMTY0LS4wMDIuMjUtLjAwNS44NTEtLjAwOCAxLjU1Ni4wNzIgMS41NDUtMS4yNjItLjAxNC0xLjMzNC0uNjA2LTIuNDE0LTEuNDU3LTIuNDA1LS44NDkuMDA4LTEuNjcxIDEuMTAyLTEuNjIyIDIuNDM1LjAxNS4zNDguMDY2LjU5OC4xNDguNzc5LS43NC4xODctLjk0NS42NDQtLjk1Ny42NzMuNDY2LS4yNTEuOTkzLS4yMTUgMS44MTktLjIxM3ptLjczMS0yLjgzNWMuMjQtLjAwOC40MzkuMTgyLjQ0Ny40MjIuMDA2LjIzOS0uMTg0LjQzOS0uNDIzLjQ0NS0uMjQuMDA4LS40NC0uMTgzLS40NDYtLjQyMi0uMDA3LS4yMzkuMTgyLS40NC40MjItLjQ0NXptMS45OTctMTQuMjE1aDIuMzg2di0uOTg0aC41Mzd2NC41NDZoLS44MTF2Mi41NDNoNC4xNzd2LTIuNTQzaC0uODEyVjcuMzQzaC41Mzh2Ljk4NGgyLjM4OFY0Ljk1OGgtOC40MDMiLz48cGF0aCBkPSJNMzkuNjg5IDE0LjgyM2MtLjAxNS0uMjg5LS4wNDMtLjU3Ny0uMDgyLS44NjEtLjAyLS4xNDQtLjA0MS0uMjg4LS4wNzEtLjQyOS0uMDI4LS4xNDYtLjA1Ni0uMjgxLS4xMDMtLjQ0bC0uMDMtLjExMi0uMDc5LS4wNjNjLS40NTUtLjM1Ni0uOTQ0LS42MjYtMS40NDEtLjg2NC0uNDExLS4xOTQtLjgyOC0uMzYzLTEuMjQ5LS41MTgtMS4xNzctMy4yNjQtMy4xOTctNi4xNTgtNS44NTYtOC4xNzNDMjguMDMxIDEuMjc5IDI0LjguMTc2IDIxLjQzNi4xNzZjLTMuMzY2IDAtNi41OTYgMS4xMDMtOS4zNDQgMy4xODctMi4wOTMgMS41ODYtMy43ODggMy43MTgtNC45ODkgNi4xNDVoLS4wMDZjLS4zMjUuNjU3LS42MTcgMS4zMzMtLjg2NyAyLjAyOC0uNDIyLjE1NS0uODM5LjMyNC0xLjI1LjUxOC0uNDk4LjIzOC0uOTg2LjUwOC0xLjQ0MS44NjRsLS4wOC4wNjMtLjAzLjExMmMtLjA0Ni4xNTktLjA3Ni4yOTQtLjEwNC40NC0uMDI4LjE0MS0uMDUxLjI4NS0uMDcuNDI5LS4wMjkuMjA1LS4wNDguNDEzLS4wNjUuNjIxaC4wMDdjLS4wMDcuMDgtLjAxMy4xNi0uMDE3LjI0MS0uMDI5LjU3Ny0uMDEzIDEuMTU0LjA1IDEuNzI4LjA2MS41NzMuMTY3IDEuMTQuMzIgMS43MDMuMDguMjguMTcxLjU1NS4yNzkuODMuMDUzLjEzNy4xMTEuMjczLjE3Ni40MS4wMzIuMDY4LjA2Ny4xMzUuMTAyLjIwNGwuMDU3LjEwMWMuMDIyLjAzOC4wMzcuMDY1LjA3LjExNGwuMDU3LjA5LjA5Mi4wNDdjLjA5Ny4wNS4xNzguMDg3LjI2Ni4xMjlsLjI1Ni4xMTZjLjA0Ny4wMTkuMDkzLjAzOS4xNDIuMDU5LTEuMTY3IDEuMDc4LTIuNDM1IDIuNDc5LTMuNTMyIDQuMjExTC4xMyAyNi43NDlsMS45NDUtMS43MDdjLjAzNC0uMDI5IDEuNjY0LTEuNDQgNC4zODQtMy4wNDFsLjAzOC4zMTMuMDk5Ljc4OS40OTggNC4wNDdjLjAxMS4wNzcuMDUxLjE0Ni4xMTUuMTlsMS4wMjEuNzE3Yy4wMTguMDU3LjAzNS4xMTQuMDUyLjE3My4wMjQuMDgxLjA0Ni4xNjEuMDcxLjI0MmwuMjMzLjcwM2MuNTExIDEuNTg5IDEuMjM1IDMuMDIyIDIuMTE2IDQuMjgxLS4wNDIuMDEzLS4wODUuMDI1LS4xMjcuMDM3LS41MzEtLjI0Ni0yLjM4Ni0xLjEwNi0zLjAwOS0xLjM4N2wtMS4zMDMtLjU4NS43NzEgMS4yMDNjLjA1Ni4wODcgMS4zODYgMi4xNjEgMi40NzEgMy41NTkgMS4wMTcgMS4zMDUgMi40MDggMi42MzcgNS4wMzIgMi42MzcuMzE1IDAgLjY0Ny0uMDIuOTg3LS4wNTYuNzgzLS4wOTEgMS4zNzEtLjE2MSAxLjgxOC0uMjE5IDIuNzU0IDEuMDE0IDUuNzkyIDEuMDc0IDguNjc1LS4wMTcuNzktLjMgMS41MDktLjY2MyAyLjE2OC0xLjA3MS4wMTQtLjAwMi4wMjctLjAwNi4wMzktLjAwOSAxLjA3Mi0uMjU4IDIuMjg5LS41NDkgMi44OS0uNzE3LjA5Ni0uMDI2LjIxNy0uMDU1LjM0Mi0uMDg1LjgzNy0uMTkxIDIuMjM2LS41MTcgMi45ODEtMi4wMDkuNzUxLTEuNTA2IDEuOTc3LTMuODEyIDEuOTg5LTMuODM1TDM3IDI5LjgyOWwtMS4xMTUuNDg1Yy0uMDYxLjAyNi0xLjQ3NS42NDMtMi4yMSAxLjAyOC0uMDI1LjAxNS0uMDU3LjAyOC0uMDg2LjA0Mi41OTgtMS4yMTkuODk2LTIuMTkgMS4wMDMtMi41NjNsLjQwMy0xLjY2MyAxLjE2MS0yLjU5N3MuMzI2LTIuODM5LjMzMy0yLjgzN2wuMTE0LTEuMDg2Yy4xMDItLjA0My43MjItLjA4Ni44NDMtLjEzNC4xNzMtLjA2OC4zNDYtLjEzNS41MTktLjIxbC4yNTUtLjExNmMuMDktLjA0Mi4xNy0uMDc5LjI2Ny0uMTI5bC4wOTItLjA0Ny4wNTgtLjA5Yy4wMzItLjA0OS4wNS0uMDc3LjA2OS0uMTE0bC4wNTYtLjEwMWMuMDM5LS4wNjkuMDcyLS4xMzYuMTAzLS4yMDQuMDY1LS4xMzYuMTIyLS4yNzIuMTc4LS40MS4xMDctLjI3NS4xOTYtLjU1LjI3Ny0uODMuMTUyLS41NjMuMjYyLTEuMTMuMzItMS43MDMuMDY0LS41NzIuMDc4LTEuMTUuMDQ5LTEuNzI3ek0xMy4xOTUgMjEuODNjLjAyNS0uMDE3IDEuMDY4LS43MDEgMi43NzEtMS41MzQgMS42OTgtLjEyOSAzLjU3MS0uMjExIDUuNTc4LS4yMTEuNDg4IDAgLjk2Ny4wMDcgMS40NC4wMTUtLjMzLjI2Ni0uNjc4LjUxNy0xLjA0LjczNGwtMS4yMDMuNzI4Yy0uNzU5LjA5Mi0xLjQxNy4yODEtMS40NTMuMjkxLS4wMjMuMDA2LS4wNDQuMDE2LS4wNjMuMDI3LS4wNTkuMDM0LS4xMDQuMDktLjEyNC4xNTZsLTEuMjMgNC4wNTEtNi41NjUgMS40NTktMi4wNjktMS40NDktLjYwMi00Ljg3M2MxLjE3NC0uMjE3IDIuNzQ2LS40NjYgNC42MzItLjY3Ny0uMjAyLjI0LS4zOTkuNDk0LS41ODQuNzYybC0xLjEyMyAxLjYxMiAxLjYzNS0xLjA5MXptOC44NiAxNi45NTRjLTEuMDgzLjA3Mi0yLjE4My0uMDU1LTMuMjYtLjM1LjA5LS4wMTguMTU1LS4wMjguMjM1LS4wMzcuMDg4LS4wMTIgMi4wMjEtLjI2NiAyLjg5LTEuNTU4bC4wODIuMDAyLjIxNi0uMDAyYy4xNDMgMCAuMjczLS4wMTIuNDEyLS4wMTguMzMtLjAyNy42NDktLjA3NS45NjYtLjEzOS4wMzQuMDMzLjA2Ni4wNjYuMTAxLjEwMi42NTMuNjQ3IDEuNDY0IDEuMDE4IDIuMzU1IDEuMDgzLTEuNDk5LjYwOS0yLjkxLjg0NS0zLjk5Ny45MTd6bS4wOTgtMi4zN2MuMTYzLS4zNjMuMjg2LS42NDYuMzc5LS44NTkuMTkyLjIxNC40MzUuNDc5LjcwOC43NjItLjIxMy4wMzItLjQyMy4wNTktLjYzLjA3Ni0uMTM2LjAwNi0uMjc4LjAyMS0uNDA4LjAyMWgtLjA0OXptLTMuMjIxLTguMzYybC41ODctLjEzMmMuMDk3LS4wMjEuMTczLS4wOTQuMjAyLS4xODhsMS4yMjYtNC4wMzNjLjIxMi0uMDI4LjM5OC0uMDQzLjU1OS0uMDQzLjE1NiAwIC4zNDUuMDE1LjU1Ni4wNDNsMS4yNjIgNC4xNTNjLjAzMi4xMDYuMTIzLjE4My4yMzEuMTk1bC40NjguMDUzLjEyLjAxMyA4LjA2Mi45MTdjLjA2Ny4wMDguMTMzLS4wMTEuMTg4LS4wNDlsLjgxLS41NjYuNzQ5LS41MjQuMjc3LS4xOTVjLS4wNDUuMTkzLS4wOTUuMzg1LS4xNDcuNTc0LS4wNzcuMjQ4LS4xNTkuNDg4LS4yNDEuNzI2LS4zMzEuOTQyLS43MDIgMS43OTctMS4xMDYgMi41NjktLjExLjAwNy0uMjI5LjAxMi0uMzU0LjAxMi0uMzkyIDAtLjc5LS4wMzItMS4yMzEtLjA3My0uNzY2LS4wNzEtMS45NzktLjIxLTIuMzYxLS4yNTMtLjMzMy0uMjY1LTEuNDY0LTEuMTYyLTIuNDAzLTEuOTAzLS4wMzYtLjAyOC0uMDY4LS4wNTgtLjEwMS0uMDg0LS4xNjUtLjE0LS4zNTMtLjI5OC0uNzM0LS4yOTgtLjM0OCAwLS44NTQuMTI2LTIuMDc3LjQ5NS42MzctLjQ3Ni42NDItMS4wODkuNjQyLTEuMDg5cy0uOTA2Ljg0NC0yLjM2OCAxLjAzYy0xLjQ1OC4xODgtMi42MDUtLjcwOS0yLjYwNS0uNzA5LjA0OC4xNTYuMTE2LjUwOS40NDcuODA4LS4zODktLjA0NS0uODA5LS4wNzctMS4yNS0uMDc3LS4xNTggMC0uMzIuMDA0LS40NzUuMDEyLS44OTguMDQ4LTIuMTYyIDEuNDA0LTMuMDQ5IDIuNzczLS40MzYuMTM4LTEuNzk5LjU3MS0zLjM3NSAxLjAzNC0uOTMxLTEuMjg4LTEuNTYyLTIuNTk1LTEuOTE5LTMuNDM2LS4yMDgtLjQ5LS4zMjQtLjgyMy0uMzUtLjkwMnYtLjAwMmMtLjAyOS0uMDgtLjA2LS4xNjUtLjA4Ny0uMjVsLjM3OC4yNjR2LjAwMmwuOTM1LjY1Ni4zMTIuMjE3Yy4wNDcuMDMzLjEwNC4wNS4xNTguMDUuMDIgMCAuMDQtLjAwMi4wNjEtLjAwNm0xMi45NzctNy44Yy0uMDI4LS4wOS0uMDk4LS4xNTctLjE4OC0uMTg0LS4wNDEtLjAxMy0uODUyLS4yNDQtMS43MTMtLjMxN2wuMTU4LS4wMTJjLjEzNi0uMDEgMi4zOTctLjI4NiA1LjA5OC0xLjIxOCAyLjk5MS4yMjkgNS40MzMuNTk2IDcuMTAzLjlsLS41OTEgNC4xNDYtMi4wODYgMS40NTktNi41NTctLjc0NW0xMy4xNzEtOS42MTRjLS4wNzMuNTM0LS4zMTUgMS41NzQtLjMxNSAxLjU3NGwtLjE4OS4wMzFjLS4wOTktLjAyNy0xLjAyMS0uMjY2LTIuNjA5LS41Ni4wNTEtLjAxMS4xLS4wMjIuMTQ4LS4wMzMuMzI5LS4wODIuNjUzLS4xODMuOTY4LS4zMS4xNTYtLjA2Ny4zMTItLjEzOC40NTktLjIyMi4xNDYtLjA4NS4yODktLjE3OS40MDEtLjMwNi0xLjU5LjUyLTQuODc2LjI4NC03Ljc5Ny0uMDE4LTIuNjY5LS4yNzYtNS4zNDgtLjQ1NC04LjA0LS40Ni0yLjY5MS4wMDYtNS4zNjcuMTg1LTguMDM5LjQ2LTIuOTIxLjMwMi02LjIwNi41MzgtNy43OTYuMDE4LjExMi4xMjYuMjU2LjIyMS40MDMuMzA2LjE0Ni4wODQuMzAxLjE1NC40NTcuMjIyLjMxNC4xMjYuNjQuMjI4Ljk2OS4zMS4xMTYuMDI3LjIzNC4wNTMuMzUyLjA3NC0xLjQ0OS4yNzUtMi4yODguNDkzLTIuMzgyLjUxOSAwIDAtLjMzNC4wNTQtLjQwNy0uMjAyLS4xNDUtLjUxMi0uMjQyLS44Ny0uMzE1LTEuNDAzLS4wNjktLjUzMS0uMTA0LTEuMDY4LS4xLTEuNjA0LjAwMS0uMDg4LS4wMDEtLjI2NC0uMDAxLS4yNjQuMDA1LS4xOC4wMTMtLjM2LjAzLS41NC4wMS0uMTMzLjQ5OS0uNTU5LjkxOS0uODAxLjQ1NC0uMjY0LjkzMS0uNTAyIDEuNDE3LS43MjMuNDgxLS4yMjggMi4wNC0uNTU5IDIuOTgtLjcyOS45NC0uMTcyIDQuMzAyLS42MTggNC41NTQtLjcxMi4yNTEtLjA5NiAxLjcwNC0xLjIxNiAxLjk1OC0xLjMwOS0uNTM0LjA4OC03LjkxMiAxLjIxNS04LjI0NCAxLjMyIDIuMzk1LTYuMjYzIDcuMDk2LTkuMzEzIDEzLjM1My05LjMxMyA2LjI1NSAwIDEwLjk1NyAyLjg4IDEzLjM1NCA5LjE0NC0zLjE2NC0uODQxLTcuNTQ2LTEuMjMzLTguMDgtMS4zMjIuMjU0LjA5MyAxLjY5MSAxLjEwNCAxLjk1OCAxLjEzOCAyLjE4OC4yODQgNS42MzggMS4wNDkgNS44ODggMS4xNDguNDk2LjIwMS45OTMuNDA4IDEuNDc1LjYzNS40ODcuMjIxLjk2My40NTkgMS40MTcuNzIzLjQyLjI0My45MDguNjY4LjkyLjgwMS4wMjMuMjY4LjAzMy41MzUuMDM2LjgwMy4wMDQuNTM2LS4wMzMgMS4wNzQtLjEwMiAxLjYwNXoiLz48L2c+PC9zdmc+\"/>";
  std::string icon = icon_none;
  switch(iIcon)
  {
  case ICON_APPVEYOR:
    icon = icon_appveyor;
    width += 17;
    break;
  case ICON_TRAVIS:
    icon = icon_travis;
    width += 17;
    break;
  case ICON_NONE:
  default:
    icon = icon_none;
    break;
  };

  //define text_x_offset
  int text_x_offset = 0;
  switch(iIcon)
  {
  case ICON_APPVEYOR:
  case ICON_TRAVIS:
    text_x_offset = 170;
    break;
  };

  const bin2cpp::File & svgTemplate = bin2cpp::getSvgTemplateFile();
  std::string svg = svgTemplate.getBuffer();

  //process with search and replace
  strReplace(svg, "{width}", toString(width));
  strReplace(svg, "{right.color}", color);
  strReplace(svg, "{left.text.x}", toString(195+text_x_offset));
  strReplace(svg, "{right.text.x}", toString(right_text_x+text_x_offset));
  strReplace(svg, "{right.text.length}", toString(right_text_length+text_x_offset));
  strReplace(svg, "{left.text}", "tests");
  strReplace(svg, "{right.text}", right_text);
  if (icon.size() > 0)
  {
    icon.append("\n");
    strReplace(svg, "{icon}", icon.c_str());
  }
  else
  {
    strReplace(svg, "{icon}", "");
  }

  //dump to file
  FILE * f = fopen(iFilename.c_str(), "w");
  if (!f)
    return false;

  //write template
  fputs(svg.c_str(), f);
  fclose(f);

  return true;
}