#include "RenderAPI.h"

#include "Utils/Logger.h"
#include "Utils/Helper.h"

bool RenderAPI::Init(const RENDER_API_INIT_DESC* desc)
{
	INIT_AND_LOG(InitDeviceAndContext, desc)
	INIT_AND_LOG(InitSwapChain, desc)
	INIT_AND_LOG(InitRenderTargetView, desc)
	INIT_AND_LOG(InitDepthStencilView, desc)
	INIT_AND_LOG(InitRasterizationState, desc)
	INIT_AND_LOG(InitViewPort)
	return true;
}

bool RenderAPI::PreRender()
{
    return true;
}

bool RenderAPI::Render(float deltaTime)
{
    return true;
}

bool RenderAPI::PostRender()
{
    return true;
}

bool RenderAPI::InitDeviceAndContext(const RENDER_API_INIT_DESC* desc)
{
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf())))
    {
        Logger::Error("Failed to create DXGIFactory.");
        return false;
    }

	Microsoft::WRL::ComPtr<IDXGIAdapter> bestAdapter = nullptr;
    SIZE_T maxDedicatedMemory = 0;

    for (UINT adapterIndex = 0;; ++adapterIndex)
    {
	    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
        if (factory->EnumAdapters(adapterIndex, adapter.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)
            break;

        DXGI_ADAPTER_DESC adapterDesc;
        adapter->GetDesc(&adapterDesc);

        // Skip software adapters
        if (adapterDesc.VendorId == 0x1414 && adapterDesc.DeviceId == 0x8c)
            continue;

        if (adapterDesc.DedicatedVideoMemory > maxDedicatedMemory)
        {
            bestAdapter = adapter;
            maxDedicatedMemory = adapterDesc.DedicatedVideoMemory;
        }
    }

    if (!bestAdapter)
    {
        Logger::Error("No suitable adapter found.");
        return false;
    }

    SetDeviceDescription(bestAdapter.Get());

    UINT deviceFlags = 0u;
#if defined(_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        bestAdapter.Get(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        deviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &mDevice,
        &featureLevel,
        &mContext
    );

    if (FAILED(hr))
    {
        Logger::Error("D3D11CreateDevice failed.");
        return false;
    }

    Logger::Info("Device and context created successfully.");

    UpdateMultiSampling();

    return true;
}

bool RenderAPI::InitSwapChain(const RENDER_API_INIT_DESC* desc)
{
    // Set up the swap chain description
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferCount = 2;
    scDesc.BufferDesc.Width = desc->Width;
    scDesc.BufferDesc.Height = desc->Height;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.SampleDesc.Count = m_maxSampleCount;
    scDesc.SampleDesc.Quality = m_maxSampleQuality;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = desc->hWnd;
    scDesc.Windowed = TRUE;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = 0;

    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    HRESULT hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf()));
    if (FAILED(hr)) {
        Logger::Error("Failed to get IDXGIDevice from D3D11Device.");
        return false;
    }

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    hr = dxgiDevice->GetAdapter(&adapter);
    if (FAILED(hr)) {
        Logger::Error("Failed to get adapter from DXGIDevice.");
        return false;
    }

    Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
    hr = adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));
    if (FAILED(hr)) {
        Logger::Error("Failed to get DXGIFactory from adapter.");
        return false;
    }

	hr = dxgiFactory->CreateSwapChain(
	    mDevice.Get(),
	    &scDesc,
	    &mSwapChain
	);

    if (FAILED(hr))
    {
        Logger::Error("Failed to create swap chain!");
        return false;
    }

    Logger::Info("Swap chain initialized successfully.");
    return true;
}

bool RenderAPI::InitRenderTargetView(const RENDER_API_INIT_DESC* desc)
{
    return true;
}

bool RenderAPI::InitDepthStencilView(const RENDER_API_INIT_DESC* desc)
{
    return true;
}

bool RenderAPI::InitRasterizationState(const RENDER_API_INIT_DESC* desc)
{
    return true;
}

bool RenderAPI::InitViewPort()
{
    return true;
}

void RenderAPI::SetOMOutputMerger()
{
}

void RenderAPI::UpdateMultiSampling()
{
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

    m_maxSampleCount = 1;
    m_maxSampleQuality = 0;

    for (UINT sampleCount = 1; sampleCount <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; ++sampleCount)
    {
        UINT qualityLevels = 0;
        if (SUCCEEDED(mDevice->CheckMultisampleQualityLevels(format, sampleCount, &qualityLevels)) &&
            qualityLevels > 0)
        {
            m_maxSampleCount = sampleCount;
            m_maxSampleQuality = qualityLevels - 1;
        }
    }

    Logger::Info("Max MSAA sample count: " + std::to_string(m_maxSampleCount));
    Logger::Info("Max MSAA quality level: " + std::to_string(m_maxSampleQuality));
}

void RenderAPI::SetDeviceDescription(IDXGIAdapter* adapter)
{
    DXGI_ADAPTER_DESC adapterDesc;
    adapter->GetDesc(&adapterDesc);

    m_videoCardDescription = adapterDesc.Description;
    m_videoMemorySize = adapterDesc.DedicatedVideoMemory / (1024ull * 1024ull);

    std::string info =
        "Selected Adapter:\n"
        "Name: " + wideToString(m_videoCardDescription) + "\n" +
        "VRam Size: " + std::to_string(m_videoMemorySize) + " MB.";

    Logger::Info(info);

}
