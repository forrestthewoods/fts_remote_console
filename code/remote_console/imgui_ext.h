// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_IMGUI_EXT_H
#define FTS_IMGUI_EXT_H

// thirdparty libs
#include <imgui/imgui.h>

// lang
#include <climits>


namespace ImGui {
    void ColumnSeparator(float cursor_pad = 4.f); // Helper for drawing columns with Imgui

    struct ImGuiTextFuzzyFilter {
        struct Entry {
            void* data;
            const char* b;
            const char* e;
            int score;
            int id;

            Entry(void* _data, const char* _b, const char* _e, int _score, int _id) { data = _data; b = _b; e = _e; score = _score; id = _id; }
            Entry(Entry const & other) { data = other.data;  b = other.b; e = other.e; score = other.score; id = other.id; };

            bool valid() const { return score != INT_MAX; }
        };

        enum class FilterStyle : int
        {
            Simple,     // simple pass/fail fuzzy match
            Scored      // matches are scored and sorted
        };

        FilterStyle         Style;
        bool                Sorted;
        char                FilterBuf[256];
        ImVector<Entry>     Results;

        ImGuiTextFuzzyFilter(const char* default_filter = NULL);
        ~ImGuiTextFuzzyFilter() {}
        void Add(void* data, const char* text, const char* text_end = NULL);
        void Insert(void* data, const char* text, const char* text_end = NULL);
        void Clear();
        void Sort();
        int Score(const char* b, const char* e = NULL) const;

        bool DrawSearchBar();
        bool DrawStyleRadios();

    };
}

#endif // FTS_IMGUI_EXT_H