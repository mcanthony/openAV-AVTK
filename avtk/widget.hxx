
#ifndef OPENAV_AVTK_WIDGET_HXX
#define OPENAV_AVTK_WIDGET_HXX

#include <stdio.h>
#include <string>
#include <cairo/cairo.h>

#include "utils.hxx"
#include "helpers.hxx"
#include "pugl/pugl.h"


namespace Avtk
{

class UI;
class Theme;
class Group;

class Widget
{
  public:
    Widget( Avtk::UI* ui_, int x_, int y_, int w_, int h_, std::string label_);
    virtual ~Widget(){}
    
    /// returns the name of the Widget
    const char* label(){return label_.c_str();}
    
    /// sets the visibility
    void visible( bool visibile );
    bool visible(){return visible_;}
    
    /// get the current value
    float value() { return value_; }
    
    /// set a new value, triggers "new-value" event
    void value( float v );
    
    virtual void draw( cairo_t* cr ) = 0;
    
    bool touches( int inx, int iny );
    
    /// called by the UI class on any event that occurs
    virtual int handle( const PuglEvent* event );
    
    /// called by the UI class when this widget has a mouse pressed
    void motion( int x, int y );
    
    int x, y, w, h;         /// widget co-ords and size
    std::string label_;      /// widget name - sometimes shown in UI
    bool  visible_;         /// widget visibility
    
    enum ClickMode {
      CLICK_NONE,           /// click has no effect
      CLICK_MOMENTARY,      /// click highlights button while mouse-down
      CLICK_TOGGLE,         /// click toggles value between zero and one
      CLICK_VALUE_FROM_Y,   /// click sets value to mouse Y position / widget Y
    };
    
    /// sets the click mode
    void clickMode( ClickMode cm );
    
    /// 0 when no mouse button is down, otherwise the mouse button pressed
    int mouseButton(){return mouseButtonPressed_;}
    
    /// the Avtk::UI pointer, used to redraw the view etc
    Avtk::UI* ui;
    
    /// sets a theme for a Widget
    void theme( Theme* t );
    
    void addToGroup( Group* parent, int itemNumber );
    
    void (*callback)(Widget* , void*);
  
  protected:
    Avtk::Group* parent_;
    
    /// local Theme pointer: themes are loaded at startup, and maintained until
    /// quitting, allowing for optimized redraws.
    Avtk::Theme* theme_;
    
    /// widget doesn't take any input: UI passes it along to other widgets
    bool noHandle_;
    
    /// group item number: useful for radio buttons and lists of selectable items
    bool groupChild;
    int groupItemNumber_;
    
    /// enum defines the way in which mouse click / drag works
    enum DragMode {
      DM_NONE,              /// drag has no effect
      DM_DRAG_VERTICAL,     /// vertical mouse drag will change value()
      DM_DRAG_HORIZONTAL,   /// horizontal mouse drag changes value()
    };
    
    void dragMode( DragMode cdm );
    DragMode dragMode(){return dm;}
    ClickMode clickMode(){return cm;}
    
    /// used for mouse-drag
    int mX, mY;
    
    /// control scroll operations
    bool scrollDisable;
    bool scrollInvert;
    
    /// sets the scroll-delta amount: that is the size of the change that occurs
    /// to value() in the widget. PX moved / <value> == step
    void setScrollDeltaAmount( float sda );
    
  
  private:
    ClickMode cm;
    DragMode dm;
    
    /// widgets current value, to get/set use value() and value( float )
    float value_;
    
    /// the callback and its userdata pointer. the user-data pointer is set to
    /// the Avtk::UI* that is passed into the contstructor. The callback 
    
    void* callbackUD;
    
    /// holds the currently pressed mouse-button
    int mouseButtonPressed_;
    
    float scrollDeltaAmount;

};

};

#endif // OPENAV_AVTK_WIDGET_HXX
