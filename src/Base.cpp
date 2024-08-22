/* BASE.CPP -- See the Incursion LICENSE file for copyright information.

     Defines the fundamental low-level data structures of Incursion,
  including classes like String, Dice, OArray, NArray and Fraction. 
  Also includes miscellaneous non-system dependant functions that do 
  not fit elsewhere, such as Serialize().

    Array::Array()
    int16 Array::Add(S& Item)
    S* Array::NewItem()
    void Array::Remove()
    void Array::Enlarge()
    void Array::Reduce()
    void Array::Set(S& s, int16 idx)
    S* Array::_Paren(int16 idx)
    void Array::Serialize(Registry &r)
    void String::Serialize(Registry &r)
    int16 Dice::Roll()
    int16 Dice::Roll(int8 Number, int8 Sides, int8 Bonus, int8 e)
    Dice& Dice::LevelAdjust(int16 level, int16 spec)
    String Trim(String s)
    String Pluralize(String s, rID iID)
    Fraction::Fraction()
    Fraction::Fraction(int32 num, int32 denom)
    Fraction::Fraction(int32 val)
    Fraction::Fraction(Fraction &f)
    Fraction& Fraction::operator=(Fraction &rhs)
    bool operator==(Fraction &lhs, Fraction &rhs)
    bool operator!=(Fraction &lhs, Fraction &rhs)
    bool operator<(Fraction &lhs, Fraction &rhs)
    bool operator>(Fraction &lhs, Fraction &rhs)
    void Fraction::Normalize()
    int32 Fraction::GCD(int32 a, int32 b)
    void Fraction::EqualizeDenominators(Fraction &a, Fraction &b)
    int16 strcatf(char *buff, const char*fmt,...)
    SymbolTable::SymbolTable()
    void SymbolTable::Empty()
    int32 SymbolTable::hashval(const char*text)
    int32 SymbolTable::operator[](const char *text)
    const char* SymbolTable::operator[](int32 id)
    int32 SymbolTable::Add(const char *text)
    Binding* SymbolTable::GetBinding(int32 id, int32 scope, btype bt)
    void SymbolTable::Bind(int32 id, Binding *b)
*/

#include "Incursion.h"

void* x_realloc(void *block, size_t unit, size_t sz, size_t osz);

String * StrBufDelQueue[STRING_QUEUE_SIZE];
int32    iStrBufDelQueue;

#define BUGTRAP \
  ASSERT(Canary == 0xABCDEF12 || !Canary); \
  ASSERT((!Length) || Length == strlen(Buffer));

String * tmpstr(const char *data, bool newbuff)
  {
    String *s;
    if (newbuff)
      s = new String(data);
    else
      { s = new String();
        s->Buffer = (char*) data;
        s->Length = strlen(data); }
    
    s->Canary = 0xABCDEF12;
    StrBufDelQueue[iStrBufDelQueue++] = s;
    ASSERT(iStrBufDelQueue < STRING_QUEUE_SIZE - 10);
        
    return s;
  }

String::String()                 
  { 
    if (theRegistry->Loading())
      return;
    Buffer = NULL; 
    Length = 0; 
    Canary = 0;
    BUGTRAP
  }

String::String(const char*str)   
  { 
    if (str == NULL) 
      { 
        Buffer = NULL; 
        Length = 0; 
      }
    else
      { 
        Buffer = strdup(str); 
        Length = strlen(str); 
      }
    Canary = 0;
    BUGTRAP
  }

String::~String() 
  { 
    int32 i;
    if (Buffer) {
      for (i=0;i!=iStrBufDelQueue;i++)
        if (StrBufDelQueue[i])
          if (Buffer == StrBufDelQueue[i]->Buffer)
            StrBufDelQueue[i] = NULL;
      Empty(); 
      }
  }
  
void String::Empty() 
  { 
    if (Buffer) {
      free(Buffer);
      Buffer = NULL;
      }
    Length = 0;
  }

String::operator const char*()
  { 
    return Buffer; 
  }
    
String & String::operator +=(const char*add)
  { 
    if (!add) 
      return *tmpstr(Buffer);
    if (!Buffer) 
      { Buffer = strdup(add); 
        Length = strlen(add); 
        return *this; }
    
    Buffer = (char*) x_realloc(Buffer,1,Length + strlen(add) + 1,Length+1);
    //Buffer = (char*) realloc(Buffer,Length + strlen(add) + 1);
    
    strcpy(Buffer+Length,add);
    Length += strlen(add); 
    BUGTRAP
    return *this; 
  }

