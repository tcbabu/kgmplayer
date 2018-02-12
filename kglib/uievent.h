#ifndef D_UIEVENT
#define D_UIEVENT
#if 0
typedef struct _kbevent {
 int event; /* 0:mouse movement,1:button press,2:button release,3:mouse move+button press,
               4: key press, 5:key release
            */
 int button;  /* button number */
 int button_state;  /* button state for mouse movement 0,1,2,4 (bit status)*/
 int key;    /* key number */
 int x, y; /* mouse position */
} KBEVENT;
#endif
KBEVENT ui_getevent(void);
int  ui_checkevent(KBEVENT *kbe);
KBEVENT  _uiSkipMouseMove();
int ui_Escape(int ch);
int ui_Delete(int ch);
int ui_Tab(int ch);
int ui_Linefeed(int ch);
int ui_Return(int ch);
int ui_Backspace(int ch);
int ui_Uparrow(int ch);
int ui_Downarrow(int ch);
int ui_Leftarrow(int ch);
int ui_Rightarrow(int ch);
int ui_Pageup(int ch);
int ui_Pagedown(int ch);
int ui_End(int ch);
int ui_Home(int ch);
int ui_Clear(int ch);

#endif
