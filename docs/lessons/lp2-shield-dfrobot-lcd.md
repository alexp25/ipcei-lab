---
title: "LP2 — Shield DFRobot RGB LCD + Keypad"
description: "Integrare shield DFRobot RGBLCD1602 pe FRDM-MCXA153: LPI2C0 pentru LCD si LPADC0 pentru butoane"
nav_order: 2
parent: Demo-uri suplimentare
layout: lesson
---

# LP2 - Shield DFRobot RGB LCD + Keypad

**LCD 16x2 pe I2C, backlight RGB si butoane citite printr-o scara rezistiva pe ADC**

---

| | |
|---|---|
| **Periferice** | `LPI2C0`, `LPADC0`, `GPIO/pin mux`, consola seriala |
| **Durata** | 2h |
| **Hardware** | FRDM-MCXA153 + DFRobot RGB LCD Keypad Shield / RGBLCD1602 compatibil |
| **Proiect** | `src/lab_shield_lcd_keypad/main` |
| **Rezultat** | LCD-ul afiseaza text si butonul apasat: RIGHT, UP, DOWN, LEFT, SELECT sau NONE |

## Context

Acest laborator adauga un shield Arduino-style la FRDM-MCXA153. Shield-ul are doua parti diferite:

1. LCD 16x2 HD44780-compatible controlat printr-un expander I2C DFRobot RGBLCD1602.
2. Cinci butoane conectate la o scara rezistiva citita pe Arduino `A0`.

Partea importanta este ca shield-ul foloseste conectorii Arduino ai placii FRDM, nu pinii mikroBUS/Pmod. In acest proiect, LCD-ul comunica pe `LPI2C0`, iar butoanele sunt citite cu `LPADC0`.

## Obiective

La final, studentul trebuie sa poata:

1. Identifice pinii corecti ai headerului Arduino pe FRDM-MCXA153.
2. Configureze `LPI2C0` pentru LCD-ul DFRobot RGBLCD1602.
3. Configureze `ADC0_A8` pentru butoanele de pe shield.
4. Integreze fisierele generate de MCUXpresso Config Tools cu proiectul CMake.
5. Afiseze pe LCD si in consola butonul apasat.
6. Calibreze praguri ADC folosind valori raw masurate.

## Hardware si pinout

Shield-ul se monteaza pe headerul Arduino al placii FRDM-MCXA153.

| Functie shield | Header Arduino | Pin MCXA153 | Periferic |
|---|---:|---|---|
| SDA LCD | SDA | `P1_8` | `LPI2C0_SDA` |
| SCL LCD | SCL | `P1_9` | `LPI2C0_SCL` |
| Butoane | A0 | `P1_10` | `ADC0_A8` |
| Alimentare | 3.3 V / 5 V, in functie de modul | - | verificati nivelurile |
| GND | GND | - | masa comuna |

Adresele I2C folosite de modulul DFRobot sunt:

```text
LCD controller: 0x3E
RGB controller: 0x60
```

> Nota: unele documentatii Arduino spun ca SDA/SCL sunt `A4/A5`. Pe Arduino Uno, pinii SDA/SCL sunt legati si la zona A4/A5, dar pe FRDM-MCXA153 trebuie folositi pinii SDA/SCL ai headerului Arduino, adica `P1_8` si `P1_9`.

## Nota electrica

FRDM-MCXA153 foloseste niveluri logice de 3.3 V. Inainte de conectarea shield-ului:

- verificati daca pull-up-urile I2C ale modulului sunt la 3.3 V sau 5 V;
- nu aplicati 5 V direct pe intrari GPIO/ADC ale MCXA153;
- daca shield-ul forteaza I2C la 5 V, folositi level shifter;
- pentru ADC A0, tensiunea trebuie sa ramana in domeniul acceptat de placa, raportat la VDDA.

In testul curent, LCD-ul functioneaza pe SDA/SCL, iar butoanele sunt citite raw prin LPADC.

## Structura codului

Fisierele relevante din proiect sunt:

```text
src/lab_shield_lcd_keypad/main/
  main.c
  dfrobot_rgb_lcd.c/.h
  dfrobot_lcd_shield.c/.h
  dfrobot_lcd_shield_keypad.c/.h
  cfg_tools/board/pin_mux.c/.h
  main.mex
```

Rolurile sunt separate astfel:

| Fisier | Rol |
|---|---|
| `dfrobot_rgb_lcd.c/.h` | Driver pentru protocolul DFRobot RGBLCD1602: init LCD, clear, cursor, text, RGB backlight |
| `dfrobot_lcd_shield.c/.h` | Legatura dintre driverul LCD si placa FRDM: init `LPI2C0`, write I2C, scan adrese |
| `dfrobot_lcd_shield_keypad.c/.h` | Init `LPADC0`, citire `ADC0_A8`, debounce si mapare raw ADC la butoane |
| `main.c` | Demo: initializeaza LCD-ul, ruleaza scan I2C, afiseaza butonul curent |
| `main.mex` | Configuratia MCUXpresso Config Tools pentru pini |

