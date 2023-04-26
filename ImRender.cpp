#include "ImRender.h"
#include <algorithm>
#include <ctype.h>
#include <string>

std::unordered_map<std::string, std::string> CStyleAnalysisSystem::AnalysisStyleMap(const std::string& style)
{
    std::unordered_map<std::string, std::string> styleMap;
    std::string::size_type pos = 0;
    std::string::size_type prePos = 0;
    while ((pos = style.find(";", prePos)) != std::string::npos)
    {
        std::string::size_type equalPos = style.find(":", prePos);
        std::string key = style.substr(prePos, equalPos - prePos);
        std::string value = style.substr(equalPos + 1, pos - equalPos - 1);
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        styleMap[key] = value;
        prePos = pos + 1;
    }
    return styleMap;
}

ImRenderVerticalAxisDockType CStyleAnalysisSystem::AnalysisVerticalAxisDockType(std::unordered_map<std::string, std::string> styleMap)
{
    if (styleMap.find("vertical") != styleMap.end())
    {
        std::string vertical = styleMap["vertical"];
        if (vertical.compare("top") == 0) return ImRenderVerticalAxisDockType::Top;
        else if (vertical.compare("bottom") == 0) return ImRenderVerticalAxisDockType::Bottom;
        else if (vertical.compare("center") == 0) return ImRenderVerticalAxisDockType::Center;
    }
}

ImRenderHorizontalAxisDockType CStyleAnalysisSystem::AnalysisHorizontalAxisDockType(std::unordered_map<std::string, std::string> styleMap)
{
    if (styleMap.find("horizontal") != styleMap.end())
    {
        std::string horizontal = styleMap["horizontal"];
        if (horizontal.compare("left") == 0) return ImRenderHorizontalAxisDockType::Left;
        else if (horizontal.compare("right") == 0) return ImRenderHorizontalAxisDockType::Right;
        else if (horizontal.compare("center") == 0) return ImRenderHorizontalAxisDockType::Center;
    }
}

bool CStyleAnalysisSystem::AnalysisBool(std::unordered_map<std::string, std::string> styleMap, const std::string& key, bool defaultValue)
{
    std::string lowerKey = key;
    std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);
    if (styleMap.find(lowerKey) != styleMap.end())
    {
        std::string value = styleMap[lowerKey];
        if (value.compare("true") == 0) return true;
        else if (value.compare("false") == 0) return false;
    }
    return defaultValue;
}

float CStyleAnalysisSystem::AnalysisFloat(std::unordered_map<std::string, std::string> styleMap, const std::string& key, float defaultValue)
{
    std::string lowerKey = key;
    std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);
    if (styleMap.find(lowerKey) != styleMap.end())
    {
        std::string value = styleMap[lowerKey];
        try
        {
            return std::stof(value);
        }
        catch (...) {}
    }
    return defaultValue;
}

ImRenderTextStyle CStyleAnalysisSystem::AnalysisTextStyle(std::string& style)
{
    ImRenderTextStyle textStyle;
    textStyle.vertical = ImRenderVerticalAxisDockType::Bottom;
    textStyle.horizontal = ImRenderHorizontalAxisDockType::Right;
    textStyle.outline = false;
    textStyle.strikethrough = false;
    textStyle.underline = false;
    textStyle.sizeScale = 1.f;
    if (style.empty())  return textStyle;
    std::unordered_map<std::string, std::string> styleMap = AnalysisStyleMap(style);
    textStyle.vertical = AnalysisVerticalAxisDockType(styleMap);
    textStyle.horizontal = AnalysisHorizontalAxisDockType(styleMap);
    textStyle.outline = AnalysisBool(styleMap, "Outline");
    textStyle.strikethrough = AnalysisBool(styleMap, "Strikethrough");
    textStyle.underline = AnalysisBool(styleMap, "Underline");
    textStyle.sizeScale = AnalysisFloat(styleMap, "SizeScale", 1.f);
    return textStyle;
}

