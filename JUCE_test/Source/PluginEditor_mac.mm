//#include "PluginEditor.h"

#include "PluginEditor_mac.h"
#import <Cocoa/Cocoa.h>


void Juce_testAudioProcessorEditor_test_close_window()
{
    // andrea
    NSWindow *window = [[NSApplication sharedApplication] keyWindow];
    [window close];

}