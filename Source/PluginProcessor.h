
/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "StkLite-4.6.1/DelayL.h"

//==============================================================================
/**
*/
class CfmeltyloopAudioProcessor  : public AudioProcessor
                                    
{
public:
    //==============================================================================
    CfmeltyloopAudioProcessor();
    ~CfmeltyloopAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
   

private:
    //==============================================================================
    
    //Member functions
    void calcAlgorithmParams();
    float delayIncCalc(int semitones);
    void fadeGainCalc();
    void bpmCalc();
    
    //Member objects
    AudioPlayHead* playHead; //for bpm calculation
    AudioPlayHead::CurrentPositionInfo currentPosInfo;
    stk::DelayL mDelayL;
    stk::DelayL mDelayR;
   
    
    //Member variables
    double bpm; //for bpm calculation
    int sampsPerBeat; //for bpm calculation
    float mFs, bars; //for bpm calculation
    unsigned long maxDelay; //set arbitrarily to 20 seconds
    float negGainTemp, fadeGain, fadeOut, fadeIn, countForGain, countForZero; //fade stuff
    float increment, semitones; //pitch stuff
    float wet, dry; //yeah
    stk::StkFloat delayLengthSamps, loopLengthSamps; //delayline params
    
    
    //Audio Parameters
    AudioParameterFloat *mBarsParam;
    AudioParameterBool *mFracParam;
    AudioParameterFloat *mFadeInParam;
    AudioParameterFloat *mFadeOutParam;
    AudioParameterInt *mSemitoneParam;
    AudioParameterFloat *mWetDryParam;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CfmeltyloopAudioProcessor)
};
