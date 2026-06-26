---
title: "LP3 - USB HID Mouse Jiggler"
description: "Adaptarea exemplului SDK usb_device_hid_mouse_lite intr-un dispozitiv USB HID mouse jiggler"
nav_order: 3
parent: Demo-uri suplimentare
layout: lesson
---

# LP3 - USB HID Mouse Jiggler

**De la exemplu SDK USB HID mouse la dispozitiv mouse jiggler controlat cu SW3**

---

| | |
|---|---|
| **Periferice** | `USB0`, `USB HID`, `GPIO`, `PORT` |
| **Durata** | 2h |
| **Hardware** | FRDM-MCXA153 - USB device port - SW3 - LED rosu |
| **Proiect sursa SDK** | `src/sdks/mcuxsdk/examples/usb_examples/usb_device_hid_mouse_lite` |
| **Proiect local** | `src/lab_usb_hid/usb_device_hid_mouse_lite/bm` |
| **Rezultat** | Placa se enumera ca mouse HID si misca periodic cursorul cu un model de jiggler; SW3 activeaza/dezactiveaza miscarea; LED-ul indica starea activa |

## Context

Acest laborator porneste de la exemplul NXP MCUXpresso SDK `usb_device_hid_mouse_lite`. Exemplul original se enumera ca mouse USB HID si trimite rapoarte relative care deplaseaza cursorul intr-un traseu dreptunghiular.

Sarcina este sa folositi un asistent GenAI ca partener de refactorizare si verificare pentru a transforma exemplul intr-un **mouse jiggler** simplu:

- proiectul local trebuie sa compileze fisierele din folderul laboratorului, nu fisierele din SDK;
- miscarea cursorului trebuie sa fie diferita de dreptunghiul original;
- jiggler-ul trebuie sa poata fi activat/dezactivat din SW3;
- LED-ul trebuie sa fie aprins cand jiggler-ul este activ si stins cand este dezactivat;
- documentatia scurta a codului trebuie generata cu ajutor GenAI.

> Nota: proiectul SDK sample nu contine fisier `.mex`. Pentru acest laborator, configurarea se face in cod si in `CMakeLists.txt`, fara suport MCUXpresso Config Tools.

## Obiective

La final, studentul trebuie sa poata:

1. Copieze/adapteze un exemplu SDK USB HID intr-un proiect local.
2. Modifice generatorul de rapoarte HID mouse pentru o miscare de tip jiggler.
3. Configureze `CMakeLists.txt` astfel incat build-ul sa foloseasca sursele locale.
4. Compare proiectul rezultat cu exemplul SDK folosind un instrument GenAI.
5. Verifice practic ca miscarea nu mai este traseul dreptunghiular din exemplul original.
6. Implementeze toggle cu SW3 si indicator LED pentru starea jiggler-ului.
7. Foloseasca GenAI pentru explicarea codului si pentru generarea unei documentatii simple.

## Cerinte functionale

Implementarea finala trebuie sa respecte urmatoarele cerinte:

1. Dispozitivul USB este recunoscut de host ca mouse HID.
2. Cursorul se misca relativ fata de pozitia curenta.
3. Miscarea ceruta este de tip jiggler: deplasari de `8` pixeli spre stanga/dreapta/sus/jos fata de pozitia curenta.
4. Miscarea nu trebuie sa fie dreptunghiul continuu din exemplul SDK original.
5. SW3 comuta intre modurile:
   - jiggler activ;
   - jiggler oprit.
6. LED-ul este aprins cand jiggler-ul este activ.
7. LED-ul este stins cand jiggler-ul este oprit.

## Structura recomandata a proiectului

Punctul de plecare este exemplul SDK:

```text
src/sdks/mcuxsdk/examples/usb_examples/usb_device_hid_mouse_lite
```

Proiectul local pentru laborator trebuie sa fie separat de SDK, de exemplu:

```text
src/lab_usb_hid/usb_device_hid_mouse_lite/bm/
  CMakeLists.txt
  CMakePresets.json
  prj.conf
  mouse.c
  mouse.h
  usb_device_ch9.c/.h
  usb_device_descriptor.c/.h
  usb_device_hid.c/.h
```

## Import SDK project doar pentru testare

Puteti importa temporar exemplul SDK in VS Code / MCUXpresso pentru a verifica rapid ca toolchain-ul, placa si exemplul original functioneaza. Imaginea de mai jos arata importul exemplului `usb_device_hid_mouse_lite` din SDK:

![Import SDK USB HID mouse project](../img/labp2/import_sdk_project_hid_usb.png)

