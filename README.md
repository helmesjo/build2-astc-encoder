# astc-encoder

This is a `build2` package repository for
[`astc-encoder`](https://github.com/ARM-software/astc-encoder), an Adaptive
Scalable Texture Compression (ASTC) encoder and decoder.

This file contains setup instructions and other details that are more
appropriate for development rather than consumption. If you want to use
`libastcenc` in your `build2`-based project, then instead see
[`libastcenc/PACKAGE-README.md`](libastcenc/PACKAGE-README.md). For the
`astcenc` command line tool, see
[`astcenc/PACKAGE-README.md`](astcenc/PACKAGE-README.md).

The development setup for `astc-encoder` uses the standard `bdep`-based workflow.
For example:

```
git clone --recursive https://github.com/build2-packaging/astc-encoder.git
cd astc-encoder

bdep init -C @gcc cc config.cxx=g++
bdep update
bdep test
```
