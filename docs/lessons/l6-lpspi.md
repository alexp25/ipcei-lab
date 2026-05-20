---
title: "L6 — LPSPI — Comunicație SPI & Device Extern"
description: "Master Mode 0, Arduino header, MAX7219"
nav_order: 7
parent: Lecții FRDM-MCXA153
layout: default
---

# 🔗 L6 — LPSPI — Comunicație SPI & Device Extern

**Master Mode 0, Arduino header, MAX7219**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 3, 24 iunie — după-amiază |
| **Periferic** | `LPSPI0 · Arduino Header · FIFO` |
| **Durată** | 2h (13:00–15:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 · device SPI pe Arduino header (MAX7219, SD card, sau loopback MOSI→MISO) |

## 📌 Context și Motivație

FRDM-MCXA153 expune LPSPI0 pe Arduino header (D10–D13) — compatibil cu shield-uri Arduino. LP = Low Power, funcționează în stop mode. API mai complex decât SPI AVR (FIFO, frame size, CS modes) dar conceptual identic: MOSI/MISO/SCK/CS. Relevanță NXP: SPI este protocolul principal pentru senzorii externi în automotive.

> **Board:** FRDM-MCXA153 · MCX A153 (Cortex-M33 @ 96 MHz) · SDK MCUXpresso 24.12 · VS Code + CMake

## 🎯 Obiective

1. LPSPI0 Master Mode 0 (CPOL=0, CPHA=0) configurat la 1 MHz
2. Funcții generice `spi_init()` și `spi_transfer(tx, rx, len)` implementate
3. Loopback MOSI→MISO verificat cu datele identice în TX și RX
4. Driver pentru un dispozitiv extern pe Arduino header (MAX7219 sau echivalent)

## 🕐 Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `13:00–13:30` | **LPSPI arhitectură** | FIFO TX/RX, frame size, prescaler, CS modes hardware vs software. CPOL/CPHA Mode 0 vs Mode 3. Comparație SPCR/SPDR AVR. | cadru UPB |
| `13:30–14:15` | **Demo: loopback test** | `LPSPI_MasterGetDefaultConfig()`, `LPSPI_MasterInit()`, `LPSPI_MasterTransferBlocking()`. Loopback MOSI→MISO → verificare date identice pe terminal. | cadru UPB |
| `14:15–15:00` | **Lab + GenAI session** | Studenții: driver device extern specific (MAX7219 LED matrix). Prompt cu protocol device inclus. | student + mentor |

## 🤖 Prompte pentru Asistentul AI UPB

> **Regulă:** Copiați prompt-ul complet — contextul hardware este obligatoriu.
> AI-ul va genera cod greșit (pentru alte familii NXP sau Arduino) fără aceste informații.

### Prompt: LPSPI0 master

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33 @ 96 MHz, SDK MCUXpresso 24.12.
LPSPI0 Master Mode 0 (CPOL=0, CPHA=0), frecvență SPI=1 MHz.
CS manual pe PIO1_20 (Arduino D10) — GPIO output.
Sarcina:
  void spi_init(void)
  status_t spi_transfer(uint8_t *tx, uint8_t *rx, size_t len)
Include: LPSPI_MasterGetDefaultConfig, LPSPI_MasterInit, LPSPI_MasterTransferBlocking.
Clock source: FRO_HF_DIV (48 MHz) → LPSPI0.
```

### Prompt: MAX7219 driver

```text
FRDM-MCXA153, LPSPI0 Master — spi_init() și spi_transfer() deja implementate.
Device: MAX7219 LED matrix. Protocol: frame 16 biți (reg 8b + data 8b), MSB first, Mode 0.
CS: PIO1_20 active LOW (GPIO manual).
Sarcina:
  void max7219_write(uint8_t addr, uint8_t data)
  void max7219_init()
Secvența init: DECODE_MODE=0x00, INTENSITY=0x08, SCAN_LIMIT=0x07, SHUTDOWN=0x01.
Afișare cifra '5' pe digit 0.
```

## ⚠️ Capcane Critice

> Ce GenAI **nu știe** fără context explicit — verificați înainte de upload pe placă.

- **LPSPI frame size implicit = 8 biți** — dacă device-ul cere 16 biți per frame (ex: MAX7219), setați `bytesPerFrame` corect
- CS hardware NXP și CS manual nu pot fi folosite simultan — dacă faceți CS manual (GPIO), dezactivați CS hardware în LPSPI config
- SPI Mode 0: CPOL=0, CPHA=0 — date capturate pe front rising. Verificați datasheet device-ului.

## ✅ Deliverable

> Loopback SPI verificat cu date identice + driver device extern funcțional + test care verifică datele trimise/primite

---

[← L5: LPADC — Conversie Analogică 12 biți](l5-lpadc.md) · [L7: LPI2C — Senzorul P3T1755 On-Board →](l7-lpi2c-p3t1755.md)
