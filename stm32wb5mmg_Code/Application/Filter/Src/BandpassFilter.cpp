//! @cond
//! VTITAN CONFIDENTIAL
//! Copyright © 2014 – 2019 vTitan Corporation.
//! All Rights Reserved
//!
//! Confidential Information. The information contained herein is, and remains
//! the property of vTitan Corporation Private Ltd, Chennai, India and vTitan
//! Corporation, Pleasanton, CA USA  and its suppliers, if any, The information
//! contained here shall not be copied, used, disclosed or reproduced except as
//! specifically authorized  by vTitan Corporation.
//!
//! Biquad.hpp
//! Created on: 16-Mar-2021
//! @endcond
//!

/*
 * ButterworthBandPass.cpp
 *
 *  Created on: 18-Mar-2021
 *      Author: c10986
 */

#include "BandpassFilter.h"

namespace vTitan
{

	/**
	 * Filters with Butterworth response characteristics. The filter order is usually set
	 * via the template parameter which reserves the correct space and is then automatically
	 * passed to the setup function. Optionally one can also provide the filter
	 * order at setup time to force a lower order than the default one.
	 **/
	namespace iirButterworth
	{
		/*
		 * Holds coefficients for a second order Infinite Impulse Response
		 * digital filter. This is the building block for all IIR filters.
		 *
		 */
		//!@brief To find the first IIR co-efficient
		//!@param None
		//!@returns  1st IIR coefficient (usually one)
		//!
		double biquad_layoutBase::GetIir0() const
		{
			return m_a0;
		}

		//!@brief To find the sec IIR co-efficient
		//!@param None
		//!@returns 2nd IIR coefficient
		//!
		double biquad_layoutBase::GetIir1() const
		{
			return m_a1 * m_a0;
		}

		//!@brief To find the third IIR co-efficient
		//!@param None
		//!@returns 3rd IIR coefficient
		//!
		double biquad_layoutBase::GetIir2() const
		{
			return m_a2 * m_a0;
		}

		//!@brief To find the first FIR co-efficient
		//!@param None
		//!@returns 1st FIR coefficient
		//!
		double biquad_layoutBase::GetFir0() const
		{
			return m_b0 * m_a0;
		}

		//!@brief To find the sec FIR co-efficient
		//!@param None
		//!@returns 2nd FIR coefficient
		//!
		double biquad_layoutBase::GetFir1() const
		{
			return m_b1 * m_a0;
		}

		//!@brief To find the third FIR co-efficient
		//!@param None
		//!@returns 3rd FIR coefficient
		//!
		double biquad_layoutBase::GetFir2() const
		{
			return m_b2 * m_a0;
		}

		//!@brief Sets all coefficients of the FIR and IIR
		//!@param a0 1st IIR coefficient
		//!@param a1 2nd IIR coefficient
		//!@param a2 3rd IIR coefficient
		//!@param b0 1st FIR coefficient
		//!@param b1 2nd FIR coefficient
		//!@param b2 3rd FIR coefficient
		//!@return None
		//!
		void biquad_layoutBase::SetCoefficients(double a0, double a1, double a2, double b0, double b1, double b2)
		{
			m_a0 = a0;
			m_a1 = a1 / a0;
			m_a2 = a2 / a0;
			m_b0 = b0 / a0;
			m_b1 = b1 / a0;
			m_b2 = b2 / a0;
		}

		//!@brief Sets two poles/zoes as a pair. Needs to be complex conjugate.
		//!@return None
		//!
		void biquad_layoutBase::SetTwoPole(complx pole1, complx zero1, complx pole2, complx zero2)
		{
			const double a0 = 1;
			double a1;
			double a2;

			if (pole1.imag() != 0)
			{
				a1 = -2 * pole1.real();
				a2 = std::norm(pole1);
			}
			else
			{
				a1 = -(pole1.real() + pole2.real());
				a2 = pole1.real() * pole2.real();
			}

			const double b0 = 1;
			double b1;
			double b2;

			if (zero1.imag() != 0)
			{
				b1 = -2 * zero1.real();
				b2 = std::norm(zero1);
			}
			else
			{
				b1 = -(zero1.real() + zero2.real());
				b2 = zero1.real() * zero2.real();
			}

			SetCoefficients(a0, a1, a2, b0, b1, b2);
		}

		//!@brief Performs scaling operation on the FIR coefficients
		//!@param scale Mulitplies the coefficients b0,b1,b2 with the scaling factor scale.
		//!@returns None
		//!
		void biquad_layoutBase::ApplyScale(double scale)
		{
			m_b0 *= scale;
			m_b1 *= scale;
			m_b2 *= scale;
		}

