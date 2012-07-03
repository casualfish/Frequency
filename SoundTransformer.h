#ifndef __SOUND_TRANSFORMER_H_
#define __SOUND_TRANSFORMER_H_

class SoundTransformer
{
public:
        static SoundTransformer& Instance();
        int ComputeSamples(short *soundData); 
        double *GetTransformedData();
        double *GetAverageFrequency();
        int GetNbMaxSamples() const;
        int GetWaveInSampleRate() const;
private:
        SoundTransformer();
        ~SoundTransformer();
        //快速傅立叶变换
        int FFT(double *fr,int n,int flag);
private:
        int m_NbMaxSamples;
        int m_WaveInSampleRate;
        double *m_Fr;
	double *m_Fi;
	double *m_Pm;
	double *m_Am;
	double *m_Tm1;
	double *m_Tm2;
	double *m_Aver;
	double *m_Amave;
};

inline double* SoundTransformer::GetTransformedData()
{
        return m_Amave;
}

inline int SoundTransformer::GetNbMaxSamples() const
{
        return m_NbMaxSamples;
}

inline int SoundTransformer::GetWaveInSampleRate() const
{
        return m_WaveInSampleRate;
}

inline double *SoundTransformer::GetAverageFrequency()
{
        return m_Aver;
}

#endif //__SOUND_TRANSFORMER_H_
