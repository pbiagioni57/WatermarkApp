#pragma once
#include "DirectXSample.h"
#include "pch.h"
#include <atlbase.h>
using namespace std;
using namespace Microsoft::WRL;
//using namespace Windows::UI;
using namespace Windows::Foundation;
//using namespace Windows::Storage::Streams;
namespace WatermarkComponent
{
	class D2DWrapper
	{
	public:
		D2DWrapper();
		~D2DWrapper();
		void Initialize();
//		void SetBitmapRenderTarget(const wchar_t* pszBackgroundImageFile);
		void SetBitmapRenderTarget(const string& pszBackgroundImageFile);
		void BeginDraw();
		void EndDraw();
		//void DrawText(Platform::String^ text, Point startPoint, Platform::String^ fontFamilyName, Color foregroundColor,
		//	float fontSize, FONT_STYLE_ENUM fontStyle, FONT_WEIGHT_ENUM fontWeight, Platform::String^ localeName);
//		void DrawImage(const wchar_t* pszWatermarkImageFile, /*Point startPoint,*/ float opacity);
		void DrawImage(const string& pszWatermarkImageFile, /*Point startPoint,*/ float opacity);
		void SaveBitmapToFile(const wchar_t* pszOutputImageFile);

	protected:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();

	private:
		ComPtr<ID2D1Factory1>					 m_d2dFactory;
		ComPtr<ID2D1Device>						 m_d2dDevice;
		ComPtr<ID2D1DeviceContext>				 m_d2dContext;
		D3D_FEATURE_LEVEL						 m_featureLevel;
		ComPtr<IWICImagingFactory2>				 m_wicFactory;
		ComPtr<IDWriteFactory1>					 m_dwriteFactory;

		ComPtr<IWICFormatConverter>				 m_backgroundBitmapConverter;
		ComPtr<IWICFormatConverter>				 m_watermarkImageConverter;

		ComPtr<ID2D1SolidColorBrush>			 m_textBrush;
		ComPtr<IDWriteTextFormat>				 m_textFormat;

		ComPtr<ID2D1Bitmap1>					 m_targetBitmap;
		ComPtr<ID2D1Bitmap1>					 m_watermarkBitmap;

		D2D1_SIZE_F								 m_renderTargetSize;

	};
}