String & String::operator +=(char ch)
  { 
    if (!Buffer) 
      { 
        Buffer = (char*)malloc(2); 
        Buffer[0] = ch; 
        Buffer[1] = 0;
        Length = 1; 
        return *this; 
      }
    
    Buffer = (char*) realloc(Buffer,Length + 2);
    Buffer[Length] = ch; Buffer[Length+1] = 0;
    Length++; 
    BUGTRAP          
    return *this; 
  }

String & String::operator =(const char*s)
  { 
    char *new_b;
    if (!s) 
      { 
        Empty(); 
        return *this; 
      }
    new_b = strdup(s);
    Empty();
    Length = strlen(new_b); 
    Buffer = new_b; 
    BUGTRAP          
    return *this; 
  }

String & String::operator +(const char* s2)
  { 
    String *s;
    s = tmpstr(Buffer);
    *s += s2; 
    BUGTRAP
    return *s; 
  }
      
bool String::operator==(const char*str2)
  { 
    if (!Buffer || !str2)
      return (!Buffer) && (!str2); 
    return strcmp(Buffer,str2) == 0; 
  }

bool String::operator!=(const char*str2)
  { 
    return !(*this == str2); 
  }

bool String::operator>(const char* str2)
  { 
    if (!Buffer || !str2) 
      return (!Buffer) && (!str2); 
    return strcmp(Buffer,str2) > 0;     
  }
  
bool String::operator<(const char* str2)
  { 
    if (!Buffer || !str2) 
      return (!Buffer) && (!str2); 
    return strcmp(Buffer,str2) < 0; 
  }

int32 String::strchr(char ch)
  { 
    char *ss; 
    if(!Buffer) 
      return 0;
    ss = ::strchr(Buffer,ch); 
    if (!ss) 
      return 0;
    return ss - Buffer; 
  }
      
int32 String::GetTrueLength() 
  {
    char *ch = Buffer; int32 ln;       
    if (!Length) return 0;
    ln = 0;
    while (*ch)
      {
        if (*ch > 0)
          ln++;
        ch++;
      }
    return ln; 
  }

String & String::Left(int32 sz)
  { 
    if (sz >= Length)
      return *tmpstr(Buffer);
    char *s;
    s = (char*)malloc(sz+1);
    strncpy(s,Buffer,sz);
    s[sz] = 0;
    return *tmpstr(s,false); 
  }

String & String::TrueLeft(int32 sz)
  { 
    if (sz >= GetTrueLength())
      return *tmpstr(Buffer);
    
    int32 i; char *s;
    for(i=0;sz && Buffer[i];i++)
      if (Buffer[i] > 0)
        sz--;
    
    s = (char*)malloc(i+1);
    strncpy(s,Buffer,i+1);
    s[i] = 0;
    return *tmpstr(s,false); 
  }

String & String::TrueRight(int32 sz)
  { 
    if (sz >= GetTrueLength())
      return *tmpstr(Buffer);
    
    int32 i, e; char *s;
    e = GetLength()-1;
    for(i=0;sz && e-i>=0;i++)
      if (Buffer[e-i] > 0)
        sz--;
    
    s = (char*)malloc(i+1);
    strncpy(s,&(Buffer[e-i]),i+1);
    s[i] = 0;
    return *tmpstr(s,false); 
  }

String & String::Right(int32 sz)
  { 
    if (sz >= Length)
      return *tmpstr(Buffer);
    char *s;
    s = strdup(&(Buffer[Length-sz]));
    return *tmpstr(s,false); 
  }

String & String::Mid(int32 start, int32 end)
  { 
    if (start > Length || end < 0 || start > end)
      return *tmpstr(Buffer);
    if (end >= Length)
      return Right(start);
    if (start < 0)
      return Left(end);
    char *s;
    s = (char*)malloc((end - start) +2);
    strncpy(s,&(Buffer[I(start,Length)]),(end-start)+1);
    s[I((end-start)+1,Length+1)] = 0;
    return *tmpstr(s,false);
  }

String & String::Trim()
  {
    int32 s,e;
    if (!Length)
      return *tmpstr(Buffer);
    for(s=0;isspace_(Buffer[s]) && Length>s;s++)
      ;
    for(e=Length-1;e>0 && isspace_(Buffer[e]);e--)
      ;
    if (s>e)
      return *tmpstr("");
    return Mid(s,e);
  }
            
String & String::Upto(const char* chlist)
  { 
    for(int16 i=0;i!=Length;i++)
      if (::strchr(chlist,Buffer[i]))
        return Left(i);
    return *tmpstr(Buffer); 
  }

