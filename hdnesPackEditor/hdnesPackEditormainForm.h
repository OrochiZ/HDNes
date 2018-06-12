#ifndef __hdnesPackEditormainForm__
#define __hdnesPackEditormainForm__

#define COLOUR_CLIENT_ROM_VIEW_BG   0
#define COLOUR_CLIENT_ROM_VIEW_1    1
#define COLOUR_CLIENT_ROM_VIEW_2    2
#define COLOUR_CLIENT_ROM_VIEW_3    3
#define COLOUR_CLIENT_GAME_OBJ_BG   4
#define COLOUR_CLIENT_NEW_SWAP_BG   5
#define COLOUR_CLIENT_NEW_SWAP_1    6
#define COLOUR_CLIENT_NEW_SWAP_2    7
#define COLOUR_CLIENT_NEW_SWAP_3    8

#define GAME_OBJ_PNL_PASTE              0
#define GAME_OBJ_PNL_COPY               1
#define GAME_OBJ_PNL_CONFIRM_PASTE      2
#define GAME_OBJ_PNL_CANCEL_PASTE       3
#define GAME_OBJ_PNL_DELETE             4
#define GAME_OBJ_PNL_HFLIP              5
#define GAME_OBJ_PNL_VFLIP              6
#define GAME_OBJ_PNL_REPLACE            7
#define GAME_OBJ_PNL_CONDITION          8
#define GAME_OBJ_PNL_CONFIRM_CONDITION  9
#define GAME_OBJ_PNL_CANCEL_CONDITION   10
#define GAME_OBJ_PNL_SHOW_NOT_UNIQUE    11
#define GAME_OBJ_PNL_AUTO_CONDITION     12
#define GAME_OBJ_PNL_GROUP_CONDITION    13
#define GAME_OBJ_PNL_HFLIP2             14
#define GAME_OBJ_PNL_VFLIP2             15

#define GAME_OBJ_PNL_PALETTE_SELECT_OFFSET    100

/**
@file
Subclass of mainForm, which is generated by wxFormBuilder.
*/

