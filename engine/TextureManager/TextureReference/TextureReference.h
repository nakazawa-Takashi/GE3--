#pragma once

#include <d3d12.h>
#include <wrl.h>
class TextureReference
{
public:
    TextureReference(Microsoft::WRL::ComPtr<ID3D12Resource> resource);
    ~TextureReference();

    void AddRef();
    void Release();
    Microsoft::WRL::ComPtr<ID3D12Resource> GetResource() const;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> textureResource_;
    unsigned int refCount_;
};