String & String::After(const char* chlist)
  { 
    for(int16 i=0;i!=Length;i++)
      if (::strchr(chlist,Buffer[i]))
        return Right(Length - i);
    return *tmpstr(Buffer); 
  }
    
String & String::Capitalize(bool all)
  { 
    String s;
    if (!Buffer) 
      return *tmpstr(Buffer);
    
    s = Buffer;
    s.Buffer[0] = toupper(s.Buffer[0]);
    if (all && Length >= 1)
      for(int16 i=1;i!=Length;i++)
        if (isalpha_(s.Buffer[i]) && !isalpha_(s.Buffer[i-1]) && s.Buffer[i-1] != '\'')
          s.Buffer[i] = toupper(s.Buffer[i]);
    return *tmpstr(s); 
  }

String & String::Upper()
  { 
    int32 i;
    if (!Buffer) 
      return *tmpstr(Buffer);
    String *s = tmpstr(Buffer);
    for (i=0;s->Buffer[i];i++)
      s->Buffer[i] = toupper(s->Buffer[i]);
    return *s; 
  }
  
String & String::Lower()
  { 
    int32 i;
    if (!Buffer) 
      return *tmpstr(Buffer);
    String *s = tmpstr(Buffer);
    for (i=0;s->Buffer[i];i++)
      s->Buffer[i] = tolower(s->Buffer[i]);
    return *s; 
}
  
int16 String::TrueLength() {
    int16 len = 0;
    if (Buffer) {
        for (int32 i=0;Buffer[i];i++)
            if (Buffer[i] < 127 && Buffer[i] > 0)
                len++;
    }
    return len; 
}
  
String & String::Decolorize()
  { 
    int32 len = GetTrueLength(), n, i;
    char *new_str;
    new_str = (char*)malloc(len+1);
    for (n=0,i=0;Buffer[i];i++)
      if (Buffer[i] > 0)
        new_str[n++] = Buffer[i];
    new_str[n] = 0;
    return *tmpstr(new_str,false); 
  }
  
void String::SetAt(int32 loc,char ch) 
  { 
    if (loc <= Length) 
      Buffer[loc] = ch; 
  }  

int8 String::GetAt(int32 loc) 
  { 
    return loc >= Length ? 0 : Buffer[loc]; 
  }  


 String & VFormat(const char*fmt,va_list ap)
  {
    static char FormatBuf[32000];
    vsprintf(FormatBuf,fmt,ap);
    ASSERT(strlen(FormatBuf) < 32000);
    return *tmpstr(FormatBuf);
  }

String & Format(const char*fmt,...)
  {
    va_list ap;
    va_start(ap,fmt);    
    String &str = VFormat(fmt,ap);
    va_end(ap);
    return str;
  }

String & Capitalize(const char *_s, bool all)
  { String s; s = _s; return s.Capitalize(all); }
String & Left(const char *_s, int32 sz)
  { String s; s = _s; return s.Left(sz); }
String & Right(const char *_s, int32 sz)
  { String s; s = _s; return s.Right(sz); }
String & Trim(const char *_s)
  { String s; s = _s; return s.Trim(); }
String & Mid(const char *_s, int32 start, int32 end)
  { String s; s = _s; return s.Mid(start,end); }
String & Upto(const char *_s, const char* chlist)
  { String s; s = _s; return s.Upto(chlist); }
String & After(const char *_s, const char* chlist)
  { String s; s = _s; return s.After(chlist); }
String & Upper(const char *_s)
  { String s; s = _s; return s.Upper(); }
String & Lower(const char *_s)
  { String s; s = _s; return s.Lower(); }
String & Decolorize(const char *_s)
  { String s; s = _s; return *tmpstr(s.Decolorize()); }
void String::Serialize(Registry &r)
  { r.Block((void**)(&Buffer),Length+1); }

void* x_realloc(void *block, size_t unit, size_t sz, size_t osz)
  {
    void *vp;
    vp = malloc(sz*unit);
    memset(vp,0,sz*unit);
    memcpy(vp,block,min(sz,osz)*unit);
    free(block);
    return vp;
  }
  
void PurgeStrings()
  {
    int32 i;
    if (iStrBufDelQueue < 256)
      return;
    for (i=0;StrBufDelQueue[i];i++)
      {
        if (StrBufDelQueue[i]->Canary != 0xABCDEF12)
          goto SkipThisFree;
        //delete (StrBufDelQueue[i]);
        /* Keep Purify Happy! */
        StrBufDelQueue[i]->Empty();
        delete StrBufDelQueue[i];
SkipThisFree:
        StrBufDelQueue[i] = NULL;
      }
    iStrBufDelQueue = 0;
    memset(StrBufDelQueue,0,STRING_QUEUE_SIZE*sizeof(String*));
  }

/*********************************************************************/



int ZeroValue = 0;

template<class S,int32 Initial,int32 Delta>
  Array<S,Initial,Delta>::Array()
	{
    if (theRegistry->Loading())
      return;
		Size=Initial;
		Count=0;
    if (Initial) {
		  Items = (S*) malloc(Size*sizeof(S));
      memset(Items,0,Size*sizeof(S));
      }
    else
      Items = NULL;
		if(!Items && Initial)
			Fatal("Can't Allocate List");
	}

template<class S,int32 Initial,int32 Delta>
  int32 Array<S,Initial,Delta>::Add(S& Item)
	{
		if (Count>=Size)
			Enlarge();
		Items[I(Count,Size)]=Item;
		Count++;
    if (Count)
      Items[0] = Items[0];
		return Count-1;
	}

template<class S,int32 Initial,int32 Delta>
  S* Array<S,Initial,Delta>::NewItem()
  {
    if ((Count+1) >= Size)
      Enlarge();
    if (Count)
      Items[0] = Items[0];
    return &(Items[I(Count++,Size)]);
  }

template<class S,int32 Initial,int32 Delta>
  void Array<S,Initial,Delta>::Remove(uint32 n)
	{
		if (n>Count-1 || n < 0)
			Fatal("List::Remove bounds error!");
		if (n==Count-1)
			{
				memset(&(Items[I(Count-1,Size)]),0,sizeof(S));
				Count--;
				if (Count+Delta<Size)
					Reduce();
				return;
			}
		memmove(&Items[I(n,Size)],&Items[I(n+1,Size)],(Count-(n+1))*sizeof(S));
		Count--;
		if(Count+Delta<Size)
			Reduce();
    if (Count)
      Items[0] = Items[0];
	}

template<class S,int32 Initial,int32 Delta>
  void Array<S,Initial,Delta>::Enlarge()
	{
		if (Count+Delta+10 < Size)
			return;
    if (!Items) {
      Items = (S*) malloc(max(Initial,Delta)*sizeof(S));
      memset(Items,0,max(Initial,Delta)*sizeof(S));
      Size = max(Initial,Delta);
      return;
      }
    Items = (S*) x_realloc(Items,sizeof(S),Size+Delta,Size);
    if (!Items)
      Fatal("Out of Memory!");
    memset(Items+Size,0,Delta*sizeof(S));
		Size+=Delta;
    if (Count)
      Items[0] = Items[0];
	}

template<class S,int32 Initial,int32 Delta>
  void Array<S,Initial,Delta>::Reduce()
	{		
    /*if (Count-Delta-1 > Size)
			return;                                 
    Items = (S*) x_realloc(Items,sizeof(S),Size-Delta,Size);
		Size-=Delta;
		if (!Items)
			Fatal("Out of Memory!");*/
	}

template<class S,int32 Initial, int32 Delta>
  void Array<S,Initial,Delta>::Set(S&s,uint32 idx)
  {
    ASSERT(idx >= 0)
    while(Size <= idx+1)
      Enlarge();
    Count = max(Count,idx+1);
    Items[I(idx,Size)] = s;
    if (Count)
      Items[0] = Items[0];
  }

template<class S,int32 Initial,int32 Delta>
S* Array<S,Initial,Delta>::_Paren(uint32 index)
	{
		ASSERT(index>=0); 
    if (index >= Count)
      return NULL;
    if (Count)
      Items[0] = Items[0];
		return &(Items[I(index,Size)]);
	}

template<class S,int32 Initial,int32 Delta>
void Array<S,Initial,Delta>::Serialize(Registry &r)
  { r.Block((void**)(&Items),sizeof(S)*Size);  }

