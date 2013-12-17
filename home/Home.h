#ifndef __home_h__
#define __home_h__

#include "nuiApplication.h"

///< Fwd. decls.
class nuiMainMenu;
class nuiMainMenuItem;

#define USE_APP_CONSOLE 0
class GLConsole : public nglConsole
{
  nglString mBacklog;
  nuiMainWindow* mpWindow;
  nuiLabel* mpText;
  nuiEventSink<GLConsole> mSink;
  
public:
  GLConsole() : nglConsole (true), mpWindow(NULL), mpText(NULL), mSink(this) {}
  void SetWindow(nuiMainWindow* pWindow) 
  { 
    mpWindow = pWindow; 
    
    nuiSimpleContainer* pCtn = new nuiSimpleContainer();
    pCtn->SetObjectClass(_T("MappingScreenFrame"));
    pCtn->SetUserSize(500, 200);
    pCtn->SetPosition(nuiBottomRight);
    pWindow->AddChild(pCtn);
    
    nuiScrollView* pScrollView = new nuiScrollView();
    pScrollView->SetBorder(4, 4, 4, 4);
    pCtn->AddChild(pScrollView);
    
    nuiLabel* pLabel = new nuiLabel(mBacklog);
    pLabel->SetObjectClass(_T("ToolbarButtonLabel"));
    pLabel->SetWrapping(true);
    pScrollView->AddChild(pLabel);
    
    nuiButton* pButton = new nuiButton(_T("CLEAR"));
    pButton->SetObjectClass(_T("ToolbarButton"));
    pButton->GetChild(0)->SetObjectClass(_T("ToolbarButtonLabel"));
    pButton->SetPosition(nuiTopRight);
    pButton->SetUserSize(45, 25);
    pButton->SetBorder(0, 8, 0, 0);
    pCtn->AddChild(pButton);
    mpText = pLabel;
    mSink.Connect(pButton->Activated, &GLConsole::OnClear);
  }
  
  void OnClear(const nuiEvent& rEvent)
  {
    mBacklog = _T("");
    Draw();
  }
  
  virtual void OnOutput (const nglString& rLine)
  {
    mBacklog += rLine;
    Draw();
  }
  
  void Draw()
  {
    if (mpWindow && mpText)
      mpText->SetText(mBacklog);
  }
};  


class MainWindow;

MainWindow* GetMainWindow();

class Home : public nuiApplication
{
public:
  Home();
  ~Home();

  void OnInit();
  void OnExit (int Code);
  void OnWillExit();
  void OnMemoryWarning();
  void OnDeactivation();
  void OnOpenFile(const nglPath& rPath);
  void OnOpenURL(const nglString& rURL);

  // Push notifications (from UIApplication)
#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
  void OnRegisterForRemoteNotificationsWithDeviceToken(CFDataRef data);
  void OnFailToRegisterForRemoteNotificationsWithError(CFErrorRef error);
  void OnReceiveRemoteNotification(CFDictionaryRef dictionary);
#endif

	inline MainWindow* GetMainWindow() const { return mpWindow; }
  

// Desktop version specifics
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
public:
  inline const nglPath& GetStartupFile() const { return mStartupFile; }
protected:
  nglPath mStartupFile; // Keep the path passed to the app launch, if any
#endif

protected:
  MainWindow* mpWindow;
};

#endif//__home_h__
