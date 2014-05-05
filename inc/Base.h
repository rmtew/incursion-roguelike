/* BASE.H -- Copyright (c) 1999-2003 Julian Mensch
     This file contains class definitions for Array, NArray,
   OArray, Dice, String, MVal, Rect, Dictionary, Fraction,
   Object and Registry. 
*/


#define BIT(bitnum) (1 << (bitnum - 1))
#define XBIT(a)     (1 << (a))

#define Check(a,b) { if (!(a)) Fatal(b);  }
#define iswhite(ch) ((ch) == 13 || (ch) == ' ' || (ch) == '\t' || \
                       (ch) == 10)
                       
#define isspace(ch) isspace(max(ch,1))
#define isalpha(ch) isalpha(max(ch,1))
#define isalnum(ch) isalnum(max(ch,1))
#define isdigit(ch) isdigit(max(ch,1))

#define SC(xx) (*tmpstr((const char*)xx))

template<class S, int32 Initial, int32 Delta>
  class Array
	{
		private:
			S *Items;          
			uint32 Size, Count;
			void Enlarge();
			void Reduce();
    protected:
      S* _Paren(uint32 index);

		public:
			Array();
			~Array() { if (Items) free(Items); }
      void Serialize(hObj myObj);
      void Set(S&,uint32 idx);
			int32 Add(S&);
			int32 Total() { return Count; }
      void Remove(uint32 i);
      S*   NewItem();
      void Serialize(Registry &r);
      void Clear() { Count = 0; }; 
	};

extern long ZeroValue;

template<class S,int32 Initial,int32 Delta>
  class NArray : public Array<S,Initial,Delta>
  {
    public:
      S& operator[](int32 index)
        { S *s = this->_Paren(index);
          ASSERT(!ZeroValue); 
          return s ? (*s) : *((S*)&ZeroValue); }
      void RemoveItem(S s)
        { Restart:
          for (int32 i=0;this->_Paren(i);i++)
            if (*this->_Paren(i) == s)
              { this->Remove(i); goto Restart; } }  
  };

template<class S,int32 Initial,int32 Delta>
  class OArray : public Array<S,Initial,Delta>
  {
    public:
      S* operator[](int32 index)
        { return this->_Paren(index); }
      void RemoveItem(S* s)
        { Restart:
          for (int32 i=0;this->_Paren(i);i++)
            if (this->_Paren(i) == s)
              { this->Remove(i); goto Restart; } }
  };


#define MapIterate(m,t,i)                  \
     for(i=0,t = (Creature*)oThing(m -> Things[0]); m && m->Things[ i ];           \
        i++, (t = m ? (Creature*)oThing(m-> Things[ i ]) : NULL))



void* x_realloc(void *block, size_t unit, size_t sz, size_t osz);

/* A String class similar to the one provided by MFC, and
   likely by most other C++ compilers nowadays as well. We
   rewrite it here to ensure portability. */

class String
  {
    friend String *tmpstr(const char*data, bool newbuff);
    friend void PurgeStrings();
    private:
      int32 Canary;
      char * Buffer;
      int32 Length;
      
    public:
      String();                 
      String(const char*str);
      ~String();
      void Empty();
      operator const char*();
      String & operator +=(const char*add);
      String & operator +=(char ch);
      String & operator =(const char*s);
      String & operator =(String &s)
        { return ((*this) = ((const char*) s)); }
      String & operator +(const char*s);
      bool operator ==(const char* s2);
      bool operator !=(const char* s2);
      bool operator>(const char* s2);
      bool operator<(const char* s2);
      int32 strchr(char ch);
      const char* GetData() { return Buffer; }
      int32 GetLength() { return Length; }
      int32 GetTrueLength();
      String & Left(int32 sz);
      String & TrueLeft(int32 sz);
      String & TrueRight(int32 sz);
      String & Right(int32 sz);
      String & Mid(int32 start, int32 end);
      String & Trim();
            
      String & Upto(const char* chlist);
      String & After(const char* chlist);
      String & Capitalize(bool all = false);
      String & Replace(const char* find, const char* rep);
      String & Upper();
      String & Lower();
      int16 TrueLength();
      String & Decolorize();
      void SetAt(int32 loc,char ch);
      int8 GetAt(int32 loc);
      void Serialize(Registry &r);
  };



extern String & VFormat(const char*fmt,va_list ap);
extern String & Format(const char*fmt,...);

String & Pluralize(const char* s, rID iID=0);
String & Replace(const char* str, const char* find, const char* rep);

String & Capitalize(const char*s, bool all = false);
String & Left(const char*s, int32 sz);
String & Right(const char*s, int32 sz);
String & Trim(const char*s);
String & Mid(const char*s, int32 start, int32 end);
String & Upto(const char*s, const char* chlist);
String & After(const char*s, const char* chlist);
String & Upper(const char*s);
String & Lower(const char*s);
String & Decolorize(const char*s);

