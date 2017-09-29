/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorder.h"
#include "RecordingThumbnail.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
	private AudioIODeviceCallback,
	private Button::Listener
{
public:
    //==============================================================================
    MainContentComponent() : audioSetupComp(audioDeviceManager, 0, 0, 0, 256,
		true, // showMidiInputOptions must be true
		true, true, false), //deviceManager.getSharedAudioDeviceManager(),
		recorder(recordingThumbnail.getAudioThumbnail())
    {

		audioDeviceManager.initialise(0, 2, nullptr, true, String(), nullptr);
		//audioDeviceManager.addMidiInputCallback(String(), this); // [6]
		audioDeviceManager.addAudioCallback(this);

		setOpaque(true);

		addAndMakeVisible(explanationLabel);
		explanationLabel.setText("This page demonstrates how to record a wave file from the live audio input..\n\nPressing record will start recording a file in your \"Documents\" folder.", dontSendNotification);
		explanationLabel.setFont(Font(15.00f, Font::plain));
		explanationLabel.setJustificationType(Justification::topLeft);
		explanationLabel.setEditable(false, false, false);
		explanationLabel.setColour(TextEditor::textColourId, Colours::black);
		explanationLabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		addAndMakeVisible(recordButton);
		recordButton.setButtonText("Record");
		recordButton.addListener(this);
		recordButton.setColour(TextButton::buttonColourId, Colour(0xffff5c5c));
		recordButton.setColour(TextButton::textColourOnId, Colours::black);

		addAndMakeVisible(recordingThumbnail);

		deviceManager.addAudioCallback(&recorder);

        setSize (800, 600); //i kept this.

        // specify the number of input and output channels that we want to open
        //setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        //shutdownAudio();
		deviceManager.removeAudioCallback(&recorder);
		//deviceManager.removeAudioCallback(&liveAudioScroller);
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
		//g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));

        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
		Rectangle<int> area(getLocalBounds());
		recordingThumbnail.setBounds(area.removeFromTop(80).reduced(8)); //recordingThumbnail not recognized
		recordButton.setBounds(area.removeFromTop(36).removeFromLeft(140).reduced(8));
		explanationLabel.setBounds(area.reduced(8));
    }

	void audioDeviceIOCallback(const float** /*inputChannelData*/, int /*numInputChannels*/,
		float** outputChannelData, int numOutputChannels,
		int numSamples) override
	{
		//so the compiler doesn't cry - grab from the recording button sample code
	}

	void audioDeviceAboutToStart(AudioIODevice* device) override
	{
		//so the compiler doesn't cry - grab from the recording button sample code.
	}

	void audioDeviceStopped() override
	{
		//so the compiler doesn't cry - originally empty - grab from the recording button sample code.
	}
	// Your private member variables go here...
	
private:
    //==============================================================================
	AudioDeviceManager audioDeviceManager;         // [3]
	AudioDeviceSelectorComponent audioSetupComp;   // [4]
  
	//AudioDeviceManager& deviceManager;
	RecordingThumbnail recordingThumbnail;
	AudioRecorder recorder;
	Label explanationLabel;
	TextButton recordButton;

	void startRecording()
	{
		const File file(File::getSpecialLocation(File::userDocumentsDirectory)
			.getNonexistentChildFile("Juce Demo Audio Recording", ".wav"));
		recorder.startRecording(file);

		recordButton.setButtonText("Stop");
		recordingThumbnail.setDisplayFullThumbnail(false);
	}

	void stopRecording()
	{
		recorder.stop();
		recordButton.setButtonText("Record");
		recordingThumbnail.setDisplayFullThumbnail(true);
	}

	void buttonClicked(Button* button) override
	{
		if (button == &recordButton)
		{
			if (recorder.isRecording())
				stopRecording();
			else
				startRecording();
		}
	}


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
