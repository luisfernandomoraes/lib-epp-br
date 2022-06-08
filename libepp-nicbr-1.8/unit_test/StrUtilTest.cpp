/* 
 * Copyright (C) 2009 Registro.br. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistribution of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY REGISTRO.BR ``AS IS AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIE OF FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL REGISTRO.BR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
/* $Id: StrUtilTest.cpp 1045 2009-10-30 16:31:50Z eduardo $ */
#include <string>

#include "libepp_nicbr.H"

#include "StrUtilTest.H"
#include "StrUtil.H"
#include "XmlException.H"

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(StrUtilTest);

StrUtilTest::StrUtilTest() {}

StrUtilTest::~StrUtilTest() {}

void StrUtilTest::setUp() {}

void StrUtilTest::tearDown() {}

void StrUtilTest::parse_test() 
{
  string to_be_parsed = "<command>"
    "<login>"
      "<clID>$(clID)$</clID>"
      "<pw>$(pw)$</pw>"
      "<newPW>$(newPW)$</newPW>"
      "<options>"
        "<version>1.0</version>"
        "<lang>pt</lang>"
      "</options>"
      "<svcs>x1 "
        "<objURI>urn:ietf:params:xml:ns:domain</objURI>"
        "<objURI>urn:ietf:params:xml:ns:contact</objURI>"
	"<objURI>urn:ietf:params:xml:ns:organization</objURI>"
        "<svcExtension>"
          "<extURI>http://registro.br/epp/domainext-1.0</extURI>"
        "</svcExtension>"
      "</svcs>"
    "</login>"
    "<clTRID>$(clTRID)$</clTRID>"
  "</command>";

  map < string, string, less<string> > to_parse;
  to_parse["clID"] = "ClientX";
  to_parse["pw"] = "foo-BAR2";
  to_parse["newPW"] = "bar-FOO2";
  to_parse["clTRID"] = "ABC-12345";

  string parsed = StrUtil::parse(to_be_parsed, to_parse, "$(", ")$");

  string expected = "<command>"
    "<login>"
      "<clID>ClientX</clID>"
      "<pw>foo-BAR2</pw>"
      "<newPW>bar-FOO2</newPW>"
      "<options>"
        "<version>1.0</version>"
        "<lang>pt</lang>"
      "</options>"
      "<svcs>x1 "
        "<objURI>urn:ietf:params:xml:ns:domain</objURI>"
        "<objURI>urn:ietf:params:xml:ns:contact</objURI>"
	"<objURI>urn:ietf:params:xml:ns:organization</objURI>"
        "<svcExtension>"
          "<extURI>http://registro.br/epp/domainext-1.0</extURI>"
        "</svcExtension>"
      "</svcs>"
    "</login>"
    "<clTRID>ABC-12345</clTRID>"
  "</command>";

  CPPUNIT_ASSERT(expected == parsed);
}

void StrUtilTest::gsub_test() 
{
  string input_txt = "<command>"
     "<login>"
       "<clID>$(clID)$</clID>"
     "</login>"
    "<clTRID>$(clID)$</clTRID>"
   "</command>";

  string pat = "$(clID)$"; 
  string drp = "ClientX";
  string output_txt = input_txt;

  StrUtil::gsub(output_txt, pat.c_str(), drp.c_str());

  string expected_output = "<command>"
    "<login>"
      "<clID>ClientX</clID>"
    "</login>"
   "<clTRID>ClientX</clTRID>"
  "</command>";

  CPPUNIT_ASSERT(expected_output == output_txt);
}

void StrUtilTest::to_string_test()
{
  unsigned int my_int;
  string my_str;
  float my_float;

  my_int = 1024;
  my_str = "1024";
  CPPUNIT_ASSERT(StrUtil::to_string("%u", my_int) == my_str);

  my_int = 0;
  my_str = "0";
  CPPUNIT_ASSERT(StrUtil::to_string("%u", my_int) == my_str);
  my_str = "1";
  CPPUNIT_ASSERT(StrUtil::to_string("%u", my_int) != my_str);

  my_float = 154.10f;
  my_str = "154.10";
  CPPUNIT_ASSERT(StrUtil::to_string("%.2f", my_float) == my_str);
}

void StrUtilTest::iso88591_to_utf8_test()
{
  unsigned char iso88591[256], utf8_compare[512];
  for (int i = 1, j = 0; i < 256; i++, j++) {
    iso88591[i-1] = i;
    if (i < 0x20 || (i >= 0x7F && i < 0xA0)) {
      utf8_compare[j] = ' ';
    } else if (i < 0x7F) {
      utf8_compare[j] = i;
    } else {
      utf8_compare[j] = (0xC0 | (i >> 6));
      j++;
      utf8_compare[j] = (0x80 | (0x3F & i));
    }
    utf8_compare[j+1] = 0;
  }
  iso88591[255] = 0;
  string utf8;
  int ret = StrUtil::iso88591_to_utf8(string((char *)iso88591), utf8);
  CPPUNIT_ASSERT_EQUAL(ret, 64);
  CPPUNIT_ASSERT(string((char *)utf8_compare) == utf8);
}

void StrUtilTest::utf8_to_iso88591_test()
{
  unsigned char iso88591[256], iso88591_compare[256];
  for (int i = 1; i < 256; i++) {
    iso88591[i-1] = i;
    if (i < 0x20 || (i >= 0x7F && i < 0xA0)) {
      iso88591_compare[i-1] = ' ';
    } else {
      iso88591_compare[i-1] = i;
    }
  }
  iso88591[255] = 0;
  iso88591_compare[255] = 0;

  string utf8;
  int ret = StrUtil::iso88591_to_utf8(string((char *)iso88591), utf8);
  CPPUNIT_ASSERT_EQUAL(ret, 64);

  string iso;
  ret = StrUtil::utf8_to_iso88591(utf8, iso);
  CPPUNIT_ASSERT_EQUAL(ret, 0);
  CPPUNIT_ASSERT_EQUAL(string((char *)iso88591_compare), iso);
}

void StrUtilTest::xml_beautifier_test()
{
  string input_txt = 
    "<command>"
    "<login>"
    "<clID>Client X</clID>"
    "</login>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>";
  string expected_output = 
    "<command>\n"
    " <login>\n"
    "  <clID>Client X</clID>\n"
    " </login>\n"
    " <clTRID>ABC-12345</clTRID>\n"
    "</command>";
  string output_txt;
  StrUtil str_util;

  CPPUNIT_ASSERT_NO_THROW(output_txt = str_util.xml_beautifier(input_txt));
  CPPUNIT_ASSERT_EQUAL(expected_output, output_txt);
  input_txt = 
    "<command>"
    "<logout/>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>";
  CPPUNIT_ASSERT_NO_THROW(output_txt = str_util.xml_beautifier(input_txt));

  expected_output = 
    "<command>\n"
    " <logout/>\n"
    " <clTRID>ABC-12345</clTRID>\n"
    "</command>";
  CPPUNIT_ASSERT_EQUAL(expected_output, output_txt);
}

void StrUtilTest::xml_escape_test()
{
  string input_txt = "&'><\"";
  string output_txt;
  StrUtil str_util;
  try {
    output_txt = str_util.esc_xml_markup(input_txt);
  } catch (const XmlException &e) {
    CPPUNIT_ASSERT(false);
  }
  string expected_output = "&amp;&apos;&gt;&lt;&quot;";
  CPPUNIT_ASSERT_EQUAL(expected_output, output_txt);
}
