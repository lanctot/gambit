//
// FILE: efgsolvd.cc -- Standard solution methods for extensive form
//
// $Id$
//

// Most of these probably aren't necessary... figure out which ones are!

#include "wx.h"
#include "wxmisc.h"
#include "efg.h"
#include "efgconst.h"
#include "treewin.h"
#include "efgshow.h"
#include "efgsoln.h"
#include "nfggui.h"
#include "efgnfgi.h"
#include "behavsol.h"

#include "efgsolvd.h"

//=========================================================================
//               EfgSolveSettings: Private member functions
//=========================================================================

void EfgSolveSettings::Warn(const char *p_warning) 
{
  wxMessageBox((char *) p_warning, "Standard Solution");
}

//=========================================================================
//              EfgSolveSettings: Constructor and destructor
//=========================================================================

EfgSolveSettings::EfgSolveSettings(const Efg &p_efg, bool p_solving /*= true*/)
  : solving(p_solving), ef(p_efg)
{
  result = SD_SAVE;
  defaults_file = "gambit.ini";
  wxGetResource(SOLN_SECT, "Use-Nfg", &use_nfg, defaults_file);
  char *alg_sect=(use_nfg) ? "Nfg-Algorithm" : "Efg-Algorithm";
  wxGetResource(SOLN_SECT,alg_sect,&algorithm,defaults_file);
  wxGetResource(SOLN_SECT,"Efg-Nfg",&normal,defaults_file);
  wxGetResource(SOLN_SECT,"Efg-Mark-Subgames",&subgames,defaults_file);
  wxGetResource(SOLN_SECT,"Efg-Interactive-Solns",&pick_solns,defaults_file);
}

EfgSolveSettings::~EfgSolveSettings()
{
  if (result != SD_CANCEL) {
    wxWriteResource(SOLN_SECT,"Use-Nfg",use_nfg,defaults_file);
    char *alg_sect=(use_nfg) ? "Nfg-Algorithm" : "Efg-Algorithm";
    wxWriteResource(SOLN_SECT,alg_sect,algorithm,defaults_file);
    wxWriteResource(SOLN_SECT,"Efg-Nfg",normal,defaults_file);
    wxWriteResource(SOLN_SECT,"Efg-Interactive-Solns",pick_solns,defaults_file);
  }
}

//========================================================================
//                dialogEfgSolveStandard: Member functions
//========================================================================

dialogEfgSolveStandard::dialogEfgSolveStandard(const Efg &p_efg,
					       wxWindow *p_parent)
  : wxDialogBox(p_parent, "Standard Solution", TRUE), m_efg(p_efg)
{
  gText defaultsFile("gambit.ini");
  int standardType = 0, standardNum = 0, precision = 0;
  wxGetResource(SOLN_SECT, "Efg-Standard-Type", &standardType, 
		defaultsFile);
  wxGetResource(SOLN_SECT, "Efg-Standard-Num", &standardNum,
		defaultsFile);
  wxGetResource(SOLN_SECT, "Efg-Standard-Precision", &precision, defaultsFile);

  char *typeChoices[] = { "Nash", "Subgame Perfect", "Sequential" };
  m_standardType = new wxRadioBox(this, (wxFunction) CallbackChanged,
				  "Type", -1, -1, -1, -1,
				  3, typeChoices, 0, wxVERTICAL);
  m_standardType->SetClientData((char *) this);
  m_standardType->SetSelection(standardType);

  char *numChoices[] = { "One", "Two", "All" };
  m_standardNum = new wxRadioBox(this, (wxFunction) CallbackChanged,
				 "Number", -1, -1, -1, -1,
				 3, numChoices, 0, wxVERTICAL);
  m_standardNum->SetClientData((char *) this);
  m_standardNum->SetSelection(standardNum);

  char *precisionChoices[] = { "Float", "Rational" };
  m_precision = new wxRadioBox(this, 0, "Precision", -1, -1, -1, -1,
			       2, precisionChoices, 0, wxVERTICAL);
  m_precision->SetSelection(precision);
  
  NewLine();
  m_description = new wxText(this, 0, "Using algorithm");
  m_description->Enable(FALSE);

  NewLine();
  wxButton *okButton = new wxButton(this, (wxFunction) CallbackOK, "OK");
  okButton->SetClientData((char *) this);
  okButton->SetDefault();
  wxButton *cancelButton = new wxButton(this, (wxFunction) CallbackCancel,
					"Cancel");
  cancelButton->SetClientData((char *) this);

  OnChanged();

  Fit();
  Show(TRUE);
}

