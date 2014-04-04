/* TEXTTERM.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Lower-level functions for interacting with a character-based
   terminal.
   
   Window Handling
     void  TextTerm::InitWindows();
     void  TextTerm::SetWin(int16 w)
     void  TextTerm::SizeWin(int16 wn, int16 x1, int16 y1, int16 x2, int16 y2)
     void  TextTerm::SizeWin(int16 wn, int16 sx, int16 sy)
     int16 TextTerm::WinTop();
     int16 TextTerm::WinBottom();
     int16 TextTerm::WinLeft();
     int16 TextTerm::WinRight();
   Tidy Message IO   
     void  TextTerm::Write(int16 x, int16 y, const char *msg)
     void  TextTerm::Write(const char *msg)
     void  TextTerm::CWrite(const char *msg)
     void  TextTerm::Message(String Msg)
     void  TextTerm::AddMessage(const char* msg)
     void  TextTerm::ShowMessages()
     void  TextTerm::DrawBorder(int16 wn, int16 border)
     void  TextTerm::Box(int16 win, int16 flags, int16 cbor, int16 ctxt, const char*text)
     int16 TextTerm::WrapWrite(int16 x,int16 y,String s, int16 x2)
   Scrolling Text
     void  TextTerm::UpdateScrollArea(int16 _offset, int16 wn)
     void  TextTerm::ClearScroll()
     void  TextTerm::SWrite(const char *text, int16 wn)
     int16 TextTerm::SWrapWrite(int16 x, int16 y, String s, int16 x2, int16 wn)
   Formatted Menus
     void  TextTerm::LOption(const char*text,int32 val,const char*desc)
     int32 TextTerm::LMenu(uint16 fl, const char*title,int8 MWin,const char*help)
*/

#include "Incursion.h"

/*****************************************************************************\
*                                 TEXTTERM                                    *
*                              Window Handling                                *
\*****************************************************************************/

void TextTerm::InitWindows()
  {
    ASSERT(sizeX >= 80);
    ASSERT(sizeY >= 48);
    SizeWin(WIN_SCREEN,   0,  0,  sizeX-1,  sizeY-1);
    SizeWin(WIN_BIGMAP,   0,  0,  sizeX-1,  sizeY-1);
    SizeWin(WIN_MAP,      0,  4,  sizeX-17, sizeY-7);
    SizeWin(WIN_DEBUG,    0,  4,  sizeX-17, sizeY-3);
    SizeWin(WIN_INVEN,    0,  4,  sizeX-17, sizeY-3);
    SizeWin(WIN_OPTIONS,  0,  4,  sizeX-17, sizeY-3);
    SizeWin(WIN_STORE,    0,  4,  sizeX-17, sizeY-3);
      
    SizeWin(WIN_CSHEET,   0,  0,  sizeX-1,  sizeY-1);
      
    SizeWin(WIN_CREATION, 0,  0,  sizeX-17, sizeY-3);
    SizeWin(WIN_MESSAGE,  0,  0,  sizeX-17, 2);
    SizeWin(WIN_MESSAGE2, 0,  sizeY-4, sizeX-1,  sizeY-2);
    SizeWin(WIN_INPUT,    0,  3,  sizeX-17, 3);
    SizeWin(WIN_INPUT2,   0,  sizeY-1, sizeX-1,  sizeY-1);
    SizeWin(WIN_LOWER,    5,  30, sizeX-5,  sizeY-1);
    SizeWin(WIN_UPPER,    1,  2,  sizeX-2,  28);
    SizeWin(WIN_TRAITS,   sizeX-15, 0,  sizeX-1,  sizeY-3);
    SizeWin(WIN_VIEWLIST, sizeX-15, 30, sizeX-1,  sizeY-3);
    SizeWin(WIN_STATUS,   0,  sizeY-2, sizeX-17, sizeY-1);
    SizeWin(WIN_SPELLS,   0,  5,  sizeX-1, 30);
    SizeWin(WIN_NUMBERS,  0,  sizeY-4, sizeX-17, sizeY-4);
    SizeWin(WIN_NUMBERS2, 0,  sizeY-3, sizeX-17, sizeY-3);
    SizeWin(WIN_NUMBERS3, 0,  sizeY-4, sizeX-17, sizeY-3);
    SizeWin(WIN_NUMBERS4, 0,  sizeY-6, sizeX-17, sizeY-5);

  }

void TextTerm::SetWin(int16 w)
  {
    if (w == WIN_INPUT && (Mode == MO_CHARVIEW || Mode == MO_SPLASH))
      activeWin = &(Windows[WIN_INPUT2]);
    else if (w == WIN_MESSAGE && (Mode == MO_CHARVIEW || Mode == MO_SPLASH))
      activeWin = &(Windows[WIN_MESSAGE2]);
    else if (w >= 0 && w <= WIN_LAST)
      activeWin = &(Windows[w]);
    else
      Fatal("Illegal window number sent to TextTerm::SetWin.");
    
  }

void TextTerm::SizeWin(int16 wn, int16 x1, int16 y1, int16 x2, int16 y2)
  {
    if (wn == WIN_SCREEN) {
      Windows[wn].Top  = y1;
      Windows[wn].Left = x1;
      Windows[wn].Bottom = y2;
      Windows[wn].Right = x2;
      return;
      }
    ASSERT(y1 >= 0  || y1 == -1)
    ASSERT(x1 >= 0  || x1 == -1)
    ASSERT(x2 <= sizeX || x2 == -1)
    ASSERT(y2 <= sizeY || y2 == -1)
    if (y1 != -1) Windows[wn].Top =    max(0,y1);
    if (x1 != -1) Windows[wn].Left =   max(0,x1);
    if (y2 != -1) Windows[wn].Bottom = min(y2,Windows[WIN_SCREEN].Bottom);
    if (x2 != -1) Windows[wn].Right =  min(x2,Windows[WIN_SCREEN].Right);
  }

void TextTerm::SizeWin(int16 wn, int16 sx, int16 sy)
  {
    if (wn != WIN_SCREEN)
      {
        ASSERT(sy <= Windows[WIN_SCREEN].Bottom)
        ASSERT(sx <= Windows[WIN_SCREEN].Right)
      }
    Windows[wn].Top    = Windows[WIN_SCREEN].Bottom/2 - sy/2;
    Windows[wn].Bottom = Windows[WIN_SCREEN].Bottom/2 + (sy/2 + sy%2);
    Windows[wn].Left   = Windows[WIN_SCREEN].Right/2 - sx/2;
    Windows[wn].Right  = Windows[WIN_SCREEN].Right/2 + (sx/2 + sx%2);
  }


/*****************************************************************************\
*                                 TEXTTERM                                    *
*                              Tidy Message IO                                *
\*****************************************************************************/

