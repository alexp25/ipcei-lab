---
title: "L3 - Intreruperi Externe & NVIC"
description: "Polling GPIO, debounce, conversie la GPIO IRQ"
nav_order: 4
parent: Lectii FRDM-MCXA153
layout: lesson
---

# L3 - Intreruperi Externe & NVIC

**De la polling pentru butonul SW3 la intreruperi GPIO/NVIC**

---

| | |
|---|---|
| **Ziua / Sesiunea** | Ziua 2, 23 iunie - dimineata |
| **Periferic** | `GPIO` - `GPIO IRQ` - `NVIC` |
| **Durata** | 2h (09:00-11:00) |
| **Responsabil** | Cadru didactic UPB |
| **Hardware** | FRDM-MCXA153 - SW3 (`GPIO1`, pin 7 / `P1_7`) - LED RGB |
| **Proiect** | `src/lab_interrupts/main` |

## Context si Motivatie

Inainte sa folosim intreruperi, pornim de la o implementare simpla cu polling: programul citeste periodic starea butonului SW3 si comuta LED-urile RGB cand detecteaza o apasare noua.

Codul curent separa clar cele doua idei importante:

- `check_button_pressed()` verifica butonul fara debounce.
- `check_button_pressed_debounce()` verifica butonul cu debounce software.
- `on_button_pressed()` contine actiunea comuna: toggle pentru LED-urile RGB si mesaj pe Serial Monitor.

La final, transformati detectia prin polling intr-o solutie cu intreruperi GPIO si NVIC, folosind GenAI ca asistent de conversie si verificand configuratia in MCUXpresso Config Tools.

> **Board:** FRDM-MCXA153 - MCX A153 (Cortex-M33) - SDK MCUXpresso - VS Code + CMake

## Obiective

1. Compilarea si rularea proiectului `src/lab_interrupts/main`.
2. Observarea diferentei dintre polling fara debounce si polling cu debounce.
3. Intelegerea comportamentului unui buton mecanic prin mesajele din Serial Monitor.
4. Conversia gestionarii butonului SW3 din polling in intrerupere GPIO/NVIC.
5. Actualizarea fisierului `.mex` astfel incat configuratia SW3/IRQ sa fie vizibila in Config Tools.

## Planul Sesiunii

| Interval | Activitate | Detaliu | Cine |
|---|---|---|---|
| `09:00-09:20` | **Ex 0: Compile and run** | Deschideti proiectul, compilati, rulati pe placa si verificati Serial Monitor. | student + mentor |
| `09:20-10:00` | **Ex 1: Polling si debounce** | Observati cate mesaje apar la o singura apasare SW3, apoi testati functia cu debounce. | student |
| `10:00-11:00` | **Ex 2: Conversie la intreruperi** | Folositi GenAI pentru a converti apasarea SW3 la GPIO IRQ/NVIC si actualizati `.mex`. | student + mentor |

## Exercitii

### Ex 0 - Compile and Run

1. Deschideti proiectul:

   ```text
   src/lab_interrupts/main
   ```

2. Compilati proiectul cu presetul `debug`:

   ```powershell
   cmake --preset debug
   cmake --build --preset debug
   ```

3. Incarcati aplicatia pe placa FRDM-MCXA153.
4. Deschideti Serial Monitor la `115200` baud.
5. Apasati SW3 si verificati ca LED-urile RGB se comuta.

La pornire, aplicatia face un scurt self-test pe LED-uri. Dupa aceea, fiecare apasare SW3 ar trebui sa produca mesaje de forma:

```text
SW3 pressed: toggled LEDs (0)
SW3 pressed: toggled LEDs (1)
```

### Ex 1 - Observati Comportamentul Butonului

Proiectul porneste cu functia fara debounce activa in `main.c`:

```c
while (1)
{
    check_button_pressed();

    // check_button_pressed_debounce();
}
```

1. Rulati proiectul cu `check_button_pressed()`.
2. Apasati SW3 o singura data.
3. Urmariti Serial Monitor.
4. Notati cate mesaje apar pentru o singura apasare de buton.

Intrebare de observatie:

```text
La o singura apasare fizica SW3, apar unul sau mai multe mesaje in Serial Monitor?
```

Apoi testati varianta cu debounce:

```c
while (1)
{
    // check_button_pressed();

    check_button_pressed_debounce();
}
```

5. Recompilati si rulati proiectul.
6. Apasati SW3 de mai multe ori, cate o apasare scurta pe rand.
7. Verificati Serial Monitor.

Rezultatul asteptat: cu functia `check_button_pressed_debounce()`, o apasare fizica produce un singur mesaj si o singura comutare a LED-urilor.

### Ex 2 - Convertiti SW3 la Intreruperi

Pornind de la varianta cu polling, modificati proiectul astfel incat apasarea butonului SW3 sa fie tratata prin intrerupere GPIO/NVIC.

Cerintele minime:

1. Configurati SW3 ca input GPIO pe `GPIO1`, pin `7` (`P1_7`).
2. Activati intreruperea pentru apasare pe falling edge.
3. Activati IRQ-ul `GPIO1_IRQn`.
4. Implementati handler-ul `GPIO1_IRQHandler` sau macro-ul deja definit in `board.h`: `BOARD_SW3_IRQ_HANDLER`.
5. In handler, verificati flag-ul de intrerupere pentru pinul SW3.
6. Stergeti flag-ul de intrerupere inainte de iesirea din handler.
7. Apelati `on_button_pressed()` la fiecare apasare valida.
8. Scoateti apelurile de polling din `while (1)`.

#### Cum ar trebui sa arate codul cu intreruperi

Varianta cu intreruperi trebuie sa pastreze ideea buna din codul initial: actiunea la apasarea butonului ramane in `on_button_pressed()`. Se schimba doar mecanismul prin care detectam apasarea.