template class Array<Field,10,5>;            /* Map::Fields */
template class Array<hObj,1000,10>;          /* Map::Things */
template class Array<hObj,30,30>;
template class Array<hObj,5,5>;
template class Array<Status,0,1>;            /* Creature::Stati */
template class Array<GroupNode,20,2>;        /* Registry::Groups */
template class Array<Object*,10,10>;         /* LoadGroup::LoadedObjects */
template class Array<uint16,20,20>;          /* Map::TorchList */
template class Array<LimboEntry,20,10>;      /* Game::Limbo */
template class Array<MTerrain,0,10>;         /* Map::TerraXY */
template class Array<TerraRecord,0,5>;       /* Map::TerraList */
template class Array<Annotation,ANNOT_INITIAL_SIZE,20>; /* Module::Annot */
template class Array<ModuleRecord,5,5>;      /* Game::ModFiles */
template class Array<uint16,200,200>;        /* PQueue::Elements */
template class Array<hObj,10,20>;            /* Thing::backRefs */
#ifndef NO_RCOMP
template class Array<uint16,5,5>;            /* yyMapSize */
#endif    
template class Array<DebugInfo,1000,1000>;   /* Module::DI */

int16 Dice::Roll()
	{
		int i,n;
		n=0;
    if (Sides < 0)
      Fatal("Negative Dice values in Dice::Roll()!");
		for(i=0;i!=abs(Number);i++)
			n+=(random(Sides)+1);
		if (Number < 0)
      n-=Bonus;
    else
      n+=Bonus;
    if (Number > 0)
		  return max(n,0);
    else if (Number < 0)
      return min(n,0);
    else
      return n;
	}

int16 Dice::Roll(int8 Number, int8 Sides, int8 Bonus,int8 e)
	{
		int i,n;
		n=0;
    if (Number < 0 || Sides < 0)
      Fatal("Negative Dice values in Dice::Roll()!");
		for(i=0;i!=Number;i++)
			n+=(random(Sides)+1);
		n+=Bonus;
		return max(n,0);
	}


Dice& Dice::LevelAdjust(int16 level, int16 spec)
  {
    static Dice d;
    d.Bonus  = (int8)::LevelAdjust(Bonus,level,spec);
    d.Number = (int8)::LevelAdjust(Number,level,spec);
    d.Sides  = (int8)::LevelAdjust(Sides,level,spec);
    return d;
  }

/* Algorithm derived from Nethack's makesal() routine */

String & Pluralize(const char *_s, rID iID)
  {
    int16 i; bool isCap;
    String s = _s;
    size_t l = s.GetLength();
    /* later, not only " of " but also:
       " labeled ", " called ", " named ",
       " versus ", " from ", " in ", " on ",
       " with ", etc. */
    for(i=0;i != l;i++) {
      if (s[i] == '/')
        {
          //String a, b, c;
          return
            Pluralize(s.Left(i),iID) +
            SC("/") +
            Pluralize(s.Right(s.GetLength()-(i+1)),iID);
          //return a + b + c;
        }
      if (s[i] != ' ' && s[i] != ',')
        continue;
      if (s[i] == ' ' && s[i+1] == '[')
        /* spellbook [Tome of Magic] -> spellbooks [Tome of Magic] */
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'o' && s[i+2] == 'f' && s[i+3] == ' ')
        /* potion of healing -> potions of healing */
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'o' && s[i+2] == 'n' && s[i+3] == ' ')
        /* lord on high -> lords on high */
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'n' && s[i+2] == 'a' && s[i+3] == 'm'
           && s[i+4] == 'e' && s[i+5] == 'd' && s[i+6] == ' ')
        /* potion named healing -> potions named healing */
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'v' && s[i+2] == 'e' && s[i+3] == 'r'
           && s[i+4] == 's' && s[i+5] == 'u' && s[i+6] == 's' && s[i+7] == ' ')
        /* amulet versus dragons -> amulets versus dragons */
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'v' && s[i+2] == 's' && s[i+3] == '.'
           && s[i+4] == ' ')
        /* amulet vs. dragons -> amulets vs. dragons */
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'f' && s[i+2] == 'r' && s[i+3] == 'o'
           && s[i+4] == 'm' && s[i+5] == ' ')
        /* warrior from Kalindor -> warriors from Kalindor */
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'i' && s[i+2] == 'n' && s[i+3] == ' ')
        /* speaker in darkness -> speakers in darkness */
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'w' && s[i+2] == 'i' && s[i+3] == 't'
           && s[i+4] == 'h' && s[i+5] == ' ')
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'l' && s[i+2] == 'a' && s[i+3] == 'b'
           && s[i+4] == 'e' && s[i+5] == 'l' && s[i+6] == 'e' && s[i+7] == 'd' 
           && s[i+8] == ' ')
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ' ' && s[i+1] == 'c' && s[i+2] == 'a' && s[i+3] == 'l'
           && s[i+4] == 'l' && s[i+5] == 'e' && s[i+6] == 'd' && s[i+7] == ' ') 
        return Pluralize(s.Left(i),iID) + s.Right(s.GetLength()-i);
      if (s[i] == ',')
        /* Kazrad, a Bringer of Darkness -> Kadrads */
        return Pluralize(s.Left(i),iID);
      }

    if (iID && TITEM(iID)->HasFlag(IT_PLURAL))
      switch (TITEM(iID)->IType) {
        case T_ARMOUR:
          return SC("suits of ") + s; /* 2 suits of plate mail */
        case T_BRACERS: case T_GAUNTLETS: case T_BOOTS:
          return SC("pairs of ") + s; /* 2 pairs of boots of speed */
        default:
          return SC("sets of ") + s;  /* 2 sets of beholder eyestalks */
        }
        
    
    const char * Plurals[] = {
      "staff", "staves",
      "loaf", "loaves",
      "dwarf", "dwarves",
      "drow", "drow",
      "wolf", "wolves",
      "elf", "elves",
      "tooth", "teeth",
      "lurker above", "lurkers above",
      "foot", "feet",
      "folk", "folk",
      "djinni", "djinn",
      "efreeti", "efreet",
      "mouse", "mice",
      "louse", "lice",
      "matzoh", "matzot",
      "tomato", "tomatoes",
      "potato", "potatoes",
      "shaman", "shamans",
      "human", "humans",
      "vermin", "vermin",
      "child", "children",
      "nunchaku", "nunchaku",
      "(female)", "females",
      "undead", "undead",
      "kin", "kin",
      "dao", "dao",
      "marid", "marid",                              
      "mumak", "mukakil",
      "fe", "ves",     
      "ium", "ia",
      "us", "i",
      "rtex", "rtices",
      "sis", "ses",
      "eau", "eaux",
      "man", "men",
      "z", "zes",
      "x", "xes",
      "s", "ses",
      "ch", "ches",
      "sh", "shes",
      "qu", "ies",
      "ey", "eys",
      "oy", "oys",
      "uy", "uys",
      "ay", "ays",
      "iy", "iys",
      "y", "ies",
      NULL, NULL };

    for (i=0;Plurals[i];i+=2)
      if (l >= strlen(Plurals[i]))
        if (stricmp(Plurals[i],s.Right(strlen(Plurals[i]))) == 0)
          {
            isCap = isupper(s[l - strlen(Plurals[i])]) != 0;
            s =  s.Left(l - strlen(Plurals[i]));
            s += Plurals[i + 1];
            if (isCap)
              s.SetAt(l - strlen(Plurals[i]),
                toupper(s[l - strlen(Plurals[i])]));
            return *tmpstr(s);
          }

    return (s + SC("s"));                                  
  }


