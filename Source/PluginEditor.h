/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CfmeltyloopAudioProcessorEditor  : public AudioProcessorEditor,
public Slider::Listener,
public Timer

{
public:
    CfmeltyloopAudioProcessorEditor (CfmeltyloopAudioProcessor&);
    ~CfmeltyloopAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    
    void sliderValueChanged(Slider* slider) override; //keep tied to params
    void timerCallback() override; //animating sliders
    
private:
    
    
    Slider mWetDrySlider;
    Slider mBarsSlider;
    Slider mSemitonesSlider;
    Slider mFadeInSlider;
    Slider mFadeOutSlider;
    Slider mFracToggle; //a slider will serve as a button in this case
    
    
    CfmeltyloopAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CfmeltyloopAudioProcessorEditor)
};
