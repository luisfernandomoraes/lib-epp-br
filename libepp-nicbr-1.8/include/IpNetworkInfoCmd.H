#ifndef __IPNETWORKINFOCMD_H__
#define __IPNETWORKINFOCMD_H__

#include "libepp_nicbr.H"

#include "Command.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP IpNetworkInfo Class
class IpNetworkInfoCmd : public Command
{
public:
  /// Default constructor
  IpNetworkInfoCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets ip range
  /**
     @param ipRange  ip range
  */
  void set_ipRange(const IpRange &ipRange)
  {
    _ipRange = ipRange;
  }

  /// Returns ip range
  /**
     @return ip range
  */
  IpRange get_ipRange() const
  {
    return _ipRange;
  }

  /// Sets repository object ID
  /**
     @param roid   contains the server-unique id for the created IP network
  */
  void set_roid(const string &roid)
  {
    _roid = roid;
  }

  /// Returns repository object ID
  /**
     @return server-unique id for the created IP network
  */
  string get_roid() const
  {
    return _roid;
  }

  void reset()
  {
    Command::reset();
    _ipRange.reset();
    _roid = "";
  }

protected:
  /// ip range
  IpRange _ipRange;

  /// server-unique id for the created IP network
  string _roid;
};

LIBEPP_NICBR_NS_END
#endif // __IPNETWORKINFOCMD_H__
