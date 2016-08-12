/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licences for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licencing:
*
*   http://www.hartinstruments.net/hise/
*
*   HISE is based on the JUCE library,
*   which also must be licenced for commercial applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/



#ifndef TCCDSPOBJECT_H_INCLUDED
#define TCCDSPOBJECT_H_INCLUDED

#include "JuceHeader.h"



class TccDspObject : public DspBaseObject
{
public:

	TccDspObject(const String &code);

	void prepareToPlay(double sampleRate, int blockSize) override { if (pp != nullptr) pp(sampleRate, blockSize); }
	void processBlock(float **data, int numChannels, int numSamples) override { if (pb != nullptr) pb(data, numChannels, numSamples); }

	// =================================================================================================================

	
	int getNumParameters() const override { if (gnp != nullptr) return gnp(); else return 0; }
	float getParameter(int index) const override { if (gp != nullptr) return gp(index); else return 0.0f; }
	void setParameter(int index, float newValue) override { if (sp != nullptr) sp(index, newValue); }

#if 0
	const char* getStringParameter(int index, size_t& textLength) override;
	void setStringParameter(int index, const char* text, size_t textLength) override;

	// =================================================================================================================

	int getNumConstants() const override;
	void getIdForConstant(int index, char* name, int &size) const noexcept override;
	bool getConstant(int index, float& value) const noexcept override;
	bool getConstant(int index, int& value) const noexcept override;
	bool getConstant(int index, char* text, size_t& size) const noexcept override;
	bool getConstant(int index, float** data, int &size) noexcept override;

#endif

private:

	struct Signatures
	{
		using prepareToPlay = void(*)(double sampleRate, int blockSize);
		using processBlock = void(*)(float **data, int numChannels, int numSamples);
		using getNumParameters = int(*)();
		using getParameter = float(*)(int index);
		using setParameter = void(*)(int index, float newValue);
		using getStringParameter = const char* (*)(int index, size_t& textLength);
		using setStringParameter = void(*)(int index, const char* text, size_t textLength);
		using getNumConstants = int(*)();
		using getIdForConstant = void(*)(int index, char* name, int &size);
		using getFloatConstant = bool(*)(int index, float& value);
		using getIntConstant = bool(*)(int index, int& value);
		using getStringConstant = bool(*)(int index, char* text, size_t& size);
		using getVectorConstant = bool(*)(int index, float** data, int &size);
	};

	
	Signatures::prepareToPlay pp;
	Signatures::processBlock pb;
	Signatures::getNumParameters gnp;
	Signatures::setParameter sp;
	Signatures::getParameter gp;

	ScopedPointer<TccContext> context;

	bool compiledOk;
};

class TccDspFactory : public DspFactory
{
public:

	TccDspFactory()
	{

	};

	Identifier getId() const override { RETURN_STATIC_IDENTIFIER("TCC"); }

	var createModule(const String &module) const override;

	/** This method is called by the DspInstance object to create a instance of a DSP module. */
	DspBaseObject *createDspBaseObject(const String &module) const override;

	/** This method is called by the DspInstance object's destructor to delete the module. */
	void destroyDspBaseObject(DspBaseObject *object) const override;

	/** This method must return an array with all module names that can be created. */
	var getModuleList() const override { return var::undefined(); }
};

#endif  // TCCDSPOBJECT_H_INCLUDED