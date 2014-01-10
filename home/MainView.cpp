#include "MainView.h"
#include "AMXClient.h"

MainView::MainView()
: nuiSimpleContainer(),
  mAMXAddr("192.168.1.100"),
  mAMXPort(3131),
  mViewSink(this)
{
  nuiVBox* pVBox=new nuiVBox();
  pVBox->SetPosition(nuiFill);
  pVBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pVBox);
//  pVBox->SetAllCellsSpacing(4);
  pVBox->SetEqualizeCells(true);

  
  nuiHBox* pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);

  nuiButton* pButton = new nuiButton("CONNECT");
  pButton->SetObjectName("Button");
  pButton->GetChild(0)->SetObjectName("ButtonLabel");
  pButton->SetPosition(nuiFill);
  pHBox->AddCell(pButton);
  mViewSink.Connect(pButton->Activated, &MainView::OnConnectActivated, pButton);

  pButton = new nuiToggleButton("LOCK");
  pButton->SetObjectName("Button");
  pButton->GetChild(0)->SetObjectName("ButtonLabel");
  pButton->SetPosition(nuiFill);
  pHBox->AddCell(pButton);
  mViewSink.Connect(pButton->Activated, &MainView::OnLockActivated, pButton);
  mViewSink.Connect(((nuiToggleButton*)pButton)->Deactivated, &MainView::OnLockActivated, pButton);
  
  pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);

  pButton=new nuiButton("ALL LIGHTS ON");
  pButton->SetObjectName("Button");
  pButton->SetPosition(nuiFill);
  pButton->GetChild(0)->SetObjectName("ButtonLabel");
  pButton->SetProperty("func", "ON");
  pButton->SetProperty("value", "D001000");
  pHBox->AddCell(pButton);
  mViewSink.Connect(pButton->Activated, &MainView::OnAllLightsActivated, pButton);

  pButton=new nuiButton("ALL LIGHTS OFF");
  pButton->SetObjectName("Button");
  pButton->SetPosition(nuiFill);
  pButton->GetChild(0)->SetObjectName("ButtonLabel");
  pButton->SetProperty("func", "OFF");
  pButton->SetProperty("value", "D000000");
  pHBox->AddCell(pButton);
  mViewSink.Connect(pButton->Activated, &MainView::OnAllLightsActivated, pButton);

  for (int j = 0; j < 6; j++)
  {
    pHBox = new nuiHBox();
    pHBox->SetExpand(nuiExpandShrinkAndGrow);
    pHBox->SetPosition(nuiFill);
    pHBox->SetEqualizeCells(true);
    pVBox->AddCell(pHBox);

    for (int i = 0; i < 4; ++i)
    {
      nglString valueStr;
      valueStr.CFormat("DIM%d-%d", j,i);
      pButton=new nuiButton(valueStr);
      pButton->SetObjectName("ButtonLight");
      pButton->SetPosition(nuiFill);
      pButton->GetChild(0)->SetObjectName("ButtonLabel");
      pButton->SetProperty("func", "SET");
      valueStr.CFormat("D%d%d4%.3d", j, i, 255);
      pButton->SetProperty("value", valueStr);
      mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
      pHBox->AddCell(pButton);
    }
    pHBox = new nuiHBox();
    pHBox->SetExpand(nuiExpandShrinkAndGrow);
    pHBox->SetPosition(nuiFill);
    pHBox->SetEqualizeCells(true);
    pVBox->AddCell(pHBox);
    
    for (int i = 0; i < 4; ++i)
    {
      nglString valueStr;
      valueStr.CFormat("DIM OFF");
      pButton=new nuiButton(valueStr);
      pButton->SetObjectName("ButtonRelay");
      pButton->SetPosition(nuiFill);
      pButton->GetChild(0)->SetObjectName("ButtonLabel");
      pButton->SetProperty("func", "SET");
      valueStr.CFormat("D%d%d0%.3d", j, i, 255);
      pButton->SetProperty("value", valueStr);
      mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
      pHBox->AddCell(pButton);
    }
  }

