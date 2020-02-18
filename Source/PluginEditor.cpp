/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CfmeltyloopAudioProcessorEditor::CfmeltyloopAudioProcessorEditor (CfmeltyloopAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (400, 300);
    startTimer(100); //for animation of sliders during automation
    
    auto& params = processor.getParameters();
    
    //bars
    AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
    mBarsSlider.setBounds(100, 50, 200, 200);
    mBarsSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mBarsSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mBarsSlider.setRange(audioParam->range.start, audioParam->range.end);
    mBarsSlider.setValue(*audioParam);
    mBarsSlider.addListener(this);
    addAndMakeVisible(mBarsSlider);
    
    //fraction switch
    audioParam = (AudioParameterFloat*)params.getUnchecked(1);
    mFracToggle.setBounds(70, 20, 20, 50);
    mFracToggle.setSliderStyle(Slider::SliderStyle::LinearVertical);
    mFracToggle.setTextBoxStyle(Slider::TextBoxLeft, false, 45, 20);
    mFracToggle.setRange(audioParam->range.start, audioParam->range.end);
    mFracToggle.setValue(*audioParam);
    mFracToggle.addListener(this);
    addAndMakeVisible(mFracToggle);
    
    //fade in
    audioParam = (AudioParameterFloat*)params.getUnchecked(2);
    mFadeInSlider.setBounds(0, 20, 50, 200);
    mFadeInSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    mFadeInSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mFadeInSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFadeInSlider.setValue(*audioParam);
    mFadeInSlider.addListener(this);
    addAndMakeVisible(mFadeInSlider);
    
    //fade out
    audioParam = (AudioParameterFloat*)params.getUnchecked(3);
    mFadeOutSlider.setBounds(350, 20, 50, 200);
    mFadeOutSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    mFadeOutSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mFadeOutSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFadeOutSlider.setValue(*audioParam);
    mFadeOutSlider.addListener(this);
    addAndMakeVisible(mFadeOutSlider);
    
    //semitones
    audioParam = (AudioParameterFloat*)params.getUnchecked(4);
    mSemitonesSlider.setBounds(160, 100, 80, 80);
    mSemitonesSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mSemitonesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mSemitonesSlider.setRange(audioParam->range.start, audioParam->range.end);
    mSemitonesSlider.setValue(*audioParam);
    mSemitonesSlider.addListener(this);
    addAndMakeVisible(mSemitonesSlider);
    
    //wet/dry
    audioParam = (AudioParameterFloat*)params.getUnchecked(5);
    mWetDrySlider.setBounds(275, 20, 80, 80);
    mWetDrySlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mWetDrySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mWetDrySlider.setRange(audioParam->range.start, audioParam->range.end);
    mWetDrySlider.setValue(*audioParam);
    mWetDrySlider.addListener(this);
    addAndMakeVisible(mWetDrySlider);
    
}

void CfmeltyloopAudioProcessorEditor::sliderValueChanged(Slider *slider){
    auto& params = processor.getParameters();
    
    //bars
    if(slider == &mBarsSlider){
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = mBarsSlider.getValue();
    }
    //fraction switch
    else if(slider == &mFracToggle){
        AudioParameterBool* audioParam = (AudioParameterBool*)params.getUnchecked(1);
        *audioParam = mFracToggle.getValue();
    }
    //fade in
    else if(slider == &mFadeInSlider){
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = mFadeInSlider.getValue();
    }
    //fade out
    else if(slider == &mFadeOutSlider){
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(3);
        *audioParam = mFadeOutSlider.getValue();
    }
    //semitones
    else if(slider == &mSemitonesSlider){
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(4);
        *audioParam = mSemitonesSlider.getValue();
    }
    //wet/dry
    else if(slider == &mWetDrySlider){
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(5);
        *audioParam = mWetDrySlider.getValue();
    }
}

CfmeltyloopAudioProcessorEditor::~CfmeltyloopAudioProcessorEditor()
{
}

//==============================================================================
void CfmeltyloopAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::darkblue);

    /* I considered trying to make this more elegant seeing as it is my final project,
       but I decided placing GUI items in juce is too finicky and that
       hard coding positions is easier (same with adding the sliders) */
    
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("Loop Length (bars)", 150, 220, 100, 100, 1, 0);
    g.setFont (12.0f);
    g.drawFittedText("Pitch", 190, 40, 20, 80, 1, 0);
    g.drawFittedText("Down", 190, 60, 20, 80, 1, 0);
    g.setFont (14.0f);
    g.drawFittedText("1/Bars", 93, 0, 50, 80, 1, 0);
    g.drawFittedText("WetDry", 245, 0, 50, 80, 1, 0);
    g.setFont (15.0f);
    g.drawFittedText("Fade", 12, 190, 30, 80, 1, 0);
    g.drawFittedText("In", 12, 205, 30, 80, 1, 0);
    g.drawFittedText("Fade", 358, 190, 30, 80, 1, 0);
    g.drawFittedText("Out", 363, 205, 30, 80, 1, 0);
 
}

void CfmeltyloopAudioProcessorEditor::resized()
{
}

void CfmeltyloopAudioProcessorEditor::timerCallback(){
    auto& params = processor.getParameters();
    
    //bars
    AudioParameterFloat* mBarsParam = (AudioParameterFloat*)params.getUnchecked(0);
    mBarsSlider.setValue(mBarsParam->get(), dontSendNotification);
    
    //fraction switch
    AudioParameterBool* mFracParam = (AudioParameterBool*)params.getUnchecked(1);
    mFracToggle.setValue(mFracParam->get(), dontSendNotification);
    
    //fade in
    AudioParameterFloat* mFadeInParam = (AudioParameterFloat*)params.getUnchecked(2);
    mFadeInSlider.setValue(mFadeInParam->get(), dontSendNotification);
    
    //fade out
    AudioParameterFloat* mFadeOutParam = (AudioParameterFloat*)params.getUnchecked(3);
    mFadeOutSlider.setValue(mFadeOutParam->get(), dontSendNotification);
    
    //semitones
    AudioParameterFloat* mSemitonesParam = (AudioParameterFloat*)params.getUnchecked(4);
    mSemitonesSlider.setValue(mSemitonesParam->get(), dontSendNotification);
    
    //wet/dry
    AudioParameterFloat* mWetDryParam = (AudioParameterFloat*)params.getUnchecked(5);
    mWetDrySlider.setValue(mWetDryParam->get(), dontSendNotification);
    
}
