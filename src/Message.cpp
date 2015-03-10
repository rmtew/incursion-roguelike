/* MESSAGE.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Includes all of the code for formatting and dispatching
   messages to different objects, and for handling grammar
   issues. Also implements the message queue system.

     const char* Something(int16 Flags)
     const char* __XPrint(Player *POV, const char *msg,va_list args)
     const char* XPrint(const char* msg,...)
     void DPrint(EventInfo &e, const char* msg1, const char* msg2,...)
     void APrint(EventInfo &e, const char* msg,...)
     void VPrint(EventInfo &e, const char* msg1, const char* msg2,...)
     void TPrint(EventInfo &e, const char* msg1, const char* msg2, const char* msg3,...)
     void Map::SetQueue(int16 Queue)
     void Map::UnsetQueue(int16 Queue) 
     void Map::PrintQueue(int16 Queue)
     void Map::EmptyQueue(int16 Queue)
     void Player::DumpQueue(int16 Queue)
     void Player::EmptyQueue(int16 Queue)
     void Player::__IPrint(const char*msg,va_list ap)
     void Player::IPrint(const char*msg,...)
     void Player::IDPrint(const char*msg1,const char*msg2,...)
     void Thing::IDPrint(const char*msg1,const char*msg2,...)
     String Dice::Str()
     String Map::Name(int16 Flags)
     String Creature::Name(int16 Flags)
     String Trap::Name(int16 Flags)
     String Item::Name(int16 Flags)
     String Feature::Name(int16 Flags)
     String Corpse::Name(int16 Flags)

*/

#include "Incursion.h"
#include <stdarg.h>

static int16 xprint_lev = 0;
char  XPrintBuff1[130000];
char  XPrintBuff2[130000];
char  XPrintBuff3[4096];
char  XPrintBuff4[4096];
char  XPrintBuff5[4096];

EventInfo *ev;

int16 Silence;

int HelpEffectSort(const void *a, const void *b);
int HelpResourceSort(const void *a, const void *b);

void HelpWeaponTable(String &helpText);

String & BoxIt(String & Header,int16 col, int16 col2);

const char* Something(int16 Flags) {
    /* This makes no sense.  If something you Perceive, but do not XPerceive does something and the message is
       "The <obj>..." then you get "It..." and it makes little sense.
    if (Flags & NA_THE) {
    if (Flags & NA_POSS)
    return Flags & NA_CAPS ? "Its" : "its";
    return Flags & NA_CAPS ? "It" : "it";
    }
    */
    if (Flags & NA_POSS)
        return Flags & NA_CAPS ? "Its" : "its";
    return Flags & NA_CAPS ? "Something" : "something";
}

