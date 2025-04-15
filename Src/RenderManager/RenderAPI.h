#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <string>
#include <wrl/client.h>


typedef struct RENDER_API_INIT_DESC
{
	int Width;
	int Height;
	HWND hWnd;
}RENDER_API_INIT_DESC;

class RenderAPI
{
public:
	RenderAPI() = default;
	~RenderAPI() = default;

	RenderAPI(RenderAPI const&) = default;
	RenderAPI(RenderAPI&&) = default;

	RenderAPI& operator=(RenderAPI&&) = default;
	RenderAPI& operator=(RenderAPI const&) = default;

	bool Init(const RENDER_API_INIT_DESC* desc);

	bool PreRender();
	bool Render(float deltaTime);
	bool PostRender();

private:
	bool InitDeviceAndContext(const RENDER_API_INIT_DESC* desc);
	bool InitSwapChain(const RENDER_API_INIT_DESC* desc);
	bool InitRenderTargetView(const RENDER_API_INIT_DESC* desc);
	bool InitDepthStencilView(const RENDER_API_INIT_DESC* desc);
	bool InitRasterizationState(const RENDER_API_INIT_DESC* desc);
	bool InitViewPort();

	//~ Helper Functions
	void SetOMOutputMerger();
	void UpdateMultiSampling();
	void SetDeviceDescription(IDXGIAdapter* adapter);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>			mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>			mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mRenderTargetBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	mRasterizerState;

	// Multi Sampling
	UINT m_maxSampleCount{ 1 };
	UINT m_maxSampleQuality{ 0 };
	SIZE_T m_videoMemorySize{ 0 };
	std::wstring m_videoCardDescription{};
};
