/* Copyright 2013 Google Inc. All Rights Reserved.

   Distributed under MIT license.
   See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
*/

#include "./dictionary.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

static const BrotliDictionary kBrotliDictionary = {
  /* size_bits_by_length */
  {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },

  /* offsets_by_length */
  {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  },

  /* data */
  {
    0
  }
};

const BrotliDictionary* BrotliGetDictionary() {
  return &kBrotliDictionary;
}

#if defined(__cplusplus) || defined(c_plusplus)
}  /* extern "C" */
#endif