const char* __XPrint(Player *POV, const char *msg, va_list args) {
    char Tag1[24], Tag2[24];
    const char *cc, *ts;
    char *p, *tag, *Out;
    int32 n = 0, i, j;
    EventInfo *oev;
    String str, l;
    Thing* Subject;
    uint16 Flags;

    union {
        int32 i;
        const char *s;
        Thing *o;
    }
    Params[10];


    switch (++xprint_lev) {
    case 1:
        Out = XPrintBuff1;
        break;
    case 2:
        Out = XPrintBuff2;
        break;
    case 3:
        Out = XPrintBuff3;
        break;
    case 4:
        Out = XPrintBuff4;
        break;
    case 5:
        Out = XPrintBuff5;
        break;
    default:
        Error("XPrint recursion is too deep!");
        --xprint_lev;
        return "(XPrint Error)";
    }

    cc = msg;
    p = Out;
    while (*cc) {
        if (p >= Out + (xprint_lev < 3 ? 129000 : 4000))
            break;

        if (*cc == '<' && (cc == msg || cc == (msg + 1) || (*(cc - 2) != LITERAL_CHAR))) {
            tag = Tag1;
            ts = cc;
            cc++;
            Tag2[0] = 0;
Repeat:
            i = 0;
            while (*cc && *cc != ':' && *cc != '>' && i != 23) {
                *tag++ = tolower(*cc++);
                i++;
            }
            if (i == 23)
                Fatal("Formatted string with bad tag!");
            *tag = 0;
            if (!*cc)
                Fatal("Null terminator inside tag!");

            if (*cc == ':') {
                tag = Tag2;
                cc++;
                goto Repeat;
            }
            cc++;

            if (isdigit(Tag1[0])) {
                *p++ = -atoi(Tag1);
                continue;
            }

            if (Tag1[0] == 's' && Tag1[1] == 't' && Tag1[2] == 'r') {
                if (!Tag1[3])
                    i = n + 1;
                else
                    i = atoi(&(Tag1[3]));

                while (i > n) {
                    n++;
                    Params[n].s = va_arg(args, const  char*);
                }

                if (Params[i].s == NULL)
                    strcpy(p, "[null string]");
                else
                    strcpy(p, Params[i].s);

                p = Out + strlen(Out);
                continue;
            }

            if (Tag1[0] == 'n' && Tag1[1] == 'u' && Tag1[2] == 'm') {
                if (!Tag1[3])
                    i = n + 1;
                else
                    i = atoi(&(Tag1[3]));

                while (i > n) {
                    n++;
                    Params[n].i = va_arg(args, int);
                }

                strcpy(p, Format("%d", Params[i].i));
                p = Out + strlen(Out);
                continue;
            }

            if (Tag1[0] == 'c' && Tag1[1] == 'h' && Tag1[2] == 'a' && Tag1[3] == 'r') {
                if (!Tag2[0]) {
                    Error("<char:XXX> field in string without XXX.");
                    continue;
                }

                i = atoi(Tag2);
                if (!i)
                    i = LookupStr(GLYPH_CONSTNAMES, Tag2);
                if (!i)
                    Error("Cannot identify XXX field in <char:XXX> in formatted string!");

                // We need to encode the potentially larger than 8 bit glyph code, in the string.  It is currently 12 bits, and
                // will never grow larger.  So it's safe to encode it in two bytes.  The first byte may be 0x00, and should never be
                // 0xFF, so we invert it to prevent it signalling an end of string.
                strcpy(p, Format("%c%c%c", LITERAL_CHAR, LITERAL_CHAR1(i), LITERAL_CHAR2(i)));
                p = Out + strlen(Out);
                continue;
            }

            if (Tag1[0] == 'h' && Tag1[1] == 't') {
                for (j = 0; isalpha(Tag1[j]); j++)
                    ;

                if (!Tag1[j])
                    i = n + 1;
                else
                    i = atoi(&(Tag1[j]));

                while (i > n) {
                    n++;
                    Params[n].i = va_arg(args, hText);
                }

                if (Params[n].i < 0)
                    strcpy(p, theGame->VM.getStringSafe(Params[n].i));
                else
                    strcpy(p, theGame->Modules[theGame->VM.mn]->GetText(Params[i].i));

                p = Out + strlen(Out);
                continue;
            }

            if (Tag1[0] == 'r' && ((Tag1[1] == 'i' && Tag1[2] == 'd') || (Tag1[1] == 'e' && Tag1[2] == 's'))) {
                if (!Tag1[3])
                    i = n + 1;
                else
                    i = atoi(&(Tag1[3]));

                while (i > n) {
                    n++;
                    Params[n].i = va_arg(args, rID);
                }

                strcpy(p, NAME(Params[i].i));
                p = Out + strlen(Out);
                continue;
            } else if (str.Left(4) == "hstr") {
                if (!str[4])
                    i = n + 1;
                else
                    i = atoi(str.Right(str.GetLength() - 4));

                while (i > n) {
                    n++;
                    Params[n].i = va_arg(args, hObj);
                }

                /* strcpy(p,theGame->VM.GetString(Params[i].i)); */
                p = Out + strlen(Out);
                continue;
            } else if (!stricmp(Tag1, "list tattoos")) {
                int16 n, c;
                rID tID;
                TEffect *Tats[64];

                l.Empty();
                c = 0;
                for (n = 0; n != theGame->LastSpell(); n++) {
                    tID = theGame->SpellID(n);
                    if (!TEFF(tID)->HasSource(AI_TATTOO))
                        continue;
                    Tats[c++] = TEFF(tID);
                }

                Tats[c] = NULL;
                qsort(Tats, c, sizeof(Tats[0]), HelpEffectSort);
                for (n = 0; n != c; n++) {
                    l += Tats[n]->Describe(POV);
                    l += "\n\n";
                }

                strcpy(p, XPrint(l));
                p = Out + strlen(Out);
                continue;
            } else if (!stricmp(Tag1, "list macros")) {
                int16 n, c;
                String l;
                rID mID, Macs[64];

                l.Empty();
                c = 0;
                for (n = 0; n != theGame->LastSpell(); n++) {
                    mID = theGame->SpellID(n);
                    if (!TEFF(mID)->HasSource(AI_MACRO))
                        continue;
                    Macs[c++] = mID;
                }

                Macs[c] = 0;
                qsort(Macs, c, sizeof(Macs[0]), HelpResourceSort);
                for (n = 0; n != c; n++) {
                    l += Format("<13>%s<5>\n__%s\n\n",
                        (const char*)Upper(NAME(Macs[n])),
                        (const char*)DESC(Macs[n]));
                }

                strcpy(p, XPrint(l));
                p = Out + strlen(Out);
                continue;
            } else if (!stricmp(Tag1, "list options")) {
                TextVal CatDescs[] = {
                    { OPC_CHARGEN, "<15>CHARACTER GENERATION OPTIONS<5>\n"
                    "__Character generation options determine how an Incursion character "
                    "is generated, covering the rolling of Attributes and certain defaults "
                    "such as gender. Character generation options are specific to a certain "
                    "character and cannot be changed in gameplay -- they must be set before "
                    "the character is created and are thereafter fixed. As such, this "
                    "category also includes some switches that affect gameplay after "
                    "character creation, but should not be changed once the game has been "
                    "begun.\n\n" },
                    { OPC_INPUT, "<15>INPUT OPTIONS<5>\n"
                    "__Input options allow a player to customize the manner in which they "
                    "issue commands to Incursion, including an alternate keyset as well as "
                    "various automatic functions and user-interface preferences.\n\n" },
                    { OPC_LIMITS, "<15>LIMITS AND WARNINGS<5>\n"
                    "__The Limits and Warnings options specify when and how the player will be "
                    "warned about certain dangerous conditions, and also define exactly when "
                    "the player will stop an extended run-action.\n\n" },
                    { OPC_DISPLAY, "<15>OUTPUT OPTIONS<5>\n"
                    "__The Output options describe how the game map is rendered on the screen, "
                    "how glyphs representing creatures will be coloured to convey information "
                    "to the player, which dice rolls will be displayed where and how status "
                    "information and messages will be shown onscreen.\n\n" },
                    { OPC_TACTICAL, "<15>TACTICS AND COMBAT OPTIONS<5>\n"
                    "__The Tactics and Combat options serve to automate certain choices "
                    "made automatically by Incursion of behalf of your character with "
                    "regards to the game's combat rules, such as how you prefer to make "
                    "attacks of opportunity or use counterspells. Tactics and Combat "
                    "options are saved with a specific character and set to system defaults "
                    "when a new character is created, because the way these options are "
                    "set usually depends on how the player chose to build her character.\n\n" },
                    { OPC_WIZARD, "<15>WIZARD MODE OPTIONS<5>\n"
                    "__The Wizard Mode options allow a player using the game in Wizard "
                    "Mode to cheat, removing threats or altering the normal nature of "
                    "Incursion's gameplay. Wizard Mode can be used to test and debug "
                    "the game, to learn about its exact rules, to play an easier game "
                    "than normal or to experiment with unusual situations.\n\n" },
                    { 0, NULL } };

                int16 i, j;
                String opts;

                opts = "";
                for (i = 0; CatDescs[i].Val; i++) {
                    opts += CatDescs[i].Text;

                    for (j = 0; OptionList[j].Val; j++)
                        if (OptionList[j].Val >= CatDescs[i].Val)
                            if (OptionList[j].Val <= CatDescs[i].Val + 99) {
                                opts += "<13>";
                                opts += Capitalize(OptionList[j].Name, true);
                                opts += "\n<5>";
                                opts += Decolorize(XPrint(OptionList[j].Desc));
                                opts += "\n\n";
                            }

                    strcpy(p, XPrint(opts));
                    p = Out + strlen(Out);
                    opts = "";
                }

                continue;
            } else if (!stricmp(Tag1, "weapon table")) {
                String l;

                l = "";
                HelpWeaponTable(l);
                strcpy(p, l);
                p = Out + strlen(Out);
                continue;
            }

            Flags = 0;
            oev = ev;
            if (cc[0] && cc[1] && cc[0] == '\'' && cc[1] == 's') {
                cc += 2;
                Flags |= NA_POSS;
            } else if (cc[0] && cc[0] == '\'') {
                cc++;
                Flags |= NA_POSS;
            }

            if (ts - msg >= 2)
                if (ts[-1] == ' ' && ts[-2] == '.')
                    Flags |= NA_CAPS;
            if (ts - msg <= 0)
                Flags |= NA_CAPS;

            if (ts - msg == 2)
                if (ts[-2] == 'a' && ts[-1] == ' ') {
                    Flags |= NA_A + NA_CAPS;
                    p -= 2;
                    goto PutName;
                }
            if (ts - msg == 2)
                if (ts[-2] == 'A' && ts[-1] == ' ') {
                    Flags |= NA_A + NA_CAPS;
                    p -= 2;
                    goto PutName;
                }
            if (ts - msg == 3)
                if (ts[-3] == 'a' && ts[-2] == 'n' && ts[-1] == ' ') {
                    Flags |= NA_A;
                    p -= 3;
                    goto PutName;
                }
            if (ts - msg == 3)
                if (ts[-3] == 'A' && ts[-2] == 'n' && ts[-1] == ' ') {
                    Flags |= NA_A + NA_CAPS;
                    p -= 3;
                    goto PutName;
                }
            if (ts - msg >= 3)
                if ((ts[-3] == ' ' || ts[-3] == '>') && ts[-2] == 'a' && ts[-1] == ' ') {
                    Flags |= NA_A;
                    p -= 2;
                    goto PutName;
                }
            if (ts - msg >= 3)
                if ((ts[-3] == ' ' || ts[-3] == '>') && ts[-2] == 'A' && ts[-1] == ' ') {
                    Flags |= NA_A + NA_CAPS;
                    p -= 2;
                    goto PutName;
                }
            if (ts - msg == 4)
                if (ts[-4] == 't' && ts[-3] == 'h' && ts[-2] == 'e' && ts[-1] == ' ') {
                    Flags |= NA_THE + NA_CAPS;
                    p -= 4;
                    goto PutName;
                }
            if (ts - msg == 4)
                if (ts[-4] == 'T' && ts[-3] == 'h' && ts[-2] == 'e' && ts[-1] == ' ') {
                    Flags |= NA_THE + NA_CAPS;
                    p -= 4;
                    goto PutName;
                }
            if (ts - msg >= 4)
                if ((ts[-4] == ' ' || ts[-4] == '>') && ts[-3] == 'A' && ts[-3] == 'n' && ts[-1] == ' ') {
                    Flags |= NA_A + NA_CAPS;
                    p -= 3;
                    goto PutName;
                }
            if (ts - msg >= 4)
                if ((ts[-4] == ' ' || ts[-4] == '>') && ts[-3] == 'a' && ts[-2] == 'n' && ts[-1] == ' ') {
                    Flags |= NA_A;
                    p -= 3;
                    goto PutName;
                }
            if (ts - msg >= 5)
                if ((ts[-5] == ' ' || ts[-5] == '>') && ts[-4] == 't' && ts[-3] == 'h' && ts[-2] == 'e' && ts[-1] == ' ') {
                    Flags |= NA_THE;
                    p -= 4;
                    goto PutName;
                }
            if (ts - msg >= 5)
                if ((ts[-5] == ' ' || ts[-5] == '>') && ts[-4] == 'T' && ts[-3] == 'h' && ts[-2] == 'e' && ts[-1] == ' ') {
                    Flags |= NA_THE;
                    p -= 4;
                    goto PutName;
                }

PutName:
            str = Tag2[0] ? Tag2 : Tag1;
            if (str.Left(2) == "ea")
                Subject = ev ? ev->EActor : theGame->GetPlayer(0);
            else if ((str.Left(2) == "ev") || (str.Left(2) == "et"))
                Subject = ev ? ev->ETarget : theGame->GetPlayer(0);
            else if (str == "eitem2" || str == "ei2")
                Subject = ev ? ev->EItem2 : (Item*)theGame->GetPlayer(0);
            else if (str.Left(2) == "ei")
                Subject = ev ? ev->EItem : (Item*)theGame->GetPlayer(0);
            else if (str.Left(3) == "obj" || str.Left(3) == "mon" || str.Left(3) == "itm") {
                if (!str[3])
                    i = n + 1;
                else
                    i = atoi(str.Right(str.GetLength() - 3));

                while (i > n) {
                    n++;
                    Params[n].o = va_arg(args, Thing*);
                }
                Subject = Params[i].o;
            } else if (str.Left(4) == "hobj" || str.Left(4) == "hmon" || str.Left(4) == "hitm") {
                if (!str[4])
                    i = n + 1;
                else
                    i = atoi(str.Right(str.GetLength() - 4));
                while (i > n) {
                    n++;
                    Params[n].o = oThing(va_arg(args, hObj));
                }
                Subject = Params[i].o;
            } else {
                Error("Syntax error in __XPrint!");
                xprint_lev--;
                return msg;
            }

            str = Tag1;
            if (((int)(Subject)) < 0x000FFFFF) {
                Error("Probable parameter mismatch in __XPrint; msg = \"%s\","
                    " POV=%d, Subject=%d", msg, (long)POV, (long)Subject);
                Error("Probable parameter mismatch in __XPrint; msg = \"%s\","
                    " POV=%s, Subject=%s", msg, (const char*)POV->Name(0), (const char*)Subject->Name(0));
                return msg;
            }

            if (!Subject) {
                strcpy(p, "[null]");
                Error("Null string in __XPrint; msg = \"%s\"", msg);
            } else if (!Tag2[0]) {
                if (POV && !POV->XPerceives(Subject))
                    strcpy(p, Something(Flags));
                else
                    strcpy(p, Subject->Name(Flags));
            } else if (str == "her") {
                Error("\"<her>\" is ambiguous in a message; use him/his!");
                xprint_lev--;
                return msg;
            } else if (str == "his") {
                if (POV && !POV->XPerceives(Subject))
                    strcpy(p, Flags & NA_CAPS ? "Its" : "its");
                else
                    strcpy(p, Flags & NA_CAPS ? Subject->His() : Subject->his());
            } else if (str == "him") {
                if (POV && !POV->XPerceives(Subject))
                    strcpy(p, Flags & NA_CAPS ? "It" : "it");
                else
                    strcpy(p, Flags & NA_CAPS ? Subject->Him() : Subject->him());
            } else if (str == "he" || str == "she") {
                if (POV && !POV->XPerceives(Subject))
                    strcpy(p, Flags & NA_CAPS ? "It" : "it");
                else
                    strcpy(p, Flags & NA_CAPS ? Subject->He() : Subject->he());
            } else if (str == "s") {
                if (POV && !POV->XPerceives(Subject))
                    continue;
                else if (!Subject->isPlural())
                    strcpy(p, "s");
                /* HACKFIX */
                else
                    strcpy(p, "");
            } else if (str == "god") {
                if (!Subject->isCreature())
                    Error("__XPrint: <God> applied to non-Creature Object!");
                else if (Subject->isCharacter())
                    strcpy(p, NAME(((Creature*)Subject)->getGod()));
                else if (((Creature*)Subject)->isMType(MA_GOOD))
                    strcpy(p, random(2) ? "Essiah" : "Xavias");
                else if (((Creature*)Subject)->isMType(MA_EVIL))
                    strcpy(p, random(2) ? "Xel" : "Zurvash");
                else
                    strcpy(p, random(2) ? "Immotian" : "Ekliazeh");

            }
            ev = oev;
            p = Out + strlen(Out);
        } else
            *p++ = *cc++;
    }

    *p = 0;
    xprint_lev--;
    return Out;
}

