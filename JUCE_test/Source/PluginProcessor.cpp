/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

/*
    IPC https://samtsai.org/2016/05/07/066-juce-diary-11-inter-process-communication/
    InterprocessConnection
    ChildProcessMaster
    ChildProcessSlave
 
    socket
    http://www.linuxhowtos.org/C_C++/socket.htm
 */


void*   cef_func( void *ptr )
{
    printf("test");
    system("/Users/hidog/code/test/JUCE_test/CEF_test.app/Contents/MacOS/CEF_test");
    
    pthread_exit(NULL);
}

void*   skc_reader_helper( void *ptr )
{
    Juce_testAudioProcessor *jt = (Juce_testAudioProcessor*)ptr;
    jt->read_socket();
    
    pthread_exit(NULL);
}


//==============================================================================
Juce_testAudioProcessor::Juce_testAudioProcessor()
{
    //pthread_t thr;
    //pthread_create( &thr, NULL , cef_func , NULL );
    //create_socket();
    printf("test 1");
    
    Timer::startTimer( 3000 );
}

Juce_testAudioProcessor::~Juce_testAudioProcessor()
{
}



void Juce_testAudioProcessor::read_socket()
{
    char buffer[256];
    int n;
    
    while(true)
    {
        bzero(buffer,256);
    
        n = read( skt, buffer, 255 );
        if (n < 0)
            printf("ERROR reading from socket");
    
        printf("Here is the message: %s\n",buffer);
    }
}



void Juce_testAudioProcessor::timerCallback()
{
    printf("\ntimer!!!!!!!\n");
    
    //AudioProcessorEditor *ptr =  AudioProcessor::getActiveEditor();
    //if( ptr == NULL )
      //  printf("it is null.");
    
    //Component * ptr2 = ptr->getParentComponent()->getParentComponent();
   
    //ptr->exitModalState(-1);
    //AudioProcessor::editorBeingDeleted( ptr );
    //delete ptr;
    
    //SystemStats::getComputerName();
    
    //JUCEApplication::quit();
    
    //stopTimer();
}



//
void Juce_testAudioProcessor::create_socket()
{
    int sockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    portno = atoi("9288");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        printf("ERROR on binding");
    
    listen( sockfd, 5 );
    clilen = sizeof(cli_addr);
    skt = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
    
    if (skt < 0)
        printf("ERROR on accept");
    
    pthread_t thr;
    pthread_create( &thr, NULL , skc_reader_helper , this );
}

//==============================================================================
const String Juce_testAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Juce_testAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Juce_testAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double Juce_testAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Juce_testAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Juce_testAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Juce_testAudioProcessor::setCurrentProgram (int index)
{
}

const String Juce_testAudioProcessor::getProgramName (int index)
{
    return String();
}

void Juce_testAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Juce_testAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Juce_testAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Juce_testAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void Juce_testAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool Juce_testAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Juce_testAudioProcessor::createEditor()
{
    printf("test 2");
    
    //return NULL;
    return new Juce_testAudioProcessorEditor (*this);
}

//==============================================================================
void Juce_testAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Juce_testAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Juce_testAudioProcessor();
}
