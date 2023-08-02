#include "frame.hpp"
#include <string>
#include <iostream>
Frame::Frame()
    : wxFrame(nullptr, wxID_ANY, "Pomo")
{
    wxPanel *panel = new wxPanel(this, -1); 

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer *gs = new wxGridSizer(5, 2, 4, 4);

    start = new wxButton(panel, ID_START_BUTTON, wxT("Start"));
    stop = new wxButton(panel, ID_STOP_BUTTON, wxT("Stop"));
    reset = new wxButton(panel, ID_RESET_BUTTON, wxT("Reset"));
    alarm = new wxCheckBox(panel, wxID_ANY, wxT("Alarm"));

    longBreakIntervals = new wxTextCtrl(panel, wxID_ANY);
    breakTime = new wxTextCtrl(panel, wxID_ANY);
    workTime = new wxTextCtrl(panel, wxID_ANY);
    longBreakTime = new wxTextCtrl(panel, wxID_ANY);
    
    wxStaticText *longBreakIntervalsText = new wxStaticText(panel, wxID_ANY, wxT("Pomodoros Before Long Break"));
    wxStaticText *breakTimeText = new wxStaticText(panel, wxID_ANY, wxT("Break Time (Mins)"));
    wxStaticText *workTimeText = new wxStaticText(panel, wxID_ANY, wxT("Work Time (Mins)"));
    wxStaticText *longBreakTimeText = new wxStaticText(panel, wxID_ANY, wxT("Long Break Time (Mins)"));
    
    wxStaticText *statusLabelText = new wxStaticText(panel, wxID_ANY, wxT("Current Operation:"));
    statusText = new wxStaticText(panel, wxID_ANY, wxT("None"));
    
    timeText = new wxStaticText(panel, wxID_ANY, wxT("00:00"), wxDefaultPosition, wxSize(100,50));


    hbox1->Add(start, 0, wxEXPAND | wxALL, 4);
    hbox1->Add(stop, 0, wxEXPAND | wxALL, 4);
    hbox1->Add(reset, 0, wxEXPAND | wxALL, 4);
    hbox1->Add(alarm, 0, wxEXPAND | wxALL, 4);


    gs->Add(longBreakIntervalsText);
    gs->Add(longBreakIntervals);

    gs->Add(breakTimeText);
    gs->Add(breakTime);

    gs->Add(workTimeText);
    gs->Add(workTime);

    gs->Add(longBreakTimeText);
    gs->Add(longBreakTime);
    
    gs->Add(statusLabelText);
    gs->Add(statusText);

    vbox->Add(timeText);
    vbox->Add(hbox1);
    vbox->Add(gs);
    panel->SetSizer(vbox);
    
    timer = new wxTimer(this, 1);

    Connect(ID_START_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Frame::OnStart));
    Connect(ID_RESET_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Frame::OnReset));
    Connect(ID_STOP_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Frame::OnStop));
    Connect(wxEVT_TIMER, wxCommandEventHandler(Frame::OnTimer));


    _secs = 0;
    _minutes = 0;
    _break = 5;
    _work = 25;
    _interval = 0;
    _longBreak = 30;
    _longBreakIntervals = 1;
    _timerState = TIMER_RESET;
    Centre();
}


void Frame::OnStart(wxCommandEvent& event)
{
    switch(_timerState)
    {
    case TIMER_RESET:
        UpdateFields();
        _minutes = _work;
        _secs = 0;
        timer->Start(1000);
        break;
    case TIMER_STOPPED:
        timer->Start(1000);
        break;
    }
    _timerState = TIMER_WORKING;
    UpdateStatus();
}
    
void Frame::OnReset(wxCommandEvent& event)
{
    timer->Stop();
    _secs = 0;
    _minutes = 0;
    timeText->SetLabel(wxT("00:00"));
    _timerState = TIMER_RESET;
    UpdateStatus();
}

void Frame::OnStop(wxCommandEvent& event)
{
    timer->Stop();
    _timerState = TIMER_STOPPED;
    UpdateStatus();
}

void Frame::OnTimer(wxCommandEvent& event)
{
    std::string min = _minutes > 9 ? std::to_string(_minutes) : "0" + std::to_string(_minutes);
    std::string sec = _secs > 9 ? std::to_string(_secs) : "0" + std::to_string(_secs);
    std::string str = min + ":" + sec;
    timeText->SetLabel(wxString(str));
    --_secs;
    if (_secs == -1)
    {
        --_minutes;
        _secs = 59;
    }

    if(!_minutes && !_secs)
    {
        if(_timerState == TIMER_WORKING)
        {
            _minutes = _break;
            _timerState = TIMER_BREAK;
        } 
        else 
        {
            _minutes = _work;
            _timerState = TIMER_WORKING;
            _interval++;
        }
        if(_interval == _longBreakIntervals)
        {
            
            _minutes = _longBreak;
            _timerState = TIMER_LONG_BREAK;
            _interval = 0;
        }
        UpdateStatus();
    }
} 
void Frame::UpdateStatus(void)
{
    switch(_timerState)
    {
    case TIMER_BREAK:
        statusText->SetLabel(wxT("Break"));
        break;
    case TIMER_LONG_BREAK:
        statusText->SetLabel(wxT("Long Break"));
        break;
    case TIMER_RESET:
        statusText->SetLabel(wxT("Reset"));
        break;
    case TIMER_STOPPED:
        statusText->SetLabel(wxT("Stopped"));
        break;
    case TIMER_WORKING:
        statusText->SetLabel(wxT("Work"));
        break;
    }
}

void Frame::UpdateFields(void)
{
    _longBreakIntervals = std::stoi(longBreakIntervals->GetLineText(0).ToStdString());
    _break = std::stoi(breakTime->GetLineText(0).ToStdString());
    _work = std::stoi(workTime->GetLineText(0).ToStdString());
    _longBreak = std::stoi(longBreakTime->GetLineText(0).ToStdString());
}


