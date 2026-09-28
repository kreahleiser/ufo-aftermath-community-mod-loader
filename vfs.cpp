#define WIN32_LEAN_AND_MEAN
#include "vfs.h"
#include "miniz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <vector>
#include <string>

#pragma pack(push, 1)
struct VfsHeader {
    float version;
    unsigned int clustersize;
    unsigned int clustercount;
    unsigned int maxrootfiles;
    unsigned int zero;
    unsigned int filenamelength;
    unsigned int windowsize;
    unsigned char md5[16];
    unsigned int versionstringlength;
    char versionstring[256];
    unsigned int usedclusters;
};
struct VfsFileEntry {
    char name[64];
    unsigned int unknown1;
    unsigned int type;
    unsigned int unknown2;
    unsigned int startcluster;
    unsigned int size;
    unsigned int size_uncompressed;
};
struct VfsFatEntry {
    unsigned int usage;
    unsigned int nextcluster;
};
#pragma pack(pop)

static void seterr(char *err, int n, const char *m)
{
    if (err && n > 0) {
        strncpy(err, m, n - 1);
        err[n - 1] = 0;
    }
}

static void split_path(const char *path, std::vector<std::string> *parts)
{
    char buf[512];
    strncpy(buf, path, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = 0;
    for (char *p = buf; *p; p++)
        if (*p == '/') *p = '\\';
    char *ctx = 0;
    char *tok = strtok_s(buf, "\\", &ctx);
    while (tok) {
        if (tok[0]) parts->push_back(tok);
        tok = strtok_s(0, "\\", &ctx);
    }
}

static int nameeq(const char *a, const char *b)
{
    return _stricmp(a, b) == 0;
}

struct Volume {
    FILE *f;
    VfsHeader hdr;
    std::vector<VfsFatEntry> fat;
    long long fat_off;
    long long root_off;
    long long clust_off;
};

static int vol_open(Volume *v, const char *path, char *err, int err_n)
{
    memset(v, 0, sizeof(*v));
    v->f = fopen(path, "rb");
    if (!v->f) { seterr(err, err_n, "cannot open vfs"); return 0; }
    if (fread(&v->hdr, sizeof(v->hdr), 1, v->f) != 1) {
        fclose(v->f); seterr(err, err_n, "bad vfs header"); return 0;
    }
    if (v->hdr.versionstringlength != 256) {
        fclose(v->f); seterr(err, err_n, "unexpected vfs header layout"); return 0;
    }
    v->fat_off = 308;
    v->root_off = v->fat_off + (long long)v->hdr.clustercount * 8;
    v->clust_off = v->root_off + (long long)v->hdr.maxrootfiles * 88;
    v->fat.resize(v->hdr.clustercount);
    _fseeki64(v->f, v->fat_off, SEEK_SET);
    if (fread(&v->fat[0], 8, v->hdr.clustercount, v->f) != v->hdr.clustercount) {
        fclose(v->f); seterr(err, err_n, "cannot read fat"); return 0;
    }
    return 1;
}

static int read_clusters(Volume *v, unsigned int start, unsigned int size, std::vector<unsigned char> *out)
{
    out->clear();
    out->reserve(size);
    unsigned int left = size;
    unsigned int cl = start;
    while (left && cl && cl != 0xFFFFFFFFu && cl <= v->hdr.clustercount) {
        unsigned int take = left < v->hdr.clustersize ? left : v->hdr.clustersize;
        _fseeki64(v->f, v->clust_off + (long long)(cl - 1) * v->hdr.clustersize, SEEK_SET);
        size_t at = out->size();
        out->resize(at + take);
        if (fread(&(*out)[at], 1, take, v->f) != take) return 0;
        left -= take;
        cl = v->fat[cl - 1].nextcluster;
    }
    return left == 0;
}

static int inflate_file(const unsigned char *src, unsigned int src_n, unsigned int uncomp, unsigned int window, std::vector<unsigned char> *out, char *err, int err_n)
{
    out->clear();
    out->reserve(uncomp);
    const unsigned char *p = src;
    const unsigned char *end = src + src_n;
    while ((unsigned)out->size() < uncomp && p + 4 <= end) {
        unsigned int n = (unsigned int)p[0] | ((unsigned int)p[1] << 8) | ((unsigned int)p[2] << 16) | ((unsigned int)p[3] << 24);
        p += 4;
        if (n == 0 || p + n > end) break;
        unsigned int room = uncomp - (unsigned)out->size();
        if (room > window) room = window;
        std::vector<unsigned char> chunk(room ? room : 1);
        mz_ulong dest = chunk.size();
        int st = mz_uncompress(chunk.data(), &dest, p, n);
        if (st != MZ_OK) {
            seterr(err, err_n, "zlib inflate failed");
            return 0;
        }
        out->insert(out->end(), chunk.begin(), chunk.begin() + dest);
        p += n;
    }
    if (out->size() != uncomp) {
        seterr(err, err_n, "decompressed size mismatch");
        return 0;
    }
    return 1;
}

static int parse_dir(const unsigned char *bytes, unsigned int size, std::vector<VfsFileEntry> *ents)
{
    unsigned int n = size / 88;
    ents->clear();
    for (unsigned int i = 0; i < n; i++) {
        VfsFileEntry e;
        memcpy(&e, bytes + i * 88, 88);
        if (e.name[0]) ents->push_back(e);
    }
    return 1;
}

static int find_entry(Volume *v, const std::vector<VfsFileEntry> &dir, const char *name, VfsFileEntry *out)
{
    for (size_t i = 0; i < dir.size(); i++) {
        if (nameeq(dir[i].name, name)) { *out = dir[i]; return 1; }
    }
    return 0;
}

int vfs_extract_file(const char *archive, const char *inner_path, const char *dest_dir, char *err, int err_n)
{
    Volume v;
    if (!vol_open(&v, archive, err, err_n)) return 0;

    std::vector<unsigned char> rootbytes;
    _fseeki64(v.f, v.root_off, SEEK_SET);
    rootbytes.resize(v.hdr.maxrootfiles * 88);
    if (fread(&rootbytes[0], 88, v.hdr.maxrootfiles, v.f) != v.hdr.maxrootfiles) {
        fclose(v.f); seterr(err, err_n, "cannot read root"); return 0;
    }
    std::vector<VfsFileEntry> dir;
    parse_dir(&rootbytes[0], v.hdr.maxrootfiles * 88, &dir);

    std::vector<std::string> parts;
    split_path(inner_path, &parts);
    if (parts.empty()) { fclose(v.f); seterr(err, err_n, "empty path"); return 0; }

    VfsFileEntry e;
    for (size_t i = 0; i < parts.size(); i++) {
        if (!find_entry(&v, dir, parts[i].c_str(), &e)) {
            fclose(v.f); seterr(err, err_n, "path not found in vfs"); return 0;
        }
        int last = (i + 1 == parts.size());
        if (e.type == 2) {
            if (last) { fclose(v.f); seterr(err, err_n, "path is a directory"); return 0; }
            std::vector<unsigned char> db;
            if (!read_clusters(&v, e.startcluster, e.size, &db)) {
                fclose(v.f); seterr(err, err_n, "cannot read directory"); return 0;
            }
            parse_dir(&db[0], e.size, &dir);
        } else if (e.type == 1 || e.type == 9) {
            if (!last) { fclose(v.f); seterr(err, err_n, "file in middle of path"); return 0; }
            std::vector<unsigned char> raw;
            if (!read_clusters(&v, e.startcluster, e.size, &raw)) {
                fclose(v.f); seterr(err, err_n, "cannot read file clusters"); return 0;
            }
            std::vector<unsigned char> data;
            if (e.type == 9) {
                unsigned int un = e.size_uncompressed ? e.size_uncompressed : e.size;
                if (!inflate_file(&raw[0], e.size, un, v.hdr.windowsize ? v.hdr.windowsize : 50000, &data, err, err_n)) {
                    fclose(v.f); return 0;
                }
            } else {
                data.swap(raw);
            }
            CreateDirectoryA(dest_dir, 0);
            char outp[MAX_PATH];
            _snprintf(outp, MAX_PATH, "%s\\%s", dest_dir, parts.back().c_str());
            FILE *o = fopen(outp, "wb");
            if (!o) { fclose(v.f); seterr(err, err_n, "cannot write extract dest"); return 0; }
            fwrite(&data[0], 1, data.size(), o);
            fclose(o);
            fclose(v.f);
            return 1;
        } else {
            fclose(v.f); seterr(err, err_n, "unknown vfs entry type"); return 0;
        }
    }
    fclose(v.f);
    seterr(err, err_n, "extract failed");
    return 0;
}

struct PackFile {
    std::string rel; /* using backslashes, no leading slash */
    std::string abs;
    int is_dir;
    std::vector<unsigned char> data;
    unsigned int start;
    unsigned int size;
};

static void walk_dir(const char *root, const char *rel, std::vector<PackFile> *files)
{
    char pattern[MAX_PATH];
    if (rel[0]) _snprintf(pattern, MAX_PATH, "%s\\%s\\*", root, rel);
    else _snprintf(pattern, MAX_PATH, "%s\\*", root);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (fd.cFileName[0] == '.' && (fd.cFileName[1] == 0 || (fd.cFileName[1] == '.' && fd.cFileName[2] == 0)))
            continue;
        PackFile pf;
        pf.is_dir = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
        if (rel[0]) pf.rel = std::string(rel) + "\\" + fd.cFileName;
        else pf.rel = fd.cFileName;
        char abs[MAX_PATH];
        _snprintf(abs, MAX_PATH, "%s\\%s", root, pf.rel.c_str());
        pf.abs = abs;
        files->push_back(pf);
        if (pf.is_dir) walk_dir(root, pf.rel.c_str(), files);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

static std::string parent_rel(const std::string &rel)
{
    size_t p = rel.rfind('\\');
    if (p == std::string::npos) return "";
    return rel.substr(0, p);
}

static std::string base_name(const std::string &rel)
{
    size_t p = rel.rfind('\\');
    if (p == std::string::npos) return rel;
    return rel.substr(p + 1);
}

int vfs_pack_dir(const char *src_dir, const char *out_archive, char *err, int err_n)
{
    std::vector<PackFile> files;
    walk_dir(src_dir, "", &files);
    if (files.empty()) { seterr(err, err_n, "nothing to pack"); return 0; }

    for (size_t i = 0; i < files.size(); i++) {
        if (files[i].is_dir) continue;
        FILE *f = fopen(files[i].abs.c_str(), "rb");
        if (!f) { seterr(err, err_n, "cannot read file to pack"); return 0; }
        fseek(f, 0, SEEK_END);
        long n = ftell(f);
        fseek(f, 0, SEEK_SET);
        files[i].data.resize(n > 0 ? n : 0);
        if (n > 0) fread(&files[i].data[0], 1, n, f);
        fclose(f);
        files[i].size = (unsigned)files[i].data.size();
    }

    /* ensure parent dirs exist as pack entries */
    std::vector<PackFile> extra;
    for (size_t i = 0; i < files.size(); i++) {
        std::string p = parent_rel(files[i].rel);
        while (!p.empty()) {
            int found = 0;
            for (size_t j = 0; j < files.size(); j++)
                if (files[j].is_dir && files[j].rel == p) { found = 1; break; }
            for (size_t j = 0; j < extra.size(); j++)
                if (extra[j].rel == p) { found = 1; break; }
            if (!found) {
                PackFile d;
                d.rel = p;
                d.is_dir = 1;
                extra.push_back(d);
            }
            p = parent_rel(p);
        }
    }
    files.insert(files.end(), extra.begin(), extra.end());

    const unsigned int CS = 4096;
    unsigned int clusters_needed = 1; /* start at 1 */
    for (size_t i = 0; i < files.size(); i++) {
        unsigned int bytes = files[i].is_dir ? 0 : files[i].size;
        /* dirs filled later */
        (void)bytes;
    }

    /* assign clusters after dir contents known — two-pass: files first, then dirs from leaves */
    /* compute dir payloads */
    /* map rel -> index */
    auto find_idx = [&](const std::string &rel) -> int {
        for (size_t i = 0; i < files.size(); i++)
            if (files[i].rel == rel) return (int)i;
        return -1;
    };

    /* children lists */
    std::vector<std::vector<int> > kids(files.size());
    std::vector<int> roots;
    for (size_t i = 0; i < files.size(); i++) {
        std::string p = parent_rel(files[i].rel);
        if (p.empty()) roots.push_back((int)i);
        else {
            int pi = find_idx(p);
            if (pi >= 0) kids[pi].push_back((int)i);
        }
    }

    auto fill_dir = [&](int di) {
        PackFile &d = files[di];
        d.data.clear();
        for (size_t k = 0; k < kids[di].size(); k++) {
            PackFile &ch = files[kids[di][k]];
            VfsFileEntry e;
            memset(&e, 0, sizeof(e));
            strncpy(e.name, base_name(ch.rel).c_str(), 63);
            e.unknown1 = 0;
            e.type = ch.is_dir ? 2u : 1u;
            e.unknown2 = 0xFFFFFFFFu;
            e.startcluster = ch.start;
            e.size = ch.is_dir ? (unsigned)ch.data.size() : ch.size;
            e.size_uncompressed = 0;
            unsigned char raw[88];
            memcpy(raw, &e, 88);
            d.data.insert(d.data.end(), raw, raw + 88);
        }
        d.size = (unsigned)d.data.size();
    };

    /* children must have start clusters before parent dir is serialized.
       assign file clusters first, then dirs bottom-up. */
    unsigned int next = 1;
    auto clusters_for = [&](unsigned int bytes) -> unsigned int {
        if (bytes == 0) return 1;
        return (bytes + CS - 1) / CS;
    };

    for (size_t i = 0; i < files.size(); i++) {
        if (files[i].is_dir) continue;
        files[i].start = next;
        next += clusters_for(files[i].size);
    }

    /* dirs: deepest first */
    std::vector<int> dir_order;
    for (size_t i = 0; i < files.size(); i++)
        if (files[i].is_dir) dir_order.push_back((int)i);
    for (size_t a = 0; a < dir_order.size(); a++)
        for (size_t b = a + 1; b < dir_order.size(); b++)
            if (files[dir_order[a]].rel.size() < files[dir_order[b]].rel.size()) {
                int t = dir_order[a]; dir_order[a] = dir_order[b]; dir_order[b] = t;
            }
    for (size_t i = 0; i < dir_order.size(); i++) {
        fill_dir(dir_order[i]);
        files[dir_order[i]].start = next;
        next += clusters_for(files[dir_order[i]].size);
    }

    /* root directory entries */
    std::vector<VfsFileEntry> rootents;
    for (size_t i = 0; i < roots.size(); i++) {
        PackFile &ch = files[roots[i]];
        VfsFileEntry e;
        memset(&e, 0, sizeof(e));
        strncpy(e.name, base_name(ch.rel).c_str(), 63);
        e.type = ch.is_dir ? 2u : 1u;
        e.unknown2 = 0xFFFFFFFFu;
        e.startcluster = ch.start;
        e.size = ch.is_dir ? (unsigned)ch.data.size() : ch.size;
        rootents.push_back(e);
    }

    unsigned int cc = next + 16;
    if (cc < 64) cc = 64;
    unsigned int mrf = 64;
    if (rootents.size() > mrf) { seterr(err, err_n, "too many root files"); return 0; }

    std::vector<VfsFatEntry> fat(cc);
    memset(&fat[0], 0, cc * 8);
    auto chain = [&](unsigned int start, unsigned int bytes) {
        unsigned int n = clusters_for(bytes);
        for (unsigned int i = 0; i < n; i++) {
            unsigned int idx = start + i - 1;
            fat[idx].usage = 1;
            fat[idx].nextcluster = (i + 1 < n) ? (start + i + 1) : 0xFFFFFFFFu;
        }
    };
    for (size_t i = 0; i < files.size(); i++)
        chain(files[i].start, files[i].is_dir ? files[i].size : files[i].size);

    VfsHeader hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.version = 1.0f;
    hdr.clustersize = CS;
    hdr.clustercount = cc;
    hdr.maxrootfiles = mrf;
    hdr.filenamelength = 64;
    hdr.windowsize = 50000;
    hdr.versionstringlength = 256;
    strncpy(hdr.versionstring, "UFO:AM RELEASE 1.4", 255);
    hdr.usedclusters = next - 1;

    FILE *o = fopen(out_archive, "wb");
    if (!o) { seterr(err, err_n, "cannot create overlay vfs"); return 0; }
    fwrite(&hdr, sizeof(hdr), 1, o);
    fwrite(&fat[0], 8, cc, o);
    std::vector<unsigned char> rootbuf(mrf * 88, 0);
    for (size_t i = 0; i < rootents.size(); i++)
        memcpy(&rootbuf[i * 88], &rootents[i], 88);
    fwrite(&rootbuf[0], 1, rootbuf.size(), o);

    std::vector<unsigned char> cluster(CS, 0);
    auto write_blob = [&](unsigned int start, const unsigned char *data, unsigned int size) {
        unsigned int n = clusters_for(size);
        for (unsigned int i = 0; i < n; i++) {
            memset(&cluster[0], 0, CS);
            unsigned int off = i * CS;
            unsigned int take = 0;
            if (off < size) {
                take = size - off;
                if (take > CS) take = CS;
                memcpy(&cluster[0], data + off, take);
            }
            fwrite(&cluster[0], 1, CS, o);
        }
        (void)start;
    };

    /* clusters must be written in cluster-index order 1..used */
    std::vector<PackFile *> by_start;
    for (size_t i = 0; i < files.size(); i++) by_start.push_back(&files[i]);
    for (size_t a = 0; a < by_start.size(); a++)
        for (size_t b = a + 1; b < by_start.size(); b++)
            if (by_start[a]->start > by_start[b]->start) {
                PackFile *t = by_start[a]; by_start[a] = by_start[b]; by_start[b] = t;
            }
    unsigned int expect = 1;
    for (size_t i = 0; i < by_start.size(); i++) {
        PackFile *pf = by_start[i];
        while (expect < pf->start) {
            memset(&cluster[0], 0, CS);
            fwrite(&cluster[0], 1, CS, o);
            expect++;
        }
        const unsigned char *data = pf->data.empty() ? (const unsigned char *)"" : &pf->data[0];
        write_blob(pf->start, data, pf->size);
        expect = pf->start + clusters_for(pf->size);
    }
    while (expect <= cc) {
        memset(&cluster[0], 0, CS);
        fwrite(&cluster[0], 1, CS, o);
        expect++;
    }
    fclose(o);
    return 1;
}
