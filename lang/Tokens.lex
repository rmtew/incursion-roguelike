%x com
%x string
%{
/* TOKENS.LEX -- See the Incursion LICENSE file for copyright information.

     Contains the lexical analyzer for the IncursionScript resource
	 compiler. Designed to be fed into FLex (a GNU Lex clone) to produce
	 tokens.cpp.
*/

#include "Incursion.h"
#include "yygram.h"
#ifdef WIN32
#include <io.h>
#endif
#include <string.h>
#include <setjmp.h>
#include <ctype.h>
#define YY_NEVER_INTERACTIVE 1
extern int16 Errors;
extern void yywarning(const char*);
char yyMap[60000];
char preprocFilename[1024]; 

uint16 yyMapSize[512];

int LitCount=0,MapCount=0;
int16 test_val = 0, brace_level = 0, decl_state = 0, if_cond = 0,
                    paren_level = 0;
void  CatLiteral(const char*);
Dice & ParseDice(const char *c_str);

int32 lpos;
#ifdef DEBUG
char LBuff[257000];
#else
char LBuff[1];
#endif
hText ht;

extern String textSeg;
extern String dataSeg;
extern SymbolTable theSymTab;

/* Really, this could go in TABLES.CPP, but it's just simpler for
   many reasons to put it here -- it doesn't really need to be a
   global for one thing. Keywords1 lists the keywords that are
   recognized as such anywhere they appear in an .IRC file, while
   Keywords2 lists keywords that are treated as such only outside
   of code blocks, and can thus be used as variable names in code
   sections. This is important, for example, because "xval" is
   both a keyword used in effect stat blocks, as well as a member
   variable of the T_EVENTINFO object. Note also that keywords are
   case sensitive inside code blocks, but case insensitive else-
   where. Identifiers are always case sensitive. This sounds
   strange, but works well in practice. */

TextVal Keywords1[] = {
  { ABS, "abs" },           { CONTINUE, "continue" }, { BOOL, "bool" },
  { BREAK, "break" },       { CASE, "case" },         { DEFAULT, "default" },
  { DO, "do" },             { ELSE, "else" },         { WORD_FALSE, "false" },
  { FOR, "for" },           { HOBJ, "hObj" },         { HTEXT, "hText" },
  { INT16, "int16" },       { INT32, "int32" },       { IF, "if" },             
  { INT8, "int8" },         { WORD_MAX, "max" },      { MEMBER, "member" },
  { WORD_MIN, "min" },      { RETURN, "return" },     { RID, "rID",  },
  { STATIC, "static" },     { STRING, "string" },     { STRING, "String" },
  { SWITCH, "switch" },     { WORD_TRUE, "true" },    { WORD_NULL, "NULL" },
  { VOID, "void",  },       { WHILE, "while" },       { WORD_RECT, "rect" },
  { WORD_RECT, "Rect" },    { UINT8, "uint8" },       { UINT16, "uint16" },
  { 0, NULL } };

  
TextVal Keywords2[] = {
  { ABILITY, "Ability" },   { ACC, "Acc" },           { ALIGN, "Align"},
  { ALL, "All" },           { ALSO, "Also" },         { AND, "And" },
  { WORD_ANY, "Any" },
  { APTITUDES, "Aptitudes"},{ ARCHERY, "Archery" },   { ARM, "Arm",  },
  { ARTIFACT, "Artifact" }, { AS, "As" },             { AT,  "At" },
  { ATTACK, "Attk" },       { AVAL, "aval",  },       { BASE, "Base" },
  { BLESSED, "Blessed" },   { BRAWL, "Brawl" },       { BRIGHT, "Bright" },
  { CHANCE, "Chance" },     { CLASS, "class",  },     { CASTER, "caster" },
  { WORD_COLOR, "Colour" },  { CONSTANTS, "Constants" }, { CAPACITY, "capacity" },
  { COST, "Cost" },         { DMG, "damage",  },      { COVERAGE, "Coverage" },
  { COVERAGE, "Cov" },      { CTYPE, "CType" },       { WORD_FILE, "File" },
  { WORD_CR, "CR" },        { CRIT, "Crit" },         { CURSED, "Cursed" },
  { CVAL, "cval" },         { DMG, "Dam" },           { DARK, "Dark" },
  { DAY, "Day" },           { WORD_DC, "DC" },        { DEF, "Def" },
  { WORD_DESC, "desc" },    { DEPTH, "Depth" },       { DISEASE, "Disease" },
  { DOOR, "door" },         { DMG, "Dmg" },           { FAVORED, "Favoured" },
  { FAVORED, "Favoured" },  { FLAVOR, "Flavour" },
  { DOMAINS, "domains" },   { DOMAIN, "domain" },     { DUNGEON, "Dungeon" },
  { DVAL, "dval" },         { EFFECT, "Effect" },     { EXPORT, "Export" },
  { WORD_ENCOUNTER, "Encounter"},{ BEHAVIOUR, "Behaviour" }, { PARTS, "Parts" },
  { EQUIP, "Equip" },       { EVAL, "eval" },         { EVENT, "Event" },
  { EVERY, "Every" },       { FACTOR, "Factor" },     { FEATS, "Feats" },
  { FEAT, "Feat" },         { FEATURE, "Feature" },   { FIRES, "Fires" },
  { FLAGS, "Flags" },       { FLOOR, "Floor" },       { FROM, "From" },
  { GEAR, "Gear" },         { GLYPH, "Glyph" },       { GOD, "God" },
  { GRANTS, "Grants" },     { GRID, "Grid" },         { GROUP, "Group" },
  { GUILD, "Guild" },       { HEAVY, "Heavy" },       { HIT, "Hit" },
  { HITDICE, "HitDice" },   { HITPOINTS, "Hitpoints"},{ HITDICE, "HD",  },
  { HITPOINTS, "HP" },      { IMAGE, "Image" },       { IMMUNE, "Immune" },
  { IMMUNE, "Imm" },        { INNATE, "Innate" },     { INVOKE, "Invoke" },
  { ITEM, "Item" },         { LDMG, "LDam" },         { LDMG, "LDmg" },
  { WORD_LEVEL, "Lev" },    { WORD_LEVEL, "Level" },  { WORD_LEVEL, "Levels" },
  { LIFESPAN, "Lifespan" }, { FUEL, "Fuel" },
  { LIGHT, "Light" },       { LISTS, "Lists" },       { LVAL, "lval" },
  { MODULE, "Module" },     { MANA, "Mana" },         { MAP, "Map" },
  { MATERIAL, "Mat" },      { MATERIAL, "Material" }, { MELEE, "Melee" },
  { MAX_SIZE, "MaxSize" },  { MTYPE, "MType" },       
  { MONSTER, "Monster" },   { MOVE, "Mov" },          { MOVE, "Move" },
  { NEAR, "Near" },         { NONE, "None" },         { NORMAL, "Normal" },
  { NPC, "NPC" },           { NUTRITION,"Nutrition" },{ NUTRITION, "Nut" },
  { OBJECTS, "Objects" },   { OBJECT, "Object" },     { OF, "Of" },
  { ON, "On" },             { OR, "Or" },             { OPTION, "Option" },
  { PARAMS, "Params" },     { PARRY, "Parry" },       { PARTS, "Parts" },
  { PARTIAL, "Partial" },   { PENALTY, "Penalty" },   { PURPOSE, "purpose" },
  { PER, "Per" },           { POWER, "power" },       { POISON, "poison" },
  { PVAL, "pval" },         { PROFICIENCIES, "Proficiencies" },
  { PROFICIENCIES, "Prof"}, { PROP, "prop" },         { PROP, "property" },
  { QUALITIES, "Qualities"},{ QUEST, "Quest" },       { QVAL, "qval"  },
  { RACE, "race" },         { RANDOM, "random" },     { RATING, "rating" },
  { REGIONS, "regions" },   { REGION, "region" },     { RANGE, "range" },
  { RESISTS, "res" },       { RESISTS, "Resists" },   { SLOT, "slot" },
  { ROOMTYPES, "roomtypes"},{ ROUTINE, "routine" },   { RVAL, "rval" },
  { SAVES, "save" },        { SAVES, "Sav" },         { SAVES, "Saves" },
  { SDMG, "SDam" },         { SDMG, "SDmg" },         { SIZE, "Size" },
  { SIZE, "Siz" },          { SKILLS, "Skills" },     { SPECIALS, "Specials" },
  { SPEED, "Spd" },         { SPEED, "Speed" },       { SPELL, "Spell" },
  { SPELLS, "Spells" },     { START, "Start" },       { STARTING, "Starting" },
  { STATI, "Stati" },       { STOCK, "Stock" },       { STORE, "Store" },
  { SUSTAINS, "Sustains" }, { SVAL, "sval" },         { SYSTEM, "system" },
  { WORD_TARGET, "target" },{ WEIGHT_LIM, "WeightLim" },{ WEIGHT_MOD, "WeightMod" },
  { WORD_TEMPLATE, "Template" }, { TERRAIN, "Terrain" },   { TEXT, "Text" },
  { THREAT, "Threat" },     { THROWING, "Throwing" }, { THROWING, "Thrown" },
  { TILE, "Tile" },         { TILES, "Tiles" },       { TIMEOUT, "Timeout" },
  { TO, "to" },
  { TURNS, "Turns" },       { TYPE, "Type" },         { TVAL, "tval" },
  { VALUE, "Value" },       { VAR, "Var" },           { WALLS, "Walls" },
  { WEEK, "Week" },         { WEIGHT, "Weight" },     { WITH, "with" },
  { XVAL, "xval" },         { YVAL, "yval" },         { 0, NULL } }; 

TextVal AttrWords[] = {
  { A_STR, "Str" },         { A_DEX, "Dex" },         { A_CON, "Con" },
  { A_INT, "Int" },         { A_WIS, "Wis" },         { A_CHA, "Cha" },
  { A_LUC, "Luc" },         { A_LOY, "Loy" },         { 0, NULL } };

TextVal DirWords[] = {
  { NORTH, "North" },       { SOUTH, "South" },       { EAST, "East" },
  { WEST, "West",  },         { NORTHEAST, "Northeast"},{ NORTHWEST, "Northwest" },
  { SOUTHEAST, "Southeast"},{SOUTHWEST, "Southwest"},{ UP, "Up" },
  { DOWN, "Down" },         { CENTER, "Center" },     { 0, NULL } };

TextVal ColourWords[] = {
  { BLACK, "black" },       { GREY, "grey" },         { GREY, "gray" },
  { WHITE, "white" },       { BLUE, "blue" },         { GREEN, "green" },
  { RED,   "red"   },       { PURPLE, "purple" },     { CYAN, "cyan" },
  { MAGENTA, "magenta" },   { YELLOW, "yellow" },     { AZURE, "azure" },
  { SKYBLUE, "sky" },       { PINK, "pink" },         { BROWN, "brown" },
  { SKYBLUE, "skyblue" },   { SHADOW, "shadow" },     { 0, NULL } };



YYSTYPE DoKeywords(const char *text)
  {
    int16 i;
    if (brace_level >= 2 || decl_state || if_cond) {
      for (i=0;Keywords1[i].Val;i++)
        if (strcmp(text,Keywords1[i].Text)==0)
          return Keywords1[i].Val;
      yylval = theSymTab.Add(text);
      return IDENT;
      }                        
    else {
      for (i=0;Keywords1[i].Val;i++)
        if (stricmp(text,Keywords1[i].Text)==0)
          {
            if (Keywords1[i].Val == INT16 || Keywords1[i].Val == INT8 ||
                Keywords1[i].Val == INT32 || Keywords1[i].Val == STRING ||
                Keywords1[i].Val == HTEXT || Keywords1[i].Val == HOBJ ||
                Keywords1[i].Val == RID || Keywords1[i].Val == WORD_RECT ||
				Keywords1[i].Val == BOOL || Keywords1[i].Val == UINT8 ||
				Keywords1[i].Val == UINT16)
              decl_state = true;
            if (Keywords1[i].Val == IF && brace_level < 2)
              if_cond = 1;
            return Keywords1[i].Val;
          }
      
      for (i=0;Keywords2[i].Val;i++)
        if (stricmp(text,Keywords2[i].Text)==0)
          {
            if (Keywords2[i].Val == ROUTINE)
              decl_state = 1;
            if (Keywords2[i].Val == SYSTEM)
              decl_state = 1;
            return Keywords2[i].Val;
          }

      for (i=0;AttrWords[i].Text;i++)
        if (stricmp(text,AttrWords[i].Text)==0)
          { yylval = AttrWords[i].Val; return ATTRIBUTE; }

      for (i=0;ColourWords[i].Text;i++)
        if (stricmp(text,ColourWords[i].Text)==0)
          { yylval = ColourWords[i].Val; return COLOR; }
      
      for (i=0;DirWords[i].Text;i++)
        if (stricmp(text,DirWords[i].Text)==0)
          { yylval = DirWords[i].Val; return DIRECTION; }

      if (stricmp(text,"slash")==0 || stricmp(text,"slashing")==0)
        { yylval = SLASH; return WEP_TYPE; }
      if (stricmp(text,"pierce")==0 || stricmp(text,"piercing")==0)
        { yylval = PIERCE; return WEP_TYPE; }
      if (stricmp(text,"blunt")==0)
        { yylval = BLUNT; return WEP_TYPE; }

      if (stricmp(text,"fort")==0)
        { yylval = FORT; return STYPE; }
      if (stricmp(text,"ref")==0)
        { yylval = REF; return STYPE; }
      if (stricmp(text,"will")==0)
        { yylval = WILL; return STYPE; }


      yyerror(Format("Illegal word token: \"%s\"\n",text));
      return ARCHERY;
      }
  }

%}
%%

