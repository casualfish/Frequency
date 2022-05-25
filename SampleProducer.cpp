#include "SampleProducer.h"

gboolean SampleProducer::DataArrives(GstPad *pad, GstBuffer *buffer, gpointer data)
{
	GstMapInfo map;
        SoundTransformer *transformer = (SoundTransformer*)data;
	if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                //对数据进行处理
                transformer->ComputeSamples((short*)map.data);
		gst_buffer_map(buffer, &map, GST_MAP_READ);
	}

        return FALSE;
}

SampleProducer& SampleProducer::Instance(SoundTransformer &transformer)
{
        static SampleProducer producer(transformer);
        return producer;
}

SampleProducer::SampleProducer(SoundTransformer &transformer):m_Transformer(&transformer)
{
        InitAssemblies();
        Assemble();
}

SampleProducer::~SampleProducer()
{
        gst_object_unref(m_Pipeline);
}

int SampleProducer::Start()
{
        gst_element_set_state(m_Pipeline, GST_STATE_PLAYING);
        return 0;
}

int SampleProducer::Stop()
{
        gst_element_set_state(m_Pipeline, GST_STATE_NULL);
        return 0;
}

int SampleProducer::InitAssemblies()
{
        m_Pipeline = gst_pipeline_new("sampleProducer");
        m_AudioSrc = gst_element_factory_make("alsasrc", "src");
        if(m_AudioSrc == NULL) {
              return -1;
        }

        m_Filter = gst_element_factory_make("capsfilter", "filter");
        g_assert(m_Filter != NULL); 

        return 0;
}

int SampleProducer::Assemble()
{
        GstCaps *filterCaps;
        GstPad *pad;

        gst_bin_add_many(GST_BIN(m_Pipeline), m_AudioSrc, m_Filter, NULL);
        gst_element_link_many(m_AudioSrc, m_Filter, NULL);
        filterCaps = gst_caps_new_simple("audio/x-raw", 
                                "endianness", G_TYPE_INT, 1234, 
                                "signed", G_TYPE_BOOLEAN, true, 
                                "width", G_TYPE_INT, 16, 
                                "depth", G_TYPE_INT, 16,
                                "rate", G_TYPE_INT, 44100, 
                                "channels", G_TYPE_INT, 2,
                                NULL);
        g_object_set(G_OBJECT(m_Filter), "caps", filterCaps, NULL);
        gst_caps_unref(filterCaps);
        
        pad = gst_element_get_static_pad(m_AudioSrc, "src");
        gst_pad_add_probe(pad, GST_PAD_PROBE_TYPE_BUFFER, (GstPadProbeCallback)DataArrives, (gpointer)(m_Transformer), NULL);
        gst_object_unref(pad);

        return 0;
}

