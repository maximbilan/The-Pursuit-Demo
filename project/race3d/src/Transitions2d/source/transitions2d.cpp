#include "IwGx.h"
#include "transitions2d.h"

CIwTexture* Transitions2D::mStartTexture = NULL;
CIwTexture* Transitions2D::mEndTexture = NULL;
bool Transitions2D::isUsingPrivateTextures = true;

CIwTexture* Transitions2D::CaptureScreen()
{
	int w = IwGxGetDeviceWidth();
	int h = IwGxGetDeviceHeight();
	int length = w * h * 4;

	uint8* buffer = new uint8[length];

	glReadPixels(0, 0, w, h, 0x1908, 0x1401, buffer);

	uint8* buffer2 = new uint8[length];

	int lineSize = w * 4;
	uint8* b1 = buffer;
	uint8* b2 = buffer2 + h * lineSize;;
	for(int y = h; y > 0; y--)
	{
		b2 -= lineSize;
		for(int x = w; x > 0; x--)
		{
			*b2++ = *b1++;
			*b2++ = *b1++;
			*b2++ = *b1++;
			*b2++ = *b1++;
		}
		b2 -= lineSize;
	}

	CIwTexture* texture = new CIwTexture;
	CIwImage& img = texture->GetImage();

	img.SetFormat(CIwImage::ABGR_8888);
	img.SetWidth(w);
	img.SetHeight(h);
	img.SetBuffers(buffer2, length);
	//img.SaveBmp("screenshot.bmp");

	texture->SetMipMapping(false);
	texture->Upload();

	delete buffer;
	delete buffer2;

	return texture;
}

void Transitions2D::CaptureStartScreen()
{
	IwGxFlush();
	mStartTexture = CaptureScreen();
}
void Transitions2D::CaptureEndScreen()
{
	IwGxFlush();
	mEndTexture = CaptureScreen();
}

void Transitions2D::Release()
{
	delete mStartTexture;
	delete mEndTexture;
	mStartTexture = NULL;
	mEndTexture = NULL;
}

void Transitions2D::Fade( uint8 transitionSpeed, bool skipFirstAndLastFrame, uint8 fadeType )
{
	IwGxSetColClear(0, 0, 0, 0);

	if (mStartTexture == NULL || mEndTexture == NULL)
		return;

	if (transitionSpeed == 0)
		transitionSpeed = 1;

	IwGxScreenOrient orient = IwGxGetScreenOrient();
	if (isUsingPrivateTextures) IwGxSetScreenOrient(IW_GX_ORIENT_NONE);

	int alpha = 0;

	if (skipFirstAndLastFrame)
		alpha += transitionSpeed;

	CIwMaterial* startMat;
	CIwMaterial* endMat;
	while (alpha <= 255)
	{
		IwGxClear();

		startMat = IW_GX_ALLOC_MATERIAL();
		startMat->SetTexture(mStartTexture);
		IwGxSetMaterial(startMat);

		CIwSVec2 xy( 0, 0 );
		CIwSVec2 dxy( mStartTexture->GetWidth(), mStartTexture->GetHeight() );
		IwGxDrawRectScreenSpace( &xy, &dxy );

		uint8 colAmbientAlpha = 0;
		switch( fadeType )
		{
			case FADE_TYPE_IN:
				colAmbientAlpha = alpha;
				break;

			case FADE_TYPE_OUT:
				colAmbientAlpha = 255 - alpha;
				break;
		}

		endMat = IW_GX_ALLOC_MATERIAL();
		endMat->SetTexture(mEndTexture);
		endMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		endMat->SetColAmbient( 255, 255, 255, colAmbientAlpha );

		IwGxSetMaterial(endMat);
		
		xy.x = 0;
		xy.y = 0;
		dxy.x = mEndTexture->GetWidth();
		dxy.y = mEndTexture->GetHeight();
		IwGxDrawRectScreenSpace( &xy, &dxy );

		IwGxFlush();
		IwGxSwapBuffers();

		s3eDeviceYield(40);
		alpha += transitionSpeed;
	}

	if (!skipFirstAndLastFrame)
	{
		IwGxClear();
		endMat = IW_GX_ALLOC_MATERIAL();
		endMat->SetTexture(mEndTexture);
		IwGxSetMaterial(endMat);

		CIwSVec2 xy( 0, 0 );
		CIwSVec2 dxy( mEndTexture->GetWidth(), mEndTexture->GetHeight() );
		IwGxDrawRectScreenSpace( &xy, &dxy );

		IwGxFlush();
		IwGxSwapBuffers();
	}

	if (isUsingPrivateTextures)
	{
		delete mStartTexture;
		delete mEndTexture;
		mStartTexture = NULL;
		mEndTexture = NULL;
	}

	IwGxSetScreenOrient(orient);
}

