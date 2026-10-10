#ifndef THREADS_FIXED_POINT_H
#define THREADS_FIXED_POINT_H

#include <stdint.h>

/* 17.14 Fixed-Point Arithmetic 
   x and y are fixed-point numbers, n is an integer. */
#define F (1 << 14)

// advanced scheduler 여기 구현
// 1. 정수를 고정 소수점으로 변환한다.
#define INT_TO_FP(n) ((n) * (F))

// 2. 고정 소수점을 정수로 변환한다 (버림).
#define FP_TO_INT_ZERO(x) ((x) / (F))

// 3. 고정 소수점을 정수로 변환한다 (반올림).
#define FP_TO_INT_NEAREST(x) ((x) >= 0 ? (((x) + (F) / 2) / (F)) : (((x) - (F) / 2) / (F)))

// 4. 사칙연산 매크로
#define ADD_FP(x, y) ((x) + (y))
#define SUB_FP(x, y) ((x) - (y))
#define ADD_MIX(x, n) ((x) + (n) * (F))
#define SUB_MIX(x, n) ((x) - (n) * (F))
#define MUL_FP(x, y) ((int32_t)(((int64_t)(x)) * (y) / (F)))
#define MUL_MIX(x, n) ((x) * (n))
#define DIV_FP(x, y) ((int32_t)(((int64_t)(x)) * (F) / (y)))
#define DIV_MIX(x, n) ((x) / (n))

#endif /* threads/fixed_point.h */
