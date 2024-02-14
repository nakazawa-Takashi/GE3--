#include "TextureReference.h"

TextureReference::TextureReference(Microsoft::WRL::ComPtr<ID3D12Resource> resource) : textureResource_(resource), refCount_(1) {}

TextureReference::~TextureReference(){}

void TextureReference::AddRef() {
    refCount_++;
}

void TextureReference::Release() {

    refCount_--;
    if (refCount_ == 0) {
        // 参照がなくなったらリソースを解放
        delete this;
    }
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureReference::GetResource() const{
    return textureResource_;
}