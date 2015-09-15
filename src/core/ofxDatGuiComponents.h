/*
    Copyright (C) 2015 Stephen Braitsch [http://braitsch.io]

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once
#include "ofxDatGuiEvents.h"
#include "ofxDatGuiConstants.h"

class ofxDatGuiInteractiveObject{

    public:

    // button events //
        typedef std::function<void(ofxDatGuiButtonEvent)> onButtonEventCallback;
        onButtonEventCallback buttonEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onButtonEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            buttonEventCallback = std::bind(listenerMethod, owner, _1);
        }

    // slider events //
        typedef std::function<void(ofxDatGuiSliderEvent)> onSliderEventCallback;
        onSliderEventCallback sliderEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onSliderEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            sliderEventCallback = std::bind(listenerMethod, owner, _1);
        }
    
    // text input events //
        typedef std::function<void(ofxDatGuiTextInputEvent)> onTextInputEventCallback;
        onTextInputEventCallback textInputEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onTextInputEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            textInputEventCallback = std::bind(listenerMethod, owner, _1);
        }
    
    // color picker events //
        typedef std::function<void(ofxDatGuiColorPickerEvent)> onColorPickerEventCallback;
        onColorPickerEventCallback colorPickerEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onColorPickerEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            colorPickerEventCallback = std::bind(listenerMethod, owner, _1);
        }
    
    // dropdown events //
        typedef std::function<void(ofxDatGuiDropdownEvent)> onDropdownEventCallback;
        onDropdownEventCallback dropdownEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onDropdownEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            dropdownEventCallback = std::bind(listenerMethod, owner, _1);
        }
    
    // 2d pad events //
        typedef std::function<void(ofxDatGui2dPadEvent)> on2dPadEventCallback;
        on2dPadEventCallback pad2dEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void on2dPadEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            pad2dEventCallback = std::bind(listenerMethod, owner, _1);
        }
    
    // matrix events //
        typedef std::function<void(ofxDatGuiMatrixEvent)> onMatrixEventCallback;
        onMatrixEventCallback matrixEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onMatrixEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            matrixEventCallback = std::bind(listenerMethod, owner, _1);
        }

    // internal events //
        typedef std::function<void(ofxDatGuiInternalEvent)> onInternalEventCallback;
        onInternalEventCallback internalEventCallback;
        
        template<typename T, typename args, class ListenerClass>
        void onInternalEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            using namespace std::placeholders;
            internalEventCallback = std::bind(listenerMethod, owner, _1);
        }

};

class ofxDatGuiFont{

    public:
        ofxDatGuiFont(bool retinaEnabled)
        {
            size = 6;
            highlightPadding = 3;
            mRetinaEnabled = retinaEnabled;
            if (mRetinaEnabled){
                size*=2;
                highlightPadding*=2;
                tFont.load(ofxDatGuiAssetDir+"/font-verdana.ttf", size);
            }
            labelHeight = getStringBoundingBox("ABCDEFG123456", 0, 0).height;
        }
        int size;
        int labelX;
        int labelHeight;
        int highlightPadding;

        void drawLabel(string text, int xpos, int ypos);
        void drawText(string text, ofColor color, int xpos, int ypos, bool highlight = false);
        ofRectangle getStringBoundingBox(string str, int x, int y);
    
    private:
        ofBitmapFont bFont;
        ofTrueTypeFont tFont;
        bool mRetinaEnabled;
};

class ofxDatGuiTextInputField : public ofxDatGuiInteractiveObject{

    public:
    
        enum ofxDatGuiTextInputFieldType
        {
            NUMERIC = 1,
            ALPHA_NUMERIC,
            COLORPICKER
        };
    
        ofxDatGuiTextInputField(float height, ofxDatGuiFont* font)
        {
            mFont = font;
            mTextColor = ofxDatGuiColor::TEXT;
            mBkgdColor = ofxDatGuiColor::INPUT;
            mRect.height = height;
            mTextChanged = false;
            mHighlightText = false;
            mMaxCharacters = 99;
            mType = ALPHA_NUMERIC;
            mTextActiveColor = ofxDatGuiColor::LABEL;
            mTextInactiveColor = ofxDatGuiColor::TEXT;
        }
    
        void setWidth(int w)
        {
            mRect.width = w;
        }
    
        void setOrigin(int x, int y)
        {
            mRect.x = x;
            mRect.y = y;
        }
    
        void draw()
        {        
        // center the text //
            int tx = mRect.width/2 - mFont->getStringBoundingBox(mType==COLORPICKER ? "#"+mText : mText, 0, 0).width/2;
            ofPushStyle();
                ofSetColor(mBkgdColor);
                ofDrawRectangle(mRect);
                mTextColor = mHighlightText ? mTextActiveColor : mTextInactiveColor;
                mFont->drawText(mType==COLORPICKER ? "#"+mText : mText, mTextColor, mRect.x+tx, mRect.y+mRect.height/2, mHighlightText);
            ofPopStyle();
        }
    
        int getWidth()
        {
            return mRect.width;
        }
    
        int getHeight()
        {
            return mRect.height;
        }
    
        bool hitTest(ofPoint m)
        {
//            cout << mRect << endl;
            return (m.x>=mRect.x && m.x<=mRect.x+mRect.width && m.y>=mRect.y && m.y<=mRect.y+mRect.height);
        }
    
        void setText(string text)
        {
            mText = text;
            mTextChanged = true;
        }
    
        string getText()
        {
            return mText;
        }
    
        void setTextActiveColor(ofColor color)
        {
            mTextActiveColor = color;
        }
    
        void setTextInactiveColor(ofColor color)
        {
            mTextInactiveColor = color;
        }
    
        void setTextInputFieldType(ofxDatGuiTextInputFieldType type)
        {
            mType = type;
        }
    
        void setBackgroundColor(ofColor color)
        {
            mBkgdColor = color;
        }
    
        void setMaxNumOfCharacters(int max)
        {
            mMaxCharacters = max;
        }
    
        virtual void onFocus()
        {
            mTextChanged = false;
            mHighlightText = true;
            if (mType != COLORPICKER) mBkgdColor = ofxDatGuiColor::BUTTON_OVER;
        }
    
        virtual void onFocusLost()
        {
            mHighlightText = false;
            if (mTextChanged){
                mTextChanged = false;
                ofxDatGuiInternalEvent e(ofxDatGuiEventType::INPUT_CHANGED, 0);
                internalEventCallback(e);
            }
            if (mType != COLORPICKER) mBkgdColor = ofxDatGuiColor::INPUT;
        }
    
        void onKeyPressed(int key)
        {
            if (!keyIsValid(key)) return;
            if (mHighlightText) mText = "";
            if (key!=OF_KEY_BACKSPACE){
                mText += key;
            }   else {
                if (mText.size() > 0) mText.resize(mText.size()-1);
            }
            mTextChanged = true;
            mHighlightText = false;
            mText = ofToUpper(mText);
        }
    
        bool keyIsValid(int key)
        {
            if (mType == COLORPICKER){
               if (key==OF_KEY_BACKSPACE){
                    return true;
            // limit string length to six hex characters //
                } else if (!mHighlightText && mText.size() == 6){
                    return false;
            // allows numbers 0-9 //
                }   else if (key>=48 && key<=57){
                    return true;
            // allows letters a-f & A-F //
                }   else if ((key>=97 && key<=102) || (key>=65 && key<=70)){
                    return true;
                }   else{
                    return false;
                }
            }   else if (mType == NUMERIC){
                if (key==OF_KEY_BACKSPACE){
                    return true;
            // allow dash (-) or dot (.) //
                }   else if (key==45 || key==46){
                    return true;
            // allows numbers 0-9 //
                }   else if (key>=48 && key<=57){
                    return true;
            // an invalid key was entered //
                }   else{
                    return false;
                }
            }   else{
            // allow anything //
                return true;
            }
        }
    
    private:
        string mText;
        ofRectangle mRect;
        bool mTextChanged;
        bool mHighlightText;
        int mMaxCharacters;
        ofColor mBkgdColor;
        ofColor mTextColor;
        ofColor mHighLightColor;
        ofColor mTextActiveColor;
        ofColor mTextInactiveColor;
        ofxDatGuiFont* mFont;
        ofxDatGuiTextInputFieldType mType;
};


