#ifndef __hdnesPackEditormainForm__
#define __hdnesPackEditormainForm__

#define COLOUR_CLIENT_ROM_VIEW_BG   0
#define COLOUR_CLIENT_ROM_VIEW_1    1
#define COLOUR_CLIENT_ROM_VIEW_2    2
#define COLOUR_CLIENT_ROM_VIEW_3    3

/**
@file
Subclass of mainForm, which is generated by wxFormBuilder.
*/

#include "common.h"
#include "mainForm.h"
#include "colourDialogClient.h"

//// end generated include

/** Implementing mainForm */
class hdnesPackEditormainForm : public mainForm, public colourDialogClient
{
	protected:
		// Handlers for mainForm events.
		void MenuFileNew( wxCommandEvent& event );
		void MenuFileOpen( wxCommandEvent& event );
		void MenuFileSave( wxCommandEvent& event );
		void MenuFileSaveAs( wxCommandEvent& event );
		void MenuFileGen( wxCommandEvent& event );
		void MenuFileExit( wxCommandEvent& event );
		void zoomRomChanged( wxSpinEvent& event );
		void romBGColour( wxCommandEvent& event );
		void romColour1( wxCommandEvent& event );
		void romColour2( wxCommandEvent& event );
		void romColour3( wxCommandEvent& event );
		void romViewPaletteHexChanged( wxCommandEvent& event );
		void romViewSizeChanged( wxSizeEvent& event );
        void romViewVScrolled( wxScrollEvent& event );
        void romViewHScrolled( wxScrollEvent& event );
		void romViewLDown( wxMouseEvent& event );
		void romViewLUp( wxMouseEvent& event );
		void romViewRUp( wxMouseEvent& event );
		void romViewMove( wxMouseEvent& event );
		void romViewLeave( wxMouseEvent& event );

        void closeWindow( wxCloseEvent& event );
	public:
		/** Constructor */
		hdnesPackEditormainForm( wxWindow* parent );
	//// end generated class members


        //--general session
        string lastDir;
        string romDir;
        string packDir;

        bool notSaved;

        Uint16 colourSelectSource;
        Uint32 tileCnt;

        void initGeneral();
        void configGeneral(string lineHdr, string lineTail);
        void saveCfgGeneral(fstream& inifile);
        void refreshCoreDataDisplay();
        void openColourDialog(Uint16 clientID);
        virtual void colourSelected(Uint8 selectedColour);
        void paintTile(wxImage &img, Uint8* tileData, Uint16 x, Uint16 y, wxColour cBG, wxColour c1, wxColour c2, wxColour c3);
        void drawLine(wxImage &img, wxPoint pt, int len, wxColour c, bool hDir);
        void drawRect(wxImage &img, wxPoint pt, wxPoint rectSize, wxColour c);

        void dataChanged();
        void dataSaved();
        //--end general session

        //--rom view session
        int romViewColours[4];
        int romViewDisplayWidth;
        int romViewDisplayHeight;
        vector<Uint32> romViewSelectedTiles;
        wxPoint romViewLDownPos;
        wxPoint romViewLCurrPos;
        bool romViewClicked;
        wxImage romViewImage;
        Uint32 rightClickedID;

        void initROMView();
        void configROMView(string lineHdr, string lineTail);
        void saveCfgROMView(fstream& inifile);
        void refreshROMView();
        void drawROMView();
        void showROMView();
        void romViewPaletteToText();
        void romViewMenu( wxCommandEvent& event );
        //--end rom view session



};

#endif // __hdnesPackEditormainForm__