		//! Describes a filter as a collection of poles and zeros along with
		//! normalization information to achieve a specified gain at a specified
		//! frequency. The poles and zeros may lie either in the s or the z plane.
		//!
		//!@brief Initial Storage settings
		//!@param other
		//!@return None
		//!
		void biquad_layoutBase::SetStorage(const biquad_layoutBase &other)
		{
			m_numPoles = 0;
			m_maxPoles = other.m_maxPoles;
			m_pair = other.m_pair;
		}

		//!@brief To perform reset
		//!@param None
		//!@return None
		//!
		void biquad_layoutBase::Reset()
		{
			m_numPoles = 0;
		}

		//!@brief To get the number of poles in the layout
		//!@param None
		//!@return None
		//!
		int biquad_layoutBase::GetNumPoles() const
		{
			return m_numPoles;
		}

		//!@brief To find the maximum(largest) pole in the poles list
		//!@param None
		//!@return Max of the pole
		//!
		int biquad_layoutBase::GetMaxPoles() const
		{
			return m_maxPoles;
		}

		//!@brief To add all the pole zero conjugate pairs
		//!@param None
		//!@return None
		//!
		void biquad_layoutBase::addPoleZeroConjugatePairs(const complx pole, const complx zero)
		{
			m_pair[m_numPoles / 2] = Pair_(pole, zero, std::conj(pole), std::conj(zero));
			m_numPoles += 2;
		}

		//!@brief To get poles and zeores pair
		//!@param pairIndex
		//!@return None
		//!
		const Pair_ &biquad_layoutBase::getPair(int pairIndex) const
		{
			return m_pair[pairIndex];
		}

		//!@brief Get initial normal value
		//!@param None
		//!@returns None
		//!
		double biquad_layoutBase::GetNormalW() const
		{
			return m_normalW;
		}

		double biquad_layoutBase::GetNormalGain() const
		{
			return m_normalGain;
		}

		//!@brief To set the normal value as param w and g
		//!@param w,g
		//!@returns None
		//!
		void biquad_layoutBase::SetNormalValue(double w, double g)
		{
			m_normalW = w;
			m_normalGain = g;
		}

		//!@brief To find the number of biquad stages
		//!@param None
		//!@returns number of Biquads

		int FilterBase::GetNumStages() const
		{
			return m_numStages;
		}

		//!@brief To set the cascade storage
		//!@param None
		//!@return None
		void FilterBase::SetCascadeStorage(const Storage &storage)
		{
			m_numStages = 0;
			m_maxStages = storage.maxStages;
			m_stageArray = storage.stageArray;
		}

		//!@brief Calculate filter response at the given normalized frequency
		//!@param normalizedFrequency Frequency from 0 to 0.5 (Nyquist)
		//!@return None
		//!
		complx FilterBase::Response(double normalizedFrequency) const
		{
			double w = 2 * Pi * normalizedFrequency;
			const complx czn1 = std::polar(1., -w);
			const complx czn2 = std::polar(1., -2 * w);
			complx ch = 1;
			complx cbot = 1;

			const biquad_layoutBase *stage = m_stageArray;
			for (int i = m_numStages; --i >= 0; ++stage)
			{
				complx cb = 1;
				complx ct = stage->GetFir0() / stage->GetIir0();
				ct = addmultiply(ct, stage->GetFir1() / stage->GetIir0(), czn1);
				ct = addmultiply(ct, stage->GetFir2() / stage->GetIir0(), czn2);
				cb = addmultiply(cb, stage->GetIir1() / stage->GetIir0(), czn1);
				cb = addmultiply(cb, stage->GetIir2() / stage->GetIir0(), czn2);
				ch *= ct;
				cbot *= cb;
			}
			return ch / cbot;
		}

		//!@brief To set the layout of the filter
		//!@param proto
		//!@return None
		void FilterBase::SetLayout(const biquad_layoutBase &proto)
		{
			const int numPoles = proto.GetNumPoles();
			m_numStages = (numPoles + 1) / 2;
			biquad_layoutBase *stage = m_stageArray;
			for (int i = 0; i < m_maxStages; ++i, ++stage)
				stage->SetCoefficients(1, 0, 0, 1, 0, 0);

			stage = m_stageArray;

			for (int i = 0; i < m_numStages; ++i, ++stage)
				stage->SetTwoPole(proto.getPair(i).poles.first, proto.getPair(i).zeros.first, proto.getPair(i).poles.second, proto.getPair(i).zeros.second);

			m_stageArray->ApplyScale(proto.GetNormalGain() / std::abs(Response(proto.GetNormalW() / (2 * Pi))));
		}

