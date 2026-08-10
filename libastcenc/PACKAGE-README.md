# libastcenc

This is a `build2` package for the [ARM astc-encoder](https://github.com/ARM-software/astc-encoder)
C library. It provides Adaptive Scalable Texture Compression (ASTC) encoding
and decoding via the public C API in `<astcenc.h>`.


## Usage

To start using `libastcenc` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libastcenc ^5.7.0
```

Then import the library in your `buildfile`:

```
import libs = libastcenc%lib{astcenc}
```

Include the public header as:

```
#include <astcenc.h>
```


## Importable targets

This package provides the following importable targets:

```
lib{astcenc}
```

The core encoder/decoder library (static and shared variants). The `astcenc`
command-line tool is a separate package.


## Configuration variables

```
[string] config.libastcenc.isa ?= <target baseline>
```

SIMD backend. One of `none`, `native`, `sse2`, `sse41`, `avx2`, `neon`,
`sve128`, `sve256` (upstream `ASTCENC_ISA_*`).

The default is a portable production baseline for the target CPU: `sse2`
on x86_64, `neon` on little-endian aarch64 / arm64, `none` otherwise
(including big-endian targets). Override with `native` for host-tuned
codegen (`-march=native` on non-MSVC), or with a fixed ISA such as `avx2`.

Upstream SIMD backends are little-endian only. A big-endian build must use
`none` (or `native`, which follows the compiler). Combining an explicit
SIMD ISA with a big-endian target is rejected.

```
[bool] config.libastcenc.decompress_only ?= false
```

Build a decompress-only library (`ASTCENC_DECOMPRESS_ONLY`).

```
[bool] config.libastcenc.invariance ?= true
```

Floating-point invariance (upstream `ASTCENC_INVARIANCE`).

```
[bool] config.libastcenc.x86_gathers ?= true
```

Use native x86 gathers for the `avx2` backend (upstream `ASTCENC_X86_GATHERS`).
Ignored for every other ISA.

```
[uint64] config.libastcenc.block_max_texels ?= 0
```

Limit supported ASTC block sizes (`ASTCENC_BLOCK_MAX_TEXELS`). `0` keeps
the header default of 216 (all 2D and 3D sizes). Use `144` to drop 3D
`6x6x6` and shrink compressor tables.

```
[bool, null] config.libastcenc.big_endian ?= [null]
```

Big-endian target (`ASTCENC_BIG_ENDIAN`). The default is auto-detected
from the compiler/target via the `HAVE_BIGENDIAN` autoconf check. Set
`true` or `false` to override. The define is applied only for `isa=none`,
matching upstream.
