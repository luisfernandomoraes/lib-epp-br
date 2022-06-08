#ifndef __IPNETWORKINFO_H__
#define __IPNETWORKINFO_H__

#include "libepp_nicbr.H"

#include "Action.H"
#include "IpNetworkInfoCmd.H"
#include "IpNetworkInfoRsp.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_BEGIN

/// EPP IpNetworkInfo Class
class IpNetworkInfo : public Action
{
public:
  /// Constructor
  IpNetworkInfo(const ActionType type = IP_NETWORK_INFO) : Action(type)
  {
    if (type == IP_NETWORK_INFO) {
      _command = auto_ptr<IpNetworkInfoCmd>(new IpNetworkInfoCmd());
      _response = auto_ptr<IpNetworkInfoRsp>(new IpNetworkInfoRsp());
    }
  }

  /// Sets XML template
  /**
     @param xml_template   XML command template
  */
  void set_xml_template(const string &xml_template);

  /// Pure virtual method to set response from a XML document
  /**
     @param xml_payload   XML document
     @param parser        reference to the XML parser
   */
  void set_response(const string &xml_payload, DomParser *parser) 
  {
    get_response()->reset();
    parser->parse_ipnetwork_info_rsp(xml_payload, get_response());
  }

  /// Returns raw pointer to the command
  /**
     @return raw pointer to the command
  */
  IpNetworkInfoCmd* get_command() 
  {
    return (IpNetworkInfoCmd*) _command.get(); 
  }

  /// Returns raw pointer to the response
  /**
     @return raw pointer to the response
  */
  IpNetworkInfoRsp* get_response() 
  { 
    return (IpNetworkInfoRsp*) _response.get(); 
  }
};

LIBEPP_NICBR_NS_END
#endif // __IPNETWORKINFO_H__