String & Replace(const char* _str, const char* _find, const char* _rep)
  {
    int16 loc; 
    String str, find, rep; 
    str = _str;
    find = _find; 
    rep = _rep;
    if (str.GetLength() == 0 || find.GetLength() == 0)
      return *tmpstr(str);
    while (strstr(str,find))
      {
        loc = strstr(str, find) - (const char*)str;
        str = str.Left(loc) + rep +
                    str.Right(str.GetLength() - (loc +
                           find.GetLength()));
      }
    return *tmpstr(str);
  }
 
String & String::Replace(const char* find, const char* rep)
  { return ::Replace(Buffer,find,rep);; }                             
      

/****************************************************************************
    Here follows public domain code written by David H. Hovemeyer for
      manipulating fractions. It's used for the line of sight stuff,
      mana calculation and a few other places. Thanks for the code,
      David. It's useful. :)
****************************************************************************/


////////////////////////////////////////////////////////////////////////
// Default constructor.
////////////////////////////////////////////////////////////////////////
Fraction::
Fraction()
    : m_numerator(1)
    , m_denominator(1)
{
}

////////////////////////////////////////////////////////////////////////
// Constructor from numerator and denominator.
////////////////////////////////////////////////////////////////////////
Fraction::
Fraction(int32 numerator, int32 denominator)
    : m_numerator(numerator)
    , m_denominator(denominator)
{
    if(denominator == 0)
      Fatal("Fraction::Fraction -- denominator 0!");
    Normalize();
}

////////////////////////////////////////////////////////////////////////
// Constructor from an integer.
////////////////////////////////////////////////////////////////////////
Fraction::
Fraction(int32 intVal)
    : m_numerator(intVal)
    , m_denominator(1)
{
}

////////////////////////////////////////////////////////////////////////
// Copy constructor.
////////////////////////////////////////////////////////////////////////
Fraction::
Fraction(const Fraction &other)
    : m_numerator(other.m_numerator)
    , m_denominator(other.m_denominator)
{
}

