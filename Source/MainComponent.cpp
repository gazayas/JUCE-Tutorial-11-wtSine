// Part 1 - https://www.youtube.com/watch?v=xTZQJ6i-zp0&list=PLLgJJsrdwhPxa6-02-CeHW8ocwSwl2jnu&index=13
// Part 2 - https://www.youtube.com/watch?v=xTZQJ6i-zp0&list=PLLgJJsrdwhPxa6-02-CeHW8ocwSwl2jnu&index=13

/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 100);
    
    freqSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    freqSlider.setRange(50, 500);
    freqSlider.setTextValueSuffix(" Hz");
    freqSlider.setNumDecimalPlacesToDisplay(0);
    freqSlider.addListener(this);
    freqSlider.setValue(200);
    addAndMakeVisible(freqSlider);
    
    freqLabel.setText("Frequency", dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);
    
    ampSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    ampSlider.setRange(0.0, 1.0);
    ampSlider.addListener(this);
    ampSlider.setValue(0.0);
    addAndMakeVisible(ampSlider);
    
    ampLabel.setText("Amplitude", dontSendNotification);
    ampLabel.attachToComponent(&ampSlider, true);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    frequency = freqSlider.getValue();
    phase = 0;
    wtSize = 1024; // 1024 value long Array that we write one cycle of the sine wave function into
    amplitude = 0.25;
    currentSampleRate = sampleRate; // This was declared so sampleRate can be used anywhere
    
    for (int i = 0; i < wtSize; i++)
    {
        // insert values into array
        // double_Pi is just Pi. It's just a double float.
        // Here we calculate the sine wave and insert it to waveTable[i]
        waveTable.insert(i, sin(2.0 * double_Pi * i / wtSize));
    }
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // The 0 represents the left hand speaker, 1 represents the right.
    float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    // Access the buffer
    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        // phase is a float, but we need an index, so it's cast to an int here
        // play the note through both speakers at a certain amplitude
        leftSpeaker[sample] = waveTable[(int)phase] * amplitude;
        rightSpeaker[sample] = waveTable[(int)phase] * amplitude;
        
        updateFrequency();
    }
}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    const int labelSpace = 100;
    freqSlider.setBounds(labelSpace, 20, getWidth() - 100, 20);
    ampSlider.setBounds(labelSpace, 50, getWidth() - 100, 20);
}
