#include "FrequencyDisplay.h"
#include "SoundTransformer.h"


gboolean FrequencyDisplay::OnTimer(TimerData *data)
{
        gtk_widget_queue_draw(data->window);
        return TRUE;
}

gboolean FrequencyDisplay::OnDraw(GtkWidget *widget, GdkEventExpose *event,  TimerData *data)
{        
        SoundTransformer *transformer = data->transformer;
        GdkWindow* window = gtk_widget_get_window(widget);

        if (window == NULL) {
                return FALSE;
        }

	int n = transformer->GetNbMaxSamples();
	int fs = transformer->GetWaveInSampleRate();
        double *amavePtr = transformer->GetTransformedData();
        double *averPtr = transformer->GetAverageFrequency();
	int x,y;

        cairo_region_t * cairoRegion = cairo_region_create();
        GdkDrawingContext * drawingContext;
        drawingContext = gdk_window_begin_draw_frame(window,cairoRegion);
        {
            cairo_t * cr = gdk_drawing_context_get_cairo_context (drawingContext);
            {
                //开始绘图 do your drawing
                int width = gdk_window_get_width(window);
                int height = gdk_window_get_height(window);

                cairo_rectangle(cr, 0, 0, width, height);
                cairo_stroke_preserve(cr);
                cairo_set_source_rgb(cr, 0, 0, 0);
                cairo_fill(cr);

                cairo_set_source_rgb(cr, 0, 255, 0);
                cairo_set_line_width (cr, 1.0);
                cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
                cairo_set_font_size(cr, 10.0);

                cairo_move_to(cr, 0.0, 3 * height / 4);
                for (x = 0; x < width; x++) {
                        y = 3 * height / 4 - (int)amavePtr[x];
                        cairo_line_to(cr, x,y);
                }

                cairo_move_to(cr, width, 3 * height / 4 - (int)averPtr[0]);
                cairo_line_to(cr, width - 20, 3 * height / 4 - (int)averPtr[0]);

                cairo_move_to(cr, 0, height / 3);
                char str[50] = {'\0'};
                cairo_move_to(cr, 0, 3 * height / 4);
                double k;
                int z;
                double div = (100.0 * n / 2) / (fs / 2);

                for(k = 0.0; k < fs / 2;) {
                        cairo_move_to(cr, (int)k, 3 * height/ 4 + 10);
                        cairo_line_to(cr, (int)k, 3 * height / 4 + 15);
                        k += div;
                }

                div = (1000.0 * n / 2) / (fs / 2);

                for(k = 0.0, z = 0; k < fs / 2; z++) {
                        snprintf(str, 50, "%dkHz", z);
                        cairo_move_to(cr, (int)k, 3 * height / 4 + 10);
                        cairo_line_to(cr, (int)k, 3 * height / 4 + 20);
                        cairo_move_to(cr, (int)k - 10, 3 * height / 4 + 25);
                        cairo_show_text(cr, str);
                        k += div;
                }


            }

            gdk_window_end_draw_frame(window, drawingContext);
        }
        cairo_region_destroy(cairoRegion);

        return FALSE;
}

FrequencyDisplay::FrequencyDisplay(SoundTransformer& transformer)
{
        m_Data.transformer = &transformer;
        m_Data.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        g_signal_connect(m_Data.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        GtkDrawingArea* drawingArea;
        drawingArea = (GtkDrawingArea*) gtk_drawing_area_new();
        gtk_container_add(GTK_CONTAINER(m_Data.window), (GtkWidget*)drawingArea);

        g_signal_connect(drawingArea, "draw", G_CALLBACK(OnDraw), (gpointer)(&m_Data));
        g_timeout_add(10, (GSourceFunc)OnTimer, (gpointer)(&m_Data));

        gtk_window_set_title(GTK_WINDOW(m_Data.window), "Frequency");
        gtk_window_set_position(GTK_WINDOW(m_Data.window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(m_Data.window), 800, 600); 
        gtk_widget_set_app_paintable(m_Data.window, TRUE);
}

FrequencyDisplay::~FrequencyDisplay()
{
}

void FrequencyDisplay::Show()
{
        gtk_widget_show_all(m_Data.window);
}

