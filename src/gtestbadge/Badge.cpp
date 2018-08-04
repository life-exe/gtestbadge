#include "Badge.h"
#include "BadgeTemplate.h"
#include "verdana.h"
#include "rapid.h"

int getVerdanaTextWidth(const std::string & iStr, const int iFontSize)
{
  if (iFontSize <= 0 || iFontSize >= VERDANA_NUM_FONT_SIZES)
    return 0; //not supported

  int width = 0;
  for(size_t i=0; i<iStr.size(); i++)
  {
    char c = iStr[i];
    if (c >= 0 && c < VERDANA_NUM_CHARACTERS)
    {
      int charWidth = VERDANA_CHAR_WIDTH[iFontSize][c];
      width += charWidth;
    }
  }

  return width;
}

Badge::Badge()
{
  mHeight = 20;

  mLeft.back_color = "#555";
  mLeft.front_color = "#fff";
  mLeft.font_size = 11;
  mLeft.text = "left";
  mLeft.width = Badge::WIDTH_AUTO;
  mLeft.text_left_padding = 1;
  mLeft.text_right_padding = 1;

  mRight = mLeft;
  mRight.back_color = "#c0c0c0";
  mRight.text = "right";
}

Badge::~Badge()
{
}

void Badge::setHeight(int size)
{
  mHeight = size;
}

void Badge::setLeftText(const std::string & text)
{
  mLeft.text = text;
}

void Badge::setLeftWidth(int width)
{
  mLeft.width = width;
}

void Badge::setLeftBackgroundColor(const std::string & color)
{
  mLeft.back_color = color;
}

void Badge::setLeftForegroundColor(const std::string & color)
{
  mLeft.front_color = color;
}

void Badge::setRightText(const std::string & text)
{
  mRight.text = text;
}

void Badge::setRightWidth(int width)
{
  mRight.width = width;
}

void Badge::setRightBackgroundColor(const std::string & color)
{
  mRight.back_color = color;
}

void Badge::setRightForegroundColor(const std::string & color)
{
  mRight.front_color = color;
}

void Badge::setLeftTextLeftPadding(int size)
{
  mLeft.text_left_padding = size;
}

void Badge::setLeftTextRightPadding(int size)
{
  mLeft.text_right_padding = size;
}

void Badge::setRightTextLeftPadding(int size)
{
  mRight.text_left_padding = size;
}

void Badge::setRightTextRightPadding(int size)
{
  mRight.text_right_padding = size;
}

void Badge::setLeftFontSize(int size)
{
  mLeft.font_size = size;
}

void Badge::setRightFontSize(int size)
{
  mRight.font_size = size;
}

bool Badge::save(const std::string & iFilePath)
{
  FILE * f = fopen(iFilePath.c_str(), "w");
  if (!f)
    return false;

  //compute font height
  int left_font_height = VERDANA_CHAR_HEIGHT[VERDANA_NUM_FONT_SIZES-1];
  if (mLeft.font_size < VERDANA_NUM_FONT_SIZES)
    left_font_height = VERDANA_CHAR_HEIGHT[mLeft.font_size];
  int right_font_height = VERDANA_CHAR_HEIGHT[VERDANA_NUM_FONT_SIZES-1];
  if (mRight.font_size < VERDANA_NUM_FONT_SIZES)
    right_font_height = VERDANA_CHAR_HEIGHT[mRight.font_size];

  //define automatic width
  if (mLeft.width == WIDTH_AUTO)
  {
    mLeft.width = getVerdanaTextWidth(mLeft.text, mLeft.font_size);
  }
  if (mRight.width == WIDTH_AUTO)
  {
    mRight.width = getVerdanaTextWidth(mRight.text, mRight.font_size);
  }
  if (mHeight == HEIGHT_AUTO)
  {
    int max_font_height = (left_font_height > right_font_height ? left_font_height : right_font_height);
    mHeight = max_font_height;
  }

  //define width calculations
  int  left_width = mLeft.width  + mLeft.text_left_padding  + mLeft.text_right_padding;
  int right_width = mRight.width + mRight.text_left_padding + mRight.text_right_padding;
  int total_width = left_width + right_width;

  //define height calculations
  int left_text_height = mHeight/2 + left_font_height/4;
  int right_text_height = mHeight/2 + right_font_height/4;
  int left_shadow_height = left_text_height+1;
  int right_shadow_height = right_text_height+1;

  std::string svg = bin2cpp::getBadgeTemplateFile().getBuffer();

  //remove CRLF from file
  strReplace(svg, "\r\n", "\n");

  //process with search and replace
  strReplace(svg, "{width}", toString(total_width));
  strReplace(svg, "{height}", toString(mHeight));
  strReplace(svg, "{left.width}", toString(left_width));
  strReplace(svg, "{right.width}", toString(right_width));
  strReplace(svg, "{left.back_color}", mLeft.back_color);
  strReplace(svg, "{right.back_color}", mRight.back_color);
  strReplace(svg, "{left.front_color}", mLeft.front_color);
  strReplace(svg, "{right.front_color}", mRight.front_color);
  strReplace(svg, "{left.text.x}",  toString( mLeft.text_left_padding + mLeft.width/2 ));
  strReplace(svg, "{right.text.x}", toString( left_width + mRight.text_left_padding + mRight.width/2 ));
  strReplace(svg, "{left.text}", mLeft.text);
  strReplace(svg, "{right.text}", mRight.text);
  strReplace(svg, "{left.font_size}", toString(mLeft.font_size));
  strReplace(svg, "{right.font_size}", toString(mRight.font_size));
  strReplace(svg, "{left.shadow.height}", toString(left_shadow_height));
  strReplace(svg, "{right.shadow.height}", toString(right_shadow_height));
  strReplace(svg, "{left.text.height}", toString(left_text_height));
  strReplace(svg, "{right.text.height}", toString(right_text_height));

  //if (icon.size() > 0)
  //{
  //  icon.append("\n");
  //  strReplace(svg, "{icon}", icon.c_str());
  //}
  //else
  {
    strReplace(svg, "{icon}", "");
  }

  //write template to file
  fputs(svg.c_str(), f);
  fclose(f);

  return true;
}