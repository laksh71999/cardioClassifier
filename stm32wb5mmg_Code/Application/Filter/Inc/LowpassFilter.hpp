#include <stdlib.h>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <complex>
#include <cstring>
#include <string>
#include <limits>
#include <vector>
#include <stdexcept>

namespace vTitan
{
    const double Pi	= 3.1415926535897932384626433832795028841971;
    const double Pi_2 = 1.5707963267948966192313216916397514420986;

	typedef std::complex<double> complx;
	typedef std::pair<complx, complx> complx_pair;

	inline const complx infinity()
	{
		return complx (std::numeric_limits<double>::infinity());
	}

	inline std::complex<double> addmultiply(const std::complex<double>& c, double v, const std::complex<double>& c1)
	{
		double real_part = c.real() + v * c1.real();
    	double imag_part = c.imag() + v * c1.imag();
    	return std::complex<double>(real_part, imag_part);
	}

	//!A conjugate or real pair
	//!
	struct  Pair_: complx_pair
	{
		complx_pair poles;
		complx_pair zeros;

		Pair_() { }

		Pair_(const complx& c1, const complx& c2) : complx_pair (c1, c2){}

		Pair_(const complx& p1, const complx& z1,const complx& p2, const complx& z2): poles (p1, p2), zeros (z1, z2){}
	};

	class  biquad_layoutBase
	{
	public:
		biquad_layoutBase () : m_numPoles (0) , m_maxPoles (0){}

		biquad_layoutBase (int maxPoles, Pair_* pairs) : m_numPoles (0) , m_maxPoles (maxPoles) , m_pair (pairs){}

		void SetStorage (const biquad_layoutBase& other);
		void Reset ();
		int GetNumPoles () const;
		int GetMaxPoles () const;
		void addPoleZeroConjugatePairs (const complx pole,const complx zero);
        void add (const complx pole,const complx zero);
		const Pair_& getPair (int pairIndex) const;
		double GetNormalW () const;
		double GetNormalGain () const;
		void SetNormalValue (double w, double g);
		void SetCoefficients (double a0, double a1, double a2, double b0, double b1, double b2);
		void SetTwoPole (complx pole1, complx zero1,complx pole2, complx zero2);
		void ApplyScale (double scale);
		double GetIir0 () const;
		double GetIir1 () const;
		double GetIir2 () const;
		double GetFir0 () const;
		double GetFir1 () const;
		double GetFir2 () const;

	public:
		double m_a0 = 1;
		double m_a1 = 0;
		double m_a2 = 0;
		double m_b1 = 0;
	    double m_b2 = 0;
		double m_b0 = 1;

	private:
		int m_numPoles;
		int m_maxPoles;
		Pair_* m_pair;
		double m_normalW;
		double m_normalGain = 1;
	};

	class  analogLowPass : public biquad_layoutBase
	{
		public:
			analogLowPass ();
			void Design (const int numPoles);

		private:
			int m_numPoles;
	};

	class FilterBase 
	{
		public:
		    // Pointer to an array of biquads
   		    struct Storage 
            {
    		    // Constructor which receives the pointer to the biquad array and the number of biquads
       	    	// @param maxStages_ Number of biquads
       	    	// @param stageArray_ The array of the biquads
       		    Storage(int maxStages_, biquad_layoutBase* stageArray_) : maxStages(maxStages_) , stageArray(stageArray_){}
        	    int maxStages;
        	    biquad_layoutBase* stageArray;
    	    };
    
    	    // Returns the number of stages in the filter
    	    int GetNumStages() const;
    
    	    // Returns the complex frequency response of the filter at a given normalized frequency
    	    complx Response(double normalizedFrequency) const;
    
    	    // Sets up the filter with a given order, sample rate, and center frequency
    	    void Setup(int order, double sampleRate, double centerFrequency);
    
    	    // Applies a low-pass transform to the filter with a given cutoff frequency and stores the result in a digital biquad layout
    	    void lowPassTransform(double fc, biquad_layoutBase& digital, const biquad_layoutBase& analog);
    
		protected:
    	    // Holds coefficients for a cascade of second-order sections
    	    FilterBase() : m_numStages(0) , m_maxStages(0) , m_stageArray(nullptr){}
    
    		// Sets the storage for the filter's cascade of biquad sections
    		void SetCascadeStorage(const Storage& storage);
    
    		// Applies a scaling factor to the filter's coefficients
    		void applyScale(double scale);
    
    		// Sets the filter's prototype layout for analog and digital filters
    		void SetPrototypeStorage(const biquad_layoutBase& analogStorage, const biquad_layoutBase& digitalStorage);
    
    		// Sets the layout for the filter's biquad sections
   			void SetLayout(const biquad_layoutBase& proto);
    
		protected:
    		analogLowPass m_analogProto;
    		biquad_layoutBase m_digitalProto;
    
		private:
    		int m_numStages;
    		int m_maxStages;
    		biquad_layoutBase* m_stageArray;
    		complx Transform(complx c);
    		double f;
	};

	//Storage for Cascade: This holds a chain of 2nd order filters
	//with its coefficients.
	template <int MaxStages,class StateType>
	class  cascadeStages
	{
		public:
			operator biquad_layoutBase ()
			{
				return biquad_layoutBase (MaxStages, m_pairs);
			}

			//!@brief Filters one sample through the whole chain of biquads and return the result
			//!@param in Sample to be filtered
			//!
			inline int16_t filter(const int16_t in)
			{
				double out = in;
				StateType* state = m_states;
				biquad_layoutBase const* stage = m_stages;
				for (int i = MaxStages; --i >= 0; ++state)
				{
					out = state->filter(out, *stage);
					++stage;
				}
				return static_cast<int16_t> (out);
			}

			FilterBase::Storage GetCascadeStorage()
			{
				return FilterBase::Storage (MaxStages, m_stages);
			}

		private:
			biquad_layoutBase m_stages[MaxStages];
			StateType m_states[MaxStages];
			Pair_ m_pairs[(MaxStages+1)/2];
	};

	template <class BaseClass, class StateType,  int MaxAnalogPoles,  int MaxDigitalPoles=MaxAnalogPoles >
	struct analogDigitalPoles : BaseClass , cascadeStages <(MaxDigitalPoles + 1) / 2 , StateType>
	{
		analogDigitalPoles ()
	    {
			// This glues together the factored base classes
			// with the templatized storage classes.
			BaseClass::SetCascadeStorage (this->GetCascadeStorage());
			BaseClass::SetPrototypeStorage (m_analogStorage, m_digitalStorage);
		}

		private:
		    cascadeStages <MaxAnalogPoles,StateType >m_analogStorage;
			cascadeStages <MaxDigitalPoles,StateType > m_digitalStorage;
	};

	class  directFormII
	{
		public:
			directFormII () { Reset ();	}

			void Reset ();

			double filter(const double in,const biquad_layoutBase& s);

		private:
			double m_v1 = 0;
			double m_v2 = 0;
	};

	//!@brief Butterworth  Lowpass filter initialization .
	//!@param FilterOrder  Reserves memory for a filter of the order FilterOrder
	//!@param StateType The filter topology: DirectFormI, DirectFormII, ...
	//!
	template <int FilterOrder, class StateType = directFormII>
	struct  ButterworthLowPass : analogDigitalPoles <FilterBase, StateType, FilterOrder, FilterOrder*2>
	{
		void Setup (double sampleRate,double cutoffFrequency)
		{
			FilterBase::Setup(FilterOrder,sampleRate,cutoffFrequency);
		}
	};
}