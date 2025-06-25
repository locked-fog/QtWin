# QWPalette

> `#include <QtWin/QWPalette.h>`

## Features

- Auto initialization: Only need to set a seed color to automatically generate suitable foreground/background color palette

- Strong compatibility: Automatically adapts to QtWin control library's color scheme without complex type conversion

- Highly customizable: Can manually specify palette to implement custom color schemes

- Easy to use: Through numerous operator overloads, makes palette interaction with other controls simple and intuitive

## Modules

### Struct

| Name | Feature | Note |
| --- | --- | --- |
| `HCTColor` | HCT color space | Contains hue, chroma, tone components |
| `RGBColor` | sRGB color space | Can be constructed from QColor or RGB values |

### Public Functions

| Name | Parameters | Return |  Feature | Note |
| --- | --- | --- | --- | --- |
| `RGB2HCT` | `const RGBColor& rgb` | `HCTColor` | Convert sRGB to HCT color space | none |
| `HCT2RGB` | `const HCTColor& hct` | `RGBColor` | Convert HCT to sRGB color space | none |
| `extractSeedColor` | `const QImage& image, std::vector<HCTColor>& colorSet` | `void` | Extract main colors from an image | none |

### Class `QWPalette`

#### enum `QWColor`

| Name | Description |
| --- | --- |
| `mainColor` | Adjust seedColor's Chroma to 30~40 |
| `subColor` | Multiply mainColor's Chroma by 50% |
| `neutralColor` | Multiply subColor's Chroma by 50% |
| `neutralAccent` | Multiply neutralColor's Chroma by 40% |
| `accentColor` | Increase mainColor's Hue by 60° and multiply Chroma by 60% |

#### Public Functions

| Name | Parameters | Return | Feature | Note |
| --- | --- | --- | --- | --- |
| `QWPalette` | `none` | - | Default constructor | none |
| `QWPalette` | `RGBColor rgb` | - | Construct with RGB seed color | none |
| `QWPalette` | `HCTColor hct` | - | Construct with HCT seed color | none |
| `setSeedColor` | `RGBColor rgb` | `void` | Set the seed color in RGB | none |
| `setSeedColor` | `HCTColor hct` | `void` | Set the seed color in HCT | none |
| `getHCTColor` | `QWColor n, int tone` | `HCTColor` | Get a specific color in HCT | none |
| `getQColor` | `QWColor n, int tone` | `QColor` | Get a specific color in QColor | none |
| `getRGBColor` | `QWColor n, int tone` | `RGBColor` | Get a specific color in RGB | none |

## Color Scheme: Dynamic Color Extraction

### Implementation Principle (HCT)

1. Extract theme color (high proportion, vibrant color)

2. Chroma adjustment (Chroma: 30~40) to get theme color #1

3. Four additional colors: Chroma: #1-50%=#2, Chroma: #2-50%=#3, Chroma: #3-40%=#4, Chroma: #1-40% Hue: #1+60°=#5

4. Based on the five obtained colors, combine with Tone:100,99,95,90,80,70,60,50,40,30,20,10,0 to create a 60-color palette

5. Coloring rule: Foreground/background Tone difference should be at least 55

6. Naming reference: #1 mainColor, #2 subColor, #3 neutralColor, #4 neutralAccent, #5 accentColor
