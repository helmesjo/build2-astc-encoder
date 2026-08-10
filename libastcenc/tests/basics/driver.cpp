// Public API smoke test for libastcenc.

#include <astcenc.h>

#undef NDEBUG
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>

int main ()
{
#if defined(ASTCENC_DECOMPRESS_ONLY)
  const unsigned int flags = ASTCENC_FLG_DECOMPRESS_ONLY;
#else
  const unsigned int flags = 0;
#endif

  // Config for a 4x4 LDR block.
  //
  astcenc_config config{};
  astcenc_error status = astcenc_config_init (ASTCENC_PRF_LDR,
                                             4, 4, 1,
                                             ASTCENC_PRE_FAST,
                                             flags,
                                             &config);
  assert (status == ASTCENC_SUCCESS);

  astcenc_context* context = nullptr;
  status = astcenc_context_alloc (&config, 1, &context, nullptr);
  assert (status == ASTCENC_SUCCESS);
  assert (context != nullptr);

  static const astcenc_swizzle swizzle {
    ASTCENC_SWZ_R,
    ASTCENC_SWZ_G,
    ASTCENC_SWZ_B,
    ASTCENC_SWZ_A,
  };

  constexpr unsigned int dim = 4;
  std::vector<uint8_t> compressed (16, 0);

#if defined(ASTCENC_DECOMPRESS_ONLY)
  // Solid-red 4x4 LDR block captured from this encoder at ASTCENC_PRE_FAST.
  //
  static const uint8_t baked[16] = {
    0xfc, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff
  };
  std::memcpy (compressed.data (), baked, sizeof (baked));
#else
  // One 4x4 solid-red image, tightly packed RGBA8.
  //
  std::vector<uint8_t> src (dim * dim * 4, 0);
  for (unsigned int i = 0; i < dim * dim; ++i)
  {
    src[i * 4 + 0] = 255;
    src[i * 4 + 1] = 0;
    src[i * 4 + 2] = 0;
    src[i * 4 + 3] = 255;
  }

  uint8_t* slices[1] = {src.data ()};
  astcenc_image image{};
  image.dim_x = dim;
  image.dim_y = dim;
  image.dim_z = 1;
  image.data_type = ASTCENC_TYPE_U8;
  image.data = reinterpret_cast<void**> (slices);

  // ASTC 4x4 is 16 bytes per block. One block for a 4x4 image.
  //
  status = astcenc_compress_image (context,
                                   &image,
                                   &swizzle,
                                   compressed.data (),
                                   compressed.size (),
                                   0);
  assert (status == ASTCENC_SUCCESS);

  status = astcenc_compress_reset (context);
  assert (status == ASTCENC_SUCCESS);
#endif

  std::vector<uint8_t> dst (dim * dim * 4, 0);
  uint8_t* dst_slices[1] = {dst.data ()};
  astcenc_image out{};
  out.dim_x = dim;
  out.dim_y = dim;
  out.dim_z = 1;
  out.data_type = ASTCENC_TYPE_U8;
  out.data = reinterpret_cast<void**> (dst_slices);

  status = astcenc_decompress_image (context,
                                     compressed.data (),
                                     compressed.size (),
                                     &out,
                                     &swizzle,
                                     0);
  assert (status == ASTCENC_SUCCESS);

  // Solid red should decode closely (codec is lossy, so not bit-exact).
  //
  assert (dst[0] > 200);
  assert (dst[1] < 30);
  assert (dst[2] < 30);
  assert (dst[3] > 200);

  astcenc_context_free (context);
  return 0;
}
