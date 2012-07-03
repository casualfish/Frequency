#include "SampleProducer.h" 
#include "FrequencyDisplay.h"
#include "SoundTransformer.h"

/* 
 * 对采集的声音进行分析并显示结果
 * by casualfish. 2011.5.18
 */

int main(int argc, char *argv[])
{
        //loop的初始化必须在最前
        GMainLoop *loop;

        gst_init(&argc, &argv);
        gtk_init(&argc, &argv);

        loop = g_main_loop_new(NULL, FALSE);

        SoundTransformer &transformer = SoundTransformer::Instance();
        SampleProducer &producer = SampleProducer::Instance(transformer);
        FrequencyDisplay display(transformer);

        producer.Start();
        display.Show();

        g_print("Running ...\n");

        gtk_main();
        g_main_loop_run(loop);
        producer.Stop();

        return 0;
}

