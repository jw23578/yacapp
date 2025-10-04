#ifndef CONFIGURATORCONSTANTS_H
#define CONFIGURATORCONSTANTS_H

#include <QObject>
#include "yacappmacros.h"
#include <QColor>

class ConfiguratorConstants : public QObject
{
    Q_OBJECT
    YACAPPPROPERTY(QColor, backgroundPrimaryColor, BackgroundPrimaryColor, 0x0d1117)
    YACAPPPROPERTY(QColor, backgroundSecondaryColor, BackgroundSecondaryColor, 0x161b22)
    YACAPPPROPERTY(QColor, inputBackgroundColor, InputBackgroundColor, 0x161b22)
    YACAPPPROPERTY(QColor, inputTextColor, InputTextColor, 0xf0f6fc)
    YACAPPPROPERTY(QColor, textPrimaryColor, TextPrimaryColor, 0xf0f6fc)
    YACAPPPROPERTY(QColor, buttonPrimaryColor, ButtonPrimaryColor, 0x1f6feb)
    YACAPPPROPERTY(QColor, buttonPrimaryHoverColor, ButtonPrimaryHoverColor, 0x0969da)
    YACAPPPROPERTY(QColor, borderPrimaryColor, BorderPrimaryColor, 0x30363d)

public:
    explicit ConfiguratorConstants(QObject *parent = nullptr);

signals:
};

#endif // CONFIGURATORCONSTANTS_H

/*

# Blue Purple Dark Theme Color Palette

## Basis-Farben
- Background_Primary: #0d1117
- Background_Secondary: #161b22
- Background_Tertiary: #21262d
- Background_Card: #1c2128
- Background_Modal: #0a0e13

## Text-Farben
- Text_Primary: #f0f6fc
- Text_Secondary: #c9d1d9
- Text_Tertiary: #8b949e
- Text_Disabled: #6e7681
- Text_Placeholder: #484f58

## Border & Divider
- Border_Primary: #30363d
- Border_Secondary: #21262d
- Border_Focus: #1f6feb
- Divider_Light: #262c36
- Divider_Heavy: #3d4148

## Status-Farben
- Success: #2ea043
- Warning: #f85149
- Error: #da3633
- Info: #58a6ff

## Highlight-Farben
- Highlight_Blue_Bright: #58a6ff
- Highlight_Blue_Dark: #1f6feb
- Highlight_Purple_Bright: #a5a3ff
- Highlight_Purple_Dark: #6f42c1
- Highlight_Purple_Deep: #8b5cf6
- Highlight_Blue_Electric: #00d4ff
- Highlight_Purple_Neon: #c084fc
- Highlight_Blue_Ice: #7dd3fc

## Element-Auszeichnungen
- Element_Hover: #262c36
- Element_Active: #373e47
- Element_Selected: #1f2937
- Element_Pressed: #0f1419

## Accent-Farben
- Accent_Primary: #1f6feb
- Accent_Secondary: #8b5cf6
- Accent_Light: #58a6ff
- Accent_Dark: #0969da

## Semantic Colors
- Link: #58a6ff
- Link_Hover: #1f6feb
- Link_Visited: #a5a3ff

## Code & Syntax
- Code_Background: #0d1117
- Code_String: #a5a3ff
- Code_Number: #79c0ff
- Code_Keyword: #ff7b72
- Code_Comment: #8b949e
- Code_Function: #d2a8ff
- Code_Variable: #ffa657

## Interactive Elements
- Button_Primary: #1f6feb
- Button_Primary_Hover: #0969da
- Button_Secondary: #8b5cf6
- Button_Secondary_Hover: #6d28d9
- Button_Danger: #da3633
- Button_Danger_Hover: #b91c1c

## Form Elements
- Input_Background: #161b22
- Input_Border: #30363d
- Input_Border_Focus: #1f6feb
- Input_Text: #f0f6fc

## Chart & Data Colors
- Chart_Blue_Primary: #1f6feb
- Chart_Blue_Light: #58a6ff
- Chart_Purple_Primary: #8b5cf6
- Chart_Purple_Light: #a5a3ff
- Chart_Blue_Dark: #0969da
- Chart_Purple_Dark: #6d28d9
- Chart_Cyan: #39d0d8
- Chart_Indigo: #6366f1

## Special Elements
- Sidebar_Background: #0d1117
- Header_Background: #161b22
- Footer_Background: #0d1117
- Card_Shadow: #000000
- Overlay_Background: #000000cc

*/
