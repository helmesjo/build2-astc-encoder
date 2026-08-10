# astcenc

This is a `build2` package for the
[ARM astc-encoder](https://github.com/ARM-software/astc-encoder) command line
tool. It compresses and decompresses Adaptive Scalable Texture Compression
(ASTC) images.


## Usage

To start using `astcenc` in your project, add the following build-time
`depends` value to your `manifest`, adjusting the version constraint as
appropriate:

```
depends: * astcenc ^5.7.0
```

Then import the executable in your `buildfile`:

```
import astcenc = astcenc%exe{astcenc}
```


## Importable targets

This package provides the following importable targets:

```
exe{astcenc}
```

The `astcenc` command line tool.


## Configuration variables

This package provides no configuration variables of its own. The CLI is built
with the same ISA and feature flags as the linked `libastcenc` package
(`config.libastcenc.*`).
