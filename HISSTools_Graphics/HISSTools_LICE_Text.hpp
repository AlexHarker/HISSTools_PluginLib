
#pragma once

#include "cairo/cairo.h"
#include <lice.h>
#include <lice_text.h>
#include "mutex.h"
#include "ptrlist.h"
#include "swell.h"

#ifdef FillRect
#undef FillRect
#endif
#ifdef DrawText
#undef DrawText
#endif
#ifdef Polygon
#undef Polygon
#endif

struct HISSTools_Text
{
    char mFont[HT_FONT_LEN];
    int mSize, mCachedSize;
    enum EStyle { kStyleNormal, kStyleBold, kStyleItalic } mStyle;
    LICE_IFont *mCached;
    
    HISSTools_Text(int size = HT_DEFAULT_TEXT_SIZE, char* font = 0, EStyle style = kStyleNormal)
    :    mSize(size), mCachedSize(0), mStyle(style), mCached(0)
    {
        strcpy(mFont, (font ? font : HT_DEFAULT_FONT));
    }
    
    HISSTools_Text()
    :    mSize(HT_DEFAULT_TEXT_SIZE), mCachedSize(0), mStyle(kStyleNormal), mCached(0)
    {
        strcpy(mFont, HT_DEFAULT_FONT);
    }
};

// HISSTools_FontStorage is very similar to FontStorage from IPlug (but does not require IPlug)

class HISSTools_FontStorage
{
public:
    
    struct FontKey
    {
        int size;
        HISSTools_Text::EStyle style;
        char face[iplug::igraphics::FONT_LEN];
        LICE_IFont* font;
    };
    
    WDL_PtrList<FontKey> m_fonts;
    WDL_Mutex m_mutex;
    
    LICE_IFont* find(HISSTools_Text *pTxt)
    {
        WDL_MutexLock lock(&m_mutex);
        int i = 0, n = m_fonts.GetSize();
        for (i = 0; i < n; ++i)
        {
            FontKey* key = m_fonts.Get(i);
            if (key->size == pTxt->mCachedSize && key->style == pTxt->mStyle && !strcmp(key->face, pTxt->mFont))
                return key->font;
        }
        return 0;
    }
    
    void add(LICE_IFont* font, HISSTools_Text *pTxt)
    {
        WDL_MutexLock lock(&m_mutex);
        FontKey* key = m_fonts.Add(new FontKey);
        key->size = pTxt->mSize;
        key->style = pTxt->mStyle;
        strcpy(key->face, pTxt->mFont);
        key->font = font;
    }
    
    ~HISSTools_FontStorage()
    {
        int i, n = m_fonts.GetSize();
        for (i = 0; i < n; ++i)
        {
            delete(m_fonts.Get(i)->font);
        }
        m_fonts.Empty(true);
    }
};

static HISSTools_FontStorage HISSTools_fontCache;

// Drawing routines are also taken from IPlug, in order that we can know what the drawing rect for text is (for shadows)
// Fix - Clip Text or add option to do so...

struct HISSTools_LICE_Text
{
    static bool text(LICE_IBitmap *bitmap, HISSTools_Text *pTxt, const char *str, double x, double y, double w, double h, double scale, HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter)
    {
        if (!str || str[0] == '\0')
            return false;
        
        LICE_IFont* font = checkFont(pTxt, scale);
        
        if (!font)
            return false;
        
        LICE_pixel color = LICE_RGBA(255, 255, 255, 255);
        font->SetTextColor(color);
        
        UINT fmt = DT_NOCLIP;
        
        if (hAlign == kHAlignLeft)
            fmt |= DT_LEFT;
        else if (hAlign == kHAlignCenter)
            fmt |= DT_CENTER;
        else
            fmt |= DT_RIGHT;
        
        if (vAlign == kVAlignCenter)
            fmt |= DT_VCENTER;
        if (vAlign == kVAlignBottom)
            fmt |= DT_BOTTOM;
        
        fmt |= LICE_DT_NEEDALPHA;
        
        RECT pR;
        
        pR.left = floor(x * scale);
        pR.right = ceil((x + w) * scale);
        pR.top = floor(y * scale);
        pR.bottom = ceil((y + h) * scale);
        
        font->DrawText(bitmap, str, -1, &pR, fmt);
        
        return true;
    }
    
    static double getTextLineHeight(HISSTools_Text *pTxt)
    {
        LICE_IFont* font = checkFont(pTxt, 1.0);
        
        if (!font)
            return 0;
        
        return font->GetLineHeight();
    }
    
    static LICE_IFont* checkFont(HISSTools_Text *pTxt, double scale)
    {
        LICE_IFont* font = pTxt->mCached;
        
        if (!font || pTxt->mCachedSize != (int) (pTxt->mSize * scale))
        {
            pTxt->mCachedSize = pTxt->mSize * scale;
            font = cacheFont(pTxt);
        }
        
        return font;
    }
    
    static LICE_IFont *cacheFont(HISSTools_Text *pTxt)
    {
        LICE_CachedFont *font = (LICE_CachedFont *) HISSTools_fontCache.find(pTxt);
        
        if (!font)
        {
            font = new LICE_CachedFont;
            int h = pTxt->mCachedSize;
            int wt = (pTxt->mStyle == HISSTools_Text::kStyleBold ? FW_BOLD : FW_NORMAL);
            int it = (pTxt->mStyle == HISSTools_Text::kStyleItalic ? TRUE : FALSE);
            
#ifdef __APPLE__
            bool resized = FALSE;
            
        Resize:
            if (h < 2) h = 2;
#endif
            HFONT hFont = CreateFont(h, 0, 0, 0, wt, it, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, pTxt->mFont);
            if (!hFont)
            {
                delete(font);
                return 0;
            }
            font->SetFromHFont(hFont, LICE_FONT_FLAG_OWNS_HFONT | LICE_FONT_FLAG_FORCE_NATIVE);
#ifdef __APPLE__
            if (!resized && font->GetLineHeight() != h)
            {
                h = int((double)(h * h) / (double)font->GetLineHeight() + 0.5);
                resized = TRUE;
                goto Resize;
            }
#endif
            HISSTools_fontCache.add(font, pTxt);
        }
        pTxt->mCached = font;
        return font;
    }
};

static int getLineHeight(HISSTools_Text* txt) { return HISSTools_LICE_Text::getTextLineHeight(txt); }
