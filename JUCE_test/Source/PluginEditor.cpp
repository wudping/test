/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "PluginEditor_mac.h"
#include "juce_KeyPressMappingSet.h"


//==============================================================================
Juce_testAudioProcessorEditor::Juce_testAudioProcessorEditor (Juce_testAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    AudioProcessorEditor::setSize (400, 300);
    //setVisible(false);
    
    
    LOG;
    
    
    //Timer::startTimer( 3000 );
    
}

Juce_testAudioProcessorEditor::~Juce_testAudioProcessorEditor()
{
    LOG;
}




void Juce_testAudioProcessorEditor::timerCallback()
{
    LOG;
    
    //KeyPress::createFromDescription( "command+N" );
    
    //JUCEApplication::quit();
    //setVisible(false);
    
    if( Component::isShowing() == true )
    {
        Juce_testAudioProcessorEditor_test_close_window();
        stopTimer();
    }
}





//==============================================================================
void Juce_testAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
    //setVisible(false);
    
}

void Juce_testAudioProcessorEditor::resized()
{
    //Component* comp = this; //static_cast<Component*>(userData);
    //Component* top = comp->getTopLevelComponent();
    //delete top;
    
    //Button *btn = getCloseButton();
    //closeButtonPressed();
    //JUCEApplicationBase::quit();
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //setVisible(false);
    LOG;
    
}
