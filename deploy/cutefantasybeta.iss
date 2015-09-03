; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Trading Football"
#define MyAppVersion "Beta"
#define MyAppPublisher "Satoshi Fantasy LLC"
#define MyAppURL "http://trading.football"
#define MyAppExeName "tradingfootball.exe"


[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{5B1C2D8F-A4FE-46B8-A914-23C1A6F1872D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\tradingfootball
DefaultGroupName={#MyAppName}
OutputDir=..\installer
OutputBaseFilename=tradingfootball-beta-setup
Compression=lzma
SolidCompression=yes
WizardImageFile=TRADINGFOOTBALL.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "tradingfootball.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libEGLd.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "msvcp120d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "msvcr120d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libGLESV2d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Cored.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Guid.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Networkd.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Svgd.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Widgetsd.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bearer\qgenericbearerd.dll"; DestDir: "{app}\bearer"; Flags: ignoreversion
Source: "bearer\qnativewifibearerd.dll"; DestDir: "{app}\bearer"; Flags: ignoreversion
Source: "iconengines\qsvgicond.dll"; DestDir: "{app}\iconengines"; Flags: ignoreversion
Source: "imageformats\qddsd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qgifd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qicnsd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qicod.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qjp2d.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qjpegd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qmngd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qsvgd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qtgad.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qtiffd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qwbmpd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qwebpd.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "platforms\qwindowsd.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "storage\genesisAlpha.out"; DestDir: "{app}\storage"; Flags: ignoreversion
Source: "translations\qt_ca.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_cs.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_de.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_fi.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_fr.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_hu.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_it.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_ja.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_ko.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_lv.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_ru.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_sk.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "translations\qt_uk.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "vcredist_x64.exe"; DestDir: "{tmp}"; Flags: ignoreversion
Source: "libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: sqldrivers\qsqlmysqld.dll; DestDir: {app}\sqldrivers; Flags: ignoreversion; 

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{tmp}\vcredist_x64.exe"; Parameters: "/q"; StatusMsg: "Installing Microsoft Visual C++ 2010 x64 Redistributable..."
Filename: {app}\{#MyAppExeName}; Flags: nowait postinstall skipifsilent 64bit RunAsOriginalUser; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; 

[Dirs]
Name: "{app}\bearer"
Name: "{app}\iconengines"
Name: "{app}\imageformats"
Name: "{app}\platforms"
Name: "{app}\storage"
Name: "{app}\translations"
Name: "{app}\sqldrivers"; 
Name: "{app}\translations";
