#pragma once
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

inline ImRenderStyleBase::ImRenderStyleBase(const std::string& style)
{
	std::string::size_type pos = 0;
	std::string::size_type prePos = 0;
	while ((pos = style.find(";", prePos)) != std::string::npos)
	{
		std::string::size_type equalPos = style.find(":", prePos);
		std::string key = style.substr(prePos, equalPos - prePos);
		std::string value = style.substr(equalPos + 1, pos - equalPos - 1);
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);
		m_styleMap[key] = value;
		prePos = pos + 1;
	}
}

inline ImRenderVerticalAxisDockType ImRenderStyleBase::GetVerticalAxisDockType()
{
	if (m_styleMap.find("vertical") != m_styleMap.end())
	{
		std::string vertical = m_styleMap["vertical"];
		if (vertical.compare("top") == 0) return ImRenderVerticalAxisDockType::Top;
		else if (vertical.compare("bottom") == 0) return ImRenderVerticalAxisDockType::Bottom;
		else if (vertical.compare("center") == 0) return ImRenderVerticalAxisDockType::Center;
	}
	return ImRenderVerticalAxisDockType::Bottom;
}

inline ImRenderHorizontalAxisDockType ImRenderStyleBase::GetHorizontalAxisDockType()
{
	if (m_styleMap.find("horizontal") != m_styleMap.end())
	{
		std::string horizontal = m_styleMap["horizontal"];
		if (horizontal.compare("left") == 0) return ImRenderHorizontalAxisDockType::Left;
		else if (horizontal.compare("right") == 0) return ImRenderHorizontalAxisDockType::Right;
		else if (horizontal.compare("center") == 0) return ImRenderHorizontalAxisDockType::Center;
	}
	return ImRenderHorizontalAxisDockType::Right;
}

inline bool ImRenderStyleBase::GetBool(const std::string& key, bool defaultValue)
{
	std::string lowerKey = key;
	std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);
	if (m_styleMap.find(lowerKey) != m_styleMap.end())
	{
		std::string value = m_styleMap[lowerKey];
		if (value.compare("true") == 0) return true;
		else if (value.compare("false") == 0) return false;
	}
	return defaultValue;
}

inline float ImRenderStyleBase::GetFloat(const std::string& key, float defaultValue)
{
	std::string lowerKey = key;
	std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);
	if (m_styleMap.find(lowerKey) != m_styleMap.end())
	{
		std::string value = m_styleMap[lowerKey];
		try
		{
			return std::stof(value);
		}
		catch (...) {}
	}
	return defaultValue;
}

inline ImVec2 CImRender::Text(std::string text, ImVec2 pos, ImVec4 color, const std::string& style)
{
	static std::unordered_map<std::string, ImRenderTextStyle*> m_styleBaseMap;
	ImRenderTextStyle* textStyle = nullptr;
	m_pDrawList = ImGui::GetBackgroundDrawList();
	if (m_styleBaseMap.find(style) != m_styleBaseMap.end())
		textStyle = m_styleBaseMap[style];
	else
		m_styleBaseMap[style] = textStyle = new ImRenderTextStyle(style);
	ImGuiContext& g = *GImGui;
	const float font_size = g.FontSize * textStyle->sizeScale;
	ImFont* font = g.Font;
	ImVec2 textSize = font->CalcTextSizeA(font_size, FLT_MAX, -1.f, text.c_str(), ImGui::FindRenderedTextEnd(text.c_str(), 0), NULL);  textSize.x = IM_FLOOR(textSize.x + 0.99999f);
	ImVec2 renderPos = pos;
	if (textStyle->vertical == ImRenderVerticalAxisDockType::Top)
		renderPos.y -= textSize.y;
	else if (textStyle->vertical == ImRenderVerticalAxisDockType::Center)
		renderPos.y -= textSize.y / 2;
	if (textStyle->horizontal == ImRenderHorizontalAxisDockType::Left)
		renderPos.x -= textSize.x;
	else if (textStyle->horizontal == ImRenderHorizontalAxisDockType::Center)
		renderPos.x -= textSize.x / 2;
	if (textStyle->outline)
	{
		m_pDrawList->AddText(font, font_size, ImVec2(renderPos.x - 1, renderPos.y - 1), IM_COL32_BLACK, text.c_str());
		m_pDrawList->AddText(font, font_size, ImVec2(renderPos.x + 1, renderPos.y - 1), IM_COL32_BLACK, text.c_str());
		m_pDrawList->AddText(font, font_size, ImVec2(renderPos.x - 1, renderPos.y + 1), IM_COL32_BLACK, text.c_str());
		m_pDrawList->AddText(font, font_size, ImVec2(renderPos.x + 1, renderPos.y + 1), IM_COL32_BLACK, text.c_str());
	}
	m_pDrawList->AddText(font, font_size, renderPos, ImGui::GetColorU32(color), text.c_str());
	if (textStyle->strikethrough)
		m_pDrawList->AddLine(ImVec2(renderPos.x, renderPos.y + textSize.y / 2), ImVec2(renderPos.x + textSize.x, renderPos.y + textSize.y / 2), IM_COL32_BLACK, 1.0f);
	if (textStyle->underline)
		m_pDrawList->AddLine(ImVec2(renderPos.x, renderPos.y + textSize.y), ImVec2(renderPos.x + textSize.x, renderPos.y + textSize.y), IM_COL32_BLACK, 1.0f);
	return textSize;
}

