#include "stdafx.h"
#include "pch.h"
#include "D2DWrapper.h"
#include "CustomError.h"

using namespace D2D1;
using namespace std;
using namespace WatermarkComponent;

// Constructor.
D2DWrapper::D2DWrapper()
{
	// Initialize COM
	CoInitialize(nullptr);
}

// Destructor.
D2DWrapper::~D2DWrapper()
{
}

// Initialize the DirectX resources.
void D2DWrapper::Initialize()
{
	CreateDeviceIndependentResources();
	CreateDeviceResources();
}


// Resources required independent of the device.
void D2DWrapper::CreateDeviceIndependentResources()
{
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));


	DX::ThrowIfFailed(
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			&options,
			&m_d2dFactory)
	);

	DX::ThrowIfFailed(
		CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_wicFactory))
	);

	DX::ThrowIfFailed(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			&m_dwriteFactory)
	);
}

// Create Device dependent resources.
void D2DWrapper::CreateDeviceResources()
{

	// This flag adds support for surfaces with a different color channel ordering than the API default.
	// For compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;


	// DirectX hardware feature levels this app  supports.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};


	// Create the DX11 API device object, and get a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	DX::ThrowIfFailed(
		D3D11CreateDevice(
			nullptr,                    // specify null to use the default adapter
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags,              // optionally set debug and Direct2D compatibility flags
			featureLevels,              // list of feature levels this app can support
			ARRAYSIZE(featureLevels),   // number of possible feature levels
			D3D11_SDK_VERSION,
			&device,                    // returns the Direct3D device created
			&m_featureLevel,            // returns feature level of device created
			&context                    // returns the device immediate context
		)
	);

	ComPtr<IDXGIDevice> dxgiDevice;

	// Obtain the underlying DXGI device of the Direct3D11 device.
	DX::ThrowIfFailed(
		device.As(&dxgiDevice)
	);

	// Obtain the Direct2D device for 2-D rendering.
	DX::ThrowIfFailed(
		m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)
	);

	// Get Direct2D device's corresponding device context object.
	DX::ThrowIfFailed(
		m_d2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_d2dContext)
	);
}

// Set bitmap render target. 
void D2DWrapper::SetBitmapRenderTarget(const string& BackgroundImageFile)
{
	// Clear the render target.
	m_d2dContext->SetTarget(nullptr);

	wstring wBackgroundImageFile = wstring(BackgroundImageFile.begin(), BackgroundImageFile.end());
	const wchar_t* pszBackgroundImageFile = wBackgroundImageFile.c_str();

	// Create the bitmap decoder.
	ComPtr<IWICBitmapDecoder> decoder;

	DX::ThrowIfFailed(
		m_wicFactory->CreateDecoderFromFilename(
			pszBackgroundImageFile,
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&decoder
		), "SetBitmapRenderTarget(): Load background image -"
	);

	// Get the first frame. 
	ComPtr<IWICBitmapFrameDecode> frame;
	DX::ThrowIfFailed(
		decoder->GetFrame(0, &frame)
	);

	DX::ThrowIfFailed(
		m_wicFactory->CreateFormatConverter(&m_backgroundBitmapConverter)
	);

	DX::ThrowIfFailed(
		m_backgroundBitmapConverter->Initialize(
			frame.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.0f,
			WICBitmapPaletteTypeCustom  // Premultiplied BGRA has no paletting, so this is ignored.
		)
	);

	double dpiX = 96.0f;
	double dpiY = 96.0f;
	DX::ThrowIfFailed(
		m_backgroundBitmapConverter->GetResolution(&dpiX, &dpiY)
	);

	DX::ThrowIfFailed(
		m_d2dContext->CreateBitmapFromWicBitmap(
			m_backgroundBitmapConverter.Get(),
			D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				static_cast<float>(dpiX),
				static_cast<float>(dpiY), 0
			),
			&m_targetBitmap
		)
	);

	m_d2dContext->SetTarget(m_targetBitmap.Get());

	// Retrieve the renderTargetSize in DIPs.
	m_renderTargetSize = m_d2dContext->GetSize();
}