dialogEfgSolveStandard::~dialogEfgSolveStandard()
{
  gText defaultsFile("gambit.ini");
  wxWriteResource(SOLN_SECT, "Efg-Standard-Type",
		  m_standardType->GetSelection(), defaultsFile);
  wxWriteResource(SOLN_SECT, "Efg-Standard-Num",
		  m_standardNum->GetSelection(), defaultsFile);
  wxWriteResource(SOLN_SECT, "Efg-Standard-Precision",
		  m_precision->GetSelection(), defaultsFile);
}

void dialogEfgSolveStandard::OnOK(void)
{
  m_completed = wxOK;
  Show(FALSE);
}

void dialogEfgSolveStandard::OnCancel(void)
{
  m_completed = wxCANCEL;
  Show(FALSE);
}

Bool dialogEfgSolveStandard::OnClose(void)
{
  m_completed = wxCANCEL;
  Show(FALSE);
  return FALSE;
}

void dialogEfgSolveStandard::OnChanged(void)
{
  switch (m_standardType->GetSelection()) {
  case 0:
  case 1:
    switch (m_standardNum->GetSelection()) {
    case 0:
      if (IsPerfectRecall(m_efg)) {
	if (m_efg.NumPlayers() == 2 && m_efg.IsConstSum()) {
	  m_description->SetValue("LpSolve[EFG]");
	  m_precision->Enable(TRUE);
	}
	else if (m_efg.NumPlayers() == 2) {
	  m_description->SetValue("LcpSolve[EFG]");
	  m_precision->Enable(TRUE);
	}
	else {
	  m_description->SetValue("SimpdivSolve[NFG]");
	  m_precision->Enable(TRUE);
	}
      }
      else {
	m_description->SetValue("QreSolve[EFG]");
	m_precision->Enable(FALSE);
      }
      break;
    case 1:
      if (m_efg.NumPlayers() == 2) {
	m_description->SetValue("EnumMixedSolve[NFG]");
	m_precision->Enable(TRUE);
      }
      else {
	m_description->SetValue("LiapSolve[EFG]");
	m_precision->Enable(FALSE);
      }
      break;
    case 2:
      m_description->SetValue("LiapSolve[EFG]");
      m_precision->Enable(FALSE);
      break;
    }
    break;
  case 2:
    m_precision->Enable(FALSE);
    switch (m_standardNum->GetSelection()) {
    case 0:
      m_description->SetValue("QreSolve[EFG]");
      break;
    case 1:
    case 2:
      m_description->SetValue("LiapSolve[EFG]");
      break;
    }
  }
}

guiStandardType dialogEfgSolveStandard::Type(void) const
{
  switch (m_standardType->GetSelection()) {
  case 0:
    return efgSTANDARD_NASH;
  case 1:
    return efgSTANDARD_PERFECT;
  case 2:
    return efgSTANDARD_SEQUENTIAL;
  default:
    return efgSTANDARD_NASH;
  }
}

guiStandardNum dialogEfgSolveStandard::Number(void) const
{
  switch (m_standardNum->GetSelection()) {
  case 0:
    return efgSTANDARD_ONE;
  case 1:
    return efgSTANDARD_TWO;
  case 2:
    return efgSTANDARD_ALL;
  default:
    return efgSTANDARD_ALL;
  }
}

