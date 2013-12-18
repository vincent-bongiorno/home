#ifndef __home__AMXClient__
#define __home__AMXClient__

#include <iostream>

class nuiTCPClient;

class AMXClient : public nuiSocket
{
private:
  AMXClient();
  ~AMXClient();
public:
  static AMXClient& Get();
  bool Connect(nglString& rAMXAddr, int Port);
  void Disconnect();
  bool IsConnected() const;

  bool Send(const nglString& rData);
  
  nuiSimpleEventSource<0> Connected;
  
private:
  nuiTCPClient* mpClient;
};

#endif /* defined(__home__AMXClient__) */



