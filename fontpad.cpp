#define WIN32_LEAN_AND_MEAN
#include "fontpad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

static void seterr(char *err, int n, const char *m)
{
    if (err && n > 0) {
        strncpy(err, m, n - 1);
        err[n - 1] = 0;
    }
}

struct Box { int x0, y0, x1, y1; };

static int load_boxes(const char *cfg, std::vector<Box> *boxes, char *err, int err_n)
{
    FILE *f = fopen(cfg, "r");
    if (!f) { seterr(err, err_n, "cannot open font cfg"); return 0; }
    char line[2048];
    int in_chars = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "CHARS", 5) == 0) { in_chars = 1; continue; }
        if (strncmp(line, "END_OF_CHARS", 12) == 0) break;
        if (!in_chars) continue;
        int idx, a, b, c, d;
        if (sscanf(line, "CHAR %d %d %d %d %d", &idx, &a, &b, &c, &d) >= 5) {
            Box box;
            box.x0 = a < c ? a : c;
            box.x1 = a < c ? c : a;
            box.y0 = b < d ? b : d;
            box.y1 = b < d ? d : b;
            if (box.x1 > box.x0 && box.y1 > box.y0)
                boxes->push_back(box);
        }
    }
    fclose(f);
    return boxes->empty() ? 0 : 1;
}

int pad_font_atlas(const char *tga_path, const char *cfg_path, char *err, int err_n)
{
    std::vector<Box> boxes;
    if (!load_boxes(cfg_path, &boxes, err, err_n)) {
        seterr(err, err_n, "no CHAR boxes in font cfg");
        return 0;
    }
    FILE *f = fopen(tga_path, "rb");
    if (!f) { seterr(err, err_n, "cannot open font tga"); return 0; }
    unsigned char hdr[18];
    if (fread(hdr, 1, 18, f) != 18) { fclose(f); seterr(err, err_n, "bad tga header"); return 0; }
    int idlen = hdr[0];
    int ctype = hdr[2];
    int w = hdr[12] | (hdr[13] << 8);
    int h = hdr[14] | (hdr[15] << 8);
    int bpp = hdr[16];
    if (ctype != 2 || (bpp != 24 && bpp != 32) || w <= 0 || h <= 0) {
        fclose(f);
        seterr(err, err_n, "unsupported font tga");
        return 0;
    }
    int spp = bpp / 8;
    if (idlen) fseek(f, idlen, SEEK_CUR);
    size_t pixn = (size_t)w * (size_t)h;
    std::vector<unsigned char> px(pixn * spp);
    if (fread(&px[0], spp, pixn, f) != pixn) {
        fclose(f);
        seterr(err, err_n, "truncated font tga");
        return 0;
    }
    std::vector<unsigned char> footer;
    {
        unsigned char buf[64];
        size_t n;
        while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
            footer.insert(footer.end(), buf, buf + n);
    }
    fclose(f);

    std::vector<unsigned char> out = px;
    auto black = [&](int x, int y) -> int {
        if ((unsigned)x >= (unsigned)w || (unsigned)y >= (unsigned)h) return 1;
        size_t o = ((size_t)y * w + x) * spp;
        return px[o] < 8 && px[o + 1] < 8 && px[o + 2] < 8;
    };
    auto copy_px = [&](int sx, int sy, int dx, int dy) {
        if ((unsigned)sx >= (unsigned)w || (unsigned)sy >= (unsigned)h) return;
        if ((unsigned)dx >= (unsigned)w || (unsigned)dy >= (unsigned)h) return;
        if (black(sx, sy) || !black(dx, dy)) return;
        size_t s = ((size_t)sy * w + sx) * spp;
        size_t d = ((size_t)dy * w + dx) * spp;
        for (int i = 0; i < spp; i++) out[d + i] = px[s + i];
    };

    /* Only paint the 1px frame *outside* each glyph box. Dilating inside
       the box fills holes in A/O/8 and turns 10px fonts into blobs. */
    for (size_t i = 0; i < boxes.size(); i++) {
        Box b = boxes[i];
        if (b.x0 < 0) b.x0 = 0;
        if (b.y0 < 0) b.y0 = 0;
        if (b.x1 > w) b.x1 = w;
        if (b.y1 > h) b.y1 = h;
        if (b.x1 - b.x0 < 2 || b.y1 - b.y0 < 2) continue;
        for (int x = b.x0; x < b.x1; x++) {
            copy_px(x, b.y0, x, b.y0 - 1);
            copy_px(x, b.y1 - 1, x, b.y1);
        }
        for (int y = b.y0; y < b.y1; y++) {
            copy_px(b.x0, y, b.x0 - 1, y);
            copy_px(b.x1 - 1, y, b.x1, y);
        }
    }

    f = fopen(tga_path, "wb");
    if (!f) { seterr(err, err_n, "cannot rewrite font tga"); return 0; }
    fwrite(hdr, 1, 18, f);
    fwrite(&out[0], spp, pixn, f);
    if (!footer.empty()) fwrite(&footer[0], 1, footer.size(), f);
    fclose(f);
    return 1;
}