void TextTerm::Write(const char*str)
  {
    const char *ch;
    if (!str)
      return;
    ch = str;
    if (cx < activeWin->Left)
      cx = activeWin->Left;
    if (cy > activeWin->Bottom
      || cy < activeWin->Top)
        { cy = activeWin->Top; cx = activeWin->Left; }
    updated = false;
    while(*ch)
      {
        if (*ch == '\n')
          {
            cy++; ch++; cx = activeWin->Left;
            if (cy > activeWin->Bottom)
              return;
            continue;
          }
        // weimer: return, but only if we're not at the beginning of a line
        if (*ch == '\r')
          {
            ch++; 
            if (cx != activeWin->Left) { 
              cy++; cx = activeWin->Left;
              if (cy > activeWin->Bottom)
                return;
            } 
            continue;
          }
        if (cx > activeWin->Right)
          { cy++; cx = activeWin->Left;
            if (cy > activeWin->Bottom)
              return;
          }
        if ((*ch < 0) && (*ch > -16))
          {
            Color(-*ch); ch++;
            continue;
          }
        if (*ch == '~')
          APutChar(cx,cy,'%' + attr*256);
        else if (*ch == '_' && (activeWin != &Windows[WIN_DEBUG]))
          APutChar(cx,cy,' ' + attr*256);
        else if (*ch == LITERAL_CHAR) {
          ch++;
          APutChar(cx,cy,*ch + attr*256);
          }
        else if (*ch == WRAP_BREAK) 
          cWrap = cx+1;
        else if (*ch == WRAP_INDENT) 
          cWrap = cx+3;
        else {
          APutChar(cx,cy,*ch + attr*256);
          }
        cx++; ch++;
      }
  }

void TextTerm::CWrite(const char* text)
  {
    uint8 oattr;
    Write(text);
    if (cy >= activeWin->Bottom - 1) {
      GotoXY(0,activeWin->Bottom);
      oattr = attr; Color(PINK);
      Write("[MORE]");
      GetCharRaw();
      Color(oattr);
      Clear();
      GotoXY(0,0);
      }
  }

void TextTerm::Message(const char* _Msg)
	{
		String Seg; String Msg; 
		int16 oldWin = (activeWin - &Windows[0]);
    int16 space, len; 
    int16 ch; bool isFull;
    bool autoMore =  p ? p->Opt(OPT_AUTOMORE) : false;
    Msg = _Msg;
    if (!Msg.GetTrueLength())
      return;
    AddMessage(Msg);

    if (Mode != MO_PLAY && Mode != MO_INV)
      {
        Box(WIN_SCREEN,0,PURPLE,GREY,Msg);
        SetWin(oldWin);
        return;
      }

		if (Msg.GetTrueLength()+4 > (Windows[WIN_MESSAGE].Right-Windows[WIN_MESSAGE].Left)*2)
      { Box(WIN_SCREEN,0,PINK,GREY,Msg); 
        SetWin(oldWin);
        return; }
    SetWin(WIN_MESSAGE);
    
    if (ClearMsgOK)
      {
        ClearMsgOK = false;
        linenum = linepos = 0;
        Clear();
      }

    Color(GREY);
    ASSERT(WinSizeX() >= 25)
    ASSERT(WinSizeY() >= 2)

    NextSeg:

    space = WinSizeX() - (linepos+1);
    
    /* Leave room for -- more -- on the last line. */
    if (linenum == WinSizeY() - 1 && !autoMore)
      space -= 12; 
    if (space < 5) {
      if (linenum == WinSizeY() - 1)
        goto DoMore;
      linenum++;
      linepos = 0;
      goto NextSeg;
      }

    /* Find a good place to break off the segment */
    len = max(space,Msg.TrueLeft(space).GetLength());
    for (; len != max(0,space-15); len--) 
      if (len > Msg.GetLength() || Msg[len] == ' ' || Msg[len] == '\n')
        break;
    /* No good place, so we have to break a word. */
    if (len == 0)
      len = space;

    if (len > Msg.GetLength())
      { 
        isFull = true; 
        Seg = Msg; 
        Msg.Empty();
      }
    else
      {
        isFull = false;
        Seg = Msg.Left(len);
        Msg = Msg.Right(Msg.GetLength() - len);
        Seg = Seg.Trim();
        Msg = Msg.Trim();
      }

    GotoXY(linepos,linenum);
    Write(Seg);

    if (isFull)
      {
        linepos += Seg.GetTrueLength() + 1;
        return;
      }
    else
      {
        if (linenum < WinSizeY() - 1)
          { linenum++; 
            linepos = 0; 
            goto NextSeg; }

        DoMore:

        if (!autoMore) { 
          int32 old_attr;
          GotoXY(WinSizeX()-11,WinSizeY()-1);
          old_attr = attr;
          Color(PINK);
          Write("-- more --");
          do 
            ch = GetCharCmd();
          while (ch >= KY_CMD_NORTH &&
                 ch <= KY_CMD_SOUTHWEST);   
          Color(old_attr);   
        } 
        ClearMsgOK = false;
        Clear();
        linenum = linepos = 0;
        goto NextSeg;
      }
    SetWin(oldWin);
	}

void TextTerm::AddMessage(const char* msg)
  {
    int16 i;
    if ((strlen(MsgHistory) + strlen(msg)) > 10230) {
      i = strlen(msg)+3;
      while(MsgHistory[i] && (MsgHistory[i] != '.'))
        i++;
      if (!MsgHistory[i])
        {
          strcpy(MsgHistory,msg);
          strcat(MsgHistory," ");
          return;
        }
      i += 2;
      memmove(MsgHistory,&(MsgHistory[i]),10239-i);
      MsgHistory[10239-i] = 0;
      }
    strcat(MsgHistory,msg);
    strcat(MsgHistory,"\n"); /* ww: now that we're scrolling, keeping line
                              * breaks is important */
   }

void TextTerm::ShowMessages()
  {
    int16 oldMode;
    oldMode = GetMode();
    SetMode(MO_DIALOG);
    /* ww: we really want to be able to scroll this! */
    SetWin(WIN_SCREEN);
    Save();
    SizeWin(WIN_CUSTOM,WinSizeX()-16,WinSizeY()-8);
    DrawBorder(WIN_CUSTOM,MAGENTA);
    ClearScroll();
    SetWin(WIN_CUSTOM); Color(YELLOW);
    SWrite((WinSizeX()/2)-6,0," -- Messages -- ");
    Color(GREY);
    SWrapWrite(1,1,XPrint(MsgHistory),WinSizeX()-2,WIN_CUSTOM);
    Color(GREY);
    UpdateScrollArea(0,WIN_CUSTOM);  
    UpdateScrollArea(4000,WIN_CUSTOM);
    do {
      Color(GREY);
      UpdateScrollArea(offset,WIN_CUSTOM);
      switch(GetCharCmd(KY_CMD_ARROW_MODE)) {
        case KY_CMD_NORTH:
          UpdateScrollArea(offset-1,WIN_CUSTOM); break;
        case KY_CMD_SOUTH:
          UpdateScrollArea(offset+1,WIN_CUSTOM); break;
        case KY_CMD_NORTHEAST:
        case KY_CMD_NORTHWEST:
          UpdateScrollArea(offset-(WinSizeY()-6),WIN_CUSTOM); break;
        case KY_CMD_SOUTHEAST:
        case KY_CMD_SOUTHWEST:
          UpdateScrollArea(offset+(WinSizeY()-6),WIN_CUSTOM); break;

        default: 
          ClearScroll();
          Restore();
          SetMode(oldMode);
          return;
        }
    } while(1);
  }

