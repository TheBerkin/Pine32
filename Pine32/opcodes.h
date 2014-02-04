#pragma once

#define OP_PUSC			0x00
#define OP_PUSX			0x01
#define OP_ADD			0x02
#define OP_SUB			0x03
#define OP_MUL			0x04
#define OP_DIV			0x05
#define OP_LERP			0x06
#define OP_CLAMP		0x07
#define OP_MOD			0x08
#define OP_SQRT			0x09
#define OP_POW			0x0a
#define OP_CEIL			0x0b
#define OP_FLOOR		0x0c
#define OP_RANGE		0x0d
#define OP_RANGEND		0x0e
#define OP_SIN			0x0f
#define OP_COS			0x10
#define OP_TAN			0x11
#define OP_ATAN			0x12
#define OP_ABS			0x13
#define OP_RAND			0x14
#define OP_COPY			0x15
#define OP_LE			0x16
#define OP_GE			0x17
#define OP_LT			0x18
#define OP_GT			0x19
#define OP_PUSR			0x1a
#define OP_POP			0x1b
#define OP_FIRE			0x1c
#define OP_ZERO			0x1d
#define OP_JNZ			0x1e
#define OP_JMP			0x1f
#define OP_NOT			0x20
#define OP_JALT			0x21
#define OP_RET			0x22
#define OP_CALL			0x23
#define OP_EQ			0x24
#define OP_NE			0x25
#define OP_SUM			0x26
#define OP_PRD			0x27
#define OP_ILERP		0x28
#define OP_STOP			0x29
#define OP_OUTS			0x2a
#define OP_OUTR			0x2b
#define OP_OUTC			0x2c

// Timer ops
// t0-t16
#define NUM_TIMERS		16

#define OP_TIMER_START	0x2d // t.start ID
#define OP_TIMER_STOP	0x2e // t.stop ID
#define OP_TIMER_SET	0x2f // t.set ID TICKS
#define OP_TIMER_STATUS 0x30 // t.status ID : (-1 = off, 0 = on, 1 = done)
#define OP_TIMER_GETT	0x31 // t.gett ID : (elapsed ticks)
#define OP_TIMER_GETL	0x32 // t.getl ID : (timer length)
#define OP_TIMER_RESET	0x33 // t.reset ID 