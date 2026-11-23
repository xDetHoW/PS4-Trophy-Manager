#include "trophy_backup.h"
#include <string.h>
#include <stdlib.h>

#define TM_BACKUP_LINE_MAX 1024
static const char k_hex[] = "0123456789ABCDEF";

static int write_hex(FILE* f, const char* s, size_t cap) {
    size_t n = 0;
    while (n < cap && s[n]) ++n;
    for (size_t i = 0; i < n; ++i) {
        unsigned char ch = (unsigned char)s[i];
        if (fputc(k_hex[ch >> 4], f) == EOF || fputc(k_hex[ch & 15], f) == EOF) return 0;
    }
    return 1;
}
static int hex_value(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}
static int read_hex(char* out, size_t cap, const char* in) {
    size_t n = strlen(in);
    if ((n & 1u) || n / 2 >= cap) return 0;
    for (size_t i = 0; i < n; i += 2) {
        int hi = hex_value((unsigned char)in[i]), lo = hex_value((unsigned char)in[i + 1]);
        if (hi < 0 || lo < 0) return 0;
        out[i / 2] = (char)((hi << 4) | lo);
        if (out[i / 2] == '\0') return 0;
    }
    out[n / 2] = '\0';
    return 1;
}
static int split_fields(char* line, char** fields, size_t expected) {
    size_t n = 0;
    char* p = line;
    while (n < expected) {
        fields[n++] = p;
        char* tab = strchr(p, '\t');
        if (!tab) break;
        *tab = '\0'; p = tab + 1;
    }
    if (n != expected || strchr(fields[expected - 1], '\t')) return 0;
    fields[expected - 1][strcspn(fields[expected - 1], "\r\n")] = '\0';
    return 1;
}
static int parse_uint(const char* s, unsigned long max, unsigned long* out) {
    if (!s || !*s) return 0;
    char* end = NULL;
    unsigned long v = strtoul(s, &end, 10);
    if (!end || *end || v > max) return 0;
    *out = v; return 1;
}

int tm_backup_write(FILE* f, const TmGame* games, size_t count) {
    if (!f || (!games && count) || count > TM_MAX_GAMES) return 0;
    if (fprintf(f, "PS4TM\t1\n") < 0) return 0;
    for (size_t i = 0; i < count; ++i) {
        const TmGame* g = &games[i];
        if (g->trophy_count > TM_MAX_TROPHIES_PER_GAME) return 0;
        if (fputs("G\t", f) == EOF || !write_hex(f, g->title_id, sizeof g->title_id) ||
            fputc('\t', f) == EOF || !write_hex(f, g->title, sizeof g->title) ||
            fprintf(f, "\t%u\n", (unsigned)g->trophy_count) < 0) return 0;
        for (uint16_t j = 0; j < g->trophy_count; ++j) {
            const TmTrophy* t = &g->trophies[j];
            if (t->grade < TM_TROPHY_BRONZE || t->grade > TM_TROPHY_PLATINUM ||
                t->earned > 1 || t->hidden > 1) return 0;
            if (fprintf(f, "T\t%u\t%u\t%u\t", (unsigned)t->grade,
                        (unsigned)t->earned, (unsigned)t->hidden) < 0 ||
                !write_hex(f, t->id, sizeof t->id) || fputc('\t', f) == EOF ||
                !write_hex(f, t->name, sizeof t->name) || fputc('\n', f) == EOF) return 0;
        }
    }
    return fputs("END\n", f) != EOF && fflush(f) == 0;
}

int tm_backup_read(FILE* f, TmGame* games, size_t capacity, size_t* read_count) {
    if (read_count) *read_count = 0;
    if (!f || !games || !read_count || capacity > TM_MAX_GAMES) return 0;
    char line[TM_BACKUP_LINE_MAX];
    if (!fgets(line, sizeof line, f) || strcmp(line, "PS4TM\t1\n") != 0) return 0;
    size_t count = 0;
    while (fgets(line, sizeof line, f)) {
        if (!strcmp(line, "END\n") || !strcmp(line, "END\r\n")) {
            *read_count = count; return 1;
        }
        if (count >= capacity) return 0;
        char* gf[4];
        if (!split_fields(line, gf, 4) || strcmp(gf[0], "G")) return 0;
        TmGame temp;
        memset(&temp, 0, sizeof temp);
        unsigned long n = 0;
        if (!read_hex(temp.title_id, sizeof temp.title_id, gf[1]) ||
            !read_hex(temp.title, sizeof temp.title, gf[2]) ||
            !parse_uint(gf[3], TM_MAX_TROPHIES_PER_GAME, &n)) return 0;
        temp.trophy_count = (uint16_t)n;
        for (uint16_t j = 0; j < temp.trophy_count; ++j) {
            if (!fgets(line, sizeof line, f)) return 0;
            char* tf[6]; unsigned long grade, earned, hidden;
            if (!split_fields(line, tf, 6) || strcmp(tf[0], "T") ||
                !parse_uint(tf[1], TM_TROPHY_PLATINUM, &grade) ||
                !parse_uint(tf[2], 1, &earned) || !parse_uint(tf[3], 1, &hidden) ||
                !read_hex(temp.trophies[j].id, sizeof temp.trophies[j].id, tf[4]) ||
                !read_hex(temp.trophies[j].name, sizeof temp.trophies[j].name, tf[5])) return 0;
            temp.trophies[j].grade = (TmTrophyGrade)grade;
            temp.trophies[j].earned = (uint8_t)earned;
            temp.trophies[j].hidden = (uint8_t)hidden;
            if (earned) temp.earned_count++;
        }
        games[count++] = temp;
    }
    return 0; /* missing END marker or truncated file */
}