In initializare, puteti fie sa extindeti `init_sw3_gpio()`, fie sa creati o functie noua, de exemplu `init_sw3_interrupt()`. Pe langa configurarea GPIO deja existenta, functia trebuie sa activeze intreruperea pentru pinul SW3:

```c
void init_sw3_interrupt(void)
{
    init_sw3_gpio();

    GPIO_SetPinInterruptConfig(BOARD_SW3_GPIO,
                               BOARD_SW3_GPIO_PIN,
                               kGPIO_InterruptFallingEdge);
    GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);

    NVIC_ClearPendingIRQ(BOARD_SW3_IRQ);
    EnableIRQ(BOARD_SW3_IRQ);
}
```

Handler-ul trebuie sa verifice ca intreruperea vine de la pinul SW3, sa stearga flag-ul, apoi sa execute actiunea:

```c
void BOARD_SW3_IRQ_HANDLER(void)
{
    if (GPIO_PinGetInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
    {
        GPIO_PinClearInterruptFlag(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN);
        on_button_pressed();
    }
}
```

Dupa conversie, `main()` nu mai trebuie sa apeleze `check_button_pressed()` sau `check_button_pressed_debounce()`. Bucla principala poate ramane simpla:

```c
init_pins();
init_sw3_interrupt();

while (1)
{
    __WFI();
}
```

`__WFI()` inseamna "wait for interrupt": procesorul asteapta urmatoarea intrerupere. Pentru laborator, este acceptabil ca handler-ul sa apeleze `on_button_pressed()` direct. In proiecte reale, un ISR ar trebui sa fie cat mai scurt: de obicei seteaza un `volatile bool` sau incrementeaza un contor, iar procesarea mai lenta se face in `while (1)`.

#### Actualizati si fisierul `.mex`

Dupa modificarea codului, actualizati configuratia proiectului in MCUXpresso Config Tools:

1. Deschideti fisierul `.mex` al proiectului.
2. Adaugati/confirmati pinul SW3 (`P1_7`) in sectiunea Pins.
3. Configurati pinul ca GPIO input cu pull-up.
4. Configurati intreruperea pe falling edge pentru SW3.
5. Confirmati ca setarea este vizibila in Config Tools, nu doar scrisa manual in cod.
6. Salvati `.mex` si regenerati fisierele de configurare daca este nevoie.
7. Recompilati proiectul.

## Prompte Pentru GenAI

### Prompt: Conversie polling la intrerupere SW3

```text
Context hardware: FRDM-MCXA153, MCX A153 Cortex-M33, SDK MCUXpresso.
Proiect: src/lab_interrupts/main.
Buton SW3: GPIO1, pin 7 (P1_7), activ low, pull-up intern.
LED-uri RGB: definite in board.h ca BOARD_LED_RED/GREEN/BLUE.

Codul curent foloseste polling si separa actiunea de detectie:
- init_sw3_gpio() configureaza SW3 ca input GPIO
- check_button_pressed() verifica o apasare fara debounce
- check_button_pressed_debounce() verifica o apasare cu debounce software
- on_button_pressed() contine actiunea comuna: toggle LED-uri + mesaj serial

Sarcina: converteste detectia apasarii SW3 din polling in intrerupere GPIO/NVIC.
Include:
- configurarea SW3 ca input GPIO cu pull-up
- falling edge interrupt pentru SW3
- EnableIRQ(GPIO1_IRQn)
- handler pentru BOARD_SW3_IRQ_HANDLER sau GPIO1_IRQHandler
- verificarea flag-ului pentru pinul SW3
- stergerea flag-ului de intrerupere
- apelarea on_button_pressed() in handler
- eliminarea apelurilor check_button_pressed() din while (1)
Explica si ce trebuie actualizat in fisierul .mex ca setarea sa fie vizibila in Config Tools.
```

### Prompt: Debug ISR care se apeleaza continuu

```text
FRDM-MCXA153, SW3 pe GPIO1 pin 7, intrerupere pe falling edge.
Problema: dupa prima apasare, handler-ul GPIO1_IRQHandler se apeleaza continuu.
Verifica daca flag-ul de intrerupere pentru pinul SW3 este sters corect.
Explica de ce un flag nesters face ca NVIC sa reintre imediat in ISR.
```

## Capcane Critice

- SW3 este activ low: apasat inseamna `GPIO_PinRead(...) == 0U`.
- Fara debounce, o singura apasare poate produce mai multe mesaje in Serial Monitor.
- In varianta cu intreruperi, flag-ul de intrerupere trebuie sters; altfel handler-ul se poate reapela continuu.
- Dupa conversia la intreruperi, `while (1)` nu mai trebuie sa faca polling pentru SW3.
- Verificati pinul exact pentru acest proiect: SW3 este `GPIO1`, pin `7` (`P1_7`).
- Dupa conversia la intreruperi, actualizati `.mex`; nu lasati configuratia doar in cod.
- GenAI poate propune API-uri pentru alta familie NXP sau pentru Arduino. Verificati mereu impotriva SDK-ului si a `board.h`.

## Deliverable

1. Proiect compilabil si functional cu polling fara debounce.
2. Observatii in Serial Monitor pentru polling fara debounce vs polling cu debounce.
3. Proiect modificat astfel incat SW3 sa fie gestionat prin intrerupere.
4. Fisier `.mex` actualizat, cu SW3/IRQ vizibil in Config Tools.
5. Scurta explicatie: de ce trebuie sters flag-ul de intrerupere in ISR.

---

[<- L2: LPUART - Comunicatie Seriala](../l2-lpuart) - [L4: Timer & PWM - SCTimer si CTIMER ->](../l4-timer-pwm)