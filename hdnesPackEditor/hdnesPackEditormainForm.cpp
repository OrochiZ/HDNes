#include "hdnesPackEditormainForm.h"
#include "hdnesPackEditornewProjectDialog.h"
#include "hdnesPackEditorcolourSelectDialog.h"
#include "coreData.h"
#include "main.h"
#include "common.h"
#include <wx/wx.h>
#include <wx/clipbrd.h>
#include "gameObjNode.h"

hdnesPackEditormainForm::hdnesPackEditormainForm( wxWindow* parent )
:
mainForm( parent )
{
    initROMView();
    initGameObjs();
    initGeneral();

    //load config
    string configPath;
    configPath = main::exeDir + string("\\config.txt");

    fstream fs;
    string line;
    string lineHdr;
    string lineTail;
    fs.open(configPath, fstream::in);
    if(fs.is_open()){
        while(getline(fs, line)){
            size_t found = line.find_first_of(">");
            if(found!=string::npos){
                lineHdr = line.substr(0, found + 1);
                lineTail = line.substr(found + 1);
                configGeneral(lineHdr, lineTail);
                configROMView(lineHdr, lineTail);
                configGameObjs(lineHdr, lineTail);
            }
        }
        fs.close();
    }
}

void hdnesPackEditormainForm::closeWindow( wxCloseEvent& event ){
	fstream inifile;
	inifile.open(main::exeDir + "\\config.txt", ios::out);

    saveCfgGeneral(inifile);
	saveCfgROMView(inifile);
	saveCfgGameObjs(inifile);

	inifile.close();

    if ( event.CanVeto() && notSaved ){
        if ( wxMessageBox("The file has not been saved... continue closing?",
                          "Please confirm",
                          wxICON_QUESTION | wxYES_NO) != wxYES ){
            event.Veto();
            return;
        }
    }

	Show(false);
	Destroy();
}

void hdnesPackEditormainForm::romChanged(){
    romViewROMChanged();
    gameObjsROMChanged();
}

void hdnesPackEditormainForm::initGeneral(){
    lastDir = "";
    romDir = "";
    packDir = "";
    m_menu3->FindItem(m_menu3->FindItem(wxString("Save Project")))->Enable(false);
    m_menu3->FindItem(m_menu3->FindItem(wxString("Save Project As...")))->Enable(false);
    m_menu3->FindItem(m_menu3->FindItem(wxString("Generate Pack Data")))->Enable(false);
}

void hdnesPackEditormainForm::configGeneral(string lineHdr, string lineTail){
    if(lineHdr == "<lastDir>"){
        lastDir = lineTail;
    }
    else if(lineHdr == "<romDir>" ){
        romDir = lineTail;
    }
    else if(lineHdr == "<packDir>"){
        packDir = lineTail;
    }
}

void hdnesPackEditormainForm::saveCfgGeneral(fstream& inifile){
	inifile << "<lastDir>" + lastDir + "\n";
	inifile << "<romDir>" + romDir + "\n";
	inifile << "<packDir>" + packDir + "\n";
}

void hdnesPackEditormainForm::MenuFileNew( wxCommandEvent& event ){
    hdnesPackEditornewProjectDialog* fp = new hdnesPackEditornewProjectDialog(this);
    fp->Show(true);
}

