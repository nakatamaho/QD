# libQD3

libQD3 is a fork of the [QD library](https://github.com/BL-highprecision/QD).
It keeps the original double-double and quad-double functionality and adds
native triple-double support.

libQD3 provides extended-precision floating-point types for C++, C, and
Fortran:

- `dd_real`: double-double precision, about 106 mantissa bits
- `td_real`: triple-double precision, about 159 mantissa bits
- `qd_real`: quad-double precision, about 212 mantissa bits

## News
2026-04-22 libQD3 1.1.0 was released.  This release adds
binary80-based `edd_real` extended-double support, including core
arithmetic, constants, conversions, transcendental functions, a small C API,
documentation, and AGM examples.  See
[CHANGES.1.1.0.md](CHANGES.1.1.0.md) for the 1.1.0 release notes.

2026-04-20 libQD3 1.0.0 was released.  This first release adds
TD/triple-double support through the new `td_real` type.  See
[CHANGES.1.0.0.md](CHANGES.1.0.0.md) for the 1.0.0 release notes.

## Citation
Y. Hida, X. S. Li and D. H. Bailey, "Algorithms for quad-double precision floating point arithmetic," Proceedings 15th IEEE Symposium on Computer Arithmetic. ARITH-15 2001, Vail, CO, USA, 2001, pp. 155-162, doi: 10.1109/ARITH.2001.930115.

## End-user documentation

For historical reasons, the original user-focused documentation is
located in the `README` file, sans the `.md` extension. In-depth
technical documentation is also provided in the `docs` directory.
Release tarballs include the generated PDFs.

To build the PDFs from LaTeX sources, run

```
$ make -C docs qd.pdf
$ make -C docs td.pdf
```

after installing the necessary LaTeX bits on your system.

Release-specific notes for libQD3 1.1.0 are in
[CHANGES.1.1.0.md](CHANGES.1.1.0.md).

## Tips for developers

Before you can build libQD3 from the git repository, you will likely have
to generate the "autotools" files (such as `./configure`) that are
normally provided for you in a release tarball. The easiest way to do
that is to run,

```
$ autoreconf -fi
```

as a shortcut for running each of *autoconf*, *autoheader*, *aclocal*,
*automake*, and *libtoolize* as many times as necessary and in the
correct order. The `-fi` flags force autoreconf to regenerate
everything, and to install any missing auxiliary files.  You will of
course need all of the relevant autotools packages (autoconf,
automake, and libtool) installed for this to work.

Afterwards, you can configure and build the package normally:

```
$ ./configure
$ make
$ make check
```

### Running the test suite

The libQD3 library comes with an automated test suite that should always
pass. To run it, execute `make check` after building the
library. Ignoring the noise from the compiler (the test suite itself
must be compiled), the output should look something like

```
$ ./configure
...
$ make
...
$ make check
...
PASS: qd_test
PASS: pslq_test
PASS: c_test
PASS: huge
PASS: f_test
============================================================================
Testsuite summary for qd3 1.0.0
============================================================================
# TOTAL: 5
# PASS:  5
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================

```

The `f_test` entry is present when Fortran support is enabled.  The main
numeric test can also be run by precision:

```
$ ./tests/qd_test -dd
$ ./tests/qd_test -td
$ ./tests/qd_test -qd
```

For release-oriented configuration coverage, run:

```
$ bash qa/check_16_builds.sh
```

This performs out-of-tree builds for the default configuration plus all
16 combinations of `ieee_add`, `sloppy_mul`, `sloppy_div`, and `fma`.
It writes logs and summaries under `_build_matrix/` by default.

### Making a release

There are several steps that need to be performed when making a new
release:

1. Ensure that all important user-facing changes are mentioned
   in the `NEWS` file and the appropriate `CHANGES.*.md` file.

2. Update the package version number in `configure.ac`.

3. Build a release tarball. First, run `git clean -x -f -d` to ensure
   that you're starting fresh. Then run `autoreconf -fi` to regenerate
   all of the autotools files. Run `./configure` to create your
   Makefiles, and finally, `make dist` to create the release tarball.

4. Run `bash qa/check_16_builds.sh` to validate the release matrix.

5. Run `make distcheck` to ensure that the release tarball works.

6. Tag the commit that corresponds to the release with `git tag -s
   <version-number>`.

7. Push everything to GitHub.

8. Upload the release tarball (created earlier) to the GitHub release
   page that corresponds to your new version tag. This ensures that
   end-users can run `./configure` and such "out of the box," without
   having to install the GNU autotools (or learn their commands).