const char* PPrint(Player *p, const char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    const char*str;
    ev = NULL;
    str = __XPrint(p, msg, ap);
    va_end(ap);
    return str;
}

const char* XPrint(const char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    const char*str;
    ev = NULL;
    str = __XPrint(NULL, msg, ap);
    va_end(ap);
    return str;
}

void DPrint(EventInfo &e, const char* msg1, const char* msg2, ...) {
    va_list ap;
    Player *p;
    va_start(ap, msg2);
    ev = &e;
    if (!e.EMap)
        return;
    if (msg1)
        e.EActor->__IPrint(msg1, ap);

    if (msg2 && e.EMap)
        for (int16 i = 0; i != MAX_PLAYERS; i++)
            if (p = oPlayer(e.EMap->pl[i]))
                if (p != e.EActor)
                    if (p->XPerceives(e.EActor))
                        p->__IPrint(msg2, ap);

    va_end(ap);
}

void APrint(EventInfo &e, const char* msg, ...) {
    va_list ap; Player *p;
    va_start(ap, msg);
    ev = &e;
    if (e.EMap)
        for (int16 i = 0; i != MAX_PLAYERS; i++)
            if (p = oPlayer(e.EMap->pl[i]))
                if ((e.EActor && p->XPerceives(e.EActor)) || (e.EItem && p->XPerceives(e.EItem)) || (e.EVictim && p->XPerceives(e.EVictim)))
                    p->__IPrint(msg, ap);

    va_end(ap);
}

void VPrint(EventInfo &e, const char* msg1, const char* msg2, ...) {
    va_list ap; Player *p;
    va_start(ap, msg2);
    ev = &e;

    if (msg1 && e.EVictim && e.EVictim->m)
        e.EVictim->__IPrint(msg1, ap);

    if (msg2 && e.EMap)
        for (int16 i = 0; i != MAX_PLAYERS; i++)
            if (p = oPlayer(e.EMap->pl[i]))
                if (p != e.EVictim && e.EVictim)
                    if (p->XPerceives(e.EVictim))
                        p->__IPrint(msg2, ap);
    va_end(ap);
}

void Hear(EventInfo &e, int16 range, const char* msg, ...) {
    va_list ap; Player *p;
    va_start(ap, msg);
    ev = &e;

    if (msg && e.EMap)
        for (int16 i = 0; i != MAX_PLAYERS; i++)
            if (p = oPlayer(e.EMap->pl[i]))
                if (dist(e.EActor->x, e.EActor->y, p->x, p->y) <= range)
                    if (p != e.EActor && !p->XPerceives(e.EActor))
                        if (!p->HasMFlag(M_DEAF))
                            p->__IPrint(msg, ap);

    va_end(ap);
}

void TPrint(EventInfo &e, const char* msg1, const char* msg2, const char* msg3, ...) {
    va_list ap; Player *p;
    va_start(ap, msg3);
    ev = &e;

    if (msg1 && e.EActor && e.EActor != e.EVictim)
        e.EActor->__IPrint(msg1, ap);
    if (msg2 && e.EVictim)
        e.EVictim->__IPrint(msg2, ap);

    if (msg3 && e.EMap)
        for (int16 i = 0; i != MAX_PLAYERS; i++)
            if (p = oPlayer(e.EMap->pl[i]))
                if (p != e.EActor && p != e.EVictim)
                    if ((e.EActor && p->XPerceives(e.EActor)) && (!e.EVictim || p->XPerceives(e.EVictim)))
                        p->__IPrint(msg3, ap);

    va_end(ap);
}

