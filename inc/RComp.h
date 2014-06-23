/* RCOMP.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains class definitions and constants
   related solely to the IncursionScript compiler. 
*/


#define HASH_SIZE 4096
#define SYMBOL_SEG 120000

enum btype { SYS_OBJ, LOC_VAR, MOD_FUNC, GLOB_VAR, MEM_VAR, MEM_FUNC, 
             RES_VAR, SYS_FUNC, RES_MEM, RES_FUNC, ANY };

#define DT_VOID   1
#define DT_INT32  2
#define DT_HOBJ   3
#define DT_HTEXT  4
#define DT_RID    5
#define DT_STRING 6
#define DT_BOOL   7
#define DT_RECT   8
#define DT_INT16  9
#define DT_INT8  10
#define DT_UNKNOWN 11


#define RT_CONSTANT    0
#define RT_REGISTER    1
#define RT_MEMORY      2
#define RT_REGMEM      3
#define RT_EXTENDED    4
#define RT_STACK       8
#define RT_MEMBER      16

#define sys_error       1
#define sys_fatal       2
#define sys_addstring   3
#define sys_str_left    4
#define sys_str_right   5
#define sys_copy_str    6
#define sys_set_str     7
#define sys_fmt_str     8
#define sys_alloc_str   9
#define sys_free_str    10
#define sys_empty_str   11
#define sys_first_thing 12
#define sys_next_thing  13
#define sys_first_creat 14
#define sys_next_creat  15
#define sys_first_item  16
#define sys_next_item   17
#define sys_thing_n     18

extern uint32 CompilerFlags, 
       MemFuncID,
       MemVarID,
       HeapHead,
       StackHead;


struct VCode
  {
    unsigned int Opcode:6;
    signed int  Param1:10;
    unsigned int P1Type:3;
    signed int  Param2:10;
    unsigned int P2Type:3;
  };


struct Binding
  {
    Binding() { next = NULL; xID = 0; Event = 0; }
    btype type;
    rID   xID;
    int16 Event;
    Binding *next;
    ~Binding()
      { if (next) delete next; }
  };

class SymbolTable
  {
    private:
      inline int32 hashval(const char*text);
      struct SymbolNode
        { 
          const char*text;
          int32 id;
          Binding *b;
          SymbolNode *left, *right, *next;
          ~SymbolNode()
            { if (next) delete next; 
              if (b) delete b; 
              if (text) free((void*)text); }
        };
      SymbolNode *HashTable[HASH_SIZE];
      SymbolNode * RefTable[HASH_SIZE];
      int32 LastID;
    public:
      SymbolTable();
      int32 GetLastID() { return LastID; }
      int32 operator[](const char*text);
      const char* operator[](int32 id);
      int32 Add(const char *text);
      void Bind(int32 id, Binding *b);
      void Empty();
      Binding *GetBinding(const char *text, rID xID=0, int16 Ev=0, btype type = ANY);
      Binding *GetBinding(int32 id, rID xID=0, int16 Ev=0, btype type = ANY);
  };

struct BSysFunc : public Binding
  {
    BSysFunc(int16 _funcid, int8 _ret)
      { funcid = _funcid; 
        ReturnType = _ret; 
        ParamCount = 0; 
        type = SYS_FUNC; 
        isVarParam = 0;
        HasDefault = 0;
        xID = 0;
        Event = 0; } 
    int16 funcid, ParamCount;
    int8  ReturnType;
    int8  ParamType[10];
    int8  ParamOType[10];
    int8  Default[10];
    int16 HasDefault;
    bool  isVarParam;
  };

struct BMemFunc : public BSysFunc
  {
    BMemFunc(int16 _funcid, int8 _ret, int8 _OType)
      : BSysFunc(_funcid, _ret)
      { OType = _OType; 
        type  = MEM_FUNC; } 
    int8  OType;
  };

struct BResFunc : public BSysFunc
  {
    BResFunc(int16 _funcid, int8 _ret, int8 _RType)
      : BSysFunc(_funcid, _ret)
      { RType = _RType; 
        type  = RES_FUNC; } 
    int8  RType;
  };


struct BMemVar : public Binding
  {
    int16 varid;
    int8 VarType;
    int8 OType;
  };

struct BResMem : public Binding
  {
    int16 varid;
    int8 VarType;
    int8 RType;
  };

struct BLocalVar : public Binding
  {
    int16 StackOffset;
    int8  VarType;
  };

struct BResVar : public Binding
  { 
    int32 Address;
    int8  VarType;
  };

struct BGlobalVar : public Binding
  {
    int32 Address;
    int8  VarType;
  };

struct BSysObj : public Binding
  {
    int16 ObjNum;
    int8  OType;
  };

class VBlock
  {
    private:
      int32 size, space;
      VCode *vc;
    public:
      VBlock()
        {
          size = 0; space = 5;
          vc = (VCode*)malloc(sizeof(VCode)*space);
          ASSERT(vc); 
        }
      ~VBlock()
        {
          //free(vc);
        }
      void Clear()
        {
          free(vc);
          size = 0; space = 5;
          vc = (VCode*)malloc(sizeof(VCode)*space);
          ASSERT(vc); 
        }

      VCode *GetCode() { return vc; }
      int32  GetSize() { return size; }
      int16  LastOpcode() { if (!size) return 0;
                            return vc[size-1].Opcode; }
      void Generate(int8 opcode, int8 typ1=0, int32 param1=0, int8 typ2=0,int32 param2=0);
      void GenDWord(int32 dw)
        { ASSERT(sizeof(int32) <= sizeof(VCode));
          Generate(0);
          size--;
          *((int32*)(&(vc[size++]))) = dw; }
      hCode Add(VBlock *b)
        {
          if (!b)
            return size;
          ASSERT(vc); 
          if (size + b->size >= space)
            {
              space += b->size;
              /* This is causing crashes, and I have no idea why!
                 vc = (VCode*)realloc(vc,sizeof(VCode)*space); */
              VCode * temp_vc;
              temp_vc = (VCode*)malloc(sizeof(VCode)*space);
              memcpy(temp_vc,vc,size * sizeof(VCode));
              free(vc);
              vc = temp_vc;
            }
          memcpy(&(vc[size]),b->vc,b->size * sizeof(VCode));
          size += b->size;
          return (size - b->size);
        }
      void TrimGratuitousMOV() {
        if (!size)
          return;
        if (vc[size-1].Opcode == MOV)
          if (vc[size-1].P1Type == RT_REGISTER)
            size--;
        return; 
        }
      void ProcessContinues();
        
  };

struct PExp
  {
    PExp() { Type = Storage = 0; Value = 0; Code = NULL; }
    VBlock* Code;
    int8 Type, Storage;
    int32 Value;
  };

struct LExp
  {
    void PExp() {
        Type = Storage = 0;
        Value = 0; 
        RCode = WCode = NULL;
    }
    VBlock *RCode, *WCode;
    int8 Type, Storage;
    int32 Value;
  };

extern VBlock theCodeSeg;

PExp& CodeOperator(int16 op, PExp &ex1,PExp &ex2);
PExp& CodeAssignment(int16 op, LExp &lv,PExp *val);
PExp& CodeRectMember(PExp *rect, int16 id);
LExp& CodeRectLVal(LExp *rect,int16 id);

void  FreeRegister(int16 reg);
int16 AllocRegister();
void  FreeString(int16 s);
void  LockString(int16 s);
void  ClearStrings();
bool  isStringLocked(int16 s);
int16 AllocString();
bool  AllowedCast(int16 from, int16 to);



