#include "stdafx.h"

namespace local {

 ICtrl::ICtrl(skin::INodeRender* node) : node_(node) {

 }
 ICtrl::~ICtrl() {

 }
 skin::INodeRender* ICtrl::Node() const {
  return node_;
 }
 void ICtrl::OnLayout() const {
  for (size_t i = 0; i < node_->TotalChild(); ++i) {
   auto child = node_->NextChild(i);
   if (!child)
    continue;

   auto sk = 0;
  }
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 Window::Window(skin::INodeRender* node) : ICtrl(node) {

 }
 Window::~Window() {

 }
 void Window::Release() const {
  delete this;
 }
 void Window::Begin() {

 }
 void Window::End() {

 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 Font::Font(skin::INodeRender* node) : ICtrl(node) {

 }
 Font::~Font() {

 }
 void Font::Release() const {
  delete this;
 }
 void Font::Begin() {

 }
 void Font::End() {
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 Frame::Frame(skin::INodeRender* node) : ICtrl(node) {

 }
 Frame::~Frame() {

 }
 void Frame::Release() const {
  delete this;
 }
 void Frame::Begin() {
  do {

   ImGuiViewport* MainViewport =  ImGui::GetMainViewport();
   ImVec2 MainClientSize = MainViewport->Size;
   // 设置Frame的大小为主窗口的大小
   //ImGui::SetNextWindowSize(mainWindowSize);

   if (!ImGui::Begin((const char*)node_->Name()->u8(),
    &flag_open_/*,
    ImGuiWindowFlags_NoTitleBar*/
    /*| ImGuiWindowFlags_NoResize*/
    /*| ImGuiWindowFlags_NoMove*/
    /*| ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoNavFocus
    | ImGuiWindowFlags_MenuBar
    | ImGuiWindowFlags_NoBackground*/
   ))
    break;

   if (node_->Floating()) {
    ImGui::SetWindowPos(ImVec2(node_->Pos()->x, node_->Pos()->y));
    ImGui::SetWindowSize(ImVec2(node_->Size()->cx, node_->Size()->cy));
   }
   else {
    ImGui::SetWindowPos(ImVec2(0, 0));
    if (node_->Size()->cx > 0 && node_->Size()->cy > 0)
     ImGui::SetWindowSize(ImVec2(node_->Size()->cx, node_->Size()->cy));
    else
     ImGui::SetWindowSize(MainClientSize);
   }






   //OnLayout();

  } while (0);

 }
 void Frame::End() {
  do {
   ImGui::End();
  } while (0);
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 VerticalLayout::VerticalLayout(skin::INodeRender* node) : ICtrl(node) {

 }
 VerticalLayout::~VerticalLayout() {

 }
 void VerticalLayout::Release() const {
  delete this;
 }
 void VerticalLayout::Begin() {
  ImGui::BeginChildFrame(node_->Identify(), ImVec2(100, 100));
 }
 void VerticalLayout::End() {
  ImGui::EndChildFrame();
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 HorizontalLayout::HorizontalLayout(skin::INodeRender* node) : ICtrl(node) {

 }
 HorizontalLayout::~HorizontalLayout() {

 }
 void HorizontalLayout::Release() const {
  delete this;
 }
 void HorizontalLayout::Begin() {
  ImGui::BeginChildFrame(node_->Identify(), ImVec2(50, 50));
 }
 void HorizontalLayout::End() {
  ImGui::EndChildFrame();
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////////////

}///namespace local


