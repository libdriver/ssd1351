[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SSD1351

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ssd1351/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der SSD1351 ist ein CMOS-OLED/PLED-Treiber mit 384 Segmenten und 128 gemeinsamen Ausgängen, der bis zu 128RGB x 128 Punktmatrixanzeige unterstützt. Dieser Chip ist für OLED/PLED-Panels vom Typ Common Cathode ausgelegt. Die SSD1351 verfügt über einen eingebetteten Grafikanzeigedaten-RAM (GDDRAM). Es unterstützt mit 8, 16, 18 Bit 8080/6800 Parallelschnittstelle, Serial Peripheral Interface. Es verfügt über einen 256-stufigen Kontrast und eine 262K-Farbsteuerung, die eine lebendige Farbdarstellung auf OLED-Panels ermöglicht.

LibDriver SSD1351 ist der voll funktionsfähige Treiber von SSD1351, der von LibDriver gestartet wurde. Er bietet die Funktionen zum Zeichnen von Einzelpunkten, Füllen von Blöcken, Bildanzeige usw. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver SSD1351-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver SSD1351 SPI.

/test enthält den Testcode des LibDriver SSD1351-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver SSD1351-Beispielcode.

/doc enthält das LibDriver SSD1351-Offlinedokument.

/Datenblatt enthält SSD1351-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

### Installieren

Verweisen Sie auf eine plattformunabhängige SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-SPI-Treiber fertig.

Fügen Sie /src, /interface und /example zu Ihrem Projekt hinzu.

### Nutzung

#### example basic

```C
#include "driver_ssd1351_basic.h"

uint8_t res;

res = ssd1351_basic_init();
if (res != 0)
{
    ssd1351_interface_debug_print("ssd1351: init failed.\n");

    return 1;
}

...

res = ssd1351_basic_clear();
if (res != 0)
{
    ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
    (void)ssd1351_basic_deinit();

    return 1;
}
res = ssd1351_basic_string(0, 0, "123", 3, 0xFFFF, SSD1351_FONT_16);
if (res != 0)
{
    ssd1351_interface_debug_print("ssd1351: show string failed.\n");
    (void)ssd1351_basic_deinit();

    return 1;
}

...

res = ssd1351_basic_rect(0, 32, 32, 32, 0xFFFF);
if (res != 0)
{
    (void)ssd1351_basic_deinit();

    return 1;
}

...

(void)ssd1351_basic_deinit();

return 0;
```

#### example advance

Das Bildextraktionsformat ist wie folgt und das Farbformat ist RGB565.

<div align=center>
<img src="/doc/image/image_format.svg" width="220" height="220"/>
</div>

```c
#include "driver_ssd1351_advance.h"

uint8_t res;
uint16_t image[16384];

res = ssd1351_advance_init();
if (res != 0)
{
    ssd1351_interface_debug_print("ssd1351: init failed.\n");

    return 1;
}

...

res = ssd1351_advance_clear();
if (res != 0)
{
    ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
    (void)ssd1351_advance_deinit();

    return 1;
}
res = ssd1351_advance_string(0, 0, "123", 3, 0xFFFF, SSD1351_FONT_16);
if (res != 0)
{
    ssd1351_interface_debug_print("ssd1351: show string failed.\n");
    (void)ssd1351_advance_deinit();

    return 1;
}

...

res = ssd1351_advance_write_point(56, 56, 0xFF00);
if (res != 0)
{
    (void)ssd1351_advance_deinit();

    return 1;
}

...

res = ssd1351_advance_rect(0, 32, 32, 32, 0xFFFF);
if (res != 0)
{
    (void)ssd1351_advance_deinit();

    return 1;
}

...

res = ssd1351_advance_set_scroll(1, 0, 128, SSD1351_SCROLL_MODE_NORMAL);
if (res != 0)
{
    (void)ssd1351_advance_deinit();

    return 1;
}
res = ssd1351_advance_enable_scroll();
if (res != 0)
{
    (void)ssd1351_advance_deinit();

    return 1;
}

...

res = ssd1351_advance_draw_pictrue_16bits(0, 0, 0, 0, image);
if (res != 0)
{
    (void)ssd1351_advance_deinit();

    return 1;
}

...

(void)ssd1351_advance_deinit();

return 0;
```

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ssd1351/index.html](https://www.libdriver.com/docs/ssd1351/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.