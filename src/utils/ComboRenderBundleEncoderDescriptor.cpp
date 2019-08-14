// Copyright 2019 The Dawn Authors
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

#include "utils/ComboRenderBundleEncoderDescriptor.h"

#include "utils/DawnHelpers.h"

namespace utils {

    ComboRenderBundleEncoderDescriptor::ComboRenderBundleEncoderDescriptor() {
        dawn::RenderBundleEncoderDescriptor* descriptor = this;

        descriptor->sampleCount = 1;
        descriptor->depthStencilFormat = nullptr;
        descriptor->colorFormatsCount = 0;
        descriptor->colorFormats = &cColorFormats[0];
    }

}  // namespace utils