Aceasta varianta este utila **doar pentru testare si inspectie**. Pentru laborator este mai bine sa copiati proiectul SDK intr-un folder local al laboratorului si apoi sa modificati configuratia proiectului astfel incat build-ul sa pointeze la fisierele locale, nu la fisierele din SDK.

Motivul este simplu: daca `CMakeLists.txt` ramane legat de `${SdkRootDirPath}/examples/...`, puteti edita `mouse.c` din proiectul local fara ca firmware-ul compilat sa se schimbe. In practica, asta duce la simptome confuze: cursorul continua sa se miste in dreptunghi, desi codul local pare modificat corect.
## Atentie la CMakeLists.txt

Un pas esential este actualizarea fisierului `CMakeLists.txt`. Multe exemple SDK au sursele listate relativ la `${SdkRootDirPath}`:

```cmake
mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/usb_examples/usb_device_hid_mouse_lite/bm/mouse.c"
)
```

Pentru laborator, build-ul trebuie sa foloseasca fisierele locale:

```cmake
mcux_add_include(
  BASE_PATH "${CMAKE_CURRENT_LIST_DIR}"
  INCLUDES "."
)

mcux_add_source(
  BASE_PATH "${CMAKE_CURRENT_LIST_DIR}"
  SOURCES
    "mouse.c"
    "mouse.h"
    "usb_device_ch9.c"
    "usb_device_ch9.h"
    "usb_device_descriptor.c"
    "usb_device_descriptor.h"
    "usb_device_hid.c"
    "usb_device_hid.h"
)
```

Daca acest pas este omis, puteti modifica fisierul local `mouse.c` fara ca firmware-ul flash-uit sa se schimbe, deoarece build-ul continua sa compileze exemplul din SDK.

## Task GenAI

Folositi un instrument GenAI pentru urmatoarea sarcina:

```text
Am un proiect MCUXpresso SDK pentru FRDM-MCXA153 pornit de la:
src/sdks/mcuxsdk/examples/usb_examples/usb_device_hid_mouse_lite

Vreau sa transform exemplul intr-un USB HID mouse jiggler.
Cerintele sunt:
- proiectul local trebuie sa compileze sursele din folderul laboratorului, nu din SDK;
- actualizeaza CMakeLists.txt pentru surse locale;
- modifica rapoartele HID mouse astfel incat cursorul sa se miste 8 pixeli stanga/dreapta/sus/jos relativ la pozitia curenta;
- miscarea nu trebuie sa fie traseul dreptunghiular din exemplul original;
- SW3 activeaza/dezactiveaza jiggler-ul;
- LED-ul rosu este aprins cand jiggler-ul este activ si stins cand este dezactivat;
- proiectul nu are fisier .mex, deci configurarea pinilor se face in cod.

Te rog sa explici fisierele care trebuie modificate si sa propui un patch minimal.
```

## Implementare: raport HID pentru jiggler

Raportul mouse HID folosit de exemplu are 4 octeti:

| Byte | Semnificatie |
|---:|---|
| `0` | butoane mouse |
| `1` | delta X relativ |
| `2` | delta Y relativ |
| `3` | wheel |

Pentru o miscare stanga/dreapta/sus/jos cu pas de 8 pixeli, puteti folosi o secventa de pasi:

```c
static const int8_t s_JigglePattern[][2] = {
    { 8,  0},
    {-8,  0},
    { 0,  8},
    { 0, -8},
};
```

La fiecare raport HID trimis catre host, selectati urmatorul pas din secventa:

```c
g_UsbDeviceHidMouse.buffer[0] = 0U;
g_UsbDeviceHidMouse.buffer[1] = (uint8_t)s_JigglePattern[index][0];
g_UsbDeviceHidMouse.buffer[2] = (uint8_t)s_JigglePattern[index][1];
g_UsbDeviceHidMouse.buffer[3] = 0U;
```

Daca jiggler-ul este dezactivat, trimiteti un raport neutru:

```c
g_UsbDeviceHidMouse.buffer[1] = 0U;
g_UsbDeviceHidMouse.buffer[2] = 0U;
```

## Implementare: SW3 toggle

Pe FRDM-MCXA153, SW3 este definit in `board.h` ca:

```c
BOARD_SW3_GPIO
BOARD_SW3_GPIO_PIN
```

In proiectele curente, SW3 este `GPIO1`, pin `7` (`P1_7`) si este citit ca buton activ-low. Pentru o solutie simpla, folositi polling cu debounce software in bucla principala.

Exemplu de strategie:

1. Cititi periodic pinul cu `GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN)`.
2. Stabilizati valoarea pentru cateva iteratii.
3. La tranzitia stabila spre `0`, comutati variabila `jiggle_enabled`.
4. Nu comutati din nou pana cand butonul nu este eliberat si apasat din nou.

