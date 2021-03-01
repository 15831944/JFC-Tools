Option Strict On
Option Explicit On 

Imports System.ServiceProcess
Imports System.Configuration.Install
Imports System.ComponentModel
Imports System.Net

<RunInstallerAttribute(True)> _
Public Class MyProjectInstaller
    'Cette classe permet de pouvoir compiler le service afin d'en obtenir un executable binaire
    'Cependant, ce n'est pas suffisant pour installer le service windows. Ceci permet juste d'avoir un .exe
    'Il faudra une etape supplementaire pour installer le service sur votre machine
    Inherits Installer
    Private ReadOnly monServiceInstaller As ServiceInstaller
    Private ReadOnly monProcessInstaller As ServiceProcessInstaller

    'Public WithEvents Connection As cConnection

    Public Sub New()
        ' Instantie les installeurs
        monProcessInstaller = New ServiceProcessInstaller
        monServiceInstaller = New ServiceInstaller

        ' The service sera lancé sous le compte Systeme
        monProcessInstaller.Account = ServiceAccount.LocalSystem

        ' Le service sera demarré manuellement
        monServiceInstaller.StartType = ServiceStartMode.Automatic

        'FrameWork 4.0
        monServiceInstaller.DelayedAutoStart = True

        'Dim NetWordkLocation As String
        If System.Environment.OSVersion.Version.Major > 5 Then
            'FrameWork 4.0
            monServiceInstaller.DelayedAutoStart = True
        Else
            'NetWordkLocation = "Nla"
            Dim DependedOn() As String = {"Netman", "Nla"}
            monServiceInstaller.ServicesDependedOn = DependedOn
        End If

        ' Le nom du service doit etre egal au nom de la classe ServiceBase dont on derive (voir le  Public Sub New() de la classe MonService pour comprendre)
        monServiceInstaller.ServiceName = "Medial Supervisor"
        monServiceInstaller.DisplayName = "Medial Supervisor"

        monServiceInstaller.Description = "Supervision des services Medial 2."

        ' Ajouter les installeurs à la collection (l'ordre n'est pas important)
        Installers.Add(monServiceInstaller)
        Installers.Add(monProcessInstaller)
    End Sub
End Class