void TextTerm::DrawBorder(int16 wn, int16 border)
  {
    int16 i;
    SetWin(WIN_SCREEN); Color(border);
    for(i=Windows[wn].Left;i!=Windows[wn].Right+1;i++)
      {
        PutChar(i,Windows[wn].Top-1,205);
        PutChar(i,Windows[wn].Bottom+1,205);
      }
    for(i=Windows[wn].Top;i!=Windows[wn].Bottom+1;i++)
      {
        PutChar(Windows[wn].Left-1,i,186);
        PutChar(Windows[wn].Right+1,i,186);
      }
    PutChar(Windows[wn].Left-1,Windows[wn].Top-1,219);
    PutChar(Windows[wn].Right+1,Windows[wn].Top-1,219);
    PutChar(Windows[wn].Left-1,Windows[wn].Bottom+1,219);
    PutChar(Windows[wn].Right+1,Windows[wn].Bottom+1,219);
  }

void TextTerm::Box(int16 win, int16 flags, int16 cbor, int16 ctxt, const char*text)
  {
    int16 LineLength, BoxLength, Avail, Lines, SpaceAt, x, y, c, i, real_c, in;
    char *ch; int8 saveMode; bool done;
    ASSERT(strlen(text) < sizeof(lbuff) - 20)
    SetWin(win);
    strcpy(lbuff,text);
    strcat(lbuff,"\n");
    ch = lbuff; LineLength = BoxLength = Lines = SpaceAt = 0;
    if (flags & BOX_WIDEBOX)
      Avail = max((activeWin->Right-activeWin->Left)-10,20);
    else
      Avail = max(min((activeWin->Right-activeWin->Left)-10,50),20);
    while (*ch)
      {
        if (*ch >= 0)
          LineLength++;
        if (*ch == '\n')
          {
            Lines++;
            if (LineLength > BoxLength)
              BoxLength = LineLength;
            LineLength = 0;
            SpaceAt = 0;
          }
        if (LineLength >= Avail)
          {
            for (i=0;ch - i > lbuff && i < Avail/3;i++)
              if (*(ch - i) == ' ')
                {
                  ch = ch - i;
                  ASSERT(*ch == ' ');
                  *ch = '\n';
                  for (int16 j=0;j!=i;j++)
                    if (ch[j+1] > 0)
                      LineLength--;
                  goto FoundSpace;
                }
            String temp;
            temp = ch;
            *ch = '\n';
            for (i=0;temp[i];i++)
              ch[i + 1] = temp[i];
            ch[i+1] = 0;
            goto FoundSpace;
          }
        ch++;
        FoundSpace:;
      }
    Lines++;

    if (!(flags & BOX_NOSAVE))
      Save();

    SetWin(WIN_SCREEN);
    SizeWin(WIN_CUSTOM,BoxLength-1,min(40,Lines-2));

    DrawBorder(WIN_CUSTOM,cbor);
    ClearScroll();
    Color(ctxt);

    ch = lbuff; c=1; real_c=1; buff2[0] = ' '; y = -1;

    while(*ch)
      {
        if (*ch == '\n')
          {
            y++;
            while(real_c<BoxLength)
              {
                buff2[c++] = ' ';
                real_c++;
              }
            buff2[c++] = 0;
            SWrite(0,y,buff2);
            c = 1; real_c = 1;
          }
        else
          {
            buff2[c++] = *ch;
            if (*ch > 0)
              real_c++;
          }
        ch++;
      }
    UpdateScrollArea(0,WIN_CUSTOM);
    saveMode = Mode; Mode = MO_SPLASH;
    done = false; in = 0;
    if (!(flags & BOX_NOPAUSE))
      do {
        int16 oldMode;
        oldMode = GetMode();
        SetMode(MO_DIALOG);
        in = GetCharCmd(KY_CMD_ARROW_MODE);
        SetMode(oldMode);
        switch(in) {
          case KY_CMD_NORTH: 
          case KY_CMD_NORTHWEST:
          case KY_CMD_NORTHEAST:
            ScrollUp(); break;
            //UpdateScrollArea(offset- WinSizeY() - 6, WIN_CUSTOM); break;

          case KY_CMD_SOUTH: 
          case KY_CMD_SOUTHWEST:
          case KY_CMD_SOUTHEAST:
            ScrollDown(); break;
            //UpdateScrollArea(offset + (WinSizeY() - 6), WIN_CUSTOM); break;

          case KY_CMD_EAST:
          case KY_CMD_WEST:
            break;

          case KY_CMD_ENTER:
          case KY_ENTER: done = true; break; 

          default:
            if (!(flags & BOX_MUST_PRESS_ENTER))
              done = true;
            break;
          }
        }
      while (!done);

    if (flags & BOX_SAVECHAR)
      QueuedChar = in;
    Mode = saveMode;
    if (!(flags & BOX_NOSAVE))
      Restore();
  }



  int16 TextTerm::WrapWrite(int16 x,int16 y,const char* _s, int16 x2, int16 maxlines) {
      String l, v, s; s = _s;
      int i, pos, lines;
      /* Trim Trailing Returns */
      while (s.GetLength() && s[s.GetLength()-1] == '\n')
          s = s.Left(s.GetLength()-1);
      GotoXY(x,y); lines = 0; cWrap = 0;
      while(1) {
          int space_left_on_this_line = (x2 ? x2 : WinSizeX()) - max(x,cWrap);
          int trueLength = s.GetTrueLength(); 
          if (space_left_on_this_line > trueLength) {
              if (trueLength > 0) { lines++; } 
              Write(s);
              GotoXY(max(x,cWrap),y+lines);
              return lines;
          }
          pos = (s.TrueLeft(space_left_on_this_line)).GetLength()-1;

          int good_breakpoint = pos;
          while (good_breakpoint && 
              s[good_breakpoint] != ' ' &&
              s[good_breakpoint] != '\n')
              good_breakpoint --;
          if (!good_breakpoint) {
              // just write out what we have 
              l = s.TrueLeft(pos);
          } else {
              l = s.Left(good_breakpoint);
          } 
          if (l.strchr('\n')) { 
              pos = l.strchr('\n'); 
              l = s.Left(pos); 
          }
          s = s.Right((s.GetLength() - l.GetLength()));
          lines++; Write(l); GotoXY(max(x,cWrap),y+lines);
          if ((y + lines) >= WinSizeY() || lines >= maxlines)
              return lines;
          while (s.GetLength() && s[0] == ' ')
              s = s.Right(s.GetLength()-1);
      }  
  }

