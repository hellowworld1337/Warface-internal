#include "mainfunc.hpp"
using WNDPROC = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);
using IDXGISwapChainPresent = HRESULT(WINAPI*)(IDXGISwapChain*, UINT, UINT);
IDXGISwapChainPresent swapChainPresent;
using D3D11ClearRenderTargetView = VOID(WINAPI*)(ID3D11DeviceContext*, ID3D11RenderTargetView*, const FLOAT[4]);
using ResizeBuffers = HRESULT(WINAPI*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
using D3D11DrawIndexed = VOID(WINAPI*)(ID3D11DeviceContext*, UINT, UINT, INT);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef unsigned int EntityId;
typedef const char* cchar;
HWND hwnd;
WNDPROC wndProc;

ID3D11Device* g_device = nullptr;
IDXGISwapChain* g_swapChain = nullptr;
ID3D11DeviceContext* g_deviceContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView = nullptr;


ResizeBuffers resizeBuffers;

int width = 1920, height = 1080;





LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
        return true;

    return ::CallWindowProc(wndProc, handle, message, wParam, lParam);
}

void InitImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_device, g_deviceContext);
}

const char* boness[4] = {"Head", "Neck", "Pelvis"};

HRESULT WINAPI PresentHook(IDXGISwapChain* swapChain, UINT interval, UINT flags)
{
    if (!globals::g_initialized)
    {
        if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_device)))
        {
            g_device->GetImmediateContext(&g_deviceContext);
            DXGI_SWAP_CHAIN_DESC sd;
            swapChain->GetDesc(&sd);
            hwnd = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            g_device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();
            wndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
            InitImGui();
            globals::g_initialized = true;
        }
        return swapChain->Present(interval, flags);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (GetAsyncKeyState(VK_INSERT) & 1)
        globals::open = !globals::open;

    if (globals::open) {
        ImGui::Begin("Ezz cheat -->");

        ImGui::Checkbox("-> ESP", &globals::esp);
        ImGui::Checkbox("-> AutoShot", &globals::autoShot);
        ImGui::Checkbox("-> Silent", &globals::silent);
        if (globals::silent) {
            ImGui::SliderFloat("Fov", &globals::silentFOV, 20, 300);
        }
        ImGui::Text("Bone");
        ImGui::ListBox("##Bones", &globals::bone, boness, 3);

        ImGui::End();
    }
    

    if(globals::esp)
        entryPoint();

    ImGui::Render();

    g_deviceContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return swapChain->Present(interval, flags);
}

class CDXGISwapChainProxy : public IDXGISwapChain
{
public:
    inline CDXGISwapChainProxy(IDXGISwapChain* pSwapChain) : m_pSwapChain(pSwapChain) {}
    inline ~CDXGISwapChainProxy() {}

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) { return m_pSwapChain->QueryInterface(riid, ppvObject); }
    virtual ULONG STDMETHODCALLTYPE AddRef(void) { return m_pSwapChain->AddRef(); }
    virtual ULONG STDMETHODCALLTYPE Release(void) { return m_pSwapChain->Release(); }

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID Name, UINT DataSize, const void* pData) { return m_pSwapChain->SetPrivateData(Name, DataSize, pData); }
    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID Name, const IUnknown* pUnknown) { return m_pSwapChain->SetPrivateDataInterface(Name, pUnknown); }
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID Name, UINT* pDataSize, void* pData) { return m_pSwapChain->GetPrivateData(Name, pDataSize, pData); }
    virtual HRESULT STDMETHODCALLTYPE GetParent(REFIID riid, void** ppParent) { return m_pSwapChain->GetParent(riid, ppParent); }

    virtual HRESULT STDMETHODCALLTYPE GetDevice(REFIID riid, void** ppDevice) { return m_pSwapChain->GetDevice(riid, ppDevice); }

    virtual HRESULT STDMETHODCALLTYPE Present(UINT SyncInterval, UINT Flags) { return m_pSwapChain->Present(SyncInterval, Flags); }
    virtual HRESULT STDMETHODCALLTYPE GetBuffer(UINT Buffer, REFIID riid, void** ppSurface) { return m_pSwapChain->GetBuffer(Buffer, riid, ppSurface); }
    virtual HRESULT STDMETHODCALLTYPE SetFullscreenState(BOOL Fullscreen, IDXGIOutput* pTarget) { return m_pSwapChain->SetFullscreenState(Fullscreen, pTarget); }
    virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(BOOL* pFullscreen, IDXGIOutput** ppTarget) { return m_pSwapChain->GetFullscreenState(pFullscreen, ppTarget); }
    virtual HRESULT STDMETHODCALLTYPE GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc) { return m_pSwapChain->GetDesc(pDesc); }
    virtual HRESULT STDMETHODCALLTYPE ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) { return m_pSwapChain->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags); }
    virtual HRESULT STDMETHODCALLTYPE ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters) { return m_pSwapChain->ResizeTarget(pNewTargetParameters); }
    virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(IDXGIOutput** ppOutput) { return m_pSwapChain->GetContainingOutput(ppOutput); }
    virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(DXGI_FRAME_STATISTICS* pStats) { return m_pSwapChain->GetFrameStatistics(pStats); }
    virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(UINT* pLastPresentCount) { return m_pSwapChain->GetLastPresentCount(pLastPresentCount); }

    inline IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }
private:
    IDXGISwapChain* m_pSwapChain;
};

class CGameFunction : public IGameFrameworkListener, public CDXGISwapChainProxy
{
public:
    CGameFunction() : CDXGISwapChainProxy(gEnv->pRenderer->GetSwapChain())
    {
        gEnv->pRenderer->SetSwapChain(this);
    }
    ~CGameFunction()
    {
        gEnv->pRenderer->SetSwapChain(GetSwapChain());
    }

    virtual HRESULT __stdcall Present(UINT SyncInterval, UINT Flags)
    {
        return PresentHook(GetSwapChain(), SyncInterval, Flags);
        //return m_pSwapChain->Present(SyncInterval, Flags); 
    }
    virtual HRESULT __stdcall ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
    {
        printf("hookResizeBuffers is called\n");
        return CDXGISwapChainProxy::ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
    }


private:


};