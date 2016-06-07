/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

/*
    DocumentWindow
    DocumentWindow::LookAndFeelMethods
 http://stackoverflow.com/questions/3843411/getting-reference-to-the-top-most-view-window-in-ios-application
 */

//==============================================================================
/**
*/
class Juce_testAudioProcessorEditor  : public AudioProcessorEditor , public Timer
{
public:
    Juce_testAudioProcessorEditor (Juce_testAudioProcessor&);
    ~Juce_testAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override;
    
    void test_close_window();
    

    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Juce_testAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_testAudioProcessorEditor)
    

};


#endif  // PLUGINEDITOR_H_INCLUDED
