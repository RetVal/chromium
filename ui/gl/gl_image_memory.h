// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_IMAGE_MEMORY_H_
#define UI_GL_GL_IMAGE_MEMORY_H_

#include "ui/gl/gl_image.h"

#if defined(OS_WIN) || defined(USE_X11) || defined(OS_ANDROID) || \
    defined(USE_OZONE)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#include "base/numerics/safe_math.h"
#include "ui/gfx/gpu_memory_buffer.h"

namespace gfx {

class GL_EXPORT GLImageMemory : public GLImage {
 public:
  GLImageMemory(const gfx::Size& size, unsigned internalformat);

  static bool StrideInBytes(size_t width,
                            gfx::GpuMemoryBuffer::Format format,
                            size_t* stride_in_bytes);

  static bool ValidSize(const gfx::Size& size,
                        gfx::GpuMemoryBuffer::Format format);

  bool Initialize(const unsigned char* memory,
                  gfx::GpuMemoryBuffer::Format format);

  // Overridden from GLImage:
  void Destroy(bool have_context) override;
  gfx::Size GetSize() override;
  bool BindTexImage(unsigned target) override;
  void ReleaseTexImage(unsigned target) override {}
  bool CopyTexImage(unsigned target) override;
  void WillUseTexImage() override;
  void DidUseTexImage() override;
  void WillModifyTexImage() override {}
  void DidModifyTexImage() override {}
  bool ScheduleOverlayPlane(gfx::AcceleratedWidget widget,
                            int z_order,
                            OverlayTransform transform,
                            const Rect& bounds_rect,
                            const RectF& crop_rect) override;

 protected:
  ~GLImageMemory() override;

 private:
  void DoBindTexImage(unsigned target);

  const gfx::Size size_;
  const unsigned internalformat_;
  const unsigned char* memory_;
  gfx::GpuMemoryBuffer::Format format_;
  bool in_use_;
  unsigned target_;
  bool need_do_bind_tex_image_;
#if defined(OS_WIN) || defined(USE_X11) || defined(OS_ANDROID) || \
    defined(USE_OZONE)
  unsigned egl_texture_id_;
  EGLImageKHR egl_image_;
#endif

  DISALLOW_COPY_AND_ASSIGN(GLImageMemory);
};

}  // namespace gfx

#endif  // UI_GL_GL_IMAGE_MEMORY_H_
