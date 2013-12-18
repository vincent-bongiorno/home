#ifndef __home__MainView__
#define __home__MainView__

#include <iostream>

class MainView : public nuiSimpleContainer
{
public:
  MainView();

private:
  nglString mAMXAddr;
  int16 mAMXPort;
  void OnConnectActivated(const nuiEvent& rEvent);
  void OnButtonActivated(const nuiEvent& rEvent);
  nuiEventSink<MainView> mViewSink;
};

#endif /* defined(__home__MainView__) */