////////////////////////////////////////////////////////////////////////
// Assignment operator.
////////////////////////////////////////////////////////////////////////
Fraction& Fraction::
operator=(const Fraction &rhs)
{
    m_numerator   = rhs.m_numerator;
    m_denominator = rhs.m_denominator;
    return *this;
}

////////////////////////////////////////////////////////////////////////
// Test for equality.
////////////////////////////////////////////////////////////////////////
bool
operator==(const Fraction &lhs, const Fraction &rhs)
{
    // Since both fractions are in normalized form, a simple
    // equality test on numerator and denominator will suffice.
    return (lhs.m_numerator == rhs.m_numerator &&
	    lhs.m_denominator == rhs.m_denominator);
}

////////////////////////////////////////////////////////////////////////
// Test for inequality.
////////////////////////////////////////////////////////////////////////
bool
operator!=(const Fraction &lhs, const Fraction &rhs)
{
    // Since both fractions are in normalized form, a simple
    // inequality test on numerator and denominator will suffice.
    return (lhs.m_numerator != rhs.m_numerator &&
	    lhs.m_denominator != rhs.m_denominator);
}

////////////////////////////////////////////////////////////////////////
// Test if lhs is less than rhs.
////////////////////////////////////////////////////////////////////////
bool
operator<(const Fraction &lhs, const Fraction &rhs)
{
    Fraction lhsCopy(lhs), rhsCopy(rhs);
    Fraction::EqualizeDenominators(lhsCopy, rhsCopy);
    return lhsCopy.m_numerator < rhsCopy.m_numerator;
}

////////////////////////////////////////////////////////////////////////
// Test if lhs is greater than rhs.
////////////////////////////////////////////////////////////////////////
bool
operator>(const Fraction &lhs, const Fraction &rhs)
{
    Fraction lhsCopy(lhs), rhsCopy(rhs);
    Fraction::EqualizeDenominators(lhsCopy, rhsCopy);
    return lhsCopy.m_numerator > rhsCopy.m_numerator;
}

////////////////////////////////////////////////////////////////////////
// Normalize the fraction by dividing numerator and denominator
// by their greatest common divisor, and making sure that only
// the numerator is negative.
////////////////////////////////////////////////////////////////////////
void Fraction::
Normalize()
{
    int gcd = GCD(m_numerator, m_denominator);
    m_numerator /= gcd;
    m_denominator /= gcd;

    if (m_denominator < 0)
    {
	m_numerator   = -m_numerator;
	m_denominator = -m_denominator;
    }

    // Postconditions.
    assert(GCD(m_numerator, m_denominator) == 1);
    assert(m_denominator > 0);
}

////////////////////////////////////////////////////////////////////////
// Find greatest common divisor of given integers, using
// Euclid's algorithm.
////////////////////////////////////////////////////////////////////////
const inline int Fraction::GCD(int32 a, int32 b)
{
    // This code was shamelessly stolen off the 'net.
    while (a > 0)
    {
	if (a < b)
	{
	    // swap a and b 
	    const int32 t = a;
	    a = b;
	    b = t;
	}  
	a = a - b;
    } 

    return b;
}

////////////////////////////////////////////////////////////////////////
// Make the denominators of given fractions equal, so that the
// fractions can be compared, added, or subtracted.
////////////////////////////////////////////////////////////////////////
void Fraction::
EqualizeDenominators(Fraction &a, Fraction &b)
{
    if (a.m_denominator != b.m_denominator)
    {
	int32 gcd = GCD(a.m_denominator, b.m_denominator);
	int32 t1, t2;

	t1 = b.m_denominator / gcd;
	t2 = a.m_denominator / gcd;

	a.m_numerator   *= t1;
	a.m_denominator *= t1;

	b.m_numerator   *= t2;
	b.m_denominator *= t2;

    }

    assert(a.m_denominator == b.m_denominator);
    // Denominators are now equal, but the fractions may not be
    // normalized, so don't return either one to the outside world.
}

int16 strcatf(char *buff,const char*fmt,...)
  {
   va_list argptr;
   int16 cnt;

   va_start(argptr, fmt);
   cnt = vsprintf(&(buff[strlen(buff)]), fmt, argptr);
   va_end(argptr);

   return(cnt);
  }


/*****************************************************************************\
|                                THE SYMBOL TABLES                            |
\*****************************************************************************/


SymbolTable::SymbolTable()
  {
    memset(HashTable,0,HASH_SIZE * sizeof(SymbolTable*));
    memset(RefTable,0,HASH_SIZE * sizeof(SymbolTable*));
    LastID    = 20;
  }