String & EventName(int16 ev);

String * tmpstr(const char*data, bool newbuff=true);

struct TextVal
  {
    int32      Val;
    const char *Text;
  };

struct MVal 
  {
    signed int Value:10;
    unsigned int VType:4;
    signed int Bound:10;
    unsigned int BType:4;
    int16 Adjust(int16 oval) {
      int16 nval;
      switch (VType) {
        case MVAL_NONE:
          nval = oval;
         break;
        case MVAL_ADD:
          nval = max(0,oval+Value);
         break;
        case MVAL_SET:
          nval = Value;
         break;
        case MVAL_PERCENT:
          nval = (oval*Value)/100;
         break;
        default:
          Error("MVal::Adjust -- illegal VType!");
        }
      switch (BType) {
        case MBOUND_NONE:
          break;
        case MBOUND_MIN:
          nval = max(Bound,nval);
         break;
        case MBOUND_MAX:
          nval = min(Bound,nval);
         break;
        case MBOUND_NEAR:
          if (nval >= oval)
            nval = max(nval,(nval+Bound*2)/3);
          else
            nval = max(nval,(nval+Bound*2)/3);
         break;
        default:
          Error("MVal::Adjust -- illegal BType!");
        } 
      return nval;                                
    }
  };


#define oThing(h)     ( theRegistry->GetThing(h) )
#define oPlayer(h)    ( theRegistry->GetPlayer(h) )
#define oCreature(h)  ( theRegistry->GetCreature(h) )
#define oItem(h)      ( theRegistry->GetItem(h) )
#define oMap(h)       ( theRegistry->GetMap(h) )
#define oObject(h)    ( theRegistry->Get(h) )
#define oCharacter(h) ( (Character*) (theRegistry->GetCreature(h)) )
#define oMonster(h)   ( (Monster*)   (theRegistry->GetCreature(h)) )
#define oContain(h)   ( (Container*) (theRegistry->GetItem(h)) )
#define oWeapon(h)    ( (Weapon*)    (theRegistry->GetItem(h)) )
#define oArmour(h)     ( (Armour*)     (theRegistry->GetItem(h)) )
#define oFeature(h)   ( (Feature*)   (theRegistry->GetThing(h)) )
#define oPortal(h)    ( (Portal*)    (theRegistry->GetThing(h)) )
#define oDoor(h)      ( (Door*)      (theRegistry->GetThing(h)) )
#define oTrap(h)      ( (Trap*)      (theRegistry->GetThing(h)) )
#define oGame(h)      ( (Game*)      (theRegistry->Get(h)) )
#define oModule(h)    ( (Module*)    (theRegistry->Get(h)) )

#define isValidHandle(h) ( theRegistry->Exists(h) )
struct RegNode
  {
    Object   *pObj;
    RegNode  *Next;
  };

struct DataNode
  {
    void     *pData;
    int32     Size;
    hObj      hOwner;
    hData     myHandle;
    DataNode *Next;
  };

struct GroupNode
  {
    hObj     hGroup;
    int32    LastUse;
    bool     Loaded;
    bool     Needed;
  };



struct Rect
  {
    uint8 x1,x2,y1,y2;
    Rect() {}
    Rect(uint8 _x1, uint8 _y1, uint8 _x2, uint8 _y2)
      { x1 = min(_x1,_x2); y1 = min(_y1,_y2);
        x2 = max(_x1,_x2); y2 = max(_y1,_y2); }
    void Set(uint8 _x1, uint8 _y1, uint8 _x2, uint8 _y2)
      { x1 = min(_x1,_x2); y1 = min(_y1,_y2);
        x2 = max(_x1,_x2); y2 = max(_y1,_y2); }
     Rect& PlaceWithin(uint8 sx, uint8 sy)
      { 
        static Rect r;
        if (sx>=(x2-x1))
          { r.x1 = x1+1; r.x2 = x2-1; }
        else
          {
            r.x1 = x1 + 1 + random(max(0,((x2-x1)-2)-sx));
            r.x2 = r.x1 + sx;
          }
        if (sy >= (y2-y1)) 
          { r.y1 = y1+1; r.y2 = y2-1; }
        else
          {
            r.y1 = y1 + 1 + random(max(0,((y2-y1)-2)-sy));
            r.y2 = r.y1 + sy;
          }
        return r;
      }
    Rect& PlaceWithinSafely(uint8 sx, uint8 sy)
      { 
        static Rect r;
        r.x1 = x1 + random(max(0,((x2-x1)-1)-sx));
        r.x2 = r.x1 + sx;
        r.y1 = y1 + random(max(0,((y2-y1)-1)-sy));
        r.y2 = r.y1 + sy;

        r.x1 = max(r.x1, x1 + 2);
        r.y1 = max(r.y1, y1 + 2);
        r.x2 = min(r.x2, x2 - 2);
        r.y2 = min(r.y2, y2 - 2);
        return r;
      }
    bool Within(uint8 x,uint8 y)
      { return (x >= x1 && x <= x2 && y >= y1 && y <= y2); }
    uint16 Volume()
      { return (x2 - x1) * (y2 - y1); }
    bool Overlaps(Rect &r)
      {
        if (((x1 >= r.x1) == (x2 <= r.x2)) && 
          ((y1 >= r.y1) == (y2 <= r.y2)))
          return false;
        if (Within(r.x1,r.y1)) return true;
        if (Within(r.x1,r.y2)) return true;
        if (Within(r.x2,r.y1)) return true;
        if (Within(r.x2,r.y2)) return true;
        return false;
      }
  };




struct DictNode
	{
		const char* Word;
		int16 ID;
		DictNode *Lower;
		DictNode *Higher;
	};
class Dictionary
	{
		private:
			DictNode Head;
			int16 Size;
			int16 RealSize;
			const char** IDs;
			bool Loaded;
			void PlaceWord(const char*,int16);
			void Enlarge();
		public:
			Dictionary();
			int16* ProcessName(const char*Name);
			void Parse(const char*line);
			bool Read(const char*filename);
			const char* Word(int16 ID);
			int16 NewID() {return Size+1;}
			int16 ID(const char *);
			int16 InsertWord(char*word);
	};
extern Dictionary Dict;

struct Dice
	{
		int8 Number;
		int8 Sides;
		int8 Bonus;
		const char* Str();
		int16 Roll();
    Dice& LevelAdjust(int16 level,int16 spec=0);
    static int16 Roll(int8 n, int8 s, int8 b=0,int8 e=0);
		void Set(int8 n,int8 s, int8 b)
			{ Number=n; Sides=s; Bonus=b; }
    bool operator==(Dice &d)
      { return Number == d.Number && Sides == d.Sides &&
                             Bonus == d.Bonus; }
	};

struct ParseArch
	{
		int16 Verb[4];
		int16 Noun1f;
		int8 Noun1t;
		int16 Sep[4];
		int16 Noun2f;
		int8 Noun2t;
		int16 Act;
	};
class Player;
class Thing;
class Object;
class Win32Term;
class Term;
class MsDosTerm;

class Parser
	{
		private:
			int16 w[40];
			int16 n[20];
			int16 FoundWith; //Which NF_ flags noun actually fell under.
			Thing* Match;
			int8 MatchScore;
			Dir DirMatch;
			Dictionary* d;
			Player *p;
			Object * P1, * P2;
			char ErrMsg[80];
		public:
      Parser() {}
			Parser(Player*_p,Dictionary*_d)
				{ p=_p; d=_d; }
			void Parse(const char* Line);
			bool CheckMatch(ParseArch&a);
			void RemoveFluff();
			void BadNoun(int16 NF, int8 Type);
			bool Tokenize(const char* Line);
			int8 MatchNoun(Thing*t);
			bool MatchNounWord(Thing*,int16);
			bool HandleNoun(int16 NF, int8 Type);
			bool ValidLoc(int16 Flags, Thing*n);

	};

struct Option
  {
    int16       Val;
    const char* Name;
    const char* Choices;
    int8        Maximum;
    int8        Default;
    int16       Prereq;
    const char* Desc;
  };

class Fraction
  {
    private:
      // Data members
      int32 m_numerator, m_denominator;
    public:
      // Public methods
      Fraction();
      Fraction(int32 numerator, int32 m_denominator);
      Fraction(int32 intVal);
      Fraction(const Fraction &other);

      Fraction& operator=(const Fraction &rhs);


      // Accessors.
      int GetNumerator() const { return m_numerator; }
      int GetDenominator() const { return m_denominator; }

      // Comparison operators.
      friend bool operator==(const Fraction &lhs, const Fraction &rhs);
      friend bool operator!=(const Fraction &lhs, const Fraction &rhs);
      friend bool operator<(const Fraction &lhs, const Fraction &rhs);
      friend bool operator>(const Fraction &lhs, const Fraction &rhs);

    private:
      // Private methods
      void Normalize();
      static const inline int GCD(int32 a, int32 b);
      static void EqualizeDenominators(Fraction &a, Fraction &b);
    public:
      inline void Set(int32 num, int32 denom) {
        m_numerator = num; m_denominator = denom;
        Normalize(); }
  };





class Archive;

class Object 
  {
    public:
    Object(int16 _Type); 
    Object(Registry *r) {}
    ~Object();
    virtual String & Name(int16 Flags=0) { return *tmpstr("<object>"); }
    int16 Type;
    hObj myHandle;
    bool isCreature() { return Type == T_MONSTER || Type == T_NPC
                         || Type == T_PLAYER; }
    bool isFeature()  { return Type == T_DOOR || Type == T_PORTAL
                         || Type == T_TRAP || Type == T_FEATURE; }
    bool isItem()     { return Type>=T_FIRSTITEM && Type<=T_LASTITEM; }
    virtual bool isWeapon() { return false; } 
    virtual bool isArmour() { return false; } 
    bool isPlayer()   { return Type == T_PLAYER; }
    bool isMonster()  { return Type == T_MONSTER; }
    bool isCharacter(){ return Type == T_PLAYER || Type == T_NPC; }
    bool isContainer() { return Type == T_CONTAIN || Type == T_CHEST; }
    bool isType(int16 t) { return (t == T_THING) || (t == T_OBJECT) ||
                                 (t == T_ITEM && isItem()) ||
                                 (t == T_FEATURE && isFeature()) ||
                                 (t == T_CREATURE && isCreature()) ||
                                 (t == T_CHARACTER && isCharacter()) ||
                                 (t == T_CONTAIN && isContainer()) ||
                                 (t == Type); }

    virtual void Serialize(Registry &r, bool isSave) {}
    virtual String & Describe(Player *p)
      { return *tmpstr("[Object::Describe] A strange, undefined thing."); }
    virtual void Dump();
    void* operator new(size_t sz)
      {
        void *vp;
        vp = malloc(sz);
        if (!vp)
          Fatal("Memory allocation error!");
        memset(vp,0,sz);
        return vp;
      }
    void* operator new[](size_t sz)
      {
        void *vp;
        vp = malloc(sz);
        if (!vp)
          Fatal("Memory allocation error!");
        memset(vp,0,sz);
        return vp;
      }
    void* operator new(size_t sz,Object *o)
      {
        /* A means to initialize the vptr without actually
         * allocating the object.
         */
        return (void*)o;
      }
    void operator delete(void* vp)
      { free(vp); }
    void operator delete[](void* vp)
      { free(vp); }
    virtual bool inGroup(hObj h)
      { return (h == 0); }

  };

#define ARCHIVE_CLASS(ClassName,Base,r)                         \
  friend class Registry; protected:                             \
  ClassName(Registry*r) : Base(r) {}                            \
  virtual size_t ObjectSize() { return sizeof(ClassName); }     \
  virtual void Serialize(Registry &r, bool isSave) {            \
  Base::Serialize(r,isSave);

#define END_ARCHIVE }
   
  

class Registry
  {
    RegNode  ObjTable[OBJ_TABLE_SIZE];
    DataNode DataTable[DATA_TABLE_SIZE];
    OArray<GroupNode,20,2> Groups;
    int32 TimeCounter;
    bool saveMode, loadMode;
    hObj hCurrent;
    FILE *fp;
    #ifdef DEBUG
      FILE *reg_log;
    #endif
    public:
    hObj LastUsedHandle, hModule;

    Registry();
    ~Registry();
    void Empty();
    bool Saving()  { return saveMode; }
    bool Loading()  { return loadMode; }
    void Block(void **Block, size_t sz);
    hData RegisterBlock(void *o, hObj Owner, size_t sz, hData h = 0);
    void ClearDataTable();
    hObj  RegisterObject(Object*o,bool loaded=false);
    void  RemoveObject(Object*);
    Object * Get(hObj h);
    bool   Exists(hObj h);
    void * GetData(hData h);
    hObj   GetModuleHandle() { return hModule; }

    inline Thing* GetThing(hObj h)
      { ASSERT((!h) || (h > 0 && h < 128) || Get(h));
        return (Thing*)Get(h); }
    inline Player* GetPlayer(hObj h)
      { ASSERT((!h) || (h > 0 && h < 128) || (!Get(h)) || Get(h)->isPlayer());
        return (Player*)Get(h); }
    inline Creature* GetCreature(hObj h)
      { ASSERT((!h) || (h > 0 && h < 128) || (!Get(h)) || Get(h)->isCreature());
        return (Creature*)Get(h); }
    inline Item* GetItem(hObj h)
      { ASSERT((!h) || (h > 0 && h < 128) || (!Get(h)) || Get(h)->isItem());
        return (Item*)Get(h); }
    inline Annotation* GetAnnot(hObj h)
      { ASSERT((!h) || (h > 0 && h < 128) || (!Get(h)) || Get(h)->Type == T_ANNOT);
        return (Annotation*)Get(h); }
    inline Map* GetMap(hObj h)
      { ASSERT((!h) || (h > 0 && h < 128) || (!Get(h)) || Get(h)->Type == T_MAP);
        return (Map*)Get(h); }                  
    
    int16 SaveGroup(Term &t, hObj hGroup, bool use_lz, bool newFile=false);
    int16 LoadGroup(Term &t, hObj hGroup, bool use_lz);

  };



