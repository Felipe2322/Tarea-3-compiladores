#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/grid.h>
#include <vector>
#include <string>

// Token structure
struct Token {
    std::string type;
    std::string lexeme;
    int line;
    int column;
};

extern "C" {
    extern FILE* yyin;
    int yylex();
    extern char* yytext;
    extern int yylineno;
}

std::vector<Token> tokens;

// Forward declaration
int yywrap() { return 1; }

class LexerFrame : public wxFrame {
public:
    LexerFrame(const wxString& title);
    
private:
    wxTextCtrl* codeEditor;
    wxGrid* tokenGrid;
    wxButton* analyzeButton;
    wxButton* clearButton;
    
    void OnAnalyze(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void UpdateTokenGrid();
    
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(LexerFrame, wxFrame)
    EVT_BUTTON(wxID_OK, LexerFrame::OnAnalyze)
    EVT_BUTTON(wxID_CLEAR, LexerFrame::OnClear)
wxEND_EVENT_TABLE()

LexerFrame::LexerFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 700)) {
    
    // Create main panel
    wxPanel* mainPanel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Title
    wxStaticText* titleText = new wxStaticText(mainPanel, wxID_ANY, 
        wxT("Analizador Léxico - FLEX + C++"));
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(14);
    titleFont.MakeBold();
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL, 10);
    
    // Splitter window for editor and results
    wxSplitterWindow* splitter = new wxSplitterWindow(mainPanel);
    
    // Left panel - Code Editor
    wxPanel* leftPanel = new wxPanel(splitter);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticText* editorLabel = new wxStaticText(leftPanel, wxID_ANY, wxT("Código:"));
    wxFont labelFont = editorLabel->GetFont();
    labelFont.MakeBold();
    editorLabel->SetFont(labelFont);
    leftSizer->Add(editorLabel, 0, wxALL, 5);
    
    codeEditor = new wxTextCtrl(leftPanel, wxID_ANY, wxT(""),
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_WORDWRAP);
    wxFont editorFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    codeEditor->SetFont(editorFont);
    leftSizer->Add(codeEditor, 1, wxEXPAND | wxALL, 5);
    
    leftPanel->SetSizer(leftSizer);
    
    // Right panel - Token Grid
    wxPanel* rightPanel = new wxPanel(splitter);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticText* gridLabel = new wxStaticText(rightPanel, wxID_ANY, wxT("Tokens:"));
    gridLabel->SetFont(labelFont);
    rightSizer->Add(gridLabel, 0, wxALL, 5);
    
    tokenGrid = new wxGrid(rightPanel, wxID_ANY);
    tokenGrid->CreateGrid(0, 5);
    tokenGrid->SetColLabelValue(0, wxT("ID"));
    tokenGrid->SetColLabelValue(1, wxT("Tipo"));
    tokenGrid->SetColLabelValue(2, wxT("Lexema"));
    tokenGrid->SetColLabelValue(3, wxT("Línea"));
    tokenGrid->SetColLabelValue(4, wxT("Columna"));
    
    tokenGrid->SetColSize(0, 40);
    tokenGrid->SetColSize(1, 100);
    tokenGrid->SetColSize(2, 120);
    tokenGrid->SetColSize(3, 60);
    tokenGrid->SetColSize(4, 70);
    
    tokenGrid->EnableEditing(false);
    rightSizer->Add(tokenGrid, 1, wxEXPAND | wxALL, 5);
    
    rightPanel->SetSizer(rightSizer);
    
    // Split the window
    splitter->SplitVertically(leftPanel, rightPanel);
    splitter->SetSashGravity(0.5);
    mainSizer->Add(splitter, 1, wxEXPAND | wxALL, 5);
    
    // Button panel
    wxPanel* buttonPanel = new wxPanel(mainPanel);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    
    analyzeButton = new wxButton(buttonPanel, wxID_OK, wxT("Analizar"));
    clearButton = new wxButton(buttonPanel, wxID_CLEAR, wxT("Limpiar"));
    
    buttonSizer->Add(analyzeButton, 0, wxALL, 5);
    buttonSizer->Add(clearButton, 0, wxALL, 5);
    
    buttonPanel->SetSizer(buttonSizer);
    mainSizer->Add(buttonPanel, 0, wxALL | wxCENTER, 5);
    
    mainPanel->SetSizer(mainSizer);
    
    Centre();
}

void LexerFrame::OnAnalyze(wxCommandEvent& event) {
    tokens.clear();
    tokenGrid->DeleteRows(0, tokenGrid->GetNumberRows());
    
    wxString code = codeEditor->GetValue();
    
    // Create temporary file with code
    FILE* fp = fopen("temp_input.txt", "w");
    if (fp) {
        fprintf(fp, "%s", code.c_str().AsChar());
        fclose(fp);
        
        // Run lexer
        fp = fopen("temp_input.txt", "r");
        if (fp) {
            yyin = fp;
            yylineno = 1;
            
            while (yylex() != 0) {
                // Tokens collected in global vector
            }
            fclose(fp);
        }
    }
    
    // Update grid with tokens
    UpdateTokenGrid();
    
    // Remove temp file
    remove("temp_input.txt");
}

void LexerFrame::OnClear(wxCommandEvent& event) {
    codeEditor->Clear();
    tokens.clear();
    tokenGrid->DeleteRows(0, tokenGrid->GetNumberRows());
}

void LexerFrame::UpdateTokenGrid() {
    if (!tokens.empty()) {
        tokenGrid->AppendRows(tokens.size());
    }
    
    for (size_t i = 0; i < tokens.size(); i++) {
        tokenGrid->SetCellValue(i, 0, wxString::Format(wxT("%zu"), i + 1));
        tokenGrid->SetCellValue(i, 1, wxString(tokens[i].type.c_str(), wxConvUTF8));
        tokenGrid->SetCellValue(i, 2, wxString(tokens[i].lexeme.c_str(), wxConvUTF8));
        tokenGrid->SetCellValue(i, 3, wxString::Format(wxT("%d"), tokens[i].line));
        tokenGrid->SetCellValue(i, 4, wxString::Format(wxT("%d"), tokens[i].column));
        
        // Color alternating rows
        if (i % 2 == 0) {
            tokenGrid->SetCellBackgroundColour(i, 0, *wxLIGHT_GREY);
            tokenGrid->SetCellBackgroundColour(i, 1, *wxLIGHT_GREY);
            tokenGrid->SetCellBackgroundColour(i, 2, *wxLIGHT_GREY);
            tokenGrid->SetCellBackgroundColour(i, 3, *wxLIGHT_GREY);
            tokenGrid->SetCellBackgroundColour(i, 4, *wxLIGHT_GREY);
        }
    }
}

class LexerApp : public wxApp {
public:
    bool OnInit() override {
        LexerFrame* frame = new LexerFrame(wxT("Analizador Léxico (FLEX + C++)"));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(LexerApp);
