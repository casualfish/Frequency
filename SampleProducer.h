#ifndef __SAMPLE_PRODUCER_H_
#define __SAMPLE_PRODUCER_H_

#include <gst/gst.h>
#include "SoundTransformer.h"

//只允许有一个SampleProducer实例存在
class SampleProducer
{
public: 
        static SampleProducer& Instance(SoundTransformer &transformer);
        int Start();
        int Stop();
private:
        SampleProducer(SoundTransformer &transformer);
        virtual ~SampleProducer();
        int Assemble();
        int InitAssemblies();
        static gboolean DataArrives(GstPad *pad, GstBuffer *buffer, 
                                gpointer data);
private:
        GstElement *m_Pipeline; 
        GstElement *m_AudioSrc;
        GstElement *m_Filter;
        SoundTransformer *m_Transformer;
};

#endif          //__SAMPLE_PRODUCER_H_
