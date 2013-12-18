#include "AMXClient.h"
#include "nuiTCPClient.h"

AMXClient::AMXClient()
: mpClient(NULL)
{
  mpClient = new nuiTCPClient();
//  mpClient->SetNonBlocking(true);
}

AMXClient::~AMXClient()
{
  if (mpClient->IsConnected())
    mpClient->Close();
  delete mpClient;
}

AMXClient& AMXClient::Get()
{
  static AMXClient client;
  return client;
}

bool AMXClient::Connect(nglString& rAMXAddr, int Port)
{
  return mpClient->Connect(rAMXAddr, Port);
}

void AMXClient::Disconnect()
{
  mpClient->Close();
}

bool AMXClient::IsConnected() const
{
  return mpClient->IsConnected();
}

bool AMXClient::Send(const nglString& rData)
{
  return (mpClient->Send(rData) == rData.GetLength());
}