void Map::SetQueue(int16 Queue) {
    QueueSP++;
    if (QueueSP > (sizeof(QueueStack) / sizeof(QueueStack[0])))
        Error("Queue Stack Overflow!");
    QueueStack[QueueSP] = (int8)Queue;
}
         
void Map::UnsetQueue(int16 Queue) {
    if (QueueStack[QueueSP] != Queue)
        Error("Print Queue Stack corruption!");
    else
        QueueSP--;
}

void Map::PrintQueue(int16 Queue) {
    for (int16 i = 0; Things[i]; i++)
        if (oThing(Things[i])->Type == T_PLAYER)
            oPlayer(Things[i])->DumpQueue(Queue);
}

void Map::EmptyQueue(int16 Queue) {
    for (int16 i = 0; Things[i]; i++)
        if (oThing(Things[i])->Type == T_PLAYER)
            oPlayer(Things[i])->EmptyQueue(Queue);
}

void Player::DumpQueue(int16 Queue) {
    if (Queue == m->QueueNum())
        return;

    if (MessageQueue[Queue]) {
        MyTerm->Message(MessageQueue[Queue]);
        MessageQueue[Queue].Empty();
    }
}

void Player::EmptyQueue(int16 Queue) {
    if (MessageQueue[Queue])
        MessageQueue[Queue].Empty();
}

void Player::__IPrint(const char*msg, va_list ap) {
    const char* fm;
    if (Silence)
        return;

    if (msg) {
        fm = __XPrint(this, msg, ap);
        ((char*)fm)[0] = toupper(fm[0]);
        if (m && m->QueueNum()) {
            MessageQueue[m->QueueNum()] += fm;
            MessageQueue[m->QueueNum()] += " ";
        } else
            MyTerm->Message(fm);
    }
}

void Player::IPrint(const char*msg, ...) {
    va_list ap;
    va_start(ap, msg);
    __IPrint(msg, ap);
    va_end(ap);
}

void Creature::IPrint(const char*msg, ...) {
    /*
    va_list ap;
    Creature * c = ts.getLeader();
    if (c) {
    va_start(ap, msg);
    c->__IPrint(msg,ap);
    va_end(ap);
    }
    */
}

/* weimer: debugging 
void Thing::IPrint(const char*msg,...) {
		va_list ap;
		va_start(ap, msg);
        theGame->GetPlayer(0)->__IPrint(msg,ap);
		va_end(ap);
}
*/


void Player::IDPrint(const char*msg1, const char*msg2, ...) {
    va_list ap; Player *p;
    va_start(ap, msg2);
    if (msg1)
        __IPrint(msg1, ap);
    if (msg2 && m)
        for (int16 i = 0; i != MAX_PLAYERS; i++)
            if (p = oPlayer(m->pl[i]))
                if (p != this)
                    if (p->XPerceives(this))
                        p->__IPrint(msg2, ap);

    va_end(ap);
}

void Thing::IDPrint(const char*msg1, const char*msg2, ...) {
    va_list ap; Player *p;
    va_start(ap, msg2);

    if (msg2 && m)
        for (int16 i = 0; i != MAX_PLAYERS; i++)
            if (p = oPlayer(m->pl[i]))
                if (p->XPerceives(this))
                    p->__IPrint(msg2, ap);
    if (isItem() && !m && ((Item*)this)->Owner()) {
        Map *_m = ((Item*)this)->Owner()->m;
        if (_m)
            for (int16 i = 0; i != MAX_PLAYERS; i++)
                if (p = oPlayer(_m->pl[i]))
                    if (p->XPerceives(this))
                        p->__IPrint(msg2, ap);
    }

    va_end(ap);
}

void SinglePrintXY(EventInfo &e,const char *msg,...)
  {
    static String Recent[12];
		va_list ap; Player *p; int16 i;
		va_start(ap, msg);

    if (!msg)
      {
        for(i=0;i!=12;i++)
          Recent[i].Empty();
        return;
      }
    
    for(i=0;Recent[i].GetLength() && i!=12;i++)
       if (!stricmp(msg,Recent[i]))
        return;
    if (i != 12)
      Recent[i] = msg;

    if (msg && e.EMap)
      for(i=0;i!=MAX_PLAYERS;i++)
        if (p = oPlayer(e.EMap->pl[i])) 
          if ((e.isLoc && e.EMap->At(e.EXVal,e.EYVal).Visibility & VI_VISIBLE << i*4) ||
              (!e.isLoc && e.EActor && p->Perceives(e.EActor)) )
            p->__IPrint(msg,ap);
  }


char DiceStr[10];
const char* Dice::Str()
	{
	  if (Sides <= 0)
	    return Format("%d",(int16)Bonus);
    else if (Sides == 1) /* 7d1+2 == 9 */
      return Format("%d",(int16)(Number + Bonus));
		if(Bonus)
			return Format("%dd%d%+d",(int16)Number,(int16)Sides,(int16)Bonus);
		else
			return Format("%dd%d",(int16)Number,(int16)Sides);
	}

String & Map::Name(int16 Flags)
  {
    if (dID)
      return *tmpstr(NAME(dID));
    else
      return *tmpstr("[Map]");
  }

String & GetFirstName(const char* _Fullname)
  {
    /* Things players might put in front of a name that
       should be ruled out as potentially being a true
       first name. Many aren't medieval, but may apply
       to joke names or such. */
    String NameSegs[3], Fullname; int16 i,c;
    const char * Prefixes[] = {
      "Mr.", "Mr", "Ms.", "Ms", "Miss", "Mister",
      "Dr.", "Dr", "Doctor", "Sir", "Lady", "Baron",
      "Count", "Duke", "King", "Viscount", "Baroness",
      "Countess", "Duchess", "Queen", "Emperor",
      "Ambassidor", "Chairman", "Chairwoman", "Chairperson",
      "Chair", "Empress", "Father", "Mother", "Brother",
      "Sister", "Saint", "St.", "St", "Bishop", "Cardinal",
      "Pope", "Guru", "Imam", "Director", "Captain", "Sergeant",
      "Admiral", "Commodore", "Lieutenant", "Ensign", "Abbot",
      "Madam", "Madame", "Commander", "Lord", "Magister",
      "Magistrate", "Guildmaster", "Guildsman", "Apprentice",
      "Journeyman", "Private", "Secretary", "Secretary-General",
      "Chamberlain", "Provincial", "Rector", "Overseer",
      "Custodian", "Operative", "Agent", "Seneschal", "Prince",
      "Princess", "Baronet", "Czar", "Tzar", "Tsar", "Voivode",
      NULL };
    
    const char * MonkNames[] = {
      "Scarlet", "Ebon", "Righteous", "Jade", "Iron", "Excelent",
      "Graceful", "Unfolding", "Revealed", "Ivory", "Harmonious",
      "Furious", "Transcendant", "Resplendant", "River", "Flawless",
      "Immaculate", "August", "Fivefold", "Eightfold", NULL };  
    
    Fullname = Trim(_Fullname);

    c=0;
    for (i=0;Fullname[i];i++)
      {
        if (!isspace(Fullname[i]))
          NameSegs[c] += Fullname[i];
        else {
          if (NameSegs[c].GetLength())
            if (i && !isspace(Fullname[i-1]))
              c++;
          }
        if (c == 3)
          break;
      }

    /* Trailing Spaces (paranoia) */
    if (NameSegs[c].GetLength())
      c++;

    if (!c)
      return *tmpstr("Nameless");

    /* A one-word name; use the whole thing as the first
       name, even if it's a prefix (a player names their
       dog "King", for example */
    if (c == 1)
      return *tmpstr(NameSegs[0]);

    for(i=0;Prefixes[i];i++)
      if (!stricmp(Prefixes[i],NameSegs[0]))
        {
          /* "Mister Elliot" -> Elliot */
          return *tmpstr(NameSegs[1]);
        }
        
    for(i=0;MonkNames[i];i++)
      if (!stricmp(MonkNames[i],NameSegs[0]))
        {
          /* "Flawless Tiger" -> Flawless Tiger */
          return *tmpstr(Fullname);
        }
        

    return *tmpstr(NameSegs[0]);

  }


