---
title: "L9 - Testing, Documentation and Final Presentation"
description: "Unity, Doxygen, CI/CD, and final NXP-style show-and-tell"
nav_order: 10
parent: FRDM-MCXA153 Lessons
layout: lesson
source_url: https://github.com/alexp25/ipcei-lab/tree/main/src
---

# L9 - Testing, Documentation and Final Presentation

**Unity tests, Doxygen documentation, CI, and final demo preparation**

---

| | |
|---|---|
| **Session** | Testing and presentation |
| **Topics** | `Unity`, `Doxygen`, `GitHub Actions`, `Show & Tell` |
| **Hardware** | FRDM-MCXA153 for final demo, PC for CI and documentation |

## Context and Motivation

The final content session focuses on code quality and communication. These skills matter in real embedded teams: code must be testable, documented, reproducible, and explainable.

The target workflow is:

1. unit-test pure logic on the host;
2. document public APIs with Doxygen;
3. run build/tests in CI;
4. prepare a concise final presentation with a live demo and AI reflection.

## Objectives

By the end of the lab you should be able to:

1. Write at least five Unity unit tests for pure functions.
2. Generate Doxygen HTML documentation from C comments.
3. Configure GitHub Actions for build and tests.
4. Mock SDK/hardware functions when testing on the host.
5. Prepare a final presentation: problem, architecture, demo, AI comparison, reflection.

## What to Test

Prefer pure functions that do not touch hardware directly:

- P3T1755 raw-to-Celsius conversion;
- ADC raw-to-millivolt conversion;
- ADC raw-to-threshold mapping;
- button threshold decoder;
- FSM transition function;
- RGB duty-cycle mapping.

Example test targets:

```c
float p3t1755_raw_to_celsius(uint8_t msb, uint8_t lsb);
uint32_t adc_raw_to_mv(uint16_t raw);
app_state_t process_fsm_step(app_state_t state, const app_inputs_t *inputs);
```

## Unity Test Prompt

```text
FRDM-MCXA153 project, MCUXpresso SDK, Unity test framework compiled for host x86.
Task: write a Unity test suite for pure conversion functions.
Test p3t1755_raw_to_celsius(msb, lsb) using:
- 0x19 0x00 -> 25.0 C
- 0x00 0x00 -> 0.0 C
- 0xE7 0x00 -> -25.0 C
Use TEST_ASSERT_FLOAT_WITHIN with a small tolerance.
The function is pure; do not mock LPI2C.
Include UnityBegin, RUN_TEST, and UnityEnd.
```

## Doxygen Prompt

```text
Generate Doxygen comments in NXP SDK style for these functions:
- status_t p3t1755_init(...)
- status_t p3t1755_read_celsius(...)
- uint32_t adc_raw_to_mv(uint16_t raw)
For each function include:
@brief, @param, @return, @note, and @warning if needed.
Use C Doxygen comment style /** ... */.
Do not use Markdown tables inside the comments.
```

## Host Testing Pattern

Separate pure logic from hardware access. For example:

```text
p3t1755.c        pure decode/conversion + protocol-level API
onboard_temp.c   board-specific LPI2C adapter
test_p3t1755.c   host tests for pure conversion
```

Hardware-facing code can be mocked or excluded from host tests. Do not try to run board peripheral initialization on the host.

## CI Checklist

A minimal CI flow should:

1. install build dependencies;
2. configure the host/unit-test build;
3. compile tests;
4. run tests;
5. optionally build firmware;
6. upload Doxygen or test reports as artifacts.

Keep host tests independent from connected hardware.

## Doxygen Checklist

- Document public headers first.
- Do not recursively include the whole MCUXpresso SDK in Doxygen input.
- Use `EXCLUDE_PATTERNS` to avoid thousands of SDK files.
- Document ownership and thread/ISR restrictions.
- Explain units and ranges: Celsius, millivolts, raw ADC range, I2C address format.

## Final Presentation Structure

Keep the final talk short and concrete:

1. Problem and hardware context.
2. System architecture diagram.
3. Peripheral map.
4. Live demo.
5. One important debugging story.
6. Baseline vs AI-assisted workflow.
7. What AI got wrong.
8. How the team verified and corrected it.
9. Tests and documentation status.
10. Final conclusion.

## Common Pitfalls

- Do not compile Unity tests for Cortex-M when the goal is host testing.
- Do not include the whole SDK in Doxygen input.
- Do not test hardware drivers by running board initialization on the host.
- Do not accept AI-generated tests without checking expected values.
- The AI reflection is part of the engineering result, not decoration.

## Deliverable

Submit:

1. at least five passing Unity tests;
2. generated Doxygen HTML;
3. green CI or documented CI attempt;
4. final presentation draft;
5. short AI reflection: wrong suggestion, verification method, final fix.

---

[<- L8: Integration - Complete FSM System](../l8-integrare)