/*****************************************************************************\
*                                 TextTerm                                   *
*                              Scrolling Text                                 *
\*****************************************************************************/

void TextTerm::UpdateScrollArea(int16 _offset, int16 wn)
  {
    int16 x,y;
    SetWin(wn);
    Clear();
    offset = _offset == -2 ? offset : _offset;
    if (offset > ScrollLines - WinSizeY())
      offset = max(0,ScrollLines - WinSizeY());
    if (offset < 0)
      offset = 0;
    for(y=0;y!=WinSizeY();y++) {
      if (y+offset > ScrollLines)
        break;
      BlitScrollLine(wn,y+offset,y);
      }
    if (offset)
      PutChar(WinSizeX()-1,0,GLYPH_ARROW_UP + (EMERALD*256));
    if (offset < ScrollLines - WinSizeY())
      PutChar(WinSizeX()-1,WinSizeY()-1,GLYPH_ARROW_DOWN + (EMERALD*256));

    updated = false;  
  }


void TextTerm::HyperTab(int16 wn)
  {
    int16 CurrLine, i, dist, ln, sx, sy;
    bool isLink;
    /* Assumptions:
       * There is only one hyper-link per line.
       * Each hyperlink fits on a single line.
       * No text in the scrolling win has attr 0+15*16.
       * All links are attr 15+0*16
       * All links are bracketed with '>' and '<'
       * The character '<' does not occur naturally in the
           same line as a hyperlink.
    */
    
    if (!LinkCount)
      return;

    /* Get Scroll Win Dimensions */
    sx = min(SCROLL_WIDTH,Windows[wn].Right - Windows[wn].Left);
    sy = Windows[wn].Bottom - Windows[wn].Top;

    /* Where "are we" on the page? */
    if (SelectedLink == -1)
      CurrLine = 0;
    else 
      CurrLine = HL[SelectedLink].LineNo;

    /* De-highlight the old link */
    if (SelectedLink != -1)
      {
        for(i=0;i!=80;i++)
          if (SGetColor(i,HL[SelectedLink].LineNo) == 15*16)
            SPutColor(i,HL[SelectedLink].LineNo, 15);
      }
        
    /* Find a link further down. */ 
    ln = -1;   
    for (i=0;i!=LinkCount;i++)
      // Is the link currently displayed in WIN_CUSTOM?
      if (HL[i].LineNo >= offset && HL[i].LineNo <= offset+sy)
        // Is the link below the current one?
        if (HL[i].LineNo > CurrLine)
          // Is it the NEAREST below the current one?
          if (ln == -1 || HL[i].LineNo < HL[ln].LineNo)
            // Make this link the current one.
            ln = i;       
    if (ln != -1)
      goto Found;      
    
    /* Ok, so find a link above, starting from the top */
    ln = -1;   
    for (i=0;i!=LinkCount;i++)
      // Is the link currently displayed in WIN_CUSTOM?
      if (HL[i].LineNo >= offset && HL[i].LineNo <= offset+sy)
        // Is the link above the current one?
        if (HL[i].LineNo < CurrLine)
          // Is it the NEAREST TO THE TOP? 
          if (ln == -1 || HL[i].LineNo < HL[ln].LineNo)
            // Make this link the current one.
            ln = i;       
    if (ln != -1)
      goto Found;

    /* No Link, so default to the old one */
    ln = SelectedLink;
    if (ln == -1)
      return;
    goto Found;

    /* Found the Link; Highlight It. */
    Found:
    isLink = false;
    for(i=0;i!=80;i++)
      {
        if (SGetChar(HL[ln].LineNo,i) == '<')
          isLink = true;
        if (isLink)
          SPutColor(HL[ln].LineNo,i,WHITE*16);
        if (SGetChar(HL[ln].LineNo,i) == '>')
          isLink = false;
      }
    
    /* Now Update the ScrollWin and SelectedLink. */
    SelectedLink = ln;
    UpdateScrollArea(offset,wn);

  }

void TextTerm::ClearScroll() {
    SClear();
    ScrollLines = 0;
    LinkCount = 0;
    SelectedLink = -1;
}

void TextTerm::SWrite(const char *text, int16 wn)
  {
    int32 oColor; bool isLink = false; int16 c;
    const char *ch;
    ScrollLines = max(scy+1,ScrollLines);
    if (scx > min(SCROLL_WIDTH,Windows[wn].Right-Windows[wn].Left))
      { scx = 0; scy++; }
    if (scy >= MAX_SCROLL_LINES)
      return;
    if (scx < 0)
      scx = 0;
    if (scy < 0)
      scy = 0;
    ch = text;
    updated = false;
    while(*ch)
      {
        if (*ch == '\n')
          {
            ASSERT(!isLink)
            scy++; ch++; scx = 0;
            cWrap = 0;
            ScrollLines = max(scy+1,ScrollLines);
            if (scy > MAX_SCROLL_LINES)
              return;
            continue;
          }
        if (scx > min(SCROLL_WIDTH,Windows[wn].Right-Windows[wn].Left))
          { scy++; scx = 0;
            ScrollLines = max(scy+1,ScrollLines);
            /*
            if (scy > activeWin->Bottom)
              return;
              */
          }
        if (*ch == '{' && isHelp)
          {
            oColor = attr;
            Color(WHITE);
            isLink = true;
            HL[LinkCount].LineNo = scy;
            HL[LinkCount].Column = scx;
            HL[LinkCount].Text[0] = 0;
            SPutChar(scx,scy,'<' + attr*256);
            scx++;
            ASSERT(isalnum(*(ch+1)));
            if (*(ch+1) && *(ch+2) && *(ch+3) && *(ch+4))
              if (*(ch+1)=='B' && *(ch+2)=='K' && *(ch+3)=='S' && *(ch+4)=='P')
                {
                  HL[LinkCount].Link[0] = 9;
                  HL[LinkCount].Link[1] = 0;
                  goto LinkNameRead;
                }
            HL[LinkCount].Link[0] = *(ch+1);
            if (isalnum(*(ch+2)))
              HL[LinkCount].Link[1] = *(ch+2);
            else
              HL[LinkCount].Link[1] = 0;
            LinkNameRead:
            HL[LinkCount].Link[2] = 0;
            isLink = true;
            ch++;
            continue;
          }
        if (*ch == ':' && isLink)
          {
            ch++;
            c = 0;
            while(isalnum(*ch) || isspace(*ch))
              {
                if (c < 38)
                  HL[LinkCount].Text[c] = *ch;
                c++;
                ch++;
              }
            HL[LinkCount].Text[c] = 0;
            continue;
          }
        if (*ch == '}' && isLink)
          {
            SPutChar(scx,scy,'>' + attr*256);
            scx++;
            Color(oColor);
            isLink = false;
            LinkCount++;
            ASSERT(LinkCount < 511);
            ch++;
            continue;
          }
        if ((*ch < 0) && (*ch > -16))
          {
            Color(-*ch); ch++;
            continue;
          }
        if (*ch == '~') {
          SPutChar(scx,scy,'%' + attr*256);
          }
        else if (*ch == '_' && (activeWin != &Windows[WIN_DEBUG])) {
          SPutChar(scx,scy,' ' + attr*256);
          }
        else if (*ch == LITERAL_CHAR) {
          ch++;
          SPutChar(scx,scy,*ch + attr*256);
          }
        else if (*ch == WRAP_BREAK) 
          cWrap = scx+1;
        else if (*ch == WRAP_INDENT) 
          cWrap = scx+3;
        else {
          SPutChar(scx,scy,*ch + attr*256);
          }
        scx++; ch++;
      }

  }