ImRenderRectStyle CStyleAnalysisSystem::AnalysisRectStyle(std::string& style)
{
    ImRenderRectStyle rectStyle;
    rectStyle.vertical = ImRenderVerticalAxisDockType::Bottom;
    rectStyle.horizontal = ImRenderHorizontalAxisDockType::Right;
    rectStyle.outline = false;
    rectStyle.fill = false;
    rectStyle.thickness = 1.0f;
    rectStyle.rounding = 0.0f;
    if (style.empty())  return rectStyle;
    std::unordered_map<std::string, std::string> styleMap = AnalysisStyleMap(style);
    rectStyle.vertical = AnalysisVerticalAxisDockType(styleMap);
    rectStyle.horizontal = AnalysisHorizontalAxisDockType(styleMap);
    rectStyle.outline = AnalysisBool(styleMap, "Outline");
    rectStyle.fill = AnalysisBool(styleMap, "Fill");
    rectStyle.thickness = AnalysisFloat(styleMap, "Thickness", 1.f);
    rectStyle.rounding = AnalysisFloat(styleMap, "Rounding");
    return rectStyle;
}

ImRenderLineStyle CStyleAnalysisSystem::AnalysisLineStyle(std::string& style)
{
    ImRenderLineStyle lineStyle;
    lineStyle.thickness = 1.0f;
    lineStyle.outline = false;
    if (style.empty())  return lineStyle;
    std::unordered_map<std::string, std::string> styleMap = AnalysisStyleMap(style);
    lineStyle.thickness = AnalysisFloat(styleMap, "Thickness", 1.f);
    lineStyle.outline = AnalysisBool(styleMap, "Outline");
    return lineStyle;
}

ImRenderCircleStyle CStyleAnalysisSystem::AnalysisCircleStyle(std::string& style)
{
    ImRenderCircleStyle circleStyle;
    circleStyle.outline = false;
    circleStyle.fill = false;
    circleStyle.thickness = 1.0f;
    if (style.empty())  return circleStyle;
    std::unordered_map<std::string, std::string> styleMap = AnalysisStyleMap(style);
    circleStyle.outline = AnalysisBool(styleMap, "Outline");
    circleStyle.fill = AnalysisBool(styleMap, "Fill");
    circleStyle.thickness = AnalysisFloat(styleMap, "Thickness", 1.f);
    return circleStyle;
}

ImVec2 CImRender::Text(std::string text, ImVec2 pos, ImVec4 color, std::string style)
{
    m_pDrawList = ImGui::GetBackgroundDrawList();
    ImRenderTextStyle textStyle = CStyleAnalysisSystem::AnalysisTextStyle(style);
    ImGuiContext& g = *GImGui;
    const float font_size = g.FontSize * textStyle.sizeScale;
    ImFont* font = g.Font;
    ImVec2 textSize = font->CalcTextSizeA(font_size, FLT_MAX, -1.f, text.c_str(), ImGui::FindRenderedTextEnd(text.c_str(), 0), NULL);  textSize.x = IM_FLOOR(textSize.x + 0.99999f);
    ImVec2 renderPos = pos;
    if (textStyle.vertical == ImRenderVerticalAxisDockType::Top)
        renderPos.y -= textSize.y;
    else if (textStyle.vertical == ImRenderVerticalAxisDockType::Center)
        renderPos.y -= textSize.y / 2;
    if (textStyle.horizontal == ImRenderHorizontalAxisDockType::Left)
        renderPos.x -= textSize.x;
    else if (textStyle.horizontal == ImRenderHorizontalAxisDockType::Center)
        renderPos.x -= textSize.x / 2;
    if (textStyle.outline)
    {
        m_pDrawList->AddText(font, font_size, ImVec2(renderPos.x - 1, renderPos.y - 1), IM_COL32_BLACK, text.c_str());
        m_pDrawList->AddText(font, font_size, ImVec2(renderPos.x + 1, renderPos.y - 1), IM_COL32_BLACK, text.c_str());
        m_pDrawList->AddText(font, font_size, ImVec2(renderPos.x - 1, renderPos.y + 1), IM_COL32_BLACK, text.c_str());
        m_pDrawList->AddText(font, font_size, ImVec2(renderPos.x + 1, renderPos.y + 1), IM_COL32_BLACK, text.c_str());
    }
    m_pDrawList->AddText(font, font_size, renderPos, ImGui::GetColorU32(color), text.c_str());
    if (textStyle.strikethrough)
        m_pDrawList->AddLine(ImVec2(renderPos.x, renderPos.y + textSize.y / 2), ImVec2(renderPos.x + textSize.x, renderPos.y + textSize.y / 2), IM_COL32_BLACK, 1.0f);
    if (textStyle.underline)
        m_pDrawList->AddLine(ImVec2(renderPos.x, renderPos.y + textSize.y), ImVec2(renderPos.x + textSize.x, renderPos.y + textSize.y), IM_COL32_BLACK, 1.0f);
    return textSize;
}

