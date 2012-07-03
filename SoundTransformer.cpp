#include <math.h>
#include <cstdio>
#include "SoundTransformer.h"

#define MAX_SAMPLES 2048
#define SAMPLE_RATE 44100
#define PI (2 * asin(1.0))
#define DELETE_S(arrayPtr) if(arrayPtr != NULL) {\
                delete [] arrayPtr;\
                arrayPtr = NULL;\
        }

SoundTransformer& SoundTransformer::Instance()
{
        static SoundTransformer transformer;
        return transformer;
}

SoundTransformer::SoundTransformer()
{
        m_NbMaxSamples = MAX_SAMPLES;
        m_WaveInSampleRate = SAMPLE_RATE;

        m_Fr = new double[m_NbMaxSamples];
	m_Fi = new double[m_NbMaxSamples];
	m_Pm = new double[m_NbMaxSamples / 2];
	m_Am = new double[m_NbMaxSamples / 2];
	m_Tm1 = new double[m_NbMaxSamples];
	m_Tm2 = new double[m_NbMaxSamples];
	m_Aver = new double[m_NbMaxSamples/2];
	m_Amave = new double[m_NbMaxSamples/2];

	for(int i = 0; i < m_NbMaxSamples / 2; i++) {
		m_Amave[i] = 0;
        }
}

SoundTransformer::~SoundTransformer()
{
        DELETE_S(m_Fr);
        DELETE_S(m_Fi);
        DELETE_S(m_Pm);
        DELETE_S(m_Am);
        DELETE_S(m_Tm1);
        DELETE_S(m_Tm2);
        DELETE_S(m_Aver);
        DELETE_S(m_Amave);
}

int SoundTransformer::ComputeSamples(short *soundData)
{
        int i, n = m_NbMaxSamples;

	for(i = 0; i < n; i++) {
                if(i < n) {
                        m_Fr[i] = soundData[i] * (1.0 / exp(18*((n) / 2.0 - i) 
                                * ((n) / 2.0 - i) / (n * n)));
                } else {
                        m_Fr[i] = 0;
                }
		m_Fi[i] = 0;
	}

	FFT(m_Fr, n, 0);
	m_Aver[0] = 1.0;

	for(i = 0; i< n / 2; i++) {
		m_Pm[i] = m_Fr[i] * m_Fr[i] + m_Fi[i] * m_Fi[i];
		m_Am[i] = sqrt(m_Pm[i]) / 10000;
		if(i < n / 4)
			m_Aver[0] += m_Am[i];
		m_Amave[i] = (m_Amave[i] + m_Am[i]) / 2;
	}

	m_Aver[0] /= (n/4);
        return 0;
}

int SoundTransformer::FFT(double *fr,int n,int flag)
{
	int mp, arg, cntr, p1, p2;
	int i, j, a, b, k;
	double sign, pr, pi, harm, t;
	double *ca, *sa;
	
	ca = new double[n];
	sa = new double[n];

	if(ca == NULL || sa == NULL) {
		return 0;
        }

	j = 0;
	if(flag != 0) {
		sign = 1.0;
		for(i = 0;i <= n-1; ++i) {
			fr[i] = fr[i] / n;
			m_Fi[i] = m_Fi[i] / n;
		}
	} else {
                sign = -1.0;
        }

	for(i = 0;i < n-2; ++i) {
		if(i < j) {
			t = fr[i];
			fr[i] = fr[j];
			fr[j] = t;
			t = m_Fi[i];
			m_Fi[i] = m_Fi[j];
			m_Fi[j] = t;
		}           
		k = n / 2;
		while(k <= j) {
			j -= k;
			k /= 2;
		}        
		j += k;
	}        

	mp = 0;
	i = n;

	while(i != 1) {
		mp += 1;
		i /= 2;
	}        
        
	harm = 2 * PI / n;
	for(i = 0; i <= n - 1; ++i) {
		sa[i] = sign* sin(harm * i);
		ca[i] = cos(harm * i);
	}                     

	a = 2;
	b = 1;
	for(cntr = 1; cntr <= mp; ++cntr) {
		p1 = n / a;
		p2 = 0;
		for(k = 0; k <= b - 1; ++k) {
			i = k;
			while(i < n) {
				arg = i + b;
				if(k == 0) {
					pr = fr[arg];
					pi = m_Fi[arg];
				} else {
					pr = fr[arg] * ca[p2] 
                                                - m_Fi[arg] * sa[p2];
					pi = fr[arg] * sa[p2] 
                                                + m_Fi[arg] * ca[p2];
				}                                    
				fr[arg] = fr[i]-pr;
				m_Fi[arg] = m_Fi[i]-pi;
				fr[i] += pr;
				m_Fi[i] += pi;
				i += a; 
			}
			p2 += p1;
		}            
		a *= 2;
		b *= 2;
	}          

	delete[] ca;
	delete[] sa;
	return 1;
}

