#include "MainWindow.h"
#include "Home.h"


#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
# include "nuiMainMenu.h"
#endif

#include "nuiCSS.h"
#include "nuiVBox.h"
#include "nuiHBox.h"

#include "nuiAttributeAnimation.h"
#include "MainView.h"

#pragma mark MainWindow
/**************
 * Life cycle *
 **************/
#pragma mark Life cycle

MainWindow::MainWindow( const nglContextInfo& rContext,
                const nglWindowInfo& rInfo,
                const nglContext* pShared )
: nuiMainWindow(rContext, rInfo, pShared, nglPath((_T("rsrc:")))),
  nuiNotificationObserver(),
  mTimer(1.0/10.0),
  mWinSink(this)
{
  SetObjectClass(_T("MainWindow"));
  SetObjectName(_T("MainWindow"));
  
///< Main menu (Desktop only)
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
  InitMainMenu();
#endif
//
//// OS X Preference file
//#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
//  nglPath prefPath = *gPrefFilePath;
//  pPrefs->SetSystemPreferencesFile(prefPath);
//#else
//  // Check if the we need to set the default values for first time launch
//  if (!pPrefs->HasPreference(BM_PREF_RESET_DB_KEY) || !pPrefs->HasPreference(BM_PREF_LIMIT_MEMORY))
//  {
//    // Open our settings bundle and find our settings description
//		NSString *pathStr = [[NSBundle mainBundle] bundlePath];
//		NSString *settingsBundlePath = [pathStr stringByAppendingPathComponent:@"Settings.bundle"];
//		NSString *finalPath = [settingsBundlePath stringByAppendingPathComponent:@"Root.plist"];
//        
//		NSDictionary *settingsDict = [NSDictionary dictionaryWithContentsOfFile:finalPath];
//		NSArray *prefSpecifierArray = [settingsDict objectForKey:@"PreferenceSpecifiers"];
//    
//    // copy our settings to the standard defaults
//    NSMutableDictionary *defaultsToRegister = [[NSMutableDictionary alloc] initWithCapacity:[prefSpecifierArray count]];
//    for (NSDictionary *prefSpecification in prefSpecifierArray) 
//    {
//      NSString *key = [prefSpecification objectForKey:@"Key"];
//      NSLog(@"%@", key);
//      if (key)
//      {
//        [defaultsToRegister setObject:[prefSpecification objectForKey:@"DefaultValue"] forKey:key];
//      }
//    }
//    
//    finalPath = [settingsBundlePath stringByAppendingPathComponent:@"Memory.plist"];
//		settingsDict = [NSDictionary dictionaryWithContentsOfFile:finalPath];
//		prefSpecifierArray = [settingsDict objectForKey:@"PreferenceSpecifiers"];
//    
//    // copy our settings to the standard defaults
//    for (NSDictionary *prefSpecification in prefSpecifierArray) 
//    {
//      NSString *key = [prefSpecification objectForKey:@"Key"];
//      NSLog(@"%@", key);
//      if (key)
//      {
//        [defaultsToRegister setObject:[prefSpecification objectForKey:@"DefaultValue"] forKey:key];
//      }
//    }
//        
//    // Anti-crack 3
//    // baseCheck(gBundlePath);
//
//    [[NSUserDefaults standardUserDefaults] registerDefaults:defaultsToRegister];
//    [defaultsToRegister release];
//  }  
//
//#endif
//
//	if (!pPrefs->LoadSystemPreferences())
//  {
//    NGL_DEBUG(NGL_OUT(_T("[MainWindow] Could not open system preferences !\n")));
//    NGL_ASSERT(0);
//  }
}

MainWindow::~MainWindow()
{
  UnregisterObserver(this);
}

void MainWindow::OnTrash()
{
}

/*********************************
 * Window refresh and properties *
 *********************************/
#pragma mark Window refresh and properties

