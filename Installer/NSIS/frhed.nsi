; NSIS script for Frhed installer.
; Script generated by the HM NIS Edit Script Wizard.
; $Id$

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Frhed"
!define PRODUCT_VERSION "1.3.1"
!define PRODUCT_PUBLISHER "Raihan Kibria"
!define PRODUCT_WEB_SITE "http://frhed.sourceforge.net"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\frhed.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "GPL.txt"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\frhed.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\..\Build\${PRODUCT_NAME}-${PRODUCT_VERSION}-Setup.exe"
InstallDir "$PROGRAMFILES\Frhed"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

; Prevent running two instances of the installer
Function .onInit
 System::Call 'kernel32::CreateMutexA(i 0, i 0, t "FrhedInstaller") i .r1 ?e'
 Pop $R0

 StrCmp $R0 0 +3
   MessageBox MB_OK|MB_ICONEXCLAMATION "The installer is already running."
   Abort
FunctionEnd

Section "ProgramFiles" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\..\Build\Release\frhed.exe"
  File "..\..\Build\Release\heksedit.dll"
  CreateDirectory "$SMPROGRAMS\Frhed"
  CreateShortCut "$SMPROGRAMS\Frhed\Frhed.lnk" "$INSTDIR\frhed.exe"
  CreateShortCut "$DESKTOP\Frhed.lnk" "$INSTDIR\frhed.exe"
  File "..\..\Build\Release\RAWIO32.dll"

  ; MS C/C++ runtime files
  File "..\Runtimes\msvcr71.dll"
  File "..\Runtimes\msvcp71.dll"

  ; Basic documents (always included)
  CreateDirectory "$INSTDIR\Docs"
  SetOutPath "$INSTDIR\Docs"
  File "GPL.txt"
  File "..\..\Docs\Users\ChangeLog.txt"
  File "..\..\Docs\Users\Contributors.txt"
  ; ChangeLog.txt refers to this file
  File "..\..\Docs\Users\History.txt"
SectionEnd

Section "Documentation" SEC02
  SetOutPath "$INSTDIR\Docs"
  File "..\..\Build\Manual\htmlhelp\frhed.chm"
  CreateShortCut "$SMPROGRAMS\Frhed\Help.lnk" "$INSTDIR\frhed.chm"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Frhed\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Frhed\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\frhed.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\frhed.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Main executable and required dlls."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Frhed documentation."
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\frhed.chm"
  Delete "$INSTDIR\RAWIO32.dll"
  Delete "$INSTDIR\frhed.exe"
  Delete "$INSTDIR\heksedit.dll"
  
  ; MS runtimes
  Delete "$INSTDIR\msvcr71.dll"
  Delete "$INSTDIR\msvcp71.dll"

  ; Basic documents
  Delete "$INSTDIR\Docs\GPL.txt"
  Delete "$INSTDIR\Docs\ChangeLog.txt"
  Delete "$INSTDIR\Docs\Contributors.txt"
  Delete "$INSTDIR\Docs\History.txt"
  RMDir "$INSTDIR\Docs"

  Delete "$SMPROGRAMS\Frhed\Uninstall.lnk"
  Delete "$SMPROGRAMS\Frhed\Website.lnk"
  Delete "$SMPROGRAMS\Frhed\Help.lnk"
  Delete "$DESKTOP\Frhed.lnk"
  Delete "$SMPROGRAMS\Frhed\Frhed.lnk"

  RMDir "$SMPROGRAMS\Frhed"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd