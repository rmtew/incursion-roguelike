/* RCOMP.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains all the support and wrapper functions for the
   IncursionScript resource compiler; the bulk of the actual
   compiler is built into the ACCENT parser file Grammar.Acc.

     bool Game::ResourceCompiler()
     void Game::CountResources()
     const char* AllocTextSeg()
     void GenerateDispatch()
     void VBlock::Generate(int8 opcode, int8 typ1, int32 param1, int8 typ2,int32 param2)
     bool AllowedCast(int16 from, int16 to)
     PExp & CodeAssignment(int16 op, LExp &p1, PExp *p2)
     PExp & CodeOperator(int16 op, PExp &p1, PExp &p2)
     int16 AllocRegister()
     void FreeRegister(int16 i)

*/

#include "Incursion.h"


#include <setjmp.h>
#include "yygram.h"

TMonster * theMon, * firstMon;
TItem * theItem, * firstItem;
TFeature * theFeat, * firstFeat;
TEffect * theEff, * firstEff;
TQuest * theQuest, * firstQuest;
TArtifact * theArti, *firstArti;
TDungeon * theDgn, *firstDgn;
TRoutine *theRou, *firstRou;
TNPC *theNPC, *firstNPC;
TClass *theClass, *firstClass;
TRace *theRace, *firstRace;
TDomain *theDom, *firstDom;
TGod *theGod, *firstGod;
TRegion *theReg, *firstReg;
TTerrain *theTer, *firstTer;
TText *theTxt, *firstTxt;
TVariable *theVar, *firstVar;
TTemplate *theTemp, *firstTemp;
TFlavor *theFlavor, *firstFlavor;
TBehaviour *theBev, *firstBev;
TEncounter *theEnc, *firstEnc;
TText *theText, *firstText;
Module * theModule;
Resource *theRes;

LocationInfo *firstTile, *theTile;

extern FILE* yyin;
void yyrestart(FILE*);
void GenerateDispatch();
int16 Errors;
extern long yypos;
extern char preprocFilename[1024]; 
extern int16 currMap;
extern int16 brace_level, decl_state;

uint32 CompilerFlags = 0, 
       MemFuncID = 0,
       MemVarID = 0,
       HeapHead = 0,
       StackHead = 0;

hText LiteralVal(const char*);
const char* AllocTextSeg();
String textSeg, dataSeg;
VBlock theCodeSeg;
jmp_buf jb;
int yyparse(), yylex();

SymbolTable theSymTab;
String Unresolved;

bool RegsInUse[64];
int8 StringsInUse[64]; 

int compare_int32 (const void * a, const void * b)
{
  return ( *(int32*)b - *(int32*)a );
}

String & Game::CompileStatistics()
  {
    TextVal ST[] = {
      { SC_ABJ,      "Abjuration...." },
      { SC_ARC,      "Arcana........" },
      { SC_DIV,      "Divination...." },
      { SC_ENC,      "Enchantment..." },
      { SC_EVO,      "Evocation....." },
      { SC_ILL,      "Illusion......" },
      { SC_THA,      "Thaumaturgy..." },
      { SC_NEC,      "Necromancy...." },
      { SC_WEA,      "Weavecraft...." },
      { -AI_DRUIDIC, "Druidic......." },
      { -AI_THEURGY, "Theurgy......." },
      { 0, NULL } };
    String s, s2; rID eID;
    uint16 i, j;
    int32 monCount, temCount, trapCount,
             poiCount, disCount, domCount, 
             enchCount, divCount, druCount,
             terCount, regCount, spCount,
             itemCount, magCount, encCount;
    
    s = Format("\n%cModule Statistics:%c\n",-14,-7);
    
    monCount = Modules[0]->szMon;
    temCount = Modules[0]->szTem;
    domCount = Modules[0]->szDom;
    terCount = Modules[0]->szTer;
    regCount = Modules[0]->szReg;
    itemCount = Modules[0]->szItm;
    encCount = Modules[0]->szEnc;
    
    trapCount = poiCount = disCount =
      enchCount = divCount = druCount =
      spCount = magCount = 0;
    
    for (i=0;i!=LastSpell();i++)
      {
        eID = SpellID(i);
        if (TEFF(eID)->Vals(0)->aval == AR_POISON)
          poiCount++;
        if (TEFF(eID)->Vals(0)->aval == AR_DISEASE)
          disCount++;
        if (TEFF(eID)->HasSource(AI_TRAP))
          trapCount++;
        if (TEFF(eID)->HasSource(AI_WIZARDRY))
          magCount++;
        if (TEFF(eID)->HasSource(AI_THEURGY))
          divCount++;
        if (TEFF(eID)->HasSource(AI_DRUIDIC))
          druCount++;
        if (TEFF(eID)->Sources[0] && !TEFF(eID)->BaseChance)
          enchCount++;
        if (TEFF(eID)->BaseChance)
          spCount++;
      }
    
    s +=        "-----TOTAL RESOURCES-----\n";
    s += Format("Monsters..............%d\n",monCount);
    s += Format("Material Items........%d\n",itemCount);
    s += Format("Magical Items.........%d\n",enchCount);
    s += Format("Templates.............%d\n",temCount);
    s += Format("Poisons...............%d\n",poiCount);
    s += Format("Diseases..............%d\n",disCount);
    s += Format("Trap Types............%d\n",trapCount);
    s += Format("Domains...............%d\n",domCount);
    s +=        "-------------------------\n";
    s += Format("Terrain Types.........%d\n",terCount);
    s += Format("Region Types..........%d\n",regCount);
    s +=        "-------------------------\n";  
    s += Format("Arcane Spells.........%d\n",magCount);
    s += Format("Divine Spells.........%d\n",divCount);
    s += Format("Druid Spells..........%d\n",druCount);
    //s += Format("Domain Spells.........%d\n",spCount - (magCount+divCount+druCount));
    s += Format("Total Spells..........%d\n\n",spCount);
    s +=        "-------------------------\n";  
    s += Format("HasStati(x,-1) calls..%d\n",ccHasNatStati);
    s += Format("HasStati(x,y) calls...%d\n",ccHasValStati);
    s += Format("HasEffStati calls.....%d\n",ccHasEffStati);
    s += Format("HasStati==true calls..%d\n",ccHasStatiTrue);
    s += Format("HasStati==false calls.%d\n",ccHasStatiFalse);
    s += Format("Perceive's HS calls...%d\n",ccHSPerceive);
    s += Format("CalcVal's HS calls....%d\n",ccHSCalcVal);
    s +=        "-------------------------\n";  
    
    int32 * sorted = new int32[256];
    memcpy(sorted,ccStatiNat,256*sizeof(int32));
    qsort(sorted,256,sizeof(int32),compare_int32);
    for(i=0;i!=256;i++)
      if (ccStatiNat[i] >= sorted[32])
        {
          s2 = Lookup(STATI_CONSTNAMES,i);
          while (s2.GetLength() < 22)
            s2 += ".";
          s2 += Format("%d\n",ccStatiNat[i]);
          s += s2;
        }
    delete[] sorted;
    
    if (!GetPlayer(0))
      goto DoneStats;
    
    Map *m;
    Creature *cr; Item *it; Container *cn; Thing *t;
    int16 mCount, iCount, cCount, tCount, fCount,
         dCount, sCount, niCount;
    m = GetPlayer(0)->m;
    
    mCount = iCount = cCount =
      tCount = fCount = dCount = 
      sCount = niCount = 0;
    
    fCount = (int16)m->Fields.Total();
    for (t=m->FirstThing();t;t=m->NextThing())
      {
        if (t->isCreature())
          {
            cr = (Creature*)t;
            mCount++;
            for(it=cr->FirstInv();it;it=cr->NextInv())
              niCount++;
          }
        if (t->isItem())
          {
            iCount++;
            if (t->isType(T_CONTAIN))
              {
                cn = (Container*)t;
                cCount++;
                for(it = oItem(cn->Contents);it;it = oItem(it->Next))
                  niCount++;
              }
          }
        if (t->isType(T_DOOR))
          dCount++;
        if (t->isType(T_PORTAL))
          sCount++;
      }
    
    
    s +=        "-----THE CURRENT MAP-----\n";  
    s += Format("Creatures.............%d\n",mCount);
    s += Format("Items on Floor........%d\n",iCount);
    s += Format("Nested Item Count.....%d\n",niCount);
    s += Format("Total Items...........%d\n",iCount + niCount);
    s += Format("Chests................%d\n",cCount);
    s += Format("Fields................%d\n",fCount);
    s += Format("Doors.................%d\n",dCount);
    s += Format("Stairs/Portals........%d\n",sCount);
    s +=        "-----SPELLS BY LEVEL-----\n";  
    

    int16 Levels[30];
    for (i=0;ST[i].Val;i++)
      {
        memset(Levels,0,sizeof(Levels));
        for(j=0;j!=theGame->LastSpell();j++)
          {
            TEffect *te = TEFF(theGame->SpellID(j));
            if (ST[i].Val < 0 ? te->HasSource((int8)(-ST[i].Val)) :
                 (te->HasSource(AI_WIZARDRY) && 
                   (te->Schools & ST[i].Val)))
              Levels[te->Level]++;
          }
        if (ST[i].Val == SC_ABJ)
          { Levels[1] += -7 + 2;
            Levels[2] += -4 + 2;
            Levels[3] += -3 + 3;
            Levels[4] += -4 + 2;
            Levels[5] +=      2;
            Levels[6] +=      2; }
        if (ST[i].Val == SC_ENC)
          { Levels[2]++; }
            
        s += Format("%s%2d/%2d/%2d/%2d/%2d/%2d = %d\n",
               ST[i].Text,Levels[1], Levels[2], Levels[3],
                          Levels[4], Levels[5], Levels[6],
                          Levels[1] + Levels[2] + Levels[3] +
                          Levels[4] + Levels[5] + Levels[6]);
      }
    
    DoneStats:
    return *tmpstr(s);
} 

bool Game::ResourceCompiler() {
    String fn;
    time_t theTime;

#ifdef DEBUG
    preprocFilename[0] = 0;

    if (sizeof(Dice) > sizeof(YYSTYPE)) {
        /* This should /never/ be true, but I am putting the test
        * here in case a wierd compiler assembles the Dice struct
        * wrong, or struct alignment options cause a problem. 
        * Without the test, this would result in wierd memory
        * bugs due to the kludgy way yyDice is defined.
        */
        Fatal("Strange compiler-level struct alignment error!");
    }

    char * file_to_open = NULL;

    T1->ChangeDirectory(T1->LibrarySubDir());

    do {
        yyin = NULL;
        Unresolved.Empty();
        T1->SetWin(WIN_SCREEN); T1->Color(EMERALD); T1->Clear();
        T1->Write(0,0,"Incursion Resource Compiler\n");
        T1->GotoXY(0,2); T1->Color(WHITE);
        T1->Write("Module Resource Files:\n"); T1->Color(GREY);
        file_to_open = T1->MenuListFiles("*.irc",MENU_ESC,"Compile Which Module?");
        T1->SetWin(WIN_SCREEN);
        if (!file_to_open) {
            T1->ChangeDirectory(T1->IncursionDirectory);
            return false; 
        }
        yyin = fopen(file_to_open, "rt");
        T1->GotoXY(0,6); 
    } while (!yyin);
    fclose(yyin);
    if (setjmp(jb)) {
        T1->Color(GREY);
        T1->Write("\n\nPress any key to continue...");
        T1->GetCharRaw(); T1->Clear();
        T1->ChangeDirectory(T1->IncursionDirectory);
        return false;
    }

    time(&theTime);
    T1->Write(Format("%s: Preprocessing ...\n", ctime(&theTime)));
    T1->Update();
#ifdef WIN32
    if (!Preprocess(file_to_open,"program.i")) {
#else
    if (0) {
#endif
PreprocError:
        T1->Color(RED);
        T1->Write("[Unknown Preprocessor Error.]\n");
        T1->GetCharRaw();
        T1->ChangeDirectory(T1->IncursionDirectory);
        return false;
    }
    time(&theTime);
    T1->Write(Format("%s: Reading preprocessed file ...\n", ctime(&theTime)));
    T1->Update();
    yyin = fopen("program.i","rt");
    if (!yyin)
        goto PreprocError;

    //ProcessFile:
    if (setjmp(jb)) {
        T1->Color(GREY);
        T1->Write("\n\nPress any key to continue...");
        T1->GetCharRaw(); T1->Clear();
        T1->ChangeDirectory(T1->IncursionDirectory);
        return false;
    }

    Errors = 0;
    fseek(yyin,0,SEEK_SET);
    yyrestart(yyin);

    Modules[0] = new Module;
    theModule = Modules[0];
    theModule->Slot = -1;

    time(&theTime);
    T1->Write(Format("%s: Counting and naming resources ...\n", ctime(&theTime)));
    T1->Update();
    CountResources();
    theMon = firstMon = theModule->QMon;
    theItem = firstItem = theModule->QItm;
    theFeat = firstFeat = theModule->QFea;
    theEff = firstEff = theModule->QEff;
    theArti = firstArti = theModule->QArt;
    theQuest = firstQuest = theModule->QQue;
    theDgn = firstDgn = theModule->QDgn;
    theRou = firstRou = theModule->QRou;
    theNPC = firstNPC = theModule->QNPC;
    theClass = firstClass = theModule->QCla;
    theRace = firstRace = theModule->QRac;
    theDom = firstDom = theModule->QDom;
    theGod = firstGod = theModule->QGod;
    theReg = firstReg = theModule->QReg;
    theTer = firstTer = theModule->QTer;
    theTxt = firstTxt = theModule->QTxt;
    theVar = firstVar = theModule->QVar;
    theTemp = firstTemp = theModule->QTem;
    theFlavor = firstFlavor = theModule->QFla;
    theBev = firstBev = theModule->QBev;
    theEnc = firstEnc = theModule->QEnc;
    theText = firstText = theModule->QTxt;
    theCodeSeg.Clear();
    memset(RegsInUse,0,sizeof(int8)*64);

    if (!Errors) {
        yypos = 1;
        fseek(yyin,0,SEEK_SET);
        yyrestart(yyin);

        T1->Color(GREY);
        theSymTab.Empty();
        time(&theTime);
        T1->Write(Format("%s: Parsing resources ...\n", ctime(&theTime)));
        T1->Update();
        brace_level = 0;
        decl_state = false;
        yyparse();
        time(&theTime);
        T1->Write((const char*)Format("%s: Parsed [%s] into [%s] ...\n",ctime(&theTime),file_to_open,theModule->GetText(theModule->Name)));
        if (!theModule->Name)
            yyerror("Module has no name.");
        if (!theModule->FName)
            yyerror("Module has no filename.");
    } else {
        T1->Color(RED);
        T1->Write("[Compilation Aborted.]\n");
    }

    theCodeSeg.Generate(HALT);
    theModule->szCodeSeg = theCodeSeg.GetSize();
    theModule->QCodeSeg = (VCode*)calloc(sizeof(VCode), theModule->szCodeSeg + 1);
    new VCode[theModule->szCodeSeg + 1];
    memcpy(theModule->QCodeSeg,theCodeSeg.GetCode(), theModule->szCodeSeg*sizeof(VCode));
    theCodeSeg.Clear();

    if (Unresolved.GetLength()) {
        T1->Color(CYAN);
        T1->Write("Warning -- Unresolved References:\n__");
        T1->Color(GREY);
        T1->Write(Unresolved);
        T1->Write("\n\n");
    } 

    time(&theTime);
    T1->Write(Format("%s: Caching TMonster MTypes ...\n", ctime(&theTime)));
    T1->Update();
    for (int i=0; i<theModule->szMon; i++) {
        rID mID = theModule->MonsterID(i); 
        theModule->QMon[i].InitializeMTypeCache(mID); 
    } 

    /* Don't save when we're only test-compiling one file for syntax */
    if (FIND("mage") && FIND("shocker lizard") && !Errors) {
        time(&theTime);
        T1->Write(Format("%s: Generating dispatch tables ...\n", ctime(&theTime)));
        T1->Update();
        T1->ChangeDirectory(T1->IncursionDirectory);
        GenerateDispatch();
        theSymTab.Empty();
        time(&theTime);
        T1->Write(Format("%s: Saving module ...\n", ctime(&theTime)));
        T1->Update();
        theGame->SaveModule(0);
    } else {
        time(&theTime);
        T1->Write(Format("%s: Forgoing save.\n", ctime(&theTime)));
        T1->Update();
    }

    T1->Color(EMERALD);
    time(&theTime);
    T1->Write(Format("\n%s: Press any key to continue...", ctime(&theTime)));
    T1->GetCharRaw();
    Cleanup();
    T1->Clear();
    T1->SetWin(WIN_SCREEN); 
    T1->Clear();
#endif
    return (!Errors); 
}

/* void Game::CountResources()
 * 
 *   This function is essentially a "first pass" in the "two pass"
 *  resource compiler. The first pass doesn't really parse at all;
 *  it just counts the resources of each type, and then allocates
 *  the arrays in which they will be stored. It also initializes
 *  the names of resources, which are rewritten (hopefully identically)
 *  later out of paranoia. This allows us to refer to resources in
 *  a resource file before they are fully defined, which can be
 *  very useful at times. We need to use the ugly SegArrays because
 *  we have no idea how many resources of each type we'll be reading
 *  in until the first pass is finished.
 */
void Game::CountResources()
  {
    int16 brack = 0, i;
    NArray<hText,30,30> MonNames, ItemNames,
      FeatNames, EffNames, ArtNames, QuestNames,
      DunNames, NPCNames, ClassNames, RaceNames,
      DomNames, GodNames, RegNames, TerNames,
      TextNames, VarNames, TempNames, RoutNames,
      FlavNames, BevNames, EncNames;
    textSeg.Empty();
    while(1)
      switch(yylex())
        {
          case '{': brack++; break;
          case '}': brack--; break;
          case MONSTER: if (!brack) { yylex();
                          
                          MonNames.Set(yylval,theModule->szMon++); } 
                        break;
          case ITEM:    if (!brack) { yylex();
                          ItemNames.Set(yylval,theModule->szItm++); } 
                        break;
          case FEATURE: if (!brack) { yylex();
                          FeatNames.Set(yylval,theModule->szFea++); } 
                        break;
          case EFFECT:
          case DISEASE:
          case POISON:
          case SPELL:   if (!brack) { yylex();
                          EffNames.Set(yylval,theModule->szEff++); } 
                        break;
          case ARTIFACT:if (!brack) { yylex();
                          ArtNames.Set(yylval,theModule->szArt++); } 
                        break;
          case QUEST:
                        if (!brack) { yylex();
                          QuestNames.Set(yylval,theModule->szQue++); } 
                        break;
          case DUNGEON: if (!brack) { yylex();
                          DunNames.Set(yylval,theModule->szDgn++); } 
                        break;
          case NPC:     if (!brack) { yylex();
                          NPCNames.Set(yylval,theModule->szNPC++); } 
                        break;
          
          case CLASS:   if (!brack) { yylex();
                          ClassNames.Set(yylval,theModule->szCla++); } 
                        break;
          case RACE:    if (!brack) { yylex();
                          RaceNames.Set(yylval,theModule->szRac++); } 
                        break;
          case DOMAIN:  if (!brack) { yylex();
                          DomNames.Set(yylval,theModule->szDom++); } 
                        break;
          case GOD:     if (!brack) { yylex();
                          GodNames.Set(yylval,theModule->szGod++); } 
                        break;
          case REGION:  if (!brack) { yylex();
                          RegNames.Set(yylval,theModule->szReg++); } 
                        break;
          case TERRAIN: if (!brack) { yylex();
                          TerNames.Set(yylval,theModule->szTer++); } 
                        break;
          case TEXT:    if (!brack) { yylex();
                          TextNames.Set(yylval,theModule->szTxt++); } 
                        break;
          case WORD_TEMPLATE: if (!brack) { yylex();
                          TempNames.Set(yylval,theModule->szTem++); } 
                        break;
          case FLAVOR:  if (!brack) { yylex();
                          FlavNames.Set(yylval,theModule->szFla++); } 
                        break;
          case BEHAVIOUR:if (!brack) { yylex();
                          BevNames.Set(yylval,theModule->szBev++); } 
                        break;
          case WORD_ENCOUNTER:if (!brack) { yylex();
                          EncNames.Set(yylval,theModule->szEnc++); } 
                        break;
          case EOF: case 0:
            goto DoneScan;
          default:
            ;
        }
    DoneScan:
    /* ww: the parser will give a better error report later 
    if (brack)
      yyerror("Mismatched brackets starting at line -1.");
      */
    if (Errors)
      return;
    
    if (theModule->szMon)
      theModule->QMon = new TMonster[theModule->szMon + 2];
    for(i=0;i!=theModule->szMon;i++)
      theModule->QMon[i].Name = MonNames[i];

    if (theModule->szItm)
      theModule->QItm = new TItem[theModule->szItm + 2];
    for(i=0;i!=theModule->szItm;i++)
      theModule->QItm[i].Name = ItemNames[i];


    if (theModule->szFea)
      theModule->QFea = new TFeature[theModule->szFea + 2];
    for(i=0;i!=theModule->szFea;i++)
      theModule->QFea[i].Name = FeatNames[i];

    if (theModule->szEff)
      theModule->QEff = new TEffect[theModule->szEff + 2];
    for(i=0;i!=theModule->szEff;i++)
      theModule->QEff[i].Name = EffNames[i];

    if (theModule->szArt)
      theModule->QArt = new TArtifact[theModule->szArt + 2];
    for(i=0;i!=theModule->szArt;i++)
      theModule->QArt[i].Name = ArtNames[i];

    if (theModule->szQue)
      theModule->QQue = new TQuest[theModule->szQue + 2];
    for(i=0;i!=theModule->szQue;i++)
      theModule->QQue[i].Name = QuestNames[i];

    if (theModule->szDgn)
      theModule->QDgn = new TDungeon[theModule->szDgn + 2];
    for(i=0;i!=theModule->szDgn;i++)
      theModule->QDgn[i].Name = DunNames[i];

    if (theModule->szRou)
      theModule->QRou = new TRoutine[theModule->szRou + 2];
    for(i=0;i!=theModule->szRou;i++)
      theModule->QRou[i].Name = RoutNames[i];

    if (theModule->szNPC)
      theModule->QNPC = new TNPC[theModule->szNPC + 2];
    for(i=0;i!=theModule->szNPC;i++)
      theModule->QNPC[i].Name = NPCNames[i];

    if (theModule->szCla)
      theModule->QCla = new TClass[theModule->szCla + 2];
    for(i=0;i!=theModule->szCla;i++)
      theModule->QCla[i].Name = ClassNames[i];

    if (theModule->szRac)
      theModule->QRac = new TRace[theModule->szRac + 2];
    for(i=0;i!=theModule->szRac;i++)
      theModule->QRac[i].Name = RaceNames[i];

    if (theModule->szDom)
      theModule->QDom = new TDomain[theModule->szDom + 2];
    for(i=0;i!=theModule->szDom;i++)
      theModule->QDom[i].Name = DomNames[i];

    if (theModule->szGod)
      theModule->QGod = new TGod[theModule->szGod + 2];
    for(i=0;i!=theModule->szGod;i++)
      theModule->QGod[i].Name = GodNames[i];

    if (theModule->szReg)
      theModule->QReg = new TRegion[theModule->szReg + 2];
    for(i=0;i!=theModule->szReg;i++)
      theModule->QReg[i].Name = RegNames[i];

    if (theModule->szTer)
      theModule->QTer = new TTerrain[theModule->szTer + 2];
    for(i=0;i!=theModule->szTer;i++)
      theModule->QTer[i].Name = TerNames[i];

    if (theModule->szTxt)
      theModule->QTxt = new TText[theModule->szTxt + 2];
    for(i=0;i!=theModule->szTxt;i++)
      theModule->QTxt[i].Name = TextNames[i];

    if (theModule->szVar)
      theModule->QVar = new TVariable[theModule->szVar + 2];
    for(i=0;i!=theModule->szVar;i++)
      theModule->QVar[i].Name = VarNames[i];

    if (theModule->szTem)
      theModule->QTem = new TTemplate[theModule->szTem + 2];
    for(i=0;i!=theModule->szTem;i++)
      theModule->QTem[i].Name = TempNames[i];

    if (theModule->szFla)
      theModule->QFla = new TFlavor[theModule->szFla + 2];
    for(i=0;i!=theModule->szFla;i++)
      theModule->QFla[i].Name = FlavNames[i];

    if (theModule->szBev)
      theModule->QBev = new TBehaviour[theModule->szBev + 2];
    for(i=0;i!=theModule->szBev;i++)
      theModule->QBev[i].Name = BevNames[i];

    if (theModule->szEnc)
      theModule->QEnc = new TEncounter[theModule->szEnc + 2];
    for(i=0;i!=theModule->szEnc;i++)
      theModule->QEnc[i].Name = EncNames[i];


    theModule->szTextSeg = textSeg.GetLength();
    theModule->QTextSeg = AllocTextSeg();

    firstTile = new LocationInfo[127];
    currMap = 0;
  }

const char* AllocTextSeg()
  {
    char* ts; int32 i,l;
    if (!textSeg.GetLength())
      return strdup("");
    ts = strdup(textSeg);
    l  = strlen(ts);
    for(i=0;i!=l;i++)
      if (ts[i] == 1)
        ts[i] = 0;
    textSeg.Empty();
    return ts;
  }

void Module::AddDebugInfo(int32 ID, Binding *b)
  {
    static DebugInfo d;
    if (1) // if (include_debug_info)
      {
        memset(&d,0,sizeof(d));
        strncpy(d.Ident,theSymTab[ID],31);
        d.BType = b->type;
        d.xID   = b->xID;
        d.Event = b->Event;
        switch (b->type)
          {
            case SYS_FUNC:
              d.Address = ((BSysFunc*)b)->funcid;
             break;
            case LOC_VAR:
              d.Address = ((BLocalVar*)b)->StackOffset;
              d.DataType = ((BLocalVar*)b)->VarType;
             break;
            case GLOB_VAR:
              d.Address = ((BGlobalVar*)b)->Address;
              d.DataType = ((BGlobalVar*)b)->VarType;
             break;
            case RES_VAR:
              d.Address = ((BResVar*)b)->Address;
              d.DataType = ((BResVar*)b)->VarType;
             break; 
            case MEM_FUNC:
              d.Address = ((BMemFunc*)b)->funcid;
             break;
            case MEM_VAR:
              d.Address = ((BMemVar*)b)->varid;
             break;
          } 
        Symbols.Add(d);
      }
  }

void GenerateDispatch() {
    int16 i,j,k; FILE *fp; String After;
    BMemFunc *b; BMemVar *b2;
    BResFunc *rb; BResMem *rb2;

    const char *ResTypes[] = { "T_TMONSTER", "T_TITEM", "T_TFEATURE", "T_TEFFECT",
        "T_TARTIFACT", "T_TQUEST", "T_TDUNGEON", "T_TROUTINE", "T_TNPC", "T_TCLASS",
        "T_TRACE", "T_TDOMAIN", "T_TGOD", "T_TREGION", "T_TTERRAIN", "T_TTEXT",
        "T_TVARIABLE", "T_TTEMPLATE", "T_TFLAVOR", "T_MODULE", "T_ANNOT","T_RESOURCE"  };
    const char *ResMacros[] = { "TMON", "TITEM", "TFEAT", "TEFF", "TART", "TQUEST",
        "TDUN", "TROU", "TNPC", "TCLASS", "TRACE", "TDOM", "TGOD", "TREG", "TTER",
        "TTEX", "TVAR", "TTEM", "TFLA" };

    /* HACKFIX */
    fp = fopen((const char*)Format(
#ifdef WIN32
        "%s\\dispatch.h"
#else
        "%s/dispatch.h"
#endif  
        ,(const char*)T1->LibrarySubDir()),"wt");
    if (!fp)
        Error("Can't open LIBDIR\\dispatch.h");
    fprintf(fp,
        "/* This file generated by the resource compiler -- do not modify! */\n"
        "\n#define DISPATCH\n"
        "\n\n\n"      
        "void VMachine::CallMemberFunc(int16 funcid, hObj h, int8 n) {\n"
        "    static Object *t;\n"
        "    static String str;\n\n"
        "    switch(funcid) {\n");
    for(i=0;i!=theSymTab.GetLastID();i++) {
        b = (BMemFunc*)theSymTab.GetBinding(i,0,0,MEM_FUNC);
        if (!b)
            b = (BMemFunc*)theSymTab.GetBinding(i,0,0,SYS_FUNC);
        if (b) {
Repeat:
            fprintf(fp,"    case %d:\n",b->funcid);
            if (b->type == MEM_FUNC && b->OType == T_STRING)
                fprintf(fp,"        ASSERT(0 > h);\n        ASSERT(h > -63);\n");
            else if (b->type == MEM_FUNC && b->OType != T_TERM)
                fprintf(fp,"        VERIFY(h,%s,\"%s\")\n",Lookup(T_CONSTNAMES,b->OType),theSymTab[i]);
            fprintf(fp,"        ASSERT(REGS(63) >= %d)\n", b->ParamCount);

            switch(b->ReturnType) {
            case DT_VOID:
                fprintf(fp,"        ");
                break;
            case DT_RID:
            case DT_INT16:
            case DT_INT32:
            case DT_HTEXT:
                fprintf(fp,"        REGS(n) = ");
                break;
            case DT_HOBJ:
                fprintf(fp,"        t = ");
                break;
            case DT_STRING:
                fprintf(fp,"        GETSTR(-1) = ");
                break;
            }

            if (b->type == MEM_FUNC)
                switch(b->OType) {
                case T_MAP:       fprintf(fp,"oMap(h)->"); break;
                case T_OBJECT:    fprintf(fp,"oObject(h)->"); break;
                case T_THING:     fprintf(fp,"oThing(h)->"); break;
                case T_PLAYER:    fprintf(fp,"oPlayer(h)->"); break;
                case T_CREATURE:  fprintf(fp,"oCreature(h)->"); break;
                case T_CHARACTER: fprintf(fp,"oCharacter(h)->",j); break; 
                case T_MONSTER:   fprintf(fp,"oMonster(h)->",j); break; 
                case T_ITEM:      fprintf(fp,"oItem(h)->"); break;
                case T_FEATURE:   fprintf(fp,"oFeature(h)->"); break;
                case T_PORTAL:    fprintf(fp,"oPortal(h)->"); break;
                case T_DOOR:      fprintf(fp,"oDoor(h)->"); break;                            
                case T_TRAP:      fprintf(fp,"oTrap(h)->"); break;                            
                case T_GAME:      fprintf(fp,"oGame(h)->"); break;                            
                case T_CONTAIN:   fprintf(fp,"oContain(h)->"); break;
                case T_TERM:      fprintf(fp,"T1->"); break;
                case T_EVENTINFO: fprintf(fp,"pe->"); break;
                case T_STRING:    fprintf(fp,"GETSTR(-h).");        
                }
            fprintf(fp,"%s(", theSymTab[i]); 
            for(k=0;k!=b->ParamCount;k++) {
                if (k)
                    fprintf(fp,", ");
                j = k+1;
                switch(b->ParamType[k]) {
                case DT_BOOL:
                    fprintf(fp,"STACK(%d)!=0",j);
                    break;
                case DT_INT16:
                    fprintf(fp,"(int16)STACK(%d)",j);
                    break;
                case DT_RID:
                case DT_INT32:
                    fprintf(fp,"STACK(%d)",j);
                    break;
                case DT_STRING:
                    fprintf(fp,"GETSTR(STACK(%d))",j);
                    break;
                case DT_HTEXT:
                    fprintf(fp,"GETSTR(STACK(%d))",j);
                    break;
                case DT_RECT:
                    fprintf(fp,"CAST_RECT(STACK(%d))",j);
                    break;
                case DT_HOBJ:
                    switch(b->ParamOType[k]) {
                    case T_EVENTINFO: fprintf(fp,"*pe"); break;
                    case T_MAP: fprintf(fp,"oMap(STACK(%d))",j); break;
                    case T_OBJECT:    fprintf(fp,"oObject(STACK(%d))",j); break;
                    case T_THING: fprintf(fp,"oThing(STACK(%d))",j); break;
                    case T_PLAYER: fprintf(fp,"oPlayer(STACK(%d))",j); break; 
                    case T_CREATURE: fprintf(fp,"oCreature(STACK(%d))",j); break;
                    case T_CHARACTER: fprintf(fp,"oCharacter(STACK(%d))",j); break; 
                    case T_MONSTER:   fprintf(fp,"oMonster(STACK(%d))",j); break; 
                    case T_ITEM: fprintf(fp,"oItem(STACK(%d))",j); break;
                    case T_FEATURE:   fprintf(fp,"oFeature(STACK(%d))",j); break;
                    case T_PORTAL:    fprintf(fp,"oPortal(STACK(%d))",j); break;
                    case T_DOOR:      fprintf(fp,"oDoor(STACK(%d))",j); break;
                    case T_TRAP:      fprintf(fp,"oTrap(STACK(%d))",j); break;
                    case T_GAME:      fprintf(fp,"oGame(STACK(%d))",j); break;                            
                    case T_CONTAIN: fprintf(fp,"oContain(STACK(%d))",j); break; 
                    case T_TERM: fprintf(fp,"T1",j); break;     
                    default:          fprintf(fp,"STACK(%d)",j); break;
                    }
                }
            }
            if (b->isVarParam)
                fprintf(fp,",\n                      VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d)",
                j+1,j+2,j+3,j+4,j+5,j+6,j+7,j+8);
            if (b->ReturnType == DT_HOBJ)
                fprintf(fp,");\n        REGS(n) = t ? t->myHandle : 0;\n        return;\n");
            else
                fprintf(fp,");\n        return;\n");
        }

        if (b && b->type == MEM_FUNC)
            if (b = (BMemFunc*)theSymTab.GetBinding(i,0,0,SYS_FUNC))
                goto Repeat;

        rb = (BResFunc*)theSymTab.GetBinding(i,0,0,RES_FUNC);
        if (!rb)
            continue;
        fprintf(fp,"    case %d:\n",rb->funcid);
        while (rb) {
            if (rb->type == RES_FUNC) {          
                /* This is a hideous kludge -- IS currently doesn't support functions
                attached to the base resource type for some convoluted reason I 
                don't remember right now, so we hardcode in that the test for correct
                resource type doesn't apply to functions called "GetConst", because
                we need it for multiple resource types. */
                fprintf(fp,"        if (RES(h)->Type == %s%s)\n",ResTypes[rb->RType-T_TMONSTER],
                    strcmp(theSymTab[i],"GetConst") ? "" : " || 1" );
                switch(rb->ReturnType) {
                case DT_VOID:
                    fprintf(fp,"          { ");
                    break;
                case DT_RID:
                case DT_INT32:
                case DT_INT16:
                case DT_HTEXT:
                    fprintf(fp,"          { REGS(n) = ");
                    break;
                case DT_HOBJ:
                    fprintf(fp,"          { t = ");
                    break;            
                case DT_STRING:
                    fprintf(fp,"          { REGS(n) = RegString(");
                    break;
                }
                fprintf(fp,"%s(h)->%s(",
                    ResMacros[rb->RType-T_TMONSTER],theSymTab[i]);
                for(k=0;k!=rb->ParamCount;k++) {
                    if (k)
                        fprintf(fp,", ");
                    j = k+1;
                    switch(rb->ParamType[k]) {
                    case DT_BOOL:
                        fprintf(fp,"STACK(%d)!=0",j);
                        break;
                    case DT_INT16:
                        fprintf(fp,"(int16)STACK(%d)",j);
                        break;
                    case DT_RID:
                    case DT_INT32:
                        fprintf(fp,"STACK(%d)",j);
                        break;
                    case DT_STRING:
                        fprintf(fp,"GETSTR(STACK(%d))",j);
                        break;
                    case DT_HTEXT:
                        /* Assume expr == hText... for now. */
                        fprintf(fp,"GETSTR(STACK(%d))",j);
                        break;
                    case DT_RECT:
                        fprintf(fp,"CAST_RECT(STACK(%d))",j);
                        break;
                    case DT_HOBJ:
                        switch(rb->ParamOType[k]) {
                        case T_EVENTINFO: fprintf(fp,"*pe"); break;
                        case T_MAP: fprintf(fp,"oMap(STACK(%d))",j); break;
                        case T_OBJECT:    fprintf(fp,"oObject(STACK(%d))",j); break;
                        case T_THING: fprintf(fp,"oThing(STACK(%d))",j); break;
                        case T_PLAYER: fprintf(fp,"oPlayer(STACK(%d))",j); break; 
                        case T_CREATURE: fprintf(fp,"oCreature(STACK(%d))",j); break;
                        case T_CHARACTER: fprintf(fp,"oCharacter(STACK(%d))",j); break; 
                        case T_MONSTER:   fprintf(fp,"oMonster(STACK(%d))",j); break; 
                        case T_ITEM: fprintf(fp,"oItem(STACK(%d))",j); break;
                        case T_FEATURE:   fprintf(fp,"oFeature(STACK(%d))",j); break;
                        case T_PORTAL:    fprintf(fp,"oPortal(STACK(%d))",j); break;
                        case T_DOOR:      fprintf(fp,"oDoor(STACK(%d))",j); break;
                        case T_TRAP:      fprintf(fp,"oTrap(STACK(%d))",j); break;
                        case T_GAME:      fprintf(fp,"oGame(STACK(%d))",j); break;                            
                        case T_CONTAIN: fprintf(fp,"oContain(STACK(%d))",j); break; 
                        case T_TERM: fprintf(fp,"T1",j); break;     
                        default:          fprintf(fp,"STACK(%d)",j); break;
                        }
                    }
                    if (rb->isVarParam)
                        fprintf(fp,",\n                      VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d),VSTACK(%d)",
                        j+1,j+2,j+3,j+4,j+5,j+6,j+7,j+8);
                    if (rb->ReturnType == DT_STRING)
                        fprintf(fp,")");
                    if (rb->ReturnType == DT_HOBJ)
                        fprintf(fp,");\n          REGS(n) = t ? t->myHandle : 0; return; }");
                    else
                        fprintf(fp,"); return; }\n");


                }
                rb = (BResFunc*)rb->next;
            }
            fprintf(fp,"        Error(\"Incorrect Resource Type: %%s->%s\",NAME(h));\n",theSymTab[i]);
            fprintf(fp,"       break;\n");
        }
    }
    fprintf(fp,"      default:\n"
        "        Error(\"Illegal member function call!\");\n"
        "      }\n"
        "  }\n\n\n");

    fprintf(fp,"void VMachine::GetMemberVar(int16 varid, hObj h, int8 n)"
        "\n  {\n    switch(varid)\n      {\n");
    for(i=0;i!=theSymTab.GetLastID();i++) {
        if (b2 = (BMemVar*)theSymTab.GetBinding(i,0,1,MEM_VAR)) {
            if (b2->VarType == DT_STRING)
                fprintf(fp,"        case %d:\n          GETSTR(-1) = ",b2->varid);
            else if (b2->VarType == DT_RECT)
                fprintf(fp,"        case %d:\n          REGS(n) = UNCAST_RECT(",b2->varid);
            else
                fprintf(fp,"        case %d:\n          REGS(n) = ",b2->varid);
            switch(b2->OType)
            { 
            case T_EVENTINFO: fprintf(fp,"(pe"); break;
            case T_MAP:       fprintf(fp,"(oMap(h)"); break;
            case T_OBJECT:    fprintf(fp,"(oObject(h)"); break;
            case T_THING:     fprintf(fp,"(oThing(h)"); break;
            case T_PLAYER:    fprintf(fp,"(oPlayer(h)"); break;
            case T_CREATURE:  fprintf(fp,"(oCreature(h)"); break;
            case T_CHARACTER: fprintf(fp,"(oCharacter(h)"); break; 
            case T_MONSTER:   fprintf(fp,"(oMonster(h)"); break; 
            case T_ITEM:      fprintf(fp,"(oItem(h)"); break;
            case T_FEATURE:   fprintf(fp,"(oFeature(h)"); break;
            case T_PORTAL:    fprintf(fp,"(oPortal(h)"); break;
            case T_DOOR:      fprintf(fp,"(oDoor(h)"); break;                            
            case T_TRAP:      fprintf(fp,"(oTrap(h)"); break;                            
            case T_GAME:      fprintf(fp,"(oGame(h)"); break;                            
            case T_CONTAIN:   fprintf(fp,"(oContain(h)"); break;
            case T_TERM:      fprintf(fp,"(T1"); break;
            }
            if (!strcmp(theSymTab[i],"m"))
                fprintf (fp, "->%s->myHandle);\n         break;\n",theSymTab[i]);
            else if (b2->VarType == DT_RECT)
                fprintf (fp, "->%s));\n         break;\n",theSymTab[i]);
            else 
                fprintf (fp, "->%s);\n         break;\n",theSymTab[i]);
        }
        rb2 = (BResMem*)theSymTab.GetBinding(i,0,1,RES_MEM);
        if (!rb2)
            continue;
        fprintf(fp,"        case %d:\n",rb2->varid);
        while (rb2) {
            if (rb2->type == RES_MEM) {          
                fprintf(fp,"          if (RES(h)->Type == %s || %s == T_RESOURCE)\n",
                    ResTypes[rb2->RType-T_TMONSTER], ResTypes[rb2->RType-T_TMONSTER]);
                fprintf(fp,"            { REGS(n) = %s(h)->%s; break; }\n",
                    ResMacros[rb2->RType-T_TMONSTER],theSymTab[i]);
            }
            rb2 = (BResMem*)rb2->next;
        }
        fprintf(fp,"          Error(\"Incorrect Resource Type: %%s->%s\",NAME(h));\n",theSymTab[i]);
        fprintf(fp,"         break;\n");
    }

    fprintf(fp,"      default:\n"
        "        Error(\"Illegal member variable access!\");\n"
        "      }\n"
        "  }\n\n\n");

    fprintf(fp,"void VMachine::SetMemberVar(int16 varid, hObj h, int32 val)"
        "\n  {\n    switch(varid)\n      {\n");
    for(i=0;i!=theSymTab.GetLastID();i++)
        if (b2 = (BMemVar*)theSymTab.GetBinding(i,0,1,MEM_VAR)) {
            fprintf(fp,"        case %d:\n          ",b2->varid);
            switch(b2->OType) { 
            case T_EVENTINFO: fprintf(fp,"pe"); break;
            case T_MAP:       fprintf(fp,"oMap(h)"); break;
            case T_OBJECT:    fprintf(fp,"oObject(h)"); break;
            case T_THING:     fprintf(fp,"oThing(h)"); break;
            case T_PLAYER:    fprintf(fp,"oPlayer(h)"); break;
            case T_CREATURE:  fprintf(fp,"oCreature(h)"); break;
            case T_CHARACTER: fprintf(fp,"oCharacter(h)",j); break; 
            case T_MONSTER:   fprintf(fp,"oMonster(h)",j); break; 
            case T_ITEM:      fprintf(fp,"oItem(h)"); break;
            case T_FEATURE:   fprintf(fp,"oFeature(h)"); break;
            case T_PORTAL:    fprintf(fp,"oPortal(h)"); break;
            case T_DOOR:      fprintf(fp,"oDoor(h)"); break;                            
            case T_TRAP:      fprintf(fp,"oTrap(h)"); break;                            
            case T_GAME:      fprintf(fp,"oGame(h)"); break;                            
            case T_CONTAIN:   fprintf(fp,"oContain(h)"); break;
            case T_TERM:      fprintf(fp,"T1"); break;
            }
            if (!strcmp(theSymTab[i],"m"))
                fprintf (fp, "->m = oMap(val);\n         break;\n");
            else if (b2->VarType == DT_STRING)
                fprintf (fp, "->%s = GETSTR(val);\n         break;\n",theSymTab[i]);
            else if (b2->VarType == DT_RECT)
                fprintf (fp, "->%s = CAST_RECT(val);\n         break;\n",theSymTab[i]);
            else
                fprintf (fp, "->%s = val;\n         break;\n",theSymTab[i]);
        }
    fprintf(fp,"      default:\n"
        "        Error(\"Illegal member variable access!\");\n"
        "      }\n"
        "  }\n\n\n");


    fclose(fp);
    return;
}

void VBlock::Generate(int8 opcode, int8 typ1, int32 param1, int8 typ2,int32 param2)
  {
    int8 sz = 1;
    if (size+3 >= space)
      {
        vc = (VCode*)x_realloc(vc,sizeof(VCode),space+10,space);
        space += 10;
      }                                                             
    
    /* Here we adjust for the fact that a jump opcode can take more
       than one word with a long parameter, thus invalidating the
       address it is to jump to. We adjust the address accordingly. */
    if (opcode == JUMP)
      {
        if (typ1 == RT_CONSTANT && param1 > 511)
          param1++;
      }  
    if (opcode == JTRU || opcode == JFAL)
      {
        if (typ2 == RT_CONSTANT && param2 > 0 && abs(param1) > 511)
          param2++;
        if (typ2 == RT_CONSTANT && param2 > 511)
          param2++;
      }
    vc[size].Opcode = opcode;
    if (param1 > 511 || param1 < -511) {
      vc[size].P1Type   = typ1 | RT_EXTENDED;
      vc[size].Param1 = 0;
      *((int32*)(&(vc[size+sz]))) = param1;
      sz++;
      }
    else {
      vc[size].P1Type   = typ1;
      vc[size].Param1 = param1;
      }

    if (param2 > 511 || param2 < -511) {
      vc[size].P2Type   = typ2 | RT_EXTENDED;
      vc[size].Param2 = 0;
      *((int32*)(&(vc[size+sz]))) = param2;
      sz++;
      }
    else {
      vc[size].P2Type   = typ2;
      vc[size].Param2 = param2;
      }

    size += sz;
  }
  
void VBlock::ProcessContinues()
  {
    int32 i, j, rel_dest;
    start_again:
    for (i=0;i!=size;i++)
      {
        ASSERT(i < size);
        if (vc[i].Opcode == CONT)
          {
            vc[i].Opcode = JUMP;
            rel_dest = size - i;
            ASSERT(rel_dest > 0);
            if (rel_dest <= 511)
              {
                vc[i].Param1 = rel_dest;
                vc[i].P1Type = RT_CONSTANT;
              }
            else
              {
                if (size+1 >= space)
                  {
                    vc = (VCode*)x_realloc(vc,sizeof(VCode),space+10,space);
                    space += 10;
                  }
                size++;
                rel_dest++;
                memmove(&(vc[i+2]),&vc[i+1],size-i);
                *((int32*)(&(vc[i+1]))) = rel_dest;
                goto start_again;
              }       
          }
        else
          {
            j = i;
            if (vc[i].P1Type & RT_EXTENDED)
              j++;
            if (vc[i].P2Type & RT_EXTENDED)
              j++;
            i = j;
          }
      }
  
  }


bool AllowedCast(int16 from, int16 to)
  {
    if (from == DT_UNKNOWN || to == DT_UNKNOWN)
      return true;
    if (from == to)
      return true;
    if (from == DT_HTEXT && to == DT_STRING)
      return true;                          
    if (from == DT_STRING && to == DT_HTEXT)
      return true;                          
    if (from != DT_STRING && from != DT_VOID && to == DT_BOOL)
      return true;
    if (from == DT_HOBJ || from == DT_RID || from == DT_INT32 || from == DT_INT16)
      if (to == DT_BOOL)
        return true;    
    if (from == DT_HTEXT || from == DT_HOBJ || from == DT_STRING
          || from == DT_RID || from == DT_VOID)
      return false;
    if (to == DT_HTEXT || to == DT_HOBJ || to == DT_STRING
          || to == DT_RID || to == DT_VOID)
      return false;

    return true;
  }

PExp & CodeAssignment(int16 op, LExp &p1, PExp *p2)
  {
    static PExp ex;
    if (p1.Type == DT_HTEXT && p2->Type != DT_HTEXT)
      yyerror("Cannot assign to a string literal / text handle!");

    /* Post-increment/decrement */
    if (op == 'i' || op == 'd') {
      if (p1.Storage == RT_REGISTER)
        FreeRegister((int16)(p1.Value));
      }
    else {
      ex.Storage = p1.Storage;
      ex.Value   = p1.Value;
      }  


    if (p2) {
      if (!AllowedCast(p2->Type,p1.Type))
        yyerror(Format("Type mismatch: %s assigned to %s.",
          Lookup(DataTypeNames, p2->Type), Lookup(DataTypeNames,p1.Type))); 
      if (p2->Storage & RT_REGISTER)
        FreeRegister((int16)(p2->Value));
      }
    ex.Code = new VBlock;
    
    ex.Type = p1.Type;

    if (p1.Type == DT_STRING) {
      switch (op) {
        case '=':
          ex.Code->Add(p2->Code);
          ex.Code->Generate(MOV,RT_REGISTER,0,p2->Storage,p2->Value);
          ex.Code->Add(p1.WCode);
          ex.Type    = DT_STRING;
          if (p2->Storage == RT_CONSTANT)
            if (p2->Value < 0)
              FreeString((int16)(-p2->Value));
          goto Done;
         break;
        case ASSIGN_ADD:
          ex.Code->Add(p2->Code);
          ex.Code->Add(p1.RCode);
          ex.Code->Generate(ASTR,p1.Storage,p1.Value,
               p2->Storage,p2->Value);
          ex.Code->Generate(MOV,RT_REGISTER,0,RT_CONSTANT,-1);
          ex.Code->Add(p1.WCode);
          ex.Type    = DT_STRING;
          if (p2->Storage == RT_CONSTANT)
            if (p2->Value < 0)
              FreeString((int16)(-p2->Value));
          goto Done;
         break;
        default:
          yyerror("Unimplemented string operator!");
        }
      }

    
    if (op != '=')
      ex.Code->Add(p1.RCode);
    if (p2)
      ex.Code->Add(p2->Code);

    switch (op) {
      case '=':
        ex.Code->Generate(MOV,RT_REGISTER,0,p2->Storage,p2->Value);
       break;
      case 'i':
      case 'd':
        /* Store the /old/ value of the variable as the expression value */
        ex.Storage = RT_REGISTER;
        ex.Value   = AllocRegister();
        ex.Code->Generate(MOV,RT_REGISTER,ex.Value,p1.Storage,p1.Value);
        /* Increment or decrement the memory location */
        ex.Code->Generate(op=='i' ? ADD : SUB,p1.Storage,p1.Value,RT_CONSTANT,1);
       break;
      case 'I':
      case 'D':
        ex.Code->Generate(op=='i' ? ADD : SUB,p1.Storage,p1.Value,RT_CONSTANT,1);
       break;
      case ASSIGN_ADD:
        ex.Code->Generate(ADD,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_SUB:
        ex.Code->Generate(SUB,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_MULT:
        ex.Code->Generate(MULT,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_DIV:
        ex.Code->Generate(DIV,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_MOD:
        ex.Code->Generate(MOD,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_NEG:
        ex.Code->Generate(NEG,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_AND:
        ex.Code->Generate(BAND,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_OR:
        ex.Code->Generate(BOR,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_LSHIFT:
        ex.Code->Generate(BSHL,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      case ASSIGN_RSHIFT:
        ex.Code->Generate(BSHR,p1.Storage,p1.Value,p2->Storage,p2->Value);
       break;  
      default:
        yyerror("Unimplemented assignment operator!");
      }  

    ex.Code->Add(p1.WCode);
    
    Done:
    delete p1.WCode;
    if (p1.RCode)
      delete p1.RCode;
    if (p2)
      if (p2->Code)
        delete p2->Code;
    // ww: you are returning the address of a stack allocated variable! (fixed)
    return ex;
  }

PExp & CodeOperator(int16 op, PExp &p1, PExp &p2)
  {
    static PExp ex; 
    bool unary = false, assign = false;
    
    if (p1.Storage & RT_REGISTER)
      FreeRegister((int16)p1.Value);
    if (p2.Storage & RT_REGISTER)
      FreeRegister((int16)p2.Value);    

    ex.Storage = RT_REGISTER;
    ex.Value   = AllocRegister();

    if (&p1 == &p2)
      unary = true;
    else if (!AllowedCast(p1.Type,p2.Type))
      yyerror(Format("Type mismatch: %s / %s.",
        Lookup(DataTypeNames, p1.Type), Lookup(DataTypeNames,p2.Type))); 

    if (op != AND_OP && op != OR_OP) {
      if (p1.Code) {
        ex.Code = p1.Code;
        if (p2.Code && &p1 != &p2) {
          ex.Code->Add(p2.Code);
          delete p2.Code;
          }
        }
      else if (p2.Code)
        ex.Code = p2.Code;
      else
        ex.Code = new VBlock;
      }

    if (p1.Type == DT_STRING || p1.Type == DT_HTEXT) {
      switch (op) {
        case '+':
          ex.Code->Generate(ASTR,p1.Storage,p1.Value,p2.Storage,p2.Value);
          ex.Storage = RT_CONSTANT;
          ex.Value   = -AllocString();
          ex.Type    = DT_STRING;
          ex.Code->Generate(WSTR,ex.Storage,ex.Value,RT_CONSTANT,-1);
          goto SkipAssign;
        default:
          yyerror("Illegal string operator.");
          ex.Storage = RT_CONSTANT;
          ex.Value = 0;
          goto SkipAssign;
        }
      }

    switch(op)
      {
        case '+':
          ex.Code->Generate(ADD,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case '-':
          if (unary)
            ex.Code->Generate(SUB,RT_CONSTANT,0, p1.Storage, p1.Value);
          else
            ex.Code->Generate(SUB,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case '~':
          ex.Code->Generate(NEG,p1.Storage, p1.Value);
         break;
        case '*':
          ex.Code->Generate(MULT,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case '/':
          ex.Code->Generate(DIV,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case '%':
          ex.Code->Generate(MOD,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case 'm':
          ex.Code->Generate(MIN,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case 'M':
          ex.Code->Generate(MAX,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case '&':
          ex.Code->Generate(BAND,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case '|':
          ex.Code->Generate(BOR,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        case AND_OP:
          if (p1.Code)
            ex.Code = p1.Code;
          else
            ex.Code = new VBlock;
          if (p2.Code) {
            ex.Code->Generate(CMEQ,p1.Storage,p1.Value,RT_CONSTANT,0);
            ex.Code->Generate(JTRU,RT_REGISTER, 0, RT_CONSTANT,1 + p2.Code->GetSize());
            ex.Code->Add(p2.Code);
            }
          ex.Code->Generate(LAND,p1.Storage,p1.Value, p2.Storage, p2.Value);
          ex.Type = DT_BOOL;
         break;
        case OR_OP:
          if (p1.Code)
            ex.Code = p1.Code;
          else
            ex.Code = new VBlock;
          if (p2.Code) {
            ex.Code->Generate(CMEQ,p1.Storage,p1.Value,RT_CONSTANT,0);
            ex.Code->Generate(JFAL,RT_REGISTER, 0, RT_CONSTANT,1 + p2.Code->GetSize());
            ex.Code->Add(p2.Code);
            }
          ex.Code->Generate(LOR,p1.Storage,p1.Value, p2.Storage, p2.Value);
          ex.Type = DT_BOOL;
         break;
        case '<':
          ex.Code->Generate(CMGT,p2.Storage,p2.Value, p1.Storage, p1.Value);
          ex.Type = DT_BOOL;
         break;
        case '>':
          ex.Code->Generate(CMGT,p1.Storage,p1.Value, p2.Storage, p2.Value);
          ex.Type = DT_BOOL;
         break;
        case GE_OP:
          ex.Code->Generate(CMGE,p1.Storage,p1.Value, p2.Storage, p2.Value);
          ex.Type = DT_BOOL;
         break;
        case LE_OP:
          ex.Code->Generate(CMGE,p2.Storage,p2.Value, p1.Storage, p1.Value);
          ex.Type = DT_BOOL;
         break;
        case EQ_OP:
          ex.Code->Generate(CMEQ,p1.Storage,p1.Value, p2.Storage, p2.Value);
          ex.Type = DT_BOOL;
         break;
        case NE_OP:
          ex.Code->Generate(CMEQ,p1.Storage,p1.Value, p2.Storage, p2.Value);
          ex.Code->Generate(CMEQ,RT_REGISTER,0,RT_CONSTANT,0);
          ex.Type = DT_BOOL;
         break;
        case LSHIFT_OP:
          ex.Code->Generate(BSHL,p1.Storage,p1.Value,p2.Storage,p2.Value);
         break;  
        case RSHIFT_OP:
          ex.Code->Generate(BSHR,p1.Storage,p1.Value,p2.Storage,p2.Value);
         break;  
        case '^': /* Always Unary */
          ex.Code->Generate(NEG,p1.Storage,p1.Value,RT_CONSTANT,0xFFFFFFFF);
          ex.Type = DT_BOOL;
         break;
        case '!': /* Always Unary */
          ex.Code->Generate(CMEQ,p1.Storage,p1.Value,RT_CONSTANT,0);
          ex.Type = DT_BOOL;
         break;
        case ':': /* Always Unary */
          ex.Code->Generate(ex.Storage,(int8)ex.Value,p1.Storage,(int8)p1.Value);
          ex.Code->Generate(CMGE,ex.Storage,ex.Value,RT_CONSTANT,0);          
          ex.Code->Generate(JTRU,RT_REGISTER,0,RT_CONSTANT,2);
          ex.Code->Generate(SUB,RT_CONSTANT,0,ex.Storage,ex.Value);
          ex.Code->Generate(MOV,ex.Storage,ex.Value,RT_REGISTER,0);
          goto SkipAssign;
        case 'd': /* the "roll dice" operator */
          ex.Code->Generate(ROLL,p1.Storage,p1.Value, p2.Storage, p2.Value);
         break;
        default:
          yyerror("Unimplemented operator!");

      }

    ex.Code->Generate(MOV,ex.Storage,ex.Value,RT_REGISTER,0);
    SkipAssign:
    // ww: you are returning the address of a stack-allocated variable! (fixed)
    return ex;
  }

PExp& CodeRectMember(PExp *rect, int16 id)
  {
    uint32 mask; int16 shift; 
    static PExp ex;
    if (id == theSymTab["x1"])
      { mask = 0x000000FF; shift = 0; }
    else if (id == theSymTab["x2"])
      { mask = 0x0000FF00; shift = 8; }
    else if (id == theSymTab["y1"])
      { mask = 0x00FF0000; shift = 16; }
    else if (id == theSymTab["y2"])
      { mask = 0xFF000000; shift = 24; }
    else {
      yyerror("Expected x1, x2, y1 or y2 as Rect member.");
      mask = 0x00000000; shift = 0;
      }

    ex.Code = rect->Code;
    if (!ex.Code)
      ex.Code = new VBlock;
    ex.Code->Generate(BAND,rect->Storage,rect->Value,RT_CONSTANT,mask);
    ex.Code->Generate(BSHL,RT_REGISTER,0,RT_CONSTANT,shift);
    if (rect->Storage == RT_REGISTER)
      FreeRegister((int16)rect->Value);
    ex.Storage = RT_REGISTER;
    ex.Value   = AllocRegister();
    ex.Code->Generate(MOV,ex.Storage,ex.Value,RT_REGISTER,0);
    return ex;
  }

LExp& CodeRectLVal(LExp *rect,int16 id)
  {
    uint32 mask; int16 shift, i; 
    static LExp lv;
    if (id == theSymTab["x1"])
      { mask = 0x000000FF; shift = 0; }
    else if (id == theSymTab["x2"])
      { mask = 0x0000FF00; shift = 8; }
    else if (id == theSymTab["y1"])
      { mask = 0x00FF0000; shift = 16; }
    else if (id == theSymTab["y2"])
      { mask = 0xFF000000; shift = 24; }
    else {
      yyerror("Expected x1, x2, y1 or y2 as Rect member.");
      mask = 0x00000000; shift = 0;
      }

    lv.RCode = rect->RCode;
    if (!lv.RCode)
      lv.RCode = new VBlock;
    lv.RCode->Generate(BAND,rect->Storage,rect->Value,RT_CONSTANT,mask);
    lv.RCode->Generate(BSHL,RT_REGISTER,0,RT_CONSTANT,shift);
    if (rect->Storage == RT_REGISTER)
      FreeRegister((int16)rect->Value);
    lv.Storage = RT_REGISTER;
    lv.Value   = AllocRegister();
    lv.RCode->Generate(MOV,lv.Storage,lv.Value,RT_REGISTER,0);


    lv.WCode = new VBlock;
    
    /* Save the shifted value for a rect member into a register */
    i = AllocRegister();
    lv.WCode->Generate(BSHR,RT_REGISTER,0,RT_CONSTANT,shift);
    lv.WCode->Generate(MOV,RT_REGISTER,i,RT_REGISTER,0);

    /* Read the entire Rect structure */
    lv.WCode->Add(rect->RCode);
    
    /* Mask out the old value of the member */
    lv.WCode->Generate(BAND,rect->Storage,rect->Value,RT_CONSTANT,~mask);
    
    /* Merge the old structure with the new value, result in R0 */    
    lv.WCode->Generate(BOR, RT_REGISTER, i, RT_REGISTER, 0);

    /* Save the new structure (in R0) back to it's origanal location */
    lv.WCode->Add(rect->WCode);

    
    return lv;
  }
int16 AllocRegister()
  {
    int16 i;
    for(i=1;i!=63;i++)
      if (!RegsInUse[i])
        {
          RegsInUse[i] = true;
          return i;
        }
    Error("All 62 registers in use!");
    return 1;
  }

void FreeRegister(int16 i)
  {
    RegsInUse[i] = false;
  }

int16 AllocString()
  {
    int16 i;
    for(i=2;i!=63;i++)
      if (!StringsInUse[i])
        {
          StringsInUse[i] = 1;
          return i;
        }
    Error("All 62 registers in use!");
    return 1;
  }         

void LockString(int16 i)
  {
    StringsInUse[i] = 2;
  }
void FreeString(int16 i)
  {
    if (StringsInUse[i] != 2)
      StringsInUse[i] = 0;
  }

void ClearStrings()
  {
    memset(StringsInUse,0,64);
  }


