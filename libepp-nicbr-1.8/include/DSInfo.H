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
/* $Id: DSInfo.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file DSInfo.H
 *  @brief EPP/DNSSEC DS information class
 */

#ifndef __DSINFO_H__
#define __DSINFO_H__

#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

/// DSInfo Class
class DSInfo {
public:
  struct KeyData {
    unsigned int _flags;
    unsigned int _protocol;
    unsigned int _algorithm;
    string _pub_key;

    KeyData& operator=(const KeyData &key_data) {
      _flags = key_data._flags;
      _protocol = key_data._protocol;
      _algorithm = key_data._algorithm;
      _pub_key = key_data._pub_key;

      return *this;
    }

    KeyData() {
      this->reset();
    }

    bool is_set() {      
      return (_flags != 0 && _protocol != 0 && _algorithm != 0 && 
	      _pub_key != "");
    }

    void reset() {
      _flags = 0;
      _protocol = 0;
      _algorithm = 0;
      _pub_key = "";
    }
  };

  DSInfo& operator=(const DSInfo &dsInfo) {
    _key_tag = dsInfo._key_tag;
    _key_tag_f = dsInfo._key_tag_f;
    _algo = dsInfo._algo;
    _algo_f = dsInfo._algo_f;
    _digest_type = dsInfo._digest_type;
    _digest_type_f  = dsInfo._digest_type_f;
    _digest = dsInfo._digest;
    _digest_f = dsInfo._digest_f;
    _max_sig_life = dsInfo._max_sig_life;
    _key_data = dsInfo._key_data;
    _hasKeyData = dsInfo._hasKeyData;

    return *this;
  }

  /// Default constructor
  DSInfo()
  {
    this->reset();
  }

  /// Sets key tag
  /**
     @param key_tag  key_tag
  */
  void set_key_tag(const unsigned int key_tag)
  { 
    _key_tag = key_tag;
    _key_tag_f = true;
  }

  /// Returns the key tag
  /**
     @return key tag
   */
  unsigned int get_key_tag() const { return _key_tag; }

  /// Sets algorithm
  /**
     @param algo algorithm
  */
  void set_algo(const unsigned int algo)
  { 
    _algo = algo;
    _algo_f = true;
  }

  /// Returns the algorithm
  /**
     @return algorithm
   */
  unsigned int get_algo() const { return _algo; }

  /// Sets the digest type
  /**
     @param digest_type digest type
  */
  void set_digest_type(const unsigned int digest_type)
  { 
    _digest_type = digest_type;
    _digest_type_f = true;
  }

  /// Returns the digest type
  /**
     @return digest type
   */
  unsigned int get_digest_type() const { return _digest_type; }

  /// Sets the digest 
  /**
     @param digest the digest 
  */
  void set_digest(const string& digest)
  { 
    _digest = digest;
    _digest_f = true;
  }

  /// Returns the digest
  /**
     @return digest
  */
  string get_digest() const
  {
    return _digest;
  }

  /// Sets the maximum signature life
  /**
     @param max_sig_life the maximum signature life 
  */
  void set_max_sig_life(const unsigned int max_sig_life)
  { 
    _max_sig_life = max_sig_life;
  }

  /// Returns the maximum signature life
  /**
     @return maximum signature life
  */
  unsigned int get_max_sig_life() const 
  {
    return _max_sig_life;
  }

  /// Sets the key data
  /**
     @param key_data the key data
  */
  void set_key_data(const KeyData& key_data)
  { 
    _key_data = key_data;
    _hasKeyData = true;
  }

  /// Returns the key data
  /**
     @return key data
  */
  KeyData get_key_data() const
  {
    return _key_data;
  }

  /// Returns what it means to
  bool hasKeyData() const
  {
    return _hasKeyData;
  }

  /// Returns the xml format
  /**
     @return xml
  */
  string get_xml_format() const 
  {
    string xml = "<secDNS:dsData>"
      "<secDNS:keyTag>" + StrUtil::to_string("%u", _key_tag) +
      "</secDNS:keyTag>"
      "<secDNS:alg>"  + StrUtil::to_string("%u", _algo) +
      "</secDNS:alg>"
      "<secDNS:digestType>" + StrUtil::to_string("%u", _digest_type) +
      "</secDNS:digestType>"
      "<secDNS:digest>" + StrUtil::esc_xml_markup(_digest) + 
      "</secDNS:digest>"; 

    if (_max_sig_life > 0) {
      xml += "<secDNS:maxSigLife>" + StrUtil::to_string("%u", _max_sig_life) +
	"</secDNS:maxSigLife>";
    }

    if (_hasKeyData) {
      xml += "<secDNS:keyData>"
	"<secDNS:flags>" + StrUtil::to_string("%u", _key_data._flags) +
	"</secDNS:flags>"
	"<secDNS:protocol>" + StrUtil::to_string("%u", _key_data._protocol) +
	"</secDNS:protocol>"
	"<secDNS:alg>"  + StrUtil::to_string("%u", _key_data._algorithm) +
	"</secDNS:alg>"
	"<secDNS:pubKey>" + _key_data._pub_key +
	"</secDNS:pubKey>" +
	"</secDNS:keyData>";
    }

    xml += "</secDNS:dsData>";
    
    return xml;
  }

  /// Reset all object attributes
  void reset()
  {
    _key_tag = 0;
    _algo = 0;
    _digest_type = 0;
    _max_sig_life = 0;
    _digest = "";
    _key_data.reset();
    _key_tag_f = false;
    _algo_f = false;
    _digest_type_f = false;
    _digest_f = false;
    _hasKeyData = false;
  }

protected:
  // Flags for DS Info attributes
  bool _key_tag_f;
  bool _algo_f;
  bool _digest_type_f;
  bool _digest_f;
  bool _hasKeyData;

  /// DS Info attributes
  unsigned int _key_tag;
  unsigned int _algo;
  unsigned int _digest_type;
  string _digest;
  unsigned int _max_sig_life;
  KeyData _key_data;

};
LIBEPP_NICBR_NS_END
#endif //__DSINFO__
