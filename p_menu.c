#include "g_local.h"

/**
 *
 */
void PMenu_Open(edict_t *ent, pmenu_t *entries, int cur, int num,
        qboolean dynamic) {
    pmenuhnd_t *hnd;
    pmenu_t *p;
    int i;

    if (!ent->client) {
        return;
    }

    //opening menu forces scoreboards down
    ent->client->showoldscores = false;
    ent->client->showscores = false;
    ent->client->showmotd = false;

    if (ent->client->pers.menu.active) {
        gi.dprintf("warning, ent already has a menu\n");
        PMenu_Close(ent);
    }

    hnd = &ent->client->pers.menu;
    hnd->active = true;
    hnd->dynamic = dynamic;
    hnd->entries = entries;
    hnd->num = num;

    if (cur < 0 || !entries[cur].SelectFunc) {
        for (i = 0, p = entries; i < num; i++, p++) {
            if (p->SelectFunc) {
                break;
            }
        }
    } else {
        i = cur;
    }

    if (i >= num) {
        hnd->cur = -1;
    } else {
        hnd->cur = i;
    }

    PMenu_Update(ent);
    gi.unicast(ent, true);
}

/**
 *
 */
void PMenu_Close(edict_t *ent) {
    if (!ent->client->pers.menu.active) {
        return;
    }

    if (ent->client->pers.menu.dynamic) {
        free(ent->client->pers.menu.entries);
    }

    ent->client->pers.menu.active = false;
    ent->client->pers.votemenu_values.show = false;
}

/**
 *
 */
void PMenu_Update(edict_t *ent) {
    char string[1400];
    int i;
    pmenu_t *p;
    int x;
    pmenuhnd_t *hnd;
    const char *t;
    qboolean alt = false;

    if (!ent->client->pers.menu.active) {
        gi.dprintf("warning:  ent has no menu\n");
        return;
    }

    hnd = &ent->client->pers.menu;

    strcpy(string, "xv 32 yv 8 picn inventory ");

    for (i = 0, p = hnd->entries; i < hnd->num; i++, p++) {
        if (!p->text || !*(p->text)) {
            continue; // blank line
        }

        t = p->text;
        if (*t == '*') {
            alt = true;
            t++;
        }

        sprintf(string + strlen(string), "yv %d ", 32 + i * 8);
        if (p->align == PMENU_ALIGN_CENTER) {
            x = 196 / 2 - strlen(t) * 4 + 64;
        } else if (p->align == PMENU_ALIGN_RIGHT) {
            x = 64 + (196 - strlen(t) * 8);
        } else {
            x = 64;
        }

        sprintf(string + strlen(string), "xv %d ",
                x - ((hnd->cur == i) ? 8 : 0));

        if (hnd->cur == i) {
            sprintf(string + strlen(string), "string2 \"\x0d%s\" ", t);
        } else if (alt) {
            sprintf(string + strlen(string), "string2 \"%s\" ", t);
        } else {
            sprintf(string + strlen(string), "string \"%s\" ", t);
        }
        alt = false;
    }

    gi.WriteByte(SVC_LAYOUT);
    gi.WriteString(string);
}

/**
 *
 */
void PMenu_Next(edict_t *ent) {
    pmenuhnd_t *hnd;
    int i;
    pmenu_t *p;

    if (!ent->client->pers.menu.active) {
        gi.dprintf("warning:  ent has no menu\n");
        return;
    }

    hnd = &ent->client->pers.menu;

    if (hnd->cur < 0) {
        return; // no selectable entries
    }

    i = hnd->cur;
    p = hnd->entries + hnd->cur;
    do {
        i++, p++;
        if (i == hnd->num) {
            i = 0, p = hnd->entries;
        }
        if (p->SelectFunc) {
            break;
        }
    } while (i != hnd->cur);

    hnd->cur = i;
    PMenu_Update(ent);
    gi.unicast(ent, true);
}

/**
 *
 */
void PMenu_Prev(edict_t *ent) {
    pmenuhnd_t *hnd;
    int i;
    pmenu_t *p;

    if (!ent->client->pers.menu.active) {
        gi.dprintf("warning:  ent has no menu\n");
        return;
    }

    hnd = &ent->client->pers.menu;

    if (hnd->cur < 0) {
        return; // no selectable entries
    }

    i = hnd->cur;
    p = hnd->entries + hnd->cur;
    do {
        if (i == 0) {
            i = hnd->num - 1;
            p = hnd->entries + i;
        } else
            i--, p--;
        if (p->SelectFunc) {
            break;
        }
    } while (i != hnd->cur);

    hnd->cur = i;
    PMenu_Update(ent);
    gi.unicast(ent, true);
}

/**
 *
 */
void PMenu_Select(edict_t *ent) {
    pmenuhnd_t *hnd;
    pmenu_t *p;

    if (!ent->client->pers.menu.active) {
        gi.dprintf("warning:  ent has no menu\n");
        return;
    }
    hnd = &ent->client->pers.menu;
    if (hnd->cur < 0) {
        return; // no selectable entries
    }
    p = hnd->entries + hnd->cur;
    if (p->SelectFunc) {
        p->SelectFunc(ent);
    }
}