"#line".*[0-9]*     { sscanf(yytext,"#line \"%s %d",preprocFilename,&yypos);
                      yypos --; /* we'll read the \n next */
                      for (int16 i=0;preprocFilename[i] && i != 1022;i++)
                        {
                          if (preprocFilename[i] == '/')
                            preprocFilename[i] = '\\';
                          if (preprocFilename[i] == '\"')
                            preprocFilename[i] = 0;
                        }
                    }

"d"               { return 'd'; }





"'"[^\']"'"        { yylval = yytext[1]; return CHAR_CONST; }

[A-Z][A-Z]"_"[A-Z0-9_]* { 
                     for (int32 i=0;yytext[i];i++)
                       if (islower(yytext[i]))
                         return DoKeywords(yytext);
                     yyerror(Format("Undefined Constant: %s (assuming value 0)\n",yytext));
                     yylval = 0; return NUMBER; }

[A-Z]"_"[A-Z0-9_]* { for (int32 i=0;yytext[i];i++)
                       if (islower(yytext[i]))
                         return DoKeywords(yytext);
                     yyerror(Format("Undefined Constant: %s (assuming value 0)\n",yytext));
                     yylval = 0; return NUMBER; }



"{:"  { char ch1, ch2; int16 MapPos = 0, sz = -1, szl = 0; long ln = yypos;
         bool warned = false; char *c, *g;
         while ((ch1 = yyinput()) != ':' && ch1!=EOF) {
           ReenterMapLoop:
           if (ch1 != '\n' && ch1 != '\t' && ch1 != '\r' && ch1 != ' ')
             { yyMap[MapPos++] = ch1; szl++; }
           if (ch1 == '\n')
             {
               yypos++; /* adjust line number! */
               ln++;
               if (sz == -1)
                 {
                   if (szl)
                     sz = szl;
                 }
               else if (szl && sz != szl && !warned)
                 {
                   warned = true;
                   yyerror(Format("Error: Mismatched grid line at line %d.",ln));
                 }
               szl = 0;
             }
                
           }
         if (ch1 == EOF)
           { yyerror(Format("Error: Unterminated Map Grid starting at line %d.",yypos));
             return ARCHERY; }
         if ((ch2 = yyinput()) == '}')
           {
             if (warned)
               yyMap[0] = 0;
             yyMap[MapPos] = 0;
             c = g = &yyMap[0];
             while (*g) {
               if (iswhite(*g))
                 g++;
               else
                 *c++ = *g++;
               }
             *c = 0;
             
             yyMapSize[MapCount++] = sz + ((strlen(yyMap)/sz)*256);
             if (strlen(yyMap) % sz)
               Error("Map grid alignment error in yylex!");
             yylval = textSeg.GetLength();
             textSeg += yyMap;
             textSeg += 1; /* ASCII char 1, our seperator */
             return MAPAREA;
           }
         yyMap[MapPos++] = ch1;
         ch1 = ch2;
         goto ReenterMapLoop;
       }

"x1"   { if (brace_level >= 2 || decl_state)
           return DoKeywords("x1");
         yylval = 1; return NUMBER; }
"x2"   { if (brace_level >= 2 || decl_state)
           return DoKeywords("x2");
         yylval = 2; return NUMBER; }
"x3"   { if (brace_level >= 2 || decl_state)
           return DoKeywords("x3");
         yylval = 3; return NUMBER; }
"x4"   { if (brace_level >= 2 || decl_state)
           return DoKeywords("x4");
         yylval = 4; return NUMBER; }
"x5"   { if (brace_level >= 2 || decl_state)
           return DoKeywords("x5");
         yylval = 5; return NUMBER; }

"::"   { return SCOPE_OPER; }
"++"   { return INCREMENT_OP; }
"--"   { return DECREMENT_OP; }
">>"   { return RSHIFT_OP; }
"<<"   { return LSHIFT_OP; }
">="   { return GE_OP; }
"<="   { return LE_OP; }
"=="   { return EQ_OP; }
"!="   { return NE_OP; }
"&&"   { return AND_OP; }
"||"   { return OR_OP; }
"*="   { return ASSIGN_MULT; }
"+="   { return ASSIGN_ADD; }
"-="   { return ASSIGN_SUB; }
"/="   { return ASSIGN_DIV; }
"%="   { return ASSIGN_MOD; }
"&="   { return ASSIGN_AND; }
"|="   { return ASSIGN_OR; }
"~="   { return ASSIGN_NEG; }
"<<="  { return ASSIGN_LSHIFT; }
">>="  { return ASSIGN_RSHIFT; }
"..."  { return ELLIPSIS; } 
"->"   { return MEMBER_OP; }

":"    { return ':'; }
"{"    { brace_level++; return '{'; }
"}"    { brace_level--; return '}'; }
";"    { decl_state = false; return ';'; }
"+"    { return '+'; }
"-"    { return '-'; }
","    { return ','; }
"%"    { return '%'; }
"/"    { return '/'; }
"|"    { return '|'; }
"!"    { return '!'; }
"@"    { return '@'; }
"#"    { return '#'; }
"$"    { return '$'; }
"^"    { return '^'; }
"&"    { return '&'; }
"*"    { return '*'; }
"("    { paren_level++; return '('; }
")"    { paren_level--; 
         if (!paren_level) 
           if_cond = 0;
         return ')'; }
"["    { return '['; }
"]"    { return ']'; }
"."    { return '.'; }
"<"    { return '<'; }
">"    { return '>'; }
"~"    { return '~'; }
"?"    { return '?'; }
"="    { return '='; }

0[xX][0-9A-Fa-f]*  { if(sscanf(yytext,"%x",&yylval)!=1)
                       Fatal("Error reading hex value in lexer!");
                     return NUMBER; }

"/*"            { BEGIN(com); }
<com>\n         { yypos++; }
<com>[^*]       { }
<com>"*"+[^\/]  { }
<com>"*/"       { BEGIN(INITIAL); }

\"                       { BEGIN(string); lpos = 0; ht = textSeg.GetLength(); }
<string>\n               { yypos++; }
<string>\"[[:space:]]*\" { /* We use string concatenation legitamately in mon4.irh.
                              Elsewhere, it's probably a typo in a message list. */
                           /*if (!strstr(preprocFilename,"mon4"))
                             yywarning("String Concatenation Warning.");*/ }
