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

  nuiButton* pButton = new nuiButton("CONNECT");
  pButton->SetObjectName("Button");
  pButton->GetChild(0)->SetObjectName("ButtonLabel");
  pButton->SetPosition(nuiFill);
  pVBox->AddCell(pButton);
  mViewSink.Connect(pButton->Activated, &MainView::OnConnectActivated, pButton);

  nuiHBox* pHBox = new nuiHBox();
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
  pHBox->SetEqualizeCells(true);
  pVBox->AddCell(pHBox);

  pButton=new nuiButton("LIGHTS ON");
  pButton->SetObjectName("Button");
  pButton->SetPosition(nuiFill);
  pButton->GetChild(0)->SetObjectName("ButtonLabel");
  pButton->SetProperty("func", "ON");
  pButton->SetProperty("value", "D001000");
  pHBox->AddCell(pButton);
  mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);

  pButton=new nuiButton("LIGHTS OFF");
  pButton->SetObjectName("Button");
  pButton->SetPosition(nuiFill);
  pButton->GetChild(0)->SetObjectName("ButtonLabel");
  pButton->SetProperty("func", "OFF");
  pButton->SetProperty("value", "D000000");
  pHBox->AddCell(pButton);
  mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);



  int steps=4;
  for (int i = 0; i <= steps; ++i)
  {
    
    pHBox = new nuiHBox();
    pHBox->SetExpand(nuiExpandShrinkAndGrow);
    pHBox->SetPosition(nuiFill);
    pHBox->SetEqualizeCells(true);
    pVBox->AddCell(pHBox);

    int value = std::max(16, std::min(255, i * (256/steps)));

    nglString valueStr;
    valueStr.CFormat("SET %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("Button");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "SET");
    valueStr.CFormat("D004%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);

    valueStr.CFormat("ANIM %.3d", value);
    pButton=new nuiButton(valueStr);
    pButton->SetObjectName("Button");
    pButton->SetPosition(nuiFill);
    pButton->GetChild(0)->SetObjectName("ButtonLabel");
    pButton->SetProperty("func", "ANIM");
    valueStr.CFormat("D009%.3d", value);
    pButton->SetProperty("value", valueStr);
    mViewSink.Connect(pButton->Activated, &MainView::OnButtonActivated, pButton);
    pHBox->AddCell(pButton);
  }
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