inline void CImRender::Rect(ImVec2 pos, ImVec2 size, ImVec4 color, const std::string& style)
{
	static std::unordered_map<std::string, ImRenderRectStyle*> m_styleBaseMap;
	ImRenderRectStyle* retctStyle = nullptr;
	m_pDrawList = ImGui::GetBackgroundDrawList();
	if (m_styleBaseMap.find(style) != m_styleBaseMap.end())
		retctStyle = m_styleBaseMap[style];
	else
		m_styleBaseMap[style] = retctStyle = new ImRenderRectStyle(style);
	ImVec2 renderPos = pos;
	if (retctStyle->vertical == ImRenderVerticalAxisDockType::Top)
		renderPos.y -= size.y;
	else if (retctStyle->vertical == ImRenderVerticalAxisDockType::Center)
		renderPos.y -= size.y / 2;
	if (retctStyle->horizontal == ImRenderHorizontalAxisDockType::Left)
		renderPos.x -= size.x;
	else if (retctStyle->horizontal == ImRenderHorizontalAxisDockType::Center)
		renderPos.x -= size.x / 2;
	if (retctStyle->fill)
	{
		if (retctStyle->outline)
			m_pDrawList->AddRectFilled(ImVec2(renderPos.x - 1.f, renderPos.y - 1.f), ImVec2(renderPos.x + size.x + 1.f, renderPos.y + size.y + 1.f), IM_COL32_BLACK, retctStyle->rounding);
		m_pDrawList->AddRectFilled(renderPos, ImVec2(renderPos.x + size.x, renderPos.y + size.y), ImGui::GetColorU32(color), retctStyle->rounding);
	}
	else
	{
		if (retctStyle->outline)
		{
			m_pDrawList->AddRect(ImVec2(renderPos.x - 1.f, renderPos.y - 1.f), ImVec2(renderPos.x + size.x + 1.f, renderPos.y + size.y + 1.f), IM_COL32_BLACK, retctStyle->rounding, ImDrawCornerFlags_All, retctStyle->thickness);
			m_pDrawList->AddRect(ImVec2(renderPos.x + 1.f, renderPos.y + 1.f), ImVec2(renderPos.x + size.x - 1.f, renderPos.y + size.y - 1.f), IM_COL32_BLACK, retctStyle->rounding, ImDrawCornerFlags_All, retctStyle->thickness);
		}
		m_pDrawList->AddRect(renderPos, ImVec2(renderPos.x + size.x, renderPos.y + size.y), ImGui::GetColorU32(color), retctStyle->rounding, 0, retctStyle->thickness);
	}
}

inline void CImRender::Line(ImVec2 pos1, ImVec2 pos2, ImVec4 color, const std::string& style)
{
	static std::unordered_map<std::string, ImRenderLineStyle*> m_styleBaseMap;
	ImRenderLineStyle* lineStyle = nullptr;
	m_pDrawList = ImGui::GetBackgroundDrawList();
	if (m_styleBaseMap.find(style) != m_styleBaseMap.end())
		lineStyle = m_styleBaseMap[style];
	else
		m_styleBaseMap[style] = lineStyle = new ImRenderLineStyle(style);
	if (lineStyle->outline)
		m_pDrawList->AddLine(pos1, pos2, IM_COL32_BLACK, lineStyle->thickness + 2);
	m_pDrawList->AddLine(pos1, pos2, ImGui::GetColorU32(color), lineStyle->thickness);
}

inline void CImRender::Circle(ImVec2 pos, float radius, ImVec4 color, const std::string& style)
{
	static std::unordered_map<std::string, ImRenderCircleStyle*> m_styleBaseMap;
	ImRenderCircleStyle* circleStyle = nullptr;
	m_pDrawList = ImGui::GetBackgroundDrawList();
	if (m_styleBaseMap.find(style) != m_styleBaseMap.end())
		circleStyle = m_styleBaseMap[style];
	else
		m_styleBaseMap[style] = circleStyle = new ImRenderCircleStyle(style);
	if (circleStyle->fill)
	{
		if (circleStyle->outline)
			m_pDrawList->AddCircleFilled(ImVec2(pos.x, pos.y), radius + 1.f, IM_COL32_BLACK);
		m_pDrawList->AddCircleFilled(pos, radius, ImGui::GetColorU32(color));
	}
	else
	{
		if (circleStyle->outline)
		{
			m_pDrawList->AddCircle(ImVec2(pos.x, pos.y), radius + 1.f, IM_COL32_BLACK, 0, circleStyle->thickness);
			m_pDrawList->AddCircle(ImVec2(pos.x, pos.y), radius - 1.f, IM_COL32_BLACK, 0, circleStyle->thickness);
		}
		m_pDrawList->AddCircle(pos, radius, ImGui::GetColorU32(color), 0, circleStyle->thickness);
	}
}
