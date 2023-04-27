#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <unordered_map>

#define IMRENDER_STYLE_VALUE_VERTICALDOCKTYPE ImRenderVerticalAxisDockType vertical = this->GetVerticalAxisDockType()
#define IMRENDER_STYLE_VALUE_HORIZONTALDOCKTYPE ImRenderHorizontalAxisDockType horizontal = this->GetHorizontalAxisDockType()
#define IMRENDER_STYLE_VALUE_BOOL(key,defaultValue) bool key = this->GetBool(#key,defaultValue)
#define IMRENDER_STYLE_VALUE_FLOAT(key,defaultValue) float key = this->GetFloat(#key,defaultValue)

enum class ImRenderVerticalAxisDockType
{
    Top,
    Bottom,
    Center
};

enum class ImRenderHorizontalAxisDockType
{
    Left,
    Right,
    Center
};

struct ImRenderStyleBase
{
private:
    std::unordered_map<std::string, std::string> m_styleMap;
public:
	ImRenderStyleBase(const std::string& style);
    ImRenderVerticalAxisDockType GetVerticalAxisDockType();
    ImRenderHorizontalAxisDockType GetHorizontalAxisDockType();
    bool GetBool(const std::string& key, bool defaultValue);
    float GetFloat(const std::string& key, float defaultValue);
};

struct ImRenderTextStyle : public ImRenderStyleBase
{
	ImRenderTextStyle(const std::string& style) : ImRenderStyleBase(style) {}
    IMRENDER_STYLE_VALUE_VERTICALDOCKTYPE;
    IMRENDER_STYLE_VALUE_HORIZONTALDOCKTYPE;
    IMRENDER_STYLE_VALUE_BOOL(outline, false);
	IMRENDER_STYLE_VALUE_BOOL(strikethrough, false);
	IMRENDER_STYLE_VALUE_BOOL(underline, false);
	IMRENDER_STYLE_VALUE_FLOAT(sizeScale, 1.f);
};

struct ImRenderRectStyle : public ImRenderStyleBase
{
	ImRenderRectStyle(const std::string& style) : ImRenderStyleBase(style) {}
    IMRENDER_STYLE_VALUE_VERTICALDOCKTYPE;
    IMRENDER_STYLE_VALUE_HORIZONTALDOCKTYPE;
	IMRENDER_STYLE_VALUE_BOOL(outline, false);
	IMRENDER_STYLE_VALUE_BOOL(fill, false);
	IMRENDER_STYLE_VALUE_FLOAT(thickness, 1.f);
	IMRENDER_STYLE_VALUE_FLOAT(rounding, 0.f);
};

struct ImRenderLineStyle : public ImRenderStyleBase
{
	ImRenderLineStyle(const std::string& style) : ImRenderStyleBase(style) {}
	IMRENDER_STYLE_VALUE_FLOAT(thickness, 1.f);
	IMRENDER_STYLE_VALUE_BOOL(outline, false);
};

struct ImRenderCircleStyle : public ImRenderStyleBase
{
	ImRenderCircleStyle(const std::string& style) : ImRenderStyleBase(style) {}
	IMRENDER_STYLE_VALUE_BOOL(outline, false);
	IMRENDER_STYLE_VALUE_BOOL(fill, false);
	IMRENDER_STYLE_VALUE_FLOAT(thickness, 1.f);
};

class CImRender
{
private:
    ImDrawList* m_pDrawList;
public:
    ImVec2 Text(std::string text, ImVec2 pos, ImVec4 color, const std::string& style);
    void Rect(ImVec2 pos, ImVec2 size, ImVec4 color, const std::string& style);
    void Line(ImVec2 pos1, ImVec2 pos2, ImVec4 color, const std::string& style);
    void Circle(ImVec2 pos, float radius, ImVec4 color, const std::string& style);
};

inline CImRender* ImRender = new CImRender();

#undef IMRENDER_STYLE_VALUE_VERTICALDOCKTYPE
#undef IMRENDER_STYLE_VALUE_HORIZONTALDOCKTYPE
#undef IMRENDER_STYLE_VALUE_BOOL
#undef IMRENDER_STYLE_VALUE_FLOAT
