# ImRender

### The Pitch

In this lib I introduced a concept, this concept is "String Style".

Like this:

```c++
ImRender->Text("Test", ImVec2(100.f,100.f), "Horizontal:center;");
```

The second string parameter is the style.

This way can avoided the difficult problem of passing parameters.

What the difficult problem of passing parameters? Like this:

```c++
// Method Definition:
ImVec2 Text(std::string text, ImVec2 pos, ImVec4 color, bool outline = false, bool top_bottom_center = false, bool to_top = false, bool left_right_center = false, bool to_left = false);

// Normal Invoke:
// I just want to set the "to_left" parameter but I have to pass so many parameters.
Text("Test", ImVec2(100.f, 100.f), false, false, false, false, true);
// It's not elegant!

// Elegantly Invoke:
ImRender->Text("Test", pos, color, "Horizontal:left;");
```

So in this lib you just need to pass the string is able to solv this problem.

### Usage

Just passing parameters like old.

How can I pass the "style" parameter?

```c++
// Do not think too diffcult.
// Examples:
ImRender->Text("Test", pos, color, "Horizontal:center;Outline:true");
ImRender->Rect(pos, size, color, "Horizontal:center;Outline:true;Thickness:2.f;");
ImRender->Line(pos1, pos2, color, "Outline:true;Thickness:2.555555;");
ImRender->Circle(pos, radius, color, "Outline:true;");
```

What style can I pass?

| Method | Acceptable Style                                             |
| ------ | ------------------------------------------------------------ |
| Text   | Horizontal, Vertical, Outline, SizeScale, Strikethrough, Underline |
| Rect   | Horizontal, Vertical, Outline, Fill, Thickness, Rounding     |
| Line   | Thickness, Outline                                           |
| Circle | Outline, Fill, Thickness                                     |

What their type?

| Name          | Type                           | Acceptable value        |
| ------------- | ------------------------------ | ----------------------- |
| Horizontal    | ImRenderHorizontalAxisDockType | "left" "right" "center" |
| Vertical      | ImRenderVerticalAxisDockType   | "top" "bottom" "center" |
| Outline       | Bool                           | "true" "false"          |
| SizeScale     | Float                          | Total natural number    |
| Strikethrough | Bool                           | "true" "false"          |
| Underline     | Bool                           | "true" "false"          |
| Fill          | Bool                           | "true" "false"          |
| Thickness     | Float                          | Total natural number    |
| Rounding      | Float                          | Total natural number    |

What should I pay attention to?

- Do not write a wrong key
- Uppercase and lowercase are not important (You even can pass "hORizOnRTal:lEfT;")
- Do not forget ';'
- Do not have space character

