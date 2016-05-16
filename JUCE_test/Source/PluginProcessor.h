/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
//#include "PluginEditor.h"


/*
 http://stackoverflow.com/questions/11444261/objective-c-how-to-put-a-close-button-in-window
 NSWindow *window = [[NSApplication sharedApplication] keyWindow];
 [window close];
 
 NSWindow *window = [[NSApplication sharedApplication] keyWindow];
 [window performClose:sender];
 
 */


//==============================================================================
/**
*/
class Juce_testAudioProcessor  : public AudioProcessor, public Timer
{
public:
    //==============================================================================
    Juce_testAudioProcessor();
    ~Juce_testAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
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
    
    
    

    
    void create_socket();
    void read_socket();
    
    
    
    void timerCallback() override;


private:
    int skt;

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_testAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
