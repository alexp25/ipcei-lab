---
title: "L9 — Testing, Documentare & Prezentare Finală"
description: "Unity, Doxygen, CI/CD, Show & Tell NXP"
nav_order: 10
parent: Lecții FRDM-MCXA153
layout: lesson
---

# ✅ L9 — Testing, Documentare & Prezentare Finală

**Unity, Doxygen, CI/CD, Show & Tell NXP**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 5 + Săptămâna 2 online |
| **Periferic** | `Unity · Doxygen · GitHub Actions · Show & Tell` |
| **Durată** | 2h sesiune + async |
| **Responsabil** | Cadru UPB + Mariana Mocanu |
| **Hardware** | FRDM-MCXA153 (pentru demo final) · PC (pentru CI/CD și documentare) |

## 📌 Context și Motivație

Ultima sesiune de conținut este despre calitatea codului și comunicare — competențe direct valorificate la NXP. Testing pentru embedded fără hardware: Unity compilat pentru host (x86) cu funcții mock pentru SDK. Documentarea cu Doxygen respectă convențiile interne NXP. Prezentarea finală în format **Show & Tell NXP** este evaluată de panelul UPB-NXP pe 2–3 septembrie.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. Cel puțin 5 unit tests scrise cu Unity pentru funcțiile pure (conversii, decodare registre)
2. Documentație Doxygen generată automat din comentarii — HTML vizibil în browser
3. CI GitHub Actions: build + test la fiecare push (cross-compile pentru host, mock SDK)
4. Draft prezentare finală structurat: problemă → arhitectura → demo → comparativ AI → reflecție

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `10:00–10:45` | **Unity testing embedded** | Mock-uri pentru SDK NXP. Testare funcții pure: `p3t1755_raw_to_celsius()`, `adc_raw_to_mv()`. CI GitHub Actions cu ARM GCC cross-compile pentru host. | cadru UPB + Stefan |
| `10:45–11:30` | **Doxygen cu GenAI** | Generare comentarii Doxygen cu AI pentru funcțiile proprii. Configurare Doxyfile. GitHub Pages pentru doc HTML. Stil: NXP SDK conventions. | cadru UPB |
| `11:30–12:00` | **Template prezentare finală** | 10 slide-uri max: problemă → arhitectura → demo live → comparativ baseline vs AI → reflecție GenAI → concluzii. | Mariana Mocanu |
| `12:00–13:00` | **Rehearsal per echipă** | 5 min prezentare draft + feedback rapid. Agentul AI evaluează calitatea secțiunii de reflecție GenAI. | panel online |

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: Unity tests conversii

```text
FRDM-MCXA153, SDK MCUXpresso 24.12, Unity test framework compilat pentru host (x86).
Sarcina: test suite pentru float p3t1755_raw_to_celsius(int16_t raw).
Test cases (raw = valoarea registrului înainte de shift >> 5):
  0x0000 →   0.000°C
  0x00C8 →  25.000°C
  0x0190 →  50.000°C
  0xFF38 → -25.000°C  (negativ, complementul față de 2, 11 biți)
Include: UnityBegin, RUN_TEST, TEST_ASSERT_FLOAT_WITHIN (toleranță ±0.125°C).
IMPORTANT: funcția testată este PURĂ — mock-ul LPI2C nu este necesar.
```

### Prompt: Comentarii Doxygen NXP style

```text
Generează comentarii Doxygen complete, stil NXP SDK, pentru funcțiile:
  float p3t1755_read_temp_celsius(void)
  void p3t1755_set_threshold(float temp_c)
  status_t p3t1755_init(../lPI2C_Type *base, uint32_t src_clock_hz)
Include per funcție:
  @brief   — 1 linie
  @param   — tip + descriere
  @return  — valori posibile
  @note    — limitări hardware, VREF, thread safety
  @warning — dacă aplicabil
Format: doxygen C style ( /** ... */ ), fără markdown.
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- **Unity pe host (x86)**: compilați pentru host cu mock-uri pentru funcțiile SDK — **NU** cross-compile în Unity mode pe Cortex-M33
- Doxygen cu `INPUT` recursiv prinde header-ele SDK NXP (mii de fișiere) — filtrați cu `EXCLUDE_PATTERNS`
- **Secțiunea reflecție GenAI** (ce a greșit AI, ce ați corectat) diferențiază o prezentare bună de una excelentă

## ✅ Deliverable

> 5+ unit tests trecute (toate verde) + Doxygen HTML generat + CI verde pe GitHub + draft prezentare V1 pe Moodle

---

[← L8: Integrare — Sistem Complet cu FSM](../l8-integrare)