/*
  pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetPosition(nuiFill);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);

  int steps=2;
  for (int i = 0; i <= steps; ++i)
  {
    int value = std::max(0, std::min(255, i * (256/steps)));

    nglString valueStr;
    valueStr.CFormat("Salon Roof %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("ButtonLight");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "SET");
    valueStr.CFormat("D004%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);
  }

  pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetPosition(nuiFill);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);

  for (int i = 0; i <= steps; ++i)
  {
    int value = std::max(0, std::min(255, i * (256/steps)));
    
    nglString valueStr;

    valueStr.CFormat("Salon Wall %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("ButtonLight");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "SET");
    valueStr.CFormat("D024%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);
  }

  pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetPosition(nuiFill);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);
  
  for (int i = 0; i <= steps; ++i)
  {
    int value = std::max(0, std::min(255, i * (256/steps)));
    
    nglString valueStr;
    
    valueStr.CFormat("Bureau Roof %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("ButtonLight");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "SET");
    valueStr.CFormat("D22\4%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);
  }

  pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetPosition(nuiFill);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);
  
  for (int i = 0; i <= steps; ++i)
  {
    int value = std::max(0, std::min(255, i * (256/steps)));
    
    nglString valueStr;
    
    valueStr.CFormat("Bureau Wall %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("ButtonLight");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "SET");
    valueStr.CFormat("D204%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);
  }

  pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetPosition(nuiFill);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);
  
  for (int i = 0; i <= steps; ++i)
  {
    int value = std::max(0, std::min(255, i * (256/steps)));
    
    nglString valueStr;
    
    valueStr.CFormat("Chambre 0 %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("ButtonLight");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "SET");
    valueStr.CFormat("D104%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);
  }

  
  pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetPosition(nuiFill);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);
  
  for (int i = 0; i <= steps; ++i)
  {
    int value = std::max(0, std::min(255, i * (256/steps)));
    
    nglString valueStr;
    
    valueStr.CFormat("Chambre 2 %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("Button");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "SET");
    valueStr.CFormat("D124%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);
  }

  
  pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetPosition(nuiFill);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);
  
  for (int i = 0; i <= steps; ++i)
  {
    int value = std::max(16, std::min(255, i * (256/steps)));
    
    nglString valueStr;
    
    valueStr.CFormat("Chambre 3 %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("ButtonLight");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "SET");
    valueStr.CFormat("D134%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);
  }
*/
/*
  const int relays = 7;
  for (int i = 5; i <= relays; ++i)
  {
    pHBox = new nuiHBox();
    pHBox->SetExpand(nuiExpandShrinkAndGrow);
    pHBox->SetPosition(nuiFill);
    pHBox->SetEqualizeCells(true);
    pVBox->AddCell(pHBox);
    
    
    nglString valueStr;
   
    for (int j = 1; j < 5; j++)
    {
      valueStr.CFormat("%d/%d ON", i, j);
      pButton=new nuiButton(valueStr);
      pButton->SetObjectName("ButtonRelay");
      pButton->SetPosition(nuiFill);
      pButton->GetChild(0)->SetObjectName("ButtonLabel");
      pButton->SetProperty("func", "SET");
      valueStr.CFormat("R%d%d1000", i, j);
      pButton->SetProperty("value", valueStr);
      mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
      pHBox->AddCell(pButton);
      
      valueStr.CFormat("OFF", i, j);
      pButton=new nuiButton(valueStr);
      pButton->SetObjectName("ButtonRelay");
      pButton->SetPosition(nuiFill);
      pButton->GetChild(0)->SetObjectName("ButtonLabel");
      pButton->SetProperty("func", "SET");
      valueStr.CFormat("R%d%d0000", i, j);
      pButton->SetProperty("value", valueStr);
      mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
      pHBox->AddCell(pButton);

    }
  }
*/
  
