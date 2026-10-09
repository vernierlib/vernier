#!/bin/sh
# Regenerates the peak search comparison from its pictures: runs both peak
# searches of the megarena detector on every picture listed in truth.csv with
# the comparingPeakSearch example, scores them against the truth, draws the
# figures and builds the PDF. About 11 minutes on 12 cores. Needs a build of
# the library with its examples, uv (for matplotlib and pandas) and pdflatex.
#
#   doc/ring-peak-search/run.sh <build-dir> <picture-dir>
#
# <picture-dir> holds the 704 pictures named in truth.csv: 16-bit PNG,
# 1280x720, linear intensities.
set -eu
build=$(realpath "$1")
pictures=$(realpath "$2")
cd "$(dirname "$0")"
"$build/examples/comparingPeakSearch" truth.csv "$pictures" 6 results.csv
python3 analyse.py
uv run --quiet --with matplotlib --with pandas --with pillow python plot.py
cd report
pdflatex -interaction=nonstopmode ring-peak-search.tex > /dev/null
pdflatex -interaction=nonstopmode ring-peak-search.tex > /dev/null