int16 TextTerm::SWrapWrite(int16 x, int16 y, const char* _s, int16 x2, int16 wn)
  {
    String l, v, s; s = _s; bool cr;
    
    int i, pos, lines;
    
    x2 = min(SCROLL_WIDTH,x2);
    
    /* Trim Trailing Returns */
    while (s.GetLength() && s[s.GetLength()-1] == '\n')
      s = s.Left(s.GetLength()-1);

    SGotoXY(x,y); lines = 0; cWrap = 0;
    while(1) {
      PurgeStrings();
      pos = (x2 ? x2 : min(SCROLL_WIDTH,Windows[wn].Right-Windows[wn].Left)) - max(cWrap,x);
      if (pos > s.GetTrueLength() && !s.strchr('\n'))
        {
          lines++; SWrite(s,wn);
          SGotoXY(max(x,cWrap),y+lines);
          return lines;
        }
      pos = (s.TrueLeft(pos)).GetLength()-1;
      while (pos && s[pos] != ' ' && s[pos] != '\n')
        pos--;
      l = s.Left(pos);
      if (l.strchr('\n') || l[0] == '\n')
        { pos = l.strchr('\n'); 
          l = s.Left(pos); cr = true; }
      else
        cr = false;
        
      s = s.Right((s.GetLength() - l.GetLength())-1);
      if (l[0] == ' ' && l[1] != ' ')
        l = l.Right(l.GetLength()-1);
      lines++; SWrite(l,wn); 
      if (cr) cWrap = 0;
      SGotoXY(max(x,cWrap),y+lines);
      if ((y + lines) >= MAX_SCROLL_LINES)
        return lines;
      }  

  }

/*****************************************************************************\
*                                 TextTerm                                   *
*                              Formatted Menus                                *
\*****************************************************************************/
void TextTerm::LOption(const char*text,int32 val,const char*desc,
                        int32 sortkey)
  {
    if (OptionCount >= 510)
      return;
    for (int i=0; i<OptionCount; i++)
      if (Option[i].Val == val)
        return; // ww: no need for duplicates!
    Option[OptionCount].Text = text;
    Option[OptionCount].Val = val;
    Option[OptionCount].Desc = desc;
    Option[OptionCount].SortKey = sortkey;
    Option[OptionCount].isMarked = false;
    OptionCount++;
  }

void TextTerm::LOptionClear()
{
        OptionCount=0;
}

int LMenuOptionAlphaSort(const void *a, const void *b) {
    LMenuOption *l = (LMenuOption *)a, *r = (LMenuOption *)b;
    if (l->SortKey == r->SortKey) {
        /* we don't want "3 uncursed potions of levitation" to come before
        *                "uncursed potion of growth" */
        const char * a ;
        const char * b ; 
        a = strstr(l->Text,"cursed"); if (!a) a = l->Text;
        b = strstr(r->Text,"cursed"); if (!b) b = r->Text;
        return (strcmp(a,b));
    } else
        return (l->SortKey - r->SortKey);
}