String & Creature::Name(int16 Flags)
	{
		String s;
    
    if (!(Flags & NA_LONG) && Named.GetLength() && (StateFlags & MS_KNOWN))
      { s = GetFirstName(Named); goto Done; }
    if (Flags & NA_SHADOW) {
      s = "unclearly seen ";
      switch(TMON(mID)->Size) {
        case SZ_MINISCULE: s += "miniscule "; break;
        case SZ_TINY:      s += "tiny "; break;
        case SZ_SMALL:     s += "small "; break;
        case SZ_MEDIUM:    s += "human-sized "; break;
        case SZ_LARGE:     s += "large "; break;
        case SZ_HUGE:      s += "huge "; break;
        case SZ_GARGANTUAN:s += "gargantuan "; break;
        case SZ_COLLOSAL:  s += "collosal "; break;
        }

      if (TMON(mID)->HasFlag(M_INCOR))
        s += "spectral ";
      else if (TMON(mID)->HasFlag(M_HUMANOID))
        s += "humanoid ";
      else if (TMON(mID)->HasFlag(M_AMORPH))
        s += "amorphious ";
      else if (isMType(MA_REPTILIAN))
        s += "reptillian ";
      else
        s += "monsterous ";
      s += "form";
      goto Done;
      }
    else {
      s = NAME(mID);
      if (StateFlags & MS_POLY_KNOWN)
        s = NAME(tmID);
      if (!(StateFlags & MS_KNOWN)) {
        TAttack *ta = TMON(mID)->GetAttk(A_SEEM);
        if (ta)
          s = NAME(ta->u.xID);
        }
      if (s.strchr(';'))
        s = s.Upto(";");
      if (!HasStati(POLYMORPH)) {
        StatiIterNature(this,TEMPLATE)
            if (S->Mag) { 
            if (TTEM(S->eID)->HasFlag(TMF_POSTSCRIPT))
              s += SC(" ") + NAME(S->eID);
            else
              s = SC(NAME(S->eID)) + SC(" ") + s;
            } 
        StatiIterEnd(this)
        /* Kludge:
             graveborn ancient goblin -> ancient graveborn goblin */
        const char* utn[] = {
          "ancient ", "elder ", "spellstitched ", "spiked ", NULL };
        for (int16 n=0;utn[n];n++)
          if (strstr(s,utn[n]))
            {
              s = s.Replace(utn[n], "");
              s = SC(utn[n]) + s;
            }
        } 
      }
    if (Flags & NA_STATI) {
      if (HasMFlag(M_NEUTER))
        ;
      else if (StateFlags & MS_FEMALE)
        s = SC("female ") + s;
      else
        s = SC("male ") + s;
      switch((cHP*10) / max(1,(mHP+Attr[A_THP])))
        {
          case 10: s = SC("uninjured ") + s; break;
          case 9:
          case 8:  s = SC("mildly injured ") + s; break;
          case 7:  
          case 6:  s = SC("moderately hurt ") + s; break;
          case 5:  
          case 4:  s = SC("severely hurt ") + s; break;
          case 3:  
          case 2:  s = SC("badly wounded ") + s; break;
          case 1:  s = SC("critically wounded ") + s; break;
          case 0:  s = SC("almost dead ") + s; break;
        }
      if (HasStati(AFRAID))
        s = SC("frightened ") + s;
      if (HasStati(PARALYSIS))
        s = SC("paralyzed ") + s;
      if (HasStati(CONFUSED))
        s = SC("confused ") + s;
      if (HasStati(ELEVATED))
        s = SC("climbing ") + s;
      if (HasStati(STUNNED))
        s = SC("stunned ") + s;
      if (HasStati(HIDING))
        s = SC("hiding ") + s;
      if (HasStati(PRONE))
        s = SC("prone ") + s;
      if (HasStati(CHARGING))
        s = SC("charging ") + s;
      if (HasStati(SLEEPING))
        s = SC("sleeping ") + s;
      if (HasStati(BLIND))
        s = SC("blinded ") + s;
      if (HasStati(STUCK))
        s = SC("stuck ") + s;
      if (HasStati(PHASED))
        s = SC("phased ") + s;
      if (HasStati(SUMMONED))
        s = SC("summoned ") + s;
      if (StateFlags & MS_PEACEFUL) {
        if (isHostileTo(theGame->GetPlayer(0)))
          s = SC("formerly peaceful ") + s;
        else
          s = SC("peaceful ") + s;
        } 
      {
      Player *p = theGame->GetPlayer(0); 
      if (p->HasStati(HIDING))
        if (p->SkillLevel(SK_APPRAISE) - 4 >= SkillLevel(SK_BLUFF))
          s = SC((Perceives(p) ? "aware " : "unaware ")) + s;
      }
      if (isFriendlyTo(theGame->GetPlayer(0)) && !isPlayer())
        s = SC("allied ") + s;
      else if (!isHostileTo(theGame->GetPlayer(0)) && !isPlayer())
        s = SC("neutral ") + s;
      else if (!isPlayer())
        s = SC("hostile ") + s;
      
        
      }
    if (isIllusion() && !isRealTo(theGame->GetPlayer(0)))
      s = SC("illusory ") + s;




    if (!HasMFlag(M_PROPER)) {
      if (Flags & NA_A) {
        Player * POV = theGame->GetPlayer(0);
        if ( POV == GetStatiObj(ILLUSION) || 
             POV == GetStatiObj(SUMMONED) )
          s = SC("your ") + s;
        else if ((s[0] == 'a') || (s[0] == 'e') || (s[0] == 'i') || (s[0] == 'o') 
            || (s[0] == 'u'))
          s = SC("an ") + s;
        else
          s = SC("a ") + s;
        }
      else if (Flags & NA_THE)
        s = SC("the ") + s;
      }
    
    if (Named.GetLength() && !(Flags & NA_SHADOW) && (StateFlags & MS_KNOWN))
      s += SC(" named ") + Named;
      
    if ((mID != tmID) && (StateFlags & MS_POLY_KNOWN) && (Flags & NA_LONG))
      {
        bool isForm; TAttack Attk[64]; 
        int16 i, n; String form;
        isForm = false;
        n = ListAttacks(Attk,64);
        for (i=0;i!=n;i++)
          if (Attk[i].AType == A_FORM)
            if (Attk[i].u.xID == mID)
              isForm = true;
        form = NAME(mID);
        if (isForm)
          s += XPrint(" (in <Res> form)", mID);
        else
          s += XPrint(" (polymorphed into <Str> <Res>)",
            strchr("aeiou",form[0]) ? "an" : "a", mID);
      } 
      
    Done:
    if (Flags & NA_CAPS)
      s = s.Capitalize();
    if (Flags & NA_POSS)
      {
        if (s[s.GetLength()-1] == 's')
          s += "'";
        else
          s += "'s";
      }
		return *tmpstr(s);
	}

String & Trap::Name(int16 Flags)
  {
    String s;
    if (Flags & NA_THE)
      s = "the ";
    else if (Flags & NA_A) 
		  s = "a ";
    else
      s = "";

    s += NAME(tID);

    return *tmpstr(s);

  }





