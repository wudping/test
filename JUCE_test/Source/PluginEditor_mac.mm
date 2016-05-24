//#include "PluginEditor.h"

#include "PluginEditor_mac.h"
#import <Cocoa/Cocoa.h>


//http://stackoverflow.com/questions/3843411/getting-reference-to-the-top-most-view-window-in-ios-application
// lastObject



void Juce_testAudioProcessorEditor_test_close_window()
{
    // andrea
    NSWindow *window = [[NSApplication sharedApplication] keyWindow];
    [window close];

}