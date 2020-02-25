/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &freqSlider)
        {
            frequency = freqSlider.getValue();
            std::cout << frequency << std::endl;
        } else if (slider == &ampSlider)
        {
            amplitude = ampSlider.getValue();
        }
    }
    
    void updateFrequency () {
        increment = frequency * wtSize / currentSampleRate; // sampleRate should be something like 44.1 kHz
        
        // What happens when we get to the end of the array?
        // We need to start over
        // If the index goes over wtSize, fmod makes go back to the beginning of the wave
        phase = fmod ((phase + increment), wtSize);
    }

private:
    //==============================================================================
    Slider freqSlider, ampSlider;
    Label freqLabel, ampLabel;
    
    Array<float> waveTable;
    double wtSize;
    double frequency;
    double phase;
    double increment;
    double amplitude;
    double currentSampleRate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