void CImRender::Rect(ImVec2 pos, ImVec2 size, ImVec4 color, std::string style)
{
    m_pDrawList = ImGui::GetBackgroundDrawList();
    ImRenderRectStyle retctStyle = CStyleAnalysisSystem::AnalysisRectStyle(style);
    ImVec2 renderPos = pos;
    if (retctStyle.vertical == ImRenderVerticalAxisDockType::Top)
        renderPos.y -= size.y;
    else if (retctStyle.vertical == ImRenderVerticalAxisDockType::Center)
        renderPos.y -= size.y / 2;
    if (retctStyle.horizontal == ImRenderHorizontalAxisDockType::Left)
        renderPos.x -= size.x;
    else if (retctStyle.horizontal == ImRenderHorizontalAxisDockType::Center)
        renderPos.x -= size.x / 2;
    if (retctStyle.fill)
    {
        if (retctStyle.outline)
            m_pDrawList->AddRectFilled(ImVec2(renderPos.x - 1.f, renderPos.y - 1.f), ImVec2(renderPos.x + size.x + 1.f, renderPos.y + size.y + 1.f), IM_COL32_BLACK, retctStyle.rounding);
        m_pDrawList->AddRectFilled(renderPos, ImVec2(renderPos.x + size.x, renderPos.y + size.y), ImGui::GetColorU32(color), retctStyle.rounding);
    }
    else
    {
        if (retctStyle.outline)
        {
            m_pDrawList->AddRect(ImVec2(renderPos.x - 1.f, renderPos.y - 1.f), ImVec2(renderPos.x + size.x + 1.f, renderPos.y + size.y + 1.f), IM_COL32_BLACK, retctStyle.rounding, ImDrawCornerFlags_All, retctStyle.thickness);
            m_pDrawList->AddRect(ImVec2(renderPos.x + 1.f, renderPos.y + 1.f), ImVec2(renderPos.x + size.x - 1.f, renderPos.y + size.y - 1.f), IM_COL32_BLACK, retctStyle.rounding, ImDrawCornerFlags_All, retctStyle.thickness);
        }
        m_pDrawList->AddRect(renderPos, ImVec2(renderPos.x + size.x, renderPos.y + size.y), ImGui::GetColorU32(color), retctStyle.rounding, 0, retctStyle.thickness);
    }
}

void CImRender::Line(ImVec2 pos1, ImVec2 pos2, ImVec4 color, std::string style)
{
    m_pDrawList = ImGui::GetBackgroundDrawList();
    ImRenderLineStyle lineStyle = CStyleAnalysisSystem::AnalysisLineStyle(style);
    if (lineStyle.outline)
        m_pDrawList->AddLine(pos1, pos2, IM_COL32_BLACK, lineStyle.thickness + 2);
    m_pDrawList->AddLine(pos1, pos2, ImGui::GetColorU32(color), lineStyle.thickness);
}

void CImRender::Circle(ImVec2 pos, float radius, ImVec4 color, std::string style)
{
    m_pDrawList = ImGui::GetBackgroundDrawList();
    ImRenderCircleStyle circleStyle = CStyleAnalysisSystem::AnalysisCircleStyle(style);
    if (circleStyle.fill)
    {
        if (circleStyle.outline)
            m_pDrawList->AddCircleFilled(ImVec2(pos.x, pos.y), radius + 1.f, IM_COL32_BLACK);
        m_pDrawList->AddCircleFilled(pos, radius, ImGui::GetColorU32(color));
    }
    else
    {
        if (circleStyle.outline)
        {
            m_pDrawList->AddCircle(ImVec2(pos.x, pos.y), radius + 1.f, IM_COL32_BLACK, 0, circleStyle.thickness);
            m_pDrawList->AddCircle(ImVec2(pos.x, pos.y), radius - 1.f, IM_COL32_BLACK, 0, circleStyle.thickness);
        }
        m_pDrawList->AddCircle(pos, radius, ImGui::GetColorU32(color), 0, circleStyle.thickness);
    }
}
