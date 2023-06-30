﻿#include "stdafx.h"

namespace local {
 ////////////////////////////////////////////////////////////////////////////////////////////////
 TreeList::TreeList(wxWindow* parent,
  wxWindowID id,
  const wxPoint& pos,
  const wxSize& size,
  long style,
  const wxString& name) :
  wxTreeListCtrl(parent, id, pos, size, style, name) {
  Init();
 }

 TreeList::~TreeList() {
  UnInit();
 }

 void TreeList::Init() {

 }

 void TreeList::UnInit() {

 }
 void TreeList::Show(const bool& show) {
  wxTreeListCtrl::Show(show);
 }
 void TreeList::Size(const ISize* size) {
  if (size)
   wxTreeListCtrl::SetSize(wxSize(size->width(), size->height()));
 }
 void TreeList::Pos(const IPos* pos) {
  if (pos)
   wxTreeListCtrl::SetPosition(wxPoint(pos->x(), pos->y()));
 }
 void TreeList::Align(const IAlign* align) {
  if (align)
   if (align->type() == AlignType::CENTER)
    wxTreeListCtrl::Center();
 }
 void* TreeList::Handle() const {
  return reinterpret_cast<void*>(const_cast<wxWindow*>(dynamic_cast<const wxWindow*>(this)));
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////////////////////////////////////////////////////////////////////////////
}///namespace local