## Configurare in MCUXpresso Config Tools

In `main.mex`, verificati ca Pins Tool contine:

```text
P1_8  -> LPI2C0_SDA  -> RGBLCD1602_ARDUINO_SDA
P1_9  -> LPI2C0_SCL  -> RGBLCD1602_ARDUINO_SCL
P1_10 -> ADC0_A8     -> LCD_KEYPAD_ARDUINO_A0
```

In fisierele generate trebuie sa fie actualizate aceleasi semnale:

```text
cfg_tools/board/pin_mux.c
cfg_tools/board/pin_mux.h
```

Daca tool-ul genereaza fisiere in alt director, proiectul trebuie realiniat astfel incat CMake sa includa fisierele din `cfg_tools/board`. In acest lab, `CMakeLists.txt` include explicit si fisierele de suport cerute de tool:

```text
cfg_tools/board/RTE_Device.h
cfg_tools/cfg_require.json
```
Daca dupa `Update Code` apar schimbari propuse pentru `pin_mux.h`, `clock_config.c/.h` sau `peripherals.c/.h`, verificati intai daca sunt schimbari generate legitim din `main.mex`. In varianta functionala a acestui lab, Config Tools genereaza si init pentru `LPI2C0` si `ADC0`, iar `BOARD_InitHardware()` apeleaza `BOARD_InitBootPeripherals()` dupa pini si ceasuri.

Un detaliu important descoperit in timpul integrarii: fisierul `cfg_tools/cfg_require.json` poate fi rescris de tool si poate pierde dependintele necesare. Daca apar erori de tipul "LPI2C Driver is not found", "LPADC Driver is not found" sau "LPUART CMSIS Driver is not found", verificati ca `prj.conf`, `frdmmcxa153/prj.conf`, `cfg_tools/project_info.json` si `cfg_tools/cfg_require.json` contin aceleasi componente folosite de proiect:

```text
CONFIG_MCUX_COMPONENT_driver.cmsis_lpuart=y
CONFIG_MCUX_COMPONENT_driver.lpi2c=y
CONFIG_MCUX_COMPONENT_driver.lpadc=y
CONFIG_MCUX_COMPONENT_device.RTE=y
```

De asemenea, `cfg_require.json` si `cfg_tools/board/RTE_Device.h` trebuie sa fie listate ca fisiere ale proiectului. Altfel Config Tools poate raporta "change, not in the project", chiar daca build-ul CMake inca poate compila.

Pentru ceasul I2C, pastrati aceeasi sursa in cod si in Config Tools. Configuratia finala foloseste `FRO12M` pentru `LPI2C0`; driverul de shield nu trebuie sa mute LPI2C0 pe alta sursa de ceas dupa initializarea generata.

## LCD pe I2C

Driverul `dfrobot_rgb_lcd` implementeaza secventa compatibila cu biblioteca Arduino `DFRobot_RGBLCD1602`:

1. initializeaza LCD-ul la adresa `0x3E`;
2. initializeaza controllerul RGB la adresa `0x60`;
3. trimite text pe cele doua randuri;
4. modifica backlight-ul RGB.

In `main.c`, demo-ul afiseaza:

```text
hello, world!
Key: NONE
```

Apoi linia a doua este actualizata cu butonul apasat.

## Keypad pe ADC raw

Butoanele shield-ului sunt conectate printr-o scara rezistiva la Arduino `A0`. Pe FRDM-MCXA153, `A0` este `P1_10 / ADC0_A8`.

Implementarea citeste direct valoarea raw LPADC pe 16 biti. Pentru placa si shield-ul testate au fost masurate aceste valori:

| Buton real | Valoare raw aproximativa |
|---|---:|
| RIGHT | `15` |
| UP | `19500` |
| DOWN | `38300` |
| LEFT | `58500` |
| SELECT | de masurat pe placa |
| NONE | aproape de capatul superior / open circuit |

Pragurile curente sunt alese la mijloc intre valorile masurate:

```c
.right_max  = 9750U,
.up_max     = 28900U,
.down_max   = 48400U,
.left_max   = 62000U,
.select_max = 65000U,
```

Astfel, decodarea este:

```text
raw <= 9750   -> RIGHT
raw <= 28900  -> UP
raw <= 48400  -> DOWN
raw <= 62000  -> LEFT
raw <= 65000  -> SELECT
raw >  65000  -> NONE
```

