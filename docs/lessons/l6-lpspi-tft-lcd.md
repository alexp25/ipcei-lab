---
title: "L6 - LPSPI - TFT LCD ILI9341"
description: "SPI Master, display TFT, de la demo static la vizualizator dinamic"
nav_order: 7
parent: Lecții FRDM-MCXA153
layout: lesson
---

# L6 - LPSPI - TFT LCD ILI9341

**SPI Master, display TFT, de la demo static la vizualizator dinamic**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 3, 24 iunie - după-amiază |
| **Periferic** | `LPSPI0 · GPIO CS/DC/RST/BL · Arduino Header` |
| **Durată** | 2h (13:00-15:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 · display TFT ILI9341 SPI · fire Dupont / shield compatibil |

## Context Și Motivație

SPI este unul dintre cele mai folosite protocoale pentru dispozitive externe rapide: display-uri, memorii flash, convertoare ADC/DAC, senzori IMU și drivere LED. În acest laborator folosim `LPSPI0` de pe FRDM-MCXA153 pentru a controla un display TFT cu controller ILI9341.

ILI9341 este un exemplu bun pentru SPI deoarece separă clar:

- magistrala SPI: `SCK`, `MOSI`, opțional `MISO`;
- selecția dispozitivului: `CS`;
- tipul transferului: `DC` pentru comandă sau date;
- reset și alimentare backlight: `RST`, `BL`.

La final, studenții pornesc de la un demo static pe LCD și îl transformă într-un screensaver dinamic: starfield cu mișcare înainte și o undă audio 2D în centru, controlată de o valoare de intrare simulată sau citită de la senzor.

> **Board:** FRDM-MCXA153 · MCX A153 Cortex-M33 @ 96 MHz · SDK MCUXpresso · VS Code + CMake

## Obiective

1. Configurarea `LPSPI0` în mod Master pentru un display SPI.
2. Înțelegerea semnalelor `CS`, `DC`, `RST` și `BL` pentru ILI9341.
3. Implementarea operațiilor de bază: comandă, date, fereastră de adresare, pixel/rect.
4. Afișarea unui demo static cu text, culori și linii.
5. Transformarea demo-ului static într-un vizualizator dinamic cu:
   - starfield full-screen;
   - undă 2D în centru;
   - temă de culoare schimbată cu `SW3`;
   - valoare de intrare rutabilă din `main()`.

## Conexiuni ILI9341

| Semnal LCD | Rol | FRDM-MCXA153 |
|---|---|---|
| `SCK` | clock SPI | pin LPSPI0 SCK pe header |
| `MOSI` | date MCU -> LCD | pin LPSPI0 MOSI pe header |
| `MISO` | date LCD -> MCU, opțional | pin LPSPI0 MISO dacă modulul îl expune |
| `CS` | selectare display, activ LOW | GPIO configurat în `board_ili9341.c` |
| `DC` | `0` comandă, `1` date | GPIO configurat în `board_ili9341.c` |
| `RST` | reset controller | GPIO configurat în `board_ili9341.c` |
| `BL` | backlight | GPIO configurat în `board_ili9341.c` |
| `VCC/GND` | alimentare | conform modulului LCD folosit |

Verificați întotdeauna tensiunea modulului LCD. Unele breakout-uri ILI9341 au regulator și level shifting, altele așteaptă semnale la 3.3 V.

### Rolul Fiecărui Pin

Un modul ILI9341 SPI are de obicei mai mulți pini decât o magistrală SPI minimă. Motivul este că SPI transportă doar octeți; liniile suplimentare îi spun controllerului LCD cum să interpreteze acei octeți.

| Pin | Direcție | Explicație |
|---|---|---|
| `SCK` / `CLK` | MCU -> LCD | Clock-ul SPI. Fiecare bit este transmis sincronizat cu acest semnal. |
| `MOSI` / `SDI` / `DIN` | MCU -> LCD | Datele trimise de microcontroller către display. Pentru acest laborator este linia principală de date. |
| `MISO` / `SDO` | LCD -> MCU | Opțional. Unele module permit citirea ID-ului sau a memoriei GRAM, dar multe breakout-uri nu leagă util acest pin. |
| `CS` / `T_CS` | MCU -> LCD | Chip select. Când este LOW, LCD-ul ascultă magistrala SPI. Când este HIGH, ignoră transferurile. |
| `DC` / `RS` / `A0` | MCU -> LCD | Data/Command. `0` înseamnă comandă ILI9341, `1` înseamnă date pentru comanda curentă. |
| `RST` / `RESET` | MCU -> LCD | Reset hardware pentru controllerul LCD. Este util la pornire pentru o stare cunoscută. |
| `BL` / `LED` | MCU -> LCD sau alimentare | Backlight. Dacă acesta este oprit, controllerul poate funcționa, dar ecranul pare negru. |
| `VCC` | alimentare | Verificați modulul: unele acceptă 5 V pe VCC, dar semnalele logice trebuie să rămână 3.3 V. |
| `GND` | alimentare | Masă comună între FRDM-MCXA153 și display. Fără GND comun, SPI nu are referință electrică. |

Pentru primele teste, conectați doar funcționalitatea de bază: alimentare, `GND`, `SCK`, `MOSI`, `CS`, `DC`, `RST`, `BL`. `MISO` poate rămâne neconectat dacă driverul nu citește din LCD.

### Checklist De Cablare

Înainte de a depana codul, verificați fizic:

1. `GND` LCD este legat la `GND` FRDM-MCXA153.
2. Backlight-ul este alimentat sau controlat de pinul `BL`.
3. `SCK` și `MOSI` sunt pe pinii configurați pentru `LPSPI0`, nu pe alt header SPI.
4. `CS`, `DC`, `RST` și `BL` corespund cu definițiile din `board_ili9341.c`.
5. Modulul LCD acceptă semnale logice de 3.3 V.
6. Firele sunt scurte și bine fixate; SPI la frecvențe mari este sensibil la conexiuni slabe.

## Ce Trebuie Înțeles Despre SPI

SPI este sincron: masterul generează clock-ul, iar datele se deplasează pe fiecare front de clock. Pentru ILI9341, în practică folosim mai ales scriere:

1. `CS = 0` selectează LCD-ul.
2. `DC = 0` înainte de un byte de comandă.
3. `DC = 1` înainte de payload-ul comenzii.
4. `CS = 1` încheie tranzacția.

Exemple de comenzi ILI9341:

- `0x01` - software reset;
- `0x11` - sleep out;
- `0x29` - display on;
- `0x2A` - column address set;
- `0x2B` - page address set;
- `0x2C` - memory write.

Pentru desenare eficientă nu trimitem fiecare pixel independent. Setăm o fereastră dreptunghiulară cu `0x2A` / `0x2B`, apoi trimitem rapid pixeli RGB565 cu `0x2C`.

### SPI Pe Scurt

SPI este o magistrală de tip master-slave. În acest laborator, FRDM-MCXA153 este master, iar ILI9341 este slave.

Un transfer SPI are patru idei importante:

- **Clock (`SCK`)**: masterul decide viteza transferului.
- **Date către slave (`MOSI`)**: fiecare bit trimis spre LCD apare pe această linie.
- **Date către master (`MISO`)**: folosit doar dacă slave-ul trimite date înapoi.
- **Selectare (`CS`)**: permite folosirea mai multor dispozitive SPI pe aceeași magistrală.

SPI este full-duplex la nivel electric: în timp ce masterul trimite un bit pe `MOSI`, poate primi simultan un bit pe `MISO`. În cazul display-ului, de obicei folosim magistrala aproape ca write-only: trimitem comenzi și pixeli, iar citirea nu este necesară pentru animație.

### Mode 0: CPOL Și CPHA

Dispozitivele SPI pot interpreta clock-ul în moduri diferite. Modul este definit de:

- `CPOL` - nivelul clock-ului când magistrala este inactivă;
- `CPHA` - frontul pe care datele sunt capturate.

Pentru ILI9341 se folosește frecvent **SPI Mode 0**:

| Parametru | Valoare | Semnificație |
|---|---|---|
| `CPOL` | `0` | `SCK` stă LOW când nu se transferă date |
| `CPHA` | `0` | datele sunt capturate pe primul front, de obicei front crescător |

Dacă modul SPI este greșit, display-ul poate rămâne alb/negru sau poate afișa pixeli aleatori, deși pinii sunt conectați corect.

### CS Hardware Vs CS Manual

Unele periferice SPI pot controla automat `CS`. Pentru display-uri este adesea mai simplu să controlăm `CS` manual prin GPIO:

```c
CS_LOW();
write_command(0x2C);
write_data(pixel_buffer, length);
CS_HIGH();
```

Controlul manual este util pentru că unele operații ILI9341 combină o comandă scurtă cu un payload mare. Vrem să păstrăm `CS` activ pe durata întregii tranzacții logice.

### Comandă Sau Date: De Ce Există `DC`

SPI trimite doar octeți. ILI9341 trebuie să știe dacă un octet este:

- o comandă, de exemplu `0x2C` pentru `Memory Write`;
- date pentru comanda precedentă, de exemplu pixeli RGB565.

Această diferență este făcută de pinul `DC`:

```c
DC_LOW();      // următorul byte este comandă
spi_write(0x2C);

DC_HIGH();     // următorii bytes sunt date
spi_write(pixel_data, pixel_count * 2);
```

O greșeală foarte comună este ca `DC` să fie inversat sau legat la pinul greșit. În acest caz, LCD-ul primește date valide electric, dar le interpretează greșit.

## Cum Funcționează ILI9341

ILI9341 este controllerul dintre microcontroller și matricea de pixeli a LCD-ului. El conține registre de configurare și o memorie grafică internă, numită de obicei GRAM. Programul nu controlează direct fiecare tranzistor din display; trimite comenzi către controller.

Secvența tipică de pornire este:

1. `RST` este tras LOW, apoi HIGH, pentru reset hardware.
2. Se trimite `Software Reset` (`0x01`) și se așteaptă stabilizarea.
3. Se trimite `Sleep Out` (`0x11`).
4. Se configurează formatul pixelilor, de obicei RGB565.
5. Se configurează orientarea ecranului cu registrul `MADCTL`.
6. Se trimite `Display On` (`0x29`).
7. Se poate începe desenarea cu `Column Address Set`, `Page Address Set`, `Memory Write`.

### Pixeli RGB565

În laborator folosim formatul RGB565: fiecare pixel are 16 biți:

| Componentă | Biți | Interval |
|---|---|---|
| Roșu | 5 biți | `0..31` |
| Verde | 6 biți | `0..63` |
| Albastru | 5 biți | `0..31` |

Exemple:

```c
#define ILI9341_COLOR_BLACK   0x0000U
#define ILI9341_COLOR_RED     0xF800U
#define ILI9341_COLOR_GREEN   0x07E0U
#define ILI9341_COLOR_BLUE    0x001FU
#define ILI9341_COLOR_WHITE   0xFFFFU
```

Un pixel se trimite ca doi octeți: mai întâi byte-ul superior, apoi byte-ul inferior.

### Fereastra De Adresare

Pentru a desena rapid, alegem întâi zona în care vrem să scriem:

1. `0x2A` setează intervalul de coloane `x0..x1`.
2. `0x2B` setează intervalul de pagini/linii `y0..y1`.
3. `0x2C` începe scrierea pixelilor în acea zonă.

De exemplu, `fill_rect(x, y, w, h, color)` nu trebuie să mute cursorul pentru fiecare pixel. Setează o singură fereastră dreptunghiulară și trimite `w * h` pixeli consecutivi. Aceasta este mult mai rapid decât apeluri repetate la `draw_pixel()`.

### De Ce Animația Nu Trebuie Să Redesenze Totul Mereu

Un display 320x240 are 76.800 pixeli. În RGB565, un frame complet are:

```text
76.800 pixeli * 2 bytes = 153.600 bytes
```

La SPI, fiecare byte consumă timp pe magistrală. Pentru animații fluide este mai eficient să:

- ștergem doar obiectele care s-au mișcat;
- redesenăm doar liniile/stelele/unda care se schimbă;
- folosim `fill_rect()` pentru zone mici;
- păstrăm formele simple.

În task-ul acestui laborator, starfield-ul și unda 2D sunt potrivite pentru embedded deoarece pot fi desenate incremental, fără framebuffer complet în RAM.

## Planul Sesiunii

| Interval | Activitate | Detaliu |
|---|---|---|
| `13:00-13:25` | SPI + ILI9341 | Mode 0, `CS`, `DC`, reset, RGB565, address window |
| `13:25-13:55` | Demo static | Inițializare LCD, text, dreptunghiuri colorate, linii |
| `13:55-14:30` | Driver drawing API | `fill_rect`, `fill_screen`, `draw_line`, `write_string` |
| `14:30-15:00` | Task: vizualizator | Transformare demo static în starfield + undă audio 2D |

## Structura Codului

Fișierele relevante din proiect:

- `board_ili9341.c/.h` - maparea pinilor LCD și inițializarea GPIO pentru `CS`, `DC`, `RST`, `BL`;
- `ili9341.c/.h` - driverul low-level ILI9341;
- `ili9341_demo.c/.h` - logica demo-ului și animației;
- `main.c` - inițializarea aplicației și bucla principală.

API-ul recomandat pentru laborator:

```c
status_t ILI9341_DemoInit(void);
void ILI9341_DemoUpdate(uint16_t input_value);
```

`input_value` este în intervalul `0..4095`, aceeași scală ca un ADC pe 12 biți. În varianta inițială poate fi o valoare simulată. Ulterior poate veni de la:

- potențiometru prin LPADC;
- senzor analogic;
- envelope de microfon;
- valoare calculată dintr-un senzor digital.

În `main.c`, ruta de integrare trebuie să rămână simplă:

```c
status_t status = ILI9341_DemoInit();

while (1)
{
    uint16_t input = read_sensor_or_generate_demo_value();
    ILI9341_DemoUpdate(input);
}
```

## Task De Laborator

Pornind de la un demo static pe ILI9341, realizați un screensaver dinamic:

1. Păstrați inițializarea LCD și funcțiile de desenare existente.
2. Înlocuiți ecranul static cu o animație full-screen.
3. Implementați un starfield care dă impresia de mișcare înainte.
4. Desenați în centru o undă audio 2D.
5. Legați amplitudinea undei de `input_value`.
6. Folosiți `SW3` pentru schimbarea temei de culoare.
7. Lăsați `main.c` pregătit pentru înlocuirea valorii simulate cu o citire reală de senzor.

### Pași Recomandați Pentru Implementare

1. **Verificați demo-ul static.** Ecranul trebuie să afișeze text, blocuri colorate sau linii. Dacă demo-ul static nu merge, nu începeți animația.
2. **Separați inițializarea de randare.** Inițializarea LCD se face o singură dată, randarea se face repetat.
3. **Introduceți `ILI9341_DemoUpdate(input_value)`.** Această funcție primește valoarea care va controla animația.
4. **Înlocuiți conținutul static.** În loc de text fix, desenați stele care se deplasează radial din centru spre margini.
5. **Adăugați unda 2D.** Calculați puncte pe axa X, iar Y-ul lor depinde de `input_value`.
6. **Optimizați redesenarea.** Ștergeți pozițiile vechi ale stelelor și ale undei, apoi desenați pozițiile noi.
7. **Testați cu valori simulate.** Abia după ce animația merge, înlocuiți simularea cu LPADC sau alt senzor.

Exemplu de valoare simulată:

```c
uint16_t screensaver_value =
    (uint16_t)((((frame * 29U) & 0x0FFFU) + (((frame * 113U) + 900U) & 0x0FFFU)) / 2U);
```

Exemplu de integrare ulterioară cu un senzor:

```c
uint16_t sensor_value = LPADC_Read12BitValue();
ILI9341_DemoUpdate(sensor_value);
```

### Criterii De Reușită

La final, proiectul este considerat funcțional dacă:

- display-ul pornește fără ecran alb/negru permanent;
- se observă clar stelele care se deplasează spre exterior;
- unda 2D se află în zona centrală a ecranului;
- amplitudinea undei se schimbă atunci când se schimbă `input_value`;
- `SW3` schimbă paleta de culori;
- codul permite înlocuirea ușoară a valorii simulate cu o citire reală.

## Debug Hardware Și Software

Dacă LCD-ul nu afișează nimic, verificați în această ordine:

| Simptom | Cauză probabilă | Verificare |
|---|---|---|
| Ecran complet negru | backlight oprit sau alimentare lipsă | verificați `BL`, `VCC`, `GND` |
| Ecran alb | controller neinițializat sau `RST` greșit | verificați secvența de reset și pinul `RST` |
| Pixeli aleatori | SPI mode greșit sau fire instabile | verificați CPOL/CPHA, `SCK`, `MOSI`, GND comun |
| Culori greșite | ordine RGB/BGR sau RGB565 greșit | verificați `MADCTL` și conversia culorilor |
| Imagine rotită/inversată | orientare `MADCTL` | ajustați `ili9341_set_rotation()` |
| Textul apare, animația nu | bucla principală nu cheamă update | verificați `ILI9341_DemoUpdate()` în `while (1)` |
| Animație foarte lentă | prea mulți pixeli redesenați | reduceți suprafața desenată sau baudrate-ul SPI |

Pentru depanare, începeți cu semnale simple:

1. Aprindeți backlight-ul.
2. Faceți `fill_screen(RED)`.
3. Faceți `fill_screen(GREEN)`.
4. Faceți `fill_screen(BLUE)`.
5. Desenați o linie diagonală.
6. Abia apoi porniți animația.

Această secvență izolează problema: dacă `fill_screen()` merge, SPI și inițializarea LCD sunt în mare parte corecte.

## Prompt-uri Pentru Asistentul AI

> Copiați promptul complet. Pentru embedded, contextul hardware este obligatoriu.

### Prompt: Driver ILI9341 peste LPSPI

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33, SDK MCUXpresso.
Display: ILI9341 TFT SPI, RGB565.
SPI: LPSPI0 Master, Mode 0, transmitere MSB first.
GPIO separate: CS, DC, RST, BL.
Sarcina:
  - init ILI9341
  - write_command()
  - write_data()
  - set_address_window()
  - fill_rect()
  - fill_screen()
  - draw_line()
Nu folosi Arduino API. Folosește MCUXpresso SDK: fsl_lpspi, fsl_gpio.
```

### Prompt: De La Demo Static La Vizualizator

```text
Am un driver ILI9341 funcțional pe FRDM-MCXA153.
Există funcții: fill_screen, fill_rect, draw_line, write_string.
Vreau să transform demo-ul static într-un screensaver:
  - starfield full-screen, ca mișcare înainte prin spațiu;
  - undă audio 2D pe centrul ecranului;
  - amplitudinea undei controlată de input_value 0..4095;
  - SW3 schimbă tema de culoare;
  - API:
      status_t ILI9341_DemoInit(void);
      void ILI9341_DemoUpdate(uint16_t input_value);
Cod C pentru embedded, fără malloc, fără float, fără biblioteci grafice externe.
```

## Capcane Critice

- Nu confundați `DC` cu `CS`: `DC` alege comandă/date, `CS` selectează dispozitivul.
- ILI9341 folosește RGB565: 16 biți per pixel, nu RGB888.
- Pentru performanță, desenați dreptunghiuri/linii sau ferestre de pixeli, nu `draw_pixel()` pentru tot ecranul.
- Dacă ecranul rămâne negru, verificați în ordine: alimentare, backlight, reset, `CS`, `DC`, pin mux SPI, baudrate SPI.
- Dacă animația pâlpâie prea tare, reduceți suprafața redesenată sau creșteți delay-ul din bucla principală.

## Deliverable

> Proiect care pornește de la demo static ILI9341 și ajunge la un vizualizator dinamic full-screen: starfield + undă 2D, cu `SW3` pentru culori și `ILI9341_DemoUpdate(input_value)` pregătit pentru citire de senzor.

---

[← L5: LPADC - Conversie Analogică 12 biți](../l5-lpadc) · [L7: LPI2C - Senzorul P3T1755 On-Board →](../l7-lpi2c-p3t1755)
