#include "app.hpp"
#include "frame.hpp"

bool App::OnInit()
{
    Frame *frame = new Frame();
    frame->Show();
    return true;
}