Daca SELECT nu este detectat corect, cititi valoarea raw din consola seriala si ajustati `select_max` in `dfrobot_lcd_shield_keypad.c`.

## Debounce

Citirea ADC poate oscila usor. De aceea driverul nu accepta imediat orice schimbare. El foloseste un filtru simplu:

1. citeste valoarea raw;
2. decodeaza butonul candidat;
3. cere acelasi candidat timp de `3` citiri consecutive;
4. abia apoi actualizeaza butonul stabil.

Aceasta abordare este suficienta pentru un demo in super-loop si evita afisari rapide gresite.

## Build si rulare

Din directorul proiectului:

```powershell
cd C:\WORKSPACE\proiecte\ipcei-lab\src\lab_shield_lcd_keypad\main
cmake --build --preset debug
```

Dupa flash, deschideti consola seriala. Ar trebui sa vedeti mesaje similare:

```text
FRDM-MCXA153 DFRobot RGBLCD1602 I2C demo
Using LPI2C0 on Arduino header SDA=P1_8 and SCL=P1_9
LCD address 0x3E, RGB address 0x60
I2C scan: 0x3E 0x60
RGBLCD1602 init sequence sent
ADC raw=... key=NONE
Button: RIGHT raw=15
```

Pe LCD trebuie sa apara `hello, world!` si linia `Key: ...`.

## Exercitiu de laborator

1. Deschideti `main.mex` si verificati pinii `P1_8`, `P1_9`, `P1_10`.
2. Comparati `main.mex` cu `cfg_tools/board/pin_mux.c`.
3. Rulati build-ul.
4. Flash-uiti aplicatia pe FRDM-MCXA153.
5. Confirmati cu scanarea I2C ca apar adresele `0x3E` si `0x60`.
6. Apasati fiecare buton si notati valorile raw din consola.
7. Ajustati pragul pentru SELECT daca este necesar.
8. Modificati mesajul de pe LCD pentru a afisa initialele echipei.

## Probleme frecvente

| Simptom | Cauza probabila | Verificare / remediere |
|---|---|---|
| Scanarea I2C listeaza toate adresele | SDA/SCL gresite sau linii flotante | Folositi `P1_8/P1_9`, nu `P3_28/P3_27` |
| LCD-ul nu se aprinde | alimentare lipsa sau modul incompatibil | verificati VCC/GND si nivelurile I2C |
| LCD-ul se aprinde dar nu afiseaza text | init I2C nu ajunge la modul sau contrast gresit | verificati scanarea `0x3E/0x60`; ajustati contrastul daca exista |
| Butoanele sunt decalate | praguri ADC nepotrivite | folositi valorile raw din consola si recalculati pragurile |
| SELECT apare ca NONE | pragul `select_max` este prea mic sau SELECT este aproape de open-circuit | masurati raw SELECT si actualizati `select_max` |
| Config Tool creeaza fisiere in alt folder | `project_link` / metadata nealiniate | verificati `main.mex`, `cfg_tools/project_info.json`, `cfg_tools/cfg_require.json` |
| Config Tool raporteaza drivere lipsa sau `cfg_require.json` ca "not in project" | metadata proiectului a fost rescrisa incomplet | restaurati componentele `cmsis_lpuart`, `lpi2c`, `lpadc`, `device.RTE` in `prj.conf`/`cfg_require.json`/`project_info.json` si includeti `cfg_require.json` in CMake |

## Ce trebuie predat

1. Proiectul compileaza fara erori.
2. LCD-ul afiseaza text pe ambele randuri.
3. Backlight-ul RGB se modifica.
4. Consola seriala arata scanarea I2C si valorile raw ADC.
5. Fiecare buton este detectat corect sau pragurile sunt documentate.
6. `main.mex` contine pinii folositi de shield.

## Prompt util pentru asistent AI

```text
Lucrez pe FRDM-MCXA153 cu un DFRobot RGB LCD Keypad Shield.
LCD-ul este pe I2C: SDA=P1_8, SCL=P1_9, LPI2C0, adrese 0x3E si 0x60.
Butoanele sunt pe Arduino A0 = P1_10 / ADC0_A8, citite cu LPADC raw 16-bit.
Valorile masurate sunt RIGHT=15, UP=19500, DOWN=38300, LEFT=58500, SELECT inca de masurat.
Te rog sa verifici driverul dfrobot_lcd_shield_keypad.c si sa ajustezi pragurile fara sa modifici partea de LCD.
Pastreaza compatibilitatea cu MCUXpresso Config Tools si main.mex.
```

---

[<- L5: LPADC](../l5-lpadc) · [L7: LPI2C - P3T1755](../l7-lpi2c-p3t1755)