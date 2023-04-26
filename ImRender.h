#pragma once
#include "imrender_example/imgui/imgui.h"
#include "imrender_example/imgui/imgui_internal.h"
#include <unordered_map>

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

struct ImRenderTextStyle
{
    ImRenderVerticalAxisDockType vertical;
    ImRenderHorizontalAxisDockType horizontal;
    bool outline;
    bool strikethrough;
    bool underline;
    float sizeScale;
};

struct ImRenderRectStyle
{
    ImRenderVerticalAxisDockType vertical;
    ImRenderHorizontalAxisDockType horizontal;
    bool outline;
    bool fill;
    float thickness;
    float rounding;
};

struct ImRenderLineStyle
{
    float thickness;
    bool outline;
};

struct ImRenderCircleStyle
{
    bool outline;
    bool fill;
    float thickness;
};

class CStyleAnalysisSystem
{
public:
    static std::unordered_map<std::string, std::string> AnalysisStyleMap(const std::string& style);
    static ImRenderVerticalAxisDockType AnalysisVerticalAxisDockType(std::unordered_map<std::string, std::string> styleMap);
    static ImRenderHorizontalAxisDockType AnalysisHorizontalAxisDockType(std::unordered_map<std::string, std::string> styleMap);
    static bool AnalysisBool(std::unordered_map<std::string, std::string> styleMap, const std::string& key, bool defaultValue = false);
    static float AnalysisFloat(std::unordered_map<std::string, std::string> styleMap, const std::string& key, float defaultValue = 0.f);
    static ImRenderTextStyle AnalysisTextStyle(std::string& style);
    static ImRenderRectStyle AnalysisRectStyle(std::string& style);
    static ImRenderLineStyle AnalysisLineStyle(std::string& style);
    static ImRenderCircleStyle AnalysisCircleStyle(std::string& style);
};

class CImRender
{
private:
    ImDrawList* m_pDrawList;
public:
    virtual ImVec2 Text(std::string text, ImVec2 pos, ImVec4 color, std::string style);
    virtual void Rect(ImVec2 pos, ImVec2 size, ImVec4 color, std::string style);
    virtual void Line(ImVec2 pos1, ImVec2 pos2, ImVec4 color, std::string style);
    virtual void Circle(ImVec2 pos, float radius, ImVec4 color, std::string style);
};

inline CImRender* ImRender = new CImRender();
