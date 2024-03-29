/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef AOM_AV1_COMMON_ALLOCCOMMON_H_
#define AOM_AV1_COMMON_ALLOCCOMMON_H_

#define INVALID_IDX -1  // Invalid buffer index.

#include "config/aom_config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct AV1Common;
struct BufferPool;

void av1_remove_common(struct AV1Common *cm);

int av1_alloc_above_context_buffers(struct AV1Common *cm,
                                    int num_alloc_above_contexts);
void av1_free_above_context_buffers(struct AV1Common *cm,
                                    int num_free_above_contexts);
int av1_alloc_context_buffers(struct AV1Common *cm, int width, int height);
void av1_init_context_buffers(struct AV1Common *cm);
void av1_free_context_buffers(struct AV1Common *cm);

void av1_free_ref_frame_buffers(struct BufferPool *pool);
void av1_alloc_restoration_buffers(struct AV1Common *cm);
void av1_free_restoration_buffers(struct AV1Common *cm);

int av1_alloc_state_buffers(struct AV1Common *cm, int width, int height);
void av1_free_state_buffers(struct AV1Common *cm);

int av1_get_MBs(int width, int height);

#if CONFIG_LPF_MASK
static INLINE int alloc_loop_filter_mask(AV1_COMMON *cm) {
  aom_free(cm->lf.lfm);
  cm->lf.lfm = NULL;

  // Each lfm holds bit masks for all the 4x4 blocks in a max
  // 64x64 (128x128 for ext_partitions) region.  The stride
  // and rows are rounded up / truncated to a multiple of 16
  // (32 for ext_partition).
  cm->lf.lfm_stride = (cm->mi_cols + (MI_SIZE_64X64 - 1)) >> MIN_MIB_SIZE_LOG2;
  cm->lf.lfm_num = ((cm->mi_rows + (MI_SIZE_64X64 - 1)) >> MIN_MIB_SIZE_LOG2) *
                   cm->lf.lfm_stride;
  cm->lf.lfm =
      (LoopFilterMask *)aom_calloc(cm->lf.lfm_num, sizeof(*cm->lf.lfm));
  if (!cm->lf.lfm) return 1;

  unsigned int i;
  for (i = 0; i < cm->lf.lfm_num; ++i) av1_zero(cm->lf.lfm[i]);

  return 0;
}

static INLINE void free_loop_filter_mask(AV1_COMMON *cm) {
  if (cm->lf.lfm == NULL) return;

  aom_free(cm->lf.lfm);
  cm->lf.lfm = NULL;
  cm->lf.lfm_num = 0;
  cm->lf.lfm_stride = 0;
}
#endif

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AOM_AV1_COMMON_ALLOCCOMMON_H_
