/**
 * GeblomiSort — root entry point
 * ==============================
 * Canonical full single-header lives at:
 *   public/geblomi-sort/GeblomiSort.hpp
 *
 * This root file re-exports it so `#include "GeblomiSort.hpp"` works
 * from the repository root and from examples/demo.cpp.
 *
 * For a standalone drop-in, copy public/geblomi-sort/GeblomiSort.hpp
 * (complete amalgamated pdqsort + ska_sort + Geblomi hybrid, ~83 KB).
 */
#pragma once

#include "public/geblomi-sort/GeblomiSort.hpp"