## Implementare: LED indicator

Proiectul `lab_hello/hello_world` arata conventia LED-urilor de pe placa:

```c
GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_ON);
GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF);
```

Aplicati aceeasi conventie aici:

```c
if (jiggle_enabled)
{
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_ON);
}
else
{
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF);
}
```

LED-ul trebuie initializat ca output GPIO. Daca proiectul USB nu initializeaza deja LED-ul in `BOARD_InitHardware()`, adaugati initializarea in codul aplicatiei sau in fisierele locale de pin mux.

## Comparatie cu exemplul SDK

Folositi GenAI pentru a compara proiectul rezultat cu exemplul original. Puteti cere asistentului sa compare:

```text
Compara aceste doua proiecte:
1. src/sdks/mcuxsdk/examples/usb_examples/usb_device_hid_mouse_lite
2. src/lab_usb_hid/usb_device_hid_mouse_lite/bm

Vreau diferentele relevante pentru:
- CMakeLists.txt si surse locale vs surse SDK;
- logica din mouse.c;
- descriptor HID si string-uri USB;
- suport SW3 si LED;
- eliminarea traseului dreptunghiular original.
```

Verificati manual concluziile. Un raspuns GenAI poate rata faptul ca un build director vechi compileaza inca sursele din SDK. Confirmati prin log-ul de build ca apar obiecte locale, de exemplu:

```text
Building C object .../mouse.c.obj
Building C object .../usb_device_descriptor.c.obj
```

## Verificare pe placa

1. Compilati proiectul:

   ```powershell
   cmake --preset debug
   cmake --build --preset debug
   ```

2. Flash-uiti fisierul binar:

   ```text
   src/lab_usb_hid/usb_device_hid_mouse_lite/bm/debug/dev_hid_mouse_lite_bm.bin
   ```

3. Conectati placa la PC prin portul USB device.
4. Verificati in sistemul de operare ca apare un mouse HID.
5. Observati miscarea cursorului:
   - trebuie sa fie miscari scurte stanga/dreapta/sus/jos;
   - nu trebuie sa deseneze dreptunghiul original al SDK-ului.
6. Apasati SW3:
   - LED-ul se stinge;
   - cursorul nu mai este deplasat.
7. Apasati SW3 din nou:
   - LED-ul se aprinde;
   - miscarea jiggler reincepe.

## Probleme frecvente

| Simptom | Cauza probabila | Verificare |
|---|---|---|
| Cursorul merge tot in dreptunghi | Build-ul compileaza inca sursele SDK | Verificati `CMakeLists.txt`, stergeti/refaceti cache-ul `debug`, cautati in log `mouse.c.obj` local |
| Dispozitivul apare ca mouse, dar nu misca | Rapoartele HID contin delta zero sau jiggler-ul este dezactivat | Verificati `jiggle_enabled`, SW3 si bufferul HID bytes 1/2 |
| SW3 nu comuta | Pinul butonului nu este configurat ca GPIO input sau logica active-low este gresita | Verificati `BOARD_SW3_GPIO`, `BOARD_SW3_GPIO_PIN`, `GPIO_PinRead` |
| LED-ul are logica inversa | LED-urile FRDM folosesc `LOGIC_LED_ON = 0` | Folositi `LOGIC_LED_ON/OFF`, nu `1/0` direct |
| Config Tools nu poate deschide proiectul | Lipseste fisierul `.mex` | Acest lab configureaza pinii in cod; nu exista suport Config Tools in sample |

## Documentatie generata cu GenAI

La final, cereti asistentului GenAI sa explice codul final:

```text
Explica pe scurt codul USB HID mouse jiggler pentru FRDM-MCXA153.
Include:
- cum functioneaza raportul HID mouse;
- cum se schimba modelul de miscare fata de exemplul SDK;
- cum comuta SW3 jiggler-ul;
- cum este folosit LED-ul ca indicator;
- ce rol are CMakeLists.txt in evitarea surselor SDK originale.
Scrie explicatia pentru studenti incepatori embedded.
```

Salvati raspunsul intr-un fisier scurt de documentatie sau intr-o sectiune `README.md` a proiectului local.

## Predare

Predarea minima include:

1. Proiectul local care compileaza.
2. `CMakeLists.txt` actualizat pentru surse locale.
3. `mouse.c` modificat pentru modelul de jiggler.
4. Toggle SW3 functional.
5. LED indicator functional.
6. O comparatie scurta cu exemplul SDK, generata si verificata cu GenAI.
7. O explicatie scurta a codului final, generata cu GenAI si corectata manual.