// Call this function before drawing.
void D2DWrapper::BeginDraw()
{
	m_d2dContext->BeginDraw();
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

// Call this function after drawing.
void D2DWrapper::EndDraw()
{
	DX::ThrowIfFailed(
		m_d2dContext->EndDraw()
	);
}

// Render the watermark image
void D2DWrapper::DrawImage(const string& WatermarkImageFile, /*Point startPoint,*/ float opacity)
{
	// todo: implement and validate starting point 
	// In case user input the invalid startPoint, we do a simple Validation.
	//if (startPoint.X < 0.0f || startPoint.X > 1.0f || startPoint.Y < 0.0f || startPoint.Y > 1.0f)
	//{
		//throw exception
	//}
	wstring wWatermarkImageFile = wstring(WatermarkImageFile.begin(), WatermarkImageFile.end());
	const wchar_t* pszWatermarkImageFile = wWatermarkImageFile.c_str();

	// Create the bitmap decoder.
	ComPtr<IWICBitmapDecoder> decoder;
	DX::ThrowIfFailed(
		m_wicFactory->CreateDecoderFromFilename(
			pszWatermarkImageFile,
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&decoder
		),"DrawImage(): Load watermark image -"
	);

	ComPtr<IWICBitmapFrameDecode> frame;
	DX::ThrowIfFailed(
		decoder->GetFrame(0, &frame)
	);

	DX::ThrowIfFailed(
		m_wicFactory->CreateFormatConverter(&m_watermarkImageConverter)
	);

	DX::ThrowIfFailed(
		m_watermarkImageConverter->Initialize(
			frame.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.0f,
			WICBitmapPaletteTypeCustom  // Premultiplied BGRA has no paletting, so this is ignored.
		)
	);

	double dpiX = 96.0f;
	double dpiY = 96.0f;

	m_watermarkImageConverter->GetResolution(&dpiX, &dpiY);
	DX::ThrowIfFailed(
		m_d2dContext->CreateBitmapFromWicBitmap(
			m_watermarkImageConverter.Get(),
			D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				static_cast<float>(dpiX),
				static_cast<float>(dpiY), 0
			),
			&m_watermarkBitmap
		)
	);

	// todo: allow user to mask out watermark color
	//
	//ComPtr<ID2D1Effect> colorMatrixEffect;
	//m_d2dContext->CreateEffect(CLSID_D2D1ColorMatrix, &colorMatrixEffect);

	//colorMatrixEffect->SetInput(0, ID2D1Image);
	//D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F(0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);
	//colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);

	//	scale to size of background image
	D2D1_SIZE_F watermarkImageSize = m_renderTargetSize;

	// ** temporary structure ***
	struct sPoint
	{
		float X;
		float Y;
	};
	sPoint startPoint;
	startPoint.X = 0.0;
	startPoint.Y = 0.0;
	// ** temporary structure ***

	float startPointOffsetX = startPoint.X * m_renderTargetSize.width;
	float startPointOffsetY = startPoint.Y * m_renderTargetSize.height;

	D2D1_RECT_F destinationRect = { startPointOffsetX, startPointOffsetY, startPointOffsetX + watermarkImageSize.width,
		startPointOffsetY + watermarkImageSize.height };

	m_d2dContext->DrawBitmap(m_watermarkBitmap.Get(), &destinationRect, opacity);
}

// Save the bitmap target to file.
void D2DWrapper::SaveBitmapToFile(const wchar_t* pszOutputImageFile)
{
	// Create and initialize WIC Bitmap Encoder.
	ComPtr<IWICBitmapEncoder> wicBitmapEncoder;
	GUID wicFormat = GUID_ContainerFormatPng;
	DX::ThrowIfFailed(
		m_wicFactory->CreateEncoder(
			wicFormat,
			nullptr,    // No preferred codec vendor.
			&wicBitmapEncoder
		)
	);
	// **** changed to ATL pointer *****
	CComPtr<IWICStream> stream;
	// Create a stream for the encoder
	m_wicFactory->CreateStream(&stream);
	// Initialize the stream using the output file path
	stream->InitializeFromFilename(pszOutputImageFile, GENERIC_WRITE);

	DX::ThrowIfFailed(
		wicBitmapEncoder->Initialize(
			stream, /* parameter is IStream when using Microsoft::WRL::ComPtr */
			WICBitmapEncoderNoCache
		)
	);

	// Create and initialize WIC Frame Encoder.
	ComPtr<IWICBitmapFrameEncode> wicFrameEncode;
	DX::ThrowIfFailed(
		wicBitmapEncoder->CreateNewFrame(
			&wicFrameEncode,
			nullptr     // No encoder options.
		)
	);

	DX::ThrowIfFailed(
		wicFrameEncode->Initialize(nullptr)
	);

	// Retrieve D2D Device.
	ComPtr<ID2D1Device> d2dDevice;
	m_d2dContext->GetDevice(&d2dDevice);

	// Create IWICImageEncoder.
	ComPtr<IWICImageEncoder> imageEncoder;
	DX::ThrowIfFailed(
		m_wicFactory->CreateImageEncoder(
			d2dDevice.Get(),
			&imageEncoder
		)
	);

	DX::ThrowIfFailed(
		imageEncoder->WriteFrame(
			m_targetBitmap.Get(),
			wicFrameEncode.Get(),
			nullptr     // Use default WICImageParameter options.
		)
	);

	DX::ThrowIfFailed(
		wicFrameEncode->Commit()
	);

	DX::ThrowIfFailed(
		wicBitmapEncoder->Commit()
	);

}