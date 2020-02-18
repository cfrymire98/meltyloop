
#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================


CfmeltyloopAudioProcessor::CfmeltyloopAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif

{
    //adding the params and such
    addParameter(mBarsParam = new AudioParameterFloat("Bars", "Bars", 1.0f, 8.0f, 2.0f));
    addParameter(mFracParam = new AudioParameterBool("Fraction toggle", "Fraction Toggle", 0));
    addParameter(mFadeInParam = new AudioParameterFloat("Fade In %", "Fade In", 0.0f, 100.0f, 0.0f));
    addParameter(mFadeOutParam = new AudioParameterFloat("Fade Out %", "Fade Out", 0.0f, 100.0f, 0.0f));
    addParameter(mSemitoneParam = new AudioParameterInt("Semitone Pitch", "Semitones", 0, 24, 12));
    addParameter(mWetDryParam = new AudioParameterFloat("Wet/Dry", "Wet/Dry", 0.0f, 100.0f, 50.0f));
}
CfmeltyloopAudioProcessor::~CfmeltyloopAudioProcessor()
{
}

//==============================================================================
const String CfmeltyloopAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CfmeltyloopAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CfmeltyloopAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CfmeltyloopAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CfmeltyloopAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CfmeltyloopAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CfmeltyloopAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CfmeltyloopAudioProcessor::setCurrentProgram (int index)
{
}

const String CfmeltyloopAudioProcessor::getProgramName (int index)
{
    return {};
}

void CfmeltyloopAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CfmeltyloopAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mFs = sampleRate;
    
    //max delay here, this is arbitrary but 20s seems fair
    maxDelay = 20000 * mFs/1000;
    mDelayL.setMaximumDelay(maxDelay);
    mDelayR.setMaximumDelay(maxDelay);
    
    delayLengthSamps = 1.0;
    mDelayL.setDelay(delayLengthSamps); //probably not necessary, but just in case
    mDelayR.setDelay(delayLengthSamps);
    
    countForZero = 1.0;
}

void CfmeltyloopAudioProcessor::releaseResources()
{
    //without this code the loop stays saved after playback, which gets weird
    mDelayL.clear();
    mDelayR.clear();
    delayLengthSamps = 1.0;
    countForZero = 1.0;

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CfmeltyloopAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CfmeltyloopAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);

    calcAlgorithmParams();
    
    float outL, outR, dryL, dryR;
    
    for (int samp = 0; samp < buffer.getNumSamples(); samp++){
        
        
        if(delayLengthSamps < loopLengthSamps){
            
            dryL = channelDataLeft[samp] * dry;
            dryR = channelDataRight[samp] * dry;
            
            outL = mDelayL.tick(channelDataLeft[samp]);
            outR = mDelayR.tick(channelDataRight[samp]);
            delayLengthSamps += increment;
            mDelayL.setDelay(delayLengthSamps);
            mDelayR.setDelay(delayLengthSamps);
            fadeGainCalc();
            channelDataLeft[samp] = (outL * fadeGain * wet) + dryL;
            channelDataRight[samp] = (outR * fadeGain * wet) + dryR;
            
            countForZero++;
        }
        
        else{
            dryL = channelDataLeft[samp] * dry;
            dryR = channelDataRight[samp] * dry;
            
            delayLengthSamps = 1.0;
            mDelayL.setDelay(delayLengthSamps);
            mDelayR.setDelay(delayLengthSamps);
            outL = mDelayL.tick(channelDataLeft[samp]);
            outR = mDelayR.tick(channelDataRight[samp]);
            fadeGainCalc();
            channelDataLeft[samp] = (outL * fadeGain * wet) + dryL;
            channelDataRight[samp] = (outR * fadeGain * wet) + dryR;
            
            countForZero = 1.0;
        }
        
    }
    
}


//==============================================================================
bool CfmeltyloopAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CfmeltyloopAudioProcessor::createEditor()
{
    return new CfmeltyloopAudioProcessorEditor (*this);
}

//==============================================================================
void CfmeltyloopAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CfmeltyloopAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


void CfmeltyloopAudioProcessor::calcAlgorithmParams(){
    
    //accessing bars param then getting loop length in samps
    bars = mBarsParam->get();
    if(mFracParam->get() == 1) bars = 1/bars;
    bpmCalc();
    
    //wet/dry linear gain calc
    wet = mWetDryParam->get()/100.0;
    dry = 1 - wet;
    
    //pitch shift
    semitones = mSemitoneParam->get();
    increment = delayIncCalc(semitones);

    //fade vals
    fadeIn = mFadeInParam->get()/100.0 * loopLengthSamps;
    fadeOut = mFadeOutParam->get()/100.0 * loopLengthSamps;
   
}

float CfmeltyloopAudioProcessor::delayIncCalc(int semitones){
    double power = (double)semitones / 12.0;
    float toSub = (float)1/(pow(2.0, power));
    return 1.0 - toSub;
}

void CfmeltyloopAudioProcessor::fadeGainCalc(){
    
    //if(mSemitoneParam->get() == 0) countForGain = countForZero;
    /*else*/ countForGain = delayLengthSamps; //fix this functionality before packaged release
    
    //I don't remember how I came up with this but it works
    negGainTemp = fadeOut - (loopLengthSamps - countForGain);
    
    //if the user breaks the rules, fade in takes priority
    if(fadeIn + fadeOut > loopLengthSamps) fadeOut = loopLengthSamps - fadeIn;
    
    //fading in
    if(countForGain < fadeIn) {
        fadeGain = (1/fadeIn) * countForGain;
        if(fadeGain > 1) fadeGain = 1;
    }
    //fading out
    else if(countForGain > (loopLengthSamps - fadeOut)){
        fadeGain = (1-(1/fadeOut * negGainTemp));
        if(fadeGain > 1) fadeGain = 1;
    }
    //area between fades
    else fadeGain = 1;
    
}

void CfmeltyloopAudioProcessor::bpmCalc(){
    
    //using the bpm from the playhead and bars to get loop length in samps
    playHead = this->getPlayHead();
    playHead->getCurrentPosition (currentPosInfo);
    bpm = currentPosInfo.bpm;
    sampsPerBeat = (60/bpm)*mFs;
    loopLengthSamps = sampsPerBeat * bars * 4;
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CfmeltyloopAudioProcessor();
}
