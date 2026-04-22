# Changes for libQD3 1.1.0

## Release Highlights

- Added `edd_real`, a native two-limb extended-double type based on binary80
  `_Float64x`, as the main feature of libQD3 1.1.0.
- Added `edd_real` core arithmetic, normalization, comparisons, parsing,
  formatting, constants, conversions, and mixed-mode usability support.
- Added a practical `edd_real` transcendental layer plus a small tested C API.
- Added `edd_real` documentation and AGM-based example programs.
- Updated release packaging so `make dist` now generates `.tar.xz`.

## Project Versioning and Packaging

- Set the package version to `1.1.0`.
- Kept the package name as `qd3`.
- Updated Automake dist settings so `make dist` builds an `.tar.xz` release
  archive instead of the previous gzip archive path.

## New `edd_real` Extended-Double Type

- Added native `edd_real`, represented as a normalized two-word expansion of
  binary80 `_Float64x` values.
- Added configure-time gating so `edd_real` is enabled only when GNU C++
  provides `_Float64x` with the intended binary80 semantics.
- Implemented core `edd_real` support for:
  constructors, assignment, unary minus, comparisons, addition, subtraction,
  multiplication, division, `sqr`, `sqrt`, `abs`, `fabs`, parsing,
  formatting, stream I/O, and conversion helpers.
- Implemented the required `_Float64x` error-free transformation building
  blocks and renormalization helpers directly for the binary80 limb type.
- Kept `edd_real` core arithmetic native to the two-limb `_Float64x`
  representation rather than routing operations through `qd_real`.

## `edd_real` Constants and Conversions

- Added a practical `edd_real` constant set including:
  `_nan`, `_inf`, `_eps`, `_min_normalized`, `_max`, `_safe_max`, `_ndigits`,
  `_2pi`, `_pi`, `_3pi4`, `_pi2`, `_pi4`, `_e`, `_log2`, and `_log10`.
- Added `dd_real` to `edd_real` and `qd_real` to `edd_real` conversions.
- Added `edd_real` to `dd_real` and `edd_real` to `qd_real` conversions.
- Added clean `_Float64x` conversion paths for `edd_real`.
- Added selected mixed-mode arithmetic and comparison support between
  `edd_real` and `_Float64x`.

## `edd_real` Transcendental Functions

- Added `edd_real` transcendental support for:
  `exp`, `log`, `log10`, `sin`, `cos`, `tan`, `sincos`, `atan`, `atan2`,
  `asin`, `acos`, `sinh`, `cosh`, `sincosh`, `tanh`, `nroot`, and `nint`.
- Kept `exp`, `log`, `log10`, and the hyperbolic functions on native
  `edd_real` paths with `_Float64x` seed values where needed.
- Added explicit bounded-range qd-backed fallback paths for selected
  trigonometric and inverse-trigonometric functions.
- Preserved normalized non-overlapping two-limb output form for public
  transcendental results.

## C API

- Added a small C wrapper layer for `edd_real`.
- Added C-facing helpers for:
  construction and copying, arithmetic, comparisons, constants, and selected
  transcendental functions.
- Added C tests to cover the new `edd_real` wrapper path.

## Documentation and Examples

- Added `docs/edd.tex`.
- Added `edd` document build targets to `docs/Makefile`.
- Added `examples/edd_agm.cpp`, an AGM-based `edd_real` example computing:
  `pi`, `log(2)`, `log(10)`, `log10(2)`, and `log10(e)`.
- Added `examples/README` build and run instructions for the AGM example.

## Tests

- Extended `tests/qd_test.cpp` with `-edd` coverage.
- Added `edd_real` tests for:
  normalization invariants, cancellation-sensitive arithmetic,
  mixed-magnitude operations, division, square roots, parsing, formatting,
  constants, conversions, mixed-mode `_Float64x` arithmetic, and
  transcendental identities.
- Added direct comparisons against `qd_real` oracle values on safe overlapping
  ranges for the new `edd_real` transcendental layer.
- Added tests for large-argument reduction behavior and wide-range
  `exp` / `log` paths.

## Scope Notes

- No Fortran support was added for `edd_real` in libQD3 1.1.0.
- Existing `dd_real`, `td_real`, and `qd_real` code paths were kept intact.
