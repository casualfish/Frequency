#ifndef __FREQUENCY_DISPLAY_H_
#define __FREQUENCY_DISPLAY_H_

#include <gtk/gtk.h>
#include <cairo.h>
#include "SoundTransformer.h"

typedef struct _TimerData
{
        GtkWidget *window;
        SoundTransformer *transformer;
}TimerData;

class FrequencyDisplay
{
public:
        FrequencyDisplay(SoundTransformer& transformer);
        virtual ~FrequencyDisplay();
        void Show();
private:
        static gboolean OnDraw(GtkWidget *widget, GdkEventExpose *event, TimerData *data);
        static gboolean OnTimer(TimerData *data);
private:
        TimerData m_Data;
};

#endif //__FREQUENCY_DISPLAY_H_