//    valueStr.CFormat("ANIM %.3d", value);
//    pButton=new nuiButton(valueStr);
//    pButton->SetObjectName("Button");
//    pButton->SetPosition(nuiFill);
//    pButton->GetChild(0)->SetObjectName("ButtonLabel");
//    pButton->SetProperty("func", "ANIM");
//    valueStr.CFormat("D009%.3d", value);
//    pButton->SetProperty("value", valueStr);
//    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
//    pHBox->AddCell(pButton);
//
//    value = (rand()+111)%1000;
//    valueStr.CFormat("KEYPADS COLOR %.3d", value);
//    pButton=new nuiButton(valueStr);
//    pButton->SetObjectName("Button");
//    pButton->SetPosition(nuiFill);
//    pButton->GetChild(0)->SetObjectName("ButtonLabel");
//    pButton->SetProperty("func", "ANIM");
//    valueStr.CFormat("K00140%.3d", value);
//    pButton->SetProperty("value", valueStr);
//    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
//    pHBox->AddCell(pButton);
//
//    valueStr.CFormat("KEYPADS ANIM %.3d", value);
//    pButton=new nuiButton(valueStr);
//    pButton->SetObjectName("Button");
//    pButton->SetPosition(nuiFill);
//    pButton->GetChild(0)->SetObjectName("ButtonLabel");
//    pButton->SetProperty("func", "ANIM");
//    valueStr.CFormat("K00141%.3d", value);
//    pButton->SetProperty("value", valueStr);
//    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
//    pHBox->AddCell(pButton);
//
//    valueStr.CFormat("KEYPADS ANIM_X2 %.3d", value);
//    pButton=new nuiButton(valueStr);
//    pButton->SetObjectName("Button");
//    pButton->SetPosition(nuiFill);
//    pButton->GetChild(0)->SetObjectName("ButtonLabel");
//    pButton->SetProperty("func", "ANIM");
//    valueStr.CFormat("K00142%.3d", value);
//    pButton->SetProperty("value", valueStr);
//    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
//    pHBox->AddCell(pButton);
//    
//    valueStr.CFormat("KEYPADS ANIM_X3 %.3d", value);
//    pButton=new nuiButton(valueStr);
//    pButton->SetObjectName("Button");
//    pButton->SetPosition(nuiFill);
//    pButton->GetChild(0)->SetObjectName("ButtonLabel");
//    pButton->SetProperty("func", "ANIM");
//    valueStr.CFormat("K00143%.3d", value);
//    pButton->SetProperty("value", valueStr);
//    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
//    pHBox->AddCell(pButton);
//
//    valueStr.CFormat("KEYPADS ANIM_X4 %.3d", value);
//    pButton=new nuiButton(valueStr);
//    pButton->SetObjectName("Button");
//    pButton->SetPosition(nuiFill);
//    pButton->GetChild(0)->SetObjectName("ButtonLabel");
//    pButton->SetProperty("func", "ANIM");
//    valueStr.CFormat("K00144%.3d", value);
//    pButton->SetProperty("value", valueStr);
//    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
//    pHBox->AddCell(pButton);
//  }
}

void MainView::OnConnectActivated(const nuiEvent& rEvent)
{
  
  AMXClient& rClient = AMXClient::Get();
  if (rClient.IsConnected())
  {
    rClient.Disconnect();
    ((nuiLabel*)((nuiButton*)rEvent.mpUser)->GetChild(0))->SetText("CONNECT");
    return;
  }
  if (rClient.Connect(mAMXAddr, mAMXPort))
  {
    ((nuiLabel*)((nuiButton*)rEvent.mpUser)->GetChild(0))->SetText("DISCONNECT");
  }
}

void MainView::OnLockActivated(const nuiEvent& rEvent)
{
  bool locked = ((nuiButton*)rEvent.mpUser)->IsPressed();
  AMXClient& rClient = AMXClient::Get();
  if (!rClient.IsConnected())
    return;
  rClient.Send(locked ? "LOCK" : "UNLOCK");
}

void MainView::OnAllLightsActivated(const nuiEvent& rEvent)
{
  AMXClient& rClient = AMXClient::Get();
  const nglString& rFunc = ((nuiWidget*)rEvent.mpUser)->GetProperty("func");
//  const nglString& rValue = ((nuiWidget*)rEvent.mpUser)->GetProperty("value");
  
  if (rClient.IsConnected())
  {
    if (rFunc == "OFF")
    {
      for (int addr=0; addr<6; addr++)
      {
        for (int c=0; c<4; c++)
        {
          nglString value;
          value.CFormat("D%d%d0000", addr, c);
          rClient.Send(value);
//          sleep(1);
        }
      }
    }
    else// (rFunc == "ON")
    {
      for (int addr=0; addr<6; addr++)
      {
        for (int c=0; c<4; c++)
        {
          nglString value;
          value.CFormat("D%d%d1000", addr, c);
          rClient.Send(value);
//          sleep(1);
        }
      }
    }
  }
}


void MainView::OnButtonActivated(const nuiEvent& rEvent)
{
  AMXClient& rClient = AMXClient::Get();
  const nglString& rFunc = ((nuiWidget*)rEvent.mpUser)->GetProperty("func");
  const nglString& rValue = ((nuiWidget*)rEvent.mpUser)->GetProperty("value");

  if (rClient.IsConnected())
  {
    rClient.Send(rValue);
  }
}