void hdnesPackEditormainForm::MenuFileOpen( wxCommandEvent& event ){
    wxFileDialog dialog(this, wxString("Open project"), lastDir, lastDir, wxString("*.hnp"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if(dialog.ShowModal() == wxID_OK){
        if(coreData::cData){
            delete(coreData::cData);
        }
        coreData::cData = new coreData();
        coreData::cData->load(dialog.GetPath().ToStdString());
    }
}

void hdnesPackEditormainForm::MenuFileSave( wxCommandEvent& event ){
    if(coreData::cData->projectPath == ""){
        string romName = coreData::cData->romPath.substr(coreData::cData->romPath.find_last_of("\\/") + 1);
        romName = romName.substr(0, romName.find_last_of("."));
        wxFileDialog dialog(this, wxString("Save As"), lastDir, wxString(string(romName + ".hnp")), wxString("*.hnp"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(dialog.ShowModal() == wxID_OK){
            coreData::cData->saveAs(dialog.GetPath().ToStdString());
        }
    }
    else{
        coreData::cData->save();
    }
}

void hdnesPackEditormainForm::MenuFileSaveAs( wxCommandEvent& event ){
    string romName = coreData::cData->projectPath.substr(coreData::cData->projectPath.find_last_of("\\/") + 1);
    romName = romName.substr(0, romName.find_last_of("."));
    wxFileDialog dialog(this, wxString("Save As"), coreData::cData->projectPath, wxString(string(romName + ".hnp")), wxString("*.hnp"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if(dialog.ShowModal() == wxID_OK){
        coreData::cData->saveAs(dialog.GetPath().ToStdString());
    }
}

void hdnesPackEditormainForm::MenuFileGen( wxCommandEvent& event ){
// TODO: Implement MenuFileGen
}

void hdnesPackEditormainForm::MenuFileExit( wxCommandEvent& event ){
// TODO: Implement MenuFileExit
}

void hdnesPackEditormainForm::colourSelected(Uint8 selectedColour){
    switch(colourSelectSource){
    case COLOUR_CLIENT_ROM_VIEW_BG:
        romViewColours[0] = selectedColour;
        romViewPaletteToText();
        refreshROMView();
        break;
    case COLOUR_CLIENT_ROM_VIEW_1:
        romViewColours[1] = selectedColour;
        romViewPaletteToText();
        refreshROMView();
        break;
    case COLOUR_CLIENT_ROM_VIEW_2:
        romViewColours[2] = selectedColour;
        romViewPaletteToText();
        refreshROMView();
        break;
    case COLOUR_CLIENT_ROM_VIEW_3:
        romViewColours[3] = selectedColour;
        romViewPaletteToText();
        refreshROMView();
        break;
    case COLOUR_CLIENT_GAME_OBJ_BG:
        wxTreeItemId tID = treeGameObjs->GetFocusedItem();
        gameObjNode* data = (gameObjNode*)(treeGameObjs->GetItemData(tID));
        data->bgColour = selectedColour;
        refreshGameObj();
        break;
    }
}

void hdnesPackEditormainForm::openColourDialog(Uint16 clientID){
    if(coreData::cData){
        colourSelectSource = clientID;
        hdnesPackEditorcolourSelectDialog* fp = new hdnesPackEditorcolourSelectDialog(this);
        fp->setClientObj(this);
        fp->Show(true);
    }
}

void hdnesPackEditormainForm::paintTile(wxImage &img, Uint8* tileData, Uint16 x, Uint16 y, wxColour cBG, wxColour c1, wxColour c2, wxColour c3){
    wxColour useColour[4];
    useColour[0] = cBG;
    useColour[1] = c1;
    useColour[2] = c2;
    useColour[3] = c3;
    Uint8 decodeByte1;
    Uint8 decodeByte2;
    Uint8 decodedVal;
    for(Uint8 dy = 0; dy < 8; ++dy){
        decodeByte1 = tileData[dy];
        decodeByte2 = tileData[dy + 8];
        for(Uint8 dx = 0; dx < 8; ++dx){
            decodedVal = ((decodeByte1 >> dx) & 0x01) | (((decodeByte2 >> dx) << 1) & 0x02);
            img.SetRGB(x + 7 - dx, y + dy, useColour[decodedVal].Red(), useColour[decodedVal].Green(), useColour[decodedVal].Blue());
        }
    }
}

void hdnesPackEditormainForm::drawRect(wxImage &img, wxPoint pt, wxPoint rectSize, wxColour c){
    drawLine(img, pt, rectSize.x, c, true);
    drawLine(img, pt, rectSize.y, c, false);

    wxPoint p = pt;
    p.x += rectSize.x;
    drawLine(img, p, rectSize.y, c, false);
    p = pt;
    p.y += rectSize.y;
    drawLine(img, p, rectSize.x, c, true);
}

void hdnesPackEditormainForm::drawLine(wxImage &img, wxPoint pt, int len, wxColour c, bool hDir){
    wxPoint p = pt;
    for(int i = 0; i < len; ++i){
        if(p.x >= 0 && p.x < img.GetWidth()
        && p.y >= 0 && p.y < img.GetHeight()){
            img.SetRGB(p.x, p.y, c.Red(), c.Green(), c.Blue());
        }
        if(hDir){
            ++(p.x);
        }
        else{
            ++(p.y);
        }
    }
}

void hdnesPackEditormainForm::dataChanged(){
    refreshCoreDataDisplay();
    romDir = coreData::cData->romPath;
    packDir = coreData::cData->packPath;
    m_menu3->FindItem(m_menu3->FindItem(wxString("Save Project As...")))->Enable(true);
    m_menu3->FindItem(m_menu3->FindItem(wxString("Generate Pack Data")))->Enable(true);
    m_menu3->FindItem(m_menu3->FindItem(wxString("Save Project")))->Enable(true);
}

void hdnesPackEditormainForm::dataSaved(){
    lastDir = coreData::cData->projectPath;
    m_menu3->FindItem(m_menu3->FindItem(wxString("Save Project")))->Enable(false);
    notSaved = false;
}

void hdnesPackEditormainForm::initROMView(){
    romViewColours[0] = 13;
    romViewColours[1] = 18;
    romViewColours[2] = 22;
    romViewColours[3] = 26;
    romViewClicked = false;
    romViewPaletteToText();
    romHScroll->SetRange(1);
    romHScroll->SetThumbSize(1);
    romVScroll->SetRange(1);
    romVScroll->SetThumbSize(1);
    zoomRom->SetValue(4);
}

void hdnesPackEditormainForm::romViewROMChanged(){
    romHScroll->SetThumbPosition(0);
    romVScroll->SetThumbPosition(0);
    romViewClicked = false;
}

void hdnesPackEditormainForm::configROMView(string lineHdr, string lineTail){
    if(lineHdr == "<romViewColour0>" ){
        romViewColours[0] = atoi(lineTail.c_str());
        romViewPaletteToText();
    }
    else if(lineHdr == "<romViewColour1>" ){
        romViewColours[1] = atoi(lineTail.c_str());
        romViewPaletteToText();
    }
    else if(lineHdr == "<romViewColour2>" ){
        romViewColours[2] = atoi(lineTail.c_str());
        romViewPaletteToText();
    }
    else if(lineHdr == "<romViewColour3>" ){
        romViewColours[3] = atoi(lineTail.c_str());
        romViewPaletteToText();
    }
}

void hdnesPackEditormainForm::saveCfgROMView(fstream& inifile){
    ostringstream convert;
    string s;

	convert << romViewColours[0];
	s = convert.str();
	inifile << "<romViewColour0>" + s + "\n";
	convert.str("");
	convert.clear();
	convert << romViewColours[1];
	s = convert.str();
	inifile << "<romViewColour1>" + s + "\n";
	convert.str("");
	convert.clear();
	convert << romViewColours[2];
	s = convert.str();
	inifile << "<romViewColour2>" + s + "\n";
	convert.str("");
	convert.clear();
	convert << romViewColours[3];
	s = convert.str();
	inifile << "<romViewColour3>" + s + "\n";
}

void hdnesPackEditormainForm::zoomRomChanged( wxSpinEvent& event ){
    refreshROMView();
}

void hdnesPackEditormainForm::romBGColour( wxCommandEvent& event ){
    openColourDialog(COLOUR_CLIENT_ROM_VIEW_BG);
}

void hdnesPackEditormainForm::romViewPaletteHexChanged( wxCommandEvent& event ){
    string v = txtRomViewPalette->GetValue().ToStdString();
    main::hexToByteArray(v, (Uint8*)romViewColours);
    refreshROMView();
}

void hdnesPackEditormainForm::romColour1( wxCommandEvent& event ){
    openColourDialog(COLOUR_CLIENT_ROM_VIEW_1);
}

void hdnesPackEditormainForm::romColour2( wxCommandEvent& event ){
    openColourDialog(COLOUR_CLIENT_ROM_VIEW_2);
}

void hdnesPackEditormainForm::romColour3( wxCommandEvent& event ){
    openColourDialog(COLOUR_CLIENT_ROM_VIEW_3);
}

void hdnesPackEditormainForm::romViewSizeChanged( wxSizeEvent& event ){
    if(coreData::cData){
        refreshROMView();
    }
}

void hdnesPackEditormainForm::romViewVScrolled( wxScrollEvent& event ){
    if(coreData::cData){
        drawROMView();
    }
}

void hdnesPackEditormainForm::romViewHScrolled( wxScrollEvent& event ){
    if(coreData::cData){
        drawROMView();
    }
}

void hdnesPackEditormainForm::refreshCoreDataDisplay(){
    refreshROMView();
}

void hdnesPackEditormainForm::refreshROMView(){
    btnRomViewBGColour->SetBackgroundColour(coreData::cData->palette[romViewColours[0]]);
    if(coreData::cData->palette[romViewColours[0]].Red() + coreData::cData->palette[romViewColours[0]].Green() + coreData::cData->palette[romViewColours[0]].Blue() > 256){
        btnRomViewBGColour->SetForegroundColour(wxColour(0,0,0));
    }
    else{
        btnRomViewBGColour->SetForegroundColour(wxColour(255,255,255));
    }

    btnRomViewColour1->SetBackgroundColour(coreData::cData->palette[romViewColours[1]]);
    if(coreData::cData->palette[romViewColours[1]].Red() + coreData::cData->palette[romViewColours[1]].Green() + coreData::cData->palette[romViewColours[1]].Blue() > 256){
        btnRomViewColour1->SetForegroundColour(wxColour(0,0,0));
    }
    else{
        btnRomViewColour1->SetForegroundColour(wxColour(255,255,255));
    }

    btnRomViewColour2->SetBackgroundColour(coreData::cData->palette[romViewColours[2]]);
    if(coreData::cData->palette[romViewColours[2]].Red() + coreData::cData->palette[romViewColours[2]].Green() + coreData::cData->palette[romViewColours[2]].Blue() > 256){
        btnRomViewColour2->SetForegroundColour(wxColour(0,0,0));
    }
    else{
        btnRomViewColour2->SetForegroundColour(wxColour(255,255,255));
    }

    btnRomViewColour3->SetBackgroundColour(coreData::cData->palette[romViewColours[3]]);
    if(coreData::cData->palette[romViewColours[3]].Red() + coreData::cData->palette[romViewColours[3]].Green() + coreData::cData->palette[romViewColours[3]].Blue() > 256){
        btnRomViewColour3->SetForegroundColour(wxColour(0,0,0));
    }
    else{
        btnRomViewColour3->SetForegroundColour(wxColour(255,255,255));
    }

    romViewTileSize = 8 * zoomRom->GetValue();
    romViewCurrentRow = -1;

    tileCnt = coreData::cData->romSize / 16;
    romViewDisplayRows = ((tileCnt / 16) + (tileCnt % 16 ? 1 : 0));
    //display 16 tiles across
    romViewDisplayWidth = 16 * romViewTileSize;
    romViewDisplayHeight = romViewDisplayRows * romViewTileSize;

    romHScroll->SetRange(romViewDisplayWidth);
    romVScroll->SetRange(romViewDisplayHeight);
    romHScroll->SetThumbSize(pnlRom->GetSize().GetWidth());
    romVScroll->SetThumbSize(pnlRom->GetSize().GetHeight());

    romViewImageDisplay = wxImage(pnlRom->GetSize().x, pnlRom->GetSize().y);

    drawROMView();
}

void hdnesPackEditormainForm::drawROMView(){
    int newRowStart = romVScroll->GetThumbPosition() / romViewTileSize;
    int bufferRows = newRowStart % 3;
    newRowStart -= bufferRows;
    if(romViewCurrentRow != newRowStart){
        romViewCurrentRow = newRowStart;
        Uint16 visibleRows = (romVScroll->GetThumbSize() / romViewTileSize) + (romVScroll->GetThumbSize() % romViewTileSize ? 1 : 0) + 2;
        romViewImage = wxImage(16 * 8, visibleRows * 8, true);

        Uint32 memAddress;
        Uint16 drawX;
        Uint16 drawY;
        for(Uint16 j = 0; j < visibleRows; ++j){
            for(Uint16 i = 0; i < 16; ++i){
                if(romViewCurrentRow + j < romViewDisplayRows){
                    memAddress = ((romViewCurrentRow + j) * 16 + i) * 16;
                    if(memAddress < coreData::cData->romSize){
                        drawX = i * 8;
                        drawY = j * 8;
                        paintTile(romViewImage, coreData::cData->romData + memAddress, drawX, drawY,
                                coreData::cData->palette[romViewColours[0]],
                                coreData::cData->palette[romViewColours[1]],
                                coreData::cData->palette[romViewColours[2]],
                                coreData::cData->palette[romViewColours[3]]);
                    }
                }
            }
        }
        drawROMViewSelection();
    }
    else{
        showROMView();
    }
}

void hdnesPackEditormainForm::drawROMViewSelection(){
    romViewImageWithSelection = romViewImage.Scale(romViewImage.GetWidth() * zoomRom->GetValue(), romViewImage.GetHeight() * zoomRom->GetValue());
    int rowY = romViewCurrentRow * romViewTileSize;

    wxPoint pt;
    wxPoint pt2;
    wxPoint tileBoxSize;
    tileBoxSize.x = romViewTileSize - 1;
    tileBoxSize.y = romViewTileSize - 1;
    for (vector<Uint32>::iterator it = romViewSelectedTiles.begin() ; it != romViewSelectedTiles.end(); ++it){
        pt.x = ((*it) % 16) * romViewTileSize;
        pt.y = ((*it) / 16) * romViewTileSize - rowY;
        pt2 = pt;
        ++(pt2.x);
        ++(pt2.y);
        drawRect(romViewImageWithSelection, pt2, tileBoxSize, wxColour(0, 0, 0));
        drawRect(romViewImageWithSelection, pt, tileBoxSize, wxColour(255, 255, 255));
    }
    showROMView();
}

void hdnesPackEditormainForm::showROMView(){
    int rowY = romViewCurrentRow * romViewTileSize;
    romViewImageDisplay.Clear();
    romViewImageDisplay.Paste(romViewImageWithSelection, -romHScroll->GetThumbPosition(), rowY - romVScroll->GetThumbPosition());

    if(romViewClicked){
        wxPoint p1;
        p1.x = min(romViewLDownPos.x, romViewLCurrPos.x);
        p1.y = min(romViewLDownPos.y, romViewLCurrPos.y);

        wxPoint p2;
        p2.x = max(romViewLDownPos.x, romViewLCurrPos.x);
        p2.y = max(romViewLDownPos.y, romViewLCurrPos.y);

        wxPoint rectSize;
        rectSize.x = p2.x - p1.x;
        rectSize.y = p2.y - p1.y;

        wxPoint p3 = p1;
        ++(p3.x);
        ++(p3.y);


        drawRect(romViewImageDisplay, p3, rectSize, wxColour(0, 0, 0));
        drawRect(romViewImageDisplay, p1, rectSize, wxColour(255, 255, 255));
    }

    wxBitmap bmp = wxBitmap(romViewImageDisplay);
	if(bmp.IsOk()){
		wxClientDC* objDC;
		objDC = new wxClientDC(pnlRom);
		objDC->DrawBitmap(bmp, 0, 0);
		delete objDC;
	}
}

void hdnesPackEditormainForm::romViewPaletteToText(){
    wxString v;
    v = wxString((main::intToHex(romViewColours[0])
                + main::intToHex(romViewColours[1])
                + main::intToHex(romViewColours[2])
                + main::intToHex(romViewColours[3])).c_str());
    txtRomViewPalette->ChangeValue(v);

}

void hdnesPackEditormainForm::romViewLDown( wxMouseEvent& event ){
    if(coreData::cData){
        if(event.GetModifiers() != wxMOD_CONTROL){
            //clear currently selected
            romViewSelectedTiles.clear();
        }
        romViewLDownPos = event.GetPosition();
        romViewLCurrPos = romViewLDownPos;
        romViewClicked = true;
        drawROMViewSelection();
    }
}

void hdnesPackEditormainForm::romViewLUp( wxMouseEvent& event ){
    if(coreData::cData){
        if(romViewClicked){
            wxPoint p = event.GetPosition();
            int x1 = min(p.x, romViewLDownPos.x);
            int x2 = max(p.x, romViewLDownPos.x);
            int y1 = min(p.y, romViewLDownPos.y);
            int y2 = max(p.y, romViewLDownPos.y);

            int tileX1 = (romHScroll->GetThumbPosition() + x1) / romViewTileSize;
            int tileY1 = (romVScroll->GetThumbPosition() + y1) / romViewTileSize;
            int tileX2 = (romHScroll->GetThumbPosition() + x2) / romViewTileSize;
            int tileY2 = (romVScroll->GetThumbPosition() + y2) / romViewTileSize;

            Uint32 tileID;
            for(int j = tileY1; j <= tileY2; ++j){
                for(int i = tileX1; i <= tileX2; ++i){
                    if(i < 16){
                        tileID = j * 16 + i;
                        if(tileID < tileCnt){
                            //look for that id in vector
                            bool tileFound = false;
                            for(Uint32 k = 0; k < romViewSelectedTiles.size(); ++k){
                                if(romViewSelectedTiles[k] == tileID){
                                    romViewSelectedTiles.erase(romViewSelectedTiles.begin() + k);
                                    tileFound = true;
                                }
                            }
                            if(!tileFound){
                                romViewSelectedTiles.push_back(tileID);
                            }
                        }
                    }
                }
            }

            romViewClicked = false;
            drawROMViewSelection();
        }
    }
}

void hdnesPackEditormainForm::romViewRUp( wxMouseEvent& event ){
    //check right click on a selected tile
    if(coreData::cData){
        wxPoint p = event.GetPosition();
        int tileX1 = (romHScroll->GetThumbPosition() + p.x) / romViewTileSize;
        int tileY1 = (romVScroll->GetThumbPosition() + p.y) / romViewTileSize;
        if(tileX1 < 16){
            Uint32 tileID = tileY1 * 16 + tileX1;
            if(tileID < tileCnt){
                //look for that id in vector
                bool tileFound = false;
                for(Uint32 k = 0; k < romViewSelectedTiles.size(); ++k){
                    if(romViewSelectedTiles[k] == tileID){
                        tileFound = true;
                        rightClickedID = tileID;
                        rightClickedTileX = tileX1;
                        rightClickedTileY = tileY1;
                    }
                }
                if(tileFound){
                    wxMenu menu(wxT(""));
                    menu.Append(wxID_ANY, wxT("Copy"));
                    menu.Connect( wxEVT_MENU, wxCommandEventHandler(hdnesPackEditormainForm::romViewMenu), NULL, this );
                    pnlRom->PopupMenu(&menu, p);
                }
                else{
                    romViewSelectedTiles.clear();
                    drawROMViewSelection();
                }
            }
        }
    }
}

void hdnesPackEditormainForm::romViewMenu( wxCommandEvent& event ){
    string copyContent = "";
    int tileX;
    int tileY;
    for(Uint32 k = 0; k < romViewSelectedTiles.size(); ++k){
        tileX = romViewSelectedTiles[k] % 16;
        tileY = romViewSelectedTiles[k] / 16;
        if(copyContent != ""){
            copyContent = copyContent + ";";
        }
        copyContent = copyContent + coreData::cData->getTileID(romViewSelectedTiles[k])
                        + "," + txtRomViewPalette->GetValue().ToStdString()
                        + "," + main::intToStr((tileX - rightClickedTileX) * 8)
                        + "," + main::intToStr((tileY - rightClickedTileY) * 8);

    }

    if (wxTheClipboard->Open()){
        wxTheClipboard->SetData( new wxTextDataObject(copyContent.c_str()) );
        wxTheClipboard->Close();
    }
}

void hdnesPackEditormainForm::romViewMove( wxMouseEvent& event ){
    if(coreData::cData){
        wxPoint p = event.GetPosition();
        int tileX1 = (romHScroll->GetThumbPosition() + p.x) / romViewTileSize;
        int tileY1 = (romVScroll->GetThumbPosition() + p.y) / romViewTileSize;
        if(tileX1 < 16){
            int tileIndex = tileY1 * 16 + tileX1;
            if(tileIndex < tileCnt){
                m_statusBar->SetLabel(wxString(coreData::cData->getTileID(tileIndex)));
            }
        }

        if(romViewClicked){
            romViewLCurrPos = p;
            showROMView();
        }
    }
}

void hdnesPackEditormainForm::romViewEnter( wxMouseEvent& event ){
    if(coreData::cData && !event.LeftIsDown()){
        romViewClicked = false;
        showROMView();
    }
}


void hdnesPackEditormainForm::initGameObjs(){
    zoomGameObjs->SetValue(4);
    scrGameObjRawH->SetRange(1);
    scrGameObjRawH->SetThumbSize(1);
    scrGameObjRawV->SetRange(1);
    scrGameObjRawV->SetThumbSize(1);
    scrGameObjNewH->SetRange(1);
    scrGameObjNewH->SetThumbSize(1);
    scrGameObjNewV->SetRange(1);
    scrGameObjNewV->SetThumbSize(1);
    clearGameObj();
}

void hdnesPackEditormainForm::gameObjsROMChanged(){
    treeGameObjs->DeleteAllItems();
    gameObjNode* node = new gameObjNode();
    node->nodeType = GAME_OBJ_NODE_TYPE_ROOT;
    node->nodeName = "";
    tItmGameObjRoot = treeGameObjs->AddRoot(wxString("\\"), -1, -1, node);
    gameObjectTreeWillMove = false;
    clearGameObj();
}

void hdnesPackEditormainForm::gameObjTItemBeginEdit( wxTreeEvent& event ){
    wxTreeItemId tID = event.GetItem();
    gameObjNode* data = (gameObjNode*)(treeGameObjs->GetItemData(tID));
    switch(data->nodeType){
    case GAME_OBJ_NODE_TYPE_ROOT:
        event.Veto();
        break;
    case GAME_OBJ_NODE_TYPE_GROUP:
        treeGameObjs->SetItemText(tID, wxString(data->nodeName));
        break;
    }
}

void hdnesPackEditormainForm::gameObjTItemChangeName( wxTreeEvent& event ){
    wxTreeItemId tID = event.GetItem();
    gameObjNode* data = (gameObjNode*)(treeGameObjs->GetItemData(tID));
    switch(data->nodeType){
    case GAME_OBJ_NODE_TYPE_GROUP:
        if(!event.IsEditCancelled()){
            data->nodeName = event.GetLabel();
            event.Veto();
        }
        treeGameObjs->SetItemText(tID, wxString(data->nodeName + "\\"));
        break;
    case GAME_OBJ_NODE_TYPE_OBJECT:
        if(!event.IsEditCancelled()){
            data->nodeName = event.GetLabel();
        }
        break;
    }
    notSaved = true;
}

void hdnesPackEditormainForm::gameObjTItemOpenMenu( wxTreeEvent& event ){
    tItmGameObjMenu = event.GetItem();
    gameObjNode* data = (gameObjNode*)(treeGameObjs->GetItemData(tItmGameObjMenu));
    wxMenu menu(wxT(""));
    if(data->nodeType != GAME_OBJ_NODE_TYPE_OBJECT){
        menu.Append(GAME_OBJ_NODE_MENU_ADD_FOLDER, wxT("Add folder"));
        menu.Append(GAME_OBJ_NODE_MENU_ADD_OBJECT, wxT("Add object"));
        if(gameObjectTreeWillMove && !(data->effectedByMove)){
            menu.Append(GAME_OBJ_NODE_MENU_MOVE_HERE, wxT("Move here"));
        }
    }
    if(data->nodeType != GAME_OBJ_NODE_TYPE_ROOT){
        menu.Append(GAME_OBJ_NODE_MENU_DEL, wxT("Delete"));
        if(treeGameObjs->GetPrevSibling(tItmGameObjMenu).IsOk()){
            menu.Append(GAME_OBJ_NODE_MENU_MOVE_UP, wxT("Move up"));
        }
        if(treeGameObjs->GetNextSibling(tItmGameObjMenu).IsOk()){
            menu.Append(GAME_OBJ_NODE_MENU_MOVE_DOWN, wxT("Move down"));
        }
        menu.Append(GAME_OBJ_NODE_MENU_MOVE_TO_FOLDER, wxT("Move to folder"));
    }

    menu.Connect( wxEVT_MENU, wxCommandEventHandler(hdnesPackEditormainForm::gameObjsTreeMenu), NULL, this );
    treeGameObjs->PopupMenu(&menu, event.GetPoint());
}

void hdnesPackEditormainForm::gameObjTItemSelected( wxTreeEvent& event ){
    tItmGameObjMenu = event.GetItem();
    refreshGameObj();
}

void hdnesPackEditormainForm::gameObjsTreeMenu( wxCommandEvent& event ){
    gameObjNode* node;
    wxTreeItemId newItm;
    switch(event.GetId()){
    case GAME_OBJ_NODE_MENU_ADD_FOLDER:
        node = new gameObjNode();
        node->nodeType = GAME_OBJ_NODE_TYPE_GROUP;
        node->nodeName = "Folder";
        newItm = treeGameObjs->AppendItem(tItmGameObjMenu, wxString("Folder\\"), -1, -1, node);
        treeGameObjs->Expand(tItmGameObjMenu);
        treeGameObjs->EditLabel(newItm);
        notSaved = true;
        break;
    case GAME_OBJ_NODE_MENU_ADD_OBJECT:
        node = new gameObjNode();
        node->nodeType = GAME_OBJ_NODE_TYPE_OBJECT;
        node->nodeName = "Object";
        newItm = treeGameObjs->AppendItem(tItmGameObjMenu, wxString("Object"), -1, -1, node);
        treeGameObjs->Expand(tItmGameObjMenu);
        treeGameObjs->EditLabel(newItm);
        treeGameObjs->SetFocusedItem(newItm);
        tItmGameObjMenu = newItm;
        refreshGameObj();
        notSaved = true;
        break;
    case GAME_OBJ_NODE_MENU_DEL:
        treeGameObjs->Delete(tItmGameObjMenu);
        notSaved = true;
        break;
    case GAME_OBJ_NODE_MENU_MOVE_UP:
        gameObjectTreeWillMove = false;
        gameObjsMoveTreeItem(tItmGameObjMenu, treeGameObjs->GetItemParent(tItmGameObjMenu), treeGameObjs->GetPrevSibling(treeGameObjs->GetPrevSibling(tItmGameObjMenu)));
        notSaved = true;
        break;
    case GAME_OBJ_NODE_MENU_MOVE_DOWN:
        gameObjectTreeWillMove = false;
        gameObjsMoveTreeItem(tItmGameObjMenu, treeGameObjs->GetItemParent(tItmGameObjMenu), treeGameObjs->GetNextSibling(tItmGameObjMenu));
        notSaved = true;
        break;
    case GAME_OBJ_NODE_MENU_MOVE_TO_FOLDER:
        gameObjectTreeWillMove = true;
        gameObjsCancelWillMove(tItmGameObjRoot);
        gameObjsSetWillMove(tItmGameObjMenu);
        tItmGameObjMove = tItmGameObjMenu;
        notSaved = true;
        break;
    case GAME_OBJ_NODE_MENU_MOVE_HERE:
        gameObjectTreeWillMove = false;
        gameObjsMoveTreeItem(tItmGameObjMove, tItmGameObjMenu, treeGameObjs->GetLastChild(tItmGameObjMenu));
        gameObjsCancelWillMove(tItmGameObjRoot);
        notSaved = true;
        break;
    }
}

void hdnesPackEditormainForm::gameObjsMoveTreeItem(wxTreeItemId item, wxTreeItemId newParent, wxTreeItemId previousItem){
    gameObjNode* node = (gameObjNode*)(treeGameObjs->GetItemData(item));
    treeGameObjs->SetItemData(item, NULL);
    wxTreeItemId newItm = treeGameObjs->InsertItem(newParent, previousItem, treeGameObjs->GetItemText(item), -1, -1, node);

    wxTreeItemIdValue cookie = 0;
    wxTreeItemId child = treeGameObjs->GetFirstChild(item, cookie);
    while(child.IsOk()){
        gameObjsMoveTreeItem(child, newItm, treeGameObjs->GetLastChild(newItm));
        child = treeGameObjs->GetFirstChild(item, cookie);
    }
    treeGameObjs->Delete(item);
}

void hdnesPackEditormainForm::gameObjsCancelWillMove(wxTreeItemId item){
    gameObjNode* node = (gameObjNode*)(treeGameObjs->GetItemData(item));
    node->effectedByMove = false;
    wxTreeItemIdValue cookie = 0;
    wxTreeItemId child = treeGameObjs->GetFirstChild(item, cookie);
    while(child.IsOk()){
        gameObjsCancelWillMove(child);
        child = treeGameObjs->GetNextSibling(child);
    }
}

void hdnesPackEditormainForm::gameObjsSetWillMove(wxTreeItemId item){
    gameObjNode* node = (gameObjNode*)(treeGameObjs->GetItemData(item));
    node->effectedByMove = true;
    wxTreeItemIdValue cookie = 0;
    wxTreeItemId child = treeGameObjs->GetFirstChild(item, cookie);
    while(child.IsOk()){
        gameObjsSetWillMove(child);
        child = treeGameObjs->GetNextSibling(child);
    }
}

void hdnesPackEditormainForm::gameObjsRawRUp( wxMouseEvent& event ){
    if(coreData::cData){
        wxPoint p = event.GetPosition();
        wxMenu menu(wxT(""));

        //show paste when context valid and object node selected
        wxTreeItemId tID = treeGameObjs->GetFocusedItem();
        if(tID.IsOk()){
            gameObjNode* data = (gameObjNode*)(treeGameObjs->GetItemData(tID));
            if(data->nodeType == GAME_OBJ_NODE_TYPE_OBJECT){
                if (wxTheClipboard->IsSupported( wxDF_TEXT )){
                    wxTextDataObject data;
                    wxTheClipboard->GetData( data );
                    if(checkPasteValid(data.GetText().ToStdString())){
                        menu.Append(GAME_OBJ_PNL_PASTE, wxT("Paste"));
                    }
                }
            }
        }

        menu.Connect( wxEVT_MENU, wxCommandEventHandler(hdnesPackEditormainForm::gameObjsRawMenu), NULL, this );
        pnlGameObjRaw->PopupMenu(&menu, p);
    }
}

void hdnesPackEditormainForm::gameObjsRawMenu( wxCommandEvent& event ){
    switch(event.GetId()){
    case GAME_OBJ_PNL_PASTE:
        gameObjNode* ndata = (gameObjNode*)(treeGameObjs->GetItemData(tItmGameObjMenu));
        if (wxTheClipboard->IsSupported( wxDF_TEXT )){
            wxTextDataObject data;
            wxTheClipboard->GetData( data );
            //read tile string into tiles and add to the current object
            vector<string> tileLines;
            vector<string> tileDetails;
            gameTile g;

            gameObjPasteData.clearAllTiles();
            main::split(data.GetText().ToStdString(), ';', tileLines);
            for(int i = 0; i < tileLines.size(); ++i){
                main::split(tileLines[i], ',', tileDetails);
                if(tileDetails.size() == 4){
                    if(coreData::cData->isCHRROM){
                        g.id = atoi(tileDetails[0].c_str());
                    }
                    else{
                        main::hexToByteArray(tileDetails[0], g.rawData);
                    }
                    main::hexToByteArray(tileDetails[1], g.palette);
                    g.objCoordX = atoi(tileDetails[2].c_str());
                    g.objCoordY = atoi(tileDetails[3].c_str());
                    gameObjPasteData.addTile(g);
                }
            }
            drawGameObjPasteTiles();
        }
        break;
    }
}

bool hdnesPackEditormainForm::checkPasteValid(string content){
    vector<string> tileLines;
    main::split(content, ';', tileLines);
    if(tileLines.size() > 0){
        bool allValid = true;
        vector<string> tileDetails;
        for(int i = 0; i < tileLines.size(); ++i){
            main::split(tileLines[i], ',', tileDetails);
            if(tileDetails.size() != 4){
                allValid = false;
            }
        }
        return allValid;
    }
    return false;
}

void hdnesPackEditormainForm::refreshGameObj(){
    clearGameObj();
    gameObjNode* ndata = (gameObjNode*)(treeGameObjs->GetItemData(tItmGameObjMenu));
    if(ndata->nodeType != GAME_OBJ_NODE_TYPE_OBJECT) return;

    //refresh bg colour button
    btnGameObjBGColour->SetBackgroundColour(coreData::cData->palette[ndata->bgColour]);
    if(coreData::cData->palette[ndata->bgColour].Red() + coreData::cData->palette[ndata->bgColour].Green() + coreData::cData->palette[ndata->bgColour].Blue() > 256){
        gameObjBlankColour = wxColour(0,0,0);
    }
    else{
        gameObjBlankColour = wxColour(255,255,255);
    }
    btnGameObjBGColour->SetForegroundColour(gameObjBlankColour);
    wxString v;
    v = wxString(main::intToHex(ndata->bgColour).c_str());
    btnGameObjBGColour->SetLabel(v);

    if(ndata->tiles.size() == 0) return;

    gameObjTileSize = 8 * zoomGameObjs->GetValue();
    scrGameObjRawH->SetRange(ndata->objectWidth * zoomGameObjs->GetValue());
    scrGameObjRawV->SetRange(ndata->objectHeight * zoomGameObjs->GetValue());
    scrGameObjRawH->SetThumbSize(pnlGameObjRaw->GetSize().GetWidth());
    scrGameObjRawV->SetThumbSize(pnlGameObjRaw->GetSize().GetHeight());

    gameObjRawImageDisplay = wxImage(pnlGameObjRaw->GetSize().x, pnlGameObjRaw->GetSize().y);
    drawGameObj();
}

void hdnesPackEditormainForm::clearGameObj(){
    gameObjViewCentreX = 0;
    gameObjViewCentreY = 0;
    wxClientDC* objDC;
    objDC = new wxClientDC(pnlGameObjRaw);
    objDC->SetBackground(*wxBLACK_BRUSH);
    objDC->Clear();
    delete objDC;
}

void hdnesPackEditormainForm::drawGameObj(){
    gameObjNode* ndata = (gameObjNode*)(treeGameObjs->GetItemData(tItmGameObjMenu));
    gameObjRawImage = wxImage(ndata->objectWidth, ndata->objectHeight, true);
    //clear image with blank colour
    gameObjRawImage.SetRGB(wxRect(0, 0, ndata->objectWidth, ndata->objectHeight), gameObjBlankColour.Red(), gameObjBlankColour.Green(), gameObjBlankColour.Blue());

    Uint32 memAddress;
    Uint16 drawX;
    Uint16 drawY;
    for(int i = 0; i < ndata->tiles.size(); ++i){
        drawX = ndata->tiles[i].objCoordX - ndata->x1;
        drawY = ndata->tiles[i].objCoordY - ndata->y1;
        if(coreData::cData->isCHRROM){
            memAddress = ndata->tiles[i].id * 16;
            paintTile(gameObjRawImage, coreData::cData->romData + memAddress, drawX, drawY,
                    coreData::cData->palette[ndata->bgColour],
                    coreData::cData->palette[ndata->tiles[i].palette[1]],
                    coreData::cData->palette[ndata->tiles[i].palette[2]],
                    coreData::cData->palette[ndata->tiles[i].palette[3]]);
        }
        else{
            paintTile(gameObjRawImage, ndata->tiles[i].rawData, drawX, drawY,
                    coreData::cData->palette[ndata->bgColour],
                    coreData::cData->palette[ndata->tiles[i].palette[1]],
                    coreData::cData->palette[ndata->tiles[i].palette[2]],
                    coreData::cData->palette[ndata->tiles[i].palette[3]]);
        }
    }
    if(gameObjPasteData.tiles.size() == 0){
        drawGameObjSelection();
    }
    else{
        drawGameObjPasteTiles();
    }
}

void hdnesPackEditormainForm::drawGameObjPasteTiles(){
    //cal image size
    int mouseX = gameObjRawCurrPos.x / zoomGameObjs->GetValue();

    //create image
    gameObjRawImage2 = wxImage(gameObjPasteData.objectWidth, gameObjPasteData.objectHeight, true);
    //clear image with blank colour
    gameObjRawImage2.SetRGB(wxRect(0, 0, gameObjPasteData.objectWidth, gameObjPasteData.objectHeight), gameObjBlankColour.Red(), gameObjBlankColour.Green(), gameObjBlankColour.Blue());

    //copy raw image here

    //draw paste tiles
    Uint32 memAddress;
    Uint16 drawX;
    Uint16 drawY;
    for(int i = 0; i < ndata->tiles.size(); ++i){
        drawX = ndata->tiles[i].objCoordX - ndata->x1;
        drawY = ndata->tiles[i].objCoordY - ndata->y1;
        if(coreData::cData->isCHRROM){
            memAddress = ndata->tiles[i].id * 16;
            paintTile(gameObjRawImage, coreData::cData->romData + memAddress, drawX, drawY,
                    coreData::cData->palette[ndata->bgColour],
                    coreData::cData->palette[ndata->tiles[i].palette[1]],
                    coreData::cData->palette[ndata->tiles[i].palette[2]],
                    coreData::cData->palette[ndata->tiles[i].palette[3]]);
        }
        else{
            paintTile(gameObjRawImage, ndata->tiles[i].rawData, drawX, drawY,
                    coreData::cData->palette[ndata->bgColour],
                    coreData::cData->palette[ndata->tiles[i].palette[1]],
                    coreData::cData->palette[ndata->tiles[i].palette[2]],
                    coreData::cData->palette[ndata->tiles[i].palette[3]]);
        }
    }


    showGameObj();
}


void hdnesPackEditormainForm::drawGameObjSelection(){
    showGameObj();
}

void hdnesPackEditormainForm::showGameObj(){
    wxBitmap bmp = wxBitmap(gameObjRawImage);
	if(bmp.IsOk()){
		wxClientDC* objDC;
		objDC = new wxClientDC(pnlGameObjRaw);
		objDC->DrawBitmap(bmp, 0, 0);
		delete objDC;
	}
}

void hdnesPackEditormainForm::gameObjBGColour( wxCommandEvent& event ){
    if(coreData::cData){
        wxTreeItemId tID = treeGameObjs->GetFocusedItem();
        if(tID.IsOk()){
            gameObjNode* data = (gameObjNode*)(treeGameObjs->GetItemData(tID));
            if(data->nodeType == GAME_OBJ_NODE_TYPE_OBJECT){
                openColourDialog(COLOUR_CLIENT_GAME_OBJ_BG);
            }
        }
    }
}

void hdnesPackEditormainForm::gameObjsRawMove( wxMouseEvent& event ){
    if(coreData::cData){
        gameObjRawCurrPos = event.GetPosition();
    }
}

void hdnesPackEditormainForm::gameObjsRawEnter( wxMouseEvent& event ){
}


void hdnesPackEditormainForm::configGameObjs(string lineHdr, string lineTail){
}

void hdnesPackEditormainForm::saveCfgGameObjs(fstream& inifile){
}

void hdnesPackEditormainForm::loadGameObjs(fstream& file){
    string line;
    getline(file, line);
    while(line != "<endChildObjects>"){
        loadGameObjItem(file, treeGameObjs->GetRootItem());
        getline(file, line);
    }
}

void hdnesPackEditormainForm::loadGameObjItem(fstream& file, wxTreeItemId item){
    gameObjNode* node = new gameObjNode();
    wxTreeItemId newItm = treeGameObjs->InsertItem(item, treeGameObjs->GetLastChild(item), "", -1, -1, node);

    string line;
    string lineHdr;
    string lineTail;

    getline(file, line);
    while(line != "<endGameObject>"){
        size_t found = line.find_first_of(">");
        if(found!=string::npos){
            lineHdr = line.substr(0, found + 1);
            lineTail = line.substr(found + 1);
            if(lineHdr == "<type>"){
                node->nodeType = atoi(lineTail.c_str());
            }
            if(lineHdr == "<name>"){
                node->nodeName = lineTail;
                if(node->nodeType == GAME_OBJ_NODE_TYPE_GROUP){
                    treeGameObjs->SetItemText(newItm, wxString(node->nodeName + "\\"));
                }
                else{
                    treeGameObjs->SetItemText(newItm, wxString(node->nodeName));
                }
            }
            if(lineHdr == "<childObjects>"){
                getline(file, line);
                while(line != "<endChildObjects>"){
                    loadGameObjItem(file, newItm);
                    getline(file, line);
                }
            }
        }
        getline(file, line);
    }
}

void hdnesPackEditormainForm::saveGameObjs(fstream& file){
    saveGameObjItem(file, tItmGameObjRoot);
}

void hdnesPackEditormainForm::saveGameObjItem(fstream& file, wxTreeItemId item){
    gameObjNode* node = (gameObjNode*)(treeGameObjs->GetItemData(item));
    if(node->nodeType != GAME_OBJ_NODE_TYPE_ROOT){
        file << "<gameObject>\n";
        file << "<type>" + main::intToStr(node->nodeType) + "\n";
        file << "<name>" + node->nodeName + "\n";
    }

    wxTreeItemIdValue cookie = 0;
    wxTreeItemId child = treeGameObjs->GetFirstChild(item, cookie);

    if(node->nodeType == GAME_OBJ_NODE_TYPE_OBJECT){
        file << "<tiles>\n";
        file << "<endTiles>\n";
    }
    else{
        file << "<childObjects>\n";
        cookie = 0;
        child = treeGameObjs->GetFirstChild(item, cookie);
        while(child.IsOk()){
            saveGameObjItem(file, child);
            child = treeGameObjs->GetNextSibling(child);
        }
        file << "<endChildObjects>\n";
    }

    if(node->nodeType != GAME_OBJ_NODE_TYPE_ROOT){
        file << "<endGameObject>\n";
    }
}