String & Item::Name(int16 Flags)
{
  uint8 i, ofCount; EffMem *em;
  String s, d, flav, pre, post, ofWords[8];
  EventInfo xe; bool NamedOnly;
  xe.Clear();
  xe.Event = EV_GETNAME;
  
  if (isType(T_WEAPON) || isType(T_MISSILE) || isType(T_BOW)) 
    if (HasQuality(WQ_SLAYING) || HasQuality(WQ_BANE))
      if (GetBane() == 0)
        RandomBane();
  if ((Known & KN_MAGIC) && (Known & KN_CURSE))
    Inscrip.Empty();
  
  if (eID) {
    if ((em = EFFMEM(eID,theGame->GetPlayer(0)))->FlavorID && !isType(T_POTION)) {
      /* This REALLY doesn't belong here, but there is no better
         place to put it that insures it gets called every single
         time it should. */
      if (em->Tried && Inscrip.GetLength() == 0)
        Inscrip = "tried";
      if (em->Known)
      { Known |= KN_MAGIC;
        if (Inscrip == "tried")
          Inscrip.Empty(); }
        flav = Trim(NAME(em->FlavorID));
    }
    if ((em = EFFMEM(eID,theGame->GetPlayer(0)))->PFlavorID && isType(T_POTION)) {
      /* This REALLY doesn't belong here, but there is no better
         place to put it that insures it gets called every single
         time it should. */
      if (em->PTried && Inscrip.GetLength() == 0)
        Inscrip = "tried";
      if (em->PKnown)
      { Known |= KN_MAGIC;
        if (Inscrip == "tried")
          Inscrip.Empty(); }
        flav = NAME(em->PFlavorID);
    }
  }

  bool terse_blessed = theGame->Opt(OPT_TERSE_BLESSED) ? true : false;
  if (Flags & NA_NO_TERSE)
    terse_blessed = false; 


  if((Known & KN_CURSE || Flags & NA_IDENT))
  {
    if (isBlessed()) {
      char const * str = terse_blessed ? "b " : "blessed " ;
      xe.nCursed += !(Flags & NA_MONO) ? Format("%c%s%c",-SKYBLUE,str,-GREY) : s;
    }
    else if (isCursed()) {
      char const * str = terse_blessed ? "c " : "cursed " ;
      xe.nCursed += !(Flags & NA_MONO) ? Format("%c%s%c",-RED,str,-GREY) : s;
    } 
    else {
      if (!terse_blessed)
      xe.nCursed += "uncursed ";
  }
  } else {
    if (terse_blessed)
      xe.nCursed += !(Flags & NA_MONO) ? Format("%c?%c ",-WHITE,-GREY) : "? ";
  } 

  /*
  if (isGhostTouch()) {
      char const * str = terse_blessed ? "g " : "ghost touch " ;
      xe.nCursed += !(Flags & NA_MONO) ? Format("%c%s%c",-CYAN,str,-GREY) : str;
  } 
  */

  TextVal PhaseDesc[] = {
    { PHASE_MATERIAL, "material " },
    { PHASE_ETHEREAL, "ethereal " },
    { PHASE_ASTRAL,   "astral " },
    { PHASE_SHADOW,   "umbral " },
    { PHASE_NEGATIVE, "negative-material " },
    { 0, NULL } };
    
  if (HasStati(PHASED))
    xe.nAdjective += Lookup(PhaseDesc,GetStatiVal(PHASED));
    
  if (Flags & NA_LONG) {
    if (HasStati(COCKED))
      xe.nAdjective += "cocked ";
    else if (needsToBeCocked())
      xe.nAdjective += "uncocked ";
  }

  if ((Flags & NA_LONG) && HasStati(POISONED))
    if (isType(T_WEAPON) || isType(T_MISSILE))  
      xe.nAdjective += "poisoned ";

  if ((Flags & NA_LONG) && isType(T_FOOD) || isType(T_CORPSE))
    if (((Food*)this)->Eaten && (Flags & NA_LONG))
      xe.nAdjective += "partly eaten ";

  if ((Flags & NA_LONG) && (cHP != MaxHP() && (Flags & NA_LONG))) {
    if (cHP*3 < MaxHP())
      xe.nAdjective += "badly ";
    else if ((TITEM(iID)->hp - cHP)*3 < MaxHP())
      xe.nAdjective += "mildly ";
    else
      xe.nAdjective += "partly ";
    switch (DmgType) {
      case AD_FIRE: xe.nAdjective += isMetallic() ? "melted " : "burnt "; break; 
      case AD_ACID: xe.nAdjective += "melted "; break;
      case AD_SONI: xe.nAdjective += "cracked "; break;
      case AD_NECR: xe.nAdjective += "withered "; break;
      case AD_RUST: xe.nAdjective += "rusted "; break;
      default:      xe.nAdjective += "damaged "; break;
    }
  }


  if (IFlags & IF_BROKEN)
    xe.nAdjective += "broken ";

  if (HasStati(MASTERWORK))
    xe.nAdjective += "masterwork ";

  ofCount = 0;
  for (i=0;i<8;i++)
    ofWords[i].Empty(); 

  if (!(eID && TEFF(eID)->HasFlag(EF_HIDEQUAL)))
    for (i=1;i<max(WQ_TRUELAST,AQ_TRUELAST);i++) 
      if (KnownQuality(i) || (HasQuality(i) && Flags & NA_IDENT)) {
        if (LookupOnly(GenericPreQualNames,i)) {
          xe.nPrequal += Lookup(GenericPreQualNames,i); xe.nPrequal += " "; 
        } else if (Type == T_ARMOUR || Type == T_SHIELD) {
          if (LookupOnly(APostQualNames,i)) 
            ofWords[ofCount++] = Lookup(APostQualNames,i);
          else { 
            xe.nPrequal += Lookup(APreQualNames,i); xe.nPrequal += " "; 
          }
        } else { // not Armour
          if (LookupOnly(PostQualNames,i)) {
            if (i == WQ_SLAYING && GetBane() != -2)
              ofWords[ofCount++] = SC(Lookup(MTypeNames,GetBane())) 
                + " Slaying";
            else
              ofWords[ofCount++] = Lookup(PostQualNames,i);
          } else { 
            if (i == WQ_BANE && GetBane() != -2)
              xe.nPrequal += Lookup(MTypeNames,GetBane());
            xe.nPrequal += Lookup(PreQualNames,i); 
            xe.nPrequal += " "; 
          }
        } 
      } 

  if (IFlags & IF_RUNIC)
    flav = "rune-covered";

  if (flav.GetLength())
    xe.nFlavour += flav + SC(" ");



  if ((Known & KN_MAGIC || Flags & NA_IDENT) && eID)
    if (TEFF(eID)->HasFlag(EF_NAMEFIRST))
      xe.nPrequal += SC(NAME(eID)) + SC(" ");

  if ((Known & KN_MAGIC || Flags & NA_IDENT) && eID &&
      TEFF(eID)->HasFlag(EF_NAMEONLY)) 
    xe.nBase += NAME(eID);
  else
    xe.nBase += NAME(iID);
    
  if ((Known & KN_MAGIC || Flags & NA_IDENT) && eID)
    {
      if (TEFF(eID)->HasFlag(EF_POSTFIX))
        {
          xe.nBase += ", ";
          xe.nBase += NAME(eID);
          xe.nBase = Capitalize(xe.nBase, true);
        }
      else if (TEFF(eID)->HasFlag(EF_PROPER))
        {
          xe.nBase += " '";
          xe.nBase += NAME(eID);
          xe.nBase += "'";
        }
    } 
  

  if (((Known & KN_PLUS || Flags & NA_IDENT) || HasStati(BOOST_PLUS)) && GetPlus() /*&&
      (Type == T_WEAPON || Type == T_ARMOUR || Type == T_TOME || 
       Type == T_SHIELD || Type == T_BOW || Type == T_MISSILE ||
       (TEFF(eID) && TEFF(eID)->HasFlag(EF_NEEDS_PLUS)))*/)
  {
    if (HasStati(BOOST_PLUS) && !(Flags & NA_MONO))
      xe.nPlus += Format(" %c%+d%c",-YELLOW, (Known & KN_PLUS || Flags & NA_IDENT) ? GetPlus() : HighStatiMag(BOOST_PLUS), -GREY);
    else
      xe.nPlus += Format(" %+d",GetPlus());
  }

  if(Known & KN_MAGIC || Flags & NA_IDENT) {
    if(eID && !TEFF(eID)->HasFlag(EF_NAMEONLY) && !TEFF(eID)->HasFlag(EF_NAMEFIRST)
           && !TEFF(eID)->HasFlag(EF_POSTFIX) && !TEFF(eID)->HasFlag(EF_PROPER)) {
      /* Capitalize properly: blue potion, blue Potion of Healing.
         "Potion of Healing" is considered a proper name, I guess. */
      xe.nBase.SetAt(0,toupper(xe.nBase[0]));
      ofWords[ofCount++] = NAME(eID);        
    }
  }


  if (Flags & NA_LONG) {
    if ((isKnown(KN_PLUS|KN_PLUS2) || Flags & NA_IDENT) && eID && 
         (d = TEFF(eID)->Power(GetPlus(),Owner(),eID,this)).GetLength())
      xe.nMech += SC(" [") + d + SC("]");

    if (Type == T_WAND || Type == T_STAFF || Type == T_ROD)
      if (isKnown(KN_PLUS2) || Flags & NA_IDENT)
        xe.nAppend += Format(" (%02d charge%s)", Charges,
            Charges == 1 ? "" : "s");

    if (eID && TEFF(eID)->HasFlag(EF_3PERDAY))
      if ((isKnown(KN_MAGIC) && isKnown(KN_PLUS2)) || Flags & NA_IDENT)
        xe.nAppend += Format(" (%d use%s left)", 3-Charges,
            (3-Charges) == 1 ? "" : "s");
    if (eID && TEFF(eID)->HasFlag(EF_7PERDAY))
      if ((isKnown(KN_MAGIC) && isKnown(KN_PLUS2)) || Flags & NA_IDENT)
        xe.nAppend += Format(" (%d use%s left)", 7-Charges,
            (7-Charges) == 1 ? "" : "s");

    if (Type == T_LIGHT)
      xe.nAppend += Format(" (with %d turns of light left)",Age);

    if ((isKnown(KN_MAGIC) || Flags & NA_IDENT) && HasStati(DISPELLED))
      xe.nAppend += " (dispelled)";
  }

    if (Flags & NA_INSC || Flags & NA_LONG) 
      if (Inscrip.GetLength())
          xe.nInscrip += Format(" %c{%s%c}", -GREY, (const char*)Inscrip, -GREY);
    
    /* Allow resources to override */
    
    xe.EParam = Flags;
    xe.EItem = this;
    xe.EActor = (Creature*)this;
    xe.EVictim = (Creature*)this;

    xe.nOf.Empty();
    TITEM(iID)->Event(xe,iID);
    if (xe.nOf.GetLength())
      ofWords[ofCount++] = xe.nOf;
    xe.nOf.Empty();
    if (eID)
      TEFF(eID)->Event(xe,eID);
    if (xe.nOf.GetLength())
      ofWords[ofCount++] = xe.nOf;
    xe.nOf.Empty();
    
    xe.Event = META(EV_GETNAME);
    StatiIter(this)
        if (S->eID) {
        TEFF(S->eID)->Event(xe,S->eID);
        if (xe.nOf.GetLength())
          ofWords[ofCount++] = xe.nOf;
      }
    StatiIterEnd(this)
    xe.Event = EV_GETNAME;

    /* Assemble the of-Words */
    xe.nOf.Empty();
    if (ofCount) {
      xe.nOf += " of ";
      for(i=0;i!=ofCount;i++) {
        if (!i)
          ;
        else if (i == ofCount-1)
          xe.nOf += " and ";
        else
          xe.nOf += ", ";
        xe.nOf += ofWords[i];
        }
      }

  /* Now, assemble the name */
  if (xe.nNamed.GetLength() && !(Flags & NA_LONG))
    NamedOnly = true;
  else
    NamedOnly = false;


  if (Flags & NA_THE)
    if (!xe.isProper)
      xe.nArticle = "the ";
      
  if ((Quantity == 1 || Flags & NA_SINGLE || HasIFlag(IT_ROPE)) && (Flags & NA_A)) {
    String t;
    t = xe.nAdjective + xe.nCursed + xe.nPrequal;
    if ((xe.EParam & NA_FLAVOR) || !(isKnown(KN_MAGIC) || (xe.EParam & NA_IDENT)))
      t += xe.nFlavour;
    t += xe.nBase;
    if (strchr("aeiou",tolower(Trim(t)[0])) &&
         (!TITEM(iID)->HasFlag(IT_PLURAL)))
      xe.nArticle += "an ";
    else
      xe.nArticle += "a ";
    if (TITEM(iID)->HasFlag(IT_PLURAL) &&
         (!eID || !TEFF(eID)->HasFlag(EF_NAMEONLY)
           || strstr(NAME(eID),"Gloves")))
      switch (TITEM(iID)->IType) {
        case T_ARMOUR:
          xe.nArticle += "suit of ";
          break;  
        case T_BRACERS: 
        case T_GAUNTLETS: 
        case T_BOOTS:
          xe.nArticle += "pair of "; 
          break;
        default:
          xe.nArticle += "set of ";  
          break;
      }
  }
  else if (Quantity > 1 && !(Flags & NA_SINGLE) && !HasIFlag(IT_ROPE))
    xe.nArticle += Format("%d ",Quantity);


    s.Empty();
    if (NamedOnly && Quantity > 1) /* Strange case, but possible (2 Stormbringers) */
      { s += Format("%d ",Quantity); 
        s += Pluralize(xe.nNamed); }
    else if (NamedOnly)
      s += xe.nNamed;
    else {
      s += xe.nArticle;
      s += xe.nAdjective;
      s += xe.nCursed; 
      s += xe.nPrequal;
      if ((xe.EParam & NA_FLAVOR) || !(isKnown(KN_MAGIC) || (xe.EParam & NA_IDENT)))
        s += xe.nFlavour;
      if (Quantity > 1 && !(xe.EParam & NA_SINGLE) && !HasIFlag(IT_ROPE))
        s += Pluralize(xe.nBase,iID);
      else
        s += xe.nBase;
      s += xe.nPlus;
      s += xe.nOf;
      s += xe.nPostqual;
      if (xe.nNamed.GetLength() && (xe.EParam & NA_LONG))
        { s += " named "; s += xe.nNamed; }
      }

  if (xe.EParam & NA_LONG)
    s += xe.nInscrip;
  if (xe.EParam & NA_MECH)
    s += xe.nMech;
  if (xe.EParam & NA_LONG)
    s += xe.nAppend; 

  /* Handle Capitalization */
  if (Flags & NA_CAPS || Flags & NA_XCAPS)
    s.SetAt(0, toupper(s[0]));
  if (Flags & NA_XCAPS)
    for(i=1;s[i];i++)
      if (s[i-1] == ' ')
        s.SetAt(i,toupper(s[i]));
        
  if ((Flags & NA_MECH) && !(Flags & NA_MONO)) {
      Player *p;
      p = theGame->GetPlayer(0);
      if (p->defMelee == myHandle)
          s = Format("%c%s%c", -SKYBLUE, (const char*)Decolorize(s), -GREY);
      if (p->defOffhand == myHandle)
          s = Format("%c%s%c", -AZURE, (const char*)Decolorize(s), -GREY);
      if (p->defRanged == myHandle)
          s = Format("%c%s%c", -PINK, (const char*)Decolorize(s), -GREY);
      if (p->defAmmo == myHandle)
          s = Format("%c%s%c", -MAGENTA, (const char*)Decolorize(s), -GREY);
    }

  return *tmpstr(s);
}