void Transitions2D::Fade(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	CIwTexture* tempStart = mStartTexture;
	CIwTexture* tempEnd = mEndTexture;
	mStartTexture = start;
	mEndTexture = end;
	isUsingPrivateTextures = false;
	Fade(transitionSpeed, skipFirstAndLastFrame);
	isUsingPrivateTextures = true;
	mStartTexture = tempStart;
	mEndTexture = tempEnd;
}

void Transitions2D::Slide(DIR d, uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	IwGxSetColClear(0, 0, 0, 0);

	if (mStartTexture == NULL || mEndTexture == NULL)
		return;

	if (transitionSpeed == 0)
		transitionSpeed = 1;

	IwGxScreenOrient orient = IwGxGetScreenOrient();
	if (isUsingPrivateTextures) IwGxSetScreenOrient(IW_GX_ORIENT_NONE);

	CIwMaterial* startMat;
	CIwMaterial* endMat;

	int w = mStartTexture->GetWidth();
	int h = mStartTexture->GetHeight();
	int pos = 0;
	int speed, size;

	if (d == LEFT || d == RIGHT)
	{
		speed = (int)((transitionSpeed * w) / 255.0);
		size = w;
	}
	else
	{
		speed = (int)((transitionSpeed * h) / 255.0);
		size = h;
	}

	if (skipFirstAndLastFrame)
		pos += speed;

	while (pos < size)
	{
		IwGxClear();
		if (d == LEFT)
		{
			startMat = IW_GX_ALLOC_MATERIAL();
			startMat->SetTexture(mStartTexture);
			IwGxSetMaterial(startMat);

			CIwSVec2 xy( -pos, 0 );
			CIwSVec2 dxy( mStartTexture->GetWidth(), mStartTexture->GetHeight() );
			IwGxDrawRectScreenSpace( &xy, &dxy );

			endMat = IW_GX_ALLOC_MATERIAL();
			endMat->SetTexture(mEndTexture);
			IwGxSetMaterial(endMat);

			xy.x = -pos + size;
			xy.y = 0;
			dxy.x = mEndTexture->GetWidth();
			dxy.y = mEndTexture->GetHeight();
			IwGxDrawRectScreenSpace( &xy, &dxy );
		}
		else if (d == RIGHT)
		{
			endMat = IW_GX_ALLOC_MATERIAL();
			endMat->SetTexture(mEndTexture);
			IwGxSetMaterial(endMat);
			CIwSVec2 xy( pos - size, 0 );
			CIwSVec2 dxy( mEndTexture->GetWidth(), mEndTexture->GetHeight() );
			IwGxDrawRectScreenSpace( &xy, &dxy );

			startMat = IW_GX_ALLOC_MATERIAL();
			startMat->SetTexture(mStartTexture);
			IwGxSetMaterial(startMat);
			xy.x = pos;
			xy.y = 0;
			dxy.x = mStartTexture->GetWidth();
			dxy.y = mStartTexture->GetHeight();
			IwGxDrawRectScreenSpace( &xy, &dxy );
		}
		else if (d == UP)
		{
			startMat = IW_GX_ALLOC_MATERIAL();
			startMat->SetTexture(mStartTexture);
			IwGxSetMaterial(startMat);
			CIwSVec2 xy( 0, -pos );
			CIwSVec2 dxy( mStartTexture->GetWidth(), mStartTexture->GetHeight() );
			IwGxDrawRectScreenSpace( &xy, &dxy );

			endMat = IW_GX_ALLOC_MATERIAL();
			endMat->SetTexture(mEndTexture);
			IwGxSetMaterial(endMat);
			xy.x = 0;
			xy.y = -pos + size;
			dxy.x = mEndTexture->GetWidth();
			dxy.y = mEndTexture->GetHeight();
			IwGxDrawRectScreenSpace( &xy, &dxy );
		}
		else if (d == DOWN)
		{
			endMat = IW_GX_ALLOC_MATERIAL();
			endMat->SetTexture(mEndTexture);
			IwGxSetMaterial(endMat);
			CIwSVec2 xy( 0, pos - size );
			CIwSVec2 dxy( mEndTexture->GetWidth(), mEndTexture->GetHeight() );
			IwGxDrawRectScreenSpace( &xy, &dxy );

			startMat = IW_GX_ALLOC_MATERIAL();
			startMat->SetTexture(mStartTexture);
			IwGxSetMaterial(startMat);
			xy.x = 0;
			xy.y = pos;
			dxy.x = mStartTexture->GetWidth();
			dxy.y = mStartTexture->GetHeight();
			IwGxDrawRectScreenSpace( &xy, &dxy );
		}

		IwGxFlush();
		IwGxSwapBuffers();

		s3eDeviceYield(40);
		pos += speed;
	}

	if (!skipFirstAndLastFrame)
	{
		IwGxClear();
		endMat = IW_GX_ALLOC_MATERIAL();
		endMat->SetTexture(mEndTexture);
		IwGxSetMaterial(endMat);
		CIwSVec2 xy( 0, 0 );
		CIwSVec2 dxy( mEndTexture->GetWidth(), mEndTexture->GetHeight() );
		IwGxDrawRectScreenSpace( &xy, &dxy );

		IwGxFlush();
		IwGxSwapBuffers();
	}

	if (isUsingPrivateTextures)
	{
		delete mStartTexture;
		delete mEndTexture;
		mStartTexture = NULL;
		mEndTexture = NULL;
	}

	IwGxSetScreenOrient(orient);
}