void MainWindow::OnCreation()
{
//EnableRenderCache(false);

  SetColor(eActiveWindowBg, nuiColor(0,0,0));

//#ifdef _UIKIT_
//  if (ial::HasWideScreen())
//    pBG = new nuiImage(_T("images:/Default_16_9.png"));
//  else
//    pBG = new nuiImage(_T("images:/Default.png"));
//#elif (TARGET_OS_MAC && !TARGET_OS_IPHONE)
//    pBG = new nuiImage(_T("images:/Default-OSX.png"));
//#else
//# error "TARGET MISSING!"
//#endif


  nglString root_css = _T("rsrc:/css/");

//#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
//  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
//#endif

//  ial::DeviceType _type = GetDeviceType();
//  if (_type == ial::eDeviceTypeTablet)
//    root_css = _T("rsrc:/css_tablet/");
  
  ///< CSS
#if (!TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE) // Real desktop versions
  SetDebugMode(true);
  LoadCSS(root_css + _T("device.css"));
#else // Device
  LoadCSS(root_css + _T("device.css"));
#endif

  
//#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
//  [pool release];
//#endif

  MainView* pView = new MainView();
  pView->SetObjectName("MainView");
  AddChild(pView);

//  nuiButton* pButton = new nuiButton("HELLO WOLRD");
//  pButton->SetObjectName("Button");
//  pButton->GetChild(0)->SetObjectName("ButtonLabel");
//  nuiSimpleContainer* pContainer = new nuiSimpleContainer();
//  pContainer->SetObjectName("MainView");
//  AddChild(pContainer);
//  pContainer->AddChild(pButton);
  
//  nuiImage* pImage = new nuiImage("rsrc:/images/8x8_square_outlined.png");
//  pImage->SetPosition(nuiTopLeft);
//  AddChild(pImage);
  
  mWinSink.Connect(mTimer.Tick, &MainWindow::OnTimerTick);
  mTimer.Start();

  return;
}

bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%s'\n"), rPath.GetChars());
    return false;
  }

  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;
  
	if (!res)
	{
		NGL_OUT(_T("CSS error: %s\n"), pCSS->GetErrorString().GetChars());
		delete pCSS;
		NGL_ASSERT(0);
		return false;
	}

	SetCSS(pCSS);
	return true;
}

void MainWindow::OnTimerTick(const nuiEvent& rEvent)
{
  
}

void MainWindow::InitHotKeys()
{
//  nuiHotKey* pHotKey =
//    RegisterHotKeyKey("SAVE", NK_S, nuiCommandKey, true, false, nglString::Empty);
//  mWinSink.Connect(GetHotKeyEvent(pHotKey->GetName()), &Window::OnHotKey,pHotKey);
//  pHotKey =
//    RegisterHotKeyKey("SAVEAS", NK_S, nuiCommandKey|nuiShiftKey, true, false, nglString::Empty);
//  mWinSink.Connect(GetHotKeyEvent(pHotKey->GetName()), &Window::OnHotKey,pHotKey);
//
//  pHotKey =
//    RegisterHotKeyKey("NEW", NK_N, nuiCommandKey, true, false, nglString::Empty);
//  mWinSink.Connect(GetHotKeyEvent(pHotKey->GetName()), &Window::OnHotKey,pHotKey);
//
//  pHotKey =
//    RegisterHotKeyKey("OPEN", NK_O, nuiCommandKey, true, false, nglString::Empty);
//  mWinSink.Connect(GetHotKeyEvent(pHotKey->GetName()), &Window::OnHotKey,pHotKey);
//  
//  pHotKey =
//    RegisterHotKeyKey("STUDIO", NK_TAB, nuiNoKey, true, false, nglString::Empty);
//  mWinSink.Connect(GetHotKeyEvent(pHotKey->GetName()), &Window::OnHotKey,pHotKey);
}

void MainWindow::OnHotKey(const nuiEvent& rEvent)
{
  nuiHotKey* pHotKey = (nuiHotKey*)rEvent.mpUser;
  const nglString& rAction = pHotKey->GetName();
}

//void MainWindow::OnTimer(const nuiEvent& rEvent)
//{
//  if (mAppLoaded)
//    mpProxy->UpdateClient();
//}

void MainWindow::OnClose()
{
  NGL_DEBUG(NGL_OUT(_T("MainWindow Closing ...\n"));)
  ///#FIXME: Check if everything went the way they should have on closing
}

bool MainWindow::OnRotation(uint Angle)
{
  //NGL_OUT(_T("Angle: %d\n"), Angle);
//  if (Angle == 90 || Angle == 270)
//if (Angle == 270)
    return true;
//  return false;
}

///< Device is waking up from sleep or app is regaining focus
void MainWindow::OnActivation()
{
  //NGL_DEBUG(NGL_OUT(_T("[MainWindow] **************** OnActivation ****************\n")));
  
  //#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#if TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR
// Renable painter
  SetPaintEnabled(true);
#endif
}

