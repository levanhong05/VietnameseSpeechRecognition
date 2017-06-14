NAME "Speech Recognition"
!define PRODUCT "SpeechRecognition"
!define VERSION "1.1.0"
!define PRODUCT_NAME "Speech Recognition"

!define URLInfoAbout "https://github.com/levanhong05/VietnameseSpeechRecognition"
!define YourName "Eric Lee"

!include "MUI.nsh"
!include "FileFunc.nsh"

!define MUI_ICON chat.ico
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP chat.bmp
!define MUI_HEADERIMAGE_RIGHT

CRCCheck On
SetCompressor lzma

OutFile "SpeechRecognition_v${VERSION}.exe"
BRANDINGTEXT "Speech Recognition Installer created by Eric Lee"

InstallDir "$PROGRAMFILES\${PRODUCT}"
InstallDirRegKey HKCU "Software\${PRODUCT}" ""

;!insertmacro MUI_PAGE_LICENSE "license.txt"
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
# These indented statements modify settings for MUI_PAGE_FINISH
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_CHECKED
!define MUI_FINISHPAGE_RUN_TEXT "Start program"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!define MUI_ABORTWARNING

!define MUI_HEADERBITMAP "${NSISDIR}\Contrib\Icons\modern-header.bmp"
!define MUI_SPECIALBITMAP "${NSISDIR}\Contrib\Icons\modern-wizard.bmp"

!insertmacro MUI_LANGUAGE "english"

Function .onInit
  SetShellVarContext all

  ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString"
  StrCmp $R0 "" done
 
  ClearErrors
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
 
  IfErrors no_remove_uninstaller done
    ;You can either use Delete /REBOOTOK in the uninstaller or add some code
    ;here to remove the uninstaller. Use a registry key to check
    ;whether the user has chosen to uninstall. If you are using an uninstaller
    ;components page, make sure all sections are uninstalled.
  no_remove_uninstaller:
 
done:
 
FunctionEnd

Section "section_1" section_1
  SetOutPath "$INSTDIR"
  FILE /r "file\*"
SectionEnd

Section Icons
  # Insert here your Icons
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${PRODUCT}"
  CreateShortcut "$SMPROGRAMS\${PRODUCT}\SpeechRecognition.lnk" "$PROGRAMFILES\${PRODUCT}\SpeechRecognition.exe"
  CreateShortcut "$DESKTOP\SpeechRecognition.lnk" "$PROGRAMFILES\${PRODUCT}\SpeechRecognition.exe"
SectionEnd

Function LaunchLink
  ExecShell "" "$PROGRAMFILES\${PRODUCT}\SpeechRecognition.exe"
FunctionEnd

Section Install
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\Uninstall.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT_NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "URLInfoAbout" "${URLInfoAbout}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "Publisher" "${YourName}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" "$INSTDIR\Uninst.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayIcon" "$\"$INSTDIR\icon.ico$\""
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoModify" 1
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoRepair" 1

  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
  IntFmt $0 "0x%08X" $0
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "EstimatedSize" "$0"

  WriteRegStr HKCU "Software\${PRODUCT}" "" $INSTDIR
  WriteUninstaller "$INSTDIR\Uninst.exe"
SectionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "${PRODUCT_NAME} was successfully removed from your computer.."
FunctionEnd
  
Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove ${PRODUCT_NAME} and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  SetShellVarContext all
  Delete "$INSTDIR\*.*"
 
  # second, remove the link from the start menu
  Delete "$SMPROGRAMS\${PRODUCT}\*.*"
  RmDir "$SMPROGRAMS\${PRODUCT}"
  
  Delete "$DESKTOP\SpeechRecognition.lnk"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${PRODUCT}"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"
  RMDir /r "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\${PRODUCT}"
SectionEnd