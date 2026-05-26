#! /bin/sh
# bootstrap.sh - Regenerate the configure script for octave_boost
#
# Run this if you modify configure.ac or if the pre-generated configure
# script is missing/outdated.
#
# Requirements: autoconf (>= 2.67), GNU m4
#
# If autoconf is not available, a pre-generated configure script is
# included in the distribution and can be used directly.

set -e

srcdir=`dirname "$0"`
test -n "$srcdir" && cd "$srcdir"

echo "Generating configure script..."
autoconf --force || {
  echo "*** autoconf not found or failed."
  echo "*** A pre-generated configure script exists; using it instead."
  echo "*** Install autoconf and re-run bootstrap.sh if you modify configure.ac."
  exit 0
}

echo "Done. Run ./configure && make to build."