void Transitions2D::SlideLeft(uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	Slide(LEFT, transitionSpeed, skipFirstAndLastFrame);
}

void Transitions2D::SlideRight(uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	Slide(RIGHT, transitionSpeed, skipFirstAndLastFrame);
}

void Transitions2D::SlideUp(uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	Slide(UP, transitionSpeed, skipFirstAndLastFrame);
}

void Transitions2D::SlideDown(uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	Slide(DOWN, transitionSpeed, skipFirstAndLastFrame);
}

void Transitions2D::SlideLeft(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	CIwTexture* tempStart = mStartTexture;
	CIwTexture* tempEnd = mEndTexture;
	mStartTexture = start;
	mEndTexture = end;
	isUsingPrivateTextures = false;
	Slide(LEFT, transitionSpeed, skipFirstAndLastFrame);
	isUsingPrivateTextures = true;
	mStartTexture = tempStart;
	mEndTexture = tempEnd;
}

void Transitions2D::SlideRight(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	CIwTexture* tempStart = mStartTexture;
	CIwTexture* tempEnd = mEndTexture;
	mStartTexture = start;
	mEndTexture = end;
	isUsingPrivateTextures = false;
	Slide(RIGHT, transitionSpeed, skipFirstAndLastFrame);
	isUsingPrivateTextures = true;
	mStartTexture = tempStart;
	mEndTexture = tempEnd;
}

void Transitions2D::SlideUp(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	CIwTexture* tempStart = mStartTexture;
	CIwTexture* tempEnd = mEndTexture;
	mStartTexture = start;
	mEndTexture = end;
	isUsingPrivateTextures = false;
	Slide(UP, transitionSpeed, skipFirstAndLastFrame);
	isUsingPrivateTextures = true;
	mStartTexture = tempStart;
	mEndTexture = tempEnd;
}

void Transitions2D::SlideDown(CIwTexture* start, CIwTexture* end, uint8 transitionSpeed, bool skipFirstAndLastFrame)
{
	CIwTexture* tempStart = mStartTexture;
	CIwTexture* tempEnd = mEndTexture;
	mStartTexture = start;
	mEndTexture = end;
	isUsingPrivateTextures = false;
	Slide(DOWN, transitionSpeed, skipFirstAndLastFrame);
	isUsingPrivateTextures = true;
	mStartTexture = tempStart;
	mEndTexture = tempEnd;
}