		//!@brief To perform bandpass transform
		//!@param fc center frequency
		//!@param fw width frquency
		//!@param digital
		//!@param analog
		//!@return None
		//!
		void FilterBase::bandPassTransform(double fc, double fw, biquad_layoutBase &digital, biquad_layoutBase const &analog)
		{

			digital.Reset();

			const double ww = (2 * Pi * fw) / 2;
			wc2 = 2 * Pi * fc - ww;
			wc = wc2 + ww;
			a = cos((wc + wc2) * 0.5) / cos((wc - wc2) * 0.5);
			b = 1 / tan((wc - wc2) * 0.5);
			a2 = a * a;
			b2 = b * b;
			ab = a * b;
			ab_2 = 2 * ab;

			const int numPoles = analog.GetNumPoles();
			const int pairs = numPoles / 2;
			for (int i = 0; i < pairs; ++i)
			{
				complx_pair p1 = Transform(analog.getPair(i).poles.first);
				complx_pair z1 = Transform(analog.getPair(i).zeros.first);

				digital.addPoleZeroConjugatePairs(p1.first, z1.first);
				digital.addPoleZeroConjugatePairs(p1.second, z1.second);
			}
			double wn = analog.GetNormalW();
			digital.SetNormalValue(2 * atan(sqrt(tan((wc + wn) * 0.5) * tan((wc2 + wn) * 0.5))), analog.GetNormalGain());
		}

		complx_pair FilterBase::Transform(complx c)
		{
			if (c == infinity())
				return complx_pair(-1, 1);

			c = (1. + c) / (1. - c); // bilinear
			complx v = 0;
			v = sqrt((((addmultiply(v, 4 * (b2 * (a2 - 1) + 1), c)) + (8 * (b2 * (a2 - 1) - 1))) * c) + (4 * (b2 * (a2 - 1) + 1)));
			complx u = -v;
			u = addmultiply(u, ab_2, c);
			u += ab_2;

			v = addmultiply(v, ab_2, c);
			v += ab_2;

			complx d = 0;
			d = addmultiply(d, 2 * (b - 1), c) + 2 * (1 + b);

			return complx_pair(u / d, v / d);
		}

		void FilterBase::SetPrototypeStorage(const biquad_layoutBase &analogStorage, const biquad_layoutBase &digitalStorage)
		{
			m_analogProto.SetStorage(analogStorage);
			m_digitalProto = digitalStorage;
		}

		//!@brief To Calculates the coefficients with the filter order provided by the instantiation
		//!@param sampleRate Sampling rate
		//!@param centerFrequency Center frequency of the bandpass
		//!@param widthFrequency Width of the bandpass
		//!@return None
		//!
		void FilterBase::Setup(int order, double sampleRate, double centerFrequency, double widthFrequency)
		{
			m_analogProto.Design(order);

			//! low pass to band pass transform
			//!
			FilterBase::bandPassTransform(centerFrequency / sampleRate,
										  widthFrequency / sampleRate,
										  m_digitalProto,
										  m_analogProto);

			FilterBase::SetLayout(m_digitalProto);
		}

		//!@State for applying a second order section to a sample using Direct Form II
		//!
		//! Difference equation:
		//!
		//! v[n] =         x[n] - (a1/a0)*v[n-1] - (a2/a0)*v[n-2]
		//! y(n) = (b0/a0)*v[n] + (b1/a0)*v[n-1] + (b2/a0)*v[n-2]
		//!
		//!@brief Reset the directFormII parameters and set value as a initial condition
		//!@param None
		//!@return NOne
		//!
		void directFormII::Reset()
		{
			m_v1 = 0;
			m_v2 = 0;

			// direct form 1
			// m_x1 = 0;
			// m_x2 = 0;
			// m_y1 = 0;
			// m_y2 = 0;
		}

		double directFormII::filter(const double in, const biquad_layoutBase &s)
		{
			double w   = in - s.m_a1*m_v1 - s.m_a2*m_v2;
			double out = s.m_b0*w + s.m_b1*m_v1 + s.m_b2*m_v2;

			m_v2 = m_v1;
			m_v1 = w;

			// direct form 1
			// double out = s.m_b0 * in + s.m_b1 * m_x1 + s.m_b2 * m_x2 - s.m_a1 * m_y1 - s.m_a2 * m_y2;
			// m_x2 = m_x1;
			// m_y2 = m_y1;
			// m_x1 = in;
			// m_y1 = out;

			return out;
		}

		//! Half-band analog prototypes (s-plane)
		//!
		analogLowPass::analogLowPass()
		{
			m_numPoles = -1;
			SetNormalValue(0, 1);
		}

		//!@brief Design an under anloglowpass filter
		//!@param numPoles Number Poles in the analoglowpass filter
		//!@returns None
		//!
		void analogLowPass::Design(int numPoles)
		{
			if (m_numPoles != numPoles)
			{
				m_numPoles = numPoles;

				Reset();

				const double n2 = 2 * numPoles;
				const int pairs = numPoles / 2;
				for (int i = 0; i < pairs; ++i)
				{
					complx c = std::polar(1., Pi_2 + (2 * i + 1) * Pi / n2);
					addPoleZeroConjugatePairs(c, infinity());
				}
			}
		}
	}
}