Public Class MonService
    Inherits System.ServiceProcess.ServiceBase

    Public Shared AppPath As String = My.Application.Info.DirectoryPath + "\"

    Public Shared svServiceIni As String = AppPath + My.Application.Info.AssemblyName + ".ini"

    Public Shared HostName As String = Dns.GetHostEntry(Dns.GetHostName).HostName

    Public Shared Serial As String

    Public Shared MaConnection As cConnection = New cConnection

    Public Shared TimeOut As Integer

    Public Shared DelayService As Integer

    Public Shared DelayReStartService As Integer

    Public Shared DelayStopService As Integer

    Public Shared DisplayNameService As String = "Medial Supervisor"

    Public Shared svCompanyNameService As String

    Public Shared AppName As String

    Public Shared DelayReConnect As Integer

    Public Shared DelayRefresh As Integer

    Public Shared DisplayNameReStart As String = "ReStart"

    Public Shared AppVersion As String

    Structure Service
        Dim Number As Integer
        Dim NameService As String
        Dim DependOnService As String
        Dim IsStopDeleteFile As String
    End Structure

    Public Shared Cle As String
    Public Shared LocalDir As String
    Public Shared Patch As String
    Public Shared CompanyName As String

    Dim MonDelegate As cDelegate

    Dim Services() As Service

    Public Sub New()
        MyBase.New()

        InitializeComponent()

        'Ici le nom doit etre identique à celui donné plus haut ! 
        ServiceName = MonService.DisplayNameService
    End Sub

    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    <MTAThread()> _
    Shared Sub Main()
        Dim ServicesToRun() As System.ServiceProcess.ServiceBase

        ServicesToRun = New System.ServiceProcess.ServiceBase() {New MonService}

        System.ServiceProcess.ServiceBase.Run(ServicesToRun)
    End Sub

    Private components As System.ComponentModel.IContainer

    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        components = New System.ComponentModel.Container
        Me.ServiceName = DisplayNameService
    End Sub

    Protected Overrides Sub OnStart(ByVal args() As String)
        ' Code lancé au demarrage du service
        'faire une action 

        Try

            If IO.File.Exists(AppPath + My.Application.Info.AssemblyName + ".old") Then
                IO.File.Delete(AppPath + My.Application.Info.AssemblyName + ".old")
            End If

            If IO.File.Exists(AppPath + DisplayNameReStart + ".old") Then
                IO.File.Delete(AppPath + DisplayNameReStart + ".old")
            End If

            CompanyName = Select_GetIniString("Parametres", "CompanyName", MonService.svServiceIni)

            svCompanyNameService = Url_Encode(CompanyName)

            AppName = Select_GetIniString("Parametres", "ServiceName", MonService.svServiceIni)

            InitServer()

            ' Choix de la langue
            Dim svLanguage As String = UCase(Select_GetIniString("Parametres", "Language", MonService.svServiceIni))
            If svLanguage = "EN" Then
                SetCurrentLanguage(0)
            ElseIf svLanguage = "FR" Then
                SetCurrentLanguage(1)
                'ElseIf svLanguage = "IT" Then
                '    SetCurrentLanguage(2)
                'ElseIf svLanguage = "PL" Then
                '    SetCurrentLanguage(3)
            Else 'Default FR
                SetCurrentLanguage(1)
            End If

            Dim svResultDelay As String = Select_GetIniString("Parametres", "TimeOut", MonService.svServiceIni)
            If Int32.TryParse(svResultDelay, TimeOut) Then
                TimeOut = TimeOut
            Else
                TimeOut = 60
            End If

            Cle = Select_GetIniString("Parametres", "Key", MonService.svServiceIni)

            LocalDir = Select_GetIniString("Parametres", "LocalDir", MonService.svServiceIni)

            ' Ajout un chemin au dossier si nécessaire
            If Mid(LocalDir, 2, 1) <> ":" And Mid(LocalDir, 2, 1) <> "\" Then LocalDir = AppPath + LocalDir

            ' Ajout d'un backslash si nécessaire
            LocalDir = StrAddLastSlash(LocalDir)

            If Not IO.Directory.Exists(LocalDir) Then
                IO.Directory.CreateDirectory(LocalDir)

                'IO.File.Delete(LocalDir + "*.exe")

            End If

            Patch = Select_GetIniString("Parametres", "Patch", MonService.svServiceIni)

            'Ajout de la version dans l'ini
            Dim AppBuild As String = Select_GetIniString("Parametres", "AppBuild", MonService.svServiceIni)
            Dim AppVersion As String = Select_GetIniString("Parametres", "AppVersion", MonService.svServiceIni)

            If Cle = Nothing And AppVersion = Nothing And AppVersion = Nothing Then
                Cle = AppendString(Crypter(CompanyName, "sha512"))
                Select_WriteIniString("Parametres", "Key", Cle, MonService.svServiceIni)
            End If

            Dim AppBuildCurrent As String = My.Application.Info.Version.ToString
            If AppBuild <> AppBuildCurrent Then
                AppBuild = AppBuildCurrent
                Select_WriteIniString("Parametres", "AppBuild", AppBuild, MonService.svServiceIni)
            End If

            Dim AppVersionCurrent As String = FileVersionInfo.GetVersionInfo(AppPath + My.Application.Info.AssemblyName + ".exe").FileVersion
            If AppVersion <> AppVersionCurrent Then
                AppVersion = AppVersionCurrent
                Select_WriteIniString("Parametres", "AppVersion", AppVersion, MonService.svServiceIni)
            End If

            If Not InitService() Then Exit Try

            MonDelegate = New CDelegate
            MonDelegate.Main(Services)

        Catch ex As Exception

            OnStop()

        End Try

    End Sub

    Private Function FormatString2Web(ByVal szString As String) As String

        FormatString2Web = Replace(szString, "&", "%26", , , CompareMethod.Text)
        FormatString2Web = Replace(FormatString2Web, " ", "%20", , , CompareMethod.Text)

    End Function

    Private Function InitService() As Boolean

        Dim NumOfServ As Integer = 0
        Dim Service As Integer = 1

        Do
            Dim svNameService As String = Select_GetIniString("Service" + CStr(Service).Trim, "NameService", MonService.svServiceIni).Trim

            If svNameService = Nothing Then Exit Do

            ReDim Preserve Services(NumOfServ)

            Services(NumOfServ).Number = NumOfServ
            Services(NumOfServ).NameService = svNameService
            Services(NumOfServ).DependOnService = Select_GetIniString("Service" + CStr(Service).Trim, "DependOnService", MonService.svServiceIni)
            Services(NumOfServ).IsStopDeleteFile = Select_GetIniString("Service" + CStr(Service).Trim, "IsStopDeleteFile", MonService.svServiceIni)

            NumOfServ += 1

            Service += 1

        Loop

        Try
            InitService = (Services.Length > 0)
        Catch ex As Exception
            InitService = False
        End Try

        If Not InitService Then
            EventLog.WriteEntry(MonService.DisplayNameService, mLanguageAU.GetString(MSG_NO_SERVICE), EventLogEntryType.Warning, 15)
        End If

        Dim svResultDelay As String

        svResultDelay = Select_GetIniString("Parametres", "DelayCheck", MonService.svServiceIni)
        If Int32.TryParse(svResultDelay, DelayRefresh) Then
            DelayRefresh = DelayRefresh
        Else
            DelayRefresh = 60
        End If

        svResultDelay = Select_GetIniString("Parametres", "DelayService", MonService.svServiceIni)
        If Int32.TryParse(svResultDelay, DelayService) Then
            DelayService = DelayService
        Else
            DelayService = 10
        End If

        svResultDelay = Select_GetIniString("Parametres", "DelayReStartService", MonService.svServiceIni)
        If Int32.TryParse(svResultDelay, DelayReStartService) Then
            DelayReStartService = DelayReStartService
        Else
            DelayReStartService = 10
        End If

        svResultDelay = Select_GetIniString("Parametres", "DelayStopService", MonService.svServiceIni)
        If Int32.TryParse(svResultDelay, DelayStopService) Then
            DelayStopService = DelayStopService
        Else
            DelayStopService = 300
        End If

    End Function

    Private Sub InitServer()

        Dim InternetBuffer As String = Select_GetIniString("Parametres", "InternetBuffer", MonService.svServiceIni)
        Dim InternetFlagsMask As String = Select_GetIniString("Parametres", "InternetFlagsMask", MonService.svServiceIni)

        With MonService.MaConnection
            .Proxy.Proxy = Select_GetIniString("Parametres", "Proxy", MonService.svServiceIni)
            .DirectConnect = (Select_GetIniString("Parametres", "DirectConnect", MonService.svServiceIni) = "1")
            .DebugMode = (Select_GetIniString("Parametres", "Debug", MonService.svServiceIni) = "1")
            .CodeAppli = Select_GetIniString("Parametres", "CodeAppli", MonService.svServiceIni)
            .RemoteFileAcces = FormatString2Web(MonService.CompanyName) & "." & AppendString(Crypter(MonService.CompanyName, "sha1"))
            .RemoteDir = Select_GetIniString("Parametres", "RemoteDir", MonService.svServiceIni)
            .ServerName = Select_GetIniString("Parametres", "ServerName", MonService.svServiceIni)

            If .Proxy.Proxy <> vbNullString Then
                .Proxy.InitProxy()
            End If

            If InternetBuffer = Nothing Then
                .InternetBuffer = 8
            Else
                .InternetBuffer = CInt(InternetBuffer)
            End If

            If InternetFlagsMask = Nothing Then
                .InternetFlagsMask = 0
            Else
                .InternetFlagsMask = CInt(InternetFlagsMask)
            End If
        End With

    End Sub

    Protected Overrides Sub OnStop()
        ' Code qui sert à effectuer des operations de "fermeture" avant la fin du service
        If MonDelegate IsNot Nothing Then
            MonDelegate.StopThread()
        End If
    End Sub

End Class