int32 TextTerm::LMenu(uint16 fl, const char*_title,int8 MWin,const char*help, int8 pos) {
    /* ww: since hijklnm etc are taken by the roguelike keyset, give
    * roguelike players 123... options instead */
    const char* MenuLetters =
        (theGame->Opt(OPT_ROGUELIKE)) ? 
        "acdefgimopqrstvwxzACDEFGIMOPQRSTVWXZ                  " :
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ  ";
    int16 ch, i,c,p,tcol, qk, szCol, Rows, Cols, Width, Height, DY;
    char buff[60], let[5]; int16 vStart, vRows;
    const char *itm;
    String title = _title;
    if (Mode == MO_RECREATE) {
        ASSERT(strncmp(RInf.Rsp[RInf.cRsp].Question,title,31) == 0);
        LOptionClear();
        return RInf.Rsp[RInf.cRsp++].Answer;
    }

    if (fl & MENU_SORTED)
        qsort(Option,OptionCount,sizeof(Option[0]),LMenuOptionAlphaSort);

    PrePrompt();

Restart:
    c = 0;
    if (pos) {
        for (int i=0; i<OptionCount; i++) 
            if (Option[i].Val == pos) 
                c = i; 
    } 

    Cols = (fl & MENU_4COLS) ? 4 :
        (fl & MENU_3COLS) ? 3 :
        (fl & MENU_2COLS) ? 2 : 1;

    Save();
    Rows = max(1,OptionCount / Cols);
    if (Cols > 1)
        if (OptionCount % Cols)
            Rows++;
    vRows = min((fl & MENU_BORDER) ? 32 : 34,Rows);
    vStart = 0;

    DY = 0;
    if (MWin == WIN_MENUBOX) {
        szCol = 0;
        for(i=0;i!=OptionCount;i++)
            if (strlen(Option[i].Text) > szCol)
                szCol = strlen(Option[i].Text);
        szCol += 6;
        if (szCol > 76 / Cols) szCol = 76 / Cols; 
        for(i=0;title[i];i++)
            if (title[i] == '\n')
                DY++;
        if (fl & MENU_LARGEBOX) {
            SetWin(WIN_SCREEN);
            Width = activeWin->Right - activeWin->Left;
            Width -= 8;
            szCol = Width / Cols;
        } else if (fl & MENU_WIDE) {
            SetWin(WIN_SCREEN);
            Width = activeWin->Right - activeWin->Left;
            Width -= 16;
            szCol = Width / Cols;          
        } else if (!title)
            Width = 4 + szCol * Cols;
        else
            Width = 2 + max(min(45,strlen(title)),szCol*Cols);
        Height = vRows + (title ? DY+1 : 0) + 1;
        if (fl & MENU_LARGEBOX) {
            SetWin(WIN_SCREEN);
            Height = WinSizeY() - 8;
        } else if (fl & MENU_DESC)
            Height += 9;

        if (fl & MENU_DESC) {          
            SizeWin(WIN_MENUBOX, Width, Height);
            SetWin(WIN_MENUBOX);
            SizeWin(WIN_MENUDESC,WinLeft(),WinTop()+(Height-9),WinRight(),WinBottom());
        } else
            SizeWin(WIN_MENUBOX, Width, Height);
        SetWin(WIN_SCREEN); Color(EMERALD);
    } else {
        SetWin(MWin);
        szCol = ((activeWin->Right - activeWin->Left) / Cols);
    }
    if (fl & MENU_BORDER)
        DrawBorder(MWin,EMERALD);
    for (i=0;i!=OptionCount;i++)
        Option[i].Text = Option[i].Text.Left(szCol - 4); // ww: - "[A] "

    /*
    SetWin(MWin);
    tcol = activeWin->Bottom - activeWin->Top;
    if (fl & MENU_DESC)
    {
    SetWin(WIN_MENUDESC);
    tcol -= (activeWin->Bottom - activeWin->Top)+1;
    ASSERT(tcol>1);
    }
    vRows = min(vRows,tcol);
    */

    do {            
        SetWin(MWin);
        Clear();
        PurgeStrings();
        DY = (MWin == WIN_MENUBOX) ? 1 : 2;
        if (title) {
            Color(15);
            if (strlen(title) > WinSizeX()-3 || strchr(title,'\n')) {
                if (fl & MENU_SWRAPWRITE) {
                    ClearScroll(); 
                    DY += SWrapWrite(3,1,(fl & MENU_RAW) ? (const char*) title : XPrint(title),WinSizeX()-2,MWin);
                    UpdateScrollArea(0,MWin); 
                } else if (MWin == WIN_CREATION) {
                    DY += WrapWrite(3,1,(fl & MENU_RAW) ? (const char*) title : XPrint(title),WinSizeX()-2);
                } else
                    DY += WrapWrite(0,0,(fl & MENU_RAW) ? (const char*) title : XPrint(title));
            } else {
                p = ((WinSizeX()/2) - (strlen(title)/2)) /* * (MWin != WIN_MENUBOX)*/; 
                GotoXY(p,(MWin == WIN_MENUBOX) ? 0 : 1);
                if (fl & MENU_RAW)
                    Write(title); 
                else
                    Write(XPrint(title));
                DY++;
            }
        }
        if (fl & MENU_DESC) {
            SetWin(MWin);
            SizeWin(WIN_MENUDESC, WinLeft(),min(WinTop()+vRows+DY+1,WinBottom()-5),WinRight(),WinBottom());
        }

        /* center things nicely */ 
        if (c >= vStart + vRows*Cols)
            vStart = c - (vRows*Cols-1);
        if (c < vStart) 
            vStart = c; 

        SetWin(MWin);
        for(i=vStart;i!=min(OptionCount,vStart+vRows*Cols);i++) {
            GotoXY(((i-vStart)/vRows)*szCol,((i-vStart)%vRows)+DY);
            let[0] = MenuLetters[min(53,i)];
            let[1] = 0;
            if (fl & MENU_QKEY)
                for (qk=0;qk!=MAX_QKEYS;qk++)
                    if (Option[i].Val == QuickKeys[qk].Value)
                        if (QKeyType == QuickKeys[qk].Type) {
                            let[0] = -EMERALD;
                            let[1] = '0' + qk;
                            let[2] = -((c == i) ? WHITE : GREY);
                            let[3] = 0;
                        }
            if (c == i) {
                /* ww: so that [A] cursed long sword 
                * shows up as <blue>[A] <red>cursed<white>long sword
                * and not <white][A] <red>cursed<white>long sword
                * which makes it impossible to tell where you are 
                * fjm: I fixed this using Decolorize(), so that the
                * entire menu item is blue. */
                Color(WHITE);
                Write(Format("  [%s] ",let));
                Color(AZURE);
                Write(Decolorize(Option[i].Text).Left(szCol - 5));
                Color(GREY);
            } else {
                Color(GREY);
                Write(Format("  [%s] %.*s",let,
                    szCol - 5, 
                    (const char*) Option[i].Text
                    )); 
            }
        }
        if (fl & MENU_DESC) {
            ClearScroll();
            Color(1);
            SWrapWrite(3,0,XPrint(Option[c].Desc),WinSizeX()-2,WIN_MENUDESC);
            UpdateScrollArea(0,WIN_MENUDESC);
            SetWin(MWin);
        }

InvalidChar:
        {
            int16 oldMode;
            oldMode = GetMode();
            SetMode(MO_DIALOG);
            if (i < 27)
                ch=tolower(GetCharCmd(KY_CMD_ARROW_MODE));
            else
                ch = GetCharCmd(KY_CMD_ARROW_MODE);
            SetMode(oldMode);
        }
        switch(ch) {
        case KY_REDRAW:
            if (fl & MENU_REDRAW)
                return -2;
            break;
        case KY_CMD_NORTHEAST:
        case KY_CMD_NORTHWEST:
            if (fl & MENU_DESC) { 
                SetWin(MWin);
                ScrollUp(WIN_MENUDESC);
            } 
            goto InvalidChar;
        case KY_CMD_SOUTHEAST:
        case KY_CMD_SOUTHWEST:
            if (fl & MENU_DESC) { 
                SetWin(MWin);
                ScrollDown(WIN_MENUDESC);
            } 
            goto InvalidChar;
        case KY_CMD_NORTH:
            if (c > 0)
                c--;
            else
                c = OptionCount - 1; 
            SetDebugText("NORTH");
            break;
        case KY_CMD_SOUTH:
            if (c < OptionCount-1)
                c++;
            else  {
                vStart = 0; 
                c = 0; 
            }
            SetDebugText("SOUTH");
            break;
        case KY_CMD_WEST:
            if (c - vRows >= vStart)
                c -= vRows;
            break;
        case KY_CMD_EAST:
            if (c + vRows <= min(OptionCount-1,vStart+(vRows*Cols-1)))
                c += vRows;
            break;   
        case KY_ENTER:
            Restore();
            OptionCount=0;
            if (Mode == MO_CREATE) {
                strncpy(RInf.Rsp[RInf.nRsp].Question,title,31);
                RInf.Rsp[RInf.nRsp].Answer = Option[c].Val;
                RInf.nRsp++;
            }             
            return Option[c].Val;
        case KY_ESC:
            if (fl & MENU_ESC) {
                Restore();
                OptionCount = 0;
                if (Mode == MO_CREATE) {
                    strncpy(RInf.Rsp[RInf.nRsp].Question,title,31);
                    RInf.Rsp[RInf.nRsp].Answer = -1;
                    RInf.nRsp++;
                }
                return -1;
            }
            break;
        case KY_HELP:
            if (help) {
                Restore();
                if (!strchr(help,','))
                    HelpTopic(help);
                else {
                    static String a, b;
                    a = Upto(help,",/");
                    b = Right(help,strlen(help) - (a.GetLength()+1));
                    HelpTopic(a,b);
                }
                goto Restart;
            }
            break;
        case ' ':
            break;
        default:
            if (isdigit(ch)) {
                if (!(fl & MENU_QKEY))
                    break;
                QuickKeys[ch - '0'].Type  = QKeyType;
                QuickKeys[ch - '0'].Value = Option[c].Val;
                QuickKeys[ch - '0'].MM    = 0;
                break; 
            }
            if (ControlKeys & ALT || ControlKeys & CONTROL) {
                if (ControlKeys & SHIFT) ch = toupper(ch); 
                for (i=0;i<OptionCount;i++) 
                    if ((Option[i].Text[0]) == ch) {
                        c = i; 
                        break; 
                    } 
            } else { 
                if (strchr(MenuLetters,ch) == NULL)
                    goto InvalidChar;
                if (strchr(MenuLetters,ch) - MenuLetters >= OptionCount)
                    goto InvalidChar;

                Restore();
                OptionCount = 0;
                if (Mode == MO_CREATE) {
                    strncpy(RInf.Rsp[RInf.nRsp].Question,title,31);
                    RInf.Rsp[RInf.nRsp].Answer =
                        Option[strchr(MenuLetters,ch)-MenuLetters].Val;
                    RInf.nRsp++;
                }              
                return Option[strchr(MenuLetters,ch)-MenuLetters].Val;
            }
        }
    } while(1);
}