<string>"\\\""           { CatLiteral("\\\""); }
<string>[^\"]*           { CatLiteral(yytext); }
<string>\"               { LBuff[lpos++] = 1;
                           LBuff[lpos] = 0;
                           textSeg += LBuff;
                           BEGIN(INITIAL); 
                           yylval = ht; 
                           return LITERAL; }

[0-9]+"st"    { yylval = atoi(yytext); return NUMBER; }
[0-9]+"nd"    { yylval = atoi(yytext); return NUMBER; }
[0-9]+"rd"    { yylval = atoi(yytext); return NUMBER; }
[0-9]+"th"    { yylval = atoi(yytext); return NUMBER; }
[0-9]+        { yylval = atoi(yytext); return NUMBER; }

" "           { /* skip blank */ }
[\t\r\f\v]    { /* skip blank */ }
\n     { yypos++; /* adjust linenumber and skip newline */ }


[A-CE-Za-ce-z_][A-Za-z0-9_]* { return DoKeywords(yytext); }
[dD][A-Za-z_][A-Za-z0-9_]*      { return DoKeywords(yytext); }


.      { yyerror(Format("Illegal token '%s'\n", yytext));
         Errors++; if (Errors >= 4) yyterminate(); }


%%
  
void CatLiteral(const char*str)
  {
    yy_size_t i;
    for(i=0;i<yyleng;i++)
      if (str[i] != '\"')
        {
          if (str[i] == '\\')
            switch(str[++i])
              {
                case 'n': LBuff[lpos++] = '\n'; break;
                case 't': LBuff[lpos++] = ' '; 
                          LBuff[lpos++] = ' '; break;
                case '\\': LBuff[lpos++] = '\\'; break;
                case '\"': LBuff[lpos++] = '\"'; break;
				// To understand LITERAL_CHAR mangling, search for it elsewhere.
                case '<': LBuff[lpos++] = LITERAL_CHAR;
                          LBuff[lpos++] = (char)0xFF;
                          LBuff[lpos++] = '<'; break;
                case '>': LBuff[lpos++] = LITERAL_CHAR;
                          LBuff[lpos++] = (char)0xFF;
                          LBuff[lpos++] = '>'; break;
                case 0: break;
                default:
                  yyerror(Format("Unknown escape sequence at line %d.", yypos));
              }
          else {      
            if (i)
              if (isspace_(str[i]) && isspace_(str[i-1]))
                continue;
            /*if (str[i] == '_')
              { LBuff[lpos++] = ' '; }*/
            if(str[i] == '\r' || str[i] == '\n')
              { LBuff[lpos++] = ' '; yypos++; }
            else
              LBuff[lpos++] = str[i];
            }
          if (lpos >= 257000)
            Fatal("Single string literal longer than 256K characters!");

        }
    PurgeStrings();
  }

