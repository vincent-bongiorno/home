#ifndef __MainWindow_h__
#define __MainWindow_h__

//#include "nuiEvent.h"
#include "nuiMainWindow.h"
#include "nuiModalContainer.h"
#include "nuiNotification.h"

//! Fwd. decls.

class __attribute__ ((visibility("default"))) MainWindow
: public nuiMainWindow,
  public nuiNotificationObserver
{
/** @name Life cycle */
//@{
public:
  MainWindow(const nglContextInfo& rContext,
         const nglWindowInfo& rInfo,
         const nglContext* pShared = NULL);
  ~MainWindow();
  void OnTrash();
//@}

/** @name Window refresh and properties */
//@{
public:
  void OnCreation();
  void OnClose();
  bool OnRotation(uint Angle);
  void OnActivation();
  void OnDesactivation();
//  ial::Signal1<const nglString&> OpenURL; // Called when opening an URL
private:
	bool LoadCSS(const nglPath& rPath);
  void InitHotKeys();
  void OnHotKey(const nuiEvent& rEvent);
//@}

/** @name Error handling */
//@{
private:
//@}

/** @name Keyboard handling */
//@{
public:
  bool KeyDown(const nglKeyEvent& rEvent);
private:
//@}
  
  
/** @name Menu handling (Desktop only) */
//@{
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
protected:
  void InitMainMenu();
//  void OnMenuNew(const nuiEvent& rEvent);
//  void OnMenuOpen(const nuiEvent& rEvent);
//  void OnMenuSave(const nuiEvent& rEvent);
//  void OnMenuSaveAs(const nuiEvent& rEvent);
//  void OnMenuAudioSetup(const nuiEvent& rEvent);
//  void OnMenuMIDISetup(const nuiEvent& rEvent);
  nuiMainMenu* mpMainMenu;
#endif
//@}
  
  
/** @name Background mode operation */
//@{
public:
  inline void SetForceBackgroundMode(bool Set) { mForceBgMode = Set; }
private:
  bool mForceBgMode;
//@}

/** @name View handling */
//@{
public:
  void OpenStudio(bool Open);
  void SetCurrentView(const nglString& rViewClass, const nglString& rBasePath=nglString::Empty);
  nuiWidget* GetCurrentView();
  nuiWidget* GetEditor();
  void SetEditor(nuiWidget* pView, bool showTransport = true, bool enableRecording = false);
//  void SetForegroundView(nuiWidget* pView);
  void TrashView(const nglString& rViewClass, const nglString& rBasePath);

  nuiSimpleEventSource<0> ViewChanged;

private:
  void ShowViewLayer(bool Show);
  void ShowStudioLayer(bool Show);
  void ShowEditorLayer(bool Show);
  void ShowTransportLayer(bool Show);
  void TrashCurrentView();
  void OnLayerAnimStop(const nuiEvent& rEvent);
  void EnableRecording(bool Enabled);

private:
  void OnStudioToggled(const nuiEvent& rEvent);
  void OnTransportToggled(const nuiEvent& rEvent);
  void OnTransportHiddenChanged(const nuiEvent& rEvent);
  void OnEditorTrashed(const nuiEvent& rEvent);
//@}

  
/** @name iOS Auto-lock control */
//@{
public:
  void SetIgnoreAutoLockTimer(bool Ignore);
//@}

private:
  bool mAppLoaded;
  nuiTimer mTimer;
  nuiEventSink<MainWindow> mWinSink;
  void OnTimerTick(const nuiEvent& rEvent);
  void OnNotification(const nuiNotification& rNotification);
};


#endif//__MainWindow_h__