bool TextTerm::LMultiSelect(uint16 fl, const char* _title,int8 MWin,const char*help,
                            int8 pos)
{
    int16 ch, i,j,c,p,tcol, qk, szCol, Rows, Cols, Width, Height, DY, barlen;
    char buff[60], let[5]; int16 vStart, vRows;
    const char *itm; String title;
    title = _title;

    barlen = 5;
    for (i=0;i!=OptionCount;i++)
        barlen = max(barlen,Option[i].Text.GetTrueLength()+6);


    if (fl & MENU_SORTED)
        qsort(Option,OptionCount,sizeof(Option[0]),LMenuOptionAlphaSort);

    PrePrompt();

Restart:

    c = 0;
    if (pos) {
        for (int i=0; i<OptionCount; i++) 
            if (Option[i].Val == pos) 
                c = i; 
    } 

    Cols = (fl & MENU_4COLS) ? 4 :
        (fl & MENU_3COLS) ? 3 :
        (fl & MENU_2COLS) ? 2 : 1;
    Save();
    Rows = max(1,OptionCount / Cols);
    if (Cols > 1)
        if (OptionCount % Cols)
            Rows++;
    vRows = min((fl & MENU_BORDER) ? 36 : 38,Rows);
    vStart = 0;


    DY = 0;
    if (MWin == WIN_MENUBOX)
    {
        szCol = 0;
        for(i=0;i!=OptionCount;i++)
            if (strlen(Option[i].Text) > szCol)
                szCol = strlen(Option[i].Text);
        szCol += 6;
        if (szCol > 76 / Cols) szCol = 76 / Cols; 
        for(i=0;title[i];i++)
            if (title[i] == '\n')
                DY++;
        if (fl & MENU_LARGEBOX)
        {
            SetWin(WIN_SCREEN);
            Width = activeWin->Right - activeWin->Left;
            Width -= 8;
            szCol = Width / Cols;
        }
        else if (fl & MENU_WIDE)
        {
            SetWin(WIN_SCREEN);
            Width = activeWin->Right - activeWin->Left;
            Width -= 16;
            szCol = Width / Cols;          
        }
        else if (!title)
            Width = 4 + szCol * Cols;
        else
            Width = 2 + max(min(45,strlen(title)),szCol*Cols);
        Height = vRows + (title ? DY+1 : 0) + 1;
        if (fl & MENU_LARGEBOX) {
            SetWin(WIN_SCREEN);
            Height = WinSizeY() - 8;
        }
        else if (fl & MENU_DESC)
            Height += 9;

        if (fl & MENU_DESC) {          
            SizeWin(WIN_MENUBOX, Width, Height);
            SetWin(WIN_MENUBOX);
            SizeWin(WIN_MENUDESC,WinLeft(),WinTop()+(Height-9),WinRight(),WinBottom());
        }
        else
            SizeWin(WIN_MENUBOX, Width, Height);
        SetWin(WIN_SCREEN); Color(EMERALD);
    }
    else
    {
        SetWin(MWin);
        szCol = ((activeWin->Right - activeWin->Left) / Cols);
    }
    if (fl & MENU_BORDER)
        DrawBorder(MWin,EMERALD);
    for (i=0;i!=OptionCount;i++)
        Option[i].Text = Option[i].Text.Left(szCol - 4); // ww: - "[A] "

    /*
    SetWin(MWin);
    tcol = activeWin->Bottom - activeWin->Top;
    if (fl & MENU_DESC)
    {
    SetWin(WIN_MENUDESC);
    tcol -= (activeWin->Bottom - activeWin->Top)+1;
    ASSERT(tcol>1);
    }
    vRows = min(vRows,tcol);
    */


    do {            
        SetWin(MWin);
        Clear();
        PurgeStrings();
        DY = (MWin == WIN_MENUBOX) ? 1 : 2;
        if (title) {
            Color(15);
            if (strlen(title) > WinSizeX()-3 || strchr(title,'\n')) {
                if (fl & MENU_SWRAPWRITE) {
                    ClearScroll(); 
                    DY += SWrapWrite(3,1,(fl & MENU_RAW) ? (const char*) title : XPrint(title),WinSizeX()-2,MWin);
                    UpdateScrollArea(0,MWin); 
                } else if (MWin == WIN_CREATION) {
                    DY += WrapWrite(3,1,(fl & MENU_RAW) ? (const char*) title : XPrint(title),WinSizeX()-2);
                }
                else
                    DY += WrapWrite(0,0,(fl & MENU_RAW) ? (const char*) title : XPrint(title));
            }
            else {
                p = ((WinSizeX()/2) - (strlen(title)/2)) /* * (MWin != WIN_MENUBOX)*/; 
                GotoXY(p,(MWin == WIN_MENUBOX) ? 0 : 1);
                if (fl & MENU_RAW)
                    Write(title); 
                else
                    Write(XPrint(title));
                DY++;
            }
        }
        if (fl & MENU_DESC) {
            SetWin(MWin);
            SizeWin(WIN_MENUDESC, WinLeft(),min(WinTop()+vRows+DY+1,WinBottom()-5),WinRight(),WinBottom());
        }

        /* center things nicely */ 
        if (c >= vStart + vRows*Cols)
            vStart = c - (vRows*Cols-1);
        if (c < vStart) 
            vStart = c; 

        SetWin(MWin);
        for(i=vStart;i!=min(OptionCount,vStart+vRows*Cols);i++) {
            GotoXY(((i-vStart)/vRows)*szCol,((i-vStart)%vRows)+DY);
            Color(WHITE);
            Write(Format("  [%c%c%c] %.*s",-PINK,Option[i].isMarked ? '*' : ' ',-GREY,
                szCol - 5, 
                (const char*) Option[i].Text
                ));
            for (j=((i-vStart)/vRows)*szCol+1+activeWin->Left;
                j!=((i-vStart)/vRows)*szCol+1+min(barlen,szCol)+activeWin->Left;j++)
                APutChar(j,((i-vStart)%vRows)+DY+activeWin->Top,
                (AGetChar(j,((i-vStart)%vRows)+DY+activeWin->Top) & 0x0FFF) | 
                ((c == i) ? 0x1000 : 0x0000));
        }
        if (fl & MENU_DESC)
        {
            ClearScroll();
            Color(1);
            SWrapWrite(3,0,XPrint(Option[c].Desc),WinSizeX()-2,WIN_MENUDESC);
            UpdateScrollArea(0,WIN_MENUDESC);
            SetWin(MWin);
        }

InvalidChar:
        {
            int16 oldMode;
            oldMode = GetMode();
            SetMode(MO_DIALOG);
            if (i < 27)
                ch=tolower(GetCharCmd(KY_CMD_ARROW_MODE));
            else
                ch = GetCharCmd(KY_CMD_ARROW_MODE);
            SetMode(oldMode);
        }
        switch(ch)
        {
        case KY_CMD_NORTHEAST:
        case KY_CMD_NORTHWEST:
            if (fl & MENU_DESC) { 
                SetWin(MWin);
                ScrollUp(WIN_MENUDESC);
            } 
            goto InvalidChar;
        case KY_CMD_SOUTHEAST:
        case KY_CMD_SOUTHWEST:
            if (fl & MENU_DESC) { 
                SetWin(MWin);
                ScrollDown(WIN_MENUDESC);
            } 
            goto InvalidChar;
        case KY_CMD_NORTH:
            if (c > 0)
                c--;
            else
                c = OptionCount - 1; 
            SetDebugText("NORTH");
            break;
        case KY_CMD_SOUTH:
            if (c < OptionCount-1)
                c++;
            else  {
                vStart = 0; 
                c = 0; 
            }
            SetDebugText("SOUTH");
            break;
        case KY_CMD_WEST:
            if (c - vRows >= vStart)
                c -= vRows;
            break;
        case KY_CMD_EAST:
            if (c + vRows <= min(OptionCount-1,vStart+(vRows*Cols-1)))
                c += vRows;
            break;   
        case KY_SPACE:
            Option[c].isMarked = !Option[c].isMarked;
            break;
        case KY_ENTER:
            Restore();
            return true;
        case KY_ESC:
            if (fl & MENU_ESC)
            {
                Restore();
                OptionCount = 0;
                return false;
            }
            break;
        case KY_HELP:
            if (help) {
                Restore();
                if (!strchr(help,','))
                    HelpTopic(help);
                else
                {
                    static String a, b;
                    a = Upto(help,",/");
                    b = Right(help,strlen(help) - (a.GetLength()+1));
                    HelpTopic(a,b);
                }
                goto Restart;
            }
            break;
        }
    }
    while(1);
}

