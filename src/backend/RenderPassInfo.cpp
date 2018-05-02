// Copyright 2017 The NXT Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "backend/RenderPassInfo.h"

#include "backend/Device.h"
#include "backend/Texture.h"
#include "common/Assert.h"
#include "common/BitSetIterator.h"

namespace backend {

    // RenderPassInfo

    RenderPassInfoBase::RenderPassInfoBase(RenderPassInfoBuilder* builder)
        : mColorAttachmentsSet(builder->mColorAttachmentsSet),
          mColorAttachments(builder->mColorAttachments),
          mDepthStencilAttachmentSet(builder->mDepthStencilAttachmentSet),
          mDepthStencilAttachment(builder->mDepthStencilAttachment),
          mWidth(builder->mWidth),
          mHeight(builder->mHeight) {
    }

    std::bitset<kMaxColorAttachments> RenderPassInfoBase::GetColorAttachmentMask() const {
        return mColorAttachmentsSet;
    }

    bool RenderPassInfoBase::HasDepthStencilAttachment() const {
        return mDepthStencilAttachmentSet;
    }

    const RenderPassColorAttachmentInfo& RenderPassInfoBase::GetColorAttachment(
        uint32_t attachment) const {
        ASSERT(attachment < kMaxColorAttachments);
        ASSERT(mColorAttachmentsSet[attachment]);

        return mColorAttachments[attachment];
    }

    RenderPassColorAttachmentInfo& RenderPassInfoBase::GetColorAttachment(uint32_t attachment) {
        ASSERT(attachment < kMaxColorAttachments);
        ASSERT(mColorAttachmentsSet[attachment]);

        return mColorAttachments[attachment];
    }

    const RenderPassDepthStencilAttachmentInfo& RenderPassInfoBase::GetDepthStencilAttachment()
        const {
        ASSERT(mDepthStencilAttachmentSet);

        return mDepthStencilAttachment;
    }

    RenderPassDepthStencilAttachmentInfo& RenderPassInfoBase::GetDepthStencilAttachment() {
        ASSERT(mDepthStencilAttachmentSet);

        return mDepthStencilAttachment;
    }

    uint32_t RenderPassInfoBase::GetWidth() const {
        return mWidth;
    }

    uint32_t RenderPassInfoBase::GetHeight() const {
        return mHeight;
    }

    // RenderPassInfoBuilder

    RenderPassInfoBuilder::RenderPassInfoBuilder(DeviceBase* device) : Builder(device) {
    }

    RenderPassInfoBase* RenderPassInfoBuilder::GetResultImpl() {
        auto CheckOrSetSize = [this](const TextureViewBase* attachment) -> bool {
            if (this->mWidth == 0) {
                ASSERT(this->mHeight == 0);

                this->mWidth = attachment->GetTexture()->GetWidth();
                this->mHeight = attachment->GetTexture()->GetHeight();
                ASSERT(this->mWidth != 0 && this->mHeight != 0);

                return true;
            }

            ASSERT(this->mWidth != 0 && this->mHeight != 0);
            return this->mWidth == attachment->GetTexture()->GetWidth() &&
                   this->mHeight == attachment->GetTexture()->GetHeight();
        };

        uint32_t attachmentCount = 0;
        for (uint32_t i : IterateBitSet(mColorAttachmentsSet)) {
            attachmentCount++;
            if (!CheckOrSetSize(mColorAttachments[i].view.Get())) {
                HandleError("Attachment size mismatch");
                return nullptr;
            }
        }

        if (mDepthStencilAttachmentSet) {
            attachmentCount++;
            if (!CheckOrSetSize(mDepthStencilAttachment.view.Get())) {
                HandleError("Attachment size mismatch");
                return nullptr;
            }
        }

        if (attachmentCount == 0) {
            HandleError("Should have at least one attachment");
            return nullptr;
        }

        return mDevice->CreateRenderPassInfo(this);
    }

    void RenderPassInfoBuilder::SetColorAttachment(uint32_t attachment,
                                                   TextureViewBase* textureView,
                                                   nxt::LoadOp loadOp) {
        if (attachment >= kMaxColorAttachments) {
            HandleError("Setting color attachment out of bounds");
            return;
        }

        if (TextureFormatHasDepthOrStencil(textureView->GetTexture()->GetFormat())) {
            HandleError("Using depth stencil texture as color attachment");
            return;
        }

        mColorAttachmentsSet.set(attachment);
        mColorAttachments[attachment].loadOp = loadOp;
        mColorAttachments[attachment].view = textureView;
    }

    void RenderPassInfoBuilder::SetColorAttachmentClearColor(uint32_t attachment,
                                                             float clearR,
                                                             float clearG,
                                                             float clearB,
                                                             float clearA) {
        if (attachment >= kMaxColorAttachments) {
            HandleError("Setting color attachment out of bounds");
            return;
        }

        mColorAttachments[attachment].clearColor[0] = clearR;
        mColorAttachments[attachment].clearColor[1] = clearG;
        mColorAttachments[attachment].clearColor[2] = clearB;
        mColorAttachments[attachment].clearColor[3] = clearA;
    }

    void RenderPassInfoBuilder::SetDepthStencilAttachment(TextureViewBase* textureView,
                                                          nxt::LoadOp depthLoadOp,
                                                          nxt::LoadOp stencilLoadOp) {
        if (!TextureFormatHasDepthOrStencil(textureView->GetTexture()->GetFormat())) {
            HandleError("Using color texture as depth stencil attachment");
            return;
        }

        mDepthStencilAttachmentSet = true;
        mDepthStencilAttachment.depthLoadOp = depthLoadOp;
        mDepthStencilAttachment.stencilLoadOp = stencilLoadOp;
        mDepthStencilAttachment.view = textureView;
    }

    void RenderPassInfoBuilder::SetDepthStencilAttachmentClearValue(float clearDepth,
                                                                    uint32_t clearStencil) {
        mDepthStencilAttachment.clearDepth = clearDepth;
        mDepthStencilAttachment.clearStencil = clearStencil;
    }

}  // namespace backend