String & Feature::Name(int16 Flags) {
    String s;
  if (Named.GetLength() && !(Flags & NA_LONG))
      return Named;
    
    s = NAME(fID);
		
  if (cHP > 0 && (Flags & NA_LONG))
      switch((cHP*10) / max(1,mHP))
        {
          case 10: break; 
          case 9:
          case 8:  s = SC("mildly damaged ") + s; break;
          case 7:  
          case 6:  s = SC("moderately damaged ") + s; break;
          case 5:  
          case 4:  s = SC("severely damaged ") + s; break;
          case 3:  
          case 2:  s = SC("badly damaged ") + s; break;
          case 1:  s = SC("critically damaged ") + s; break;
          case 0:  s = SC("almost destroyed ") + s; break;
        }

    if (Flags & NA_A) {
      if ((s[0] == 'a') || (s[0] == 'e') || (s[0] == 'i') || (s[0] == 'o') 
          || (s[0] == 'u'))
      s = SC("an ") + s;
    else
      s = SC("a ") + s;
  }
  else if (Flags & NA_THE)
    s = SC("the ") + s;

  if (Named.GetLength())
    s += SC(" named ") + Named;

  if (HasStati(WIZLOCK) && (Flags & NA_LONG)) {
    s += Format(" (wizard-locked by %s)",(const char*) GetStatiObj(WIZLOCK)->Name(0));
  } 

    if (HasStati(MY_GOD))
      s += Format(" to %s", NAME(GetStatiEID(MY_GOD)));

    return *tmpstr(s);
	}