///< App is going to sleep or app is losing focus
void MainWindow::OnDesactivation()
{
  //NGL_DEBUG(NGL_OUT(_T("[MainWindow] **************** OnDesactivation ****************\n")));
#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
  SetPaintEnabled(false);
#endif
}

//void MainWindow::OnOpenURL(const nglString& rURL)
//{
//  OpenURL(rURL);
//}

#pragma mark Keyboard handling
/*******************************************************************************
 * Keyboard handling ***********************************************************
 ******************************************************************************/

bool MainWindow::KeyDown(const nglKeyEvent& rEvent)
{
  return false;
}

// iOS : Ignore/enable activity status and cancel auto-lock
void MainWindow::SetIgnoreAutoLockTimer(bool Ignore)
{
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
	UIApplication* app = [UIApplication sharedApplication];
	app.networkActivityIndicatorVisible = Ignore;
	app.idleTimerDisabled = Ignore;
#endif
}


/*************************
 * Notification Handling *
 *************************/
#pragma mark Notification
void MainWindow::OnNotification(const nuiNotification& rNotification)
{

}

/*****************
 * Menu handling *
 *****************/
#pragma mark -
#pragma mark Menu handling

#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
void MainWindow::InitMainMenu()
{
//  nuiMainMenuItem* pItem = NULL;
//  
//  // root item for the menu bar. it's not displayed
//  nuiMainMenuItem* pRoot = new nuiMainMenuItem(_T("MainMenuRoot"));
//  
//  // create the main menu object, you'll have to register it from the main window
//  mpMainMenu = new nuiMainMenu(pRoot);
//  
//  //***********************************************************
//  // menu File
//  //
//  nuiMainMenuItem* pMenuFile = new nuiMainMenuItem(_T("Project"));
//  pRoot->AddChild(pMenuFile);
//  
//  pItem = new nuiMainMenuItem(_T("New...\t\t⌘N"));
//  pMenuFile->AddChild(pItem);
//  mWinSink.Connect(pItem->Activated, &Window::OnMenuNew);
//  
//  pItem = new nuiMainMenuItem(_T("Open...\t\t⌘O"));
//  pMenuFile->AddChild(pItem);
//  mWinSink.Connect(pItem->Activated, &Window::OnMenuOpen);
//  
//  pItem = new nuiMainMenuItem(_T("Save\t\t⌘S"));
//  pMenuFile->AddChild(pItem);
//  mWinSink.Connect(pItem->Activated, &Window::OnMenuSave);
//  
//  pItem = new nuiMainMenuItem(_T("Save As...\t⇧⌘S"));
//  pMenuFile->AddChild(pItem);
//  mWinSink.Connect(pItem->Activated, &Window::OnMenuSaveAs);
//
//  //***********************************************************
//  // menu Setup
//  //
//  nuiMainMenuItem* pMenuSetup = new nuiMainMenuItem(_T("Setup"));
//  pRoot->AddChild(pMenuSetup);
//  
//  pItem = new nuiMainMenuItem(_T("Audio Setup..."));
//  pMenuSetup->AddChild(pItem);
//  mWinSink.Connect(pItem->Activated, &Window::OnMenuAudioSetup);
//  
//  pItem = new nuiMainMenuItem(_T("MIDI Setup..."));
//  pMenuSetup->AddChild(pItem);
//  mWinSink.Connect(pItem->Activated, &Window::OnMenuMIDISetup);
//  
//  SetMainMenu(mpMainMenu);
}

//void Window::OnMenuNew(const nuiEvent& rEvent)
//{
//  NewProject();
//}
//void Window::OnMenuOpen(const nuiEvent& rEvent)
//{
//  OpenProject();
//}
//void Window::OnMenuSave(const nuiEvent& rEvent)
//{
//  SaveProject();
//}
//void Window::OnMenuSaveAs(const nuiEvent& rEvent)
//{
//  SaveProjectAs();
//}
//void Window::OnMenuAudioSetup(const nuiEvent& rEvent)
//{
//  if (HasMessagePanel() || !mAppLoaded)
//    return;
//
//  AudioDeviceSetupPanel* pPanel = new AudioDeviceSetupPanel(mpProxy);
//  pPanel->PushTopLevel(GetTopLevel());
//}
//void Window::OnMenuMIDISetup(const nuiEvent& rEvent)
//{
//  if (HasMessagePanel() || !mAppLoaded)
//    return;
//
//  MIDISetupPanel* pPanel = new MIDISetupPanel(mpProxy);
//  pPanel->PushTopLevel(GetTopLevel());
//}
#endif

