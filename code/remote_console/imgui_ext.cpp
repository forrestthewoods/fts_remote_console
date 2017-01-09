// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "imgui_ext.h"

// internal libs
#include <lib_fts/fts_fuzzy_match.h>

// thirdparty libs
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

// lang
#include <cstdlib>



namespace ImGui {

int EntryCompareScore(void const * _a, void const * _b) 
{
    ImGuiTextFuzzyFilter::Entry const * a = (ImGuiTextFuzzyFilter::Entry const *)_a;
    ImGuiTextFuzzyFilter::Entry const * b = (ImGuiTextFuzzyFilter::Entry const *)_b;

    if (a->score < b->score)
        return -1;
    else if (a->score == b->score)
        return 0;
    else
        return 1;
}

int EntryCompareId(void const * _a, void const * _b)
{
    ImGuiTextFuzzyFilter::Entry const * a = (ImGuiTextFuzzyFilter::Entry const *)_a;
    ImGuiTextFuzzyFilter::Entry const * b = (ImGuiTextFuzzyFilter::Entry const *)_b;

    if (a->id < b->id)
        return -1;
    else if (a->id == b->id)
        return 0;
    else
        return 1;
}


void ColumnSeparator(float cursor_pad) 
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();;
    draw_list->AddLine(ImVec2(p.x - 9999, p.y), ImVec2(p.x + 9999, p.y), ImGui::GetColorU32(ImGuiCol_Border));
    ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + cursor_pad));
}

ImGuiTextFuzzyFilter::ImGuiTextFuzzyFilter(const char* default_filter) 
    : Style(FilterStyle::Simple)
    , Sorted(true)
{
    if (default_filter) {
        ImStrncpy(FilterBuf, default_filter, IM_ARRAYSIZE(FilterBuf));
    }
    else
        FilterBuf[0] = 0;
}

void ImGuiTextFuzzyFilter::Add(void* data, const char* text, const char* text_end) 
{
    Entry new_entry(data, text, text_end, Score(text, text_end), Results.size());
    Results.push_back(new_entry);
    Sorted = false;
}

void ImGuiTextFuzzyFilter::Insert(void* data, const char* text, const char* text_end) {
    if (Style == FilterStyle::Simple || FilterBuf[0] == 0) {
        Add(data, text, text_end);
        return;
    }

    Entry new_entry(data, text, text_end, Score(text, text_end), Results.size());
    if (Sorted)
    {
        // Add invalid entries to the end
        if (!new_entry.valid()) 
        {
            Results.push_back(new_entry);
            return;
        }

        // Linear search for insertion point
        Entry const * low = Results.begin();
        Entry const * high = Results.end();
        Entry const * iter = nullptr;

        bool done = false;
        while (!done) {
            iter = low + (high - low) / 2;

            if (iter == Results.end())
                done = true;
            else if (iter->score < new_entry.score) {
                if (low == iter) {
                    iter = high;
                    done = true;
                }
                low = iter;
            }
            else if (iter == Results.begin() || (iter - 1)->score <= new_entry.score)
                done = true;
            else
                high = iter;
        }

        Results.insert(iter, new_entry);
    }
    else {
        Results.push_back(new_entry);
        Sort();
    }
}

void ImGuiTextFuzzyFilter::Clear() 
{
    Results.clear();
}

void ImGuiTextFuzzyFilter::Sort() 
{
    if (!Sorted)
    {
        if (Style == FilterStyle::Simple || FilterBuf == 0)
            qsort(Results.begin(), Results.size(), sizeof(Entry), EntryCompareId);
        else
            qsort(Results.begin(), Results.size(), sizeof(Entry), EntryCompareScore);
        Sorted = true;
    }
}

int ImGuiTextFuzzyFilter::Score(const char* b, const char* e) const 
{
    int score = INT_MAX;
    if (FilterBuf[0] == 0)
        score = 0;
    else if (Style == FilterStyle::Simple && fts::fuzzy_match(FilterBuf, b, e))
        score = 0;
    else if (Style == FilterStyle::Scored)
        fts::fuzzy_match(score, FilterBuf, b, e);
        
    return score;
}

bool ImGuiTextFuzzyFilter::DrawSearchBar()
{
    ImGui::Text("Search:");
    ImGui::SameLine();
    size_t buf_size = sizeof(FilterBuf) / sizeof(*FilterBuf);
    bool changed = ImGui::InputText("", FilterBuf, buf_size);
    if (changed)
    {
        for (int i = 0; i < Results.size(); ++i)
            Results[i].score = Score(Results[i].b, Results[i].e);

        if (Style == FilterStyle::Scored)
            Sorted = false;
    }

    return changed;
}

bool ImGuiTextFuzzyFilter::DrawStyleRadios()
{
    bool changed = false;
    int* style_ptr = (int*)(&Style);
    changed |= ImGui::RadioButton("Fuzzy Simple", style_ptr, (int)FilterStyle::Simple);
    changed |= ImGui::RadioButton("Fuzzy Scored", style_ptr, (int)FilterStyle::Scored);
    if (changed)
    {
        for (int i = 0; i < Results.size(); ++i)
            Results[i].score = Score(Results[i].b, Results[i].e);

        if (Style == FilterStyle::Scored)
            Sorted = false;
    }

    return changed;
}

} // namespace ImGui

#undef _CRT_SECURE_NO_WARNINGS
