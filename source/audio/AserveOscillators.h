/*
 *  AserveOscillators.h
 *
 *  Created by Thomas Mitchell on 25/09/2008.
 *
 */
#ifndef H_ASERVEOSCILLATORS
#define H_ASERVEOSCILLATORS

class Oscillator
{	
	public:
		Oscillator();
		virtual ~Oscillator();
		virtual void reset(void);
		virtual void setFrequency(const double val);
		void setAmplitude(const float val);
		virtual void prepare(double sampleRate_);
		void stop(void);
		double nextSample(void);
		virtual double renderWaveShape(void) = 0;
    
protected:
	double frequency, amplitude, sampleRate, tailOff; //used by derrived classes
};

class SineOscillator : public Oscillator
{
	private:
		double currentAngle, angleDelta;
	
	public:
		SineOscillator();
		virtual ~SineOscillator();
		void reset(void);
		void setFrequency(const double val);
		double renderWaveShape(void);
};

class SquareOscillator : public Oscillator
{
	private:		
		void updateHarmonics( void );
		
		unsigned int nHarmonics_, m_;
		double rate_, phase_;
		double p_,a_;
		double lastBlitOutput_, dcbState_, lastOutput_;
		
	public:
		SquareOscillator();
		virtual ~SquareOscillator();
		
		void reset(void);
		void prepare(double sampleRate_);
		void setFrequency(const double val);
		double renderWaveShape(void);
};

class SawOscillator : public Oscillator
{
	private:
		void updateHarmonics( void );

		unsigned int nHarmonics_, m_;
		double rate_, phase_;
		double p_,a_,C2_;
		double state_, lastOutput_;
		
	public:
		SawOscillator();
		~SawOscillator();
		
		void reset(void);
		void setFrequency(const double val);
		void prepare(double sampleRate_);
		double renderWaveShape(void);
};

class WaveOscillator
{
	private:
		int wave;							//sets the current wavesahpe SIN = 0, SQUR = 1, SAW = 2
		SineOscillator sinOscillator;
		SquareOscillator squareOscillator;
		SawOscillator sawOscillator;
	public:
    enum Constants 
    {
        Sinusoid = 0,
        Square,
        Sawtooth,
        
        NumWaveforms
    };
		WaveOscillator();
		~WaveOscillator();
		void setWave(int val);
		void setFrequency(const double val);
		void setAmplitude(const float val);
		void prepare(double sampleRate_);
		void stop(void);
		double nextSample(void);
};

#endif   // H_ASERVEOSCILLATORS