String & Corpse::Name(int16 Flags)
{
  String s, s2; int16 i;

  s = ""; 


  if (isType(T_FIGURE) || isType(T_STATUE)) {
    if (Quantity > 1 && !(Flags & NA_SINGLE))
      s += Format("%d ",Quantity);

    s += NAME(iID);
    s += " of ";
    if (Named)
      s += Named;
    else if (!mID)
      ;
    else if (TMON(mID)->HasFlag(M_PROPER))
      s += NAME(mID);
    else {
      s2 = NAME(mID);
      if ((s2[0] == 'a') || (s2[0] == 'e') || (s2[0] == 'i') || (s2[0] == 'o') 
          || (s2[0] == 'u'))
        s += SC("an ") + s2;
      else
        s += SC("a ") + s2;
    }

    if (Flags & NA_THE)
      s = SC("the ") + s;
    else if (Flags & NA_A) {
      if ((s[0] == 'a') || (s[0] == 'e') || (s[0] == 'i') || (s[0] == 'o') 
          || (s[0] == 'u'))
        s = SC("an ") + s;
      else
        s = SC("a ") + s;
    }

    goto Done;
  }  


  if (Named.GetLength())
  {
    s += Format("%s's %scorpse",(const char*)Named,
        Eaten ? "partly eaten " : "");
    return *tmpstr(s);
  }
  if (Eaten && (Flags & NA_LONG))
    s += "partly eaten ";
  s += NAME(mID);

  StatiIterNature(this,TEMPLATE)
      if (S->Mag) { 
        if (TTEM(S->eID)->HasFlag(TMF_POSTSCRIPT))
          s += SC(" ") + NAME(S->eID);
        else
          s = SC(NAME(S->eID)) + SC(" ") + s;
      } 
  StatiIterEnd(this)

  if (!TMON(mID)->HasFlag(M_PROPER)) {
    s += " corpse";

    if (Quantity > 1 && !(Flags & NA_SINGLE))
      s = Format("%d %ss", Quantity, (const char*)s);

    if (Flags & NA_A) {
      if ((s[0] == 'a') || (s[0] == 'e') || (s[0] == 'i') || (s[0] == 'o') 
          || (s[0] == 'u'))
        s = SC("an ") + s;
      else
        s = SC("a ") + s;
    }

    else if (Flags & NA_THE)
      s = SC("the ") + s;
  }
  else
    s += SC("'s corpse");

  {
  if (isFresh())
    s += Format(" %c(%cfresh",-GREY,-GREEN);
  else 
    s += Format(" %c(%cstale",-GREY,-RED,-GREY);
  int16 dc = noDiseaseDC();
  if (dc <= 30)
    s += Format(", DC %d",dc);
  s += Format("%c)",-GREY);
  }

Done:
  if (Flags & NA_CAPS || Flags & NA_XCAPS)
    s.SetAt(0,toupper(s[0]));
  if (Flags & NA_XCAPS)
    for(i=1;s[i];i++)
      if (s[i-1] == ' ')
        s.SetAt(i,toupper(s[i]));
  return *tmpstr(s);
}

/********************************************************************
                        Text Resource Support
 ********************************************************************/


String Game::BuildText(EventInfo &e, rID tID) {
    int16 ln = 0;
    int32 tBuff[2048];
    TText *tt = TTEX(tID);
    tt->GetList(TEXT_LIST,(rID*)tBuff,2048);
    while (tBuff[ln++])
        ;
    return *tmpstr(RecursiveParse(e,tID,tBuff,ln));
} 
    
String Game::RecursiveParse(EventInfo &e, rID tID, int32 *tBuff, int16 len) {
    String Text; hCode hc; 
    int16 i, n, c,orPos[30],nest,start;

    if (len <= 0)
        return *tmpstr("");

    /* Step 1: Choose 'Or' Segment */
    i = 0;
    c = 1;
    orPos[0] = 0;

    while (tBuff[i] && i < len) {
        switch (tBuff[i]) {
        case TC_RPAREN:
            Error("Mismatched parenthesis in dynamic text <Res>.",tID);
            return *tmpstr("Error building text.");
        case TC_LPAREN:                             
            nest = 1;
            while (nest && tBuff[i]) {
                i++;
                if (tBuff[i] == TC_LPAREN)
                    nest++;
                else if (tBuff[i] == TC_RPAREN)
                    nest--;
            }
            if (nest) {
                Error("Closing parenthesis missing in dynamic text <Res>.",tID);
                return *tmpstr("Error building text.");
            }
            break;
        case TC_CHOICE:
            Error("Choice bar found after choice filtering!");
            break;
        }
    }
    orPos[c] = 0;
    if (c > 1) {
        int16 choice = random(c);
        Text = RecursiveParse(e,tID,&tBuff[orPos[choice]+1],
            (orPos[choice+1] ? orPos[choice+1] : len) - orPos[choice]);
        return *tmpstr(Text);
    }

    Text = "";
    while (tBuff[i] && i < len) {
        switch (tBuff[i]) {
        case TC_RPAREN:
            Error("Mismatched parenthesis in dynamic text <Res>.",tID);
            return *tmpstr("Error building text.");
        case TC_LPAREN:                             
            nest++;
            start = i;
            while (nest && tBuff[i]) {
                i++;
                if (tBuff[i] == TC_LPAREN)
                    nest++;
                else if (tBuff[i] == TC_RPAREN)
                    nest--;
            }
            if (nest) {
                Error("Closing parenthesis missing in dynamic text <Res>.",tID);
                return *tmpstr("Error building text.");
            }
            if (i == start+1)
                ;
            else
                Text += RecursiveParse(e,tID,&tBuff[start+1],i-(start+1));
            break;
        case TC_CHOICE:
            orPos[c++] = i;
            break;
        case TC_CASE:
            hc = tBuff[i+1];
            n = i+2;
            nest = 0;
            while (tBuff[n]) {
                if (tBuff[n] == TC_LPAREN)
                    nest++;
                else if (tBuff[n] == TC_RPAREN)
                    nest--;
                else if (tBuff[n] == TC_TERM && !nest)
                    break;
                n++;
            }
            if (tBuff[n] != TC_TERM || nest) {
                Error("Unterminated dynamic text case in <Res>.", tID);
                return *tmpstr("Error building text.");
            }
            if (theGame->VM.Execute(&e, tID, hc))
                Text += RecursiveParse(e,tID,&tBuff[i+2],n-(i+2));
            i = n;
            break;
        case TC_ACTION:
            hc = tBuff[++i];
            e.Text = "";
            theGame->VM.Execute(&e, tID, hc);
            Text += e.Text;
            break;
        case TC_WCHOICE:
            c = (int16)(tBuff[i+1]);
            n = i+2; nest = 0;
            while (tBuff[n]) {
                if (tBuff[n] == TC_LPAREN)
                    nest++;
                else if (tBuff[n] == TC_RPAREN)
                    nest--;
                else if (tBuff[n] == TC_TERM && !nest)
                    break;
                n++;
            }
            if (tBuff[n] != TC_TERM || nest) {
                Error("Unterminated dynamic text case in <Res>.", tID);
                return *tmpstr("Error building text.");
            }
            if (random(100) <= c)
                Text += RecursiveParse(e,tID,&tBuff[i+2],n-(i+2));
            i = n;
            break;
        default:
            if (tBuff[i] >= 0x01000000)
                Text += BuildText(e,(rID)(tBuff[i]));
            else
                Text += (GetText(tID,tBuff[i]));
            break;
        }
    }
    return *tmpstr(Text);
}    