#include "common.h"
#include "mainForm.h"
#include "colourDialogClient.h"
#include "gameObjNode.h"
#include "classList.h"

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
		void romViewEnter( wxMouseEvent& event );

		void zoomGameObjsChanged( wxSpinEvent& event );
        void gameObjTItemBeginEdit( wxTreeEvent& event );
		void gameObjTItemChangeName( wxTreeEvent& event );
		void gameObjTItemOpenMenu( wxTreeEvent& event );
		void gameObjTItemSelected( wxTreeEvent& event );
        void gameObjsRawRUp( wxMouseEvent& event );
        void gameObjSpriteClicked( wxCommandEvent& event );
        void gameObjBGClicked( wxCommandEvent& event );
		void gameObjBGColour( wxCommandEvent& event );
		void gameObjsRawMove( wxMouseEvent& event );
		void gameObjsRawEnter( wxMouseEvent& event );
		void gameObjsRawSizeChanged( wxSizeEvent& event );
		void gameObjsRawVScrolled( wxScrollEvent& event );
		void gameObjsRawHScrolled( wxScrollEvent& event );
		void gameObjsRawLDown( wxMouseEvent& event );
		void gameObjsRawLUp( wxMouseEvent& event );
		void replaceBrightnessChanged( wxScrollEvent& event );
		void gameObjDefaultClicked( wxCommandEvent& event );

        void BGImageSelect( wxCommandEvent& event );
		void BGImageBrightness( wxSpinEvent& event );
		void BGImageHScrollRate( wxSpinEvent& event );
		void BGImageVScrollRate( wxSpinEvent& event );

		void SwapSeleted( wxListEvent& event );
        void SwapPaletteSelected( wxListEvent& event );
        void SwapPaletteNewChanged( wxCommandEvent& event );
        void SwapNewPaletteBGClicked( wxCommandEvent& event );
		void SwapNewPalette1Clicked( wxCommandEvent& event );
		void SwapNewPalette2Clicked( wxCommandEvent& event );
        void SwapNewPalette3Clicked( wxCommandEvent& event );
        void AddSwapClicked( wxCommandEvent& event );
        void UpdateSwapClicked( wxCommandEvent& event );
        void DeleteSwapClicked( wxCommandEvent& event );

        void ConditionSelected( wxListEvent& event );
		void ConditionTypeSelect( wxCommandEvent& event );
		void ConditionAdd( wxCommandEvent& event );
		void ConditionUpdate( wxCommandEvent& event );
		void ConditionDelete( wxCommandEvent& event );



		void HDImgSelected( wxListEvent& event );
		void HDImgSizeChanged( wxSizeEvent& event );
		void HDImgTileSelected( wxListEvent& event );
		void HDImgLDown( wxMouseEvent& event );
		void HDImgLUp( wxMouseEvent& event );
		void HDImgMove( wxMouseEvent& event );
		void HDImgEnter( wxMouseEvent& event );
		void HDImgRUp( wxMouseEvent& event );
		void HDImgAdd( wxCommandEvent& event );
		void HDImgRemove( wxCommandEvent& event );

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
        void paintTile(wxImage &img, Uint8* tileData, int x, int y, bool hFlip, bool vFlip, wxColour c1, wxColour c2, wxColour c3);
        void drawLine(wxImage &img, wxPoint pt, int len, wxColour c, bool hDir);
        void drawRect(wxImage &img, wxPoint pt, wxPoint rectSize, wxColour c);

        void romChanged();
        void dataChanged();
        void dataSaved();
        //--end general session

        //--rom view session
        Uint8 romViewColours[4];
        int romViewDisplayRows;
        int romViewDisplayWidth;
        int romViewDisplayHeight;
        int romViewCurrentRow;
        int romViewTileSize;
        wxImage romViewImage;

        //selection
        vector<Uint32> romViewSelectedTiles;
        wxPoint romViewLDownPos;
        wxPoint romViewLCurrPos;
        bool romViewClicked;
        Uint32 rightClickedID;
        int rightClickedTileX;
        int rightClickedTileY;
        wxImage romViewImageWithSelection;
        wxImage romViewImageDisplay;

        void initROMView();
        void configROMView(string lineHdr, string lineTail);
        void saveCfgROMView(fstream& inifile);
        void romViewROMChanged();
        void refreshROMView();
        void drawROMView();
        void drawROMViewSelection();
        void showROMView();
        void romViewPaletteToText();
        void romViewMenu( wxCommandEvent& event );
        //--end rom view session

        //--game objs session
        wxTreeItemId tItmGameObjRoot;
        wxTreeItemId tItmGameObjMenu;
        bool gameObjectTreeWillMove;
        wxTreeItemId tItmGameObjMove;

        int gameObjTileSize;
        wxImage gameObjRawImage;
        wxImage gameObjRawImage2;
        wxImage gameObjRawImageDisplay;
        wxImage gameObjNewImage;
        wxImage gameObjNewImage2;
        wxImage gameObjNewImageDisplay;
        int gameObjViewCentreX;
        int gameObjViewCentreY;
        int gameObjScrollMinH;
        int gameObjScrollMaxH;
        int gameObjScrollSizeH;
        int gameObjScrollMinV;
        int gameObjScrollMaxV;
        int gameObjScrollSizeV;
        int gameObjRawPasteX;
        int gameObjRawPasteY;
        int gameObjZoom;
        wxColor gameObjBlankColour;
        gameObjNode gameObjPasteData;

        wxPoint gameObjRawCurrPos;
        int gameObjectGenImageCnt;
        int gameObjectGenImageX;
        int gameObjectGenImageY;
        wxImage gameObjectGenImage;

        //selection
        vector<Uint32> gameObjSelectedTiles;
        wxPoint gameObjLDownPos;
        wxPoint gameObjLCurrPos;
        bool gameObjClicked;
        bool editCondition;
        Uint32 rightClickedgameObjID;
        int rightClickedGameObjTileX;
        int rightClickedGameObjTileY;
        vector<Uint32> gameObjconditionTiles;
        int conditionGameObjTileX;
        int conditionGameObjTileY;
        int conditionCounter;
        wxImage gameObjImageWithSelection;
        wxImage gameObjBaseTile;
        wxImage gameObjBaseTileNew;
        int selectedSwap;
        int selectedSwapPalette;
        Uint8 swapNewColours[4];
        int selectedCondition;
        bool loadingTab;

        void initGameObjs();
        void configGameObjs(string lineHdr, string lineTail);
        void saveCfgGameObjs(fstream& inifile);
        void loadGameObjs(fstream& file);
        void loadChildGameObjs(fstream& file, wxTreeItemId item);
        void loadGameObjItem(fstream& file, wxTreeItemId item, bool isRoot);
        void saveGameObjs(fstream& file);
        void saveChildGameObjs(fstream& file, wxTreeItemId item);
        void saveGameObjItem(fstream& file, wxTreeItemId item);
        void renameGameObjConditions();
        void renameChildGameObjConditions(wxTreeItemId item);
        void renameChildGameObjItemConditions(wxTreeItemId item);
        void removeGameObjImage(int index);
        void removeChildGameObjImage(wxTreeItemId item, int index);
        void removeChildGameObjItemImage(wxTreeItemId item, int index);
        void genGameObjsConditionPack(fstream& file);
        void genChildGameObjsConditionPack(fstream& file, wxTreeItemId item);
        void genGameObjItemConditionPack(fstream& file, wxTreeItemId item);
        void genGameObjsTilePack(fstream& file, bool withCondition);
        void genChildGameObjsTilePack(fstream& file, wxTreeItemId item, bool withCondition);
        void genGameObjItemTilePack(fstream& file, wxTreeItemId item, bool withCondition);
        void genCustomImage(fstream& file, gameTile t, paletteSwap s, bool isSprite, int swapID, bool isDefault, gameObjNode* gObj);

        void gameObjsROMChanged();
        void gameObjsTreeMenu( wxCommandEvent& event );

        void gameObjsMoveTreeItem(wxTreeItemId item, wxTreeItemId newParent, wxTreeItemId previousItem);
        void gameObjsSetWillMove(wxTreeItemId item);
        void gameObjsCancelWillMove(wxTreeItemId item);
        void gameObjsRawMenu( wxCommandEvent& event );
        bool checkPasteValid(string content);
        wxPoint convertGameObjRawPosition(wxPoint pos);
        gameObjNode* getGameObjsSelectedObjectTreeNode();

        void refreshNode();
        void refreshBGImage();
        void drawBGImage();
        void refreshGameObj();
        void clearGameObj();
        void drawGameObj();
        void adjustGameObjSize();
        void drawGameObjEdits();
        void drawGameObjPasteTiles();
        void drawGameObjSelection();
        void showGameObj(wxImage& displayImg, wxImage& displayImg2, int x, int y);
        void setReplacement(int imageID, int x, int y);
        void findGameObjNotUniqueTile();
        void addGameObjNotUniqueTileCondition();

        void loadSwaps();
        void showSwap();
        void showSwapPalette();
        void showNewSwapPalette();
        void updateNewSwapText();
        void updateSwapData(paletteSwap& s);
        void applySwap(Uint8* palette, paletteSwap& s);

        void loadConditions();
        void showCondition();
        void updateConditionData(condition& c);
        void showConditionPanel();

        //--end game objs session

        //--image session
        int selectedHDImg;
        wxPoint hdImgLDownPos;
        wxPoint hdImgLCurrPos;
        bool hdImgClicked;
        Uint32 rightClickedHDImgID;
        int rightClickedHDImgTileX;
        int rightClickedHDImgTileY;
        wxImage hdImgImageWithSelection;
        float hdImgScale;
        bool hdImgRendering;

        void initHDImg();
        void showHDImgImage();
        void listOutHDImgImages();
        void listOutHDImgTiles();
        wxPoint convertHDImgPosition(wxPoint pos);
        void hdImgMenu( wxCommandEvent& event );
        //--end image session
};

#endif // __hdnesPackEditormainForm__