void SymbolTable::Empty()
  {
    int16 i;
    for(i=0;i!=HASH_SIZE;i++)
      if (RefTable[I(i,HASH_SIZE)])
        delete RefTable[I(i,HASH_SIZE)];
    memset(HashTable,0,HASH_SIZE * sizeof(SymbolTable*));
    memset(RefTable,0,HASH_SIZE * sizeof(SymbolTable*));
    LastID    = 20;
  }

int32 SymbolTable::hashval(const char*text)
  {
    unsigned int h=0, g;
    ASSERT(text && *text);

    for(;*text;text++)
      {
        h = (h << 4) + *text;
        if (g = h & 0xF0000000)
          {
            h = h ^ (g >> 24);
            h = h ^ g;
          }
      }
    return h % HASH_SIZE;
  }



int32 SymbolTable::operator[](const char *text)
  {
    SymbolNode *n; int16 i;
    #ifdef DEBUG
    if (!*text)
      Error("Bounds error");
    #endif

    n = HashTable[I(hashval(text),HASH_SIZE)];

    if (!n)
      return 0;

    if (!n->text)
      return 0;

    while (n)
      {
        if (!(i = strcmp(text,n->text)))
          return n->id;
        else if (i > 0)
          n = n->right;
        else
          n = n->left;
      }
    return 0;
  }

const char* SymbolTable::operator[](int32 id)
  {
    SymbolNode *s;
    ASSERT (id >= 0 && id <= LastID+1);

    if (RefTable[id % HASH_SIZE]) {
      s = RefTable[id % HASH_SIZE];
      do {
        if (s->id == id)
          return s->text;
        s = s->next;
        }
      while(s);
      }
    return NULL;
  }


int32 SymbolTable::Add(const char *text)
  {
    ASSERT(text);
    if (int32 i = (*this)[text])
      return i;

    SymbolNode *n2, *n = new SymbolNode;
    n->text = strdup(text);
    n->next = n->right = n->left = NULL;
    n->id   = LastID++; n->b = NULL;

    if (!RefTable[n->id % HASH_SIZE])
      RefTable[n->id % HASH_SIZE] = n;
    else {
      n2 = RefTable[n->id % HASH_SIZE];
      while (n2->next)
        n2 = n2->next;
      n2->next = n;
      }

    n2 = HashTable[I(hashval(text),HASH_SIZE)];

    if (!n2)
      {
        HashTable[I(hashval(text),HASH_SIZE)] = n;
        return n->id;
      }

    while (1)
      {
        if (strcmp(text,n2->text) > 0)
          {
            if (n2->right)
              n2 = n2->right;
            else
              { n2->right = n; return n->id; }
          }
        else
          {
            if (n2->left)
              n2 = n2->left;
            else
              { n2->left = n; return n->id; }

          }
      }
  }

Binding* SymbolTable::GetBinding(int32 id, rID xID, int16 Ev, btype bt)
  {
    SymbolNode *n; Binding *b, *best; 
    n = RefTable[id % HASH_SIZE];
    while(n && n->id != id)
      n = n->next;
    if (!n)
      return NULL;
    b = n->b;
    best = NULL;
    while (b) {
      if ((bt == ANY && b->type != RES_MEM && b->type != RES_FUNC) || bt == b->type)
        if (xID == b->xID || !b->xID)
          if (Ev == b->Event || !b->Event) 
          return b;
      b = b->next;
      }
    return NULL;
        
  }


#ifdef DEBUG
void SymbolTable::Bind(int32 id, Binding *b)
  {
    Binding *b2; SymbolNode *n; b->next = NULL;
    ASSERT(id>=0 && id <= LastID && b);

    n = RefTable[id % HASH_SIZE];
    while(n && n->id != id)
      n = n->next;
    if (!n)
      Error("Failed to bind symbol!");
    if (!n->b)
      n->b = b;
    else
      {
        b2 = n->b;
        while (b2->next)
          b2 = b2->next;
        b2->next = b;
      }
    theGame->Modules[0]->AddDebugInfo(id,b); 
  }
#endif




/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, KY_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static uint32_t mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(uint32_t s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* KY_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(uint32_t init_key[], int KY_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>KY_length ? N : KY_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=KY_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
uint32_t genrand_int32(void)
{
    uint32_t y;
    static uint32_t mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
int genrand_int31(void)
{
    return (int)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void) 
{ 
    uint32_t a=genrand_int32()>>5, b=genrand_int32()>>6;
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 

