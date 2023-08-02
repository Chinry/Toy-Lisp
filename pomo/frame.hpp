#pragma once
#include <wx/wx.h>
enum 
{
    ID_START_BUTTON,
    ID_STOP_BUTTON,
    ID_RESET_BUTTON,
    ID_TIMER
};
enum TimerState
{
    TIMER_BREAK,
    TIMER_LONG_BREAK,
    TIMER_RESET,
    TIMER_STOPPED,
    TIMER_WORKING
};
class Frame : public wxFrame
{

public:
    Frame();
    void OnStart(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnTimer(wxCommandEvent& event);
    void UpdateStatus(void);
    void UpdateFields(void);
private:
    wxTimer *timer;
    wxTextCtrl *longBreakIntervals;
    wxTextCtrl *breakTime;
    wxTextCtrl *workTime;
    wxTextCtrl *longBreakTime;
    wxButton *start;
    wxButton *stop;
    wxButton *reset;
    wxCheckBox *alarm;
    wxStaticText *statusText;
    wxStaticText *timeText;
    int _secs;
    int _minutes;
    int _break;
    int _work;
    int _longBreak;
    int _longBreakIntervals;
    int _interval;
    enum TimerState _timerState;
};