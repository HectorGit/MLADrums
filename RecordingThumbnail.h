#include "../JuceLibraryCode/JuceHeader.h"


class RecordingThumbnail : public Component,
	private ChangeListener
{
public:
	RecordingThumbnail()
		: thumbnailCache(10),
		thumbnail(512, formatManager, thumbnailCache),
		displayFullThumb(false)
	{
		formatManager.registerBasicFormats();
		thumbnail.addChangeListener(this);
	}

	~RecordingThumbnail()
	{
		thumbnail.removeChangeListener(this);
	}

	AudioThumbnail& getAudioThumbnail() { return thumbnail; }

	void setDisplayFullThumbnail(bool displayFull)
	{
		displayFullThumb = displayFull;
		repaint();
	}

	void paint(Graphics& g) override
	{
		g.fillAll(Colours::darkgrey);
		g.setColour(Colours::lightgrey);

		if (thumbnail.getTotalLength() > 0.0)
		{
			const double endTime = displayFullThumb ? thumbnail.getTotalLength()
				: jmax(30.0, thumbnail.getTotalLength());

			Rectangle<int> thumbArea(getLocalBounds());
			thumbnail.drawChannels(g, thumbArea.reduced(2), 0.0, endTime, 1.0f);
		}
		else
		{
			g.setFont(14.0f);
			g.drawFittedText("(No file recorded)", getLocalBounds(), Justification::centred, 2);
		}
	}

private:
	AudioFormatManager formatManager;
	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;
	bool displayFullThumb;

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		if (source == &thumbnail)
			repaint();
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecordingThumbnail)
};