int32 TextTerm::FirstSelected()
  {
    cOption = 0;
    do {
      if (cOption >= OptionCount)
        return 0;
      if (Option[cOption].isMarked)
        return Option[cOption].Val;
      cOption++;
      }
    while(1);
  }
      
int32 TextTerm::NextSelected()
  {
    cOption++;
    do {
      if (cOption >= OptionCount)
        return 0;
      if (Option[cOption].isMarked)
        return Option[cOption].Val;
      cOption++;
      }
    while(1);
  }


/*****************************************************************************\
*                                 TEXTTERM                                    *
*                              DumpContainers                                 *
\*****************************************************************************/

int16 TextTerm::DumpContainers()
{
  Thing *t; Item *it; 
  /* find all sources */ 
  bool anyGroundItems = false; 

  for (t = p->m->FirstAt(p->x,p->y) ; t ; t = p->m->NextAt(p->x,p->y)) {
    if (t->isItem()) {
      anyGroundItems = true; 
      if (t->isType(T_CHEST) || t->isType(T_CONTAIN) ) 
        LOption(Format("(on ground) %s",(const char*)t->Name(0)),t->myHandle,NULL,0);
    } 
  } 

  if (anyGroundItems)
    LOption("(All Items On Ground)",-2,0,0);

  for (it = p->FirstInv(); it; it = p->NextInv()) {
    if (it->isType(T_CONTAIN) || it->isType(T_CHEST))
      LOption(it->Name(0),it->myHandle,NULL,0);
  } 

  int32 c1 = LMenu(MENU_ESC|MENU_BORDER,"Dump What?",WIN_MENUBOX,0,0);
  if (c1 == -1) 
    return -1; 
  Thing * ct1 = NULL; 
  Container * cc1 = NULL;
  if (c1 != -2) {
    ct1 = oThing(c1);
    ASSERT(ct1);
    ASSERT(ct1->isType(T_CONTAIN) || ct1->isType(T_CHEST));
    cc1 = (Container *)ct1; 
  } 

  LOption("(Onto The Ground)",-2,0,0);
  for (it = p->FirstInv(); it; it = p->NextInv()) {
    if (it == ct1) continue; 
    if (it->isType(T_CONTAIN) || it->isType(T_CHEST))
      LOption(it->Name(0),it->myHandle,NULL,0);
  } 
  for (t = p->m->FirstAt(p->x,p->y) ; t ; t = p->m->NextAt(p->x,p->y)) {
    if (t->isItem() && t != ct1) {
      if (t->isType(T_CHEST) || t->isType(T_CONTAIN) ) 
        LOption(Format("(on ground) %s",(const char*)t->Name(0)),t->myHandle,NULL,0);
    } 
  } 

  int32 c2 = LMenu(MENU_ESC|MENU_BORDER,"Place Them Where?",WIN_MENUBOX,0,0);
  if (c2 == -1) return -1; 
  if (c2 == -2 && c1 == -2) return -1; 

  EventInfo e;
  e.Clear();
  e.EPActor = p; 
  if (c2 == -2) { 
    return cc1->DumpOutContents(e);
  } else {
    /* put things from ct1 into ct2 */
    Thing * ct2 = oThing(c2);
    ASSERT(ct2);
    ASSERT(ct2->isType(T_CONTAIN) || ct2->isType(T_CHEST));
    Container * cc2 = (Container *)ct2; 
    e.EItem = cc2; 

    if (!ct1) {
      /* put things from ground into ct2 */
      e.EItem2 = (Item *)p->m->FirstAt(p->x,p->y);
    } else {
      /* put things from container ct1 into ct2 */
      e.EItem2 = (Item *)oThing(cc1->Contents); 
    } 
    return cc2->PourInContents(e);
  } 
} 

