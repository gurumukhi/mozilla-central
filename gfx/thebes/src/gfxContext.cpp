/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Oracle Corporation code.
 *
 * The Initial Developer of the Original Code is Oracle Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2005
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Stuart Parmenter <pavlov@pavlov.net>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "gfxContext.h"

#include "gfxColor.h"
#include "gfxMatrix.h"
#include "gfxASurface.h"
#include "gfxPattern.h"

gfxContext::gfxContext()
{
    mCairo = cairo_create();
}
gfxContext::~gfxContext()
{
    cairo_destroy(mCairo);
}

void gfxContext::SetSurface(gfxASurface* surface)
{
    cairo_set_target_surface(mCairo, surface->CairoSurface());
}
gfxASurface* gfxContext::CurrentSurface()
{
    // XXX refcounting of get_target_surface may change
    cairo_surface_t *surface = cairo_current_target_surface(mCairo);
    return gfxASurface::LookupSurface(surface);
}

void gfxContext::Save()
{
    cairo_save(mCairo);
}

void gfxContext::Restore()
{
    cairo_restore(mCairo);
}

// drawing
void gfxContext::NewPath()
{
    cairo_new_path(mCairo);
}
void gfxContext::ClosePath()
{
    cairo_close_path(mCairo);
}

void gfxContext::Stroke()
{
    cairo_stroke(mCairo);
}
void gfxContext::Fill()
{
    cairo_fill(mCairo);
}

void gfxContext::MoveTo(gfxPoint pt)
{
    cairo_move_to(mCairo, pt.x, pt.y);
}
void gfxContext::LineTo(gfxPoint pt)
{
    cairo_line_to(mCairo, pt.x, pt.y);
}

void gfxContext::CurveTo(gfxPoint pt1, gfxPoint pt2, gfxPoint pt3)
{
    cairo_curve_to(mCairo, pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y);
}

void gfxContext::Arc(gfxPoint center, gfxFloat radius,
                     gfxFloat angle1, gfxFloat angle2)
{
    cairo_arc(mCairo, center.x, center.y, radius, angle1, angle2);
}

void gfxContext::Rectangle(gfxRect rect)
{
    cairo_rectangle(mCairo, rect.x, rect.y, rect.width, rect.height);
}

void gfxContext::Polygon(const gfxPoint points[], unsigned long numPoints)
{
    cairo_new_path(mCairo);
    cairo_move_to(mCairo, (gfxFloat)points[0].x, (gfxFloat)points[0].y);
    for (unsigned long i = 1; i < numPoints; ++i) {
        cairo_line_to(mCairo, (gfxFloat)points[i].x, (gfxFloat)points[i].y);
    }
}

void gfxContext::DrawSurface(gfxASurface *surface, gfxSize size)
{
    cairo_show_surface(mCairo, surface->CairoSurface(),
                       (int)size.width, (int)size.height);
}

// transform stuff
void gfxContext::Translate(gfxPoint pt)
{
    cairo_translate(mCairo, pt.x, pt.y);
}
void gfxContext::Scale(gfxFloat x, gfxFloat y)
{
    cairo_scale(mCairo, x, y);
}
void gfxContext::Rotate(gfxFloat angle)
{
    cairo_rotate(mCairo, angle);
}

void gfxContext::SetMatrix(const gfxMatrix& matrix)
{
    cairo_matrix_t *t = cairo_matrix_create();
    matrix.FillInCairoMatrix(t);
    cairo_set_matrix(mCairo, t); // this does a copy
    cairo_matrix_destroy(t);
}
gfxMatrix gfxContext::CurrentMatrix() const
{
    gfxMatrix matrix;
    cairo_matrix_t *t = cairo_matrix_create();
    cairo_current_matrix(mCairo, t);
    matrix = t;
    cairo_matrix_destroy(t);
    return matrix;
}



// properties
void gfxContext::SetColor(const gfxRGBA& c)
{
    cairo_set_rgb_color(mCairo, c.r, c.g, c.b);
    cairo_set_alpha(mCairo, c.a);
}
gfxRGBA gfxContext::CurrentColor() const
{
    gfxRGBA c;
    cairo_current_rgb_color(mCairo, &c.r, &c.b, &c.b);
    c.a = cairo_current_alpha(mCairo);
    return c;
}

void gfxContext::SetAntialiasMode(AntialiasMode mode)
{
    // XXX implement me
}

gfxContext::AntialiasMode gfxContext::CurrentAntialiasMode()
{
    return MODE_COVERAGE;
}

void gfxContext::SetDash(gfxFloat* dashes, int ndash, gfxFloat offset)
{
    cairo_set_dash(mCairo, dashes, ndash, offset);
}
//void getDash() const;

void gfxContext::SetLineWidth(gfxFloat width)
{
    cairo_set_line_width(mCairo, width);
}
gfxFloat gfxContext::CurrentLineWidth() const
{
    return cairo_current_line_width(mCairo);
}

void gfxContext::SetOperator(GraphicsOperator op)
{
    cairo_set_operator(mCairo, (cairo_operator_t)op);
}
gfxContext::GraphicsOperator gfxContext::CurrentOperator() const
{
    return (GraphicsOperator)cairo_current_operator(mCairo);
}

void gfxContext::SetLineCap(GraphicsLineCap cap)
{
    cairo_set_line_cap(mCairo, (cairo_line_cap_t)cap);
}
gfxContext::GraphicsLineCap gfxContext::CurrentLineCap() const
{
    return (GraphicsLineCap)cairo_current_line_cap(mCairo);
}

void gfxContext::SetLineJoin(GraphicsLineJoin join)
{
    cairo_set_line_join(mCairo, (cairo_line_join_t)join);
}
gfxContext::GraphicsLineJoin gfxContext::CurrentLineJoin() const
{
    return (GraphicsLineJoin)cairo_current_line_join(mCairo);
}


void gfxContext::SetMiterLimit(gfxFloat limit)
{
    cairo_set_miter_limit(mCairo, limit);
}
gfxFloat gfxContext::CurrentMiterLimit() const
{
    return cairo_current_miter_limit(mCairo);
}


// clipping
void gfxContext::Clip(const gfxRect& rect)
{
    cairo_new_path(mCairo);
    cairo_rectangle(mCairo, rect.x, rect.y, rect.width, rect.height);
    cairo_clip(mCairo);
}
void gfxContext::Clip(const gfxRegion& region)
{

}
void gfxContext::Clip()
{
    cairo_clip(mCairo);
}

void gfxContext::ResetClip()
{
    cairo_init_clip(mCairo);
}


// patterns
void gfxContext::SetPattern(gfxPattern& pattern)
{
    cairo_set_pattern(mCairo, pattern.CairoPattern());
}

// fonts?
void gfxContext::DrawString(gfxTextRun& text, int pos, int len)
{

}

// filters
void gfxContext::PushFilter(gfxFilter& filter, gfxRect& maxArea)
{

}

void gfxContext::PopFilter()
{

}
