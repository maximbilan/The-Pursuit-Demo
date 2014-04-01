class Transitions2D
{
public:
	enum FADE_TYPE
	{
		FADE_TYPE_IN,
		FADE_TYPE_OUT
	};

private:
	enum DIR
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	static CIwTexture* mStartTexture;
	static CIwTexture* mEndTexture;
	static bool isUsingPrivateTextures;

	static void Slide(DIR d, uint8 transitionSpeed, bool skipFirstAndLastFrame);

public:
	static CIwTexture* CaptureScreen();
	static void CaptureStartScreen();
	static void CaptureEndScreen();
	static void Release();
	static void Fade( uint8 transitionSpeed = 20, bool skipFirstAndLastFrame = false, uint8 fadeType = FADE_TYPE_IN );
	static void Fade(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed = 20, bool skipFirstAndLastFrame = false);
	static void SlideLeft(uint8 transitionSpeed = 30, bool skipFirstAndLastFrame = false);
	static void SlideLeft(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed = 30, bool skipFirstAndLastFrame = false);
	static void SlideRight(uint8 transitionSpeed = 30, bool skipFirstAndLastFrame = false);
	static void SlideRight(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed = 30, bool skipFirstAndLastFrame = false);
	static void SlideUp(uint8 transitionSpeed = 30, bool skipFirstAndLastFrame = false);
	static void SlideUp(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed = 30, bool skipFirstAndLastFrame = false);
	static void SlideDown(uint8 transitionSpeed = 30, bool skipFirstAndLastFrame = false);
	static void SlideDown(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed = 30, bool skipFirstAndLastFrame = false);
};
